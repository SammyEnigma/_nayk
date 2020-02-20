#===============================================================================
#
# nayk library - network
#
#===============================================================================

!contains(QT, network) {
	QT += network
}

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/http_server.cpp \
    $${PWD}/sources/network_client.cpp \
    $${PWD}/sources/http_client.cpp \
    $${PWD}/sources/telegram.cpp

HEADERS *= \
	$${PWD}/include/nayk_global.h \
    $${PWD}/include/http_const.h \
    $${PWD}/include/http_server.h \
    $${PWD}/include/network_client.h \
    $${PWD}/include/http_client.h \
    $${PWD}/include/telegram.h

# Release ======================================================================

CONFIG(release, debug|release) {

    TRANSLATIONS += \
        $${PWD}/resources/translations/nayk_network_ru.ts

    nayk_network_tr.commands = lrelease $${PWD}/resources/translations/nayk_network_ru.ts -qm $${TRANSLATIONS_DIR}/nayk_network_ru.qm

    POST_TARGETDEPS += \
        nayk_network_tr

    QMAKE_EXTRA_TARGETS += \
        nayk_network_tr
}
