<!-- 
<?php
$alluser = array();
$connect = mysql_connect("127.0.0.1", "root", "root123!");
mysql_select_db("pay_verify_db");

$sql = "select cpUserInfo, payFee from op_xiaomi_ntf";
$result = mysql_query($sql);
while (($row = mysql_fetch_assoc($result))) {
	array_push($alluser, $row);
}
mysql_close($connect);

file_put_contents("pay_user.json", json_encode($alluser));

?> -->



<?php
$alluser = array();
$connect = mysql_connect("127.0.0.1", "root", "root123!");
mysql_select_db("ofdp_360");

$sql = "select password from op_xiaomi_ntf";
$result = mysql_query($sql);
while (($row = mysql_fetch_assoc($result))) {
        array_push($alluser, $row);
}
mysql_close($connect);

file_put_contents("pay_user.json", json_encode($alluser));

?>