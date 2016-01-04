<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Clover extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('Log_data'));
        $this->load->helper(array('json'));
    }

    function index() {
    }

    function ajaxGetCloverOfUserData($user_id, $is_used = 0) {
        $this->load->config('solipop');
        $item_name_array = $this->config->item('item_name');
        $clover_log_type_array = $this->config->item('clover_log_type');

        $data = array(
                'USER_ID' => $user_id,
                'TABLE_MONTH' => $this->input->post('year_month'),
                'IS_USED' => $is_used,
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Log_data->getCloverOfUserRows($data);
        $paymentRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        foreach ($paymentRows as $index => $row) {
            $response['rows'][$index]['id'] = $row['ITEM_CODE'] . '.' . $row['CREATE_DATE'];
            $response['rows'][$index]['cell'] = array(
                    ($is_used == 1) ? $item_name_array[$row['ITEM_CODE']] : $clover_log_type_array[$row['TYPE']],
                    $row['CLOVER'],
                    $row['CURRENT_CLOVER'],
                    $row['CREATE_DATE']
                );
        }

        json_output($response, true);
    }
}
