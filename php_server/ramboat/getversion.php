<?php

function helper_sendMsg($dataArray) {
	$jsonStr = json_encode($dataArray);
	$base64Str = base64_encode($jsonStr);
	echo $base64Str;
	// echo $jsonStr
}

helper_sendMsg(array('version' => 1));

?>

