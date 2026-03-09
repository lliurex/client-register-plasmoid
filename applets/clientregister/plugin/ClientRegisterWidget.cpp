#include "ClientRegisterWidget.h"
#include "ClientRegisterWidgetUtils.h"

#include <KLocalizedString>
#include <KFormat>
#include <KNotification>
#include <KIO/CommandLauncherJob>

#include <QTimer>
#include <QFile>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QThreadPool>
#include <QPointer>

#include <QtConcurrent>

using namespace edupals;
using namespace std;
using namespace edupals::variant;


ClientRegisterWidget::ClientRegisterWidget(QObject *parent)
    : QObject(parent)
    , m_utils(new ClientRegisterWidgetUtils(this))
    , m_timer(new QTimer(this))

   
{
    notificationTitle=i18n("Client Register");
    TARGET_FILE.setFileName(m_utils->clientRegisterVar);
    firstRun=true;
    bool isManualCheck=false;
    
    connect(m_utils,&ClientRegisterWidgetUtils::startWidgetFinished,this,&ClientRegisterWidget::handleStartFinished);
    connect(m_utils,&ClientRegisterWidgetUtils::getWidgetStatusFinished,this,&ClientRegisterWidget::initPlasmoid);
    connect(m_timer, &QTimer::timeout, this, [this, isManualCheck](){
        this->ClientRegisterWidget::testConnection(isManualCheck);
        });

    connect(m_utils,&ClientRegisterWidgetUtils::getCurrentInfoFinished,this,&ClientRegisterWidget::getInfoFinished);
    setSubToolTip(notificationTitle);
    m_utils->startWidget();

}  

void ClientRegisterWidget::handleStartFinished(bool startOk){

    if (startOk){
        m_utils->getWidgetStatus();
    }else{
        notificationBody=i18n("Client Register not be initialized correctly");
        setCanEdit(false);
        setIconName("client_register_error");
        setIconNamePh("client_register_error");
        setSubToolTip(notificationBody);
        changeTryIconState(0);
    }
}

void ClientRegisterWidget::initPlasmoid(bool isAvailable, bool isError){

    if (isAvailable){
        createWatcher();
        if (!isError){
            m_timer->start(defaultTimeOutToCheck);
            getInfo();  
        }else{
            showError();
        }

    }else{
        disableApplet();
    }

}

void ClientRegisterWidget::createWatcher(){

    watcher=new QFileSystemWatcher(this);

    if (TARGET_FILE.exists()){
        if (!createFileWatcher){
            createFileWatcher=true;
            connect(watcher,&QFileSystemWatcher::fileChanged,this,&ClientRegisterWidget::getInfo);
            watcher->addPath(m_utils->clientRegisterVar);
        }
    }else{
        createFileWatcher=false;
    }
} 

void ClientRegisterWidget::getInfo(){

    if (!isWorking){
        qDebug()<<"[CLIENT_REGISTER]: Detecting changed in n4d vars directory";
        isWorking=true;
        m_utils->getCurrentInfo();
    }

} 

void ClientRegisterWidget::getInfoFinished(bool isEnable, bool isError, bool canCreateWatcher, bool isConnectedWithADI, int currentCart){

    showNotification=true;
    if (canCreateWatcher){
        createWatcher();
    }

    if (!isError){
        if (isEnable){
           if (initCart!=currentCart){
                initCart=currentCart;
            }else{
                if (!firstRun){
                    showNotification=false;
                }
            }
            connectedWithServer=isConnectedWithADI;
            if (!updateWidget || firstRun){
                if (firstRun){
                    firstRun=false;
                }
                updateWidgetFeedbak();
            }
            changeTryIconState(0);
            setCanEdit(true);
            setCanTest(true);
            isWorking=false;

        }else{
            m_timer->stop();
            isWorking=false;
            firstRun=true;
            disableApplet();
        }
    }else{
        m_timer->stop();
        showError();
        showNotification=true;
        isWorking=false;
        firstRun=true;
    }      
       
}

