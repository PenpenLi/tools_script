<?php
include_once 'helper.php';
include_once 'gamedata.php';

$registerInfo = helper_receiveMsg();
$userId = $registerInfo['userId'];

$gameData = new GameData();
$todayRank = $gameData->getScoreRank($userId);

if (!$todayRank) {

	helper_sendMsg(array ('ret' => 'error'));
	exit();
}

$retMsg = array(
	'ret' => 'ok',
	'rank' => $todayRank
);

helper_sendMsg($retMsg);

?>