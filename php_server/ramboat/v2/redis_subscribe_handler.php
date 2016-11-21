<?php
include_once 'helper.php';
include_once 'db.php';

ini_set('default_socket_timeout', -1);

function userinfo_chan($msg) {

    $pubMsg = json_decode($msg, true);
    $db = new DB();
    $db->updateUser($pubMsg);
}

function subscribe_handler($redis, $chan, $msg) {
	print_r($msg."---\r\n");
	helper_log("redis submsg: " . $msg);
    switch($chan) {
        case 'userinfo_chan':
        	userinfo_chan($msg);
            break;

        case 'chan-2':
            break;

        case 'chan-3':
            break;
    }
}

$redis = new Redis();
$redis->connect('127.0.0.1', 6380);
$redis->subscribe(array('userinfo_chan'), 'subscribe_handler');


?>
