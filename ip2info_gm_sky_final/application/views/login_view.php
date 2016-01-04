<!DOCTYPE html>
<html>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<head><title>登陆</title></head>
<body align="center">
<style type="text/css">
.submitbtn {width:90px;height:40px;font-size:32px}
</style>

<h1>登陆</h1>
<form name="input" action="<?php echo site_url(array('manager', 'gameinfo')); ?>" method="post">
用户
<input type="text" name="username">
<p></p>
密码
<input type="password" name="pw">
<p></p>
<input type="submit" class="submitbtn" value="登陆"/>
</form>
</body>
</html>
