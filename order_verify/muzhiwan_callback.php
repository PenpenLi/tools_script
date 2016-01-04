<?php

include 'db_config.php';

$url = $_SERVER["REQUEST_URI"];

$queryStr = $_SERVER["QUERY_STRING"];

parse_str($queryStr, $params);


$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
if (!$mysql_connect)
{
	//die('Could not connect: ' . mysql_error());
	error_log("db connect failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
	exit();
}

mysql_select_db("pay_verify_db", $mysql_connect);

$sql="INSERT INTO op_muzhiwan_notify (APPKEY, ORDERID, PRODUCTNAME, PRODUCTDESC, PRODUCTID, MONEY, EXTERN)
VALUES
('$params[appkey]', '$params[orderID]', '$params[productName]', '$params[productDesc]', '$params[productID]', '$params[money]', '$params[uid]', '$params[extern]')";

if (!mysql_query($sql, $mysql_connect))
{
  //die('Error: ' . mysql_error());
	error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
	exit();
}

// update op_order_info 
$sql = "UPDATE op_order_info SET VERIFY_STATE='SUCCESS', TRANSACTION_ID='$params[productName]' WHERE ORDER_ID='$params[orderID]'";
if (!mysql_query($sql))
{
  //die('Error: ' . mysql_error());
	error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
	exit();
}

mysql_close($mysql_connect);

print_r('SUCCESS');

?>

