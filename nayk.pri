#===============================================================================
#
# Projects Settings - nayk library
#
#===============================================================================

include( $${PWD}/nayk_common.pri )

nayk_core {
	include( $${PWD}/nayk_core.pri )
}

nayk_qml {
    include( $${PWD}/nayk_qml.pri )
} 

nayk_widgets {
    include( $${PWD}/nayk_widgets.pri )
}

nayk_graph {
    include( $${PWD}/nayk_graph.pri )
}

nayk_network {
    include( $${PWD}/nayk_network.pri )
}

nayk_port {
    include( $${PWD}/nayk_port.pri )
}

nayk_sql {
    include( $${PWD}/nayk_sql.pri )
}

nayk_hardware {
    include( $${PWD}/nayk_hardware.pri )
}
