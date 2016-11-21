<?php
include_once 'helper.php';
include_once 'gamedata.php';
include_once 'config.php';

$gameData = new GameData();

// 1. copy today rank to lastday' rank
$gameData->backupScoreRank_redis();

// 2. save today rank to disk file
date_default_timezone_set("Asia/Shanghai");
$backup_file = Config::BACKUP_BASE_PATH . "rank_" . date("Y-m-d");
$backup_content = "";
$totalScoreRank = $gameData->getTotalScoreRank();
$ranknum = 0;
foreach ($totalScoreRank as $key => $value) {
	if (!$key) {continue;}
	$ranknum = $ranknum + 1;
	$backup_content = $backup_content . $ranknum . ", " . $key . ", " . $value . "\r\n";
}
file_put_contents($backup_file, $backup_content);

// 3. clear today scorerank
$gameData->clearScoreRank_redis();

?>
