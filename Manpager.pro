PRODUCT = Manpager
TEMPLATE = app
VERSION = 1.1.0
COPYRIGHT = 2017
ARCHIVE = manpager
TARGET = manpager

# basic compile and link config
CONFIG += c++11 warning widgets gui core
QT += widgets

# build type specific options
CONFIG(release,release|debug):DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG
else:!CONFIG(no-testdata) {
    CONFIG(userdata):DEFINES += PROJECT_TESTDATA=\\\"$${PWD}/userdata\\\"
    else:DEFINES += PROJECT_TESTDATA=\\\"$${PWD}/testdata\\\"
    CONFIG -= app_bundle
}

# platform specific options
unix {
    isEmpty(PREFIX):PREFIX=$$system(echo $$[QT_INSTALL_DATA] | sed s:/[a-z0-9]*/qt5$::)
    system(rm -f "$${OUT_PWD}/$${TARGET}")

    exists(/usr/bin/lrelease-qt5) {
        LRELEASE = lrelease-qt5
        LUPDATE = lupdate=qt5
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
        QMAKE_INFO_PLIST = "$${PRODUCT}.plist"
        TARGET = "$${PRODUCT}"
        ICON = "$${PRODUCT}.icns"
        system(rm -rf "$${OUT_PWD}/$${TARGET}.app")
    }
}

win32 {
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /debug
    CONFIG -= debug_and_release debug_and_release_target
    CONFIG += skip_target_version_ext app_bundle
    LRELEASE = lrelease
    LUPDATE = lupdate
    DEFINES += WIN32_LEAN_AND_MEAN
    TARGET = "$${PRODUCT}"
    RC_ICONS += "$${PRODUCT}.ico"
    system(rmdir /S/Q $$shell_path($${OUT_PWD}/bundled) 2>nul)
    system($$sprintf("$$QMAKE_MKDIR_CMD", $$shell_path($${OUT_PWD}/bundled))) 
    system(del /q $$shell_path($${OUT_PWD}/$${PRODUCT}.exe) 2>nul)
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
QMAKE_EXTRA_COMPILERS += lrelease
lupdate.commands += $${LUPDATE} "$${PWD}/$${PRODUCT}.pro"
system($$sprintf("$$QMAKE_MKDIR_CMD", $$shell_path($${OUT_PWD}/generated))) 

lrelease.input = TRANSLATIONS
lrelease.output = "$${OUT_PWD}"/generated/${QMAKE_FILE_BASE}.qm
lrelease.commands += $${LRELEASE} ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_OUT}
lrelease.CONFIG += no_link
PRE_TARGETDEPS += compiler_lrelease_make_all

# extra install targets based on bundle state
!CONFIG(app_bundle) {
    QMAKE_EXTRA_TARGETS += target man1 locale
    INSTALLS += target man1 locale

    target.path = "$$PREFIX/bin"
    target.depends = all

    locale.path = "$$PREFIX/share/translations"
    locale.depends = target
    locale.commands += $${QMAKE_INSTALL_FILE} generated/*.qm "$(INSTALL_ROOT)/$$PREFIX/share/translations"

    man1.files = xdg/*.1
    man1.path = "$$PREFIX/share/man/man1"
    man1.depends = target

    unix:!macx {
        QMAKE_EXTRA_TARGETS += desktop pixmaps appdata
        INSTALLS += desktop pixmaps appdata

        desktop.files = xdg/*.desktop
        desktop.path = "$$PREFIX/share/applications"
        desktop.depends = target

        pixmaps.files = xdg/*.png
        pixmaps.path = "$$PREFIX/share/pixmaps"
        pixmaps.depends = target

        appdata.files = xdg/*.appdata.xml
        appdata.path = "$$PREFIX/share/appdata"
        appdata.depends = target
    }
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
        QMAKE_POST_LINK += $${LRELEASE} -silent "$${PWD}/$${PRODUCT}.pro" &&
        QMAKE_POST_LINK += cp -a "$$[QT_INSTALL_TRANSLATIONS]"/qt_??.qm generated/*.qm "$${TARGET}.app/Contents/Translations"
    }
    
    QMAKE_EXTRA_TARGETS += clean extra_clean
    clean.depends += extra_clean
    macx:extra_clean.commands += rm -rf $${TARGET}.app $${TARGET}.app.dSYM
    win32:extra_clean.commands += rmdir /S/Q bundled && mkdir bundled
} 

# publish support
QMAKE_EXTRA_TARGETS += publish
publish.commands += $$QMAKE_DEL_FILE *.tar.gz &&
publish.commands += cd $${PWD} &&
publish.commands += git archive --output="$${OUT_PWD}/$${ARCHIVE}-$${VERSION}.tar.gz" --format tar.gz  --prefix=$${ARCHIVE}-$${VERSION}/ HEAD
linux:exists("/usr/bin/rpmbuild"):\
    publish.commands += && rm -f *.src.rpm && rpmbuild --define \"_tmppath /tmp\" --define \"_sourcedir .\" --define \"_srcrpmdir .\" --nodeps -bs $${ARCHIVE}.spec

# documentation processing
QMAKE_EXTRA_TARGETS += docs
QMAKE_SUBSTITUTES += doxyfile
DOXYPATH = $${PWD}
doxyfile.input = $${PWD}/Doxyfile
doxyfile.output = $${OUT_PWD}/Doxyfile.out
macx:docs.commands += PATH=/usr/local/bin:/usr/bin:/bin:/Library/Tex/texbin:$PATH && export PATH &&
docs.commands += cd $${OUT_PWD} && doxygen Doxyfile.out
macx:docs.commands += && cd doc/html && make docset && cd ../..
unix:docs.commands += && cd doc/latex && make

# clean additional testing files on distclean...
QMAKE_EXTRA_TARGETS += distclean publishclean
distclean.depends += publishclean
publishclean.commands += rm -rf Archive $${ARCHIVE}-*.tar.gz $${ARCHIVE}-*.pdf $${ARCHIVE} doc Doxyfile.out

RESOURCES += $${PRODUCT}.qrc
OTHER_FILES += \
    $${ARCHIVE}.spec \
    CHANGELOG \
    LICENSE \
    README.md \
    CONTRIBUTING.md \
    $${PRODUCT}.icns \
    $${PRODUCT}.ico \
    $${PRODUCT}.plist \
    xdg/$${ARCHIVE}.1 \
    xdg/$${ARCHIVE}.png \
    xdg/$${ARCHIVE}.desktop \
    xdg/$${ARCHIVE}.appdata.xml \

QMAKE_TARGET_COMPANY = "Tycho Softworks"
QMAKE_TARGET_COPYRIGHT = "$${COPYRIGHT} Tycho Softworks"
QMAKE_TARGET_PRODUCT = "$${PRODUCT}"
QMAKE_TARGET_DESCRIPTION = "View system manpages"

