<?php
include_once 'rewardconfig.php';
include_once 'helper.php';

$result = array (
	'day' => $ramboatDailyRewardConfig
);
helper_sendMsg($result);
?>