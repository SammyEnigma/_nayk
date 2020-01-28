#===============================================================================
#
# Common QML Projects Settings
#
#===============================================================================

QT *= quick quickcontrols2

include( $${PWD}/common.pri )

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/gui_app_core.cpp \

HEADERS *= \
    $${PWD}/include/gui_app_core.h \
    $${PWD}/include/images_const.h \

# Resources files ==============================================================
RESOURCES *= \
    $${PWD}/qml/qml_resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH *= \
    $${PWD}/qml/

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH *= \
    $${PWD}/qml/
