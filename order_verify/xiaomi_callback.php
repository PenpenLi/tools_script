<?php
include 'db_config.php';
include_once 'xiaomi_lib/HttpHelper.php';
include_once 'xiaomi_lib/SignatureHelper.php';
include_once 'xiaomi_lib/Configuration.php';

/**
 * 验证登陆信息接口
 * @param type $userId
 * @param type $session
 */
function verifySession($userId,$session) {
    
    $params = array('appId' => Configuration::APP_ID,'uid' => $userId, 'session' => $session);
    
    $signObj = new SignatureHelper();
    
    $signature = $signObj->sign($params, Configuration::SECRET_KEY);
    
    $params['signature'] = $signature;
    
    $request = new HttpHelper();
    $response = $request->get(Configuration::VERIFY_SESSION_URL, $params);
    
    //TODO: 后续业务逻辑处理
    echo $response;
}

/**
 * 查询订单接口
 * @param type $userId
 * @param type $cpOrderId
 */
function queryOrder($userId,$cpOrderId) {
    
    $params = array('appId' => Configuration::APP_ID,'uid' => $userId, 'cpOrderId' => $cpOrderId);
    $signObj = new SignatureHelper();
    
    $signature = $signObj->sign($params, Configuration::SECRET_KEY);
    
    $params['signature'] = $signature;
    
    $request = new HttpHelper();
    $response = $request->get(Configuration::QUERY_ORDER_URL, $params);
    
    //TODO: 后续业务逻辑处理
    echo $response;
}

/**
 * 接收订单通知接口
 */

$reqparams = $_REQUEST;
$params = array();
$httpHelper = new HttpHelper();
foreach ($reqparams as $key => $value) {
    if($key != 'signature'){
        $params[$key] = $httpHelper->urlDecode($value);
    }
}
$signature = $reqparams['signature'];

$signObj = new SignatureHelper();

if($signObj->verifySignature($params, $signature, Configuration::SECRET_KEY)){
    //TODO:后续业务逻辑处理
    echo '{"errcode":200}';
}else{
   echo '{"errcode":1525}';
}

// update database
$mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
if (!$mysql_connect)
{
    //die('Could not connect: ' . mysql_error());
    error_log("db connect failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
    exit();
}

mysql_select_db("pay_verify_db", $mysql_connect);

$sql="INSERT INTO op_xiaomi_ntf (appId, cpOrderId, cpUserInfo, uid, orderId, orderStatus, payFee, productCode, productName, productCount, payTime, orderConsumeType, partnerGiftConsume)
VALUES
('$params[appId]', '$params[cpOrderId]', '$params[cpUserInfo]', '$params[uid]', '$params[orderId]', '$params[orderStatus]', '$params[payFee]', '$params[productCode]', '$params[productName]', '$params[productCount]', '$params[payTime]', '$params[orderConsumeType]', '$params[partnerGiftConsume]')";

if (!mysql_query($sql, $mysql_connect))
{
  //die('Error: ' . mysql_error());
    error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
    exit();
}

// update op_order_info 
$sql = "UPDATE op_order_info SET VERIFY_STATE='SUCCESS', TRANSACTION_ID='$params[orderId]' WHERE ORDER_ID='$params[cpOrderId]'";
if (!mysql_query($sql))
{
  //die('Error: ' . mysql_error());
    error_log("db query failed" ."\r\n", 3, "/nfsroot/html_onefigure/debug.info");
    exit();
}

mysql_close($mysql_connect);


?>

