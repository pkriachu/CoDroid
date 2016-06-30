<!DOCTYPE html>
<html>
<head>
<?php
	include("common.php");
	include("apks.php");
    htmlheads("Customization");


    global $page_title, $page_content;

    if ( isset($_POST["upload"]) ) {
        process_upload();
    } else {
        generate_index_page();
    }



    function generate_index_page() {
        global $page_title, $page_content;
        $page_title = "Create customized packge for online/offline testing";
        $page_content = 
            '<form enctype="multipart/form-data" class="form-horizontal" role="form" method="post" action="upload.php">' . 
            '<div class="form-group">' .
          	    '<div class="col-sm-10">' .
        			'<div class="input-group">' .
        			    '<i class="input-group-addon fa fa-file-code-o"></i>' .
        			    '<input name="file" class="form-control" type="file" />' .
        			    '<span class="input-group-addon">.apk</span>' .
        		    '</div>' .
        	    '</div>' .
        	    '<div class="col-sm-2">' .
        		    '<input class="form-control" type="submit" />' .
        	    '</div>' .
            '</div><!-- form-group -->' .
            '<div class="form-group">' .
        	    '<label class="control-label col-sm-1" for="type">Type:</label>' .
        	    '<div class="col-sm-3">' .
        	    '<select id="type" name="type" class="form-control">' .
        		    '<option value="local" selected>Local (Offline)</option>' .
        		    '<option value="remote">Remote (Online)</option>' .
        	    '</select>' .
        	    '</div>' .
            '</div><!-- form-group -->' .
            '<input name="upload" type="hidden" value="1" />' .
            '</form>';
    }



    function process_upload() {
        global $page_title, $page_content;
        $page_title = "Upload Files";
        $page_content = "";

        $message = "";
        // Processing file uploading
        $file_name = "" . time() . rand(100,999) . ".apk";
        $file_path = "uploads/$file_name";

        if ( move_uploaded_file($_FILES["file"]["tmp_name"], $file_path) ) {
            //Upload successfully
            // Set the parameter of the uploaded file
            $hash = hash_file( "md5", $file_path );

            if ( isset($_POST["uploader"]) )
                $uploader = $_POST["uploader"];
            else
                $uploader = "Guest";

            if ( isset($_POST["type"]) )
                $type = $_POST["type"];
            else
                $type = "local";

            $message .= "Upload successfully.<br />";
            $message .= "File name: $file_name<br />";
            $message .= "File hash: $hash<br />";
            $message .= "Uploader: $uploader<br />";
            $message .= "Modify type: $type<br />";

            // wtire into database
            $db = new PDO("sqlite:./codroid.db");
            $insert = "INSERT INTO uploads (file_name, file_hash, uploader, modify_type, upload_time) ";
            $insert .= "VALUES ('$file_name', '$hash', '$uploader', '$type', datetime('now'))";
            
            if ( $db->exec($insert) )
                $message .= "Insert success.";
            else {
                $message .= "Insert faild.<br />";
                $message .= implode(" ", $db->errorInfo());
            }
                
            // handle exception
        } else {
            $message .= "An error occurred.";
        }
        $page_content = $message;
    }



    function display_content() {
        global $page_title, $page_content;
        echo "<h2>" . $page_title . "</h2>";
        echo $page_content;
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

    <?php display_content(); ?>

	<?php footer(); ?>

</div> <!-- /container -->

</body>
</html>
