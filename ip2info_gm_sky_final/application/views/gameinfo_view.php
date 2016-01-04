<!DOCTYPE html>
<html>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<head><title>游戏配置信息</title></head>
<body align="center">

<style type="text/css">
p {line-height:50px;}
th {width:120px;height:40px;font-size:20px;}
td {width:120px;height:40px;font-size:18px;}
select {width:120px;height:40px;font-size:24px;}
.managerbtn {width:180px;height:40px;font-size:24px;}
.submitbtn {width:100px;height:40px;font-size:32px}
</style>

<h1>配置管理</h1>
<p></p>
<HR style="FILTER: alpha(opacity=100,finishopacity=0,style=1)" width="80%" color=#987cb9 SIZE=10>
<p></p>
<!-- <div>
<button type="button" class="managerbtn">添加</button>
<button type="button" class="managerbtn">修改</button>
<button type="button" class="managerbtn">删除</button>
</div> -->


<form name="input" action="<?php echo site_url(array('manager', 'query')); ?>" method="post">
<select name="game_name" align="center" border="2"> 
<option value="no_limit">不限</option>
<option value="bbr">bbr</option> 
<option value="ramboat">ramboat</option> 
<option value="onefigure">onefigure</option> 
<option value="gunfinger">gunfinger</option>
</select>

<select name="platform" align="center" border="2"> 
<option value="no_limit">不限</option>
<option value="360">360</option> 
<option value="ios">ios</option> 
<option value="wali">wali</option> 
<option value="baidu">baidu</option>
<option value="wandoujia">wandoujia</option>
<option value="tencentApp">tencentApp</option>
<option value="uc">uc</option>
</select>

<select name="province" align="center" border="2"> 
<option value="no_limit">不限</option>
<option value="北京市">北京市</option> 
<option value="天津市">天津市</option> 
<option value="上海市">上海市</option> 
<option value="重庆市">重庆市</option>
<option value="山西省">山西省</option>
<option value="辽宁省">辽宁省</option>
<option value="吉林省">吉林省</option>
<option value="黑龙江省">黑龙江省</option> 
<option value="江苏省">江苏省</option> 
<option value="浙江省">浙江省</option> 
<option value="安徽省">安徽省</option>
<option value="福建省">福建省</option>
<option value="江西省">江西省</option>
<option value="山东省">山东省</option>
<option value="河南省">河南省</option> 
<option value="湖北省">湖北省</option> 
<option value="湖南省">湖南省</option> 
<option value="海南省">海南省</option>
<option value="四川省">四川省</option>
<option value="贵州省">贵州省</option>
<option value="云南省">云南省</option>
<option value="陕西省">陕西省</option> 
<option value="甘肃省">甘肃省</option> 
<option value="青海省">青海省</option> 
<option value="广西壮族自治区">广西壮族自治区</option>
<option value="内蒙古自治区">内蒙古自治区</option>
<option value="西藏自治区">西藏自治区</option>
<option value="宁夏回族自治区">宁夏回族自治区</option>
<option value="新疆维吾尔自治区">新疆维吾尔自治区</option>
</select>

<select name="wireless_type" align="center" border="2"> 
<option value="no_limit">不限</option>
<option value="移动">移动</option> 
<option value="联通">联通</option> 
<option value="电信">电信</option> 
</select>

<select name="item_key" align="center" border="2"> 
<option value="no_limit">不限</option>
<option value="bbr_Dialog_StarterPackEx">bbr弹窗_新手礼包</option> 
<option value="bbr_Dialog_Buy_MonsterTruck_Pack">bbr弹窗_蓝翔礼包</option> 
<option value="bbr_Dialog_Buy_MonsterTruck_Pack_Discount">bbr弹窗_蓝翔打折礼包</option> 
<option value="bbr_Dialog_Tuhao_PackByPC">bbr弹窗_土豪礼包</option> 
<option value="bbr_Dialog_CarMoon">bbr弹窗_月球漫步者</option> 
<option value="bbr_Dialog_SuperVip_Pack">bbr弹窗_超级vip</option> 
<option value="bbr_Button_StarterPackEx">bbr按钮_新手礼包</option> 
<option value="bbr_Button_Dialog_Tuhao_Pack">bbr按钮_土豪礼包</option> 
<option value="bbr_Button_Dialog_CarMoon">bbr按钮_月球漫步者</option> 
<option value="bbr_Button_Dialog_SuperVip_Pack">bbr按钮_超级vip</option> 
</select>

