<?php
include_once 'helper.php';
include_once 'gamedata.php';

$registerInfo = helper_receiveMsg();
$userId = $registerInfo['userId'];

$gameData = new GameData();
$day = intval(date("w"));
$yesterdayRank = $gameData->getLastRank($userId, $day);

if (!$yesterdayRank) {

	helper_sendMsg(array ('ret' => 'error'));
	exit();
}

$retMsg = array(
	'ret' => 'ok',
	'rank' => $yesterdayRank
);

helper_sendMsg($retMsg);

?>
