#===============================================================================
#
# Common settings for lib projects
#
#===============================================================================

CONFIG  += c++14

TEMPLATE = lib

#CONFIG += staticlib

DEFINES += LIBRARY_FEATURES_ENABLE

DEFINES += QT_DEPRECATED_WARNINGS

lessThan( QT_MAJOR_VERSION, 6 ):lessThan( QT_MINOR_VERSION, 14 ) {
    QMAKE_CXXFLAGS += -Wno-deprecated-copy
}

# Version and Build date =======================================================

VERSION = 1.0

win32: BUILD_DATE = '$(shell echo %DATE:~6,4%-%DATE:~3,2%-%DATE:~0,2%)'
else:  BUILD_DATE = '$(shell date +%Y-%m-%d)'

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_BUILD_DATE=\\\"$$BUILD_DATE\\\"

# Includes =====================================================================

INCLUDEPATH *= \
    $${PWD}/../include

# Output dir ===================================================================

DESTDIR = $${PWD}/../_distrib/$${QMAKE_HOST.os}-$${QMAKE_HOST.arch}/bin

# translations dir =============================================================

TRANSLATIONS_DIR = $$absolute_path( $${DESTDIR}/../translations )

!exists( $${TRANSLATIONS_DIR} ) {

    translations_dir.commands = $${QMAKE_MKDIR} $$shell_path( $${TRANSLATIONS_DIR} )

    QMAKE_EXTRA_TARGETS += \
        translations_dir

    PRE_TARGETDEPS += \
        translations_dir
}
