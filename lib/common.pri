#===============================================================================
#
# Common settings for lib projects
#
#===============================================================================

TEMPLATE = lib

#CONFIG += staticlib

DEFINES += NAYK_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

VERSION  = 1.0
VER_MAJ  = 1
VER_MIN  = 0

include( $${PWD}/../nayk.pri )

LIBS += -L$${DESTDIR}
