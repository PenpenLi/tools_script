<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Banner extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('facebook/Log_data'));
        $this->load->helper(array('json'));
    }

    function index() {
    }

    function ajaxGetBannerOfUserData($user_id) {
        $data = array(
                'USER_ID' => $user_id,
                'TABLE_MONTH' => $this->input->post('year_month'),
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Log_data->getBannerOfUserRows($data);
        $paymentRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        foreach ($paymentRows as $index => $row) {
            $response['rows'][$index]['id'] = $row['GAME_CODE'];
            $response['rows'][$index]['cell'] = array(
                    '모두의 게임',
                    ($row['STATE'] == 0) ? '클릭만' : '보상지급',
                    $row['CLICK_DATE'],
                    $row['PAY_DATE']
                );
        }

        json_output($response, true);
    }
}