<select name="item_value" align="center" border="2"> 
<!-- <option value="0">不限</option>
<option value="1">0</option> 
<option value="2">1</option> 
<option value="3">2</option> 
<option value="4">3</option> 
<option value="5">4</option> 
<option value="6">5</option>  -->
</select>
<input type="submit" class="submitbtn" value="查询"/>
</form>

<p></p>
<HR style="FILTER: alpha(opacity=100,finishopacity=0,style=1)" width="80%" color=#987cb9 SIZE=10>
<p></p>
<form name="input" action="<?php echo site_url(array('manager', 'add')); ?>" method="post">
<select name="game_name" align="center" border="2"> 
<option value="bbr">bbr</option> 
<option value="ramboat">ramboat</option> 
<option value="onefigure">onefigure</option> 
<option value="gunfinger">gunfinger</option>
</select>

<select name="platform" align="center" border="2"> 
<option value="360">360</option> 
<option value="ios">ios</option> 
<option value="wali">wali</option> 
<option value="baidu">baidu</option>
<option value="wandoujia">wandoujia</option>
<option value="tencentApp">tencentApp</option>
<option value="uc">uc</option>
</select>

<select name="province" align="center" border="2"> 
<option value="北京市">北京市</option> 
<option value="天津市">天津市</option> 
<option value="上海市">上海市</option> 
<option value="重庆市">重庆市</option>
<option value="山西省">山西省</option>
<option value="辽宁省">辽宁省</option>
<option value="吉林省">吉林省</option>
<option value="黑龙江省">黑龙江省</option> 
<option value="江苏省">江苏省</option> 
<option value="浙江省">浙江省</option> 
<option value="安徽省">安徽省</option>
<option value="福建省">福建省</option>
<option value="江西省">江西省</option>
<option value="山东省">山东省</option>
<option value="河南省">河南省</option> 
<option value="湖北省">湖北省</option> 
<option value="湖南省">湖南省</option> 
<option value="海南省">海南省</option>
<option value="四川省">四川省</option>
<option value="贵州省">贵州省</option>
<option value="云南省">云南省</option>
<option value="陕西省">陕西省</option> 
<option value="甘肃省">甘肃省</option> 
<option value="青海省">青海省</option> 
<option value="广西壮族自治区">广西壮族自治区</option>
<option value="内蒙古自治区">内蒙古自治区</option>
<option value="西藏自治区">西藏自治区</option>
<option value="宁夏回族自治区">宁夏回族自治区</option>
<option value="新疆维吾尔自治区">新疆维吾尔自治区</option>
</select>

<select name="wireless_type" align="center" border="2"> 
<option value="移动">移动</option> 
<option value="联通">联通</option> 
<option value="电信">电信</option> 
</select>

<select name="item_key" align="center" border="2"> 
<option value="bbr_Dialog_StarterPackEx">bbr弹窗_新手礼包</option> 
<option value="bbr_Dialog_Buy_MonsterTruck_Pack">bbr弹窗_蓝翔礼包</option> 
<option value="bbr_Dialog_Buy_MonsterTruck_Pack_Discount">bbr弹窗_蓝翔打折礼包</option> 
<option value="bbr_Dialog_Tuhao_PackByPC">bbr弹窗_土豪礼包</option> 
<option value="bbr_Dialog_CarMoon">bbr弹窗_月球漫步者</option> 
<option value="bbr_Dialog_SuperVip_Pack">bbr弹窗_超级vip</option> 
<option value="bbr_Button_StarterPackEx">bbr按钮_新手礼包</option> 
<option value="bbr_Button_Dialog_Tuhao_Pack">bbr按钮_土豪礼包</option> 
<option value="bbr_Button_Dialog_CarMoon">bbr按钮_月球漫步者</option> 
<option value="bbr_Button_Dialog_SuperVip_Pack">bbr按钮_超级vip</option> 
</select>

<select name="item_value" align="center" border="2"> 
<option value="1">弹窗打开</option>
<option value="0">弹窗关闭</option>
<option value="2">礼包领取</option>
<option value="3">礼包购买</option>
</select>
<input type="submit" class="submitbtn" value="添加"/>
</form>


