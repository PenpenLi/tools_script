
<?php
include_once 'helper.php';
include_once 'gamedata.php';

// date_default_timezone_set("Asia/Shanghai");
// $backup_file = Config::BACKUP_BASE_PATH . "rank_" . date("Y-m-d");
// $backup_content = "";
// $totalScoreRank = $gameData->getTotalScoreRank();
// $ranknum = 0;
// foreach ($totalScoreRank as $key => $value) {
// 	if (!$key) {continue;}
// 	$ranknum = $ranknum + 1;
// 	$backup_content = $backup_content . $ranknum . ", " . $key . ", " . $value . "\r\n";
// }
// file_put_contents($backup_file, $backup_content);

$gameData = new GameData();

date_default_timezone_set("Asia/Shanghai");
$backup_file = getcwd() . '/../ramboat_rank/' . 'rank_' . date('Y-m-d');
$backup_content = "";
$totalScoreRank = $gameData->getTotalScoreRank();
$ranknum = 0;
foreach ($totalScoreRank as $key => $value) {
	if (!$key) {continue;}
	$ranknum = $ranknum + 1;
	$backup_content = $backup_content . $ranknum . ", " . $key . ", " . $value . "\r\n";
}
file_put_contents($backup_file, $backup_content);

?>


