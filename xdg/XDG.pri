# xdg builds
install_desktop {
    QMAKE_EXTRA_TARGETS += desktop pixmaps appdata
    INSTALLS += desktop pixmaps appdata

    desktop.files = $${PWD}/$${ARCHIVE}.desktop
    desktop.path = $$PREFIX/share/applications
    desktop.depends = target

    pixmaps.files = $${PWD}/$${ARCHIVE}.png
    pixmaps.path = $$PREFIX/share/pixmaps
    pixmaps.depends = target

    appdata.files = $${PWD}/$${ARCHIVE}.appdata.xml
    appdata.path = $$PREFIX/share/appdata
    appdata.depends = target
}

# translations
win32:system(lrelease -silent ../$${PRODUCT}.pro)
else {
    exists(/usr/bin/lrelease-qt5):system(lrelease-qt5 -silent ../$${PRODUCT}.pro)
    else:system(lrelease -silent ../$${PRODUCT}.pro)
}

# source publication
unix {
    QMAKE_EXTRA_TARGETS += publish
    publish.commands += cd $${PWD}/.. &&
    publish.commands += rm -f $${OUT_PWD}/$${ARCHIVE}-${VERSION}.tar.gz &&
    publish.commands += git archive --format tar --prefix=$${ARCHIVE}-$${VERSION}/ HEAD |
    publish.commands += gzip >$${OUT_PWD}/$${ARCHIVE}-$${VERSION}.tar.gz
}

# binary builds for mac release
macx:CONFIG(release, release|debug):CONFIG(app_bundle) {
    QMAKE_EXTRA_TARGETS += archive publish_and_archive
    archive.depends = all
    archive.commands += $${PWD}/archive.sh $${TARGET}
    publish_and_archive.depends = publish archive
}

# binary builds for windows release
win32:CONFIG(release, release|debug) {
    QMAKE_EXTRA_TARGETS += archive
    archive.depends = all
    archive.commands += $${PWD}/archive.cmd $${TARGET}
}

macx:QMAKE_INFO_PLIST = $${PWD}/$${ARCHIVE}.plist
macx:ICON = $${PWD}/$${ARCHIVE}.icns
win32:RC_ICONS += $${PWD}/$${ARCHIVE}.ico

TRANSLATIONS += \
    $${PWD}/$${ARCHIVE}_es.ts \

OTHER_FILES += \
    $${PWD}/archive.sh \
    $${PWD}/archive.cmd \
    $${PWD}/$${ARCHIVE}.png \
    $${PWD}/$${ARCHIVE}.desktop \
    $${PWD}/$${ARCHIVE}.appdata.xml \
    $${PWD}/$${ARCHIVE}.plist \
    $${PWD}/$${ARCHIVE}.icns \
    $${PWD}/$${ARCHIVE}.ico \

