<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Item_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->load->database();
    }

    public function __destruct() {
        $this->db->close();
    }

    public function selectInstantItemInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));


        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-instant_item'.$version;

        if (!$return = $this->cache->get($cacheName)) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('newdocs');

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['instant_item']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_instant_item'.$version, $contents);
            $docsRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_instant_item'.$version);

            $return = array();
            foreach ($docsRows as $row) {
                $return[$row['index']] = array(
                    'INSTANT_ITEM_ID' => $row['index'],
                    'NAME' => $row['name'],
                    'MONEY' => $row['price'],
                    'RATE' => $row['rate'],
                );
            }

            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }

    public function selectUserInstantItems($data = array()) {
        $itemInfoRows = $this->selectInstantItemInfo($data);

        $fields = array(
            'USER_ID',
            'INSTANT_ITEM_ID',
            'AMOUNT',
        );
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
        );

        if (!empty($data['INSTANT_ITEM_ID'])) {
            $wheres['INSTANT_ITEM_ID'] = $data['INSTANT_ITEM_ID'];
        }

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_instant_items')
            ->where($wheres)
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            $item_info = $itemInfoRows[$row['INSTANT_ITEM_ID']];
            $row['NAME'] = $item_info['NAME'];

            $return[$row['INSTANT_ITEM_ID']] = $row;
        }

        return $return;
    }

    public function insertUserInstantItem($data = array()) {
        $insert = array(
            'INSTANT_ITEM_ID' => $data['INSTANT_ITEM_ID'],
            'USER_ID' => $data['USER_ID'],
        );

        if ($data['INSTANT_ITEM_ID'] == 0) {
            $insert['AMOUNT'] = $data['AMOUNT'];
        } else {
            $insert['AMOUNT'] = 1;
        }

        $this->db->insert('users_instant_items', $insert);

        if ($this->db->affected_rows() > 0) {
            return $this->db->insert_id();
        } else {
            return -1;
        }
    }

    public function updateUserInstantItem($data = array()) {
        $wheres = array(
            'INSTANT_ITEM_ID' => $data['INSTANT_ITEM_ID'],
            'USER_ID' => $data['USER_ID'],
        );

        if ($this->db->from('users_instant_items')->where($wheres)->count_all_results() == 0) {
            $insert = $wheres;

            if ($data['INSTANT_ITEM_ID'] == 0) {
                $insert['AMOUNT'] = $data['AMOUNT'];
            } else {
                $insert['AMOUNT'] = 1;
            }

            return $this->db->insert('users_instant_items', $insert);
        } else {
            if ($data['INSTANT_ITEM_ID'] == 0) {
                $this->db->set('AMOUNT', $data['AMOUNT']);
            } else {
                $this->db->set('AMOUNT', 'AMOUNT + 1', false);
            }

            return $this->db
                ->where($wheres)
                ->update('users_instant_items');
        }
    }

    public function updateUserInstantItemUsed($data = array()) {
        $wheres = array(
            'INSTANT_ITEM_ID' => $data['INSTANT_ITEM_ID'],
            'USER_ID' => $data['USER_ID'],
        );

        if ($data['INSTANT_ITEM_ID'] == 0) {
            $this->db->set('AMOUNT', 0);
        } else {
            $this->db->set('AMOUNT', 'AMOUNT - 1', false);
        }

        return $this->db
            ->where($wheres)
            ->update('users_instant_items');
    }
}