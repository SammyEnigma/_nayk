#===============================================================================
#
# nayk library - widgets
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
    $${PWD}/sources/gui_app_core.cpp \
    $${PWD}/sources/gui_utils.cpp \
    $${PWD}/sources/dialog_log.cpp \
    $${PWD}/sources/dialog_busy.cpp \
    $${PWD}/sources/ex_controls.cpp \
    $${PWD}/sources/highlighter.cpp

HEADERS *= \
	$${PWD}/include/nayk_global.h \
    $${PWD}/include/gui_app_core.h \
    $${PWD}/include/images_const.h \
    $${PWD}/include/gui_utils.h \
    $${PWD}/include/dialog_log.h \
    $${PWD}/include/dialog_busy.h \
    $${PWD}/include/ex_controls.h \
    $${PWD}/include/highlighter.h

# Resources files ==============================================================

RESOURCES *= \
    $${PWD}/resources/icons.qrc \
    $${PWD}/resources/led_images.qrc

# Release ======================================================================

CONFIG(release, debug|release) {

    TRANSLATIONS += \
        $${PWD}/resources/translations/nayk_widgets_ru.ts

    nayk_widgets_tr.commands = lrelease $${PWD}/resources/translations/nayk_widgets_ru.ts -qm $${TRANSLATIONS_DIR}/nayk_widgets_ru.qm

    POST_TARGETDEPS += \
        nayk_widgets_tr

    QMAKE_EXTRA_TARGETS += \
        nayk_widgets_tr
}
