TEMPLATE = app
VERSION = 1.2.0
COPYRIGHT = 2017-2018
ARCHIVE = manpager

# basic compile and link config
CONFIG += c++11 warning widgets gui core
QT += widgets

# build type specific options
CONFIG(release,release|debug):DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG
else:DEFINES += PROJECT_TESTDATA=\\\"$${PWD}/testdata\\\"

# platform specific options
unix {
    !macx:TARGET=manpager
    isEmpty(PREFIX):PREFIX=$$system(echo $$[QT_INSTALL_DATA] | sed s:/[a-z0-9]*/qt5$::)
    system(rm -f "$${OUT_PWD}/$${TARGET}")

    exists(/usr/bin/lrelease-qt5) {
        LRELEASE = lrelease-qt5
        LUPDATE = lupdate-qt5
    }
    else {
        LRELEASE = lrelease
        LUPDATE = lupdate
    }    
}

macx {
    equals(PREFIX, "/usr/local"):CONFIG -= app_bundle
    CONFIG(app_bundle) {
        CONFIG(release, release|debug):CONFIG += separate_debug_info force_debug_info
        QMAKE_INFO_PLIST = "$${TARGET}.plist"
        ICON = "$${TARGET}.icns"
        system(rm -rf "$${OUT_PWD}/$${TARGET}.app")
    }
    else:TARGET=manpager
}

win32 {
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /debug
    CONFIG -= debug_and_release debug_and_release_target
    CONFIG += skip_target_version_ext
    LRELEASE = lrelease
    LUPDATE = lupdate
    DEFINES += WIN32_LEAN_AND_MEAN
    RC_ICONS += "$${TARGET}.ico"
    system(rmdir /S/Q $$shell_path($${OUT_PWD}/bundled) 2>nul)
    system($$sprintf("$$QMAKE_MKDIR_CMD", $$shell_path($${OUT_PWD}/bundled))) 
    system(del /q $$shell_path($${OUT_PWD}/$${TARGET}.exe) 2>nul)
}

# global defines
DEFINES += \
    PROJECT_VERSION=\\\"$${VERSION}\\\" \
    PROJECT_COPYRIGHT=\\\"$${COPYRIGHT}\\\" \

# project layout
OBJECTS_DIR = objects
RCC_DIR = generated
MOC_DIR = generated
UI_DIR = generated

HEADERS += $$files(*.hpp)
SOURCES += $$files(*.cpp)
FORMS += $$files(*.ui)
TRANSLATIONS += $$files(xdg/*.ts)

# generate and support translations
QMAKE_EXTRA_TARGETS += lupdate
lupdate.commands += $${LUPDATE} "$${PWD}/$${TARGET}.pro"
system($$sprintf("$$QMAKE_MKDIR_CMD", $$shell_path($${OUT_PWD}/generated))) 

for(tr, TRANSLATIONS) {
    tr_out=$$basename(tr)
    tr_out=$$split(tr_out, .)
    system($${LRELEASE} -silent $${tr} -qm "$${OUT_PWD}/generated/$$first(tr_out).qm")
}

# source publish
unix {
    QMAKE_EXTRA_TARGETS += source
    source.commands += $$QMAKE_DEL_FILE *.tar.gz &&
    source.commands += cd $${PWD} &&
    source.commands += git archive --output="$${OUT_PWD}/$${ARCHIVE}-$${VERSION}.tar.gz" --format tar.gz  --prefix=$${ARCHIVE}-$${VERSION}/ v$${VERSION} 2>/dev/null || git archive --output="$${OUT_PWD}/$${ARCHIVE}-$${VERSION}.tar.gz" --format tar.gz  --prefix=$${ARCHIVE}-$${VERSION}/ HEAD 
}

# extra install targets based on bundle state
unix:!CONFIG(app_bundle) {
    QMAKE_EXTRA_TARGETS += target locale man1
    INSTALLS += target locale man1

    target.path = "$$PREFIX/bin"
    target.depends = all

    locale.path = "$$PREFIX/share/translations"
    locale.depends = target
    locale.commands += $${QMAKE_INSTALL_FILE} generated/*.qm "$(INSTALL_ROOT)/$$PREFIX/share/translations"

    man1.files = xdg/*.1
    man1.path = "$$PREFIX/share/man/man1"
    man1.depends = target

    !macx {
        QMAKE_EXTRA_TARGETS += desktop pixmaps appdata
        INSTALLS += desktop pixmaps appdata

        desktop.files = xdg/*.desktop
        desktop.path = "$$PREFIX/share/applications"
        desktop.depends = target

        pixmaps.files = xdg/*.png
        pixmaps.path = "$$PREFIX/share/pixmaps"
        pixmaps.depends = target

        appdata.files = xdg/*.appdata.xml
        appdata.path = "$$PREFIX/share/metainfo"
        appdata.depends = target
    }
    exists(debian/Debian.pri):include(debian/Debian.pri)
    exists(package/Package.pri):include(package/Package.pri)

}
else {
    win32:CONFIG(release, release|debug) {
        QMAKE_POST_LINK += rmdir /S/Q bundled && mkdir bundled && mkdir bundled\\translations &&
        QMAKE_POST_LINK += copy /y $${TARGET}.exe bundled\\$${TARGET}.exe && cd bundled &&
        QMAKE_POST_LINK += windeployqt "$${TARGET}.exe" -verbose=0 &&
        QMAKE_POST_LINK += copy /y $$shell_path("../generated/*.qm") translations
    }

    macx:CONFIG(release, release|debug): {
        QMAKE_POST_LINK += mkdir -p "$${TARGET}.app/Contents/Translations" &&
        QMAKE_POST_LINK += macdeployqt "$${TARGET}.app" -verbose=0 -always-overwrite &&
        QMAKE_POST_LINK += $${LRELEASE} -silent "$${PWD}/$${TARGET}.pro" &&
        QMAKE_POST_LINK += cp -a "$$[QT_INSTALL_TRANSLATIONS]"/qt_??.qm generated/*.qm "$${TARGET}.app/Contents/Translations"
    }
    
    QMAKE_EXTRA_TARGETS += clean extra_clean
    clean.depends += extra_clean
    macx:extra_clean.commands += rm -rf $${TARGET}.app $${TARGET}.app.dSYM
    win32:extra_clean.commands += rmdir /S/Q bundled && mkdir bundled
} 

RESOURCES += qrc/desktop.qrc
OTHER_FILES += \
    $${TRANSLATIONS} \
    CHANGELOG \
    LICENSE \
    README.md \
    CONTRIBUTING.md \
    $${TARGET}.icns \
    $${TARGET}.ico \
    $${TARGET}.plist \
    xdg/$${ARCHIVE}.1 \
    xdg/$${ARCHIVE}.png \
    xdg/$${ARCHIVE}.desktop \
    xdg/$${ARCHIVE}.appdata.xml \

QMAKE_TARGET_COMPANY = "Tycho Softworks"
QMAKE_TARGET_COPYRIGHT = "$${COPYRIGHT} Tycho Softworks"
QMAKE_TARGET_PRODUCT = "$${PRODUCT}"
QMAKE_TARGET_DESCRIPTION = "View system manpages"

