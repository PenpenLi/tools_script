<?php

include 'db_config.php';

define('CRYPT_SET_CIPHER',              MCRYPT_RIJNDAEL_256);
define('CRYPT_SET_MODE',                MCRYPT_MODE_CBC);
define('CRYPT_KEY',                     '8ac0f0c039c77d2c3a97d2cd447237dc');
define('CRYPT_IV',                      '514365216be9a3cb6fbcfac1f888f6ba');

function encrypt_string($string) {
    $return = mcrypt_encrypt(MCRYPT_RIJNDAEL_256, CRYPT_KEY, $string, MCRYPT_MODE_CBC, CRYPT_IV);
    $return = base64_encode($return);
    return $return;
}

function decrypt_string($string) {
    $string = base64_decode($string);
    $return = mcrypt_decrypt(MCRYPT_RIJNDAEL_256, CRYPT_KEY, $string, MCRYPT_MODE_CBC, CRYPT_IV);
    $return = rtrim($return, "\0\4");
    return $return;
}

$postdata = file_get_contents("php://input");
if ($postdata == '')
{
    print_r('post data is blank..');
    exit();
}

$msg = json_decode($postdata, true);
$email = $msg['email'];
//$email = 'chris111';

if ($email == '') {
    print_r('data format wrong');
    exit();
}

$redis = new Redis();

$ret = $redis->connect('127.0.0.1', 6379);
if (false == $ret) {
   print_r('redis connect fail');
   exit();
}

$baseurl = 'itofdp2.miyigame.com:35002';
$url = '1';
$userCount = 0;
if (false == $redis->exists('onefigure_ios_user_count')) {
    $mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
    if (!$mysql_connect) {
        //error_log("db connect failed" ."\r\n", 3, "/tmp/debug.info");
        print_r('db connect failed 1');
	exit();
    }
    mysql_select_db("fenqu", $mysql_connect);
    $sql = "SELECT * FROM op_user";


    $result = mysql_query($sql, $mysql_connect);
    $userCount = mysql_num_rows($result);
    mysql_close($mysql_connect);
    $redis->set('onefigure_ios_user_count', $userCount);

} else {
    $userCount = intval($redis->get('onefigure_ios_user_count'));
}

// no such email, create a new user account
if (false == $redis->hExists('onefigure_email2url', $email)) {
    $mysql_connect = mysql_connect($db_ip, $db_user, $db_pw);
    if (!$mysql_connect) {
        //error_log("db connect failed" ."\r\n", 3, "/tmp/debug.info");
        print_r('db connect failed 2');
        exit();
    }
    mysql_select_db("fenqu", $mysql_connect);

    $sql = "SELECT * FROM op_user WHERE EMAIL='$email'";
    $result = mysql_query($sql);
    $row = mysql_fetch_array($result, MYSQL_ASSOC);
    // this user is exist
    if ($row) 
    {
        $url = $row['URL'];
    }
    // no such user, create a new one
    else 
    {
        if ($userCount >=0 && $userCount <= 10000) {
            $url = '1';
        } else if ($userCount > 10000 && $userCount <= 20000) {
            $url = '2';
        } else if ($userCount > 20000 && $userCount <= 30000) {
            $url = '3';
        }
        $sql = "INSERT INTO op_user(EMAIL, URL) VALUES('$email', '$url')";

        if (!mysql_query($sql, $mysql_connect)) {
            //error_log("db query failed" ."\r\n", 3, "/tmp/debug.info");
            print_r('db query failed');
            exit();
        }
    }

    $redis->hSet('onefigure_email2url', $email, $url);
    mysql_close($mysql_connect);
    $userCount = $userCount + 1;
    $redis->set('onefigure_ios_user_count', $userCount);

} else {
    $url = $redis->hGet('onefigure_email2url', $email);
}


if ('1' == $url) {
    $baseurl = 'itofdp2.miyigame.com:35002';
} elseif ('2' == $url) {
    $baseurl = 'itofdp2.miyigame.com:35002';
} elseif ('3' == $url) {
    $baseurl = 'itofdp2.miyigame.com:35002';
} elseif ('4' == $url) {
    $baseurl = 'itofdp2.miyigame.com:35002';
} else {
    $baseurl = 'itofdp2.miyigame.com:35002';
}

$redis->close();

echo json_encode(array('baseurl' => $baseurl, 'userCount' => $userCount));

?>

