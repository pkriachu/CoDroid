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


    global $task;
    $task = "index";

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
    if( isset($_POST["upload"]) ) {
        $task = "upload";
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



<!-- Default page: uploading form -->
<?php
    if($task == "index") { ?>
    <h2>Create customized packge for online/offline testing(<?php echo $USER->username; ?>)</h2>

    <!-- User menu -->
    <?php usermenu(); ?>

        <form enctype="multipart/form-data" class="form-horizontal" role="form" method="post" action="customization.php">
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
<?php   if ($USER->authenticated) { ?>
                        <option value="remote">Remote (Online)</option>
<?php   } ?>
                    </select>
                </div>
            </div><!-- form-group -->
            <input name="upload" type="hidden" value="1" />
<?php   if(!$USER->authenticated) { ?>
            <p>* You can only make a local-patching apk if you do not sign in our site. Try to <a href="customization.php?task=login">sign in</a> or <a href="customization.php?task=regist">sign up</a> to get a remote-patching apk!</p>
<?php   } ?>
        </form>
<?php 
    } ?>
<!-- End of index page -->



<!-- Uploading page -->
<?php
    if($task == "upload") { 
    // Processing file uploading
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
        Patch type: <?php echo $type; ?>
    </p>
<?php
            // wtire into database
            $db = new PDO("sqlite:db/codroid.db");
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
<!-- End of upload page -->



<!-- Sign up page -->
<?php
    if($task == "regist") { ?>

    <h2>Sign up to CoDroid.</h2>

    <div class="col-sm-12">
        <form class="form-horizontal col-sm-6" role="form" id="registration" method="post" action="customization.php">
            <input type="hidden" name="task" value="doregist" />
            <input type="hidden" name="op" value="register" />
            <input type="hidden" name="sha1" value="" />
            <div class="form-group">
                <label for="username">Username:</label>
                <input type="text" class="form-control" id="username" name="username" onblur="User.validName(document.getElementById('registration')['username'])" />
            </div>
            <div class="form-group">
                <label for="password1">Password:</label>
                <input type="password" class="form-control" id="password1" name="password1" onblur="User.strongPassword(document.getElementById('registration')['password1'])" />
            </div>
            <div class="form-group">
                <label for="password2">Confirm password:</label>
                <input type="password" class="form-control" id="password2" name="password2" onblur="User.passwordMatch(document.getElementById('registration')['password1'], document.getElementById('registration')['password2'])" />
            </div>
            <div class="form-group">
                <label for="mail">Email:</label>
                <input type="email" class="form-control" id="email" name="email" onblur="User.validEmail(document.getElementById('registration')['email'])" />
            </div>
            <input type="button" class="btn btn-primary" id="process" value="Sign up" onClick="User.processRegistration()" />
        </form>
    </div>
    <script type="text/javascript">
        $('#registration').keypress(function(e) {
            if(e.which == 13) {
                jQuery(this).blur();
                jQuery('#process').focus().click();
            }
        });
    </script>
<?php
    } 

    if($task == "doregist") { ?>
    <h2>Sign up to CoDroid.</h2>

    <div class="col-sm-12">
<?php   if($USER->error=="") { ?>
        <p>Congradtulationis! You have been sign up to CoDroid. Now you can use the remote-patching mode and see the coverage reports on CoDorid. <a href="customization.php?task=login">Sign in and try it now!</a>
    <script type="text/javascript">
        $(document).ready(function() {
            window.setTimeout(function () {
                location.href = "customization.php?task=login";
            }, 10000);
        });
    </script>
<?php   } else { ?>
    <p><?php echo(join($USER->info_log)); ?></p>
    <p><?php echo($USER->error); ?></p>
    <p>Please check all the data is correct, and back to the <a href="customization.php?task=regist">sign up page</a> to try again.</p>
    <script type="text/javascript">
        $(document).ready(function() {
            window.setTimeout(function () {
                location.href = "customization.php?task=regist";
            }, 10000);
        });
    </script>
<?php   }
    } ?>
<!-- End of sign up page -->



<!-- Sign in page -->
<?php
    if($task == "login") { ?>

    <h2>Sign in to CoDroid.</h2>

    <div class="col-sm-12">
        <form class="form-horizontal col-sm-6" role="form" name="log in" id="login" method="post">
            <input type="hidden" name="task" value="dologin" />
            <input type="hidden" name="op" value="login" />
            <input type="hidden" name="sha1" value="" />
            <div class="form-group">
                <label class="sr-only" for="username">Username</label>
                <input type="text" class="form-control" id="username" name="username" placeholder="Username" onblur="User.validName(document.getElementById('login')['username'])" />
            </div>
            <div class="form-group">
                <label class="sr-only" for="password">Password</label>
                <input type="password" class="form-control" id="password1" name="password1" placeholder="Password" onblur="User.validPassword(document.getElementById('login')['password1'])" />
            </div>
            <!--
            <div class="checkbox">
                <label>
                    <input type="checkbox"> Remember me
                </label>
            </div>
            -->
            <input type="button" value="Sign in" id="process" class="btn btn-primary" onclick="User.processLogin()" />
        </form>
    </div>
    <script type="text/javascript">
        $('#login').keypress(function(e) {
            if(e.which == 13) {
                jQuery(this).blur();
                jQuery('#process').focus().click();
            }
        });
    </script>
<?php
    }

    if($task == "dologin") { ?>

    <h2>Sign in to CoDroid.</h2>
<?php
        if($USER->authenticated) { ?>
    <p>Welcome back to CoDroid, <?php echo $USER->username; ?>!</p>
    <p>It will go to the index page of customization in 10 seconds. If not, please click <a href="customization.php">here</a>.</p>
    <script type="text/javascript">
        $(document).ready(function() {
            window.setTimeout(function () {
                location.href = "customization.php";
            }, 10000);
        });
    </script>
<?php   } else { ?>
    <p><?php echo(join($USER->info_log)); ?></p>
    <p>Please check your user name and password, and back to the <a href="customization.php?task=login">sign in page</a> to try again.</p>
    <script type="text/javascript">
        $(document).ready(function() {
            window.setTimeout(function () {
                location.href = "customization.php?task=login";
            }, 10000);
        });
    </script>
<?php   }
    } ?>

<!-- End of sign in page -->



<!-- Sign out page -->
<?php
    if($task == "logout") { ?>

    <h2>Sign in to CoDroid.</h2>

    <div class="col-sm-12">
        <form class="form-horizontal col-sm-6" role="form" name="log out" id="logout" method="post">
            <input type="hidden" name="task" value="dologout" />
            <input type="hidden" name="op" value="logout" />
            <input type="hidden" name="username"value="<?php echo $USER->username; ?>" />
        </form>
    </div>
    <script type="text/javascript">
        $(document).ready(function() {
            document.getElementById('logout').submit();
        });
    </script>
<?php
    }

    if($task == "dologout") { ?>

    <h2>Sign out of CoDroid.</h2>

    <div class="container">
        <p>You hava been sign out of CoDroid.</p>
    </div>
    <script type="text/javascript">
        $(document).ready(function() {
            window.setTimeout(function () {
                location.href = "index.php";
            }, 10000);
        });
    </script>
<?php
    } ?>



    <p>&nbsp;</p>
	<?php footer(); ?>

</div> <!-- /container -->

</body>
</html>
