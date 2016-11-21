<?php
include_once 'helper.php';
include_once 'gamedata.php';

$msg = helper_receiveMsg();
$userId = $msg['userId'];
$day = $msg['day'];
$token = $msg['token'];

session_id($token);
session_start();

$islogin = false;
if (isset($_SESSION['userId'])) {
	// if ($_SESSION['userId'] == $userId) {
	// 	$islogin = true;
	// }
	$islogin = true;
}
if ($islogin == false) {
	helper_sendMsg(array ('ret' => 'user_not_login'));
	exit();
}


$gameData = new GameData();
$userInfo = $gameData->getUserInfo($userId);
if (!$userInfo) {
	helper_sendMsg(array ('ret' => 'user_not_exist'));
	exit();
}

$lastRank = $gameData->getLastDayRank_redis($userId);
helper_sendMsg(array ('ret' => 'ok', 'rank' => $lastRank));

?>