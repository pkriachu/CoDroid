<!DOCTYPE html>
<html>
<head>
<?php
	include("common.php");
    htmlheads("Customization");
?>
    <script type="text/javascript" src="lib/jquery.min.js"></script>
    <script type="text/javascript" src="lib/sha1.js"></script>
    <script type="text/javascript" src="lib/user.js"></script>

<?php
	ini_set("display_errors", 1);
    ini_set("error_reporting", E_ALL | E_STRICT);

    require_once("user.php");
    $USER = new User();


    $codroid_db = "../codroid.db";


    global $task;
    $task = "index";

    if(isset($_GET["id"])) {
        $task = "query";
/*
        if(isset($_GET["task"])) {
            switch($_GET["task"]) {
                case "login" : $task = "login"; break;
                case "logout" : if($USER->authenticated) {$task = "logout";} break;
                case "regist" : $task = "regist"; break;
            }
        }
        if(isset($_POST["task"])) {
            switch($_POST["task"]) {
                case "dologin" : $task = "dologin"; break;
                case "dologout" : $task = "dologout"; break;
                case "doregist" : $task = "doregist"; break;
            }
        }
 */
    }


    function usermenu() {
        global $USER;
?>
    <p><a href="customization.php">Upload APK</a> | <a href="view.php">View APKs</a></p>
<?php
        if ($USER->authenticated) {
?>
    <p>Hello, <?php echo $USER->username; ?>. | <a href="#">View your apks</a> | <a href="#">View your online reports</a> | <a href="customization.php?task=logout">Sign out</a></p>
<?php } else { ?>
    <p>To get more service, <a href="customization.php?task=login">Sign in</a> your account or <a href="customization.php?task=regist">Sign up</a> a new account.</p>
<?php }
    }
?>

<style>
a {
	text-decoration: none;
}
a:hover {
	text-decoration: none;
}
a:visited {
	text-decoration: none;
}
</style>
</head>
<body>

<div class="container">

	<?php navbar("customization"); ?>



<!-- Default page: List recent apks -->
<?php
    if($task == "index") { ?>
    <h2>Patch History</h2>

    <!-- User menu -->
    <?php usermenu(); ?>


        <div class="col-sm-12">
<?php
        $db = new PDO("sqlite:$codroid_db");

        $uploads = 0;
        # checking unpatched files
        $query = "SELECT Count(*) FROM uploads";
        $result = $db->query($query);
        $row = $result->fetchALL();
        $uploads = $row[0][0];
?>
            <h3><?php echo $uploads; ?> file(s) in the processing queue now.</h3>

            <h3>Recent Patched files</h3>
            <ul>
<?php
        $apks = array();
        # get the infomation of patched apks
        $query = "SELECT * FROM apks ORDER BY upload_time DESC LIMIT 20";
        $result = $db->query($query);
        if($result) {
            $row = $result->fetchALL();

            # print the newest rows
            $bound = 5;
            if(sizeof($row) < $bound) $bound = sizeof($row);
            for($i=0; $i<$bound; $i++) {
                $patched_apk = "downloads/" . $row[$i]['id'] . ".patched.apk";
                $metafile = "downloads/" . $row[$i]['id'] . ".meta";

                $type = "[" . strtoupper($row[$i]['type']) . "]";
?>
                <li><a href="?id=<?php echo $row[$i]['id']; ?>"><?php echo $type; ?> <?php echo $row[$i]['apk_id'];?>, uploading by <?php echo $row[$i]['uploader'] ?> ,at <?php echo $row[$i]['upload_time'];?></a></li>
<?php
            }
        } else {
?>
                <li>No files patched.</li>
<?php   }
?>
            <i/ul>

        </div>
<?php 
    } ?>
<!-- End of index page -->



<!-- Query page: query apk infomation by input id -->
<?php
    if($task == "query") { ?>
    <h2>View APK Infomation</h2>

    <!-- User menu -->
    <?php usermenu(); ?>


        <div class="col-sm-12">
<?php
        $file_id = $_GET["id"];
        $status = "no-match";
        $db = new PDO("sqlite:$codroid_db");

        # getting info by id
        # not patching yet
        $query = "SELECT * FROM uploads WHERE id = $file_id;";
        $result = $db->query($query);
        $row = $result->fetchALL();
        if($row) { 
            $status = "on-processing";
?>
            <h3>This APK is not patched yet. Please wait for a while.</h3>
<?php   }
        # patched
        $query = "SELECT * FROM apks WHERE id = $file_id";
        $result = $db->query($query);
        $row = $result->fetchALL();
        if($row) {
            $status = "patched";

            $apk = $row[0];

            # generate download link
            $patched_apk = "downloads/" . $apk['id'] . ".patched.apk";
            $metafile = "downloads/" . $apk['id'] . ".meta";
            $download_link = "";
            switch($apk['type']) {
            case "local" :
                # local patch files are available to everyone
                if(file_exists($patched_apk)) {
                    $download_link .= "<a href='$patched_apk'>Download APK</a>";
                    $download_link .= ", <a href='$metafile'>Metafile</a>";
                }
                break;
            case "remote" :
                # remote patch files are only provide to theri owner
                if(file_exists($patched_apk) && $USER->authenticated && $apk["uploader"]==$USER->username) {
                    $download_link .= "<a href='$patched_apk'>Download APK</a>";
                    $download_link .= ", <a href='$metafile'>Metafile</a>";
                }
                break;
            }
?>

            <h3>Apk Id: <?php echo $apk["apk_id"]; ?></h3>
            <ul>
                <li>Apk version: <?php echo $apk["apk_version"]; ?></li>
                <li>File hash: <?php echo $apk["apk_hash"]; ?></li>
                <li>Uploader: <?php echo $apk["uploader"]; ?></li>
                <li>Patch type: <?php echo $apk["type"]; ?></li>
                <li>Upload time: <?php echo $apk["upload_time"]; ?></li>
                <li>Process time: <?php echo $apk["process_time"]; ?></li>
            </ul>
            <p><?php echo $download_link;?></p>
<?php
            if($apk["type"] == "remote") { ?>
            <p>* Remote-patching APK downloading is only for registed users.</p>

<?php       }
        }
        if($status === "no-match") { ?>
            <script type="text/javascript">
                $(document).ready(function() {
                    location.href = "view.php";
                });
            </script>
<?php   }
    } ?>
<!-- End of index page -->




    <p>&nbsp;</p>
	<?php footer(); ?>

</div> <!-- /container -->

</body>
</html>
