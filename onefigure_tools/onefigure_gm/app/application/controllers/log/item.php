<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Item extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('facebook/Log_data'));
        $this->load->helper(array('javascript', 'url', 'json'));
    }

    function ajaxGetCoinOfLevelData() {
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
            $data = array('STAT_DATE' => $date, 'TOTAL_GOLD' => 0, 'TOTAL_RUBY' => 0, 'TOTAL_USER' => 0);
            $userCountRows = $rows[1];
            $goldRows = $rows[2];
            $rubyRows = $rows[3];
            ksort($userCountRows);

            foreach ($userCountRows as $level => $user_count) {
                if (!isset($data['GOLD_' . $level])) {
                    $data['GOLD_' . $level] = 0;
                }

                if (!isset($data['RUBY_' . $level])) {
                    $data['RUBY_' . $level] = 0;
                }

                $data['GOLD_' . $level] += number_format($goldRows[$level] / $user_count, 2, '.', '');
                $data['RUBY_' . $level] += number_format($rubyRows[$level] / $user_count, 2, '.', '');
                $data['TOTAL_GOLD'] += $goldRows[$level];
                $data['TOTAL_RUBY'] += $rubyRows[$level];
                $data['TOTAL_USER'] += $user_count;
            }

            $data['TOTAL_GOLD'] = number_format($data['TOTAL_GOLD'] / $data['TOTAL_USER'], 2, '.', '');
            $data['TOTAL_RUBY'] = number_format($data['TOTAL_RUBY'] / $data['TOTAL_USER'], 2, '.', '');

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetRuneCountData() {
        $data = array(
                'STAT_TYPE' => 3,
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

            foreach ($rows as $level => $runeRows) {
                foreach ($runeRows as $rune_info_id => $rune_count) {
                    if (!isset($data['RUNE_COUNT_' . $rune_info_id])) {
                        $data['RUNE_COUNT_' . $rune_info_id] = 0;
                    }

                    $data['RUNE_COUNT_' . $rune_info_id] += $rune_count;
                    $data['TOTAL_COUNT'] += $rune_count;
                }
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetLotboxCountData() {
        $data = array(
                'STAT_TYPE' => 9,
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
            $data = array('STAT_DATE' => $date, 'LOT_TYPE1_COUNT' => 0, 'LOT_TYPE2_COUNT' => 0, 'TOTAL_COUNT' => 0);
            ksort($rows);

            foreach ($rows as $lot_type => $product_types) {
                foreach ($product_types as $product_type => $products) {
                    foreach ($products as $product_id => $product_count) {
                        if (!isset($data['PRODUCT_LOT_TYPE' . $lot_type . '_COUNT_' . $product_id])) {
                            $data['PRODUCT_LOT_TYPE' . $lot_type . '_COUNT_' . $product_id] = 0;
                        }

                        if (!isset($data['PRODUCT_COUNT_' . $product_id])) {
                            $data['PRODUCT_COUNT_' . $product_id] = 0;
                            $data['PRODUCT_RATE_' . $product_id] = 0;
                        }

                        $data['PRODUCT_COUNT_' . $product_id] += $product_count;
                        $data['PRODUCT_LOT_TYPE' . $lot_type . '_COUNT_' . $product_id] += $product_count;
                        $data['LOT_TYPE' . $lot_type . '_COUNT'] += $product_count;
                        $data['TOTAL_COUNT'] += $product_count;
                    }
                }
            }

            foreach ($data as $index => $count) {
                if (strpos($index, 'PRODUCT_COUNT_') === 0) {
                    $rate_index = str_replace('_COUNT_', '_RATE_', $index);
                    $data[$rate_index] = number_format($count / $data['TOTAL_COUNT'] * 100, 2);
                }
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetCoinData($gain_use = 1) {
        $data = array(
                'STAT_TYPE' => 10,
                'STAT_DEPTH2' => $gain_use,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsDailyDepthRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $data['STAT_TYPE'] = -10;
        $dataArray = $this->Log_data->getStatsDailyDepthRows($data);
        $statCountRows = $dataArray['DATA'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        foreach ($statRows as $date => $rows) {
            $data = array('STAT_DATE' => $date, 'TOTAL_GOLD_AMOUNT' => 0, 'TOTAL_RUBY_AMOUNT' => 0, 'TOTAL_FP_AMOUNT' => 0, 'TOTAL_GOLD_COUNT' => 0, 'TOTAL_RUBY_COUNT' => 0, 'TOTAL_FP_COUNT' => 0);
            ksort($rows);

            foreach ($rows as $coin_type => $coin_types) {
                foreach ($coin_types as $gain_use => $gain_uses) {
                    foreach ($gain_uses as $category => $coin_count) {
                        if ($coin_type == 1) {
                            $key_amount = 'GOLD_AMOUNT_' . $category;
                            $key_count = 'GOLD_COUNT_' . $category;
                            $key_total_amount = 'TOTAL_GOLD_AMOUNT';
                            $key_total_count = 'TOTAL_GOLD_COUNT';
                        } else if ($coin_type == 2) {
                            $key_amount = 'RUBY_AMOUNT_' . $category;
                            $key_count = 'RUBY_COUNT_' . $category;
                            $key_total_amount = 'TOTAL_RUBY_AMOUNT';
                            $key_total_count = 'TOTAL_RUBY_COUNT';
                        } else if ($coin_type == 3) {
                            $key_amount = 'FP_AMOUNT_' . $category;
                            $key_count = 'FP_COUNT_' . $category;
                            $key_total_amount = 'TOTAL_FP_AMOUNT';
                            $key_total_count = 'TOTAL_FP_COUNT';
                        }

                        if (!isset($data[$key_amount])) {
                            $data[$key_amount] = 0;
                            $data[$key_count] = 0;
                        }

                        if (isset($statCountRows[$date][$coin_type][$gain_use][$category])) {
                            $data[$key_count] += $statCountRows[$date][$coin_type][$gain_use][$category];
                            $data[$key_total_count] += $statCountRows[$date][$coin_type][$gain_use][$category];
                        }

                        $data[$key_amount] += $coin_count;
                        $data[$key_total_amount] += $coin_count;
                    }
                }
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function downloadGetStatsDailyItemSaleData($start_date, $end_date) {
        header("Content-type:application/vnd.ms-excel;charset=UTF-8");
        header("Content-Disposition:attachment; filename=Stats_Daily_Item_Sale_Data_" . date("Ymd") . ".xls");
        header("Expires:0");
        header("Cache-Control:must-revalidate,post-check=0,pre-check=0");
        header("Pragma:public");

        $this->load->config('solipop');
        $item_name_array = $this->config->item('item_name');
        ksort($item_name_array);

        $data = array(
                'STAT_TYPE' => 5,
                'START_DATE' => $start_date,
                'END_DATE' => $end_date,
            );

        $dataArray = $this->Log_data->getStatsDailyDepthRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $header_array = array('"날짜"', '"합계"');
        foreach ($item_name_array as $item_code => $name) {
            if ($item_code > 5) break;
            $header_array[] = '"' . $name . '"';
        }
        echo iconv('UTF-8', 'EUC-KR', implode("\t", $header_array));
        echo "\n";

        foreach ($statRows as $stat_date => $rows) {
            $row_array = array($stat_date, 0, 0, 0, 0, 0, 0, 0);

            foreach ($rows as $item_code => $count) {
                $row_array[($item_code + 2)] = $count;
                $row_array[1] += $count;
            }

            echo implode("\t", $row_array);
            echo "\n";
        }
    }
}
