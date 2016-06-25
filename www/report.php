<?php require("env.php"); ?>
<?php
    if ( is_null($_GET["version_e"]) )
    {
        $EMULATOR_VER = "2.3.3";
    }
    else
    {
        $EMULATOR_VER = $_GET["version_e"];
    }

    $ERROR_PAGE = false;
    if ( is_null($_GET["ID"]) ) {
        $ERROR_PAGE = true;
    }

    $ID = $_GET["ID"];
    
    $PAGE_DL = "";
    if ( file_exists("uploads/$ID.apk") ) {
        $PAGE_DL .= "<a href=\"uploads/$ID.apk\">Original APK file (local output)</a><br />";
    }
    if ( file_exists("repacked/$ID.repacked.apk") ) {
        $PAGE_DL .= "<a href=\"repacked/$ID.repacked.apk\">Repackaged APK file (local output)</a><br />";
    } else {
        $PAGE_DL = "Repackaging has not been completed yet. Please try again later.";
    }
    if ( file_exists("repacked/$ID.meta") ) {
        $PAGE_DL .= "<a href=\"repacked/$ID.meta\">Codroid meta file</a><br />";
    }

?>
<?php #require("parse-report.php"); ?>


<!DOCTYPE  html>
<html>
    <head>
        <meta charset="utf-8">
        <title>Codroid Test</title>

        <!-- CSS -->
        <link rel="stylesheet" href="css/style.css" type="text/css" media="screen" />
        <link rel="stylesheet" href="css/social-icons.css" type="text/css" media="screen" />
        <!--[if IE 8]>
            <link rel="stylesheet" type="text/css" media="screen" href="css/ie8-hacks.css" />
        <![endif]-->
        <!-- ENDS CSS -->    

        <!-- GOOGLE FONTS 
        <link href='http://fonts.googleapis.com/css?family=Ubuntu' rel='stylesheet' type='text/css'>-->

        <!-- JS -->
        <script type="text/javascript" src="js/jquery-1.5.1.min.js"></script>
        <script type="text/javascript" src="js/jquery-ui-1.8.13.custom.min.js"></script>
        <script type="text/javascript" src="js/easing.js"></script>
        <script type="text/javascript" src="js/jquery.scrollTo-1.4.2-min.js"></script>
        <script type="text/javascript" src="js/jquery.cycle.all.js"></script>
        <script type="text/javascript" src="js/custom.js"></script>

        <!-- Isotope -->
        <script src="js/jquery.isotope.min.js"></script>

        <!--[if IE]>
            <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
        <![endif]-->

        <!--[if IE 6]>
            <script type="text/javascript" src="js/DD_belatedPNG.js"></script>
            <script>
                  /* EXAMPLE */
                  //DD_belatedPNG.fix('*');
            </script>
        <![endif]-->
        <!-- ENDS JS -->


        <!-- Nivo slider -->
        <link rel="stylesheet" href="css/nivo-slider.css" type="text/css" media="screen" />
        <script src="js/nivo-slider/jquery.nivo.slider.js" type="text/javascript"></script>
        <!-- ENDS Nivo slider -->

        <!-- tabs -->
        <link rel="stylesheet" href="css/tabs.css" type="text/css" media="screen" />
        <script type="text/javascript" src="js/tabs.js"></script>
          <!-- ENDS tabs -->

          <!-- prettyPhoto -->
        <script type="text/javascript" src="js/prettyPhoto/js/jquery.prettyPhoto.js"></script>
        <link rel="stylesheet" href="js/prettyPhoto/css/prettyPhoto.css" type="text/css" media="screen" />
        <!-- ENDS prettyPhoto -->

        <!-- superfish -->
        <link rel="stylesheet" media="screen" href="css/superfish.css" /> 
        <link rel="stylesheet" media="screen" href="css/superfish-left.css" /> 
        <script type="text/javascript" src="js/superfish-1.4.8/js/hoverIntent.js"></script>
        <script type="text/javascript" src="js/superfish-1.4.8/js/superfish.js"></script>
        <script type="text/javascript" src="js/superfish-1.4.8/js/supersubs.js"></script>
        <!-- ENDS superfish -->

        <!-- poshytip -->
        <link rel="stylesheet" href="js/poshytip-1.0/src/tip-twitter/tip-twitter.css" type="text/css" />
        <link rel="stylesheet" href="js/poshytip-1.0/src/tip-yellowsimple/tip-yellowsimple.css" type="text/css" />
        <script type="text/javascript" src="js/poshytip-1.0/src/jquery.poshytip.min.js"></script>
        <!-- ENDS poshytip -->

        <!-- Tweet -->
        <link rel="stylesheet" href="css/jquery.tweet.css" media="all"  type="text/css"/> 
        <script src="js/tweet/jquery.tweet.js" type="text/javascript"></script> 
        <!-- ENDS Tweet -->

        <!-- Fancybox -->
        <link rel="stylesheet" href="js/jquery.fancybox-1.3.4/fancybox/jquery.fancybox-1.3.4.css" type="text/css" media="screen" />
        <script type="text/javascript" src="js/jquery.fancybox-1.3.4/fancybox/jquery.fancybox-1.3.4.pack.js"></script>
        <!-- ENDS Fancybox -->



    </head>

    <body class="home">

            <!-- HEADER -->
            <div id="header">
                <!-- wrapper-header -->
                <div class="wrapper">
                    <a href="index.html"></a>
                    <!-- search -->
                    <div class="top-search">
                        <form  method="get" id="searchform" action="#">
                            <div>
                                <input type="text" value="Search..." name="s" id="s" onfocus="defaultInput(this)" onblur="clearInput(this)" />
                                <input type="submit" id="searchsubmit" value=" " />
                            </div>
                        </form>
                    </div>
                    <!-- ENDS search -->
                </div>
                <!-- ENDS wrapper-header -->                    
            </div>
            <!-- ENDS HEADER -->


            <!-- Menu -->
            <div id="menu">



                <!-- ENDS menu-holder -->
                <div id="menu-holder">
                    <!-- wrapper-menu -->
                    <div class="wrapper">
                        <!-- Navigation -->
                        <ul id="nav" class="sf-menu">
                            <li><a href="index.html">Upload<span class="subheader">your APK</span></a></li>
                            <li class="current-menu-item"><a href="report.php">Download<span class="subheader">Repackaged APKs</span></a></li>
                            <li><a href="contact.html">Contact<span class="subheader">Get in touch</span></a></li>
                        </ul>
                        <!-- Navigation -->
                    </div>
                    <!-- wrapper-menu -->
                </div>
                <!-- ENDS menu-holder -->
            </div>
            <!-- ENDS Menu -->





            <!-- MAIN -->
            <div id="main">
                <!-- wrapper-main -->
                <div class="wrapper">

                    <form id="form1" enctype="multipart/form-data" method="post" action="upload.php">
                        <input type="file" name="fileToUpload" id="fileToUpload" style="display:none;" onchange="fileSelected();" />
                    <!-- headline -->
                    <div class="clear"></div>
                    <div id="headline">
                        <span class="main">Download Repackaged APKs</span>
                        <span class="sub"></span>
                    </div>
                    <!-- ENDS headline -->
                    </form>

                    <!-- content -->
                    <div id="content">

                            <!-- TABS -->
                            <!-- the tabs -->
                            <ul class="tabs">
                                <li><a href="#"><span>Infomation</span></a></li>
                                <li><a href="#"><span>File Download</span></a></li>
                            </ul>

                            <!-- tab "panes" -->
                            <div class="panes">

                                <!-- Infomation -->
                                <div>
                                    <div class="plain-text">
                                        <h6>APK Infomation</h6>
                                        <p>No infomation.</p>
                                    </div>
                                </div>
                                <!-- ENDS Infomation -->

                                <!-- Files -->
                                <div>
                                    <div class="plain-text">
                                        <h6>Download Repackaged File</h6>
                                        <p><?php echo $PAGE_DL; ?></p>
                                    </div>
                                </div>
                                <!-- ENDS Files -->

                            </div>
                            <!-- ENDS TABS -->



                    </div>
                    <!-- ENDS content -->
                </div>
                <!-- ENDS wrapper-main -->
            </div>
            <!-- ENDS MAIN -->



            <!-- Bottom -->
            <div id="bottom">
                <!-- wrapper-bottom -->
                <div class="wrapper">
                    <div id="bottom-text">copyright. <a href="#"> copyright</a> </div>
                    <!-- Social -->
                    <ul class="social ">
                        <li><a href="http://www.ntou.edu.tw" class="poshytip  ntou" title="National Taiwan Ocean Univisity"></a></li>
                    </ul>
                    <!-- ENDS Social -->
                    <div id="to-top" class="poshytip" title="To top"></div>
                </div>
                <!-- ENDS wrapper-bottom -->
            </div>
            <!-- ENDS Bottom -->

    </body>
</html>