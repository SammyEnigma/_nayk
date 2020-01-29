#===============================================================================
#
# Projects Settings - nayk library
#
#===============================================================================

CONFIG  += c++14

DEFINES += QT_DEPRECATED_WARNINGS

# Includes =====================================================================

INCLUDEPATH *= \
    $${PWD}/include

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/app_core.cpp \
    $${PWD}/sources/log.cpp \
    $${PWD}/sources/convert.cpp \
    $${PWD}/sources/file_sys.cpp \
    $${PWD}/sources/system_utils.cpp \
    $${PWD}/sources/geo.cpp \
    $${PWD}/sources/http_server.cpp

HEADERS *= \
    $${PWD}/include/app_core.h \
    $${PWD}/include/log.h \
    $${PWD}/include/convert.h \
    $${PWD}/include/file_sys.h \
    $${PWD}/include/system_utils.h \
    $${PWD}/include/geo.h \
    $${PWD}/include/http_const.h \
    $${PWD}/include/http_server.h

contains(QT, quick) | contains(QT, widgets) {

    SOURCES *= \
        $${PWD}/sources/gui_app_core.cpp

    HEADERS *= \
        $${PWD}/include/gui_app_core.h \
        $${PWD}/include/images_const.h
}

contains(QT, widgets) {

    SOURCES *= \
        $${PWD}/sources/gui_utils.cpp \
        $${PWD}/sources/dialog_log.cpp \
        $${PWD}/sources/ex_controls.cpp \
        $${PWD}/sources/graph.cpp \
        $${PWD}/sources/highlighter.cpp

    HEADERS *= \
        $${PWD}/include/gui_utils.h \
        $${PWD}/include/dialog_log.h \
        $${PWD}/include/ex_controls.h \
        $${PWD}/include/graph.h \
        $${PWD}/include/highlighter.h
}

contains(QT, quick) {

} else {

    SOURCES *= \
        $${PWD}/sources/console.cpp

    HEADERS *= \
        $${PWD}/include/console.h
}

contains(QT, network) {

    SOURCES *= \
        $${PWD}/sources/network_client.cpp

    HEADERS *= \
        $${PWD}/include/network_client.h
}

contains(QT, serialport) {

    SOURCES *= \
        $${PWD}/sources/com_port.cpp

    HEADERS *= \
        $${PWD}/include/com_port.h
}

contains(QT, sql) {

    SOURCES *= \
        $${PWD}/sources/database_client.cpp

    HEADERS *= \
        $${PWD}/include/database_client.h
}

contains(CONFIG, hardware) {

    SOURCES *= \
        $${PWD}/sources/hardware_utils.cpp

    HEADERS *= \
        $${PWD}/include/hardware_utils.h

    win32:LIBS += -lKernel32 -lPsapi
}

# Resources files ==============================================================

contains(QT, widgets) {

    RESOURCES *= \
        $${PWD}/resources/icons.qrc \
        $${PWD}/resources/led_images.qrc
}

contains(QT, quick) {
    RESOURCES *= \
        $${PWD}/qml/qml_resources.qrc

    # Additional import path used to resolve QML modules in Qt Creator's code model
    QML_IMPORT_PATH *= \
        $${PWD}/qml/

    # Additional import path used to resolve QML modules just for Qt Quick Designer
    QML_DESIGNER_IMPORT_PATH *= \
        $${PWD}/qml/
}

# Version and Build date =======================================================

win32: BUILD_DATE = '$(shell echo %DATE:~6,4%-%DATE:~3,2%-%DATE:~0,2%)'
else:  BUILD_DATE = '$(shell date +%Y-%m-%d)'
!defined(VERSION) {
    VERSION = 1.0
}
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_BUILD_DATE=\\\"$$BUILD_DATE\\\"
