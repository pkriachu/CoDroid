<?php
    require 'vendor/autoload.php';
?>

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
        
        
        <link rel="stylesheet" href="css/index.css" type="text/css" media="screen" />

    </head>
    
    <body class="home">

            <!-- HEADER -->
            <div id="header">
                <!-- wrapper-header -->
                <div class="wrapper">
                    <!--<a href="index.html"><img id="logo" src="img/logo.png" alt="Android Behavior Monitor" /></a>-->
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
                            <li class="current-menu-item"><a href="index.html">Upload<span class="subheader">your APK</span></a></li>
                            <li><a href="report.php">Download<span class="subheader">Repackaged APKs</span></a></li>
                            <li><a href="#">Contact<span class="subheader">Get in touch</span></a></li>
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
                    
                    <script type="text/javascript" src="js/index.js"></script>
                    <form id="uploadForm" enctype="multipart/form-data" method="post" action="upload.php">
                        <input type="file" name="fileToUpload" id="fileToUpload" style="display:none;" onchange="fileSelected();" />
                        <input type="submit" name="startUpload" id="startUpload" style="display:none;" />
                    <!-- headline -->
                    <div class="clear"></div>
                    <div id="headline">
                        <span class="main">Upload your APK</span>
                        <span class="sub">（格式說明等等）</span>
                    </div>
                    <div id="uploadwindow">
                        <input name="filename" id="filename" type="text" class="form-poshytip" readonly="readonly" />
                        <div class="button">
                            <a href="#" id="select" class="link-button" onclick="preSelect();"><span>Select</span></a>
                        </div>
                        <div class="button">
                            <a href="#" id="upload" class="link-button-locked"><span>Upload</span></a>
                        </div>
                    </div>
                    <!-- ENDS headline -->
                    </form>
                    
                </div>
                <!-- ENDS wrapper-main -->
            </div>
            <!-- ENDS MAIN -->
            

            
            <!-- FOOTER -->
            <div id="footer">
                <!-- wrapper-footer -->
                <div class="wrapper">
                    <!-- footer-cols -->
                    <ul id="footer-cols">
                        <li class="col">
                            <h6>Pages</h6>
                            <ul>
                                <li class="page_item"><a href="index.html">Upload</a></li>
                                <li class="page_item"><a href="report.php">Report</a></li>
                                <li class="page_item"><a href="#">Contact</a></li>
                            </ul>
                        </li>
                        
                        <li class="col">
                            <h6>_</h6>
                            <ul>
                                <li><a href="#/">_</a></li>
                            </ul>
                        </li>
                        <li class="col">
                            <h6>-</h6>
                            -
                        </li>
                        
                    </ul>
                    <!-- ENDS footer-cols -->
                </div>
                <!-- ENDS wrapper-footer -->
            </div>
            <!-- ENDS FOOTER -->
        
        
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
