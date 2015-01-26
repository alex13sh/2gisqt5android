MODULE = webenginecore
TARGET = QtWebEngineCore

CMAKE_MODULE_TESTS = "-"

QT += qml quick
QT_PRIVATE += gui-private

# Needed to set a CFBundleIdentifier
QMAKE_INFO_PLIST = Info_mac.plist

# Look for linking information produced by gyp for our target according to core_generated.gyp
!include($$OUT_PWD/$$getConfigDir()/$${TARGET}_linking.pri) {
    error("Could not find the linking information that gyp should have generated.")
}

REPACK_DIR = $$OUT_PWD/$$getConfigDir()/gen/repack
# Duplicated from resources/resources.gyp
LOCALE_LIST = am ar bg bn ca cs da de el en-GB en-US es-419 es et fa fi fil fr gu he hi hr hu id it ja kn ko lt lv ml mr ms nb nl pl pt-BR pt-PT ro ru sk sl sr sv sw ta te th tr uk vi zh-CN zh-TW
for(LOC, LOCALE_LIST) {
    locales.files += $$REPACK_DIR/qtwebengine_locales/$${LOC}.pak
}
resources.files = $$REPACK_DIR/qtwebengine_resources.pak

PLUGIN_EXTENSION = .so
PLUGIN_PREFIX = lib
osx: PLUGIN_PREFIX =
win32 {
    PLUGIN_EXTENSION = .dll
    PLUGIN_PREFIX =
}
icu.files = $$OUT_PWD/$$getConfigDir()/icudtl.dat

plugins.files = $$OUT_PWD/$$getConfigDir()/$${PLUGIN_PREFIX}ffmpegsumo$${PLUGIN_EXTENSION}

!debug_and_release|!build_all|CONFIG(release, debug|release):contains(QT_CONFIG, qt_framework) {
    locales.version = Versions
    locales.path = Resources/qtwebengine_locales
    resources.version = Versions
    resources.path = Resources
    icu.version = Versions
    icu.path = Resources
    plugins.version = Versions
    plugins.path = Libraries
    # No files, this prepares the bundle Helpers symlink, process.pro will create the directories
    qtwebengineprocessplaceholder.version = Versions
    qtwebengineprocessplaceholder.path = Helpers
    QMAKE_BUNDLE_DATA += icu locales resources plugins qtwebengineprocessplaceholder
} else {
    locales.CONFIG += no_check_exist
    locales.path = $$[QT_INSTALL_TRANSLATIONS]/qtwebengine_locales
    resources.CONFIG += no_check_exist
    resources.path = $$[QT_INSTALL_DATA]
    icu.CONFIG += no_check_exist
    icu.path = $$[QT_INSTALL_DATA]
    plugins.CONFIG += no_check_exist
    plugins.path = $$[QT_INSTALL_PLUGINS]/qtwebengine
    INSTALLS += icu locales resources plugins
}

# We distribute the module binary but headers are only available in-tree.
CONFIG += no_module_headers
load(qt_module)

# Using -Wl,-Bsymbolic-functions seems to confuse the dynamic linker
# and doesn't let Chromium get access to libc symbols through dlsym.
CONFIG -= bsymbolic_functions

contains(QT_CONFIG, egl): CONFIG += egl

linux: contains(QT_CONFIG, separate_debug_info): QMAKE_POST_LINK="cd $(DESTDIR) && $(STRIP) --strip-unneeded $(TARGET)"