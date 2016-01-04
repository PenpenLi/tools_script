<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Country extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('facebook/Log_data'));
        $this->load->helper(array('json'));
    }

    function index() {
    }

    function ajaxGetUserJoinedCountOfCountryData() {
        $data = array(
                'STAT_TYPE' => 1,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsDailyDepthCountryRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        foreach ($statRows as $date => $rows) {
            $data = array('STAT_DATE' => $date, 'TOTAL_COUNT' => 0);
            ksort($rows);

            foreach ($rows as $country => $user_count) {
                if (!isset($data['USER_COUNT_' . $country])) {
                    $data['USER_COUNT_' . $country] = 0;
                }

                $data['USER_COUNT_' . $country] += $user_count;
                $data['TOTAL_COUNT'] += $user_count;
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetActiveUserCountOfCountryData() {
        $data = array(
                'STAT_TYPE' => 2,
                'STAT_DEPTH2' => 1,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsDailyDepthCountryRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        foreach ($statRows as $date => $rows) {
            $data = array('STAT_DATE' => $date, 'TOTAL_COUNT' => 0);
            ksort($rows);

            foreach ($rows as $country => $user_count) {
                if (!isset($data['USER_COUNT_' . $country])) {
                    $data['USER_COUNT_' . $country] = 0;
                }

                $data['USER_COUNT_' . $country] += $user_count;
                $data['TOTAL_COUNT'] += $user_count;
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetRemainedUserCountOfCountryData() {
        $data = array(
                'STAT_TYPE' => 2,
                'STAT_DEPTH2' => 1,
                'START_DATE' => date('Y-m-d', strtotime($this->input->post('start_date') . ' -2 day')),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsDailyDepthCountryRows($data);
        $activeUserRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $data = array(
                'STAT_TYPE' => 2,
                'STAT_DEPTH2' => 2,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsDailyDepthCountryRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        foreach ($statRows as $date => $rows) {
            $data = array('STAT_DATE' => $date, 'TOTAL_COUNT' => 0);
            ksort($rows);
            
            $yesterday = date('Y-m-d', strtotime($date . ' -1 day'));
            $today_total_count = 0;
            $yesterday_total_count = 0;

            foreach ($rows as $country => $user_count) {
                if (isset($activeUserRows[$yesterday][$country])) {
                    $yesterday_count = $activeUserRows[$yesterday][$country];
                    $data['USER_COUNT_' . $country] = number_format($user_count / $yesterday_count * 100, 2) . '% (' . number_format($user_count) . ')';
                } else {
                    $yesterday_count = 0;
                    $data['USER_COUNT_' . $country] = '- (' . number_format($user_count) . ')';
                }
                
                $today_total_count += $user_count;
                $yesterday_total_count += $yesterday_count;
            }

            if ($yesterday_total_count > 0) {
                $data['TOTAL_COUNT'] = number_format($today_total_count / $yesterday_total_count * 100, 2) . '% (' . number_format($today_total_count) . ')';
            } else {
                $data['TOTAL_COUNT'] = '- (' . number_format($today_total_count) . ')';
            }
            
            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetUserConnectedCountOfCountryData() {
        $data = array(
                'STAT_TYPE' => 3,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsDailyDepthCountryRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        foreach ($statRows as $date => $rows) {
            $data = array('STAT_DATE' => $date, 'TOTAL_COUNT' => 0);
            ksort($rows);

            foreach ($rows as $country => $user_count) {
                if (!isset($data['USER_COUNT_' . $country])) {
                    $data['USER_COUNT_' . $country] = 0;
                }

                $data['USER_COUNT_' . $country] += $user_count;
                $data['TOTAL_COUNT'] += $user_count;
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetUserPlayTimeOfCountryData() {
        $data = array(
                'STAT_TYPE' => 4,
                'STAT_DEPTH2' => 1,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsDailyDepthCountryRows($data);
        $totalPlayTimeRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $data = array(
                'STAT_TYPE' => 4,
                'STAT_DEPTH2' => 2,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsDailyDepthCountryRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        foreach ($statRows as $date => $rows) {
            $data = array('STAT_DATE' => $date, 'TOTAL_COUNT' => 0);
            ksort($rows);
            
            $total_play_time_total = 0;
            $total_play_time_average = 0;

            foreach ($rows as $country => $user_count) {
                $total_play_time = $totalPlayTimeRows[$date][$country];
                $data['USER_COUNT_' . $country] = number_format($user_count / 60, 2) . 'm (' . number_format($total_play_time / 60) . 'm)';
                
                $total_play_time_total += $total_play_time;
                $total_play_time_average += $user_count;
            }

            $data['TOTAL_COUNT'] = number_format($total_play_time_average / 60, 2) . 'm (' . number_format($total_play_time_total / 60) . 'm)';
            
            $response['rows'][] = $data;
        }

        json_output($response, true);
    }
}