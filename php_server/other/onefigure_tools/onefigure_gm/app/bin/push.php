<?php
require_once 'DB.php';


	function checkAPNSFeedback() {
		   # Initialize stream context
    $streamContext = stream_context_create();
    # Load private key
        $apnsCert = dirname(__FILE__) . '/purple_production.pem';
    stream_context_set_option($streamContext, 'ssl', 'local_cert', $apnsCert);
    # Connect to the Apple Push Notification Service
    $fp = stream_socket_client(
            'ssl://feedback.push.apple.com:2196',
            $errorNumber,
            $errorString,
            60,
            STREAM_CLIENT_CONNECT,
            $streamContext);
    # Connection failed?
    if ($errorString)
      throw new Exception('Can\'t connect to Apple Push Feedback Service: '.$errorString);
    while ($data = fread($fp, 38)) {
      $feedback[] = unpack("N1timestamp/n1length/H*devtoken", $data);
    }
    # Return Feedback
    return $feedback;
    # Close Connection
    fclose($fp);	
	}

    //FUNCTION to check if there is an error response from Apple
    //         Returns TRUE if there was and FALSE if there was not
    function checkAppleErrorResponse($fp) {
    

       $apple_error_response = fread($fp, 6); //byte1=always 8, byte2=StatusCode, bytes3,4,5,6=identifier(rowID). Should return nothing if OK.
       //NOTE: Make sure you set stream_set_blocking($fp, 0) or else fread will pause your script and wait forever when there is no response to be sent.

       if ($apple_error_response) {

            $error_response = unpack('Ccommand/Cstatus_code/Nidentifier', $apple_error_response); //unpack the error response (first byte 'command" should always be 8)

            if ($error_response['status_code'] == '0') {
                $error_response['status_code'] = '0-No errors encountered';

            } else if ($error_response['status_code'] == '1') {
                $error_response['status_code'] = '1-Processing error';

            } else if ($error_response['status_code'] == '2') {
                $error_response['status_code'] = '2-Missing device token';

            } else if ($error_response['status_code'] == '3') {
                $error_response['status_code'] = '3-Missing topic';

            } else if ($error_response['status_code'] == '4') {
                $error_response['status_code'] = '4-Missing payload';

            } else if ($error_response['status_code'] == '5') {
                $error_response['status_code'] = '5-Invalid token size';

            } else if ($error_response['status_code'] == '6') {
                $error_response['status_code'] = '6-Invalid topic size';

            } else if ($error_response['status_code'] == '7') {
                $error_response['status_code'] = '7-Invalid payload size';

            } else if ($error_response['status_code'] == '8') {
                $error_response['status_code'] = '8-Invalid token';

            } else if ($error_response['status_code'] == '255') {
                $error_response['status_code'] = '255-None (unknown)';

            } else {
                $error_response['status_code'] = $error_response['status_code'].'-Not listed';

            }

            echo '<br><b>+ + + + + + ERROR</b> Response Command:<b>' . $error_response['command'] . '</b>&nbsp;&nbsp;&nbsp;Identifier:<b>' . $error_response['identifier'] . '</b>&nbsp;&nbsp;&nbsp;Status:<b>' . $error_response['status_code'] . '</b><br>';
            echo 'Identifier is the rowID (index) in the database that caused the problem, and Apple will disconnect you from server. To continue sending Push Notifications, just start at the next rowID after this Identifier.<br>';

            return true;
       }
       return false;
    }

