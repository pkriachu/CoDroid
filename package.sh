#!/bin/bash

if [[ $# < 2 ]]; then
	echo "usage: $0 apkfile local|network"
	echo "	e.g., $0 com.rovio.angrybirds.apk local"
	exit 1
fi

APK_PATH=$1
APK_NAME=$(basename $1 .apk)
APK_DIR=$(dirname $1)

CODROIDHOME=/home/pkriachu/codroid/coverage
CODROID=$CODROIDHOME/codroid
PATCHMANIFEST=$CODROIDHOME/manifestParser.py
APKTOOL=apktool


if [ ! -f "$APK_PATH" ]; then
	echo "unable to read '$APK_PATH.apk'"
	exit 2
fi

if [ ! -x $CODROID -o ! -x $PATCHMANIFEST ]; then
	echo "unable to find $CODROID or $PATCHMANIFEST"
	exit 3
fi



# decomile the apk
echo $APKTOOL d $APK_PATH
$APKTOOL d -o "/tmp/$APK_NAME" $APK_PATH

# patch AndroidManifest.xml
MANIFEST=/tmp/$APK_NAME/AndroidManifest.xml
if [ ! -e "$MANIFEST" ]; then
	echo "Unable to find $MANIFEST"
	exit 3
fi

# patch codes
$PATCHMANIFEST $MANIFEST
case $2 in
  "local")
	echo "Build apk with local writer ..."
	$CODROID -l -d /tmp/cc -m "$APK_DIR/$APK_NAME.meta" -o "/tmp/$APK_NAME/codroid.smali" "/tmp/$APK_NAME/"	# LocalWriter
	;;
  "network")
	echo "Build apk with network writer ..."
	$CODROID -n -d /tmp/cc -m "$APK_DIR/$APK_NAME.meta" -o "/tmp/$APK_NAME/codroid.smali" "/tmp/$APK_NAME/"	# NetworkWriter
	;;
esac

#
rm -rf "/tmp/$APK_NAME/smali"
mv "/tmp/$APK_NAME/codroid.smali" "/tmp/$APK_NAME/smali"
cp -r "$CODROIDHOME/org" "/tmp/$APK_NAME/smali/"

# rebuild the apk
$APKTOOL b "/tmp/$APK_NAME/" -o "$APK_DIR/$APK_NAME.patched.apk"

# sign the apk
rm -f /tmp/debug.keystore
keytool -genkey -v -keystore /tmp/debug.keystore -alias android -keyalg RSA -keysize 2048 -validity 20000 \
	-keypass aaaaaa -storepass aaaaaa \
	-dname "CN=codroid.org, OU=ID, O=CoDroid, L=Android, S=CoDroid, C=TW"
jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keypass aaaaaa -storepass aaaaaa \
	-keystore /tmp/debug.keystore "$APK_DIR/$APK_NAME.patched.apk" android

# clean temp files
rm -rf "/tmp/$APK_NAME"

