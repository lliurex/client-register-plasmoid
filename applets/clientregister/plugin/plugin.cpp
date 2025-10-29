#include "plugin.h"
#include "ClientRegisterWidget.h"
#include "ClientRegisterWidgetUtils.h"

#include <QtQml>

void ClientRegisterPlugin::registerTypes (const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.private.clientregister"));
    qmlRegisterType<ClientRegisterWidget>(uri, 1, 0, "ClientRegisterWidget");
    qmlRegisterType<ClientRegisterWidgetUtils>(uri, 1, 0, "ClientRegisterWidgetUtils");
}
