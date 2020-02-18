#===============================================================================
#
# nayk library - Common Projects Settings - include first!!!
#
#===============================================================================

CONFIG  += c++14

DEFINES += QT_DEPRECATED_WARNINGS

lessThan( QT_MAJOR_VERSION, 6 ):lessThan( QT_MINOR_VERSION, 14 ) {
    QMAKE_CXXFLAGS += -Wno-deprecated-copy
}

# Includes =====================================================================

INCLUDEPATH *= \
    $${PWD}/include

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/include/console.h \
    $${PWD}/sources/app_core.cpp \
    $${PWD}/sources/log.cpp \
    $${PWD}/sources/convert.cpp \
    $${PWD}/sources/file_sys.cpp \
    $${PWD}/sources/system_utils.cpp \
    $${PWD}/sources/geo.cpp

HEADERS *= \
    $${PWD}/sources/console.cpp \
    $${PWD}/include/app_core.h \
    $${PWD}/include/log.h \
    $${PWD}/include/convert.h \
    $${PWD}/include/file_sys.h \
    $${PWD}/include/system_utils.h \
    $${PWD}/include/geo.h

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

    TRANSLATIONS += \
        $${PWD}/resources/translations/nayk_common_ru.ts

    nayk_common_tr.commands = lrelease $${PWD}/resources/translations/nayk_common_ru.ts -qm $${TRANSLATIONS_DIR}/nayk_common_ru.qm

    POST_TARGETDEPS += \
        nayk_common_tr

    QMAKE_EXTRA_TARGETS += \
        nayk_common_tr
}
