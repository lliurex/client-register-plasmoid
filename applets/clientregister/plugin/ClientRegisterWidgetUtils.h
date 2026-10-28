#ifndef PLASMA_CLIENT_REGISTER_WIDGET_UTILS_H
#define PLASMA_CLIENT_REGISTER_WIDGET_UTILS_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>

#include <QFutureWatcher>
#include <QPair>

#include <n4d.hpp>
#include <variant.hpp>

using namespace std;
using namespace edupals;
using namespace edupals::variant;


class ClientRegisterWidgetUtils : public QObject
{
    Q_OBJECT


public:

    static ClientRegisterWidgetUtils& instance(){
        static ClientRegisterWidgetUtils _instance;
        return _instance;
    }

   ClientRegisterWidgetUtils(const ClientRegisterWidgetUtils&)=delete;
   ClientRegisterWidgetUtils& operator=(const ClientRegisterWidgetUtils&) =delete;

   
   void startWidget();
   void getWidgetStatus();
   void getCurrentInfo();
   
   bool requestConnectionTest(bool isManualCheck);
   bool isWifiAlu();
   bool isThereConnectionWithADI();

   QString clientRegisterVar="/var/lib/n4d/variables/CONTROLLED_CLASSROOM";

signals:

    void startWidgetFinished(bool startOk);
    void getWidgetStatusFinished (bool isAvailable, bool isError);
    void getCurrentInfoFinished (bool isEnable, bool isError, bool canCreateWatcher, bool isConnectedWithADI, int currentCart);
    void connectionTestFinished(bool isWifiAlu, bool connected, bool isManualCheck);

private slots:

    void onConnectionTestFinished();

private:    

    explicit ClientRegisterWidgetUtils();

    bool m_isStarting=false;
    bool m_isInitialized=false;
    bool m_isGettingStatus=false;
    bool m_isGettingInfo=false;
    bool m_isChecking=false;
    bool m_isManualChek=false;
      
    n4d::Client client;
    QMutex clientMutex;
    QString natfreeTie="/usr/bin/natfree-tie";
    QString natfreeAdi="/usr/bin/natfree-adi";

    bool showWidget();
    QVariantList isClientRegisterAvailable();
    QVariantList getCurrentCart();

    QFutureWatcher<QPair<bool,bool>>m_connectionWatcher;
     
};

#endif // PLASMA_CLASSROOM_CONTROL_WIDGET_UTILS_H
