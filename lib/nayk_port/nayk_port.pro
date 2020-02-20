#===============================================================================
#
# nayk_port
#
#===============================================================================

QT += gui widgets
CONFIG += nayk_port

include( $${PWD}/../common.pri )

win32: LIBS += -lnayk_core$${VER_MAJ}
else:  LIBS += -lnayk_core
