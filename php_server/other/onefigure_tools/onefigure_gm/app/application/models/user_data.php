<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class User_data extends CI_Model {
    function __construct() {
        parent::__construct();

        $this->SERVIVE_DB = $this->load->database($_SESSION['db_group']);
    }

    function __destruct() {
        $this->SERVIVE_DB->close();
    }

    function selectUser($data = array()) {
        $fields = array(
                'USER_ID',
                'EMAIL',
                'NICKNAME',
                'LEVEL',
                'EXP_POINT',
                'MONEY',
                'CASH',
                'HEART',
                'HEART_TIME',
                'LOTTERY_POINT',
                'LOTTERY_HIGH_COUPON',
                'LOTTERY_COUPON',
                'CHARACTER_ID',
                'SKILL_SLOT',
                'TREASURE_SLOT',
                'TREASURE_INVENTORY',
                'BEST_SCORE',
                'AGREE_MESSAGE',
                'STATUS',
                'DEVICE',
                'OS_TYPE',
                'OS_VERSION',
                'MARKET_TYPE',
                'TUTORIAL',
                'ATTENDANCE_COUNT',
                'ATTENDANCE_DATE',
                'CELLPHONE',
                'CREATE_DATE',
                'DELETE_DATE',
                'LOGIN_DATE',
                'CREATE_IP',
                'DELETE_IP',
                'LOGIN_IP',
            );

        $wheres = array();

        if (isset($data['USER_ID'])) {
            $this->SERVIVE_DB->where('USER_ID', $data['USER_ID']);
        } else if (isset($data['NICKNAME'])) {
            $this->SERVIVE_DB->like('NICKNAME', $data['NICKNAME'], 'after');
        } else {
            $this->SERVIVE_DB->limit(10);
        }

        $result = $this->SERVIVE_DB
            ->select(implode(', ', $fields))
            ->from('users')
            ->where($wheres)
            ->order_by('USER_ID', 'DESC')
            ->get();
        $return = $result->result_array();

        return $return;
    }
}
