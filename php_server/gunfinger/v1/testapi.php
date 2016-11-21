<?php
include_once "helper.php";

function http_post_data($url, $data_string) 
{
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
	curl_setopt($ch, CURLOPT_HTTPHEADER, array(
		'Content-Type: application/json; charset=utf-8',
		'Content-Length: ' . strlen($data_string))
	);
    ob_start();
    curl_exec($ch);
    $return_content = ob_get_contents();
    ob_end_clean();

    $return_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    return array($return_code, $return_content);
}

/*
$dataArray = array(
	"USER_ID" => "sfdfd",
	"PASSWORD" => "123",
	"SCORE" => 2000
);

$sql = helper_getInsertSQL("op_user", $dataArray);
print_r($sql . "\r\n");

$sql = helper_getUpdateSQL("op_user", "USER_ID", $dataArray);
print_r($sql . "\r\n");
*/

/*
print_r("api register ...\r\n");
$url = "http://127.0.0.1/gunfinger/register.php";
$msg = array (
	"userId" => "chris0456",
	"pw" => "chris123",
	"name" => "fatiao12222",
	"icon" => "icon",
);
$data = json_encode($msg);
list($return_code, $return_content) = http_post_data($url, $data);
print_r($return_code);
print_r($return_content);
print_r("\r\n");
*/

/*
print_r("api login ...\r\n");
$url = "http://127.0.0.1/gunfinger/login.php";
$msg = array (
	"userId" => "chris0456",
	"pw" => "chris123"
);
$data = json_encode($msg);
list($return_code, $return_content) = http_post_data($url, $data);
print_r($return_code);
print_r($return_content);
print_r("\r\n");
*/

/*
print_r("api modify ...\r\n");
$url = "http://127.0.0.1/gunfinger/modify.php";
$msg = array (
	"userId" => "chris0456",
	"name" => "fuck1345",
	"icon" => "jay"
);
$data = json_encode($msg);
list($return_code, $return_content) = http_post_data($url, $data);
print_r($return_code);
print_r($return_content);
print_r("\r\n");
*/

/*
print_r("api updatescore ...\r\n");
$url = "http://127.0.0.1/gunfinger/updatescore.php";
$msg = array (
	"userId" => "chris0456",
	"score" => 1569
);
$data = json_encode($msg);
list($return_code, $return_content) = http_post_data($url, $data);
print_r($return_code);
print_r($return_content);
print_r("\r\n");
*/
/*
print_r("api queryrank ...\r\n");
$url = "http://127.0.0.1/gunfinger/queryrank.php";
$msg = array (
	"userId" => "chris0456",
	"day" => 1
);
$data = json_encode($msg);
list($return_code, $return_content) = http_post_data($url, $data);
print_r($return_code);
print_r($return_content);
print_r("\r\n");
*/

?>
