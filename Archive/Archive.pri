# source publication

QMAKE_EXTRA_TARGETS += publish
publish.commands += cd $${PWD}/.. &&

win32 {
    publish.commands += del $${OUT_PWD}/$${ARCHIVE}-$${VERSION}.zip &&
    publish.commands += git archive --format zip -o $${OUT_PWD}/$${ARCHIVE}-$${VERSION}.zip --prefix=$${ARCHIVE}-$${VERSION}/ HEAD
}

unix {
    publish.commands += rm -f $${OUT_PWD}/$${ARCHIVE}-${VERSION}.tar.gz &&
    publish.commands += git archive --format tar --prefix=$${ARCHIVE}-$${VERSION}/ HEAD |
    publish.commands += gzip >$${OUT_PWD}/$${ARCHIVE}-$${VERSION}.tar.gz

    exists($${PWD}/Archive.spec) {
        specfile.input = $${PWD}/Archive.spec
        specfile.output = $${ARCHIVE}.spec

        RELEASE=$$system($${PWD}/Archive.bld $${VERSION})
        QMAKE_SUBSTITUTES += specfile
    }
}

# binary packages, for release builds only...

macx:CONFIG(release, release|debug):CONFIG(app_bundle) {
    QMAKE_EXTRA_TARGETS += archive publish_and_archive
    archive.depends = all
    archive.commands += $${PWD}/Archive.sh $${TARGET}
    publish_and_archive.depends = publish archive
}

win32:CONFIG(release, release|debug) {
    QMAKE_EXTRA_TARGETS += archive publish_and_archive
    archive.depends = all
    archive.commands += $${PWD}/Archive.cmd $${TARGET}
    publish_and_archive.depends = publish archive
}

OTHER_FILES += \
    $${PWD}/Archive.spec \
    $${PWD}/Archive.sh \
    $${PWD}/Archive.cmd \
    $${PWD}/Archive.bld \
