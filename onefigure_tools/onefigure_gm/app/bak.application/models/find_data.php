<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Find_data extends CI_Model {
    function __construct() {
        parent::__construct();

        $this->load->database('facebook_service');
    }

    function __destruct() {
        $this->db->close();
    }

    function getUserRows($data = array()) {
        $where_string = ' WHERE 1=1';

        $sql = 'SELECT COUNT(*) AS TOTAL_COUNT FROM `' . SERVICE_TABLE_USER . '` ' . $where_string;
        $result = $this->db->query($sql);
        $row = $result->row_array();
        $total_count = $row['TOTAL_COUNT'];

        $sql = 'SELECT
                  `user_id` AS USER_ID,
                  `bestscore` AS BEST_SCORE,
                  (`lv` + 1) AS LEVEL,
                  `exp` AS EXP,
                  `heart` AS HEART,
                  `diamond` AS DIAMOND,
                  `clover` AS CLOVER,
                  `partner_id` AS PARTNER_ID,
                  `date` AS CREATE_DATE,
                  `lastdate` AS LAST_DATE,
                  `attendcnt` AS ATTEND_COUNT
                FROM ' . SERVICE_TABLE_USER .
                $where_string . '
                ORDER BY date DESC
                LIMIT ' . (($data['PAGE'] - 1) * $data['LIMIT']) . ', ' . $data['LIMIT'];
        $result = $this->db->query($sql);

        $dataRows = array();
        foreach ($result->result_array() as $row) {
            $dataRows[] = $row;
        }

        return array('DATA' => $dataRows, 'TOTAL_COUNT' => $total_count);
    }

    function getUserCardRow($data = array()) {
        $sql = 'SELECT
                  `carddeck1` AS CARD_DECK1,
                  `carddeck2` AS CARD_DECK2,
                  `carddeck3` AS CARD_DECK3,
                  `carddeck4` AS CARD_DECK4,
                  `carddeck5` AS CARD_DECK5,
                  `carddeck6` AS CARD_DECK6,
                  `carddeck7` AS CARD_DECK7,
                  `carddeck8` AS CARD_DECK8,
                  `carddeck9` AS CARD_DECK9,
                  `carddeck10` AS CARD_DECK10,
                  `carddeck11` AS CARD_DECK11,
                  `carddeck12` AS CARD_DECK12,
                  `carddeck13` AS CARD_DECK13,
                  `carddeck14` AS CARD_DECK14,
                  `carddeck15` AS CARD_DECK15
                FROM ' . SERVICE_TABLE_CARD . '
                WHERE user_id=' . $data['USER_ID'];
        $result = $this->db->query($sql);
        $dataRow = $result->row_array();

        return $dataRow;
    }

    function getUserMissionRow($data = array()) {
        $sql = 'SELECT
                  `m_invite` AS INVITE_COUNT,
                  `m_heart` AS HEART_MAIL_COUNT,
                  `m_partner` AS HAS_PARTNER,
                  `m_review` AS IS_REVIEW
                FROM ' . SERVICE_TABLE_MISSION . '
                WHERE user_id=' . $data['USER_ID'];
        $result = $this->db->query($sql);
        $dataRow = $result->row_array();

        return $dataRow;
    }

    function setUserRow($data = array()) {
        $set_fields = array();

        if (!empty($data['CLOVER'])) {
            $set_fields[] = 'clover=clover' . $data['CLOVER'];
        }

        if (!empty($data['DIAMOND'])) {
            $set_fields[] = 'diamond=diamond' . $data['DIAMOND'];
        }

        if (!empty($data['HEART'])) {
            $set_fields[] = 'heart=heart' . $data['HEART'];
        }

        $sql = 'UPDATE ' . SERVICE_TABLE_USER . ' SET '
                . implode(', ', $set_fields) . '
                WHERE user_id=' . $data['USER_ID'];

        $this->db->trans_start();
        $this->db->query($sql);
        $this->db->trans_complete();

        return true;
    }

    function addCloverLogRow($data = array()) {
        $sql = 'INSERT INTO ' . SERVICE_TABLE_CLOVER . '
                (user_id, item_code, type, clover, create_date)
                VALUES
                (' . $data['USER_ID'] . ', 100, 9, ' . $data['CLOVER'] . ', NOW())';

        $this->db->trans_start();
        $this->db->query($sql);
        $this->db->trans_complete();

        if ($this->db->affected_rows() > 0) {
            return true;
        } else {
            return false;
        }
    }

    function addDiamondLogRow($data = array()) {
        $sql = 'INSERT INTO ' . SERVICE_TABLE_DIAMOND . '
                (user_id, item_code, type, diamond, create_date)
                VALUES
                (' . $data['USER_ID'] . ', 100, 9, ' . $data['DIAMOND'] . ', NOW())';

        $this->db->trans_start();
        $this->db->query($sql);
        $this->db->trans_complete();

        if ($this->db->affected_rows() > 0) {
            return true;
        } else {
            return false;
        }
    }
}
