<?php
include_once 'helper.php';
include_once 'gamedata.php';

$registerInfo = helper_receiveMsg();
$userId = $registerInfo['userId'];

$gameData = new GameData();
$dairyRank = $gameData->getScoreRank($userId);

if (!$dairyRank) {

	helper_sendMsg(array ('ret' => 'get_rank_failed'));
	exit();
}

$retMsg = array(
	'ret' => 'ok',
	'rank' => $dairyRank
);

helper_sendMsg($retMsg);

?>