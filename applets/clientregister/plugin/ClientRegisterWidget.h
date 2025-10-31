#ifndef PLASMA_CLIENT_REGISTER_WIDGET_H
#define PLASMA_CLIENT_REGISTER_WIDGET_H

#include <QObject>
#include <QProcess>
#include <QPointer>
#include <KNotification>
#include <QDir>
#include <QFile>
#include <QThread>
#include <QFileSystemWatcher>
#include <KIO/CommandLauncherJob>

#include <variant.hpp>

#include "ClientRegisterWidgetUtils.h"
using namespace edupals;
using namespace edupals::variant;

class QTimer;
class KNotification;
class AsyncDbus;


class ClientRegisterWidget : public QObject
{
    Q_OBJECT


    Q_PROPERTY(TrayStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString toolTip READ toolTip NOTIFY toolTipChanged)
    Q_PROPERTY(QString subToolTip READ subToolTip NOTIFY subToolTipChanged)
    Q_PROPERTY(QString iconName READ iconName NOTIFY iconNameChanged)
    Q_PROPERTY(QString iconNamePh READ iconNamePh NOTIFY iconNamePhChanged)
    Q_PROPERTY(int currentCart READ currentCart NOTIFY currentCartChanged)
    Q_PROPERTY(bool canEdit READ canEdit NOTIFY canEditChanged)
    Q_PROPERTY(bool canTest READ canTest NOTIFY canTestChanged)
    Q_PROPERTY(bool testInProgress READ testInProgress NOTIFY testInProgressChanged)
    Q_ENUMS(TrayStatus)

public:
    /**
     * System tray icon states.
     */
    enum TrayStatus {
        ActiveStatus=0,
        PassiveStatus
    };

    ClientRegisterWidget(QObject *parent = nullptr);

    TrayStatus status() const;
    void changeTryIconState (int state);
    void setStatus(TrayStatus status);

    QString toolTip() const;
    void setToolTip(const QString &toolTip);

    QString subToolTip() const;
    void setSubToolTip(const QString &subToolTip);

    QString iconName() const;
    void setIconName(const QString &name);

    QString iconNamePh() const;
    void setIconNamePh(const QString &name);

    int currentCart();
    void setCurrentCart(int);

    bool canEdit();
    void setCanEdit(bool);

    bool canTest();
    void setCanTest(bool);

    bool testInProgress();
    void setTestInProgress(bool);

public slots:
    
    void updateInfo();
    void launchGui();
    void openHelp();
    void launchTest();
  
signals:
   
    void toolTipChanged();
    void subToolTipChanged();
    void iconNameChanged();
    void iconNamePhChanged();
    void statusChanged();
    void currentCartChanged();
    void canEditChanged();
    void canTestChanged();
    void testInProgressChanged();

private:

    QTimer *m_timer = nullptr;
    TrayStatus m_status = PassiveStatus;
    QString m_iconName = QStringLiteral("client_register");
    QString m_iconNamePh = QStringLiteral("client_register");
    QString m_toolTip;
    QString m_subToolTip;
    int m_currentCart=0;
    bool isWorking=false;
    bool m_canEdit=false;
    bool m_canTest=false;
    bool m_testInProgress=false;
    bool checkingConnection=false;
    bool connectedWithServer=false;
    int initCart=1;
    QString notificationTitle;
    QString notificationBody;
    QString notificationServerBody;
    QString tmpIcon;
    bool firstRun=true;
    bool updateWidget=true;
    QFile TARGET_FILE;
    QFileSystemWatcher *watcher=nullptr;
    ClientRegisterWidgetUtils* m_utils;
    bool createFileWatcher=false;
    bool showNotification=true; 
    QPointer<KNotification> m_notification;
    void plasmoidMode();
    void createWatcher();
    void disableApplet();
    void showError();
    void updateWidgetFeedbak();
    void testConnection();

};


#endif // PLASMA_CLIENT_REGISTER_WIDGET_H
