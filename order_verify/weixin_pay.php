<?php

include 'db_config.php';

$url = $_SERVER["REQUEST_URI"];

//if(file_exists("/nfsroot/html_onefigure/debug.info"))
//{
	//unlink("/nfsroot/html_onefigure/debug.info");
//}

//error_log(json_encode(parse_url($url)) ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");

$postdata = file_get_contents("php://input");
//error_log($postdata ."\r\n", 3, "/tmp/debug.info");
if ($postdata == '')
{
	print_r('post data is blank..');
	exit();
}

$xml = simplexml_load_string($postdata);
//error_log(json_encode($values) ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");

$rowdata = array();
foreach($xml->children() as $child)
{
  	$rowdata[$child->getName()] = $child->__toString();
}

$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
if (!$mysql_connect)
{
	//die('Could not connect: ' . mysql_error());
	error_log("db connect failed" ."\r\n", 3, "/tmp/debug.info");
	exit();
}

mysql_select_db("pay_verify_db", $mysql_connect);

$sql="INSERT INTO op_weixin_notify (APPID, ATTACH, BANK_TYPE, CASH_FEE, FEE_TYPE, IS_SUBSCRIBE, NONCE_STR, OPEN_ID, OUT_TRADE_NO, RESULT_CODE, RETURN_CODE, SIGN, TIME_END, TOTAL_FEE, TRADE_TYPE, TRANSACTION_ID)
VALUES
('$rowdata[appid]','$rowdata[attach]','$rowdata[bank_type]','$rowdata[cash_fee]','$rowdata[fee_type]','$rowdata[mch_id]', '$rowdata[nonce_str]','$rowdata[openid]','$rowdata[out_trade_no]', '$rowdata[result_code]','$rowdata[return_code]','$rowdata[sign]', '$rowdata[time_end]','$rowdata[total_fee]','$rowdata[trade_type]','$rowdata[transaction_id]')";

if (!mysql_query($sql, $mysql_connect))
{
  //die('Error: ' . mysql_error());
	error_log("db query failed" ."\r\n", 3, "/tmp/debug.info");
	exit();
}

// update op_order_info 
$sql = "UPDATE op_order_info SET VERIFY_STATE='SUCCESS', TRANSACTION_ID='$rowdata[out_trade_no]' WHERE ORDER_ID='$rowdata[attach]'";
if (!mysql_query($sql))
{
  //die('Error: ' . mysql_error());
	error_log("db query failed" ."\r\n", 3, "/tmp/debug.info");
	exit();
}

mysql_close($mysql_connect);

echo "<xml><return_code><![CDATA[SUCCESS]]></return_code><return_msg><![CDATA[OK]]></return_msg></xml>";

?>
