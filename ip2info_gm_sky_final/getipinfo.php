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

$config = file_get_contents("config_version.json");
$config = json_decode($config, TRUE);
$server_version = $config['version'];
$db_ip = "127.0.0.1";
$db_user = "root";
$db_pw = "root123!";

$postdata = file_get_contents("php://input");
if ($postdata == '') {
	print_r('post data is blank..');
	exit();
}

$msg = json_decode($postdata, true);
$client_version = $msg['version'];
$gamename = $msg['gamename'];
$platform = $msg['platform'];
$wirelesstype = $msg['wirelesstype'];
if ($wirelesstype == 'yidong') {
	$wirelesstype = '移动';
} else if ($wirelesstype == 'liantong') {
	$wirelesstype = '联通';
} else if ($wirelesstype == 'dianxin') {
	$wirelesstype = '电信';
}

if ($client_version >= $server_version) {
	$retmsg = array("iteminfo" => "", "version" => $server_version);
	print_r(json_encode($retmsg));
	exit();
}

$ip = getip();
$ip = ip2long($ip);

$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
mysql_select_db("ip2info", $mysql_connect);

mysql_query("SET NAMES 'UTF8'");
mysql_query("SET CHARACTER SET UTF8");
mysql_query("SET CHARACTER_SET_RESULTS='UTF8'");

$sql = "SELECT * FROM op_ip WHERE $ip >= START_IP AND $ip <= END_IP";
$rows = mysql_query($sql, $mysql_connect);
$row = mysql_fetch_array($rows);

$province = $row['PROVINCE'];

$iteminfo = array();
if ($province) {	
	$sql = "SELECT ITEM_KEY, ITEM_VALUE FROM op_gameinfo WHERE PROVINCE='$province' AND GAME_NAME='$gamename' AND PLATFORM='$platform' AND WIRELESS_TYPE='$wirelesstype'";
	$rows = mysql_query($sql, $mysql_connect);
	while ($row = mysql_fetch_assoc($rows)) {
		//array_push($iteminfo, $row);
		$iteminfo[$row['ITEM_KEY']] = $row['ITEM_VALUE'];
	}	
}
if (count($iteminfo) == 0) {$iteminfo = "";}
$retmsg = array('iteminfo' => $iteminfo, 'version' => $server_version);
print_r(json_encode($retmsg));
mysql_close($mysql_connect);

?>

