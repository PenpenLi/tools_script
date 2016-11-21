<?php
$connect = mysql_connect("192.168.169.66", "ofdpuser", "ofdp_onefigure");
mysql_select_db("ofdp");

$sql = "select email from op_users";
$result = mysql_query($sql);
while (($row = mysql_fetch_assoc($result))) {
	$email = $row['email'];
	echo $email . "\r\n";
}
mysql_close($connect);

?>
