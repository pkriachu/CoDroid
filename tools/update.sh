#!/bin/sh

CODROIDHOME=$HOME/myprog/android/analyze/coverage
CODROID=$CODROIDHOME/codroid
MANIFEST=$1/AndroidManifest.xml

if [ -z "$1" ]; then
	echo "usage: $0 sample-apk-without-dot-apk"
	echo "	e.g., $0 com.rovio.angrybirds"
	exit 1
fi

if [ ! -f "$1.apk" ]; then
	echo "unable to read '$1.apk'"
	exit 2
fi

# remove prebuilt apks
rm -f "./$1/dist/$1.apk"
# rebuild the apk
apktool b "./$1/"
# sign the apk
rm -f debug.keystore
keytool -genkey -v -keystore ./debug.keystore -alias android -keyalg RSA -keysize 2048 -validity 20000 \
	-keypass aaaaaa -storepass aaaaaa \
	-dname "CN=codroid.org, OU=ID, O=CoDroid, L=Android, S=CoDroid, C=TW"
jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keypass aaaaaa -storepass aaaaaa \
	-keystore ./debug.keystore "./$1/dist/$1.apk" android

