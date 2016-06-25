<?php
//require("env.php");
require("function.php");


$hashid = md5($APK_NAME);
$fd = $hashid{0}.$hashid{1};
$sd = $hashid{2}.$hashid{3};
$PATH_OF_REPORT = "$OUT_PATH$fd/$sd/$APK_NAME-$APK_VER-$APK_MD5/$EMULATOR_VER/";


$fp = fopen($PATH_OF_REPORT."overview", "r");
while( !feof( $fp ) )
{
$TEST_FILES .= getline($fp, "\n");
$TEST_FILES .= "<br />";
}

fclose($fp);



//file list
$FILE_LIST = "";

$fp = fopen($PATH_OF_REPORT."/"."filelist", "r");
while( !feof( $fp ) )
{
	$FILE_LIST .= getline($fp, "\n");
	$FILE_LIST .= "<br />";
}
fclose($fp);



//permission list
$PERMISSIONS = "";

$fp = fopen($PATH_OF_REPORT."permissions", "r");
while( !feof( $fp ) )
{
	$PERMISSIONS .= getline($fp, "\n");
	$PERMISSIONS .= "<br />";
}
fclose($fp);






$TEST_SIGN = "";

$fp = fopen($PATH_OF_REPORT."sign", "r");
while( !feof( $fp ) )
{
$TEST_SIGN .= getline($fp, "\n");
$TEST_SIGN .= "<br />";
}
fclose($fp);

?>
