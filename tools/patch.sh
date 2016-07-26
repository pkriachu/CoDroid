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
	echo "usage: $0 file_name(in uploads folder) local|network"
	echo "	e.g., $0 com.rovio.angrybirds.apk local"
	exit 1
fi

APK_NAME=$(echo $1 | sed 's/\.apk$//g')

echo "$0 $@" > $DOWNLOADS/$APK_NAME.log
echo "$0 $@" > $DOWNLOADS/$APK_NAME.time

echo $TIME -ap -o $DOWNLOADS/$APK_NAME.time $CODROID_ROOT/package.sh $UPLOADS/$1 $2 >>$DOWNLOADS/$APK_NAME.log 2>&1
$TIME -ap -o $DOWNLOADS/$APK_NAME.time $CODROID_ROOT/package.sh $UPLOADS/$1 $2 >>$DOWNLOADS/$APK_NAME.log 2>&1

if [ -z ${2+x} ]; then
	mv $UPLOADS/$APK_NAME.patched.apk $DOWNLOADS
	mv $UPLOADS/$APK_NAME.meta $DOWNLOADS
else
	mv $UPLOADS/$APK_NAME.patched.apk $DOWNLOADS/$2.patched.apk
	mv $UPLOADS/$APK_NAME.meta $DOWNLOADS/$2.meta
fi