<p></p>
<HR style="FILTER: alpha(opacity=100,finishopacity=0,style=1)" width="80%" color=#987cb9 SIZE=10>
<p></p>
<form name="input" action="<?php echo site_url(array('manager', 'modify')); ?>" method="post">
<select name="game_name" align="center" border="2"> 
<option value="bbr">bbr</option> 
<option value="ramboat">ramboat</option> 
<option value="onefigure">onefigure</option> 
<option value="gunfinger">gunfinger</option>
</select>

<select name="platform" align="center" border="2"> 
<option value="360">360</option> 
<option value="ios">ios</option> 
<option value="wali">wali</option> 
<option value="baidu">baidu</option>
<option value="wandoujia">wandoujia</option>
<option value="tencentApp">tencentApp</option>
<option value="uc">uc</option>
</select>

<select name="province" align="center" border="2"> 
<option value="北京市">北京市</option> 
<option value="天津市">天津市</option> 
<option value="上海市">上海市</option> 
<option value="重庆市">重庆市</option>
<option value="山西省">山西省</option>
<option value="辽宁省">辽宁省</option>
<option value="吉林省">吉林省</option>
<option value="黑龙江省">黑龙江省</option> 
<option value="江苏省">江苏省</option> 
<option value="浙江省">浙江省</option> 
<option value="安徽省">安徽省</option>
<option value="福建省">福建省</option>
<option value="江西省">江西省</option>
<option value="山东省">山东省</option>
<option value="河南省">河南省</option> 
<option value="湖北省">湖北省</option> 
<option value="湖南省">湖南省</option> 
<option value="海南省">海南省</option>
<option value="四川省">四川省</option>
<option value="贵州省">贵州省</option>
<option value="云南省">云南省</option>
<option value="陕西省">陕西省</option> 
<option value="甘肃省">甘肃省</option> 
<option value="青海省">青海省</option> 
<option value="广西壮族自治区">广西壮族自治区</option>
<option value="内蒙古自治区">内蒙古自治区</option>
<option value="西藏自治区">西藏自治区</option>
<option value="宁夏回族自治区">宁夏回族自治区</option>
<option value="新疆维吾尔自治区">新疆维吾尔自治区</option>
</select>

<select name="wireless_type" align="center" border="2"> 
<option value="移动">移动</option> 
<option value="联通">联通</option> 
<option value="电信">电信</option> 
</select>

<select name="item_key" align="center" border="2"> 
<option value="bbr_Dialog_StarterPackEx">bbr弹窗_新手礼包</option> 
<option value="bbr_Dialog_Buy_MonsterTruck_Pack">bbr弹窗_蓝翔礼包</option> 
<option value="bbr_Dialog_Buy_MonsterTruck_Pack_Discount">bbr弹窗_蓝翔打折礼包</option> 
<option value="bbr_Dialog_Tuhao_PackByPC">bbr弹窗_土豪礼包</option> 
<option value="bbr_Dialog_CarMoon">bbr弹窗_月球漫步者</option> 
<option value="bbr_Dialog_SuperVip_Pack">bbr弹窗_超级vip</option> 
<option value="bbr_Button_StarterPackEx">bbr按钮_新手礼包</option> 
<option value="bbr_Button_Dialog_Tuhao_Pack">bbr按钮_土豪礼包</option> 
<option value="bbr_Button_Dialog_CarMoon">bbr按钮_月球漫步者</option> 
<option value="bbr_Button_Dialog_SuperVip_Pack">bbr按钮_超级vip</option> 
</select>

<select name="item_value" align="center" border="2"> 
<option value="1">弹窗打开</option>
<option value="0">弹窗关闭</option>
<option value="2">礼包领取</option>
<option value="3">礼包购买</option>
</select>
<input type="submit" class="submitbtn" value="修改"/>
</form>


<p></p>
<HR style="FILTER: alpha(opacity=100,finishopacity=0,style=1)" width="80%" color=#987cb9 SIZE=10>
<p></p>
<form name="input" action="<?php echo site_url(array('manager', 'remove')); ?>" method="post">
<select name="game_name" align="center" border="2"> 
<option value="bbr">bbr</option> 
<option value="ramboat">ramboat</option> 
<option value="onefigure">onefigure</option> 
<option value="gunfinger">gunfinger</option>
</select>

