#include "ClientRegisterWidget.h"
#include "ClientRegisterWidgetUtils.h"

#include <KLocalizedString>
#include <KFormat>
#include <KNotification>
#include <KRun>
#include <QTimer>
#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <QFileSystemWatcher>
#include <QThread>
#include <QtCore/QStringList>
#include <QJsonObject>
#include <variant.hpp>
#include <json.hpp>
#include <QDebug>
#include <KIO/CommandLauncherJob>

using namespace edupals;
using namespace std;
using namespace edupals::variant;


ClientRegisterWidget::ClientRegisterWidget(QObject *parent)
    : QObject(parent)
    , m_utils(new ClientRegisterWidgetUtils(this))
    , m_applyChanges(new QProcess(this))
   
{
    m_utils->cleanCache();
    notificationTitle=i18n("Client Register");
    TARGET_FILE.setFileName(m_utils->clientRegisterVar);

    setSubToolTip(notificationTitle);
    plasmoidMode();

}  

void ClientRegisterWidget::plasmoidMode(){

    if (m_utils->showWidget()){
        if (TARGET_FILE.exists()){
            createWatcher();
        }
        if (m_utils->isClientRegisterAvailable()){
            updateInfo();
    	}else{
            disableApplet();
    	}
    }else{
        setCanEdit(false);
        changeTryIconState(1);
    }

}

void ClientRegisterWidget::createWatcher(){

    watcher=new QFileSystemWatcher(this);

    if (TARGET_FILE.exists()){
        if (!createFileWatcher){
            createFileWatcher=true;
            connect(watcher,SIGNAL(fileChanged(QString)),this,SLOT(updateInfo()));
            watcher->addPath(m_utils->clientRegisterVar);
        }
    }else{
        createFileWatcher=false;
    }
} 

void ClientRegisterWidget::updateInfo(){

    if (!isWorking){
        qDebug()<<"[CLIENT_REGISTER]: Detecting changed in n4d vars directory";
        isWorking=true;
        bool enable=false;
        bool disable=false;
       
        if (TARGET_FILE.exists()){
            qDebug()<<"[CLIENT_REGISTER]: Updating info...";
            QVariantList ret=m_utils->getCurrentCart();
            initCart=ret[1].toInt();

            if (initCart==0){
                disable=true;
            }else{
                if (initCart>0){
                    enable=true;
                }
                createWatcher();
            }
        }else{
            disable=true;
        }

        if (disable){
            disableApplet();
            isWorking=false;
        }else{
            /*title=i18n("Client Register Enabled");*/
            QString cart=QString::number(initCart);
            QString tmpIcon="client_register_cart_";
            tmpIcon.append(QString("%1").arg(cart));
            setIconName(tmpIcon);
            setIconNamePh("client_cart");
            notificationBody=i18n("Laptop assigned to cart number: ")+cart;
            setSubToolTip(notificationBody); 
            if (showNotification){
                m_notification=KNotification::event(QStringLiteral("Set"),title,notificationBody,tmpIcon,nullptr,KNotification::CloseOnTimeout,QStringLiteral("clientregister"));
            }         
        
            changeTryIconState(0);
            setCanEdit(true);
            showNotification=true;
            isWorking=false;
        }
    }
}

void ClientRegisterWidget::disableApplet(){

    notificationBody=i18n("Client-Register not available in this computer");
   
    setCanEdit(false);
    setIconName("client_register");
    setIconNamePh("client_register");
    setSubToolTip(notificationBody);
    changeTryIconState(1);

}

ClientRegisterWidget::TrayStatus ClientRegisterWidget::status() const
{
    return m_status;
}

void ClientRegisterWidget::changeTryIconState(int state){

    if (state==0){
    	setStatus(ActiveStatus);
        setToolTip(notificationTitle);
    }else{
        setStatus(PassiveStatus);
    }

}

void ClientRegisterWidget::launchGui()
{
    KIO::CommandLauncherJob *job = nullptr;
    QString cmd="lliurex-client-register";
    job = new KIO::CommandLauncherJob(cmd);
    job->start();
}

void ClientRegisterWidget::openHelp(){

    QString command="xdg-open https://wiki.edu.gva.es/lliurex";
    KIO::CommandLauncherJob *job = nullptr;
    job = new KIO::CommandLauncherJob(command);
    job->start();
}

void ClientRegisterWidget::setStatus(ClientRegisterWidget::TrayStatus status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

QString ClientRegisterWidget::iconName() const
{
    return m_iconName;
}

void ClientRegisterWidget::setIconName(const QString &name)
{
    if (m_iconName != name) {
        m_iconName = name;
        emit iconNameChanged();
    }
}

QString ClientRegisterWidget::iconNamePh() const
{
    return m_iconNamePh;
}

void ClientRegisterWidget::setIconNamePh(const QString &name)
{
    if (m_iconNamePh != name) {
        m_iconNamePh = name;
        emit iconNamePhChanged();
    }
}

QString ClientRegisterWidget::toolTip() const
{
    return m_toolTip;
}

void ClientRegisterWidget::setToolTip(const QString &toolTip)
{
    if (m_toolTip != toolTip) {
        m_toolTip = toolTip;
        emit toolTipChanged();
    }
}

QString ClientRegisterWidget::subToolTip() const
{
    return m_subToolTip;
}

void ClientRegisterWidget::setSubToolTip(const QString &subToolTip)
{
    if (m_subToolTip != subToolTip) {
        m_subToolTip = subToolTip;
        emit subToolTipChanged();
    }
}

int ClientRegisterWidget::currentCart(){

    return m_currentCart;
}

void ClientRegisterWidget::setCurrentCart(int newCart){

    if (m_currentCart!=newCart){
        m_currentCart=newCart;
        emit currentCartChanged();
    }
}

bool ClientRegisterWidget::canEdit(){

    return m_canEdit;
}

void ClientRegisterWidget::setCanEdit(bool canEdit){

    if (m_canEdit!=canEdit){
        m_canEdit=canEdit;
        emit canEditChanged();
    }

}

bool ClientRegisterWidget::showWaitMsg(){

    return m_showWaitMsg;
}

void ClientRegisterWidget::setShowWaitMsg(bool showWaitMsg){

    if (m_showWaitMsg!=showWaitMsg){
        m_showWaitMsg=showWaitMsg;
        emit showWaitMsgChanged();
    }
}


