#===============================================================================
#
# nayk library - port
#
#===============================================================================

!contains(QT, serialport) {
    QT += serialport
}

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/simple_uart.cpp \
    $${PWD}/sources/com_port.cpp \
    $${PWD}/sources/lpt_port.cpp

HEADERS *= \
	$${PWD}/include/nayk_global.h \
    $${PWD}/include/abstract_port.h \
    $${PWD}/include/simple_uart.h \
    $${PWD}/include/com_port.h \
    $${PWD}/include/lpt_port.h

# Release ======================================================================

CONFIG(release, debug|release) {

    TRANSLATIONS += \
        $${PWD}/resources/translations/nayk_port_ru.ts

    nayk_port_tr.commands = lrelease $${PWD}/resources/translations/nayk_port_ru.ts -qm $${TRANSLATIONS_DIR}/nayk_port_ru.qm

    POST_TARGETDEPS += \
        nayk_port_tr

    QMAKE_EXTRA_TARGETS += \
        nayk_port_tr
}
