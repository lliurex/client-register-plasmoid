#include "ClientRegisterWidget.h"
#include "ClientRegisterWidgetUtils.h"

#include <KLocalizedString>
#include <KFormat>
#include <KNotification>
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
#include <QtConcurrent/QtConcurrent>
#include <QFuture>


using namespace edupals;
using namespace std;
using namespace edupals::variant;


ClientRegisterWidget::ClientRegisterWidget(QObject *parent)
    : QObject(parent)
    , m_utils(new ClientRegisterWidgetUtils(this))
    , m_timer(new QTimer(this))

   
{
    m_utils->cleanCache();
    notificationTitle=i18n("Client Register");
    TARGET_FILE.setFileName(m_utils->clientRegisterVar);
    firstRun=true;
    connect(m_timer, &QTimer::timeout, this, &ClientRegisterWidget::testConnection);

    setSubToolTip(notificationTitle);
    plasmoidMode();

}  

void ClientRegisterWidget::plasmoidMode(){

    if (m_utils->showWidget()){
        if (TARGET_FILE.exists()){
            createWatcher();
        }
        QVariantList ret=m_utils->isClientRegisterAvailable();
        if (ret[0].toBool()){
            if (!ret[1].toBool()){
                m_timer->start(300000);
                updateInfo();
            }else{
                showError();
            }
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
        bool disable=false;
        bool canCreateWatcher=false;
        bool error=false;
        showNotification=true;
        int tmpCart=0;
       
        if (TARGET_FILE.exists()){
            qDebug()<<"[CLIENT_REGISTER]: Updating info...";
            QVariantList ret=m_utils->getCurrentCart();
            tmpCart=ret[1].toInt();
            if (!ret[0].toBool()){
                if (tmpCart==0){
                    disable=true;
                }else{
                    if (tmpCart>14){
                        disable=true;
                    }else{
                        if (tmpCart<-1){
                            disable=true;
                        }
                    }
                    canCreateWatcher=true;
                }
            }else{
                canCreateWatcher=true;
                error=true;
            }
            if (canCreateWatcher){
                createWatcher();
            }
        }else{
            disable=true;
        }

        if (!error){
            if (disable){
                disableApplet();
                m_timer->stop();
                isWorking=false;
                firstRun=true;
            }else{
                if (initCart!=tmpCart){
                    initCart=tmpCart;
                }else{
                    showNotification=false;
                }
                testConnection();
                if (!updateWidget){
                    updateWidgetFeedbak();
                }
                changeTryIconState(0);
                setCanEdit(true);
                setCanTest(true);
                isWorking=false;
            }
        }else{
            m_timer->stop();
            showError();
            showNotification=true;
            isWorking=false;
            firstRun=true;
        }
    }
}

void ClientRegisterWidget::disableApplet(){

    notificationBody=i18n("Client Register not available in this computer");
   
    setCanEdit(false);
    setCanTest(false);
    setIconName("client_register");
    setIconNamePh("client_register");
    setSubToolTip(notificationBody);
    changeTryIconState(1);

}

void ClientRegisterWidget::launchGui()
{
    KIO::CommandLauncherJob *job = nullptr;
    QString cmd="lliurex-client-register";
    job = new KIO::CommandLauncherJob(cmd);
    job->start();
}

void ClientRegisterWidget::openHelp()
{

    QString command="xdg-open https://wiki.edu.gva.es/lliurex/tiki-index.php?page=Configuración-de-los-equipos-del-aula-móvil";
    KIO::CommandLauncherJob *job = nullptr;
    job = new KIO::CommandLauncherJob(command);
    job->start();
}

void ClientRegisterWidget::launchTest(){

    if (!checkingConnection){
        setTestInProgress(true);
        QFuture<void> future=QtConcurrent::run([this](){
            this->ClientRegisterWidget::testConnection();
        });
    }
}

void ClientRegisterWidget::testConnection()
{

    if (!checkingConnection){
        checkingConnection=true;
        bool ret=m_utils->isThereConnectionWithADI();

        if (connectedWithServer != ret ){
            connectedWithServer=ret;
            updateWidget=true;
        }else{
            if (firstRun){
                showNotification=true;
                updateWidget=true;
            }
        }

        if (updateWidget){
            firstRun=false;
            updateWidgetFeedbak();
         }

         checkingConnection=false;
         updateWidget=false;
         setTestInProgress(false);
    }
}

void ClientRegisterWidget::updateWidgetFeedbak()
{
    QString cart=QString::number(initCart);
    if (initCart>-1){
        notificationBody=i18n("Laptop assigned to cart: ")+cart;
    }else{
        notificationBody=i18n("Laptop NO assigned to cart");
        tmpIcon="client_register_error";
        setIconNamePh("client_register_error");
    }

    if (connectedWithServer){
        if (initCart>0){
            tmpIcon="client_register_cart_";
            tmpIcon.append(QString("%1").arg(cart));
            setIconNamePh("client_register_ok"); 
        }
        notificationServerBody=i18n("Connected with ADI");
        setSubToolTip(notificationBody+"\n"+notificationServerBody);
       
  
    }else{
        if (initCart>0){
            tmpIcon="client_register_warning_cart_";
            tmpIcon.append(QString("%1").arg(cart));
            setIconNamePh("client_register_warning");
        }
        notificationServerBody=i18n("No connection to the ADI");
        
    }
    setIconName(tmpIcon);
    setSubToolTip(notificationBody+"\n"+notificationServerBody); 
    
    if (showNotification){
        m_notification = new KNotification(QStringLiteral("Update"),KNotification::CloseOnTimeout,this);
        m_notification->setComponentName(QStringLiteral("clientregister"));
        m_notification->setTitle(notificationBody);
        m_notification->setText(notificationServerBody);
        m_notification->setIconName(tmpIcon);
        m_notification->sendEvent();
 
    }
}

void ClientRegisterWidget::showError(){

    notificationBody=i18n("Unable to get cart assigned to laptop");
    tmpIcon="client_register_error";
    setCanEdit(true);
    setCanTest(false);
    setIconName(tmpIcon);
    setIconNamePh(tmpIcon);
    setSubToolTip(notificationBody);
    changeTryIconState(0);
    m_notification = new KNotification(QStringLiteral("Error"),KNotification::CloseOnTimeout,this);
    m_notification->setComponentName(QStringLiteral("clientregister"));
    m_notification->setTitle(notificationBody);
    m_notification->setIconName(tmpIcon);
    m_notification->sendEvent();
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

bool ClientRegisterWidget::canTest(){

    return m_canTest;
}

void ClientRegisterWidget::setCanTest(bool canTest){

    if (m_canTest!=canTest){
        m_canTest=canTest;
        emit canTestChanged();
    }

}

bool ClientRegisterWidget::testInProgress(){

    return m_testInProgress;
}

void ClientRegisterWidget::setTestInProgress(bool testInProgress){

    if (m_testInProgress!=testInProgress){
        m_testInProgress=testInProgress;
        emit testInProgressChanged();
    }

}
