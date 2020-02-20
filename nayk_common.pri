#===============================================================================
#
# nayk library - Common Projects Settings - include first!!!
#
#===============================================================================

!contains(QT, core) {
	QT += core
}

CONFIG  += qt warn_on c++14

DEFINES += QT_DEPRECATED_WARNINGS

lessThan( QT_MAJOR_VERSION, 6 ):lessThan( QT_MINOR_VERSION, 14 ) {
    QMAKE_CXXFLAGS += -Wno-deprecated-copy
}

# Includes =====================================================================

INCLUDEPATH *= \
    $${PWD}/include

# Version and Build date =======================================================

win32: BUILD_DATE = '$(shell echo %DATE:~6,4%-%DATE:~3,2%-%DATE:~0,2%)'
else:  BUILD_DATE = '$(shell date +%Y-%m-%d)'

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_BUILD_DATE=\\\"$$BUILD_DATE\\\"

# Release ======================================================================

CONFIG(release, debug|release) {

    # Output dir ------------------------------------------

    DESTDIR = $${_PRO_FILE_PWD_}/../_distrib/$${QMAKE_HOST.os}-$${QMAKE_HOST.arch}/bin

    # translations ----------------------------------------

    TRANSLATIONS_DIR = $$absolute_path( $${DESTDIR}/../translations )

    !exists( $${TRANSLATIONS_DIR} ) {

        translations_dir.commands = $${QMAKE_MKDIR} $$shell_path( $${TRANSLATIONS_DIR} )

        QMAKE_EXTRA_TARGETS += \
            translations_dir

        PRE_TARGETDEPS += \
            translations_dir
    }
}
