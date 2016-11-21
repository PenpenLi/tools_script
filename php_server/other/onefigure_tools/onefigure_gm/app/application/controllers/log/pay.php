<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Pay extends CI_Controller {
    function __construct() {
        parent::__construct();

        $this->load->model(array('facebook/Log_data'));
        $this->load->helper(array('json'));
    }

    function index() {
    }

    function ajaxGetPaymentOfUserData($user_id) {
        $this->load->config('item');
        $item_info_array = $this->config->item('item_info');

        $data = array(
                'USER_ID' => $user_id,
                'TABLE_MONTH' => $this->input->post('year_month'),
                'PAGE' => !$this->input->post('page') ? 1 : $this->input->post('page'),
                'LIMIT' => !$this->input->post('rows') ? 20 : $this->input->post('rows'),
            );

        $dataArray = $this->Log_data->getPaymentOfUserRows($data);
        $paymentRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = $data['PAGE'];
        $response['total'] = ceil($total_count / $data['LIMIT']);
        $response['records'] = $total_count;
        foreach ($paymentRows as $index => $row) {
            $response['rows'][$index]['id'] = $row['ORDER_ID'];
            $response['rows'][$index]['cell'] = array(
                    $item_info_array[$row['PRODUCT_ID']]['NAME'],
                    $row['CREATE_DATE'],
                    date('Y-m-d H:i:s', strtotime($row['CREATE_DATE'] . ' -17 hours'))
                );
        }

        json_output($response, true);
    }

    function ajaxGetStatsDailyItemSaleData() {
        $this->load->config('solipop');
        $item_name_array = $this->config->item('item_name');
        $item_price_array = $this->config->item('item_price');
        ksort($item_name_array);

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
            $data = array('STAT_DATE' => $date, 'TOTAL_PRICE' => 0, 'TOTAL_COUNT' => 0);

            foreach ($item_name_array as $item_code => $item_name) {
                $data['PRICE_' . $item_code] = isset($rows[$item_code]) ? $rows[$item_code] * $item_price_array[$item_code] : 0;
                $data['COUNT_' . $item_code] = isset($rows[$item_code]) ? $rows[$item_code] : 0;

                $data['TOTAL_PRICE'] += $data['PRICE_' . $item_code];
                $data['TOTAL_COUNT'] += $data['COUNT_' . $item_code];
            }

            $response['rows'][] = $data;
        }

        json_output($response, true);
    }

    function ajaxGetStatsDailyDiamondSaleHourlyData() {
        $data = array(
                'STAT_TYPE' => 5,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsHourlyRows($data);
        $priceRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $data = array(
                'STAT_TYPE' => 4,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsHourlyRows($data);
        $countRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        $index = 0;
        foreach ($priceRows as $stat_date => $rows) {
            $response['rows'][$index]['id'] = $stat_date;
            $response['rows'][$index]['cell'][] = $stat_date;
            $response['rows'][$index]['cell'][] = 0;
            $response['rows'][$index]['cell'][] = 0;

            $total_price = 0;
            $total_count = 0;
            foreach (range(0, 23) as $hour) {
                $price = isset($priceRows[$stat_date][$hour]) ? $priceRows[$stat_date][$hour] : 0;
                $count = isset($countRows[$stat_date][$hour]) ? $countRows[$stat_date][$hour] : 0;

                $response['rows'][$index]['cell'][] = $price;
                $response['rows'][$index]['cell'][] = $count;
                $total_price += $price;
                $total_count += $count;
            }

            $response['rows'][$index]['cell'][1] = $total_price;
            $response['rows'][$index]['cell'][2] = $total_count;
            $index++;
        }

        json_output($response, true);
    }

    function downloadGetStatsHourlyData($stat_type = 1, $start_date, $end_date) {
        header("Content-type:application/vnd.ms-excel;charset=UTF-8");
        header("Content-Disposition:attachment; filename=Stats_Hourly_Data_" . date("Ymd") . ".xls");
        header("Expires:0");
        header("Cache-Control:must-revalidate,post-check=0,pre-check=0");
        header("Pragma:public");

        $data = array(
                'STAT_TYPE' => $stat_type,
                'START_DATE' => $start_date,
                'END_DATE' => $end_date,
            );

        $dataArray = $this->Log_data->getStatsHourlyRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        echo "Date";
        foreach (range(0, 23) as $hour) {
            echo "\t" . $hour . "H";
        }
        echo "\n";

        $index = 0;
        foreach ($statRows as $stat_date => $rows) {
            echo $stat_date;
            foreach (range(0, 23) as $hour) {
                $value = isset($rows[$hour]) ? $rows[$hour] : 0;
                echo "\t" . $value;
            }
            echo "\n";
        }
    }
}
