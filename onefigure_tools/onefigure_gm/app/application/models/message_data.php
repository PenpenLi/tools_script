<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Message_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->load->database($_SESSION['db_group']);
    }

    public function __destruct() {
        $this->db->close();
    }

    function selectMessage($data = array()) {
        $fields = array(
                'M.MESSAGE_ID',
                'M.SEND_USER_ID',
                'M.RECIEVE_USER_ID',
                'M.TEXT_ID',
                'M.GIFT',
                'M.CREATE_DATE',
                'NICKNAME',
                'E.EVENT_ID',
                'E.TITLE',
            );
        $wheres = array(
                'M.MESSAGE_ID' => $data['MESSAGE_ID'],
                'M.RECIEVE_USER_ID' => $data['USER_ID'],
                'M.IS_DELETE' => 0
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('messages M')
            ->join('users U', 'M.SEND_USER_ID=USER_ID', 'LEFT OUTER')
            ->join('events E', 'M.EVENT_ID=E.EVENT_ID', 'LEFT OUTER')
            ->where($wheres)
            ->get();
        $return = $result->row_array();
        
        if (!empty($return['GIFT'])) {
            $gift = json_decode($return['GIFT'], true);
            $return['GIFT_TYPE'] = $gift['TYPE'];
            $return['ITEM_ID'] = $gift['ITEM_ID'];
            $return['AMOUNT'] = $gift['AMOUNT'];
        }

        return $return;
    }

    function selectMessages($data = array()) {
        $fields = array(
                'M.MESSAGE_ID',
                'M.SEND_USER_ID',
                'M.RECIEVE_USER_ID',
                'M.TEXT_ID',
                'M.GIFT',
                'M.CREATE_DATE',
                'NICKNAME',
                'E.EVENT_ID',
                'E.TITLE',
            );
        $wheres = array(
                'M.RECIEVE_USER_ID' => $data['USER_ID'],
                'M.IS_DELETE' => 0
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('messages M')
            ->join('users U', 'M.SEND_USER_ID=USER_ID', 'LEFT OUTER')
            ->join('events E', 'M.EVENT_ID=E.EVENT_ID', 'LEFT OUTER')
            ->where($wheres)
           // ->where('ADDDATE(M.CREATE_DATE, INTERVAL IF(M.SEND_USER_ID=0, ' . MESSAGE_LIMIT_FROM_ADMIN . ', ' . MESSAGE_LIMIT_FROM_USER . ') DAY) >= NOW()')
            ->get();
        $return = array();
        
        foreach ($result->result_array() as $row) {
            if (!empty($row['GIFT'])) {
                $gift = json_decode($row['GIFT'], true);
                $row['GIFT_TYPE'] = $gift['TYPE'];
                $row['ITEM_ID'] = $gift['ITEM_ID'];
                $row['AMOUNT'] = $gift['AMOUNT'];
            }
            
            $return[] = $row;
        }

        return $return;
    }

    function selectRecentMessage($data = array()) {
        $fields = array(
                'MESSAGE_ID',
                'SEND_USER_ID',
                'RECIEVE_USER_ID',
                'TEXT_ID',
                'CREATE_DATE',
            );
        $wheres = array(
                'SEND_USER_ID' => $data['USER_ID'],
                'RECIEVE_USER_ID' => $data['FRIEND_USER_ID'],
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('messages')
            ->where($wheres)
            ->get();
        $return = $result->row_array();

        return $return;
    }

    public function selectMessageCount($data = array()) {
        $wheres = array(
            'M.RECIEVE_USER_ID' => $data['USER_ID'],
            'M.IS_DELETE' => 0
        );

        $count = $this->db
            ->from('messages M')
            ->join('users U', 'M.SEND_USER_ID=USER_ID', 'LEFT OUTER')
            ->where($wheres)
            ->where('ADDDATE(M.CREATE_DATE, INTERVAL IF(M.SEND_USER_ID=0, ' . MESSAGE_LIMIT_FROM_ADMIN . ', ' . MESSAGE_LIMIT_FROM_USER . ') DAY) >= NOW()')
            ->count_all_results();

        return $count;
    }

    function insertMessage($data = array()) {
        $gift = array();
        if (isset($data['GIFT_TYPE'])) {
            $gift = array(
                    'TYPE' => $data['GIFT_TYPE'],
                    'ITEM_ID' => empty($data['ITEM_ID']) ? null : $data['ITEM_ID'],
                    'AMOUNT' => empty($data['AMOUNT']) ? null : $data['AMOUNT'],
                );
        }
        
        $insert = array(
                'SEND_USER_ID' => $data['SEND_USER_ID'],
                'RECIEVE_USER_ID' => $data['RECEIVE_USER_ID'],
                'EVENT_ID' => empty($data['EVENT_ID']) ? null : $data['EVENT_ID'],
                'TEXT_ID' => empty($data['TEXT_ID']) ? null : $data['TEXT_ID'],
                'GIFT' => json_encode($gift),
                'CREATE_DATE' => date('Y-m-d H:i:s'),
                'CREATE_IP' => !empty($_SERVER['HTTP_X_FORWARDED_FOR']) ? $_SERVER['HTTP_X_FORWARDED_FOR'] : $_SERVER['REMOTE_ADDR'],
                'IS_DELETE' => 0,
            );

        $this->db->insert('messages', $insert);

        if ($this->db->affected_rows() > 0) {
            return $this->db->insert_id();
        } else {
            return -1;
        }
    }

    function deleteMessage($data = array()) {
        $update = array(
                'DELETE_DATE' => date('Y-m-d H:i:s'),
                'DELETE_IP' => !empty($_SERVER['HTTP_X_FORWARDED_FOR']) ? $_SERVER['HTTP_X_FORWARDED_FOR'] : $_SERVER['REMOTE_ADDR'],
                'IS_DELETE' => 1
            );

        $wheres = array(
                'RECIEVE_USER_ID' => $data['USER_ID']
            );

        if (isset($data['MESSAGE_ID'])) {
            $wheres['MESSAGE_ID'] = $data['MESSAGE_ID'];
        }

        $this->db
            ->where($wheres)
            ->update('messages', $update);
    }
}
