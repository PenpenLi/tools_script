
<?php
include_once 'helper.php';
include_once 'gamedata.php';


$userId = "RamboatRamboatRamboat2222";
$gameData = new GameData();
$day = intval(date("w"));
if ($day == 0) {$day = 7;}

//$yesterdayRank = $gameData->getLastDayRank_redis($userId);
$yesterdayRank = $gameData->getScoreRank($userId);
if (!$yesterdayRank) {

        helper_sendMsg(array ('ret' => 'error'));
        exit();
}



$retMsg = array(
        'ret' => 'ok',
        'rank' => $yesterdayRank
);

echo json_encode($retMsg);




?>


