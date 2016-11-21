<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Event_data extends CI_Model {
    function __construct() {
        parent::__construct();

        $this->load->database('service');
    }

    function __destruct() {
        $this->db->close();
    }

    function selectEvents($data = array()) {
        $fields = array(
                'EVENT_ID',
                'PROCESS_CODE',
                'TITLE',
                'EVENT_DETAIL',
                'START_DATE',
                'END_DATE',
                'USER_COUNT',
                'CREATE_DATE',
                'UPDATE_DATE',
            );
        $wheres = array(
                'IS_DELETE' => 0
            );

        $get_object = $this->db
            ->from('events')
            ->where($wheres);
        $count_object = clone $get_object;

        $result = $get_object
            ->select(implode(', ', $fields))
            ->order_by('EVENT_ID', 'DESC')
            ->limit($data['LIMIT'], (($data['PAGE'] - 1) * $data['LIMIT']))
            ->get();
        $total_count = $count_object->count_all_results();

        $dataRows = array();
        foreach ($result->result_array() as $row) {
            $event_detail = json_decode($row['EVENT_DETAIL'], true);
            $row['REPEAT_CODE'] = $event_detail['REPEAT_CODE'];
            $row['REPEAT_COUNT'] = $event_detail['REPEAT_COUNT'];
            $row['LIMIT_COUNT'] = $event_detail['LIMIT_COUNT'];
            $row['GIFT_TYPE'] = $event_detail['GIFT_TYPE'];
            $row['ITEM_ID'] = $event_detail['ITEM_ID'];
            $row['AMOUNT'] = $event_detail['AMOUNT'];
            $row['SPOT_START_TIME'] = $event_detail['SPOT_START_TIME'];
            $row['SPOT_END_TIME'] = $event_detail['SPOT_END_TIME'];

            if ($row['PROCESS_CODE'] == 1) {
                $row['PROCESS_NAME'] = 'Login';
            } else if ($row['PROCESS_CODE'] == 3) {
                $row['PROCESS_NAME'] = 'Payment';
            } else if ($row['PROCESS_CODE'] == 8) {
                $row['PROCESS_NAME'] = 'Game Start';
            } else if ($row['PROCESS_CODE'] == 9) {
                $row['PROCESS_NAME'] = 'Stage Clear';
            } else if ($row['PROCESS_CODE'] == 17) {
                $row['PROCESS_NAME'] = 'Add Friend';
            }

            if ($row['GIFT_TYPE'] == 1) {
                $row['REWARD'] = 'Gold (' . number_format($row['AMOUNT']) . ')';
            } else if ($row['GIFT_TYPE'] == 2) {
                $row['REWARD'] = 'Ruby (' . number_format($row['AMOUNT']) . ')';
            } else if ($row['GIFT_TYPE'] == 3) {
                $row['REWARD'] = 'F.Point (' . number_format($row['AMOUNT']) . ')';
            }

            $dataRows[] = $row;
        }

        return array('DATA' => $dataRows, 'TOTAL_COUNT' => $total_count);
    }

    function selectEvent($data = array()) {
        $fields = array(
            'EVENT_ID',
            'PROCESS_CODE',
            'TITLE',
            'EVENT_DETAIL',
            'START_DATE',
            'END_DATE',
            'USER_COUNT',
            'CREATE_DATE',
            'UPDATE_DATE',
        );
        $wheres = array(
            'EVENT_ID' => $data['EVENT_ID'],
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('events')
            ->where($wheres)
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            $event_detail = json_decode($row['EVENT_DETAIL'], true);
            $row['REPEAT_CODE'] = $event_detail['REPEAT_CODE'];
            $row['REPEAT_COUNT'] = $event_detail['REPEAT_COUNT'];
            $row['LIMIT_COUNT'] = $event_detail['LIMIT_COUNT'];
            $row['GIFT_TYPE'] = $event_detail['GIFT_TYPE'];
            $row['ITEM_ID'] = $event_detail['ITEM_ID'];
            $row['AMOUNT'] = $event_detail['AMOUNT'];
            $row['SPOT_START_TIME'] = $event_detail['SPOT_START_TIME'];
            $row['SPOT_END_TIME'] = $event_detail['SPOT_END_TIME'];

            if ($row['PROCESS_CODE'] == 1) {
                $row['PROCESS_NAME'] = 'Login';
            } else if ($row['PROCESS_CODE'] == 2) {
                $row['PROCESS_NAME'] = 'Game start';
            } else if ($row['PROCESS_CODE'] == 3) {
                $row['PROCESS_NAME'] = 'Game finish';
            } else if ($row['PROCESS_CODE'] == 4) {
                $row['PROCESS_NAME'] = 'Payment';
            } else if ($row['PROCESS_CODE'] == 9) {
                $row['PROCESS_NAME'] = 'Stage Clear';
            } else if ($row['PROCESS_CODE'] == 17) {
                $row['PROCESS_NAME'] = 'Add Friend';
            }

            if ($row['GIFT_TYPE'] == 1) {
                $row['REWARD'] = 'Gold (' . number_format($row['AMOUNT']) . ')';
            } else if ($row['GIFT_TYPE'] == 2) {
                $row['REWARD'] = 'Ruby (' . number_format($row['AMOUNT']) . ')';
            } else if ($row['GIFT_TYPE'] == 3) {
                $row['REWARD'] = 'F.Point (' . number_format($row['AMOUNT']) . ')';
            }

            $return = $row;
        }

        return $return;
    }

    function insertEvent($data = array()) {
        $event_detail = array(
                'REPEAT_CODE' => $data['REPEAT_CODE'],
                'REPEAT_COUNT' => $data['REPEAT_COUNT'],
                'LIMIT_COUNT' => $data['LIMIT_COUNT'],
                'GIFT_TYPE' => $data['GIFT_TYPE'],
                'ITEM_ID' => is_numeric($data['ITEM_ID']) ? intval($data['ITEM_ID']) : null,
                'AMOUNT' => $data['AMOUNT'],
                'SPOT_START_TIME' => is_numeric($data['SPOT_START_TIME']) ? intval($data['SPOT_START_TIME']) : null,
                'SPOT_END_TIME' => is_numeric($data['SPOT_END_TIME']) ? intval($data['SPOT_END_TIME']) : null,
            );

        $insert = array(
                'PROCESS_CODE' => $data['PROCESS_CODE'],
                'TITLE' => $data['TITLE'],
                'EVENT_DETAIL' => json_encode($event_detail),
                'START_DATE' => $data['START_DATE'] . ' 00:00:00',
                'END_DATE' => $data['END_DATE'] . ' 23:59:59',
                'CREATE_DATE' => date('Y-m-d H:i:s'),
            );

        $this->db->insert('events', $insert);

        return $this->db->affected_rows();
    }

    function updateEvent($data = array()) {
        $wheres = array(
                'EVENT_ID' => $data['EVENT_ID'],
            );

        $event_detail = array(
                'REPEAT_CODE' => $data['REPEAT_CODE'],
                'REPEAT_COUNT' => $data['REPEAT_COUNT'],
                'LIMIT_COUNT' => $data['LIMIT_COUNT'],
                'GIFT_TYPE' => $data['GIFT_TYPE'],
                'ITEM_ID' => is_numeric($data['ITEM_ID']) ? intval($data['ITEM_ID']) : null,
                'AMOUNT' => $data['AMOUNT'],
                'SPOT_START_TIME' => is_numeric($data['SPOT_START_TIME']) ? intval($data['SPOT_START_TIME']) : null,
                'SPOT_END_TIME' => is_numeric($data['SPOT_END_TIME']) ? intval($data['SPOT_END_TIME']) : null,
            );

        $update = array(
                'PROCESS_CODE' => $data['PROCESS_CODE'],
                'TITLE' => $data['TITLE'],
                'EVENT_DETAIL' => json_encode($event_detail),
                'START_DATE' => $data['START_DATE'] . ' 00:00:00',
                'END_DATE' => $data['END_DATE'] . ' 23:59:59',
                'UPDATE_DATE' => date('Y-m-d H:i:s'),
            );

        $this->db->where($wheres);
        $this->db->update('events', $update);

        return $this->db->affected_rows();
    }

    function deleteEvent($data = array()) {
        $wheres = array(
                'EVENT_ID' => $data['EVENT_ID'],
            );

        $update = array(
                'DELETE_DATE' => date('Y-m-d H:i:s'),
                'IS_DELETE' => 1,
            );

        $this->db->where($wheres);
        $this->db->update('events', $update);

        return $this->db->affected_rows();
    }
}