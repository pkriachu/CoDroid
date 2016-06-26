#!/bin/bash

# $1: apk file
# $2: patch mode

TIME=/usr/bin/time
CODROID_ROOT=/home/pkriachu/codroid

if [[ $# < 2 ]]; then
	echo "usage: $0 apk-file"
	echo "	e.g., $0 com.rovio.angrybirds.apk"
	exit 1
fi

APK_NAME=$(echo $1 | sed 's/\.apk$//g')

echo "$0 $@" > $APK_NAME.log
echo "$0 $@" > $APK_NAME.time
case $2 in 
  "local")
	$TIME -ap -o $APK_NAME.time $CODROID_ROOT/package.sh $APK_NAME local >>$APK_NAME.log 2>&1
	;;
  "network")
	$TIME -ap -o $APK_NAME.time $CODROID_ROOT/package.sh $APK_NAME network >>$APK_NAME.log 2>&1
	;;
esac

mv $APK_NAME.repacked.apk ../repacked
mv $APK_NAME.meta ../repacked

