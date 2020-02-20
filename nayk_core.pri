#===============================================================================
#
# nayk library - core classes and utils
#
#===============================================================================

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/console.cpp \
    $${PWD}/sources/app_core.cpp \
    $${PWD}/sources/convert.cpp \
    $${PWD}/sources/file_sys.cpp \
    $${PWD}/sources/system_utils.cpp \
    $${PWD}/sources/geo.cpp \
    $${PWD}/sources/log.cpp

HEADERS *= \
	$${PWD}/include/nayk_global.h \
    $${PWD}/include/console.h \
    $${PWD}/include/app_core.h \
    $${PWD}/include/convert.h \
    $${PWD}/include/file_sys.h \
    $${PWD}/include/system_utils.h \
    $${PWD}/include/geo.h \
    $${PWD}/include/log.h

# Release ======================================================================

CONFIG(release, debug|release) {

    TRANSLATIONS += \
        $${PWD}/resources/translations/nayk_core_ru.ts

    nayk_core_tr.commands = lrelease $${PWD}/resources/translations/nayk_core_ru.ts -qm $${TRANSLATIONS_DIR}/nayk_core_ru.qm

    POST_TARGETDEPS += \
        nayk_core_tr

    QMAKE_EXTRA_TARGETS += \
        nayk_core_tr
}
