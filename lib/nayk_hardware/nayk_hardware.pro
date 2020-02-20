#===============================================================================
#
# nayk_hardware
#
#===============================================================================

QT -= gui
CONFIG += nayk_hardware

include( $${PWD}/../common.pri )

win32: LIBS += -lnayk_core$${VER_MAJ}
else:  LIBS += -lnayk_core
