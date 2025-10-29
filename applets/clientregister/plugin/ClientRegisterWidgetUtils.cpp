#include "ClientRegisterWidgetUtils.h"

#include <QFile>
#include <QDateTime>
#include <QFileInfo>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QDebug>
#include <QTextStream>
#include <QJsonObject>
#include <QList>
#include <KLocalizedString>
#include <sys/types.h>

#include <grp.h>
#include <pwd.h>
#include <n4d.hpp>
#include <variant.hpp>
#include <json.hpp>

#include <tuple>
#include <sys/types.h>
#include <QDebug>

using namespace edupals;
using namespace std;
using namespace edupals::variant;


ClientRegisterWidgetUtils::ClientRegisterWidgetUtils(QObject *parent)
    : QObject(parent)
       
{
    user=qgetenv("USER");
    n4d::Client client;
    client=n4d::Client("https://127.0.0.1:9779");
  
}

void ClientRegisterWidgetUtils::cleanCache(){

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

bool ClientRegisterWidgetUtils::showWidget(){

    return true; 
}   

bool ClientRegisterWidgetUtils::isClientRegisterAvailable(){

    TARGET_FILE.setFileName("/usr/bin/natfree-tie");
    bool isAvailable=false;

    if (TARGET_FILE.exists()){
        if (!getHideAppletValue()){
            TARGET_FILE.setFileName(clientRegisterVar);
            if (TARGET_FILE.exists()){
                QVariantList ret=getCurrentCart();
                if (!ret[0].toBool()){
                    if (ret[1].toInt()==0){
                        isAvailable=false;
                    }else{
                        isAvailable=true;
                    }
                }
            }
        }
    }
    
    qDebug()<<"[CLIENT_REGISTER]: Client Register Available: "<<isAvailable;
    return isAvailable;

}

QVariantList ClientRegisterWidgetUtils::getCurrentCart(){

    bool isError=false;
    int numCart=0;
    QVariantList result;

    try{
        variant::Variant cartInfo = client.get_variable("CONTROLLED_CLASSROOM",true);
        auto tmpCart=cartInfo["value"];
        
        if (tmpCart.size()>0){
            numCart=cartInfo["value"];
            qDebug()<<"[CLIENT_REGISTER]: Reading CONTROLLED_CLASSROOM var: "<<QString::number(numCart);
        }else{
            numCart=0;
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

bool ClientRegisterWidgetUtils::getHideAppletValue(){

    bool hideApplet=false;

    TARGET_FILE.setFileName(clientRegisterVar);

    if (TARGET_FILE.exists()){
        try{
            variant::Variant appletInfo = client.get_variable("CONTROLLED_CLASSROOM",true);
            int currentValue=appletInfo["value"];
            if (currentValue==0){
                hideApplet=true;
            }
            qDebug()<<"[CLIENT_REGISTER]: Reading CONTROLLED_CLASSROOM var: "<<QString::number(currentValue);
        }catch (std::exception& e){
            qDebug()<<"[CLIENT_REGISTER]: Error reading CONTROLLED_CLASSROOM var: " <<e.what();
            hideApplet=false;
        } 
   }else{
        hideApplet=true;
   }
   
   return hideApplet;


}
