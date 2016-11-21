<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Realtime_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->load->database('service');
    }

    public function __destruct() {
        $this->db->close();
    }

    public function getUserTotalCount() {
        $result = $this->db
            #->select('OS_TYPE, COUNT(USER_ID) AS C', false)
            ->select('MARKET_TYPE, COUNT(USER_ID) AS C', false)
            ->from('users')
            ->where('USER_ID > ', 0)
            ->where('STATUS', 1)
            #->group_by(array('OS_TYPE'))
            ->group_by(array('MARKET_TYPE'))
            ->get();
        
        $return = array(1 => 0, 2 => 0);
        foreach ($result->result_array() as $row) {
            #$return[$row['OS_TYPE']] = $row['C'];
            $return[$row['MARKET_TYPE']] = $row['C'];
        }
        
        return $return;
    }
    
    public function getUserDailyJoinCountRow() {
        $result = $this->db
            #->select('DATE_FORMAT(CREATE_DATE, "%m-%d") AS D, OS_TYPE, COUNT(*) AS C', false)
            ->select('DATE_FORMAT(CREATE_DATE, "%m-%d") AS D, MARKET_TYPE, COUNT(*) AS C', false)
            ->from('users')
            ->where('USER_ID > ', 0)
            ->where('STATUS', 1)
            ->where('CREATE_DATE >', date('Y-m-d 00:00:00', strtotime('-4 day')))
            #->group_by(array('D', 'OS_TYPE'))
            ->group_by(array('D', 'MARKET_TYPE'))
            ->get();
        
        $return = array();
        foreach ($result->result_array() as $row) {
            #$return[$row['OS_TYPE']][$row['D']] = $row['C'];
            $return[$row['MARKET_TYPE']][$row['D']] = $row['C'];
        }
        
        return $return;
    }
    
    public function getUserDailyPaymentCountRow() {
        $result = $this->db
            ->select('DATE_FORMAT(CREATE_DATE, "%m-%d") AS D, PRODUCT_ID, COUNT(*) AS C', false)
            ->from('market_order')
            ->where('CREATE_DATE >', date('Y-m-d 00:00:00', strtotime('-4 day')))
            #->where('PURCHASE_TOKEN <>', '')
            ->group_by(array('D', 'PRODUCT_ID'))
            ->get();

        $this->load->model('Shop_data');
        $item_info = $this->Shop_data->selectShopItemInfo();
        
        $return = array();
        foreach ($result->result_array() as $row) {
            if (!isset($return[$row['D']])) {
                $return[$row['D']] = array('COUNT' => 0, 'PRICE' => 0);
            }

            if (isset($item_info[$row['PRODUCT_ID']]) && $row['C'] > 0) {
                $return[$row['D']]['COUNT'] += $row['C'];
                $return[$row['D']]['PRICE'] += $item_info[$row['PRODUCT_ID']]['PRICE_US'] * $row['C'];
            }
        }
        
        return $return;
    }
############################IOS 결제 금액###############

    public function getUserDailyPaymentCountRow_IOS() {
        $result = $this->db
            ->select('DATE_FORMAT(CREATE_DATE, "%m-%d") AS D, PRODUCT_ID, COUNT(*) AS C', false)
            ->from('market_order')
            ->where('CREATE_DATE >', date('Y-m-d 00:00:00', strtotime('-4 day')))
            #->where('PURCHASE_TOKEN =', '')
            ->group_by(array('D', 'PRODUCT_ID'))
            ->get();

        $this->load->model('Shop_data');
        $item_info = $this->Shop_data->selectShopItemInfo();

        $return = array();
        foreach ($result->result_array() as $row) {
            if (!isset($return[$row['D']])) {
                $return[$row['D']] = array('COUNT' => 0, 'PRICE' => 0);
            }

            if (isset($item_info[$row['PRODUCT_ID']]) && $row['C'] > 0) {
                $return[$row['D']]['COUNT'] += $row['C'];
                $return[$row['D']]['PRICE'] += $item_info[$row['PRODUCT_ID']]['PRICE_US'] * $row['C'];
            }
        }

        return $return;
    }

####################################################################

############################NAVER 결제 금액###############

    public function getUserDailyPaymentCountRow_Naver() {
        $result = $this->db
            ->select('DATE_FORMAT(CREATE_DATE, "%m-%d") AS D, PRODUCT_ID, COUNT(*) AS C', false)
            ->from('market_order')
            ->where('CREATE_DATE >', date('Y-m-d 00:00:00', strtotime('-4 day')))
            #->where('PURCHASE_TOKEN =', '')
            ->group_by(array('D', 'PRODUCT_ID'))
            ->get();

       $this->load->model('Shop_data');
       $item_info = $this->Shop_data->selectShopItemInfo();

        $return = array();
        foreach ($result->result_array() as $row) {
            if (!isset($return[$row['D']])) {
                $return[$row['D']] = array('COUNT' => 0, 'PRICE' => 0);
            }

            if (isset($item_info[$row['PRODUCT_ID']]) && $row['C'] > 0) {
                $return[$row['D']]['COUNT'] += $row['C'];
                $return[$row['D']]['PRICE'] += $item_info[$row['PRODUCT_ID']]['PRICE_US'] * $row['C'];
            }
        }

       return $return;
    }

####################################################################



}
