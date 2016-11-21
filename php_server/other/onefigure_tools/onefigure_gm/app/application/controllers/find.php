<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Find extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('facebook/Find_data'));
        $this->load->helper(array('javascript', 'url', 'form', 'json'));

        if (!IS_BHSOFT) {
            js_alert_back('접근이 불가능 합니다.');
        }
    }

    function index() {
        $this->viewer->all('find/index');
    }

    function execAddClover($user_id = 0) {
        $value = $this->input->post('value');
        $add_type = $this->input->post('add_type');
        $memo = $this->input->post('memo');

        if ($add_type == 'plus') {
            $clover = '+' . $value;
        } else if ($add_type == 'minus') {
            $clover = '-' . $value;
        }

        $data = array(
                'USER_ID' => $user_id,
                'CLOVER' => $clover
            );

        if ($this->User_data->setUserRow($data)) {
            $this->User_data->addCloverLogRow($data);
            js_alert_redirect('처리가 완료 되었습니다.', site_url(array('user', 'view', $user_id)));
        } else {
            js_alert_back('처리하지 못하였습니다.');
        }
    }

    function ajaxGetFindUserData() {
        $data = array(
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Find_data->getUserRows($data);
        $paymentRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        foreach ($paymentRows as $index => $row) {
            $response['rows'][$index]['id'] = $row['ITEM_CODE'] . '.' . $row['CREATE_DATE'];
            $response['rows'][$index]['cell'] = array(
                    ($is_used == 1) ? $item_name_array[$row['ITEM_CODE']] : $diamond_log_type_array[$row['TYPE']],
                    $row['DIAMOND'],
                    $row['CURRENT_DIAMOND'],
                    $row['CREATE_DATE']
                );
        }

        json_output($response, true);
    }

    function execAddDiamond($user_id = 0) {
        $value = $this->input->post('value');
        $add_type = $this->input->post('add_type');
        $memo = $this->input->post('memo');

        if ($add_type == 'plus') {
            $diamond = '+' . $value;
        } else if ($add_type == 'minus') {
            $diamond = '-' . $value;
        }

        $data = array(
                'USER_ID' => $user_id,
                'DIAMOND' => $diamond
            );

        if ($this->User_data->setUserRow($data)) {
            $this->User_data->addDiamondLogRow($data);
            js_alert_redirect('처리가 완료 되었습니다.', site_url(array('user', 'view', $user_id)));
        } else {
            js_alert_back('처리하지 못하였습니다.');
        }
    }

    function execAddHeart($user_id = 0) {
        $value = $this->input->post('value');
        $add_type = $this->input->post('add_type');
        $memo = $this->input->post('memo');

        if ($add_type == 'plus') {
            $heart = '+' . $value;
        } else if ($add_type == 'minus') {
            $heart = '-' . $value;
        }

        $data = array(
                'USER_ID' => $user_id,
                'HEART' => $heart
            );

        if ($this->User_data->setUserRow($data)) {
            js_alert_redirect('처리가 완료 되었습니다.', site_url(array('user', 'view', $user_id)));
        } else {
            js_alert_back('처리하지 못하였습니다.');
        }
    }
}