<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Treasure_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->load->database();
    }

    public function __destruct() {
        $this->db->close();
    }

    public function selectTreasureInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));

        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-treasure'.$version;

        if (!$return = $this->cache->get($cacheName)) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('newdocs');

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['item']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_item'.$version, $contents);
            $itemRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_item'.$version);

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['price']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_price'.$version, $contents);
            $priceRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_price'.$version);

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['enchant']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_enchant'.$version, $contents);
            $enchantRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_enchant'.$version);

            $enchant = array();
            foreach ($enchantRows as $index => $row) {
                $level = $row['Level'] + 1;
                $enchant['A'][$level] = array('RATE' => $row['ArtifactARate'], 'CASH' => $row['ArtifactACash'], 'MONEY' => $row['ArtifactA']);
                $enchant['B'][$level] = array('RATE' => $row['ArtifactBRate'], 'CASH' => $row['ArtifactBCash'], 'MONEY' => $row['ArtifactB']);
                $enchant['C'][$level] = array('RATE' => $row['ArtifactCRate'], 'CASH' => $row['ArtifactCCash'], 'MONEY' => $row['ArtifactC']);
                $enchant['S'][$level] = array('RATE' => $row['ArtifactSRate'], 'CASH' => $row['ArtifactSCash'], 'MONEY' => $row['ArtifactS']);
            }

            $return = array();
            foreach ($itemRows as $row) {
                if ($row['index'] < 1000) continue;

                $buy = array();
                foreach ($priceRows as $r) {
                    if ($r['type'] == 'MystiqueSlot' || $r['type'] == 'ArtifactSlot') continue;

                    if ($r['index'] == $row['index']) {
                        if ($r['limitType'] == 'Level') {
                            $buy['LEVEL'] = $r['limitValue'];
                        }

                        if ($r['limitBuyType'] == 'Cash') {
                            $buy['CASH'] = $r['limitBuyValue'];
                        }

                        if ($r['buyType'] == 'Money') {
                            $buy['MONEY'] = $r['buyValue'];
                        }
                    }
                }

                $return[$row['index']] = array(
                    'TREASURE_INFO_ID' => $row['index'],
                    'NAME' => $row['name ref'],
                    'GRADE' => $row['grade'],
                    'GATCHA_RATE' => $row['gatchaRate'],
                    'BUY' => $buy,
                    'SELL' => array('MONEY' => $row['sellPrice']),
                    'UPGRADE' => $enchant[$row['grade']],
                    'KEY'=> $row['name']
                );
            }

            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }

    public function selectUserTreasures($data = array()) {
        $treasureInfoRows = $this->selectTreasureInfo($data);

        $fields = array(
                'TREASURE_ID',
                'TREASURE_INFO_ID',
                'LEVEL',
                'SLOT_NUMBER',
                'CREATE_DATE',
            );
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
                'IS_DELETE' => 0
            );
            
        if (!empty($data['TREASURE_ID'])) {
            $wheres['TREASURE_ID'] = $data['TREASURE_ID'];
        }
            
        if (!empty($data['TREASURE_INFO_ID'])) {
            $wheres['TREASURE_INFO_ID'] = $data['TREASURE_INFO_ID'];
        }
            
        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_treasures')
            ->where($wheres)
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            $treasure_info = $treasureInfoRows[$row['TREASURE_INFO_ID']];
            $row['NAME'] = $treasure_info['NAME'];
				//jLee Ãß°¡
				$row['KEY']=$treasure_info['KEY'];
            $return[] = $row;
        }

        return $return;
    }

    public function selectSlotTreasures($data = array(), $return_type = 'json') {
        $fields = array(
                'TREASURE_ID',
                'TREASURE_INFO_ID',
                'LEVEL',
                'SLOT_NUMBER',
            );
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
                'IS_DELETE' => 0
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_treasures')
            ->where($wheres)
            ->where_in('SLOT_NUMBER', array(1, 2, 3))
            ->order_by('SLOT_NUMBER')
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            if ($return_type == 'json') {
                $return[] = array(
                        'itemID' => $row['TREASURE_ID'],
                        'itemOriginalID' => $row['TREASURE_INFO_ID'],
                        'level' => $row['LEVEL'],
                        'slot' => $row['SLOT_NUMBER']
                    );
            } else {
                $return[] = $row;
            }
        }

        return $return;
    }

    public function selectRandomTreasures($data = array()) {
        $treasureInfoRows = $this->selectTreasureInfo($data);
        $treasureRows = $this->selectUserTreasures($data, 'array');

        foreach ($treasureRows as $row) {
            if ($row['LEVEL'] == MAX_TREASURE_LEVEL) {
                unset($treasureInfoRows[$row['TREASURE_INFO_ID']]);
            }
        }

        $return = array();
        foreach ($treasureInfoRows as $row) {
            $return[$row['GRADE']][] = $row;
        }

        return $return;
    }

    public function selectGatchaTreasures($data = array()) {
        $treasureInfoRows = $this->selectTreasureInfo($data);
        $treasureRows = $this->selectUserTreasures($data, 'array');

        foreach ($treasureRows as $row) {
            if ($row['LEVEL'] == MAX_TREASURE_LEVEL) {
                unset($treasureInfoRows[$row['TREASURE_INFO_ID']]);
            }
        }

        $gatcha_rate = array('A' => 0, 'B' => 0, 'C' => 0);
        $return = array();
        foreach ($treasureInfoRows as $row) {
            $gatcha_rate[$row['GRADE']] = $gatcha_rate[$row['GRADE']] + $row['GATCHA_RATE'];
            $return[$row['GRADE']][] = $row;
        }

        $return['GATCHA_RATE'] = $gatcha_rate;

        return $return;
    }

    public function insertUserTreasure($data = array()) {
        $insert = array(
                'TREASURE_INFO_ID' => $data['TREASURE_INFO_ID'],
                'USER_ID' => $data['USER_ID'],
                'LEVEL' => 1,
                'CREATE_DATE' => date('Y-m-d H:i:s'),
                'IS_DELETE' => 0,
            );

        $this->db->insert('users_treasures', $insert);

        if ($this->db->affected_rows() > 0) {
            return $this->db->insert_id();
        } else {
            return -1;
        }
    }
    
    public function updateUserTreasure($data = array()) {
        $wheres = array(
                'TREASURE_ID' => $data['TREASURE_ID'],
                'USER_ID' => $data['USER_ID'],
            );

        return $this->db
            ->where($wheres)
            ->set('LEVEL', 'IF(LEVEL = ' . MAX_TREASURE_LEVEL . ', LEVEL, LEVEL + 1)', false)
            ->update('users_treasures');
    }
    
    public function updateUserTreasureSlot($data = array()) {
        $this->db
            ->where('USER_ID', $data['USER_ID'])
            ->where('SLOT_NUMBER', $data['SLOT_NUMBER'])
            ->set('SLOT_NUMBER', null)
            ->update('users_treasures');
            
        $update = array(
                'SLOT_NUMBER' => $data['SLOT_NUMBER'],
            );

        $wheres = array(
                'TREASURE_ID' => $data['TREASURE_ID'],
                'USER_ID' => $data['USER_ID'],
            );

        return $this->db
            ->where($wheres)
            ->update('users_treasures', $update);
    }
    
    public function deleteUserTreasure($data = array()) {
        $wheres = array(
                'TREASURE_ID' => $data['TREASURE_ID'],
                'USER_ID' => $data['USER_ID'],
            );

        return $this->db
            ->where($wheres)
            ->set('IS_DELETE', 1)
            ->set('DELETE_DATE', date('Y-m-d H:i:s'))
            ->update('users_treasures');
    }
    
}