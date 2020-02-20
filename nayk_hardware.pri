#===============================================================================
#
# nayk library - hardware
#
#===============================================================================

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/hardware_utils.cpp

HEADERS *= \
	$${PWD}/include/nayk_global.h \
    $${PWD}/include/hardware_utils.h

# Libs =========================================================================

win32:LIBS += -lKernel32 -lPsapi
