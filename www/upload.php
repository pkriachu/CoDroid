<!DOCTYPE html>
<html>
<head>
<?php
	include("common.php");
	include("apks.php");
	htmlheads("Downloads");
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

	<?php navbar("downloads"); ?>

	<hr/>
	<ul class="nav nav-pills">
	<li role="presentation"><a href="#prebuilt">Pre-built</a></li>
	<li role="presentation"><a href="#customized">Customization</a></li>
	</ul>
	<hr/>

	<a name="prebuilt"></a>
	<h2>Pre-built packages for offline (local) testing</h2>

	<div class="container">
	<?php
		for($i = 0; $i < sizeof($apks); $i++) {
			if($i % 3 == 0) {
				echo '<div class="row">';
			}
			$baseurl = "http://codroid.org/repo";
			$cat = $apks[$i]["cat"];
			$id = $apks[$i]["id"];
			$name = $apks[$i]["name"];
			$icon = $apks[$i]["icon"];
			$apk = $apks[$i]["apks"];
			$pkg = $apks[$i]["repackaged"];
			$pat = $apks[$i]["patched"];
			$meta = $apks[$i]["meta"];

			$pkgurl = "Repackaged";
			$pkgicon = "fa-ban text-danger";
			if($pkg != FALSE) {
				$pkgicon = "fa-copy text-primary";
				$pkgurl = '<a href="'."$baseurl/repackaged/$cat/$pkg".'">Repackaged</a>';
			}

			$paticon = "fa-ban text-danger";
			$patinfo = "Patched";
			if($pat != FALSE && $meta != FALSE) {
				$paticon = "fa-check-square-o text-success";
				$patinfo = '<a href="'."$baseurl/patched/$cat/$pat".'">Patched</a> &nbsp; &nbsp; ';
				$patinfo .= '<span class="fa fa-file-text-o"></span> ';
				$patinfo .= '<a href="'."$baseurl/meta/$cat/$meta".'">Metadata</a>';
			}
?>
<div class="col-sm-4">
<div class="media">
<div class="media-left">
<img src="<?= "$baseurl/icons/$cat/$icon"; ?>" width="64" height="64"/>
</div>
<div class="media-body">
<strong><?= $name; ?></strong><br/>
<?= $id; ?><br/>
<span class="fa fa-cube"></span> <a href="<?= "$baseurl/apks/$cat/$apk"; ?>">Original</a><br/>
<span class="fa <?= $pkgicon; ?>"></span> <?= $pkgurl; ?><br/>
<span class="fa <?= $paticon; ?>"></span> <?= $patinfo; ?><br/><br/>
</div>
</div>
</div>
<?php
			if($i % 3 == 2) {
				echo '</div>';
			}
		}
		if($i % 3 != 0) {
			echo '</div>';
		}
	?>
	</div>

	<a name="customized"></a>
	<h2>Create customized packge for online/offline testing</h2>
	<p>Coming soon ...</p>

	<form class="form-horizontal" role="form">
	<div class="form-group">
		<div class="col-sm-10">
			<div class="input-group">
				<i class="input-group-addon fa fa-file-code-o"></i>
				<input name="file" class="form-control" type="file" disabled/>
				<span class="input-group-addon">.apk</span>
			</div>
		</div>
		<div class="col-sm-2">
			<input class="form-control" type="submit" disabled/>
		</div>
	</div><!-- form-group -->
	<div class="form-group">
		<label class="control-label col-sm-1" for="type">Type:</label>
		<div class="col-sm-3">
		<select id="type" name="type" class="form-control" disabled>
			<option selected>Local (Offline)</option>
			<option>Remote (Online)</option>
		</select>
		</div>
	</div><!-- form-group -->
	</form>

	<?php footer(); ?>

</div> <!-- /container -->

</body>
</html>
