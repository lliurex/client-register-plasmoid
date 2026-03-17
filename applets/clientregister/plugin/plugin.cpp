#include "plugin.h"
#include "ClientRegisterWidget.h"
#include <QtQml>

void ClientRegisterPlugin::registerTypes (const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.private.clientregister"));
    qmlRegisterType<ClientRegisterWidget>(uri, 2, 0, "ClientRegisterWidget");
}
