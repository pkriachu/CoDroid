#!/bin/sh

APK_NAME=$(echo $1 | sed 's/\.apk$//g')

# decomile the apk
$HOME/coverage/apktool/apktool d $APK_NAME.apk
# rebuild the apk
$HOME/coverage/apktool/apktool b -o "./$APK_NAME/dist/$APK_NAME.repacked.apk" "./$APK_NAME/"
# sign the apk
rm -f debug.keystore
keytool -genkey -v -keystore ./debug.keystore -alias android -keyalg RSA -keysize 2048 -validity 20000 \
	-keypass aaaaaa -storepass aaaaaa \
	-dname "CN=codroid.org, OU=ID, O=CoDroid, L=Android, S=CoDroid, C=TW"
jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keypass aaaaaa -storepass aaaaaa \
	-keystore ./debug.keystore "./$APK_NAME/dist/$APK_NAME.repacked.apk" android