<select name="platform" align="center" border="2"> 
<option value="360">360</option> 
<option value="ios">ios</option> 
<option value="wali">wali</option> 
<option value="baidu">baidu</option>
<option value="wandoujia">wandoujia</option>
<option value="tencentApp">tencentApp</option>
<option value="uc">uc</option>
</select>

<select name="province" align="center" border="2"> 
<option value="北京市">北京市</option> 
<option value="天津市">天津市</option> 
<option value="上海市">上海市</option> 
<option value="重庆市">重庆市</option>
<option value="山西省">山西省</option>
<option value="辽宁省">辽宁省</option>
<option value="吉林省">吉林省</option>
<option value="黑龙江省">黑龙江省</option> 
<option value="江苏省">江苏省</option> 
<option value="浙江省">浙江省</option> 
<option value="安徽省">安徽省</option>
<option value="福建省">福建省</option>
<option value="江西省">江西省</option>
<option value="山东省">山东省</option>
<option value="河南省">河南省</option> 
<option value="湖北省">湖北省</option> 
<option value="湖南省">湖南省</option> 
<option value="海南省">海南省</option>
<option value="四川省">四川省</option>
<option value="贵州省">贵州省</option>
<option value="云南省">云南省</option>
<option value="陕西省">陕西省</option> 
<option value="甘肃省">甘肃省</option> 
<option value="青海省">青海省</option> 
<option value="广西壮族自治区">广西壮族自治区</option>
<option value="内蒙古自治区">内蒙古自治区</option>
<option value="西藏自治区">西藏自治区</option>
<option value="宁夏回族自治区">宁夏回族自治区</option>
<option value="新疆维吾尔自治区">新疆维吾尔自治区</option>
</select>

<select name="wireless_type" align="center" border="2"> 
<option value="移动">移动</option> 
<option value="联通">联通</option> 
<option value="电信">电信</option> 
</select>

<select name="item_key" align="center" border="2"> 
<option value="bbr_Dialog_StarterPackEx">bbr弹窗_新手礼包</option> 
<option value="bbr_Dialog_Buy_MonsterTruck_Pack">bbr弹窗_蓝翔礼包</option> 
<option value="bbr_Dialog_Buy_MonsterTruck_Pack_Discount">bbr弹窗_蓝翔打折礼包</option> 
<option value="bbr_Dialog_Tuhao_PackByPC">bbr弹窗_土豪礼包</option> 
<option value="bbr_Dialog_CarMoon">bbr弹窗_月球漫步者</option> 
<option value="bbr_Dialog_SuperVip_Pack">bbr弹窗_超级vip</option> 
<option value="bbr_Button_StarterPackEx">bbr按钮_新手礼包</option> 
<option value="bbr_Button_Dialog_Tuhao_Pack">bbr按钮_土豪礼包</option> 
<option value="bbr_Button_Dialog_CarMoon">bbr按钮_月球漫步者</option> 
<option value="bbr_Button_Dialog_SuperVip_Pack">bbr按钮_超级vip</option> 
</select>

<select name="item_value" align="center" border="2"> 
</select>
<input type="submit" class="submitbtn" value="删除"/>
</form>
<p></p><p></p><p></p>

<p></p>
<HR style="FILTER: alpha(opacity=100,finishopacity=0,style=1)" width="80%" color=#987cb9 SIZE=10>
<p></p>

<table border="2" align="center">
	<tr>
    <th>游戏名</th>
    <th>平台</th>
    <th>省份</th>
    <th>运营商</th>
    <th>配置项</th>
    <th>配置值</th>
    <th>更新时间</th>
  	</tr>
  	<?php foreach ($items as $item): ?>
	<tr>
	    <td><?php echo $item['GAME_NAME']; ?></td>
	    <td><?php echo $item['PLATFORM']; ?></td>
	    <td><?php echo $item['PROVINCE']; ?></td>
	    <td><?php echo $item['WIRELESS_TYPE']; ?></td>
	    <td><?php echo $keysmap[$item['ITEM_KEY']]; ?></td>
	    <td><?php echo $keysmap[$item['ITEM_VALUE']]; ?></td>
	    <td><?php echo $item['UPDATE_TIME']; ?></td>
	</tr>
	<?php endforeach; ?>

</table>

</body>
</html>

