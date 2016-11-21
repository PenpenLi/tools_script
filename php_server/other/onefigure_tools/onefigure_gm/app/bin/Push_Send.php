<?php
require_once 'DB.php';

$connect_log = array( // 로그
        'phptype'  => 'mysqli',
        'username' => 'tangent',
        'password' => 'xks!@wpsxm!@',
        'hostspec' => '172.31.29.217',
        'port'     => '3306',
        'database' => 'DH_LogDB',
    );

$db_log = & DB::connect($connect_log);
if (DB::isError($db_log)) {
    die ($db_log->getMessage());
} else {
    $db_log->query("SET NAMES UTF8");
    $db_log->setFetchMode(DB_FETCHMODE_ASSOC);
    $db_log->setOption('debug', 9);
}

$sql = 'SELECT * FROM
        dh_push_messages
        WHERE IS_DELETE=0 AND SEND_DATE="' . date('Y-m-d') . '" AND SEND_TIME="' . date('G') . '"';
$result = $db_log->query($sql);

while ($row = & $result->fetchRow()) {
    $title = $row['TITLE']; //제목
    $message = $row['MESSAGE']; //내용
    $os = $row['OS_TYPE'];
    $message_id = $row['MESSAGE_ID'];
}

$apiKey ="AIzaSyDgysKRa4USHTzaBdDryqmDp66PvP0pThc";
$url = 'https://android.googleapis.com/gcm/send';

$headers = array(
    'Authorization: key=' . $apiKey,
    'Content-Type: application/json'
);

if (!empty($message)) {
    // Android
    $where_string = '';

    if ($os == 2 || $os == 3) {
        $where_string = ' AND OS_TYPE=2';

        $sql = 'SELECT COUNT(*) AS TOTAL_COUNT FROM dh_users WHERE STATUS=1 AND PUSH_ID!="" ' . $where_string;
        $result = $db_log->query($sql);
        $dataRow = $result->fetchRow();
        $total_count = $dataRow['TOTAL_COUNT'];

        $sql = 'SELECT PUSH_ID FROM dh_users WHERE STATUS=1 AND PUSH_ID!="" ' . $where_string;
        $result = $db_log->query($sql);
        $count = 0;
        $success_count = 0;
        $fail_count = 0;
        $registrationIDs = array();
        while ($row = & $result->fetchRow()) {
            $registrationIDs[] = $row['PUSH_ID'];

            $count++;

            if ($count % 1000 == 0 || $count == $total_count) {
                $fields = array(
                        'registration_ids' => $registrationIDs,
                        'data'             => array("title" => $title, "msg" => $message),
                    );

                $ch = curl_init();
                curl_setopt($ch, CURLOPT_URL, $url);
                curl_setopt($ch, CURLOPT_POST, true);
                curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
                curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
                curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($fields));
                $post_json = curl_exec($ch);
                curl_close($ch);

                $return_array = json_decode($post_json, true);

                //echo $registrationIDs . "\t" . $return_array['success'] . "\n";
                $registrationIDs = array();

                $success_count = $success_count + $return_array['success'];
                $fail_count = $fail_count + $return_array['failure'];
            }
        }
    }

    // iOS
    $where_string = '';

    if ($os == 1 || $os == 3) {
        $where_string = ' AND OS_TYPE=1';

        $sql = 'SELECT COUNT(*) AS TOTAL_COUNT FROM dh_users WHERE STATUS=1 AND PUSH_ID!="" ' . $where_string;
        $result = $db_log->query($sql);
        $dataRow = $result->fetchRow();
        $total_count = $dataRow['TOTAL_COUNT'];

        $sql = 'SELECT PUSH_ID FROM dh_users WHERE STATUS=1 AND PUSH_ID!="" ' . $where_string;
        $result = $db_log->query($sql);

        $apnsHost = 'gateway.push.apple.com';
        $apnsCert = dirname(__FILE__) . '/aps_production-devolver_dungeonhearts.pem';

        $apnsPort = 2195;

        $payload = array('aps' => array('alert' => $message, 'badge' => 0, 'sound' => 'default'));
        $payload = json_encode($payload);

        $streamContext = stream_context_create();
        stream_context_set_option($streamContext, 'ssl', 'local_cert', $apnsCert);

        $apns = stream_socket_client('ssl://'.$apnsHost.':'.$apnsPort, $error, $errorString, 2, STREAM_CLIENT_CONNECT, $streamContext);

        while ($row = & $result->fetchRow()) {
            if ($apns) {
                $apnsMessage = chr(0).chr(0).chr(32).pack('H*', str_replace('%', '', $row['PUSH_ID'])).chr(0).chr(strlen($payload)).$payload;
                fwrite($apns, $apnsMessage);
                $success_count++;
            } else {
                $fail_count++;
            }

            $count++;
        }
    }

    $sql = 'UPDATE dh_push_messages SET
                FINISH_DATE=NOW(),
                TOTAL_USER_COUNT=' . $count . ',
                SUCCESS_COUNT=' . $success_count . '
            WHERE MESSAGE_ID=' . $message_id;
    $db_log->query($sql);

    echo '[' . date('Y-m-d H:i:s') . '] Total Count : ' . number_format($total_count) . ' / Success : ' . number_format($success_count) . ' / Fail : ' . number_format($fail_count) . "\n";
}

$db_log->disconnect();

