#include "ClientRegisterWidgetUtils.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QThreadPool>
#include <QPointer>

#include <n4d.hpp>
#include <variant.hpp>
#include <json.hpp>

using namespace edupals;
using namespace edupals::variant;


ClientRegisterWidgetUtils::ClientRegisterWidgetUtils(QObject *parent)
    : QObject(parent)
       
{
    user=qgetenv("USER");
  
}

void ClientRegisterWidgetUtils::startUtils(){

    QPointer<ClientRegisterWidgetUtils>safeThis(this);

    QThreadPool::globalInstance()->start([safeThis]() {

        if (!safeThis){
            return;
        }

        bool startOk=false;

        try{
            safeThis->cleanCache();
            safeThis->client=n4d::Client("https://127.0.0.1:9779");
            startOk=true;
        }catch (std::exception& e){
            qDebug()<<"[CLIENT_REGISTER]: Error creating n4d client: " <<e.what();
        } 

        if (safeThis){
            emit safeThis->startUtilsFinished(startOk);
        }

    });
}

void ClientRegisterWidgetUtils::cleanCache(){

    qDebug()<<"[CLIENT_REGISTER]: Clean cache";
    
    QFile CURRENT_VERSION_TOKEN;
    QDir cacheDir("/home/"+user+"/.cache/plasmashell/qmlcache");
    QString currentVersion="";
    bool clear=false;

    CURRENT_VERSION_TOKEN.setFileName("/home/"+user+"/.config/client-register-widget.conf");
    QString installedVersion=getInstalledVersion();

    if (!CURRENT_VERSION_TOKEN.exists()){
        if (CURRENT_VERSION_TOKEN.open(QIODevice::WriteOnly)){
            QTextStream data(&CURRENT_VERSION_TOKEN);
            data<<installedVersion;
            CURRENT_VERSION_TOKEN.close();
            clear=true;
        }
    }else{
        if (CURRENT_VERSION_TOKEN.open(QIODevice::ReadOnly)){
            QTextStream content(&CURRENT_VERSION_TOKEN);
            currentVersion=content.readLine();
            CURRENT_VERSION_TOKEN.close();
        }

        if (currentVersion!=installedVersion){
            if (CURRENT_VERSION_TOKEN.open(QIODevice::WriteOnly)){
                QTextStream data(&CURRENT_VERSION_TOKEN);
                data<<installedVersion;
                CURRENT_VERSION_TOKEN.close();
                clear=true;
            }
        }
    } 
    if (clear){
        if (cacheDir.exists()){
            cacheDir.removeRecursively();
        }
    }   

}

QString ClientRegisterWidgetUtils::getInstalledVersion(){

    QFile INSTALLED_VERSION_TOKEN;
    QString installedVersion="";
    
    INSTALLED_VERSION_TOKEN.setFileName("/var/lib/client-register-plasmoid/version");

    if (INSTALLED_VERSION_TOKEN.exists()){
        if (INSTALLED_VERSION_TOKEN.open(QIODevice::ReadOnly)){
            QTextStream content(&INSTALLED_VERSION_TOKEN);
            installedVersion=content.readLine();
            INSTALLED_VERSION_TOKEN.close();
        }
    }
    return installedVersion;

}

void ClientRegisterWidgetUtils::getWidgetStatus(){

    QPointer<ClientRegisterWidgetUtils>safeThis(this);

    QThreadPool::globalInstance()->start([safeThis]() {

        if (!safeThis){
            return;
        }

        bool isAvailable=false;
        bool isError=false;
        QVariantList result;

        if (safeThis->showWidget()){
            result=safeThis->isClientRegisterAvailable();
            isAvailable=result[0].toBool();
            isError=result[1].toBool();
        }
        if (safeThis){
            emit safeThis->getWidgetStatusFinished(isAvailable, isError);
        }
    });

}  

bool ClientRegisterWidgetUtils::showWidget(){

    return true; 
}   

