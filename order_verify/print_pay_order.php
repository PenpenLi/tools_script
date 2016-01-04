<?php
include 'db_config.php';


$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
if (!$mysql_connect)
{
	//die('Could not connect: ' . mysql_error());
	error_log("db connect failed" ."\r\n", 3, "/nfsroot/html_onefigure/app/debug.info");
	print_r("db connect failed\r\n");
	exit();
}

mysql_select_db("pay_verify_db", $mysql_connect);

//$sql = "SELECT * FROM op_order_info WHERE VERIFY_STATE='SUCCESS'";
$sql = "SELECT * FROM op_order_info WHERE GAME_NAME='OneFinger' AND VERIFY_STATE='SUCCESS' AND SDK_PAY_TYPE='wali'";
//$sql = "SELECT * FROM op_order_info";
$result = mysql_query($sql);
if (!$result)
{
  //die('Error: ' . mysql_error());
	//error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/app/debug.info");
	print_r("db query failed\r\n");
	exit();
}
$row = mysql_fetch_array($result);
mysql_close($mysql_connect);
//var_dump($row);

echo"<table border=1 cellspacing=0>\n";
//印出栏位名称
echo"<tr>\n";//横行开始（一笔记录开始）
for($i=0;$i<mysql_num_fields($result);$i++)//取得返回栏位的数目
{
  echo"<td nowrap>".mysql_field_name($result,$i)."</td>\n";//返回指定字段的名称，使用范例mysql_field_name($result,2);
}
//将资料回到第一笔
mysql_data_seek($result,0);//移动内部返回指针,语法mysql_data_seek(int result_identifier, int row_number);本函数可移动内部返回的列指针到指定的row_number去。之后若使用mysql_fetch_row()可以返回新列的值
//印出资料内容
while($row=mysql_fetch_row($result))
{
  echo"<tr>\n";
  for($i=0;$i<mysql_num_fields($result);$i++)
  {
    echo"<td nowrap>$row[$i]</td>\n";
  }
  echo"</tr>\n";    
}
//表格结束
echo"</table>";

mysql_free_result($result);

?>
