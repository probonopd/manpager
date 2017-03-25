#!/bin/sh
ORIGIN=`dirname $0`
TARGET="$1"
APPTRANSLATE="${ORIGIN}/../translate"
ARCHIVE="../Archive"
set -e
mkdir -p ${ARCHIVE}
rm -rf ${ARCHIVE}/${TARGET}.app ${ARCHIVE}/${TARGET}.dSYM
cp -a ${TARGET}.app ${ARCHIVE}
cp -a ${TARGET}.app.dSYM ${ARCHIVE}/${TARGET}.dSYM
cd ${ARCHIVE}
if test -d "${APPTRANSLATE}" ; then
	SYSTRANSLATE=`qmake -query QT_INSTALL_TRANSLATIONS`
	mkdir -p ${TARGET}.app/Contents/Translations
	cp -a ${APPTRANSLATE}/*.qm ${TARGET}.app/Contents/Translations/
	cp -a ${SYSTRANSLATE}/qt_??.qm ${TARGET}.app/Contents/Translations/
fi
macdeployqt ${TARGET}.app -verbose=0 -always-overwrite
