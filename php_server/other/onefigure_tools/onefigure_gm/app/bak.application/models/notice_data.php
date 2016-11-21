<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Notice_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->load->database('service');
    }

    function selectNotices($data = array()) {
        $fields = array(
                'NOTICE_ID',
                'TITLE',
                'MANAGER_ID',
                'START_DATE',
                'END_DATE',
                'IS_SHOW',
                'CREATE_DATE',
            );
        $wheres = array(
            );

        $get_object = $this->db
            ->from('notices')
            ->where($wheres);
        $count_object = clone $get_object;

        $result = $get_object
            ->select(implode(', ', $fields))
            ->order_by('NOTICE_ID', 'DESC')
            ->limit($data['LIMIT'], (($data['PAGE'] - 1) * $data['LIMIT']))
            ->get();
        $total_count = $count_object->count_all_results();

        $dataRows = array();
        foreach ($result->result_array() as $row) {
            $dataRows[] = $row;
        }

        return array('DATA' => $dataRows, 'TOTAL_COUNT' => $total_count);
    }

    public function selectNotice($data = array()) {
        $fields = array(
                'NOTICE_ID',
                'TITLE',
                'CONTENT',
                'MANAGER_ID',
                'START_DATE',
                'END_DATE',
                'IS_SHOW',
                'CREATE_DATE',
            );

        $wheres = array(
                'NOTICE_ID' => $data['NOTICE_ID']
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('notices')
            ->where($wheres)
            ->get();
        $return = $result->row_array();
        $return['CONTENT'] = json_decode($return['CONTENT'], true);

        return $return;
    }

    public function insertNotice($data = array()) {
        $insert = array(
                'TITLE' => trim($data['TITLE']),
                'CONTENT' => json_encode($data['CONTENT']),
                'MANAGER_ID' => trim($data['MANAGER_ID']),
                'START_DATE' => trim($data['START_DATE']),
                'END_DATE' => trim($data['END_DATE']),
                'IS_SHOW' => trim($data['IS_SHOW']),
                'CREATE_DATE' => date('Y-m-d H:i:s'),
            );

        $this->db->insert('notices', $insert);

        return $this->db->affected_rows();
    }

    public function updateNotice($data = array()) {
        $update = array(
                'TITLE' => trim($data['TITLE']),
                'CONTENT' => json_encode($data['CONTENT']),
                'START_DATE' => trim($data['START_DATE']),
                'END_DATE' => trim($data['END_DATE']),
                'IS_SHOW' => trim($data['IS_SHOW']),
                'UPDATE_DATE' => date('Y-m-d H:i:s'),
            );

        $wheres = array(
                'NOTICE_ID' => $data['NOTICE_ID']
            );

        $this->db
            ->where($wheres)
            ->update('notices', $update);

        return true;
    }
}