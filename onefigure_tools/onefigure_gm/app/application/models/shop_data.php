<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Shop_data extends CI_Model {
    public function __construct() {
        parent::__construct();
        //print_r($this);exit();

        $this->load->database($_SESSION['db_group']);
    }

    public function __destruct() {
        $this->db->close();
    }

    public function selectItemInfo($data = array()) {
        $fields = array(
            'ITEM_INFO_ID',
            'NAME',
            'PRICE_TYPE',
            'PRICE_AMOUNT',
            'AMOUNT'
        );
        $wheres = array(
            'IS_DELETE' => 0
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('game_item')
            ->where($wheres)
            ->order_by('ITEM_INFO_ID', 'ASC')
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            $return[$row['ITEM_INFO_ID']] = $row;
        }

        return $return;
    }

    public function selectShopItemInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));
        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-shop_item'.$version;
        if (!$return = $this->cache->get($cacheName)) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('newdocs');

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['iap']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_iap'.$version, $contents);
            $docsRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_iap'.$version);

            $return = array();
            foreach ($docsRows as $row) {
                $packages = array();
                if (strtolower($row['productType']) == 'money') {
                    $product_type = 0;
                    $product_id = $row['itemID'];
                    $price_type = 1;
                    $amount = $row['amount'];
                } else if (strtolower($row['productType']) == 'cash') {
                    $product_type = 1;
                    $product_id = $row['productID'];
                    $price_type = -1;
                    $amount = $row['amount'];
                } else if (strtolower($row['productType']) == 'stamina') {
                    $product_type = 2;
                    $product_id = $row['itemID'];
                    $price_type = 1;
                    $amount = $row['amount'];
                } else if (strtolower($row['productType']) == 'special') {
                    $product_type = 3;
                    $product_id = $row['productID'];
                    $price_type = 1;
                    $amount = $row['amount'];
                } else if (strtolower($row['productType']) == 'package') {
                    $product_type = 99;
                    $product_id = $row['itemID'];
                    $price_type = 1;
                    $amount = 0;

                    foreach (explode(',', $row['amount']) as $item) {
                        if (is_string($item) && strlen($item) > 5) {
                            list($item_type, $item_amount) = explode(' ', trim(strtolower($item)));

                            if ($item_type == 'money') {
                                $packages[] = array('TYPE' => 0, 'AMOUNT' => $item_amount);
                            } else if ($item_type == 'cash') {
                                $packages[] = array('TYPE' => 1, 'AMOUNT' => $item_amount);
                            } else if ($item_type == 'stamina') {
                                $packages[] = array('TYPE' => 2, 'AMOUNT' => $item_amount);
                            } else if ($item_type == 'mystique') {
                                $packages[] = array('TYPE' => 3, 'ITEM_ID' => $item_amount, 'AMOUNT' => 1);
                            } else if ($item_type == 'stance') {
                                $packages[] = array('TYPE' => 4, 'ITEM_ID' => $item_amount, 'AMOUNT' => 1);
                            } else if ($item_type == 'artifact') {
                                $packages[] = array('TYPE' => 5, 'ITEM_ID' => $item_amount, 'AMOUNT' => 1);
                            } else if ($item_type == 'lotterypoint') {
                                $packages[] = array('TYPE' => 6, 'AMOUNT' => $item_amount);
                            } else if ($item_type == 'exp') {
                                $packages[] = array('TYPE' => 7, 'AMOUNT' => $item_amount);
                            } else if (strpos(strtolower($item_type), 'booster') === 0) {
                                $packages[] = array('TYPE' => 8, 'ITEM_ID' => substr($item_type, 7), 'AMOUNT' => $item_amount);
                            } else if ($item_type == 'item') {
                                if ($item_amount < 100) {
                                    $type = 4;
                                } else if ($item_amount < 1000) {
                                    $type = 3;
                                } else {
                                    $type = 5;
                                }

                                $packages[] = array('TYPE' => $type, 'ITEM_ID' => $item_amount, 'AMOUNT' => 1);
                            } else if ($item_type == 'highticket') {
                                $packages[] = array('TYPE' => 9, 'AMOUNT' => $item_amount);
                            } else if ($item_type == 'ticket') {
                                $packages[] = array('TYPE' => 10, 'AMOUNT' => $item_amount);
                            }
                        }
                    }
                } else {
                    $product_type = -1;
                    $product_id = $row['productID'];
                    $price_type = -1;
                    $amount = $row['amount'];
                }

                $return[$product_id] = array(
                    'ITEM_ID' => $row['itemID'],
                    'PRODUCT_TYPE' => $product_type,
                    'PRODUCT_ID' => $product_id,
                    'AMOUNT' => $amount,
                    'NAME' => $row['name'],
                    'PRICE_TYPE' => $price_type,
                    'PRICE_KR' => $row['priceKr'],
                    'PRICE_US' => $row['priceUs'],
                    'FIRST_BUY' => $row['firstBuy'],
                    'MORE_AMOUNT' => 0,
                    'MORE_PERCENT' => 0,
                    'PACKAGE' => $packages
                );
            }

            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
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

    public function selectGamblingInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));
        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-gambling'.$version;
        if (!$return = $this->cache->get($cacheName)) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('newdocs');

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['gambling']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_gambling'.$version, $contents);
            $docsRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_gambling'.$version);

            $group_array = array('Reward_Stage' => 1, 'PresentBox' => 2);
            $type_array = array('Money' => 0, 'Cash' => 1, 'Stamina' => 2, 'Mystique' => 3, 'Stance' => 4, 'Artifact' => 5, 'Point' => 6, 'Exp' => 7);

            $return = array();
            foreach ($docsRows as $row) {
                $return[$group_array[$row['groupID']]][] = array(
                    'GROUP' => $group_array[$row['groupID']],
                    'TYPE' => $type_array[trim($row['type'])],
                    'ITEM_ID' => $row['itemIndex'],
                    'AMOUNT' => $row['count'],
                    'RATE' => $row['rate'],
                );
            }

            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }

    public function checkExpAndLevelup( $levelInfoRows, &$data = Array())
    {
        $current_level_info = $levelInfoRows[$data['LEVEL']];
        $levelup = false;

        while (true) {
            if ($current_level_info['MAX_EXP_POINT'] <= $data['EXP_POINT'] && array_key_exists($data['LEVEL']+1, $levelInfoRows )) {
                $data['LEVEL'] = $data['LEVEL'] + 1;
                $data['EXP_POINT'] = $data['EXP_POINT'] - $current_level_info['MAX_EXP_POINT'];

                $this->updateUserPoint(1, $current_level_info['REWARD']['MONEY'], USER_POINT_LEVELUP_REWARD, $data['USER_ID']);
                $this->updateUserPoint(2, $current_level_info['REWARD']['CASH'], USER_POINT_LEVELUP_REWARD, $data['USER_ID']);

                //                    if (MAX_USER_HEART > $data['HEART']) {
                //                        $data['HEART'] = MAX_USER_HEART;
                //                    }

                $data['HEART'] = $data['HEART'] + LEVELUP_HEART_BONUS;

                $current_level_info = $levelInfoRows[$data['LEVEL']];
                $levelup = true;
            } else {
                break;
            }
        }

        return $levelup;

    }

    public function updateUserPointSecure($point_type, $point, $category, $user_id) {
        if (!$point) return false;

        $this->load->model('Log_data');
        $bonus_point = 0;

        if ($point_type == USER_POINT_MONEY) {
            $point_field = '`MONEY`';
            $field_set = $point_field . ' + ' . $point;

            if ($point <= -1000) {
                $bonus_point = floor(abs($point) / 1000);
            }
//            else if ($point <= -500) {
//                $bonus_point = 3;
//            }

            $this->datainputoutput->set('money', $this->datainputoutput->get('money') + $point);
        } else if ($point_type == USER_POINT_CASH) {
            $point_field = '`CASH`';
            $field_set = $point_field . ' + ' . $point;

            if($point < 0)
                $bonus_point = abs($point);

            $this->datainputoutput->set('cash', $this->datainputoutput->get('cash') + $point);
        } else if ($point_type == USER_POINT_LOTTERY_POINT) {
            $point_field = '`LOTTERY_POINT`';
            $field_set = 'IF((' . $point_field . ' + ' . $point . ') > ' . MAX_LOTTERY_POINT . ', ' . MAX_LOTTERY_POINT . ', ' . $point_field . ' + ' . $point . ')';

            $this->datainputoutput->set('lotteryPoint', $this->datainputoutput->get('lotteryPoint') + $point);
        } else if ($point_type == USER_POINT_COUPON) {
            $point_field = '`LOTTERY_COUPON`';
            $field_set = $point_field . ' + ' . $point;

            $this->datainputoutput->set('lotteryCoupon', $this->datainputoutput->get('lotteryCoupon') + $point);
        } else if ($point_type == USER_POINT_HIGH_COUPON) {
            $point_field = '`LOTTERY_HIGH_COUPON`';
            $field_set = $point_field . ' + ' . $point;

            $this->datainputoutput->set('lotteryHighCoupon', $this->datainputoutput->get('lotteryHighCoupon') + $point);
        }

        $this->db->trans_start();

//        $this->db->query( 'update `op_users` set '.$point_field.'='.$field_set.' where `USER_ID`='.$user_id.' and '.$field_set.'>=0');
        $this->db
            ->where('USER_ID', $user_id)
            ->where( $field_set .'>=', 0)
            ->set($point_field, $field_set, false)
            ->update('users');

        $updated = $this->db->affected_rows() > 0;
        if ($updated) {
            $this->Log_data->insertPointLog($point_type, $point, $category, $user_id);

            if ($bonus_point > 0) {
                $this->db
                    ->where('USER_ID', $user_id)
                    ->set('LOTTERY_POINT', 'IF((LOTTERY_POINT + ' . $bonus_point . ') > ' . MAX_LOTTERY_POINT . ', ' . MAX_LOTTERY_POINT . ', LOTTERY_POINT + ' . $bonus_point . ')', false)
                    ->update('users');
                $this->Log_data->insertPointLog(3, $bonus_point, $category, $user_id);

                $this->datainputoutput->set('lotteryPoint', $this->datainputoutput->get('lotteryPoint') + $bonus_point);
            }
        }








        $this->db->trans_complete();

        return $updated;
    }



    public function updateUserPoint($point_type, $point, $category, $user_id) {
        if (!$point) return false;

        $this->load->model('Log_data');
        $bonus_point = 0;

        if ($point_type == 1) {
            $point_field = 'MONEY';
            $field_set = $point_field . ' + ' . $point;

            if ($point <= -1000) {
                $bonus_point = floor(abs($point) / 1000);
            }
//            else if ($point <= -500) {
//                $bonus_point = 3;
//            }

      //      $this->datainputoutput->set('money', $this->datainputoutput->get('money') + $point);
        } else if ($point_type == 2) {
            $point_field = 'CASH';
            $field_set = $point_field . ' + ' . $point;

            if($point < 0)
                $bonus_point = abs($point);

     //       $this->datainputoutput->set('cash', $this->datainputoutput->get('cash') + $point);
        } else if ($point_type == 3) {
            $point_field = 'LOTTERY_POINT';
            $field_set = 'IF((' . $point_field . ' + ' . $point . ') > ' . MAX_LOTTERY_POINT . ', ' . MAX_LOTTERY_POINT . ', ' . $point_field . ' + ' . $point . ')';

      //      $this->datainputoutput->set('lotteryPoint', $this->datainputoutput->get('lotteryPoint') + $point);
        } else if ($point_type == 4) {
            $point_field = 'LOTTERY_COUPON';
            $field_set = $point_field . ' + ' . $point;

       //     $this->datainputoutput->set('lotteryCoupon', $this->datainputoutput->get('lotteryCoupon') + $point);
        } else if ($point_type == 5) {
            $point_field = 'LOTTERY_HIGH_COUPON';
            $field_set = $point_field . ' + ' . $point;

       //     $this->datainputoutput->set('lotteryHighCoupon', $this->datainputoutput->get('lotteryHighCoupon') + $point);
        }

        $this->db->trans_start();
        $this->db
            ->where('USER_ID', $user_id)
            ->set($point_field, $field_set, false)
            ->update('users');
        $this->Log_data->insertPointLog($point_type, $point, $category, $user_id);

        /*
        if ($bonus_point > 0) {
            $this->db
                ->where('USER_ID', $user_id)
                ->set('LOTTERY_POINT', 'IF((LOTTERY_POINT + ' . $bonus_point . ') > ' . MAX_LOTTERY_POINT . ', ' . MAX_LOTTERY_POINT . ', LOTTERY_POINT + ' . $bonus_point . ')', false)
                ->update('users');
            $this->Log_data->insertPointLog(3, $bonus_point, $category, $user_id);

            $this->datainputoutput->set('lotteryPoint', $this->datainputoutput->get('lotteryPoint') + $bonus_point);
        }
*/
        $this->db->trans_complete();
    }

    public function selectUserGameItem($data = array()) {
        $fields = array(
            'USER_ID',
            'ITEM_INFO_ID',
            'AMOUNT',
        );
        $wheres = array(
            'USER_ID' => $data['USER_ID']
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_items')
            ->where($wheres)
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            $return[$row['ITEM_INFO_ID']] = $row['AMOUNT'];
        }

        return $return;
    }

    public function updateUserGameItem($data = array()) {
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'ITEM_INFO_ID' => $data['ITEM_INFO_ID'],
        );

        $this->db->trans_start();

        $count = $this->db
            ->from('users_items')
            ->where($wheres)
            ->count_all_results();

        if ($count > 0) {
            if (isset($data['IS_AMOUNT']) && $data['IS_AMOUNT'] == true) {
                $this->db->set('AMOUNT', 'AMOUNT + 1', false);
            } else if (isset($data['IS_LEVEL']) && $data['IS_LEVEL'] == true) {
                $this->db->set('LEVEL', 'LEVEL + 1', false);
            }

            $this->db
                ->where($wheres)
                ->update('users_items');
        } else {
            $insert = array(
                'USER_ID' => $data['USER_ID'],
                'ITEM_INFO_ID' => $data['ITEM_INFO_ID'],
                'AMOUNT' => 1,
                'CREATE_DATE' => date('Y-m-d H:i:s'),
            );

            if (isset($data['IS_LEVEL']) && $data['IS_LEVEL'] == true) {
                $insert['LEVEL'] = 1;
            }

            $this->db->insert('users_items', $insert);
        }

        $this->db->trans_complete();
    }

    public function selectMarketOrder($data = array()) {
        $fields = array(
            'USER_ID',
            'PRODUCT_ID',
            'ORDER_ID',
            'PURCHASE_TOKEN',
            'SIGNATURE',
            'CREATE_DATE',
        );
        $wheres = array(
            'ORDER_ID' => $data['ORDER_ID']
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('market_order')
            ->where($wheres)
            ->get();
        $return = $result->row_array();

        return $return;
    }

    public function selectUserMarketOrderCount($data = array()) {
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'PRODUCT_ID' => $data['PRODUCT_ID'],
        );

        $count = $this->db
            ->from('market_order')
            ->where($wheres)
            ->count_all_results();

        return $count;
    }

    public function selectUserMarketOrderForAll($data = array()) {
//        $wheres = array(
//            'USER_ID' => $data['USER_ID'],
//        );
//
//        $result = $this->db->select( 'PRODUCT_ID, count(PRODUCT_ID) as COUNT')
//            ->from('market_order')
//            ->where($wheres)->group_by('PRODUCT_ID')
//            ->get();
//
//        $return = $result->row_array();
//
        $sql = 'SELECT `PRODUCT_ID`, count(`PRODUCT_ID`) as COUNT FROM `op_market_order` WHERE `USER_ID`='.$data['USER_ID'].' GROUP BY `PRODUCT_ID`';

        $result = $this->db->query( $sql);

        $return = array();
        foreach ($result->result_array() as $row) {
            $return[$row['PRODUCT_ID']] = $row;
        }

        return $return;
    }

    public function insertMarketOrder($data = array()) {
        $insert = array(
            'USER_ID' => $data['USER_ID'],
            'PRODUCT_ID' => $data['PRODUCT_ID'],
            'ORDER_ID' => $data['ORDER_ID'],
            'PURCHASE_TOKEN' => $data['PURCHASE_TOKEN'],
            'SIGNATURE' => $data['SIGNATURE'],
            'RECEIPT' => $data['RECEIPT_DATA'],
            'CREATE_DATE' => date('Y-m-d H:i:s'),
        );

        return $this->db->insert('market_order', $insert);
    }

    public function selectBuy($data = array()) {
        $fields = array(
            'BUY_ID',
            'IS_FINISH',
            'CREATE_DATE',
            'FINISH_DATE',
        );
        $wheres = array(
            'BUY_ID' => $data['BUY_ID'],
            'USER_ID' => $data['USER_ID']
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('buy')
            ->where($wheres)
            ->get();
        $return = $result->row_array();

        return $return;
    }

    public function insertBuy($data = array()) {
        $insert = array(
            'USER_ID' => $data['USER_ID'],
            'IS_FINISH' => 0,
            'CREATE_DATE' => date('Y-m-d H:i:s'),
        );

        $this->db->insert('buy', $insert);

        if ($this->db->affected_rows() > 0) {
            return $this->db->insert_id();
        } else {
            return -1;
        }
    }

    public function updateBuy($data = array()) {
        $wheres = array(
            'BUY_ID' => $data['BUY_ID'],
            'USER_ID' => $data['USER_ID']
        );

        $update = array(
            'IS_FINISH' => 1,
            'FINISH_DATE' => date('Y-m-d H:i:s'),
        );

        return $this->db
            ->where($wheres)
            ->update('buy', $update);
    }
}
