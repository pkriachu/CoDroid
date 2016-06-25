<?php
// Import the necessary classes
use Illuminate\Database\Capsule\Manager as Capsule;

// Include the composer autoload file
require 'vendor/autoload.php';

// Setup a new Eloquent Capsule instance
$capsule = new Capsule;

$capsule->addConnection([
    'driver'    => 'mysql',
    'database'  => 'user.db',
    'prefix'    => '',
]);

// Set the event dispatcher used by Eloquent models... (optional)
#use Illuminate\Contracts\Events\Dispatcher;
use Illuminate\Container\Container;
#$capsule->setEventDispatcher(new Dispatcher(new Container));

// Make this Capsule instance available globally via static methods... (optional)
$capsule->setAsGlobal();

// Setup the Eloquent ORM... (optional; unless you've used setEventDispatcher())
$capsule->bootEloquent();
?>

