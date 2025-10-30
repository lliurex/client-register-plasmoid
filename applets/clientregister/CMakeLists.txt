
#######################################################################################
# Package
plasma_install_package(package org.kde.plasma.clientregister)


#######################################################################################
# Notes Library
add_definitions(-DTRANSLATION_DOMAIN="plasma_applet_org.kde.plasma.clientregister")

set(clientregisterwidget_SRCS
    plugin/plugin.cpp
    plugin/ClientRegisterWidget.cpp
    plugin/ClientRegisterWidgetUtils.cpp
   
)

add_library(clientregisterplugin SHARED ${clientregisterwidget_SRCS})

target_link_libraries(clientregisterplugin
                      Qt5::Quick
                      Qt5::Core
                      Qt5::Widgets
                      KF5::CoreAddons
                      KF5::I18n
                      KF5::Notifications
                      KF5::KIOWidgets
                      Edupals::Base Edupals::N4D

                      )

install(FILES plugin/qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/clientregister)
install(TARGETS clientregisterplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/clientregister)
install(FILES "icons/client_register.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_1.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_2.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_3.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_4.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_5.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_6.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_7.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_8.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_9.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_10.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_11.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_12.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_13.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_14.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_15.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_error.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_ok.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_1.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_2.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_3.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_4.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_5.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_6.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_7.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_8.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_9.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_10.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_11.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_12.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_13.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_14.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_15.svg"
        DESTINATION ${DATAROOTDIR}/usr/share/icons/hicolor/scalable/apps/)
install(FILES "knotifications5/clientregister.notifyrc"
       DESTINATION ${DATAROOTDIR}/usr/share/knotifications5/)
