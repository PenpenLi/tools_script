<?php
include_once 'helper.php';
include_once 'gamedata.php';

$registerInfo = helper_receiveMsg();

if (!$registerInfo) {
     helper_sendMsg(array ('ret' => 'error', 'code' => 3000));
     exit();
}

$userId = $registerInfo['userId'];
// $pw = $registerInfo['pw'];

$gameData = new GameData();
$userInfo = $gameData->getUserInfo($userId);
if (!$userInfo) {
	helper_sendMsg(array ('ret' => 'error', 'code' => 3001));
	exit();
}

// if ($userInfo['PW'] != $pw) {
// 	helper_sendMsg(array ('ret' => 'pw_not_right'));
// 	exit();
// }

// update continual login times
// $timeStamp = intval(date("w"));
// if ($userInfo['LOGINSTAMP'] != $timeStamp) {
// 	$loginTimes = $userInfo['LOGINTIMES'];
// 	if ($timeStamp == $userInfo['LOGINSTAMP'] + 1) {
// 		$loginTimes = $loginTimes + 1;
// 	} else {
// 		$loginTimes = 1;
// 	}
// 	$gameData->updateLoginInfo($userId, $timeStamp, $loginTimes);
// }


//$scoreRank = $gameData->getScoreRank($userId);
// $loginRank = $gameData->getLoginRank($userId);
$reward = $gameData->getReward($userId);
$selfScoreRank = $gameData->getSelfScoreRank($userId);

$retMsg = array(
	'ret' => 'ok',
	'info' => $selfScoreRank,
	//'rank' => $scoreRank,
	'reward' => $reward
);

helper_sendMsg($retMsg);

?>
