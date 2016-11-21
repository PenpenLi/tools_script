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


 print_r("api register ...\r\n");
 $url = "http://itramboat.miyigame.com:35003//register.php";
 $msg = array (
 	"userId" => "sjy0081",
 	"pw" => "1234567",
 	"name" => "",
 	"icon" => "3",
 );
 $data = json_encode($msg);
 list($return_code, $return_content) = http_post_data($url, $data);
 print_r($return_code);
 print_r($return_content);
 print_r("\r\n");


// print_r("api totalrank ...\r\n");
// $url = "http://127.0.0.1/ramboat/getTotalRank.php";
// $msg = array (
// 	"userId" => "sjy0079"
// );
// $data = json_encode($msg);
// list($return_code, $return_content) = http_post_data($url, $data);
// print_r($return_code);
// print_r($return_content);
// print_r("\r\n");

// print_r("api lastrank ...\r\n");
// $url = "http://127.0.0.1/ramboat/getLastWeekRank.php";
// $msg = array (
// 	"userId" => "frankyuan"
// );
// $data = json_encode($msg);
// list($return_code, $return_content) = http_post_data($url, $data);
// print_r($return_code);
// print_r($return_content);
// print_r("\r\n");

// print_r("api login ...\r\n");
// $url = "http://127.0.0.1/ramboat/login.php";
// $msg = array (
// 	"userId" => "frankyuan",
// );
// $data = json_encode($msg);
// list($return_code, $return_content) = http_post_data($url, $data);
// print_r($return_code);
// print_r($return_content);
// print_r("\r\n");



// print_r("api modify ...\r\n");
// $url = "http://127.0.0.1/ramboat/modify.php";
// $msg = array (
// 	"userId" => "sjy0079",
// 	"name" => "HuangCe",
// 	"icon" => "icon3"
// );
// $data = json_encode($msg);
// list($return_code, $return_content) = http_post_data($url, $data);
// print_r($return_code);
// print_r($return_content);
// print_r("\r\n");


/*
print_r("api updatescore ...\r\n");
$url = "http://127.0.0.1/ramboat/getYesterdayRank.php";
$msg = array (
    "userId" => "RamboatRamboatRamboat_abc",
    "score" => 100,
    "icon" => 7
);
$data = json_encode($msg);
list($return_code, $return_content) = http_post_data($url, $data);
print_r($return_code);
print_r($return_content);
print_r("\r\n");
*/


?>
