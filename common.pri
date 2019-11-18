#===============================================================================
#
# Common Projects Settings
#
#===============================================================================

CONFIG  += c++14

DEFINES += QT_DEPRECATED_WARNINGS

# Includes =====================================================================
INCLUDEPATH *= \
    $${PWD}/include \

# Version and Build date =======================================================
win32: BUILD_DATE = '$(shell echo %DATE:~6,4%-%DATE:~3,2%-%DATE:~0,2%)'
else:  BUILD_DATE = '$(shell date +%Y-%m-%d)'
VERSION  = 1.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_BUILD_DATE=\\\"$$BUILD_DATE\\\"
