#===============================================================================
#
# nayk_sql
#
#===============================================================================

QT -= gui
CONFIG += nayk_sql

include( $${PWD}/../common.pri )

win32: LIBS += -lnayk_core$${VER_MAJ}
else:  LIBS += -lnayk_core