<?php
include_once 'helper.php';
include_once 'gamedata.php';

$gameData = new GameData();
// $rows = $gameData->getAllUserId();
// $rows = $gameData->getReward('sjy0079');
$gameData->clearTodayRank();
print_r($rows);
?>