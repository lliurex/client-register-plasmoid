#ifndef CLIENT_REGISTER_PLUGIN_H
#define CLIENT_REGISTER_PLUGIN_H

#include <QQmlEngine>
#include <QQmlExtensionPlugin>

class ClientRegisterPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
/*    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")*/
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) Q_DECL_OVERRIDE;
};

#endif // CLIENT_REGISTER_PLUGIN_H
