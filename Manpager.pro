win32|macx:TARGET = Manpager
else:TARGET = manpager

COPYRIGHT = 2017
PRODUCT = Manpager
ARCHIVE = manpager
TEMPLATE = app
VERSION = 1.0.2
CONFIG += c++11 warning widgets gui core
QT += widgets

xcode_export:CONFIG -= app_bundle

unix {
    isEmpty(PREFIX):PREFIX=$$system(echo $$[QT_INSTALL_DATA] | sed s:/[a-z0-9]*/qt5$::)
    !macx:CONFIG += install_program install_desktop
}

macx:equals(PREFIX, "/usr/local") {
    CONFIG -= app_bundle
    CONFIG += install_program
}

win32 {
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /debug
    CONFIG -= debug_and_release debug_and_release_target
    CONFIG += skip_target_version_ext
    DEFINES += WIN32_LEAN_AND_MEAN
}

CONFIG(release,release|debug) {
    DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG
    macx:CONFIG+=force_debug_info separate_debug_info
}
else:!no-testdata:DEFINES += PROJECT_TESTDATA=\"$${PWD}/testdata\"

DEFINES += PROJECT_VERSION=$${VERSION} PROJECT_COPYRIGHT=$${COPYRIGHT}

include(src/Main.pri)
include(src/Utils.pri)
include(src/Viewer.pri)
include(xdg/XDG.pri)

install_program {
    QMAKE_EXTRA_TARGETS += target locale
    INSTALLS += target locale

    target.path = $$PREFIX/bin
    target.depends = all

    locale.files = $${PWD}/xdg/*.qm
    locale.path = $$PREFIX/share/translations
    locale.depends = target
}

# extra unix cleanup
unix {
    QMAKE_EXTRA_TARGETS += clean extra_clean distclean publish_clean
    clean.depends += extra_clean
    extra_clean.commands += rm -f $${TARGET}
    macx:extra_clean.commands += && rm -rf $${TARGET}.app $${TARGET}.app.dSYM
    distclean.depends += publish_clean
    publish_clean.commands = rm -rf Archive $${ARCHIVE}-*.tar.gz $${PWD}/xdg/*.qm
}

# extra windows cleanup
win32 {
    QMAKE_EXTRA_TARGETS += clean extra_clean
    clean.depends += extra_clean
    extra_clean.commands += if exists $${TARGET}.exe del $${TARGET}.exe
}

RESOURCES += Manpager.qrc
OTHER_FILES += \
    manpager.spec \
    CHANGELOG \
    LICENSE \
    README.md \
    CONTRIBUTING.md \

QMAKE_TARGET_COMPANY = "Tycho Softworks"
QMAKE_TARGET_COPYRIGHT = "$${COPYRIGHT} Tycho Softwork"
QMAKE_TARGET_PRODUCT = "$${PRODUCT}"
QMAKE_TARGET_DESCRIPTION = "View system manpages"

