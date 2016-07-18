<!DOCTYPE html>
<html>
<head>
<?php
	include("common.php");
	include("apks.php");
    htmlheads("Customization");
?>
    <script type="text/javascript" src="lib/jquery.min.js.js"></script>
    <script type="text/javascript" src="lib/sha1.js"></script>
    <script type="text/javascript" src="lib/user.js"></script>

<?php
	$data = false;

	ini_set("display_errors", 1);
    ini_set("error_reporting", E_ALL | E_STRICT);

	// this is a demonstrator function, which gets called when new users register
	function registration_callback($username, $email, $userdir) {
		// all it does is bind registration data in a global array,
		// which is echoed on the page after a registration
		global $data;
		$data = array($username, $email, $userdir);
	}

    require_once("user.php");
    global $USER;
    $USER = new User();


    global $task;
    $task = "index";

    if (isset($_GET["task"])) {
        switch($_GET["task"]) {
            case "login" : $task = "login"; break;
            case "regist" : $task = "regist"; break;
        }
    }


    if( isset($_POST["upload"]) ) {
        $task = "upload";
    }



    function usermenu() {
        global $USER;
        if ($USER->authenticated) {
?>
    <p>Hello, <?php echo $USER->username; ?>.</p>
<?php } else { ?>
    <p>To get more service, <a href="upload.php?task=login">Sign in</a> your account or <a href="upload.php?task=regist">Sign up</a> a new account.</p>
<?php }
    }



    // Processing file uploading
    function process_upload() {
?>
    <h2>Upload file</h2>
<?php
        usermenu();

        $file_name = "" . time() . rand(100,999) . ".apk";
        $file_path = "uploads/$file_name";

        if ( move_uploaded_file($_FILES["file"]["tmp_name"], $file_path) ) {
            //Upload successfully
            // Set the parameter of the uploaded file
            $hash = hash_file( "md5", $file_path );

            $uploader = $USER->username;

            if ( isset($_POST["type"]) )
                $type = $_POST["type"];
            else
                $type = "local";
?>
    <p>
        Upload successfully.<br />
        File name: <?php echo $file_name; ?><br />
        File hash: <?php echo $hash; ?><br />
        Uploader: <?php echo $USER->username; ?><br />
        Patch type: <?echo $type; ?>
    </p>
<?php
            // wtire into database
            $db = new PDO("sqlite:./codroid.db");
            $insert = "INSERT INTO uploads (file_name, file_hash, uploader, modify_type, upload_time) ";
            $insert .= "VALUES ('$file_name', '$hash', '$uploader', '$type', datetime('now', 'localtime'))";
            
            if ( !$db->exec($insert) ) {
?>
    <p>
        Database writting failed.<br />
        Please contact manager.<br />
<?php
                //echo implode(" ", $db->errorInfo());
            }
                
            // handle exception
        } else {
?>
    <p>
        Upload failed.<br />
        An error occurred.
    </p>
<?php
        }
    }
    // END OF process_upload();

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



<!-- Default page: uploading form -->
<?php
    if($task == "index") { ?>
    <h2>Create customized packge for online/offline testing</h2>

    <!-- User menu -->
    <?php usermenu(); ?>

    <form enctype="multipart/form-data" class="form-horizontal" role="form" method="post" action="upload.php">
        <div class="form-group">
          	    <div class="col-sm-10">
        			<div class="input-group">
        			    <i class="input-group-addon fa fa-file-code-o"></i>
        			    <input name="file" class="form-control" type="file" />
        			    <span class="input-group-addon">.apk</span>
        		    </div>
        	    </div>
        	    <div class="col-sm-2">
        		    <input class="form-control" type="submit" />
        	    </div>
            </div><!-- form-group -->
            <div class="form-group">
        	    <label class="control-label col-sm-1" for="type">Type:</label>
        	    <div class="col-sm-3">
        	    <select id="type" name="type" class="form-control">
                    <option value="local" selected>Local (Offline)</option>
<?php               if ($USER->authenticated) { ?>
                    <option value="remote">Remote (Online)</option>
<?php               } ?>
                </select>
        	    </div>
            </div><!-- form-group -->
            <input name="upload" type="hidden" value="1" />
            <p>* You can only make a local-patching apk if you do not sign in our site. Try to <a href="upload.php?task=login">sign in</a> or <a href="upload.php?task=regist">sign up</a> to get a remote-patching apk!</p>
            </form>
<?php 
    } ?>



<!-- Uploading page -->
<?php
    if($task == "upload") { process_upload(); } 
?>



<?php
    if($task == "regist") { ?>

    <h2>Sign up to CoDroid.</h2>

    <div class="container">
        <form class="form-horizontal col-sm-6" role="form" id="registration" method="post" action="upload.php">
            <div class="form-group">
                <label for="username">Username:</label>
                <input type="text" class="form-control" id="username" onblur="User.validName(document.getElementById('registration')['username'])" />
                some message.
            </div>
            <div class="form-group">
                <label for="password1">Password:</label>
                <input type="password" class="form-control" id="password1" />
            </div>
            <div class="form-group">
                <label for="password2">Confirm password:</label>
                <input type="password" class="form-control" id="password2" />
            </div>
            <div class="form-group">
                <label for="mail">Email:</label>
                <input type="email" class="form-control" id="email" />
            </div>
            <input type="button" class="btn btn-primary" value="Sign up" onClick="User.processRegistration()" />
        </form>
    </div>
<?php
    } ?>





<!-- Sign in page -->
<?php
    if($task == "login") { ?>

    <h2>Sign in to CoDroid.</h2>

    <form class="form-inline" role="form" method="post" action="upload.php">
        <div class="form-group">
            <label class="sr-only" for="username">Username</label>
            <input type="email" class="form-control" id="username" placeholder="Username" />
        <div class="form-group">
            <label class="sr-only" for="password">Password</label>
            <input type="password" class="form-control" id="password" placeholder="Password" />
        </div>
        <div class="checkbox">
            <label>
                <input type="checkbox"> Remember me
            </label>
        </div>
        <button type="submit" class="btn btn-primary">Sign in</button>
    </form>
<?php
    } ?>




	<?php footer(); ?>

</div> <!-- /container -->

</body>
</html>
