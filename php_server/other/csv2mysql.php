<?php
/*
解决mysql中文乱码的问题

CREATE DATABASE `ip2info` CHARACTER SET 'utf8' COLLATE 'utf8_general_ci';

CREATE TABLE `op_ip` (
   	`ID` bigint(32) NOT NULL AUTO_INCREMENT,
    `START_IP` bigint(32) NOT NULL,
    `END_IP` bigint(32) NOT NULL,
    `PROVINCE` varchar(40) CHARACTER SET utf8,
    `TYPE` varchar(32) CHARACTER SET utf8,
    PRIMARY KEY (`ID`)
 ) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `op_gameinfo` (
    `ID` bigint(32) NOT NULL AUTO_INCREMENT,
    `PROVINCE` varchar(40) CHARACTER SET utf8,
    `GAME_NAME` varchar(40) CHARACTER SET utf8,
    `PLATFORM` varchar(40) CHARACTER SET utf8,
    `WIRELESS_TYPE` varchar(40) CHARACTER SET utf8,
    `ITEM_KEY` varchar(40) CHARACTER SET utf8,
    `ITEM_VALUE` varchar(10240) CHARACTER SET utf8,
    PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
*/

$mysql_connect = mysql_connect("127.0.0.1", "root", "root123!");
mysql_select_db("ip2info", $mysql_connect);

mysql_query("SET NAMES 'UTF8'");
mysql_query("SET CHARACTER SET UTF8");
mysql_query("SET CHARACTER_SET_RESULTS='UTF8'");


if (($handle = fopen("ipinfo.csv", "r")) !== FALSE) {
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
        $START_IP = intval($data[0]);
        $END_IP = intval($data[1]);
        $PROVINCE = $data[2];
        $TYPE = $data[3];

        $sql = "INSERT INTO op_ip (START_IP,END_IP,PROVINCE,TYPE) VALUES ($START_IP, $END_IP, '$PROVINCE', '$TYPE')";
        echo $sql . "\n";
        mysql_query($sql, $mysql_connect);
    }
    fclose($handle);
}

mysql_close($mysql_connect);

?>