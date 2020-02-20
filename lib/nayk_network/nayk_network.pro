#===============================================================================
#
# nayk_network
#
#===============================================================================

QT -= gui
CONFIG += nayk_network

include( $${PWD}/../common.pri )

win32: LIBS += -lnayk_core$${VER_MAJ}
else:  LIBS += -lnayk_core
