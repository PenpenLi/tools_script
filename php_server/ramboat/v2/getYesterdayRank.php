<?php
include_once 'helper.php';
include_once 'gamedata.php';

$registerInfo = helper_receiveMsg();
$userId = $registerInfo['userId'];

$gameData = new GameData();
$day = intval(date("w"));
if ($day == 0) {$day = 7;}
//$yesterdayRank = $gameData->getLastRank($userId, $day);
$yesterdayRank = $gameData->getLastDayRank($userId);
if (!$yesterdayRank) {
	helper_sendMsg(array ('ret' => 'error'));
	exit();
}

foreach ($yesterdayRank as $key => $value) {
	if (!isset($value['military'])) {
		$yesterdayRank[$key]['military'] = -1;
	}
	if (!isset($value['ship'])) {
		$yesterdayRank[$key]['ship'] = -1;
	}
}

$retMsg = array(
	'ret' => 'ok',
	'rank' => $yesterdayRank
);

helper_sendMsg($retMsg);

?>
