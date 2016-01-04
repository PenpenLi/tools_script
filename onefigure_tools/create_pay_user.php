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

$jsonstr = file_get_contents("pay_user2.json");
$users = json_decode($jsonstr, true);

$pay_user3 = array();

foreach ($users as $key => $user_row) {
	$user3["email"] = $user_row["email"];
	$user3["fee"] = $user_row["fee"];

	// 1. get baseurl and fenqu
	$baseurl = "";
	$return_content = http_post_data("http://itandroidbalance.miyigame.com:35001/getbaseurl2.php", 
									json_encode(array('email'=>$user_row["email"], 'platform'=>'xiaomi')));
	$ret = json_decode($return_content, true);
	$baseurl = $ret['baseurl'];

	$user3["baseurl"] = $baseurl;

	// 2. register user
	$data = array('email' => $user_row['email'], 'password' => '1003');
	$json_data = json_encode($data);
	$url = $baseurl."/index.php/v22/user/Join";
	print_r($url."\r\n");
	print_r($json_data."\r\n");
	$return_content = http_post_data($url, encrypt_rc4($json_data));
	//print_r(decrypt_rc4($return_content));
	print_r("\r\n");
	print_r("\r\n");


	// 3. login
	$data = array('email' => $user_row['email'], 'password' => '1003', 'device'=>'php', 'os'=>'mac');
	$json_data = json_encode($data);
	$url = $baseurl."/index.php/v22/user/Login";
	print_r($url."\r\n");
	print_r($json_data."\r\n");
	$return_content = http_post_data($url, encrypt_rc4($json_data));
	//print_r(decrypt_rc4($return_content));
	print_r("\r\n");
	print_r("\r\n");

	$user_info = json_decode(decrypt_rc4($return_content), true);
    $user_id  = $user_info["userID"];
    $user3["user_id"] = $user_id;


    array_push($pay_user3, $user3);
}

file_put_contents("pay_user3.json", json_encode($pay_user3));


?>