QVariantList ClientRegisterWidgetUtils::isClientRegisterAvailable(){

    bool isAvailable=false;
    bool isError=false;
    QVariantList result;


    if (!QFile::exists(natfreeAdi)){
        if (isWifiAlu()){
            if (QFile::exists(natfreeTie)){
                if (QFile::exists(clientRegisterVar)){
                    QVariantList ret=getCurrentCart();
                    if (!ret[0].toBool()){
                        if (ret[1].toInt()==0){
                            isAvailable=false;
                        }else{
                            if (ret[1].toInt()>0){
                                isAvailable=true;
                            }else{
                                if (ret[1].toInt()==-1){
                                    isAvailable=true;
                                }
                            }
                        }
                    }else{
                        isAvailable=true;
                        isError=true;
                    }
                }
            }
        }
    }
    result.append(isAvailable);
    result.append(isError);

    qDebug()<<"[CLIENT_REGISTER]: Client Register Available: "<<isAvailable;
    return result;

}

void ClientRegisterWidgetUtils::getCurrentInfo(){

    QPointer<ClientRegisterWidgetUtils>safeThis(this);

    QThreadPool::globalInstance()->start([safeThis]() {

        if (!safeThis){
            return;
        }
        qDebug()<<"[CLIENT_REGISTER]: Getting current info";

        bool isEnable=false;
        bool isError=false;
        bool canCreateWatcher=false;
        bool isConnectedWithADI=false;
        int currentCart=0;

        if (safeThis->isWifiAlu() && QFile::exists(safeThis->clientRegisterVar)){
            QVariantList ret=safeThis->getCurrentCart();
            currentCart=ret[1].toInt();
            if (!ret[0].toBool()){
                if (currentCart>0 && currentCart<15){
                    isEnable=true;
                    canCreateWatcher=true;
                    isConnectedWithADI=safeThis->isThereConnectionWithADI();
                }else{
                    if (currentCart<-1 || currentCart>14){
                        canCreateWatcher=true;
                    }
                }
            }else{
                isError=true;
                canCreateWatcher=true;
            }
        }
        if (safeThis){
            emit safeThis->getCurrentInfoFinished(isEnable, isError, canCreateWatcher, isConnectedWithADI, currentCart);
        }
    });
}

QVariantList ClientRegisterWidgetUtils::getCurrentCart(){

    bool isError=false;
    int numCart=0;
    QVariantList result;
    Variant cartInfo;

    try{
        cartInfo = client.call("ClientRegisterManager","get_current_cart");
        auto tmpCart=cartInfo["return"];
        
        if (tmpCart.size()>0){
            numCart=cartInfo["return"];
            qDebug()<<"[CLIENT_REGISTER]: Reading CONTROLLED_CLASSROOM var: "<<QString::number(numCart);
        }else{
            numCart=-1;
            qDebug()<<"[CLIENT_REGISTER]: Reading CONTROLLED_CLASSROOM var: ''";

        }
    }catch (std::exception& e){
        qDebug()<<"[CLIENT_REGISTER]: Error reading CONTROLLED_CLASSROOM var: " <<e.what();
        isError=true;
    }

   result.append(isError);
   result.append(numCart);
   return result;

}

bool ClientRegisterWidgetUtils::isThereConnectionWithADI()
{
    bool isConnected=false;

    try{
        Variant ret=client.call("NatfreeTIE","check_server");
        isConnected=ret;
        qDebug()<<"[CLIENT_REGISTER]: Testing connection with ADI. Result: "<<isConnected;
        return isConnected;
    }catch(std::exception& e){
        qDebug()<<"[CLIENT_REGISTER]: Testing connection with ADI. Error: "<<e.what();
        return isConnected;
    }
 
}

bool ClientRegisterWidgetUtils::isWifiAlu(){

    bool matchWifi=false;
    QProcess process;
    QStringList flavours;
    QString cmd="lliurex-version -v";
    process.start("/bin/sh",QStringList()<<"-c"<<cmd);
    process.waitForFinished(-1);
    QString stdout=process.readAllStandardOutput();
    QString stderr=process.readAllStandardError();
    flavours=stdout.split('\n');


    for (int i=0;i<flavours.count();i++){
        if (flavours[i].contains("wifi")){
            matchWifi=true;
            break;
        }
    }

    return matchWifi;

}
