<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class User extends CI_Controller {
    public function __construct() {
        parent::__construct();

        $this->load->model(array('User_data', 'Character_data', 'Skill_data', 'Treasure_data', 'Game_data', 'Item_data', 'Shop_data'));

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
        }

        $this->viewer->all('user/view', $data);
    }

    public function find($keyword) {
        $data = array(
                'NICKNAME' => rawurldecode($keyword),
            );

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
}