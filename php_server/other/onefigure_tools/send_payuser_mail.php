<?php
define('CRYPT_SET_CIPHER',              MCRYPT_RIJNDAEL_256);
define('CRYPT_SET_MODE',                MCRYPT_MODE_CBC);
define('CRYPT_KEY',                     '8ac0f0c039c77d2c3a97d2cd447237dc');
define('CRYPT_IV',                      '514365216be9a3cb6fbcfac1f888f6ba');



function crypt_rc4 ($pwd, $data, $ispwdHex = 0)
{
    if ($ispwdHex)
        $pwd = @pack('H*', $pwd); // valid input, please!

    $key[] = '';
    $box[] = '';
    $cipher = '';

    $pwd_length = strlen($pwd);
    $data_length = strlen($data);

    for ($i = 0; $i < 256; $i++)
    {
        $key[$i] = ord($pwd[$i % $pwd_length]);
        $box[$i] = $i;
    }
    for ($j = $i = 0; $i < 256; $i++)
    {
        $j = ($j + $box[$i] + $key[$i]) % 256;
        $tmp = $box[$i];
        $box[$i] = $box[$j];
        $box[$j] = $tmp;
    }
    for ($a = $j = $i = 0; $i < $data_length; $i++)
    {
        $a = ($a + 1) % 256;
        $j = ($j + $box[$a]) % 256;
        $tmp = $box[$a];
        $box[$a] = $box[$j];
        $box[$j] = $tmp;
        $k = $box[(($box[$a] + $box[$j]) % 256)];
        $cipher .= chr(ord($data[$i]) ^ $k);
    }
    return $cipher;
}
function encrypt_rc4 ($data)
{
    return base64_encode(crypt_rc4(CRYPT_KEY, $data));
}

function decrypt_rc4 ($data)
{
    $return = crypt_rc4(CRYPT_KEY, base64_decode($data));
    $return = rtrim($return, "\0\4");

    return $return;
}

function http_post_data($url, $data_string) {
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
        'Content-Type: application/xml; charset=utf-8',
        'Content-Length: ' . strlen($data_string))
    );
    ob_start();
    curl_exec($ch);
    $return_content = ob_get_contents();
    ob_end_clean();
    $return_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    return $return_content;
}

function helper_getInsertSQL($tableName, $dataArray) {
    $str1 = '';
    $str2 = '';
    foreach ($dataArray as $key => $value) {
    if ($value==null)$value=0;
        $str1 = $str1 . $key . ',';
        if (is_string($value)) {
            $str2 = $str2 . "'" . $value . "'" . ',';
        } else {
            $str2 = $str2 . $value . ',';
        }
    }
    $str1 = substr($str1, 0, strlen($str1) - 1);
    $str2 = substr($str2, 0, strlen($str2) - 1);

    $sql = "INSERT INTO " . $tableName . " (" . $str1 . ")" . " VALUES " . "(" . $str2 . ")";

    return $sql;
}

function insertMessage($ip, $data = array()) {
    $connect = mysql_connect($ip, "ofdpuser", "ofdp_onefigure");
    mysql_select_db("ofdp");

    $gift = array();
    if (isset($data['GIFT_TYPE'])) {
        $gift = array(
                'TYPE' => $data['GIFT_TYPE'],
                'ITEM_ID' => empty($data['ITEM_ID']) ? null : $data['ITEM_ID'],
                'AMOUNT' => empty($data['AMOUNT']) ? null : $data['AMOUNT'],
            );
    }
    
    $insert = array(
            'SEND_USER_ID' => $data['SEND_USER_ID'],
            'RECIEVE_USER_ID' => $data['RECEIVE_USER_ID'],
            'EVENT_ID' => empty($data['EVENT_ID']) ? null : $data['EVENT_ID'],
            'TEXT_ID' => empty($data['TEXT_ID']) ? null : $data['TEXT_ID'],
            'GIFT' => json_encode($gift),
            'CREATE_DATE' => date('Y-m-d H:i:s'),
            'CREATE_IP' => "127.0.0.1",
            'IS_DELETE' => 0,
        );

    $sql = helper_getInsertSQL("op_messages", $insert);
    print_r($sql . "\r\n");
    $result = mysql_query($sql);
    mysql_close($connect);
}


$jsonstr = file_get_contents("pay_user3.json");
$users = json_decode($jsonstr, true);

foreach ($users as $key => $user_row) {
    // // 1. get baseurl and fenqu
    // $baseurl = "";
    // $return_content = http_post_data("http://itandroidbalance.miyigame.com:35001/getbaseurl2.php", 
    //                              json_encode(array('email'=>$user_row["email"], 'platform'=>'xiaomi')));
    // $ret = json_decode($return_content, true);
    // $baseurl = $ret['baseurl'];

    // // // 2. register user
    // // $data = array('email' => $user_row['email'], 'password' => '1003');
    // // $json_data = json_encode($data);
    // // $url = $baseurl."/index.php/v22/user/Join";
    // // print_r($url."\r\n");
    // // print_r($json_data."\r\n");
    // // $return_content = http_post_data($url, encrypt_rc4($json_data));
    // // print_r(decrypt_rc4($return_content));
    // // print_r("\r\n");
    // // print_r("\r\n");

    // // 3. login
    // $data = array('email' => $user_row['email'], 'password' => '1003', 'device'=>'php', 'os'=>'mac');
    // $json_data = json_encode($data);
    // $url = $baseurl."/index.php/v22/user/Login";
    // print_r($url."\r\n");
    // print_r($json_data."\r\n");
    // $return_content = http_post_data($url, encrypt_rc4($json_data));
    // //print_r(decrypt_rc4($return_content));
    // print_r("\r\n");
    // print_r("\r\n");

 //    $user_info = json_decode(decrypt_rc4($return_content), true);
    
    $user_id  = intval($user_row["user_id"]);
    $baseurl =  $user_row["baseurl"];
    $fee = $user_row["fee"];

    $amount = '0';
    if ($fee == "100") {
        $amount = '8';
    } elseif ($fee == "600") {
        $amount = '120';
    } elseif ($fee == "1200") {
        $amount = '288';
    } elseif ($fee == "3000") {
        $amount = '780';
    } elseif ($fee == "6800") {
        $amount = '2040';
    } elseif ($fee == "12800") {
        $amount = '4608';
    } else {
        $amount = '' . intval($fee)/10;
    }
    
    $message = array(
        'SEND_USER_ID' => 0,
        'RECEIVE_USER_ID' => $user_id,
        'GIFT_TYPE' => 1,
        'AMOUNT' => $amount,
        'TEXT_ID' => 1000
    );
    $dp_ip = "";
    if ($baseurl == "itofdp4.miyigame.com:40002") {
        $dp_ip = "192.168.169.66";
    } elseif ($baseurl == "itofdp5.miyigame.com:40003") {
        $dp_ip = "192.168.169.68";
    } elseif ($baseurl == "itofdp6.miyigame.com:40004") {
        $dp_ip = "192.168.169.69";
    }
    print_r($dp_ip . "\r\n");
    insertMessage($dp_ip, $message);

    return;
}




?>