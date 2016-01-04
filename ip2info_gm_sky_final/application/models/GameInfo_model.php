<?php
class GameInfo_model extends CI_Model
{
    public function __construct() {
    	parent::__construct();
        $this->load->database();
    }
    
    public function __destruct() {
        $this->db->close();
    }

    public function selectGameInfo($data = array()) {
    	$fields = array('PROVINCE', 'GAME_NAME', 'PLATFORM', 'WIRELESS_TYPE', 'ITEM_KEY', 'ITEM_VALUE', 'UPDATE_TIME');
    	$wheres = $data;

    	$result = $this->db->select(implode(',', $fields))->from('op_gameinfo')->where($wheres)->get();
    	// $result = $this->db->select(implode(',', $fields))->from('op_gameinfo')->get();
    	return $result->result_array();
    }

    public function insertGameInfo($data = array()) {
    	date_default_timezone_set("Asia/Shanghai");
    	$data['UPDATE_TIME'] = date("Y-m-d h:i:sa");
    	$this->db->insert('op_gameinfo', $data);
    }

    public function modifyGameInfo($data = array()) {
    	date_default_timezone_set("Asia/Shanghai");
    	$data['UPDATE_TIME'] = date("Y-m-d h:i:sa");
    	$where = array(
    		'PROVINCE' => $data['PROVINCE'],
    		'GAME_NAME' => $data['GAME_NAME'],
    		'PLATFORM' => $data['PLATFORM'],
    		'WIRELESS_TYPE' => $data['WIRELESS_TYPE'],
    		'ITEM_KEY' => $data['ITEM_KEY'],
    	);
    	$this->db->where($where)->update('op_gameinfo', $data);
    }

    public function deleteGameInfo($data = array()) {
    	$where = array(
    		'PROVINCE' => $data['PROVINCE'],
    		'GAME_NAME' => $data['GAME_NAME'],
    		'PLATFORM' => $data['PLATFORM'],
    		'WIRELESS_TYPE' => $data['WIRELESS_TYPE'],
    		'ITEM_KEY' => $data['ITEM_KEY'],
    	);
    	$this->db->where($where)->delete('op_gameinfo', $data);
    }
}
?>