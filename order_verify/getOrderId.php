<?php

include 'db_config.php';

function calc_md5($array)
{
	if (!is_array($array))
	{
		return "";
	}

	ksort($array);

	$preSignStr = "";

	foreach ($array as $key => $value) 
	{
		if ($key != "sign")
		{
			if ($preSignStr != "")
			{
				$preSignStr = $preSignStr . "&";
			}
			$preSignStr = $preSignStr . $key . "=" . $value;
		}
	}

	$preSignStr = $preSignStr . "&private_key=sky_game_miyi";

	return md5($preSignStr);
}

$postdata = file_get_contents("php://input");

if ($postdata == '')
{
	print_r('post data is blank..');
	exit();
}

$msg = json_decode($postdata, true);

$md5Str = calc_md5($msg);

if ($md5Str != $msg['sign'])
{
	print_r('sign is not correct' . "\r\n");
	exit();
}


$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
if (!$mysql_connect)
{
	//die('Could not connect: ' . mysql_error());
	//error_log("db connect failed" ."\r\n", 3, "/nfsroot/html_onefigure/app/debug.info");
	print_r("db connect failed\r\n");
	exit();
}

mysql_select_db("pay_verify_db", $mysql_connect);

$msg['verify_state'] = 'FAIL';
$msg['transaction_id'] = 'NOTHING';

$sql="INSERT INTO op_order_info (CLIENT_ID, GAME_NAME, SDK_PAY_TYPE, TIME_STAMP, VERIFY_STATE, TRANSACTION_ID)
VALUES
('$msg[client_id]','$msg[game_name]','$msg[sdk_pay_type]','$msg[time_stamp]', '$msg[verify_state]', '$msg[transaction_id]')";

if (!mysql_query($sql))
{
  //die('Error: ' . mysql_error());
	//error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/app/debug.info");
	print_r("db query failed\r\n");
	exit();
}

//Now insert one order to DB, and get orderId
$orderId = mysql_insert_id();

/*
// test modify ordrinfo
$sql = "UPDATE op_order_info SET VERIFY_STATE='SUCCESS', TRANSACTION_ID='12344354556576' WHERE ORDER_ID='3'";
if (!mysql_query($sql))
{
  //die('Error: ' . mysql_error());
	error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
	exit();
}
*/
mysql_close($mysql_connect);

$retMsg = array();
$retMsg['order_id'] = $orderId;
$retMsg['time_stamp'] = date('Y-m-d H:i:s');
$retMd5 = calc_md5($retMsg);
$retMsg['sign'] = $retMd5;

print_r(json_encode($retMsg));

//print_r($orderId);
//var_dump($msg);
?>
