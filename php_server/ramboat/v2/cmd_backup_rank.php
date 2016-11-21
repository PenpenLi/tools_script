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
$backup_file = '/home/www/ramboat/backup/' . 'rank_' . date('Y-m-d');
$backup_content = "";
$totalScoreRank = $gameData->getTotalScoreRank();
$ranknum = 0;
foreach ($totalScoreRank as $key => $value) {
	$ranknum = $ranknum + 1;
	$backup_content = $backup_content . $ranknum . ", " . json_encode($value) . "\r\n";
}
file_put_contents($backup_file, $backup_content);

date_default_timezone_set("Asia/Shanghai");
$backup_file = "/home/www/ramboat/backup/" . date("Y-m-d");
$backup_content = "rank_num:" . count($allUserId);
file_put_contents($backup_file, $backup_content); 

$gameData->clearTodayRank();
?>

