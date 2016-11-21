<?php
include_once 'helper.php';
include_once 'gamedata.php';

$msg = helper_receiveMsg();
$userId = $msg['userId'];
$newname = $msg['name'];
$newicon = $msg['icon'];

$gameData = new GameData();
$userInfo = $gameData->getUserInfo($userId);
if (!$userInfo) {
	helper_sendMsg(array ('ret' => 'user_not_exist'));
	exit();
}
$gameData->modifyUserInfo($userId, $newname, $newicon);
helper_sendMsg(array ('ret' => 'ok'));

?>