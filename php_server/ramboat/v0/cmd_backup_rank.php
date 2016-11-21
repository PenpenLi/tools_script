<?php
include_once 'helper.php';
include_once 'gamedata.php';

$gameData = new GameData();
$allUserId = $gameData->getAllUserId();
foreach ($allUserId as $key => $value) {
	$userId = $value;
	$scoreRank = $gameData->getScoreRank($userId);
	$thisDayRank = json_encode($scoreRank);
	$gameData->backupScoreRank($userId);
}
//delete today rank
$gameData->clearTodayRank();
?>
