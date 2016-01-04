<?php
/*
define('CRYPT_SET_CIPHER',              MCRYPT_RIJNDAEL_256);
define('CRYPT_SET_MODE',                MCRYPT_MODE_CBC);
define('CRYPT_KEY',                     '8ac0f0c039c77d2c3a97d2cd447237dc');
define('CRYPT_IV',                      '514365216be9a3cb6fbcfac1f888f6ba');
*/
function encrypt_string($string) {
    //$return = mcrypt_encrypt(MCRYPT_RIJNDAEL_256, CRYPT_KEY, $string, MCRYPT_MODE_CBC, CRYPT_IV);
    $return = $string;
    //$return = base64_encode($return);

    return $return;
}

function decrypt_string($string) {
    //$string = base64_decode($string);
    //$return = mcrypt_decrypt(MCRYPT_RIJNDAEL_256, CRYPT_KEY, $string, MCRYPT_MODE_CBC, CRYPT_IV);
    $return = $string;
    //$return = rtrim($return, "\0\4");

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
    return array($return_code, $return_content);
}

/*
                'EMAIL' => $this->_input['email'],
                'PASSWORD' => $this->_input['password'],
                'DEVICE' => $this->_input['device'],
                'OS_TYPE' => $this->_input['os'],
                'OS_VERSION' => $this->_input['osVersion'],
                'MARKET_TYPE' => $this->_input['market'],
                'PUSH_ID' => $this->_input['pushID'],
                'CELLPHONE' => $this->_input['cellphone'],
                'LANGUAGE' => $this->_input['language'],
*/

$url1 = "http://123.59.34.11:9001/index.php/v20/user/Login";
$url2 = "http://123.59.34.163:9001/index.php/v20/user/Login";
$url = $url1;

$json_data = '{"email":"chris1","password":"chris1234"}';
// $json_data = '{"userID":"chris1"}';
$data_string = encrypt_string($json_data);

while (true) {
    $time = explode ( " ", microtime () );  
    $time = $time [1] . ($time [0] * 1000);  
    $time2 = explode ( ".", $time );  
    $time = $time2 [0];  
    $timeStart = $time;

    $http_connect_num = 1000;
    $curl_arr = array(); 
    $master = curl_multi_init(); 
    
    for($i = 0; $i < $http_connect_num; $i++) { 
        if ($i % 2 == 0) {
            $url = $url1;
        } else {
            $url = $url2;
        }
        $curl_arr[$i] = curl_init($url); 
        curl_setopt($curl_arr[$i], CURLOPT_POST, 1);
        curl_setopt($curl_arr[$i], CURLOPT_POSTFIELDS, $data_string);
        curl_setopt($curl_arr[$i], CURLOPT_RETURNTRANSFER, true); 
        curl_multi_add_handle($master, $curl_arr[$i]); 
    } 
    
    do { 
        curl_multi_exec($master,$running); 
    } while($running > 0); 
    
    $time = explode ( " ", microtime () );  
    $time = $time [1] . ($time [0] * 1000);  
    $time2 = explode ( ".", $time );  
    $time = $time2 [0];  
    $timeEnd = $time;

    echo $http_connect_num . ' reqs cost: ' . ($timeEnd - $timeStart) . "\r\n";

    // echo "results: \r\n"; 
    for($i = 0; $i < $http_connect_num; $i++) 
    { 
        $results = curl_multi_getcontent($curl_arr[$i]); 
        $results = decrypt_string($results);
        //echo($results . "\n"); 
        //echo ($i."\n");
    }



    //sleep(1);
}


//list($return_code, $return_content) = http_post_data($url, $xml);
//print_r($return_content);print_r("\r\n");
//$data = array('EMAIL' => 'chris');
//$json_data = json_encode($data);
// $url = "http://ofdp.miyigame.com:9001/index.php/v20/user/Join";
// $json_data = '{"email":"chris1","password":"chris1234"}';
//print_r($json_data);
// print_r(encrypt_string($json_data));
//print_r('finish!!');
// $ret = http_post_data($url, encrypt_string($json_data));
// print_r(decrypt_string($ret));

?>


