#!/bin/bash

# Author: Kaze Mewno (C.H. Chiu)
# Last Modify: 2016.6.30
#
# Reading apk files from uploading folder, and use CoDroid to patch them.

# $1: file name
# $2: patch mode

TIME=/usr/bin/time
CODROID_ROOT=/home/pkriachu/codroid
UPLOADS=$CODROID_ROOT/www/uploads
DOWNLOADS=$CODROID_ROOT/www/downloads

if [[ $# < 2 ]]; then
	echo "usage: $0 file_name(in uploads folder)"
	echo "	e.g., $0 com.rovio.angrybirds.apk"
	exit 1
fi

APK_NAME=$(echo $1 | sed 's/\.apk$//g')

echo "$0 $@" > $DOWNLOADS/$APK_NAME.log
echo "$0 $@" > $APK_NAME.time
case $2 in 
  "local")
	$TIME -ap -o $APK_NAME.time $CODROID_ROOT/package.sh $APK_NAME local >>$CODROID_ROOT/www/downloads/$APK_NAME.log 2>&1
	;;
  "network")
	$TIME -ap -o $APK_NAME.time $CODROID_ROOT/package.sh $APK_NAME network >>$CODROID_ROOT/www/downloads/$APK_NAME.log 2>&1
	;;
esac

mv $APK_NAME.repacked.apk ../repacked
mv $APK_NAME.meta ../repacked

