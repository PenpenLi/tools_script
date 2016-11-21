<?php
include_once 'helper.php';
include_once 'gamedata.php';

echo "start backup database:\r\n";

$gameData = new GameData();

echo "gameData ready!\r\n";
$allUserId = $gameData->getAllUserId();
/*
echo "allUserId ready!\r\n";
foreach ($allUserId as $key => $value) {
	$userId = $value;
	//$scoreRank = $gameData->getScoreRank($userId);
	//$thisDayRank = json_encode($scoreRank);helper_log($thisDayRank);
	//$mysql_real_escape_string($thisDayRank);
	echo $userId . "\r\n";
	$gameData->backupScoreRank($userId);
	
}
//delete today rank
echo "backup data done\r\n";
*/

$gameData->backupScoreRank_redis();


date_default_timezone_set("Asia/Shanghai");
$backup_file = "/home/www/ramboat/backup/" . date("Y-m-d");
$backup_content = "rank_num:" . count($allUserId);
file_put_contents($backup_file, $backup_content); 

$gameData->clearTodayRank();
?>
