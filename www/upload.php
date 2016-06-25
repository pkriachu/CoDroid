<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<?php
    // form submit
    #$uploadfile = "uploads/".$_FILES['fileToUpload']['name'];
    #echo iconv("utf-8", "big5", $uploadfile)
    $filename = "" . time() . rand(100,999);
    $uploadfile = "uploads/$filename.apk";

    echo "<pre>";
    if (move_uploaded_file($_FILES['fileToUpload']['tmp_name'], iconv("utf-8", "big5", $uploadfile))) {
        echo "Upload OK \n";
        echo $uploadfile."\n";
        $exec = "/home/pkriachu/androguard/digest.py '".$uploadfile."'";
        echo $exec."\n";
        $res = shell_exec($exec);
        echo $res;

#        $apk = json_decode($res);
#        foreach( $apk as $k => $v ) {
#            echo "res[".$k."] = ".$v."\n";
#        }

        chdir("uploads/");
        $exec = "/usr/bin/nohup /home/pkriachu/codroid/patch.sh $filename.apk local &";
        echo $exec."\n";
        $res = shell_exec($exec);
        echo $res;
        chdir("..");

    } else {
        echo "Upload failed \n";
    }
    #print_r($_FILES);
    echo "</pre>";
    
    header("Location: report.php?ID=$filename"); 
    exit;
?>
