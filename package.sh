#!/bin/bash

MANIFEST=$1/AndroidManifest.xml
CODROIDHOME=/home/pkriachu/codroid/coverage
CODROID=$CODROIDHOME/codroid
PATCHMANIFEST=$CODROIDHOME/manifestParser.py
APKTOOL=apktool

if [[ $# < 2 ]]; then
	echo "usage: $0 sample-apk-without-dot-apk local|network"
	echo "	e.g., $0 com.rovio.angrybirds local"
	exit 1
fi

if [ ! -f "$1.apk" ]; then
	echo "unable to read '$1.apk'"
	exit 2
fi

if [ ! -x $CODROID -o ! -x $PATCHMANIFEST ]; then
	echo "unable to find $CODROID or $PATCHMANIFEST"
	exit 3
fi



# decomile the apk
echo $APKTOOL d $1.apk
$APKTOOL d $1.apk

# patch AndroidManifest.xml
if [ ! -e "$MANIFEST" ]; then
	echo "Unable to find $MANIFEST"
	exit 3
fi

# patch codes
$PATCHMANIFEST $MANIFEST
case $2 in
  "local")
	echo "Build apk with local writer ..."
	$CODROID -l -d /tmp/cc -m "$1.meta" -o $1.codroid.smali "./$1/"	# LocalWriter
	;;
  "network")
	echo "Build apk with network writer ..."
	$CODROID -n -d /tmp/cc -m "$1.meta" -o $1.codroid.smali "./$1/"	# NetworkWriter
	;;
esac

#
rm -rf "./$1/smali"
mv "$1.codroid.smali" "./$1/smali"
cp -r "$CODROIDHOME/org" "./$1/smali/"

# rebuild the apk
$APKTOOL b "./$1/" -o $1.repacked.apk

# sign the apk
rm -f debug.keystore
keytool -genkey -v -keystore ./debug.keystore -alias android -keyalg RSA -keysize 2048 -validity 20000 \
	-keypass aaaaaa -storepass aaaaaa \
	-dname "CN=codroid.org, OU=ID, O=CoDroid, L=Android, S=CoDroid, C=TW"
jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keypass aaaaaa -storepass aaaaaa \
	-keystore ./debug.keystore "./$1.repacked.apk" android

# clean temp files
rm -rf $1

