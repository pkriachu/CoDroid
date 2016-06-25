<?php
	function navbar_active($current, $tag) {
		if(strtolower($current) == $tag)
			return ' class="active"';
		return '';
	}

	function navbar($active = "home") {
?>
	<div class="masthead">
		<h3 class="text-muted">CoDroid: Measure Code Coverage for Android Dynamic Analysis Tools</h3>
		<nav>
			<ul class="nav nav-justified">
			<li<?php echo navbar_active($active, "home"); ?>><a href="index.php">Home</a></li>
			<li<?php echo navbar_active($active, "perf"); ?>><a href="measurement.php">Measurements</a></li>
			<li<?php echo navbar_active($active, "docs"); ?>><a href="docs.php">Documents</a></li>
			<li<?php echo navbar_active($active, "downloads"); ?>><a href="downloads.php">Downloads</a></li>
			<li<?php echo navbar_active($active, "customization"); ?>><a href="customization.php">Customization</a></li>
			<li<?php echo navbar_active($active, "contact"); ?>><a href="contact.php">Contact</a></li>
			</ul>
		</nav>
	</div>
<?php
	}

	function footer() {
?>
<footer class="footer">
<p>&copy; 2015&mdash;2016 Security and Systems Laboratory, National Chiao Tung University.</p>
</footer>
<script>
  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
  })(window,document,'script','//www.google-analytics.com/analytics.js','ga');
  ga('create', 'UA-71969359-1', 'auto');
  ga('send', 'pageview');
</script>
<?php
	}

	function htmlheads($title = "CoDroid") {
?>
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" />
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<!-- jquery -->
<script src="lib/jquery.min.js"></script>
<!-- font-awesome -->
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.5.0/css/font-awesome.min.css">
<!-- bootstrap -->
<link rel="stylesheet" href="lib/bootstrap/css/bootstrap.min.css" />
<link rel="stylesheet" href="lib/bootstrap/css/bootstrap-theme.min.css" />
<script src="lib/bootstrap/js/bootstrap.min.js"></script>
<!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
<link href="lib/ie10-viewport-bug-workaround.css" rel="stylesheet">
<!-- Custom styles for this template -->
<link href="lib/justified-nav.css" rel="stylesheet">
<!-- Just for debugging purposes. Don't actually copy these 2 lines! -->
<!--[if lt IE 9]><script src="../../assets/js/ie8-responsive-file-warning.js"></script><![endif]-->
<script src="lib/ie-emulation-modes-warning.js"></script>
<!--[if lt IE 9]>
<script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
<script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
<![endif]-->
<script src="lib/ie10-viewport-bug-workaround.js"></script>
<title><?php echo $title; ?></title>
<?php
	}

?>