void ClientRegisterWidget::disableApplet(){

    notificationBody=i18n("Client Register not available in this computer");
   
    setCanEdit(false);
    setCanTest(false);
    setIconName("client_register");
    setIconNamePh("client_register");
    setSubToolTip(notificationBody);
    changeTryIconState(2);

}

void ClientRegisterWidget::launchGui()
{
    
    if (!m_launchGuiInProgress){
        
        setLaunchGuiInProgress(true);
        QPointer<ClientRegisterWidget>safeThis(this);

        QThreadPool::globalInstance()->start([safeThis]() {

            if (!safeThis){
                return;
            }
            bool isWifi = safeThis->m_utils->isWifiAlu();

            QMetaObject::invokeMethod(safeThis.data(), [safeThis, isWifi]() {
                if (isWifi) {
                    QString cmd = "lliurex-client-register";
                    auto *job = new KIO::CommandLauncherJob(cmd);
                    job->start();
                    safeThis->setLaunchGuiInProgress(false);

                } else {
                    safeThis->m_timer->stop();
                    safeThis->disableApplet();
                }
            }, Qt::QueuedConnection);
        });
    }else{
        return;
    }
}

void ClientRegisterWidget::openHelp()
{

    QString command="xdg-open https://wiki.edu.gva.es/lliurex/tiki-index.php?page=Configuración-de-los-equipos-del-aula-móvil";
    KIO::CommandLauncherJob *job = nullptr;
    job = new KIO::CommandLauncherJob(command);
    job->start();
}

void ClientRegisterWidget::launchTest(){

    if (checkingConnection || manualCheckCount >= maxManualCheck) {
        return;
    }

    setTestInProgress(true);
    testConnection(true); 
    
    manualCheckCount++;
    if (manualCheckCount >= maxManualCheck) {
        setCanTest(false);
    }
}

void ClientRegisterWidget::testConnection(bool isManualCheck)
{

  
    if (checkingConnection){
        return;
    }
    checkingConnection = true;

    QPointer<ClientRegisterWidget>safeThis(this);

    QThreadPool::globalInstance()->start([safeThis, isManualCheck]() {

        if (!safeThis){
            return;
        }

        bool isWifi = safeThis->m_utils->isWifiAlu(); 
        bool connected=false;

        if (isWifi){
            connected = safeThis->m_utils->isThereConnectionWithADI();
        }
    
        QMetaObject::invokeMethod(safeThis.data(), [safeThis, isWifi, connected, isManualCheck]() {
            if (!isWifi){
                safeThis->m_timer->stop();
                safeThis->disableApplet();
            }else{
                if (safeThis->connectedWithServer != connected) {
                    safeThis->connectedWithServer = connected;
                    safeThis->updateWidgetFeedbak();
                }
            }
            safeThis->checkingConnection = false;
            if (!isManualCheck) {
                safeThis->manualCheckCount = 0;
                safeThis->setCanTest(true);
            }
            safeThis->setTestInProgress(false);
            
        },Qt::QueuedConnection);
    });

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
    updateWidget=false;

    if (showNotification){
        m_notification=KNotification::event(QStringLiteral("Update"),notificationBody,notificationServerBody,tmpIcon,nullptr,KNotification::CloseOnTimeout,QStringLiteral("clientregister"));
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
    m_notification=KNotification::event(QStringLiteral("Error"),notificationBody,"",tmpIcon,nullptr,KNotification::CloseOnTimeout,QStringLiteral("clientregister"));

}

ClientRegisterWidget::TrayStatus ClientRegisterWidget::status() const
{
    return m_status;
}

void ClientRegisterWidget::changeTryIconState(int state){

    if (state==0){
    	setStatus(ActiveStatus);
        setToolTip(notificationTitle);
    }else if (state==1){
        setStatus(PassiveStatus);
    }else if (state==2){
        setStatus(HiddenStatus);
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

bool ClientRegisterWidget::launchGuiInProgress(){

    return m_launchGuiInProgress;
}

void ClientRegisterWidget::setLaunchGuiInProgress(bool launchGuiInProgress){

    if (m_launchGuiInProgress!=launchGuiInProgress){
        m_launchGuiInProgress=launchGuiInProgress;
        emit launchGuiInProgressChanged();
    }

}
