<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Character_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        //die($this->SELECT_DB);

        $this->load->database();
    }

    public function __destruct() {
        $this->db->close();
    }



    public function selectCharacterInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));


        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-character'.$version;

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
                $enchant['StanceA'][$level]['MONEY'] = $row['StanceA'];
                $enchant['StanceB'][$level]['MONEY'] = $row['StanceB'];
                $enchant['StanceC'][$level]['MONEY'] = $row['StanceC'];

                if (isset($row['MantisL'])) $enchant['MantisL'][$level]['MONEY'] = $row['MantisL'];
                if (isset($row['DrunkenL'])) $enchant['DrunkenL'][$level]['MONEY'] = $row['DrunkenL'];
                if (isset($row['CraneL'])) $enchant['CraneL'][$level]['MONEY'] = $row['CraneL'];
                if (isset($row['EagleL'])) $enchant['EagleL'][$level]['MONEY'] = $row['EagleL'];
                if (isset($row['TigerL'])) $enchant['TigerL'][$level]['MONEY'] = $row['TigerL'];
                if (isset($row['CapoeiraL'])) $enchant['CapoeiraL'][$level]['MONEY'] = $row['CapoeiraL'];
                if (isset($row['MuayThaiL'])) $enchant['MuayThaiL'][$level]['MONEY'] = $row['MuayThaiL'];
            }

            $return = array();
            foreach ($itemRows as $row) {
                if ($row['index'] > 99) continue;

                $buy = array();
                foreach ($priceRows as $r) {
                    if ($r['type'] == 'MystiqueSlot' || $r['type'] == 'ArtifactSlot') continue;

                    if ($r['index'] == $row['index']) {
                        if ($r['limitType'] == 'Level') {
                            $buy['LIMIT']['LEVEL'] = $r['limitValue'];
                        }

                        if ($r['limitBuyType'] == 'Cash') {
                            $buy['LIMIT']['CASH'] = (int)($r['limitBuyValue']*(100-$r['saleFactor'])/100);
                        }

                        if ($r['buyType'] == 'Money') {
                            $buy['MONEY'] = (int)($r['buyValue']*(100-$r['saleFactor'])/100);
                        } else if ($r['buyType'] == 'Cash') {
                            $buy['CASH'] = (int)($r['buyValue']*(100-$r['saleFactor'])/100);
                        }
                    }
                }

                $return[$row['index']] = array(
                    'CHARACTER_INFO_ID' => $row['index'],
                    'NAME' => $row['name'],
                    'ENCHANT_TYPE' => $row['enchantPriceType'],
                    'BUY' => $buy,
                    'UPGRADE' => $enchant[$row['enchantPriceType']],
                );
            }

            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }

    /*
    public function selectCharacterInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));

        if (!$return = $this->cache->get('ofdp-character')) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('docs');

            $this->curl->create($docs['url']['item']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_item', $contents);
            $itemRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_item');

            $this->curl->create($docs['url']['price']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_price', $contents);
            $priceRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_price');

            $this->curl->create($docs['url']['enchant']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_enchant', $contents);
            $enchantRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_enchant');

            $enchant = array();
            foreach ($enchantRows as $index => $row) {
                $level = $row['Level'] + 1;
                $enchant['StanceA'][$level]['MONEY'] = $row['StanceA'];
                $enchant['StanceB'][$level]['MONEY'] = $row['StanceB'];
                $enchant['StanceC'][$level]['MONEY'] = $row['StanceC'];

                if (isset($row['MantisL'])) $enchant['MantisL'][$level]['MONEY'] = $row['MantisL'];
                if (isset($row['DrunkenL'])) $enchant['DrunkenL'][$level]['MONEY'] = $row['DrunkenL'];
                if (isset($row['CraneL'])) $enchant['CraneL'][$level]['MONEY'] = $row['CraneL'];
                if (isset($row['EagleL'])) $enchant['EagleL'][$level]['MONEY'] = $row['EagleL'];
                if (isset($row['TigerL'])) $enchant['TigerL'][$level]['MONEY'] = $row['TigerL'];
            }

            $return = array();
            foreach ($itemRows as $row) {
                if ($row['index'] > 99) continue;

                $buy = array();
                foreach ($priceRows as $r) {
                    if ($r['type'] == 'MystiqueSlot' || $r['type'] == 'ArtifactSlot') continue;

                    if ($r['index'] == $row['index']) {
                        if ($r['limitType'] == 'Level') {
                            $buy['LIMIT']['LEVEL'] = $r['limitValue'];
                        }

                        if ($r['limitBuyType'] == 'Cash') {
                            $buy['LIMIT']['CASH'] = $r['limitBuyValue'];
                        }

                        if ($r['buyType'] == 'Money') {
                            $buy['MONEY'] = $r['buyValue'];
                        } else if ($r['buyType'] == 'Cash') {
                                $buy['CASH'] = $r['buyValue'];
                        }
                    }
                }

                $return[$row['index']] = array(
                        'CHARACTER_INFO_ID' => $row['index'],
                        'NAME' => $row['name ref'],
                        'ENCHANT_TYPE' => $row['enchantPriceType'],
                        'BUY' => $buy,
                        'UPGRADE' => $enchant[$row['enchantPriceType']],
                    );
            }

            $this->cache->save('ofdp-character', $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }
    */

    public function selectUserCharacters($data = array()) {
        $characterInfoRows = $this->selectCharacterInfo($data);

        $fields = array(
                'CHARACTER_ID',
                'CHARACTER_INFO_ID',
                'LEVEL',
                'CREATE_DATE',
            );
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
            );
            
        if (!empty($data['CHARACTER_ID'])) {
            $wheres['CHARACTER_ID'] = $data['CHARACTER_ID'];
        }
            
        if (!empty($data['CHARACTER_INFO_ID'])) {
            $wheres['CHARACTER_INFO_ID'] = $data['CHARACTER_INFO_ID'];
        }
            
        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_characters')
            ->where($wheres)
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            $character_info = $characterInfoRows[$row['CHARACTER_INFO_ID']];
            $row['NAME'] = $character_info['NAME'];

            $return[] = $row;
        }

        return $return;
    }

    public function selectSlotCharacter($data = array(), $return_type = 'json') {
        $fields = array(
                'CHARACTER_ID',
                'CHARACTER_INFO_ID',
                'LEVEL',
                'CREATE_DATE',
            );
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
                'CHARACTER_ID' => $this->_user['CHARACTER_ID'],
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_characters')
            ->where($wheres)
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            if ($return_type == 'json') {
                $return[] = array(
                        'itemID' => $row['CHARACTER_ID'],
                        'itemOriginalID' => $row['CHARACTER_INFO_ID'],
                        'level' => $row['LEVEL'],
                    );
            } else {
                $return[] = $row;
            }
        }

        return $return;
    }

    public function insertUserCharacter($data = array()) {
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'CHARACTER_INFO_ID' => $data['CHARACTER_INFO_ID'],
        );

        if ($this->db->from('users_characters')->where($wheres)->count_all_results() > 0) {
            $this->updateUserCharacter($data);
        } else {
            $insert = array('CHARACTER_INFO_ID' => $data['CHARACTER_INFO_ID'], 'USER_ID' => $data['USER_ID'], 'LEVEL' => 1, 'CREATE_DATE' => date('Y-m-d H:i:s'),);

            $this->db->insert('users_characters', $insert);

            if ($this->db->affected_rows() > 0) {
                return $this->db->insert_id();
            } else {
                return -1;
            }
        }
    }
    
    public function updateUserCharacter($data = array()) {
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
            );

        if (isset($data['CHARACTER_ID'])) {
            $wheres['CHARACTER_ID'] = $data['CHARACTER_ID'];
        } else if (isset($data['CHARACTER_INFO_ID'])) {
            $wheres['CHARACTER_INFO_ID'] = $data['CHARACTER_INFO_ID'];
        }

        return $this->db
            ->where($wheres)
            ->set('LEVEL', 'LEVEL + 1', false)
            ->update('users_characters');
    }
}
