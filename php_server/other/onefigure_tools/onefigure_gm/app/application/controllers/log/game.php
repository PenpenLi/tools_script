<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Game extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('facebook/Log_data'));
        $this->load->helper(array('json'));
    }

    function index() {
    }

    function ajaxGetUserCountOfLevelData() {
        $data = array(
                'STAT_TYPE' => 1,
                'STAT_DEPTH1' => 1,
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
            ksort($rows[1]);

            foreach ($rows[1] as $level => $user_count) {
                if (!isset($data['USER_COUNT_' . $level])) {
                    $data['USER_COUNT_' . $level] = 0;
                }

                $data['USER_COUNT_' . $level] += $user_count;
                $data['TOTAL_COUNT'] += $user_count;
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetUserCountOfCharacterLevelData() {
        $data = array(
                'STAT_TYPE' => 2,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $job_name = array(1 => 'Fighter', 2 => 'Mage', 3 => 'Healer', 4 => 'Archer');

        $dataArray = $this->Log_data->getStatsDailyDepthRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        $jobData = array();
        foreach ($statRows as $date => $rows) {
            foreach ($rows as $job => $jobRows) {
                $data = array('STAT_DATE' => $date, 'JOB' => $job_name[$job]);

                foreach ($jobRows as $level => $user_count) {
                    if (!isset($data['LEVEL_' . $level])) {
                        $data['LEVEL_' . $level] = 0;
                    }

                    $data['LEVEL_' . $level] += $user_count;
                }

                $jobData[$job][] = $data;
            }
        }

        $response['rows'] = array_merge($jobData[1], $jobData[2], $jobData[3], $jobData[4]);

        json_output($response, true);
    }

    function ajaxGetStatsDailyPlayCountOfLevelData() {
        $data = array(
                'STAT_TYPE' => 1,
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

            foreach ($rows as $level => $play_count) {
                if (!isset($data['PLAY_COUNT_' . $level])) {
                    $data['PLAY_COUNT_' . $level] = 0;
                }

                $data['PLAY_COUNT_' . $level] += $play_count;
                $data['TOTAL_COUNT'] += $play_count;
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetStatsDailyUserCountOfGamePlayData() {
        $data = array(
                'STAT_TYPE' => 2,
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

            foreach ($rows as $play_count => $user_count) {
                if ($play_count <= 10) {
                    $key = $play_count;
                } else if ($play_count <= 100) {
                    $key = ceil($play_count / 10) * 10;
                } else if ($play_count <= 200) {
                    $key = ceil($play_count / 100) * 100;
                } else {
                    $key = 201;
                }

                if (!isset($data['PLAY_COUNT_' . $key])) {
                    $data['PLAY_COUNT_' . $key] = 0;
                }

                $data['PLAY_COUNT_' . $key] += $user_count;
                $data['TOTAL_COUNT'] += $user_count;
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetStatsDailyScoreOfPlayCountData() {
        $data = array(
                'STAT_TYPE' => 4,
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

            foreach ($rows as $score => $play_count) {
                if (!isset($data['SCORE_' . $score])) {
                    $data['SCORE_' . $score] = 0;
                }

                $data['SCORE_' . $score] += $play_count;
                $data['TOTAL_COUNT'] += $play_count;
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetUserOutCountOfLevelData() {
        $data = array(
                'STAT_TYPE' => 7,
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
                if (!isset($data['USER_COUNT_' . $level])) {
                    $data['USER_COUNT_' . $level] = 0;
                }

                $data['USER_COUNT_' . $level] += $user_count;
                $data['TOTAL_COUNT'] += $user_count;
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetStageData() {
        $data = array(
                'STAT_TYPE' => 8,
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

            foreach ($rows as $stage_type => $stages) {
                foreach ($stages as $stage_id => $stage_count) {
                    if (!isset($data['STAGE_COUNT_' . $stage_type . '_' . $stage_id])) {
                        $data['STAGE_COUNT_' . $stage_type . '_' . $stage_id] = 0;
                    }

                    $data['STAGE_COUNT_' . $stage_type . '_' . $stage_id] += $stage_count;
                    $data['TOTAL_COUNT'] += $stage_count;
                }
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }
}
