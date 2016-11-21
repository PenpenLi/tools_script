<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Social extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('facebook/Log_data'));
        $this->load->helper(array('json'));
    }

    function index() {
    }

    function ajaxGetFriendCountOfLevelData() {
        $data = array(
                'STAT_TYPE' => 5,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsDailyDepthRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        foreach ($statRows as $date => $rows) {
            $data = array('STAT_DATE' => $date, 'TOTAL_COUNT' => 0);
            ksort($rows);

            foreach ($rows as $level => $user_count) {
                if (!isset($data['FRIEND_COUNT_' . $level])) {
                    $data['FRIEND_COUNT_' . $level] = 0;
                }

                $data['FRIEND_COUNT_' . $level] += $user_count;
                $data['TOTAL_COUNT'] += $user_count;
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetFriendCountOfClassTypeData() {
        $data = array(
                'STAT_TYPE' => 6,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsDailyDepthRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        foreach ($statRows as $date => $rows) {
            $data = array('STAT_DATE' => $date, 'TOTAL_COUNT' => 0);
            ksort($rows);

            foreach ($rows as $class_type => $user_count) {
                if (!isset($data['FRIEND_COUNT_' . $class_type])) {
                    $data['FRIEND_COUNT_' . $class_type] = 0;
                }

                $data['FRIEND_COUNT_' . $class_type] += $user_count;
                $data['TOTAL_COUNT'] += $user_count;
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }
}
