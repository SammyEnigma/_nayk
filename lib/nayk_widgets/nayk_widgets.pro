#===============================================================================
#
# nayk_widgets
#
#===============================================================================

CONFIG += nayk_widgets

include( $${PWD}/../common.pri )

win32: LIBS += -lnayk_core$${VER_MAJ}
else:  LIBS += -lnayk_core
