import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.kde.plasma.plasmoid
import org.kde.plasma.core as PlasmaCore
import org.kde.plasma.components as PC3
import org.kde.plasma.extras as PlasmaExtras
import org.kde.kirigami as Kirigami

import org.kde.plasma.private.clientregister 1.0
// Item - the most basic plasmoid component, an empty container.
PlasmoidItem {

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

    switchWidth: Kirigami.Units.gridUnit * 5
    switchHeight: Kirigami.Units.gridUnit * 5

    Plasmoid.icon:clientRegisterWidget.iconName
    toolTipMainText: clientRegisterWidget.toolTip
    toolTipSubText: clientRegisterWidget.subToolTip

    Component.onCompleted: {
       Plasmoid.setInternalAction("configure", configureAction)

    }

   
    fullRepresentation: PC3.Page {
        implicitWidth: Kirigami.Units.gridUnit * 12
        implicitHeight: Kirigami.Units.gridUnit * 6
        
        PlasmaExtras.PlaceholderMessage {
            id:phMsg
            anchors.centerIn: parent
            width: parent.width - (Kirigami.Units.gridUnit * 4)
            iconName: clientRegisterWidget.iconNamePh
            text:clientRegisterWidget.subToolTip
        }
        RowLayout{
            id:btnLayout
            anchors.top:phMsg.bottom
            anchors.horizontalCenter:parent.horizontalCenter
            spacing:15

            PC3.Button {
                id:testBtn
                height:35
                visible:clientRegisterWidget.canEdit?true:false
                enabled:{
                    if ((clientRegisterWidget.canTest) && (!clientRegisterWidget.testInProgress)){
                        true
                    }else{
                        false
                    }
                }
                display:AbstractButton.TextBesideIcon
                icon.name:"view-refresh"
                text:clientRegisterWidget.testInProgress?i18n("Running test..."):i18n("Test connection with ADI")
                onClicked:clientRegisterWidget.launchTest()
            }

            PC3.Button {
                id:editBtn
                height:35
                visible:clientRegisterWidget.canEdit?true:false
                display:AbstractButton.TextBesideIcon
                icon.name:"lliurex-client-register"
                text:i18n("Change cart")
                onClicked:clientRegisterWidget.launchGui()
            }
        } 
    }

    function action_launchGui() {
    
        clientRegisterWidget.launchGui()
    }

    function action_openHelp(){

        clientRegisterWidget.openHelp()

    }

    Plasmoid.contextualActions: [
        PlasmaCore.Action{
            text: i18n("See help")
            icon.name:"help-contents.svg"
            onTriggered:action_openHelp()
        }

    ]

    PlasmaCore.Action {
        id: configureAction
        text: i18n("Change cart")
        icon.name:"lliurex-client-register.svg"
        onTriggered:action_launchGui()
    }


 }  
