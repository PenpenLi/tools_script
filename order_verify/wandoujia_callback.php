<?php

include 'db_config.php';
include "wandoujia_rsa.php";

$content=$_REQUEST['content'];
$sign=$_REQUEST['sign'];
$rsa=new Rsa;
$result=$rsa->verify($content,$sign);
if($result) {
    //deal order info in $content
    //echo "success";
} else {
    echo "fail";
    exit();
}

$contentVal = json_decode($content);

$timeStamp = $contentVal[timeStamp'];

$orderId = $contentVal['orderId'];
$money = $contentVal['money'];
$chargeType = $contentVal['chargeType'];
$appKeyId = $contentVal['appKeyId'];
$buyerId = $contentVal['buyerId'];
$out_trade_no = $contentVal['out_trade_no'];


$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
if (!$mysql_connect) {
        //die('Could not connect: ' . mysql_error());
        //error_log("db connect failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
                phplog("db connect failed");
        exit();
}

mysql_select_db("pay_verify_db", $mysql_connect);

$sql="INSERT INTO op_wandoujia_notify (TIME_STAMP, ORDER_ID, MONEY, CHARGE_TYPE, APP_KEY_ID, BUYER_ID, OUT_TRADE_NO)
VALUES
('$timeStamp', '$orderId', '$money', '$chargeType', '$appKeyId', '$buyerId', '$out_trade_no')";

if (!mysql_query($sql, $mysql_connect))
{
  //die('Error: ' . mysql_error());
        //error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
        phplog("db query failed 1");
        exit();
}

// update op_order_info 
$out_trade_no = intval($out_trade_no);
$sql = "UPDATE op_order_info SET VERIFY_STATE='SUCCESS', TRANSACTION_ID='$orderId' WHERE ORDER_ID='$out_trade_no'";
if (!mysql_query($sql))
{
  //die('Error: ' . mysql_error());
        //error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
        phplog("db query failed 2");
        exit();
}

mysql_close($mysql_connect);
print_r('success');

?>
