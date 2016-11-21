<?php
include_once 'helper.php';
include_once 'gamedata.php';

$registerInfo = helper_receiveMsg();
$userId = $registerInfo['userId'];
$pw = $registerInfo['pw'];
$name = $registerInfo['name'];
$icon = $registerInfo['icon'];

$gameData = new GameData();
$userInfo = $gameData->getUserInfo($userId);
// not exist, create a new user
if (!$userInfo) {
	$gameData->addNewUser(array('userId' => $userId,'pw' => $pw,'name' => $name,'icon' => $icon));
} else {
	helper_sendMsg(array ('ret' => 'user_id_exist'));
	exit();
}

$scoreRank = $gameData->getScoreRank($userId);
$loginRank = $gameData->getLoginRank($userId);
$reward = $gameData->getReward($userId);

session_start();
$token = session_id();
$_SESSION['userId'] = $userId;

$retMsg = array(
	'token' => $token,
	'ret' => 'ok',
	'scoreRank' => $scoreRank,
	'loginRank' => $loginRank,
	'reward' => $reward
);

helper_sendMsg($retMsg);

?>

