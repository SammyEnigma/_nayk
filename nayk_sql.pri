#===============================================================================
#
# Projects Settings sql - nayk library
#
#===============================================================================

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/database_client.cpp

HEADERS *= \
    $${PWD}/include/database_client.h

# Release ======================================================================

CONFIG(release, debug|release) {

    TRANSLATIONS += \
        $${PWD}/resources/translations/nayk_sql_ru.ts

    nayk_sql_tr.commands = lrelease $${PWD}/resources/translations/nayk_sql_ru.ts -qm $${TRANSLATIONS_DIR}/nayk_sql_ru.qm

    POST_TARGETDEPS += \
        nayk_sql_tr

    QMAKE_EXTRA_TARGETS += \
        nayk_sql_tr
}
