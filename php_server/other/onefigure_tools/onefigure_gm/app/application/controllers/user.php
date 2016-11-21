<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class User extends CI_Controller {
    public function __construct() {
        parent::__construct();
        $this->load->library('session');
        if (isset($_SESSION['db_group'])) {
            $this->load->model(array('User_data', 'Character_data', 'Skill_data', 'Treasure_data', 'Game_data', 'Item_data', 'Shop_data','Message_data'));
        }
        $this->load->library('Auth', array('LEVEL' => 1));
    }

    public function index() {
        $this->viewer->all('user/find');
    }

    public function view($user_id = null) {
        $data = array(
                'USER_ID' => $user_id,
            );
        $userRows = $this->User_data->selectUser($data);

        if (empty($userRows[0]['USER_ID'])) {
            js_alert_back('Empty no data.');
        } else {
            $data['USER_ROW'] = $userRows[0];

            $data['INSTANT_ITEM_ROWS'] = $this->Item_data->selectUserInstantItems($data);
            $data['CHARACTER_ROWS'] = $this->Character_data->selectUserCharacters($data);
            $data['SKILL_ROWS'] = $this->Skill_data->selectUserSkills($data);
            $data['TREASURE_ROWS'] = $this->Treasure_data->selectUserTreasures($data);
            $data['STAGE_ROWS'] = $this->Game_data->selectUserStages($data, null);
            $data['ACHIEVEMENT_ROWS'] = $this->Game_data->selectUserAchievements($data, null);

            $messages = $this->Message_data->selectMessages($data);
            $data['USER_MESSAGES'] = $messages;
        }

        $this->viewer->all('user/view', $data);
    }

    public function http_post_data($url, $data_string) {
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

    public function find($email) {
        $data = array(
                'EMAIL' => rawurldecode($email),
            );
        $return_content = http_post_data("http://itandroidbalance.miyigame.com/getbaseurl2.php", 
                                    json_encode(array('email'=>$email, 'platform'=>'xiaomi')));
        $return = json_decode($return_content, true);
        $baseurl =  $return["baseurl"];
        $db_group_var = "db1";
        if ($baseurl == "itofdp4.miyigame.com:40002") {
            $db_group_var = "db1";
        } elseif ($baseurl == "itofdp5.miyigame.com:40003") {
            $db_group_var = "db2";
        } elseif ($baseurl == "itofdp6.miyigame.com:40004") {
            $db_group_var = "db3";
        }
        $this->session->set_tempdata('db_group', $db_group_var, 600);
        $this->load->model(array('User_data', 'Character_data', 'Skill_data', 'Treasure_data', 'Game_data', 'Item_data', 'Shop_data','Message_data'));
        $userRows = $this->User_data->selectUser($data);

        if (count($userRows) == 0) {
            js_alert_back('Empty no data.');
        } else if (count($userRows) == 1) {
            js_redirect(site_url(array('user', 'view', $userRows[0]['USER_ID'])));
        } else {
            $data['USER_ROWS'] = $userRows;

            $this->viewer->all('user/find', $data);
        }
    }

    public function execAddPoint($user_id = 0) {
        $amount = $this->input->post('amount');
        $point_type = $this->input->post('point_type');
        $memo = $this->input->post('memo');

        $data = array(
            'USER_ID' => $user_id,
        );

        if ($amount == 0) {
            js_alert_back('Invalid amount.');
        } else if ($amount > 0) {
            $category = 99;
        } else {
            $category = -99;
        }

        if ($point_type == 'MONEY') {
            $this->Shop_data->updateUserPoint(1, $amount, $category, $user_id, $memo);
        } else if ($point_type == 'CASH') {
            $this->Shop_data->updateUserPoint(2, $amount, $category, $user_id, $memo);
        } else if ($point_type == 'LOTTERY_POINT') {
            $this->Shop_data->updateUserPoint(3, $amount, $category, $user_id, $memo);
        } else if ($point_type == 'LOTTERY_COUPON') {
            $this->Shop_data->updateUserPoint(4, $amount, $category, $user_id, $memo);
        } else if ($point_type == 'LOTTERY_HIGH_COUPON') {
            $this->Shop_data->updateUserPoint(5, $amount, $category, $user_id, $memo);
        } else {
            $this->Shop_data->updateUserPoint($data);
        }

        js_alert_redirect('처리가 완료 되었습니다.', site_url(array('user', 'view', $user_id)));
    }

    // gift type
    // 0 money
    // 1 cash
    // 2 heart
    // 5 gift type
    // 6 lottery_point
    // 7 exp
    public function execSendMailCash($user_id = 0) {
        $amount = $this->input->post('amount');
        $message = array(
            'SEND_USER_ID' => 0,
            'RECEIVE_USER_ID' => $user_id,
            'GIFT_TYPE' => 1,
            'AMOUNT' => $amount,
            'TEXT_ID' => 1000
        );
        $this->Message_data->insertMessage($message);
        js_alert_redirect('发送'.$amount.'金锭补偿邮件？', site_url(array('user', 'view', $user_id)));
    }

    public function execSendMailMoney($user_id = 0) {
        $amount = $this->input->post('amount');
        $message = array(
            'SEND_USER_ID' => 0,
            'RECEIVE_USER_ID' => $user_id,
            'GIFT_TYPE' => 0,
            'AMOUNT' => $amount,
            'TEXT_ID' => 1001
        );
        $this->Message_data->insertMessage($message);
        js_alert_redirect('发送'.$amount.'银锭补偿邮件？', site_url(array('user', 'view', $user_id)));
    }

    public function execSendMailHeart($user_id = 0) {
        $amount = $this->input->post('amount');
        $message = array(
            'SEND_USER_ID' => 0,
            'RECEIVE_USER_ID' => $user_id,
            'GIFT_TYPE' => 2,
            'AMOUNT' => $amount,
            'TEXT_ID' => 1002
        );
        $this->Message_data->insertMessage($message);
        js_alert_redirect('发送'.$amount.'体力补偿邮件？', site_url(array('user', 'view', $user_id)));
    }
}



