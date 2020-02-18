#===============================================================================
#
# nayk library - Projects Settings for QML
#
#===============================================================================

# Sources and Headers ==========================================================

SOURCES *= \
    $${PWD}/sources/gui_app_core.cpp

HEADERS *= \
    $${PWD}/include/gui_app_core.h \
    $${PWD}/include/images_const.h


# Resources files ==============================================================

RESOURCES *= \
    $${PWD}/qml/qml_resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH *= \
    $${PWD}/qml/

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH *= \
    $${PWD}/qml/

# Release ======================================================================

CONFIG(release, debug|release) {

    TRANSLATIONS += \
        $${PWD}/resources/translations/nayk_qml_ru.ts

    nayk_qml_tr.commands = lrelease $${PWD}/resources/translations/nayk_qml_ru.ts -qm $${TRANSLATIONS_DIR}/nayk_qml_ru.qm

    POST_TARGETDEPS += \
        nayk_qml_tr

    QMAKE_EXTRA_TARGETS += \
        nayk_qml_tr
}
