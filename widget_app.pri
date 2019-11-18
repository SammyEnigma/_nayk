#===============================================================================
#
# Common Widget Projects Settings
#
#===============================================================================

QT      += core gui widgets

include( $${PWD}/common.pri )

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/app_core.cpp \
    $${PWD}/sources/gui_app_core.cpp \
    $${PWD}/sources/gui_utils.cpp \
    $${PWD}/sources/log.cpp \
    $${PWD}/sources/dialog_log.cpp \
    $${PWD}/sources/convert.cpp \
    $${PWD}/sources/file_sys.cpp \
    $${PWD}/sources/system_utils.cpp

HEADERS *= \
    $${PWD}/include/app_core.h \
    $${PWD}/include/gui_app_core.h \
    $${PWD}/include/gui_utils.h \
    $${PWD}/include/log.h \
    $${PWD}/include/dialog_log.h \
    $${PWD}/include/images_const.h \
    $${PWD}/include/convert.h \
    $${PWD}/include/file_sys.h \
    $${PWD}/include/system_utils.h

# Resources files ==============================================================
RESOURCES *= \
    $${PWD}/resources/common.qrc
