<?php
include_once 'helper.php';
include_once 'gamedata.php';

$registerInfo = helper_receiveMsg();
$userId = $registerInfo['userId'];

$gameData = new GameData();
$lastRank = $gameData->getLastScoreRank($userId);

if (!$lastRank) {

	helper_sendMsg(array ('ret' => 'error'));
	exit();
}

$retMsg = array(
	'ret' => 'ok',
	'rank' => $lastRank
);

helper_sendMsg($retMsg);

?>