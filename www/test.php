<?php
// Import the necessary classes
use Cartalyst\Sentinel\Native\Facades\Sentinel;

// Include the composer autoload file
require 'vendor/autoload.php';
require 'dbconfig.php';


$user = Sentinel::register(array(
    'email'    => 'john.doe@example.com',
    'password' => 'foobar',
));

echo $user;

?>

