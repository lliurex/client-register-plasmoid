import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.1 as PlasmaCore
import org.kde.plasma.components 3.0 as PC3
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.plasma.private.clientregister 1.0
// Item - the most basic plasmoid component, an empty container.
Item {

    id:clientRegisterApplet
    
    ClientRegisterWidget{
        id:clientRegisterWidget

    }


    Plasmoid.status: {
        /* Warn! Enum types are accesed through ClassName not ObjectName */
        switch (clientRegisterWidget.status){
            case ClientRegisterWidget.ActiveStatus:
                return PlasmaCore.Types.ActiveStatus
            case ClientRegisterWidget.PassiveStatus:
                return PlasmaCore.Types.PassiveStatus
           
        }
        return  PlasmaCore.Types.ActiveStatus
        
    }

    Plasmoid.switchWidth: units.gridUnit * 5
    Plasmoid.switchHeight: units.gridUnit * 5

    Plasmoid.icon:clientRegisterWidget.iconName
    Plasmoid.toolTipMainText: clientRegisterWidget.toolTip
    Plasmoid.toolTipSubText: clientRegisterWidget.subToolTip

    Component.onCompleted: {
       plasmoid.removeAction("configure");
       plasmoid.setAction("launchGui",i18n("Change assigned cart"),"lliurex-client-register")
       plasmoid.setAction("openHelp",i18n("See help"),"help-contents")
    }

   
    Plasmoid.preferredRepresentation: Plasmoid.fullRepresentation
    Plasmoid.fullRepresentation: PC3.Page {
        implicitWidth: PlasmaCore.Units.gridUnit * 12
        implicitHeight: PlasmaCore.Units.gridUnit * 6
        
        PlasmaExtras.PlaceholderMessage {
            id:phMsg
            anchors.centerIn: parent
            width: parent.width - (PlasmaCore.Units.gridUnit * 4)
            iconName: Plasmoid.icon
            text:Plasmoid.toolTipSubText
        }

        PC3.Button {
            height:35
            anchors.top:phMsg.bottom
            anchors.horizontalCenter:phMsg.horizontalCenter
            visible:clientRegisterWidget.canEdit?true:false
            display:AbstractButton.TextBesideIcon
            icon.name:"lliurex-client-register"
            text:i18n("Change assigned cart")
 
            onClicked:clientRegisterWidget.launchGui()
        } 
    }

    function action_launchGui() {
    
        clientRegisterWidget.launchGui()
    }

    function action_openHelp(){

        clientRegisterWidget.openHelp()

    }

 }  
