<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Log_data extends CI_Model {
    function __construct() {
        parent::__construct();

        $this->load->database('service');
    }

    function __destruct() {
        $this->db->close();
    }

    public function insertPointLog($point_type, $point, $category, $user_id) {
        if (!empty($user_id) && !empty($point_type) && is_numeric($category) && is_numeric($point) && $point != 0) {
            if ($point_type == 1) {
                $point_field = 'MONEY';
            } else if ($point_type == 2) {
                $point_field = 'CASH';
            } else if ($point_type == 3) {
                $point_field = 'LOTTERY_POINT';
            } else if ($point_type == 4) {
                $point_field = 'LOTTERY_COUPON';
            } else if ($point_type == 5) {
                $point_field = 'LOTTERY_HIGH_COUPON';
            } else {
                return false;
            }

            $this->db
                ->set('USER_ID', $user_id)
                ->set('POINT_TYPE', $point_type)
                ->set('CATEGORY', $category)
                ->set('POINT', $point)
                ->set('CURRENT_POINT', '( SELECT ' . $point_field . ' FROM op_users WHERE USER_ID=' . $user_id . ' )', false)
                ->set('CREATE_DATE', date('Y-m-d H:i:s'))
                ->insert('log_point_' . LOG_TABLE_DATE);
        }
    }

    function getGamePlayData() {
        $sql = 'SELECT
                    DATE_FORMAT(start_date, "%Y-%m-%d %H:00:00") AS play_date, COUNT(idx) AS cnt
                FROM ' . LOG_TABLE_GAME . YESTERDAY_MONTH . '
                GROUP BY play_date';
        $result = $this->db->query($sql);
    }

    function getPaymentOfUserRows($data = array()) {
        $fields = array(
                'USER_ID',
                'PRODUCT_ID',
                'ORDER_ID',
                'KAKAO_CODE',
                'CREATE_DATE',
            );
        $wheres = array(
                'USER_ID' => $data['USER_ID']
            );

        $get_object = $this->db
            ->from('market_order')
            ->where('DATE_FORMAT(CREATE_DATE, "%Y%m") = ' . $data['TABLE_MONTH'])
            ->where($wheres);
        $count_object = clone $get_object;

        $result = $get_object
            ->select(implode(', ', $fields))
            ->order_by('CREATE_DATE', 'DESC')
            ->get();
        $total_count = $count_object->count_all_results();

        $dataRows = array();
        foreach ($result->result_array() as $row) {
            $row['CREATE_DATE_US'] = date('Y-m-d H:i:s', strtotime($row['CREATE_DATE'] . ' -17 hours'));
            $dataRows[] = $row;
        }

        return array('DATA' => $dataRows, 'TOTAL_COUNT' => $total_count);
    }

    function getStatsHourlyRows($data = array()) {
        $where_string = ' WHERE 1=1 ';

        if (!empty($data['STAT_TYPE'])) {
            $where_string .= ' AND STAT_TYPE=' . $data['STAT_TYPE'];
        }

        if (!empty($data['START_DATE']) && !empty($data['END_DATE'])) {
            $where_string .= ' AND STAT_DATE BETWEEN "' . $data['START_DATE'] . '" AND "' . $data['END_DATE'] . '" ';
        }

        $sql = 'SELECT
                  STAT_TYPE,
                  STAT_DATE,
                  STAT_HOUR,
                  STAT_VALUE
                FROM `' . LOG_TABLE_HOURLY_DATA . '`
                ' . $where_string . '
                ORDER BY STAT_DATE, STAT_HOUR';
        $result = $this->db->query($sql);

        $dataRows = array();
        foreach ($result->result_array() as $row) {
            $dataRows[$row['STAT_DATE']][$row['STAT_HOUR']] = $row['STAT_VALUE'];
        }

        $total_count = count($dataRows);

        return array('DATA' => $dataRows, 'TOTAL_COUNT' => $total_count);
    }

    function getStatsDailyDepthRows($data = array()) {
        $where_string = ' WHERE 1=1 ';

        if (!empty($data['STAT_TYPE'])) {
            $where_string .= ' AND STAT_TYPE=' . $data['STAT_TYPE'];
        }

        if (!empty($data['STAT_DEPTH1'])) {
            $where_string .= ' AND STAT_DEPTH1=' . $data['STAT_DEPTH1'];
        }

        if (!empty($data['STAT_DEPTH2'])) {
            $where_string .= ' AND STAT_DEPTH2=' . $data['STAT_DEPTH2'];
        }

        if (!empty($data['STAT_DEPTH3'])) {
            $where_string .= ' AND STAT_DEPTH3=' . $data['STAT_DEPTH3'];
        }

        if (!empty($data['START_DATE']) && !empty($data['END_DATE'])) {
            $where_string .= ' AND STAT_DATE BETWEEN "' . $data['START_DATE'] . '" AND "' . $data['END_DATE'] . '" ';
        }

        $sql = 'SELECT
                  STAT_TYPE,
                  STAT_DATE,
                  STAT_DEPTH1,
                  STAT_DEPTH2,
                  STAT_DEPTH3,
                  STAT_VALUE
                FROM `' . LOG_TABLE_DAILY_DEPTH_DATA . '`
                ' . $where_string . '
                ORDER BY STAT_DATE, STAT_DEPTH1, STAT_DEPTH2, STAT_DEPTH3';
        $result = $this->db->query($sql);

        $dataRows = array();
        foreach ($result->result_array() as $row) {
            if (empty($row['STAT_DEPTH2']) && empty($row['STAT_DEPTH3'])) {
                $dataRows[$row['STAT_DATE']][$row['STAT_DEPTH1']] = $row['STAT_VALUE'];
            } else if (empty($data['STAT_DEPTH1']) && !empty($data['STAT_DEPTH2'])) {
                $dataRows[$row['STAT_DATE']][$row['STAT_DEPTH1']] = $row['STAT_VALUE'];
            } else if (empty($row['STAT_DEPTH3'])) {
                $dataRows[$row['STAT_DATE']][$row['STAT_DEPTH1']][$row['STAT_DEPTH2']] = $row['STAT_VALUE'];
            } else {
                $dataRows[$row['STAT_DATE']][$row['STAT_DEPTH1']][$row['STAT_DEPTH2']][$row['STAT_DEPTH3']] = $row['STAT_VALUE'];
            }
        }

        $total_count = count($dataRows);

        return array('DATA' => $dataRows, 'TOTAL_COUNT' => $total_count);
    }

    function getStatsDailyDepthCountryRows($data = array()) {
        $where_string = ' WHERE 1=1 ';

        if (!empty($data['STAT_TYPE'])) {
            $where_string .= ' AND STAT_TYPE=' . $data['STAT_TYPE'];
        }

        if (!empty($data['STAT_DEPTH1'])) {
            $where_string .= ' AND STAT_DEPTH1=' . $data['STAT_DEPTH1'];
        }

        if (!empty($data['STAT_DEPTH2'])) {
            $where_string .= ' AND STAT_DEPTH2=' . $data['STAT_DEPTH2'];
        }

        if (!empty($data['STAT_DEPTH3'])) {
            $where_string .= ' AND STAT_DEPTH3=' . $data['STAT_DEPTH3'];
        }

        if (!empty($data['START_DATE']) && !empty($data['END_DATE'])) {
            $where_string .= ' AND STAT_DATE BETWEEN "' . $data['START_DATE'] . '" AND "' . $data['END_DATE'] . '" ';
        }

        $sql = 'SELECT
                  STAT_TYPE,
                  STAT_DATE,
                  STAT_DEPTH1,
                  STAT_DEPTH2,
                  STAT_DEPTH3,
                  STAT_VALUE
                FROM `' . LOG_TABLE_DAILY_DEPTH_COUNTRY_DATA . '`
                ' . $where_string . '
                ORDER BY STAT_DATE, STAT_DEPTH1, STAT_DEPTH2, STAT_DEPTH3';
        $result = $this->db->query($sql);

        $dataRows = array();
        foreach ($result->result_array() as $row) {
            if (empty($row['STAT_DEPTH2']) && empty($row['STAT_DEPTH3'])) {
                $dataRows[$row['STAT_DATE']][$row['STAT_DEPTH1']] = $row['STAT_VALUE'];
            } else if (empty($data['STAT_DEPTH1']) && !empty($data['STAT_DEPTH2'])) {
                $dataRows[$row['STAT_DATE']][$row['STAT_DEPTH1']] = $row['STAT_VALUE'];
            } else if (empty($row['STAT_DEPTH3'])) {
                $dataRows[$row['STAT_DATE']][$row['STAT_DEPTH1']][$row['STAT_DEPTH2']] = $row['STAT_VALUE'];
            } else {
                $dataRows[$row['STAT_DATE']][$row['STAT_DEPTH1']][$row['STAT_DEPTH2']][$row['STAT_DEPTH3']] = $row['STAT_VALUE'];
            }
        }

        $total_count = count($dataRows);

        return array('DATA' => $dataRows, 'TOTAL_COUNT' => $total_count);
    }

    function getStatsCountryNameRows($data = array()) {
        $sql = 'SELECT DISTINCT STAT_DEPTH1 AS COUNTRY FROM `' . LOG_TABLE_DAILY_DEPTH_COUNTRY_DATA . '` ORDER BY 1';
        $result = $this->db->query($sql);

        $dataRows = array();
        foreach ($result->result_array() as $row) {
            $dataRows[] = $row['COUNTRY'];
        }

        return $dataRows;
    }
}
