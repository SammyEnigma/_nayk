#===============================================================================
#
# Common Widget Projects Settings
#
#===============================================================================

QT *= core gui widgets

include( $${PWD}/common.pri )

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/gui_app_core.cpp \
    $${PWD}/sources/gui_utils.cpp \
    $${PWD}/sources/dialog_log.cpp \
    $${PWD}/sources/ex_controls.cpp \

HEADERS *= \
    $${PWD}/include/gui_app_core.h \
    $${PWD}/include/gui_utils.h \
    $${PWD}/include/dialog_log.h \
    $${PWD}/include/images_const.h \
    $${PWD}/include/ex_controls.h \

# Resources files ==============================================================
RESOURCES *= \
    $${PWD}/resources/icons.qrc \
    $${PWD}/resources/led_images.qrc
