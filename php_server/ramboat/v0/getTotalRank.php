<?php
include_once 'helper.php';
include_once 'gamedata.php';

$registerInfo = helper_receiveMsg();
$userId = $registerInfo['userId'];

$gameData = new GameData();
$totalrank = $gameData->getTotalScoreRank($userId);

if (!$totalrank) {

	helper_sendMsg(array ('ret' => 'get_totalrank_failed'));
	exit();
}

$retMsg = array(
	'ret' => 'ok',
	'rank' => $totalrank
);

helper_sendMsg($retMsg);

?>