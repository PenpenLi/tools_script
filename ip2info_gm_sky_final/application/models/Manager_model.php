<?php
class Manager_model extends CI_Model
{
    public function __construct() {
    	parent::__construct();
        $this->load->database();
    }
    
    public function __destruct() {
        $this->db->close();
    }

    public function selectManagerInfo($data = array()) {
    	if (!isset($data['USERNAME'])) {
    		return array();
    	}
    	$fields = array('USERNAME', 'PASSWORD');
    	$wheres = array('USERNAME' => $data['USERNAME']);

    	$result = $this->db->select(implode(',', $fields))->from('op_manager')->where($wheres)->get();
    	return $result->row_array();
    }
}
?>
