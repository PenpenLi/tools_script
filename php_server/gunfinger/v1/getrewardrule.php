<?php
include_once 'helper.php';
include_once 'gamedata.php';

$msg = helper_receiveMsg();
$userId = $msg['userId'];
$token = $msg['token'];

session_id($token);
session_start();
$islogin = false;
if (isset($_SESSION['userId'])) {
	// if ($_SESSION['userId'] == $userId) {
	// 	$islogin = true;
	// }
	$islogin = true;
}
if ($islogin == false) {
	helper_sendMsg(array ('ret' => 'user_not_login'));
	exit();
} 

$rewardrule = '{"globalChallenge":{},"dailyChallenge":[{"percentile":1,"gold":0,"coins":0,"mods":6,"showfailure":1},{"percentile":10,"gold":0,"coins":0,"mods":3,"showfailure":1},{"percentile":25,"gold":0,"coins":0,"mods":2,"showfailure":1},{"percentile":40,"gold":0,"coins":0,"mods":1,"showfailure":1}]}';

$retMsg = array(
	'ret' => 'ok',
	'rewardrule' => $rewardrule
);

helper_sendMsg($retMsg);

?>