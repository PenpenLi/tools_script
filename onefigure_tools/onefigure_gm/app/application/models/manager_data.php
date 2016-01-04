<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Manager_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->LOG_DB = $this->load->database($_SESSION['db_group']);
    }

    public function selectManager($data = array()) {
        $fields = array(
                'MANAGER_ID',
                'EMAIL',
                'PASSWORD',
                'NAME',
                'LEVEL',
                'CREATE_DATE',
                'LOGIN_DATE',
            );

        $wheres = array(
                'EMAIL' => $data['EMAIL']
            );

        $result = $this->LOG_DB
            ->select(implode(', ', $fields))
            ->from('managers')
            ->where($wheres)
            ->get();
        $return = $result->row_array();

        return $return;
    }

    function selectManagers($data = array()) {
        $fields = array(
                'MANAGER_ID',
                'EMAIL',
                'PASSWORD',
                'NAME',
                'LEVEL',
                'CREATE_DATE',
                'LOGIN_DATE',
            );
        $wheres = array(
            );

        $get_object = $this->LOG_DB
            ->from('managers')
            ->where($wheres);
        $count_object = clone $get_object;

        $result = $get_object
            ->select(implode(', ', $fields))
            ->order_by('EMAIL', 'ASC')
            ->limit($data['LIMIT'], (($data['PAGE'] - 1) * $data['LIMIT']))
            ->get();
        $total_count = $count_object->count_all_results();

        $dataRows = array();
        foreach ($result->result_array() as $row) {
            $row['PASSWORD'] = null;
            $dataRows[] = $row;
        }

        return array('DATA' => $dataRows, 'TOTAL_COUNT' => $total_count);
    }

    public function insertManager($data = array()) {
        $insert = array(
                'EMAIL' => trim($data['EMAIL']),
                'PASSWORD' => md5(trim($data['PASSWORD'])),
                'NAME' => trim($data['NAME']),
                'LEVEL' => trim($data['LEVEL']),
                'CREATE_DATE' => date('Y-m-d H:i:s'),
                'CREATE_IP' => $this->input->ip_address(),
            );

        $this->LOG_DB->insert('managers', $insert);

        return $this->LOG_DB->insert_id();
    }

    public function updateManager($data = array()) {
        $update = array();

        if (!empty($data['EMAIL'])) {
            $update['EMAIL'] = trim($data['EMAIL']);
        }

        if (!empty($data['NAME'])) {
            $update['NAME'] = trim($data['NAME']);
        }

        if (!empty($data['LEVEL'])) {
            $update['LEVEL'] = trim($data['LEVEL']);
        }

        if (!empty($data['PASSWORD'])) {
            $update['PASSWORD'] = md5(trim($data['PASSWORD']));
        }

        if (isset($data['IS_LOGIN'])) {
            $update['LOGIN_DATE'] = date('Y-m-d H:i:s');
            $update['LOGIN_IP'] = $this->input->ip_address();
        } else {
            $update['UPDATE_DATE'] = date('Y-m-d H:i:s');
            $update['UPDATE_IP'] = $this->input->ip_address();
        }

        $wheres = array(
                'MANAGER_ID' => $data['MANAGER_ID']
            );

        $this->LOG_DB->where($wheres);
        $this->LOG_DB->update('managers', $update);

        return true;
    }

    public function insertManagerLog($action, $user_id = 0, $query, $memo = null) {
        $this->load->library('Auth');

        $this->LOG_DB
            ->set('ACTION_TYPE', $action)
            ->set('MANAGER_ID', $this->auth->getManagerId())
            ->set('USER_ID', $user_id)
            ->set('QUERY', $query)
            ->set('MEMO', $memo)
            ->set('CREATE_DATE', date('Y-m-d H:i:s'))
            ->set('CREATE_IP', $this->input->server('REMOTE_ADDR'))
            ->insert('manager_logs');
    }
}