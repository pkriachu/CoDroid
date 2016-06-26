<?php
	header("Content-Type: text/plain; charset=utf-8");

	function nothing() {
		echo 'nothing done.';
		die;
	}

	function filter($s) {
		return trim(preg_replace(',\.\.,', '', $s));
	}

	if(isset($_POST['package']) == FALSE)	nothing();
	if(isset($_POST['id']) == FALSE)	nothing();
	if(isset($_POST['timestamp']) == FALSE)	nothing();

	$package = filter($_POST['package']);
	$id = 0 + filter($_POST['id']);
	$ts = filter($_POST['timestamp']);

	$name = sprintf("%s_0x%x_%s", $package, $id, $ts);
	echo "$name\n";

	if(isset($_POST['data']) == FALSE)	die;

	$decoded = base64_decode($_POST['data']);
	$inflated = zlib_decode($decoded);
	file_put_contents("./dat/" . $name, $inflated);
	//file_put_contents("./dat/" . $name, $inflated);

	die;
?>
