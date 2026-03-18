#ifndef CLIENT_REGISTER_PLUGIN_H
#define CLIENT_REGISTER_PLUGIN_H

#include <QQmlEngineExtensionPlugin>

class ClientRegisterPlugin : public QQmlEngineExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlEngineExtensionInterface_iid)

public:
    using QQmlEngineExtensionPlugin::QQmlEngineExtensionPlugin;
};

#endif // CLIENT_REGISTER_PLUGIN_H
