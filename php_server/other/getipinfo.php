<?php

function getip() {
    if ($_SERVER["HTTP_X_FORWARDED_FOR"])
        $ip = $_SERVER["HTTP_X_FORWARDED_FOR"];
    else if ($_SERVER["HTTP_CLIENT_IP"])
        $ip = $_SERVER["HTTP_CLIENT_IP"];
    else if ($_SERVER["REMOTE_ADDR"])
        $ip = $_SERVER["REMOTE_ADDR"];
    else if (getenv("HTTP_X_FORWARDED_FOR"))
        $ip = getenv("HTTP_X_FORWARDED_FOR");
    else if (getenv("HTTP_CLIENT_IP"))
        $ip = getenv("HTTP_CLIENT_IP");
    else if (getenv("REMOTE_ADDR"))
        $ip = getenv("REMOTE_ADDR");
    else
        $ip = "Unknown";
    return $ip;
}

$db_ip = "127.0.0.1";
$db_user = "root";
$db_pw = "root123!";
// $postdata = file_get_contents("php://input");
// if ($postdata == '') {
// 	print_r('post data is blank..');
// 	exit();
// }

// $msg = json_decode($postdata, true);
$ip = $_SERVER['HTTP_CLIENT_IP'];
$ip = ip2long($ip);

$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
mysql_select_db("ip2info", $mysql_connect);

$sql = "SELECT * FROM op_ip WHERE $ip >= START_IP AND $ip <= END_IP";
$rows = mysql_query($sql, $mysql_connect);
$row = mysql_fetch_array($rows);
mysql_close($mysql_connect);

$province = $row['PROVINCE'];

$ret = 0;
if ($province == "浙江省") {
	$ret = 1;
}
$retmsg = array('ret' => $ret);
print_r(json_encode($retmsg));

?>
