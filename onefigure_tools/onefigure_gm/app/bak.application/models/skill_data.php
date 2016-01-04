<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Skill_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->load->database();
    }

    public function __destruct() {
        $this->db->close();
    }

    public function selectSkillInfo($data = array()) {

        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));

        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-skill'.$version;
		
		//apc_delete($cacheName);
		
        if (!$return = $this->cache->get($cacheName)) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('newdocs');

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['item']);
            //echo $docs['baseurl'].$version.'/'.$docs['filename']['item'].'<br>';
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_item'.$version, $contents);
            $itemRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_item'.$version);

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['price']);
            //echo $docs['baseurl'].$version.'/'.$docs['filename']['price'].'<br>';
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_price'.$version, $contents);
            $priceRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_price'.$version);

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['enchant']);
            //echo $docs['baseurl'].$version.'/'.$docs['filename']['enchant'].'<br>';
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_enchant'.$version, $contents);
            $enchantRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_enchant'.$version);

            $enchant = array();
            foreach ($enchantRows as $index => $row) {
                $level = $row['Level'] + 1;
                $enchant['A'][$level] = array('RATE' => $row['MystiqueARate'], 'CASH' => $row['MystiqueACash'], 'MONEY' => $row['MystiqueA']);
                $enchant['B'][$level] = array('RATE' => $row['MystiqueBRate'], 'CASH' => $row['MystiqueBCash'], 'MONEY' => $row['MystiqueB']);
                $enchant['C'][$level] = array('RATE' => $row['MystiqueCRate'], 'CASH' => $row['MystiqueCCash'], 'MONEY' => $row['MystiqueC']);
                $enchant['S'][$level] = array('RATE' => $row['MystiqueSRate'], 'CASH' => $row['MystiqueSCash'], 'MONEY' => $row['MystiqueS']);
            }

            $return = array();
            foreach ($itemRows as $row) {
                if ($row['index'] < 100 || $row['index'] >= 1000) continue;

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
                    'SKILL_INFO_ID' => $row['index'],
                    'NAME' => $row['name ref'],
                    'GRADE' => $row['grade'],
                    'GATCHA_RATE' => $row['gatchaRate'],
                    'BUY' => $buy,
                    'UPGRADE' => $enchant[$row['grade']],
                    'MAX_LEVEL' => array(
                        'TREASURE_INFO_ID' => $row['MaxLevelArtifact']
                    ),
                    
                    'KEY'=> $row['name']
                );
                
            }

            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }

    public function selectUserSkills($data = array()) {

        $skillInfoRows = $this->selectSkillInfo($data);

        $fields = array(
                'SKILL_ID',
                'SKILL_INFO_ID',
                'LEVEL',
                'SLOT_NUMBER',
                'CREATE_DATE',
            );
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
            );
            
        if (!empty($data['SKILL_ID'])) {
            $wheres['SKILL_ID'] = $data['SKILL_ID'];
        }
            
        if (!empty($data['SKILL_INFO_ID'])) {
            $wheres['SKILL_INFO_ID'] = $data['SKILL_INFO_ID'];
        }
            
        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_skills')
            ->where($wheres)
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            $skill_info = $skillInfoRows[$row['SKILL_INFO_ID']];
            $row['NAME'] = $skill_info['NAME'];
				
				//jLee Ãß°¡
				$row['KEY']=$skill_info['KEY'];
            $return[] = $row;
        }

        return $return;
    }

    public function selectGatchaSkills($data = array()) {
        $skillInfoRows = $this->selectSkillInfo($data);
        $skillRows = $this->selectUserSkills($data, 'array');

        foreach ($skillRows as $row) {
            if ($row['LEVEL'] == MAX_SKILL_LEVEL) {
                unset($skillInfoRows[$row['SKILL_INFO_ID']]);
            }
        }

        $gatcha_rate = array('A' => 0, 'B' => 0, 'C' => 0);
        $return = array();
        foreach ($skillInfoRows as $row) {
            $gatcha_rate[$row['GRADE']] = $gatcha_rate[$row['GRADE']] + $row['GATCHA_RATE'];
            $return[$row['GRADE']][] = $row;
        }

        $return['GATCHA_RATE'] = $gatcha_rate;

        return $return;
    }

    public function selectSlotSkills($data = array(), $return_type = 'json') {
        $fields = array(
                'SKILL_ID',
                'SKILL_INFO_ID',
                'LEVEL',
                'SLOT_NUMBER',
            );
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_skills')
            ->where($wheres)
            ->where_in('SLOT_NUMBER', array(1, 2, 3))
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            if ($return_type == 'json') {
                $return[] = array(
                        'itemID' => $row['SKILL_ID'],
                        'itemOriginalID' => $row['SKILL_INFO_ID'],
                        'level' => $row['LEVEL'],
                        'slot' => $row['SLOT_NUMBER']
                    );
            } else {
                $return[] = $row;
            }
        }

        return $return;
    }

    public function insertUserSkill($data = array()) {
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'SKILL_INFO_ID' => $data['SKILL_INFO_ID'],
        );

        if ($this->db->from('users_skills')->where($wheres)->count_all_results() > 0) {
            $this->updateUserSkill($data);
        } else {
            $insert = array('SKILL_INFO_ID' => $data['SKILL_INFO_ID'], 'USER_ID' => $data['USER_ID'], 'LEVEL' => 1, 'CREATE_DATE' => date('Y-m-d H:i:s'),);

            $this->db->insert('users_skills', $insert);

            if ($this->db->affected_rows() > 0) {
                return $this->db->insert_id();
            } else {
                return -1;
            }
        }
    }
    
    public function updateUserSkill($data = array()) {
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
            );

        if (isset($data['SKILL_ID'])) {
            $wheres['SKILL_ID'] = $data['SKILL_ID'];
        } else if (isset($data['SKILL_INFO_ID'])) {
            $wheres['SKILL_INFO_ID'] = $data['SKILL_INFO_ID'];
        }

        return $this->db
            ->where($wheres)
            ->set('LEVEL', 'IF(LEVEL = ' . MAX_SKILL_LEVEL . ', LEVEL, LEVEL + 1)', false)
            ->update('users_skills');
    }
    
    public function updateUserSkillSlot($data = array()) {
        $this->db
            ->where('USER_ID', $data['USER_ID'])
            ->where('SLOT_NUMBER', $data['SLOT_NUMBER'])
            ->set('SLOT_NUMBER', null)
            ->update('users_skills');
            
        $update = array(
                'SLOT_NUMBER' => $data['SLOT_NUMBER'],
            );

        $wheres = array(
                'SKILL_ID' => $data['SKILL_ID'],
                'USER_ID' => $data['USER_ID'],
            );

        return $this->db
            ->where($wheres)
            ->update('users_skills', $update);
    }
}