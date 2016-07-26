<?php

ini_set("display_errors", 1);
ini_set("error_reporting", E_ALL | E_STRICT);


$codroid_db = "../codroid.db";
$info = false;

$key  = "null";
$package  = "null";
$id   = "null";
$data = "null";
$timestamp = "null";


# get remote ip
if(!empty($_SERVER['HTTP_CLIENT_IP'])){
    $ip = $_SERVER['HTTP_CLIENT_IP'];
} else if(!empty($_SERVER['HTTP_X_FORWARDED_FOR'])){
    $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
} else {
    $ip = $_SERVER['REMOTE_ADDR'];
}


# pase GET parameters
if(isset($_POST["key"]))       $key = $_POST["key"];                else die(0);
if(isset($_POST["package"]))   $package = $_POST["package"];        else die(0);
if(isset($_POST["id"]))        $id = $_POST["id"];                  else die(0);
if(isset($_POST["data"]))      $data = $_POST["data"];              else die(0);
if(isset($_POST["timestamp"])) $timestamp = $_POST["timestamp"];    else die(0);


echo "key = $key<br />";
echo "package = $package<br />";
echo "id = $id<br />";
echo "data = $data<br />";



# validate the key
$db = new PDO("sqlite:$codroid_db");
$query = "SELECT fid, salt, exp_time FROM remote_auth WHERE authcode='$key'";
$result = $db->query($query);
$row = $result->fetchALL();
if(sizeof($row) == 1) {
    $modify_ini = false;
    $valid = true;

    $now = time();
    # check expire time
    if ($now > $row[0]["exp_time"])
        die(0);

    $file_id = $row[0]["fid"];

    # if key is validated, save the records.
    $record_dir = "records/$file_id";
    if(!is_dir($record_dir)) {
        mkdir($record_dir);
    }

    # setting apk's infomation, and check the validation of records
    $info_file = "$record_dir/info";
    if(!is_file($info_file)) {
        # no info file, the first time to receive a record
        # create a new info array and save to file
        $info = array(
            "basic" => array(
                "package" => $package,
                "key"     => $key,
            ),
            "ip"      => array($id => $ip),
        );
        write_php_ini($info, $info_file);
    } else {
        # info file exists, validate the record info
        global $info;
        $info = parse_ini_file($info_file, true);
        if(strcmp($package, $info["basic"]["package"]) !== 0  ||  strcmp($key, $info["basic"]["key"]) !== 0) {
            $valid = false;
        }
        # check ip adrress and log it
        if(!array_key_exists($id, $info["ip"])) {
            $info["ip"][$id] = $ip;
            write_php_ini($info, $info_file);
        } else {
            $count = 0;
            foreach($info["ip"] as $k => $v) {
                if(strpos($k, $id) === 0) {
                    if(strcmp($v, $ip) === 0) {
                        $count = -1;
                        break;
                    } else {
                        $count += 1;
                    }
                }
            }
            # if the ip is not in list, append it with a new suffix
            if($count > 0) {
                $new_id = "$id" . "_" . "$count";
                $info["ip"][$new_id] = $ip;
                write_php_ini($info, $info_file);
            }
        }
    }


    # split recrods by runtime id, and save the ip in $info
    $rid_dir = "$record_dir/$id";
    if(!is_dir($rid_dir)) {
        mkdir($rid_dir);
    }

    # writting record to file, and log the timestamp-servertime pair.
    $record_file = "$rid_dir/$timestamp.out";
    $time_log = "$rid_dir/time_log";
    if(!is_file($record_file)) {
        file_put_contents($record_file, $data);
        file_put_contents($time_log, "$timestamp\t$now\n");
    }
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
        {
            $canWrite = flock($fp, LOCK_EX);
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

