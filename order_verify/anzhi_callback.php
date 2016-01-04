<?php

include 'db_config.php';
require 'anzhi_class_desc.php';


$data = $_POST;

if(!isset($data['data'])){ 
	return 'failed';		
}

$des = new DES();
$code = $des->decrypt($data['data']);
if(!isset($code) || empty($code))
{
	return 'failed';
}
//$code = 'callback is coming';
//phplog($code);

$response = json_decode($code, true);
if(!isset($response))
{
	return 'failed';
}

$uid = $response['uid'];
$orderId = $response['orderId'];
$orderAmount = $response['orderAmount'];
$orderTime = $response['orderTime'];
$orderAccount = $response['orderAccount'];
$code = $response['code'];
$payAmount = $response['payAmount'];
$cpInfo = $response['cpInfo'];
$notifyTime = $response['notifyTime'];
$memo = $response['memo'];

$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
if (!$mysql_connect) {
        //die('Could not connect: ' . mysql_error());
        //error_log("db connect failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
		phplog("db connect failed");
        exit();
}

mysql_select_db("pay_verify_db", $mysql_connect);

$sql="INSERT INTO op_anzhi_notify (PAY_AMOUNT, UID, NOTIFY_TIME, CP_INFO, ORDER_AMOUNT, ORDER_ACCOUNT, CODE, ORDER_TIME, MSG, ORDER_ID)
VALUES
('$orderAmount', '$uid', '$notifyTime', '$cpInfo', '$orderAmount', '$orderAccount', '$code', '$orderTime', '$memo', '$orderId')";

if (!mysql_query($sql, $mysql_connect))
{
  //die('Error: ' . mysql_error());
        //error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
        phplog("db query failed 1");
        exit();
}

// update op_order_info 
$cpInfo = intval($cpInfo);
$sql = "UPDATE op_order_info SET VERIFY_STATE='SUCCESS', TRANSACTION_ID='$uid' WHERE ORDER_ID='$cpInfo'";
if (!mysql_query($sql))
{
  //die('Error: ' . mysql_error());
        //error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
        phplog("db query failed 2");
        exit();
}

mysql_close($mysql_connect);

echo "success";

	
?>



