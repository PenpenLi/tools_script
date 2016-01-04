<?php

$db_ip = '127.0.0.1';

$db_user = 'root';

$db_pw = 'root123!';


function phplog($str) {
	error_log($str ."\r\n", 3, "/home/www/order_verify/php.log");
}

?>
