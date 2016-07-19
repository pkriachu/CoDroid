<?php

ini_set("display_errors", 1);
ini_set("error_reporting", E_ALL | E_STRICT);


$codroid_db = "../codroid.db";

$key  = "null";
$package  = "null";
$id   = "null";
$data = "null";


# get remote ip
if(!empty($_SERVER['HTTP_CLIENT_IP'])){
    $ip = $_SERVER['HTTP_CLIENT_IP'];
} else if(!empty($_SERVER['HTTP_X_FORWARDED_FOR'])){
    $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
} else {
    $ip = $_SERVER['REMOTE_ADDR'];
}


# pase GET parameters
if(isset($_GET["key"])) {
    $key = $_GET["key"];
}
if(isset($_GET["package"])) {
    $package = $_GET["package"];
}
if(isset($_GET["id"])) {
    $id = $_GET["id"];
}
if(isset($_GET["data"])) {
    $data = $_GET["data"];
}


echo "key = $key<br />";
echo "package = $package<br />";
echo "id = $id<br />";
echo "data = $data<br />";



# validate the key
$db = new PDO("sqlite:$codroid_db");
$query = "SELECT fid FROM remote_auth WHERE authcode='$key'";
$result = $db->query($query);
$row = $result->fetchALL();
if(sizeof($row) == 1) {
    $file_id = $row[0]["fid"];

    # if key is validated, save the records.
    $record_dir = "records/$file_id";
    if(!is_dir($record_dir)) {
        mkdir($record_dir);
    }

    # setting apk's infomation
    $info_file = "$record_dir/info";
    if($is_file($info_file)) {
        $info = parse_ini_file($info_file)["info"];
    } else {
        $info = array(
            "package" => $package,
            "key"     => $key,

        );
    }

    
#    $record_path = "/var/www/records/" . $package . "-" . $ip . "-" . $id;
#    $file = fopen($record_path, "w+");
#    fprintf($file, "%s\t%s\t%s\t%s\n", date('Y-m-d H:i:s'), $package, $id, $ip);
#    fprintf($file, "%s\n", $data);
}



function write_php_ini($array, $file)
{
    $res = array();
    foreach($array as $key => $val)
    {
        if(is_array($val))
        {
            $res[] = "[$key]";
            foreach($val as $skey => $sval) $res[] = "$skey = ".(is_numeric($sval) ? $sval : '"'.$sval.'"');
        }
        else $res[] = "$key = ".(is_numeric($val) ? $val : '"'.$val.'"');
    }
    safefilerewrite($file, implode("\r\n", $res));
}

function safefilerewrite($fileName, $dataToSave)
{    if ($fp = fopen($fileName, 'w'))
    {
        $startTime = microtime(TRUE);
        do
        {            $canWrite = flock($fp, LOCK_EX);
           // If lock not obtained sleep for 0 - 100 milliseconds, to avoid collision and CPU load
           if(!$canWrite) usleep(round(rand(0, 100)*1000));
        } while ((!$canWrite)and((microtime(TRUE)-$startTime) < 5));

        //file was locked so now we can store information
        if ($canWrite)
        {            fwrite($fp, $dataToSave);
            flock($fp, LOCK_UN);
        }
        fclose($fp);
    }

}


?>

