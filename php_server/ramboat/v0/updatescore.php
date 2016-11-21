<?php
include_once 'helper.php';
include_once 'gamedata.php';

$msg = helper_receiveMsg();
$userId = $msg['userId'];
$userIcon = $msg['icon'];
$newscore = $msg['score'];

$gameData = new GameData();
$userInfo = $gameData->getUserInfo($userId);
if (!$userInfo) {
	helper_sendMsg(array ('ret' => 'error', 'code' => '2000'));
	exit();
}
$returnCode = $gameData->updateUserScore($userId, $newscore, $userIcon);
if (!$returnCode) {
	helper_sendMsg(array ('ret' => 'error', 'code' => '0000'));
	exit();
} elseif ($returnCode != 1) {
	helper_sendMsg(array ('ret' => 'error', 'code' => $returnCode));
	exit();
}

// $scoreRank = $gameData->getScoreRank($userId);
$selfScoreRank = $gameData->getSelfScoreRank($userId);
helper_sendMsg(array ('ret' => 'ok', 'info' => $selfScoreRank));

?>
