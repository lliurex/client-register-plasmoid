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

   void cleanCache();
   QVariantList getCurrentCart();
   bool showWidget();
   QVariantList isClientRegisterAvailable();
   bool isThereConnectionWithADI();

   QString user;
   QString clientRegisterVar="/var/lib/n4d/variables/CONTROLLED_CLASSROOM";
   QString natfreeTie="/usr/bin/natfree-tie";
   QString natfreeAdi="/usr/bin/natfree-adi";

private:    
     
    n4d::Client client;
    QFile TARGET_FILE;
    QString getInstalledVersion();
     
};



#endif // PLASMA_CLASSROOM_CONTROL_WIDGET_UTILS_H