$connect_log = array( // 로그
        'phptype'  => 'mysqli',
        'username' => 'purple_ofdp_log',
        'password' => 'vjvmf!@xkswpsxmfhrm#$',
        'hostspec' => '10.128.1.16',
        'port'     => '3306',
        'database' => 'Purple_OFDP_LogDB',
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
        op_push_messages
        WHERE (isnull(IS_PROCESSED) or IS_PROCESSED<>1) AND IS_DELETE=0 AND SEND_DATE="' . date('Y-m-d') . '" AND SEND_TIME="' . date('G') . '"';
$result = $db_log->query($sql);

while ($row = & $result->fetchRow()) {
    $title = $row['TITLE']; //제목
    $message = $row['MESSAGE']; //내용
    $os = $row['OS_TYPE'];
    $message_id = $row['MESSAGE_ID'];
    $user_code = $row['USER_CODE'];
}

#$apiKey ="AIzaSyDgysKRa4USHTzaBdDryqmDp66PvP0pThc";
$apiKey ="AIzaSyDHo4Fw8AekkJqm1QKB-4vvwSsrBZS45l4";

$url = 'https://android.googleapis.com/gcm/send';

$headers = array(
    'Authorization: key=' . $apiKey,
    'Content-Type: application/json'
);

if (!empty($message)) {
$connect_user = array( //
        'phptype'  => 'mysqli',
        'username' => 'purple_ofdp_log',
        'password' => 'vjvmf!@xkswpsxmfhrm#$',
        'hostspec' => '10.128.1.16',
        'port'     => '3306',
        'database' => 'Purple_OFDP',
    );

$db_user = & DB::connect($connect_user);
if (DB::isError($db_user)) {
    die ($db_user->getMessage());
} else {
    $db_user->query("SET NAMES UTF8");
    $db_user->setFetchMode(DB_FETCHMODE_ASSOC);
    $db_user->setOption('debug', 9);
}

    $common_where = ' AND USER_CODE>='.$user_code;
//   $common_where .= ' AND ADDDATE(LOGIN_DATE, INTERVAL 3 DAY) >= NOW()';
//    $common_where .= ' AND ( USER_ID in ( 4, 13, 1660, 2935, 79176, 81483))';
    $where_string = $common_where;
    
    // Android
    if ($os == 2 || $os == 3) {
        $where_string .= ' AND OS_TYPE=2';

        $sql = 'SELECT COUNT(*) AS TOTAL_COUNT FROM op_users WHERE STATUS=1 AND PUSH_ID!="" ' . $where_string;
        $result = $db_user->query($sql);
        $dataRow = $result->fetchRow();
        $total_count = $dataRow['TOTAL_COUNT'];
    
        $sql = 'SELECT PUSH_ID FROM op_users WHERE STATUS=1 AND PUSH_ID!="" ' . $where_string;
        $result = $db_user->query($sql);
 //echo $sql;   
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
    
//                echo $registrationIDs . "\t" . $return_array['success'] . "\n";
                $registrationIDs = array();
    
                $success_count = $success_count + $return_array['success'];
                $fail_count = $fail_count + $return_array['failure'];
            }
        }
    }

    // iOS
    $where_string = $common_where;
    
    if ($os == 1 || $os == 3) {

//	$feedback = checkAPNSFeedback();
//	echo count($feedback);

        $where_string .= ' AND OS_TYPE=1';
        
        $sql = 'SELECT COUNT(*) AS TOTAL_COUNT FROM op_users WHERE STATUS=1 AND PUSH_ID!="" ' . $where_string;
        $result = $db_user->query($sql);
        $dataRow = $result->fetchRow();
        $total_count = $dataRow['TOTAL_COUNT'];
    
        $sql = 'SELECT PUSH_ID FROM op_users WHERE STATUS=1 AND PUSH_ID!="" ' . $where_string;
        $result = $db_user->query($sql);
    
        $apnsHost = 'gateway.push.apple.com';
        $apnsCert = dirname(__FILE__) . '/purple_production.pem';
    
        $apnsPort = 2195;
    
        $payload = array('aps' => array('alert' => $message, 'badge' => 0, 'sound' => 'default'));
        $payload = json_encode($payload);
    
        $streamContext = stream_context_create();
        stream_context_set_option($streamContext, 'ssl', 'local_cert', $apnsCert);

 
        $apns = stream_socket_client('ssl://'.$apnsHost.':'.$apnsPort, $error, $errorString, 2, STREAM_CLIENT_CONNECT, $streamContext);
    
	stream_set_blocking( $apns, 0);   

        while ($row = & $result->fetchRow()) {
            if ($apns) {
                $apnsMessage = chr(0).chr(0).chr(32).pack('H*', str_replace('%', '', $row['PUSH_ID'])).chr(0).chr(strlen($payload)).$payload;
                fwrite($apns, $apnsMessage);

		if( checkAppleErrorResponse( $apns) == false)
                	$success_count++;
		else
			$fail_count++;
            } else {
                $fail_count++;
            }
            
            $count++;
        }

	$db_user->disconnect();
    }

    $sql = 'UPDATE op_push_messages SET
		IS_PROCESSED=1,
                FINISH_DATE=NOW(),
                TOTAL_USER_COUNT=' . $count . ',
                SUCCESS_COUNT=' . $success_count . '
            WHERE MESSAGE_ID=' . $message_id;
    $db_log->query($sql);

    echo '[' . date('Y-m-d H:i:s') . '] Total Count : ' . number_format($total_count) . ' / Success : ' . number_format($success_count) . ' / Fail : ' . number_format($fail_count) . "\n";
}

$db_log->disconnect();

