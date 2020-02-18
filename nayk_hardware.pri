#===============================================================================
#
# Projects Settings with config harware - nayk library
#
#===============================================================================

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/hardware_utils.cpp

HEADERS *= \
    $${PWD}/include/hardware_utils.h

# Libs =========================================================================

win32:LIBS += -lKernel32 -lPsapi
