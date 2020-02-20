#===============================================================================
#
# nayk library - graph
#
#===============================================================================

!contains(QT, gui) {
	QT += gui
}

!contains(QT, widgets) {
	QT += widgets
}

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/graph.cpp

HEADERS *= \
	$${PWD}/include/nayk_global.h \
    $${PWD}/include/graph.h
