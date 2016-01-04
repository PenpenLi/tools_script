<?php

include 'db_config.php';

/* *
 * 联想支付平台RSA函数
 * 详细：RSA签名、验签
 * 版本：1.0
 * 日期：2014-10-30
 * 说明：
 * 以下代码只是为了方便商户测试而提供的样例代码，商户可以根据自己网站的需要，按照技术文档编写,并非一定要使用该代码。
 * 该代码仅供学习和研究联想支付平台接口使用，只是提供一个参考。
 */

/**
 * RSA签名
 * @param $data   待签名数据
 * @param $priKey 密钥
 * return 签名结果
 */
function sign($data, $priKey) {
	if(strpos($priKey, "BEGIN RSA PRIVATE KEY") === false)
	{
		//$priKey = wordwrap($priKey, 64, "\n", true);
		//$priKey = "-----BEGIN PRIVATE KEY-----\n".$priKey."\n-----END PRIVATE KEY-----";
	}
    $res = openssl_get_privatekey($priKey);
    openssl_sign($data, $sign, $res);
    openssl_free_key($res);
    $sign = base64_encode($sign);
    return $sign;
}

/**
 * RSA验签
 * @param $data   待签名数据
 * @param $priKey 密钥
 * @param $sign   要校对的的签名结果
 * return 验证结果
 */
function verify($data, $priKey, $sign)  {
	$selfSign = sign($data, $priKey);
    if($selfSign == $sign){
		return true;
	}else{
		return false;
	}
}



/**
 * 功能：联想支付平台签名使用方法示例
 * 说明：
 * 以下代码只是为了方便商户测试而提供的样例代码，商户可以根据自己网站的需要，按照技术文档编写,并非一定要使用该代码。
 * 该代码仅供学习和研究联想支付平台接口使用，只是提供一个参考。
 * @date		2014/10/30
 */

//require_once("lenovo_lib.php");

$priKey ="MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBAIi3HOzLAtYdy9tRLjl8yuH/JDvT2w/9wiwWHiO62EK2joyGyntGXZJKFaPQV2JcpxdcogZt2oESZRZTSjp4dzVlwRwlacwLmqQehpWuO7MAIla0dSrEFPmklEF0f2g/BlgMLfcB3mI6DJd3JXWOb+RSzbQ6Gn0HVZMVIac6x++fAgMBAAECgYAUAfBKux8MJMorq4K/SQML9eHgm42IlpNvDkGU6wgxIzRTXnqNdevFt+/YzGmtbG/LnHPdyv/aOc3vaXsIeY7AW/sUXskBi9aoI5VQAnHrkBK8u9vA8rBWZH0F7AB8qXYNaC+CRwuZ7v70GamcN0gK8vdHlI7vbF6FcOaNQ3YhCQJBAMI9kx9Xd/kX/NX+uCykGvbXiZ3jir1OV+SswYfTc2/W/u48CRTxJGUoo8XkLo+DErGUm8bcf7p1Jmh8IkQsvY0CQQC0LzTy0RyccpXtZKt3PNeJN6sX/0ecnY0YS8vO7R6i9uYRxp+EZ8HHhVqgNOwdDZcRZu/vvsArD0zYvc3wIujbAkBJPsDgu7IQ6zu7Uk8O/l7wn/xT3n7tvcIlCAP+fPz1uoPtQBRIoloQD1Aail4IXevJrPlE9tbvb8eo9M5TGO/tAkEAldLC9jzGy5fD7tdMdCNVHTX6wEN2alXfIpqz52XnmZ4vBAziSxhzWtIZ3yjiQ+4u9TtguzsjUyqvvXFlsg11DQJAJEvjXgOO8bYCDgnF59fMYuqlj6xBPRmU2iMLUPdJvzk4HrW4D8n1EErOHAjT/AfKdZyg/Us/ZiIT4kmm72G3JQ==";
$transdata = trim($_POST['transdata']);
$sign =  trim($_POST['sign']);

if (empty($transdata)) {
	echo '{"error":"3"}';
	return false;
}

if (empty($sign)) {
	echo '{"error":"3"}';
	return false;
}

$sign_local = sign($transdata, $priKey);
$result = verify($transdata, $priKey, $sign_local);
if ($result) {
    echo 'SUCCESS';
} else {
    echo 'FAILTURE';
}

$params =  json_decode($transdata, true);


$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
if (!$mysql_connect)
{
	//die('Could not connect: ' . mysql_error());
	error_log("db connect failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
	exit();
}

mysql_select_db("pay_verify_db", $mysql_connect);

$sql="INSERT INTO op_lenovo_notify (exorderno, transid, appid, waresid, feetype, money,count, result, transtype, transtime, cpprivate, paytype)
VALUES
('$params[exorderno]', '$params[transid]', '$params[appid]', '$params[waresid]', '$params[feetype]', '$params[money]', '$params[count]', '$params[result]', '$params[transtype]', '$params[transtime]', '$params[cpprivate]', '$params[paytype]')";

if (!mysql_query($sql, $mysql_connect))
{
  //die('Error: ' . mysql_error());
	error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
	exit();
}

// update op_order_info 
$sql = "UPDATE op_order_info SET VERIFY_STATE='SUCCESS', TRANSACTION_ID='$params[transid]' WHERE ORDER_ID='$params[exorderno]'";
if (!mysql_query($sql))
{
  //die('Error: ' . mysql_error());
	error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
	exit();
}

mysql_close($mysql_connect);

?>

