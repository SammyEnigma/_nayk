#===============================================================================
#
# Common Projects Settings
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

HEADERS *= \
    $${PWD}/include/app_core.h \
    $${PWD}/include/log.h \
    $${PWD}/include/convert.h \
    $${PWD}/include/file_sys.h \
    $${PWD}/include/system_utils.h \

contains(QT, network) {

}

contains(QT, serialport) {

}

contains(QT, sql) {

}

# Version and Build date =======================================================
win32: BUILD_DATE = '$(shell echo %DATE:~6,4%-%DATE:~3,2%-%DATE:~0,2%)'
else:  BUILD_DATE = '$(shell date +%Y-%m-%d)'
!defined(VERSION) {
    VERSION = 1.0
}
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_BUILD_DATE=\\\"$$BUILD_DATE\\\"
