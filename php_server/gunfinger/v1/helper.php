<?php
date_default_timezone_set("Asia/Shanghai");
include_once 'config.php';

function helper_log($str) {
	error_log($str . "\r\n", 3, '/tmp/gunfinger.log');
}


function helper_receiveMsg() {
	$postdata = file_get_contents("php://input");
	if ($postdata == '') {
		helper_log('[helper] post data is blank..');
		echo 'post data is blank..';
		exit();
	}
	$sign_private_key = "gunfinger_!@#$%^&*()_zxcvbnm";
	$msgRaw = json_decode($postdata, true);
	$msgbase64 = $msgRaw['msg'];
	$msgSigh = $msgRaw['sign'];
	if (md5($msgbase64 . $sign_private_key) != $msgSigh) {
		echo 'sign is not right..';
		exit();
	}
	
	$msg64 = base64_decode($msgbase64);
	$msg = json_decode($msg64, true);
	return $msg;
}

function helper_sendMsg($dataArray) {
	$msg = array();
	$jsonStr = json_encode($dataArray);
	$msgbase64 = base64_encode($jsonStr);
	$sign_private_key = "gunfinger_!@#$%^&*()_zxcvbnm";
	$sign = md5($msgbase64 . $sign_private_key);
	$msg['msg'] = $msgbase64;
	$msg['sign'] = $sign;
	$msgStr = json_encode($msg);
	//$gzipStr = gzencode($msgStr);
	//echo $gzipStr;
	echo $msgStr;
}

function helper_getInsertSQL($tableName, $dataArray) {
	$str1 = '';
	$str2 = '';
	foreach ($dataArray as $key => $value) {
		$str1 = $str1 . $key . ',';
		if (is_string($value)) {
			$str2 = $str2 . "'" . $value . "'" . ',';
		} else {
			$str2 = $str2 . $value . ',';
		}
	}
	$str1 = substr($str1, 0, strlen($str1) - 1);
	$str2 = substr($str2, 0, strlen($str2) - 1);

	$sql = "INSERT INTO " . $tableName . " (" . $str1 . ")" . " VALUES " . "(" . $str2 . ")";

	return $sql;
}

function helper_getUpdateSQL($tableName, $keyName, $dataArray) {
	$str = '';
	foreach ($dataArray as $key => $value) {
		if ($key == $keyName) continue;
		if (is_string($value)) {
			$str .= $key . "=" . "'" . $value . "'" . ",";
		} else {
			$str .= $key . "=" . $value . ",";
		}
	}
	$str = substr($str, 0, strlen($str) - 1);

	$tableKeyValue = $dataArray[$keyName];
	if (is_string($tableKeyValue)) {
		$tableKeyValue = "'" . $tableKeyValue . "'";
	}
	$sql = "UPDATE " . $tableName . " SET " . $str . " WHERE " . $keyName . "=" . $tableKeyValue;
	
	return $sql;
}



?>
