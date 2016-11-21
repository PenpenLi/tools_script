<?php
include_once 'helper.php';
include_once 'gamedata.php';

$registerInfo = helper_receiveMsg();
$userId = $registerInfo['userId'];
$pw = $registerInfo['pw'];

$gameData = new GameData();
$userInfo = $gameData->getUserInfo($userId);
if (!$userInfo) {
	helper_sendMsg(array ('ret' => 'user_not_exist'));
	exit();
}

if ($userInfo['PW'] != $pw) {
	helper_sendMsg(array ('ret' => 'pw_not_right'));
	exit();
}

$reward = '';

// update continual login times
$loginDay = intval(date("w"));
if ($loginDay == 0) {$loginDay = 7;}
if ($userInfo['LOGINSTAMP'] != $loginDay) {
	$reward = $gameData->getReward($userId);
	$loginTimes = $userInfo['LOGINTIMES'];
	if ($loginDay == $userInfo['LOGINSTAMP'] + 1) {
		$loginTimes = $loginTimes + 1;
	} else {
		$loginTimes = 1;
	}
	$gameData->updateLoginInfo($userId, $loginDay, $loginTimes);
}

$scoreRank = $gameData->getScoreRank($userId);
$loginRank = $gameData->getLoginRank($userId);

session_start();
$token = session_id();
$_SESSION['userId'] = $userId;


//helper_log("[login]_SESSION: " . $_SESSION['userId']);

$retMsg = array(
	'token' => $token,
	'ret' => 'ok',
	'scoreRank' => $scoreRank,
	'loginRank' => $loginRank,
	'reward' => $reward
);

helper_sendMsg($retMsg);

?>
