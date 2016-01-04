<?php

include 'db_config.php';

////////////////weixin/////////////////////////
function weixin_helper_toXml($array)
{  	
	$xml = "<?xml version='1.0' encoding='gb2312' ?>";
	//$xml = "<xml>";
   	$xml .= "<root>";
   	foreach ($array as $key=>$val)
   	{
   		/*
   		if (is_numeric($val)){
   			$xml.="<".$key.">".$val."</".$key.">";
   		}else{
   			$xml.="<".$key."><![CDATA[".$val."]]></".$key.">";
   		}
   		*/
   		$xml.="<".$key.">".$val."</".$key.">";
    }
    $xml.="</root>";
    return $xml; 
}


function weixin_calc_md5($array)
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
	$preSignStr = $preSignStr."&key=skymobimiyigame20150716abcdefg00";
	$md5Str = md5($preSignStr);

	return strtoupper($md5Str);
}

function weixin_create_nonce_str($length = 8)
{
     // 密码字符集，可任意添加你需要的字符
    $chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890';
    $password = '';
    for ( $i = 0; $i < $length; $i++ )
    {
        // 这里提供两种字符获取方式
        // 第一种是使用 substr 截取$chars中的任意一位字符；
        // 第二种是取字符数组 $chars 的任意元素
        // $password .= substr($chars, mt_rand(0, strlen($chars) - 1), 1);
        $password .= $chars[ mt_rand(0, strlen($chars) - 1) ];
    }

    return $password;
}

function weixin_http_post_data($url, $data_string) 
{
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_POST, 1);
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
		curl_setopt($ch, CURLOPT_HTTPHEADER, array(
			'Content-Type: application/xml; charset=utf-8',
			'Content-Length: ' . strlen($data_string))
		);
        ob_start();
        curl_exec($ch);
        $return_content = ob_get_contents();
        ob_end_clean();

        $return_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
        return array($return_code, $return_content);
}

function bbr_weixin_query_order($out_trade_no)
{
	$query_url = "https://api.mch.weixin.qq.com/pay/orderquery";
	$msg = array();
	$msg['appid'] = "wx5b1cfdecadb2c1a1";
	$msg['mch_id'] = "1248515901";
	$msg['nonce_str'] = weixin_create_nonce_str(32);
	//$msg['out_trade_no'] = "f91e24dfe80012e2a7984afa4480a6d6";
	$msg['out_trade_no'] = $out_trade_no;
	$msg['sign'] = weixin_calc_md5($msg);
	$xml = weixin_helper_toXml($msg);

	$ch = curl_init();
	curl_setopt ($ch, CURLOPT_URL, $query_url);
	curl_setopt($ch, CURLOPT_HEADER, 0);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1); 
	curl_setopt($ch, CURLOPT_POST, 1);
	curl_setopt($ch, CURLOPT_POSTFIELDS, $xml);
	$file_contents = curl_exec($ch);
	curl_close($ch);
	
	$xml = simplexml_load_string($file_contents);
	//error_log($file_contents."\r\n", 3, "/tmp/debug.info");	
	$rowdata = array();
	foreach($xml->children() as $child)
	{
	  	$rowdata[$child->getName()] = $child->__toString();
	}

	if ($rowdata['trade_state'] == 'SUCCESS')
	{
		return 'SUCCESS';
	}
	else
	{
		return 'FAIL';
	}
}
///////////////////weixin/////////////////////


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

//error_log($postdata ."\r\n", 3, "/nfsroot/html_onefigure/app/debug.info");

$msg = json_decode($postdata, true);

$md5Str = calc_md5($msg);

if ($md5Str != $msg['sign'])
{
	print_r('sign is not correct' . "\r\n");
	error_log("sign is not correct" ."\r\n", 3, "/tmp/debug.info");
	exit();
}


$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
if (!$mysql_connect)
{
	//die('Could not connect: ' . mysql_error());
	error_log("db connect failed" ."\r\n", 3, "/tmp/debug.info");
	print_r("db connect failed\r\n");
	exit();
}

mysql_select_db("pay_verify_db", $mysql_connect);

$sql = "SELECT * FROM op_order_info WHERE ORDER_ID='$msg[order_id]'";
$result = mysql_query($sql);


if (!$result)
{
  //die('Error: ' . mysql_error());
	error_log("db query failed" ."\r\n", 3, "/tmp/debug.info");
	print_r("db query failed\r\n");
	exit();
}
$row = mysql_fetch_array($result);
mysql_close($mysql_connect);

error_log("out_trade_no:".$msg['transaction_id']."\r\n", 3, "/tmp/debug.info");

$pay_result = 'FAIL';
// if order state fail, means that server have not receive notify callback from weixin
// if client hava transaction_id from weixin, server should query order with sending msg to weixin
if ($row['VERIFY_STATE'] == 'FAIL')
{
	if ($msg['transaction_id'] != "" && $row['GAME_NAME'] == 'bbr')
	{
		$pay_result = bbr_weixin_query_order($msg['transaction_id']);
	}
}
else if ($row['VERIFY_STATE'] == 'SUCCESS')
{
	$pay_result = 'SUCCESS';
}

$retMsg = array();
$retMsg['pay_result'] = $pay_result;
$retMsg['time_stamp'] = date('Y-m-d H:i:s');
$retMd5 = calc_md5($retMsg);
$retMsg['sign'] = $retMd5;

print_r(json_encode($retMsg));

?>
