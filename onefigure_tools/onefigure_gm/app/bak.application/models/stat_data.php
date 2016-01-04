<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Stat_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->REPLICATION_DB = $this->load->database('replication', true);
        $this->LOG_DB = $this->load->database('log', true);
    }

    public function __destruct() {
        $this->REPLICATION_DB->close();
        $this->LOG_DB->close();
    }

    public function selectStat($data = array()) {
        $fields = array(
            'DATA_DATE',
            'DATA_KEY',
            'DATA_VALUE',
        );

        $result = $this->LOG_DB
            ->select(implode(', ', $fields))
            ->from('stat_data')
            ->like('DATA_KEY', $data['DATA_KEY'], 'after')
            ->where('DATE_FORMAT(`DATA_DATE`, "%Y-%m-%d") BETWEEN "' . $data['START_DATE'] . '" AND "' . $data['END_DATE'] . '"', null, false)
            ->order_by('DATA_DATE')
            ->order_by('DATA_KEY')
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            if (strpos($row['DATA_KEY'], ':') > 0) {
                $exp = explode(':', $row['DATA_KEY']);

                if (count($exp) == 2) {
                    $return[$row['DATA_DATE']][$exp[1]] = $row;
                } else if (count($exp) == 3) {
                    $return[$row['DATA_DATE']][$exp[1]][$exp[2]] = $row;
                } else if (count($exp) == 4) {
                    $return[$row['DATA_DATE']][$exp[1]][$exp[2]][$exp[3]] = $row;
                }
            } else {
                $return[$row['DATA_DATE']] = $row;
            }
        }

        return $return;
    }
}