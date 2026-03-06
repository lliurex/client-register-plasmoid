#ifndef PLASMA_CLIENT_REGISTER_WIDGET_UTILS_H
#define PLASMA_CLIENT_REGISTER_WIDGET_UTILS_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QDir>

#include <n4d.hpp>
#include <variant.hpp>

using namespace std;
using namespace edupals;
using namespace edupals::variant;


class ClientRegisterWidgetUtils : public QObject
{
    Q_OBJECT


public:
   

   ClientRegisterWidgetUtils(QObject *parent = nullptr);

   void startUtils();
   void getWidgetStatus();
   void getCurrentInfo();
   bool isWifiAlu();
   bool isThereConnectionWithADI();

   QString clientRegisterVar="/var/lib/n4d/variables/CONTROLLED_CLASSROOM";

signals:

    void startUtilsFinished(bool startOk);
    void getWidgetStatusFinished (bool isAvailable, bool isError);
    void getCurrentInfoFinished (bool isEnable, bool isError, bool canCreateWatcher, bool isConnectedWithADI, int currentCart);

private:    
     
    QString user;
    n4d::Client client;
    QString natfreeTie="/usr/bin/natfree-tie";
    QString natfreeAdi="/usr/bin/natfree-adi";

    void cleanCache();
    QString getInstalledVersion();
    bool showWidget();
    QVariantList isClientRegisterAvailable();
    QVariantList getCurrentCart();
     
};

#endif // PLASMA_CLASSROOM_CONTROL_WIDGET_UTILS_H
