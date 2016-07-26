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
    $task = "not-login";

    if($USER->authenticated) {
        $task = "index";
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



<!-- Default page: Overview -->
<?php
    if($task == "index") { ?>
    <h2>My APKs (Overview Page)</h2>

    <!-- User menu -->
    <?php usermenu(); ?>


        <div class="col-sm-12">
<?php
        $username = $USER->username;
        $db = new PDO("sqlite:$codroid_db");

        $uploads = 0;
        # checking unpatched files
        $query = "SELECT Count(*) FROM uploads WHERE uploader='$username'";
        $result = $db->query($query);
        $row = $result->fetchALL();
        $uploads = $row[0][0];
?>
            <h3>You have <?php echo $uploads; ?> files in the processing queue.</h3>

            <h3>Patched files</h3>
            <ul>
<?php
        $apks = array();
        # get the infomation of patched apks
        $query = "SELECT * FROM apks WHERE uploader='$username' ORDER BY upload_time DESC";
        $result = $db->query($query);
        if($result) {
            $row = $result->fetchALL();

            # print the newest rows
            $bound = 5;
            if(sizeof($row) < $bound) $bound = sizeof($row);
            for($i=0; $i<$bound; $i++) {
                $patched_apk = "downloads/" . $row[$i]['id'] . ".patched.apk";
                $metafile = "downloads/" . $row[$i]['id'] . ".meta";
                $download_link = "";
                if(file_exists($patched_apk)) {
                    $download_link .= "<a href='$patched_apk'>Download APK</a>";
                    $download_link .= ", <a href='$metafile'>Metafile</a>";
                }
?>
                <li><?php echo $row[$i]['apk_id'];?> uploading at <?php echo $row[$i]['upload_time'];?><br />
                <?php echo $download_link;?></li>
<?php
            }
        } else {
?>
                <li>You have no files patched.</li>
<?php   }
?>
            <i/ul>

        </div>
<?php 
    } ?>
<!-- End of index page -->



<!-- Not login: redicect to login page -->
<?php
    if($task == "not-login") { ?>
    <script type="text/javascript">
        $(document).ready(function() {
            location.href = "customization.php?task=login";
        });
    </script>
<?php   } else { ?>
<?php 
    } ?>
<!-- End of index page -->




    <p>&nbsp;</p>
	<?php footer(); ?>

</div> <!-- /container -->

</body>
</html>
