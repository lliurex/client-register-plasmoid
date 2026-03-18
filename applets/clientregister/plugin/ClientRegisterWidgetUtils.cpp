#include "ClientRegisterWidgetUtils.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QThreadPool>
#include <QPointer>
#include <QMutexLocker>

#include <n4d.hpp>
#include <variant.hpp>
#include <json.hpp>

using namespace edupals;
using namespace edupals::variant;


ClientRegisterWidgetUtils::ClientRegisterWidgetUtils(QObject *parent)
    : QObject(parent)
       
{
  
}

void ClientRegisterWidgetUtils::startWidget(){

    QPointer<ClientRegisterWidgetUtils>safeThis(this);

    QThreadPool::globalInstance()->start([safeThis]() {

        if (!safeThis){
            return;
        }

        bool startOk=false;

        try{
            QMutexLocker locker(&safeThis->clientMutex); 
            safeThis->client=n4d::Client("https://127.0.0.1:9779");
            startOk=true;
        }catch (std::exception& e){
            qDebug()<<"[CLIENT_REGISTER]: Error creating n4d client: " <<e.what();
        } 

        if (safeThis){
            emit safeThis->startWidgetFinished(startOk);
        }

    });
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
    QString tmpClientVar=this->clientRegisterVar;

    QThreadPool::globalInstance()->start([safeThis,tmpClientVar]() {

        if (!safeThis){
            return;
        }
        qDebug()<<"[CLIENT_REGISTER]: Getting current info";

        bool isEnable=false;
        bool isError=false;
        bool canCreateWatcher=false;
        bool isConnectedWithADI=false;
        int currentCart=0;

        if (safeThis->isWifiAlu() && QFile::exists(tmpClientVar)){
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
    QMutexLocker locker(&clientMutex); 

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
    QMutexLocker locker(&clientMutex); 
    
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
