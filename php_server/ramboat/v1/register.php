<?php
include_once 'helper.php';
include_once 'gamedata.php';

$registerInfo = helper_receiveMsg();
$userId = $registerInfo['userId'];
$pw = $registerInfo['pw'];
$name = $registerInfo['name'];
$icon = $registerInfo['icon'];


//helper_sendMsg(array('ret' => 'GameData configuration error'));exit();

$gameData = new GameData ();
if (!$gameData) {
	helper_sendMsg(array('ret' => 'GameData configuration error'));
	helper_log('gameData init fail');
	exit();
}

//helper_sendMsg(array('ret' => 'GameData configuration error 2'));exit();

$userInfo = $gameData->getUserInfo($userId);
// not exist, create a new user
$reward = null;
if (!$userInfo) {
	$returnCode = $gameData->userNameCheck($name);
	if ($returnCode) {
		helper_sendMsg(array ('ret' => 'error', 'code' => $returnCode['code'], 'msg' => $returnCode['msg']));
		exit();
	}
	$gameData->addNewUser(array ('userId' => $userId,'pw' => $pw,'name' => $name,'icon' => $icon));
	$reward = array ('item' => 'coin', 'count' => 1000);
} else {
	helper_sendMsg(array ('ret' => 'error', 'code' => 1000));
	exit();
}

//$scoreRank = $gameData->getScoreRank($userId);
// $loginRank = $gameData->getLoginRank($userId);
$selfScoreRank = $gameData->getSelfScoreRank($userId);
// $retMsg = array(
// 	'ret' => 'ok',
// 	'scoreRank' => $scoreRank,
// 	'loginRank' => $loginRank,
// 	'reward' => $reward
// );

$retMsg = array (
	'ret' => 'ok',
	'info' => $selfScoreRank,
	//'rank' => $scoreRank,
	'reward' => $reward
);

helper_sendMsg($retMsg);

?>

