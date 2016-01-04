<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Push_data extends CI_Model {
    function __construct() {
        parent::__construct();

        $this->load->database('log');
    }

    function __destruct() {
        $this->db->close();
    }

    function selectPushMessageRows($data = array()) {
        $fields = array(
                'MESSAGE_ID',
                'TITLE',
                'MESSAGE',
                'SEND_DATE',
                'SEND_TIME',
                'FINISH_DATE',
                'OS_TYPE',
                'TOTAL_USER_COUNT',
                'SUCCESS_COUNT',
                'CREATE_DATE',
		'USER_CODE',
            );
        $wheres = array(
                'IS_DELETE' => 0
            );

        $get_object = $this->db
            ->from('push_messages')
            ->where($wheres);
        $count_object = clone $get_object;

        $result = $get_object
            ->select(implode(', ', $fields))
            ->order_by('MESSAGE_ID', 'DESC')
            ->limit($data['LIMIT'], (($data['PAGE'] - 1) * $data['LIMIT']))
            ->get();
        $total_count = $count_object->count_all_results();

        $dataRows = array();
        foreach ($result->result_array() as $row) {
            if ($row['OS_TYPE'] == 2) {
                $row['OS_NAME'] = 'Android';
            } else if ($row['OS_TYPE'] == 1) {
                $row['OS_NAME'] = 'iOS';
            } else {
                $row['OS_NAME'] = 'All';
            }

	    if( $row['USER_CODE'] == 3) {
		$row['USER_CODE_NAME'] = 'TANGENT';
	    } else if( $row['USER_CODE'] == 1) {
		$row['USER_CODE_NAME'] = 'Purple';
	    } else {
		$row['USER_CODE_NAME'] = 'USER';
	    }
             
            $dataRows[] = $row;
        }

        return array('DATA' => $dataRows, 'TOTAL_COUNT' => $total_count);
    }

    function insertPushMessageRow($data = array()) {
        $insert = array(
                'SEND_DATE' => $data['SEND_DATE'],
                'SEND_TIME' => $data['SEND_TIME'],
                'OS_TYPE' => $data['OS_TYPE'],
                'TITLE' => $data['TITLE'],
                'MESSAGE' => $data['MESSAGE'],
                'USER_CODE' => $data['USER_CODE'],
                'CREATE_DATE' => date('Y-m-d H:i:s'),
            );

        $this->db->insert('push_messages', $insert);

        return $this->db->affected_rows();
    }

    function updatePushMessageRow($data = array()) {
        $wheres = array(
                'MESSAGE_ID' => $data['MESSAGE_ID'],
            );

        $update = array(
                'SEND_DATE' => $data['SEND_DATE'],
                'SEND_TIME' => $data['SEND_TIME'],
                'OS_TYPE' => $data['OS_TYPE'],
                'USER_CODE' => $data['USER_CODE'],
                'TITLE' => $data['TITLE'],
                'MESSAGE' => $data['MESSAGE'],
            );

        $this->db->where($wheres);
        $this->db->update('push_messages', $update);

        return $this->db->affected_rows();
    }

    function deletePushMessageRow($data = array()) {
        $wheres = array(
                'MESSAGE_ID' => $data['MESSAGE_ID'],
            );

        $update = array(
                'IS_DELETE' => 1,
            );

        $this->db->where($wheres);
        $this->db->update('push_messages', $update);

        return $this->db->affected_rows();
    }
}
