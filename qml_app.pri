#===============================================================================
#
# Common QML Projects Settings
#
#===============================================================================

QT += quick quickcontrols2

include( $${PWD}/common.pri )

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/app_core.cpp \
    $${PWD}/sources/gui_app_core.cpp \
    $${PWD}/sources/log.cpp \
    $${PWD}/sources/convert.cpp \
    $${PWD}/sources/file_sys.cpp \
    $${PWD}/sources/system_utils.cpp

HEADERS *= \
    $${PWD}/include/app_core.h \
    $${PWD}/include/gui_app_core.h \
    $${PWD}/include/log.h \
    $${PWD}/include/images_const.h \
    $${PWD}/include/convert.h \
    $${PWD}/include/file_sys.h \
    $${PWD}/include/system_utils.h

# Resources files ==============================================================
RESOURCES *= \
    $${PWD}/qml/qml_resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH *= \
    $${PWD}/qml/

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH *= \
    $${PWD}/qml/
