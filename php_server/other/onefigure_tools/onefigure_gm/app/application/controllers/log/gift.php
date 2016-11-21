<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Gift extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('facebook/Log_data'));
        $this->load->helper(array('json', 'url'));
    }

    function index() {
    }

    function ajaxGetSendGiftOfUserData($user_id) {
        $data = array(
                'USER_ID' => $user_id,
                'TABLE_MONTH' => $this->input->post('year_month'),
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Log_data->getSendGiftOfUserRows($data);
        $giftRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        foreach ($giftRows as $index => $row) {
            $response['rows'][$index]['id'] = $row['NO'];
            $response['rows'][$index]['cell'] = array(
                    number_format($row['NO']),
                    '<a href="' . site_url(array('user', 'view', $row['RECEIVE_ID'])) . '" target="_blank">' . $row['RECEIVE_ID'] . '</a>',
                    $row['SEND_DATE'],
                    $row['RECEIVE_DATE']
                );
        }

        json_output($response, true);
    }

    function ajaxGetReceiveGiftOfUserData($user_id) {
        $data = array(
                'USER_ID' => $user_id,
                'TABLE_MONTH' => $this->input->post('year_month'),
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Log_data->getReceiveGiftOfUserRows($data);
        $giftRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        foreach ($giftRows as $index => $row) {
            $response['rows'][$index]['id'] = $row['NO'];
            $response['rows'][$index]['cell'] = array(
                    $row['NO'],
                    '<a href="' . site_url(array('user', 'view', $row['SEND_ID'])) . '" target="_blank">' . $row['SEND_ID'] . '</a>',
                    $row['SEND_DATE'],
                    $row['RECEIVE_DATE']
                );
        }

        json_output($response, true);
    }
}
