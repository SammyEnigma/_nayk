#===============================================================================
#
# Projects Settings - nayk library
#
#===============================================================================

include( $${PWD}/nayk_common.pri )

contains(QT, quick) {

    include( $${PWD}/nayk_qml.pri )
} 

contains(QT, widgets) {

    include( $${PWD}/nayk_widgets.pri )
}

contains(QT, network) {

    include( $${PWD}/nayk_network.pri )
}

contains(QT, serialport) | contains(CONFIG, parallelport) {

    include( $${PWD}/nayk_port.pri )
}

contains(QT, sql) {

    include( $${PWD}/nayk_sql.pri )
}

contains(CONFIG, hardware) {

    include( $${PWD}/nayk_hardware.pri )
}
