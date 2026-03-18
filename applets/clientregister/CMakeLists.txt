
#######################################################################################
# Package
plasma_install_package(package org.kde.plasma.clientregister)


#######################################################################################
# Notes Library
set(PLUGIN_TARGET "clientregisterplugin")
add_definitions(-DTRANSLATION_DOMAIN="plasma_applet_org.kde.plasma.clientregister")
ecm_add_qml_module(${PLUGIN_TARGET} URI org.kde.plasma.private.clientregister VERSION 2.0)

target_sources(${PLUGIN_TARGET} PRIVATE
    plugin/plugin.cpp
    plugin/ClientRegisterWidget.cpp
    plugin/ClientRegisterWidgetUtils.cpp
    plugin/plugin.h
    plugin/ClientRegisterWidget.h
    plugin/ClientRegisterWidgetUtils.h
   
)

target_link_libraries(${PLUGIN_TARGET} PRIVATE
                      Qt6::Quick
                      Qt6::Core
                      Qt6::Widgets
                      KF6::CoreAddons
                      KF6::I18n
                      KF6::Notifications
                      KF6::KIOWidgets
                      Edupals::Base Edupals::N4D

                      )

target_include_directories(${PLUGIN_TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/plugin)  
ecm_finalize_qml_module(${PLUGIN_TARGET})

install(TARGETS ${PLUGIN_TARGET} DESTINATION ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/plasma/private/clientregister)
install(FILES "icons/client_register.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_1.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_2.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_3.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_4.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_5.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_6.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_7.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_8.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_9.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_10.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_11.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_12.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_13.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_14.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_cart_15.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_error.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_ok.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_1.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_2.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_3.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_4.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_5.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_6.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_7.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_8.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_9.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_10.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_11.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_12.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_13.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_14.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "icons/client_register_warning_cart_15.svg"
        DESTINATION ${KDE_INSTALL_ICONDIR}/hicolor/scalable/apps/)
install(FILES "knotifications6/clientregister.notifyrc"
        DESTINATION ${DATAROOTDIR}/usr/bin/)
