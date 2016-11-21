<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

/**
 * Class Cron_data
 *
 * Key information
 * 101 : 누적 회원수
 * 102 : 가입자수
 * 103 : 접속 회원수
 * 201 : 상품별 결제수
 * 202 : 결제 회원수
 * 301 : 포인트 사용수
 * 302 : 포인트 사용액
 * 401 : 스테이지 현황
 * 402 : 캐릭터 보유 현황
 * 403 : 스킬 보유 현황
 * 404 : 보물 보유 현환
 * 405 : 게임 플레이수
 * 406 : 게임 점수
 */

class Cron_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->REPLICATION_DB = $this->load->database('replication', true);
        $this->LOG_DB = $this->load->database('log', true);
    }

    public function __destruct() {
        $this->REPLICATION_DB->close();
        $this->LOG_DB->close();
    }

    public function userCount($data = array()) {
        $result = $this->REPLICATION_DB
            ->select(array('COUNT(USER_ID) AS DATA_VALUE'))
            ->from('users')
            ->where('USER_ID >', 0)
            ->where('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") <= "' . $data['START_DATE'] . '"', null, false)
            ->get();

        foreach ($result->result_array() as $row) {
            $this->updateStatQuery($data['START_DATE'], 101, $row['DATA_VALUE']);
        }
    }

    public function userJoinCount($data = array()) {
        $result = $this->REPLICATION_DB
            ->select(array('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") AS DATA_DATE', 'COUNT(USER_ID) AS DATA_VALUE'))
            ->from('users')
            ->where('USER_ID >', 0)
            ->where('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") BETWEEN "' . $data['START_DATE'] . '" AND "' . $data['END_DATE'] . '"', null, false)
            ->group_by('DATA_DATE')
            ->get();

        foreach ($result->result_array() as $row) {
            $this->updateStatQuery($row['DATA_DATE'], 102, $row['DATA_VALUE']);
        }
    }

    public function userLoginCount($data = array()) {
        $result = $this->LOG_DB
            ->select(array('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") AS DATA_DATE', 'COUNT(DISTINCT USER_ID) AS DATA_VALUE'))
            ->from('log_login_' . date('Ym', strtotime($data['START_DATE'])))
            ->where('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") BETWEEN "' . $data['START_DATE'] . '" AND "' . $data['END_DATE'] . '"', null, false)
            ->group_by('DATA_DATE')
            ->get();

        foreach ($result->result_array() as $row) {
            $this->updateStatQuery($row['DATA_DATE'], 103, $row['DATA_VALUE']);
        }
    }

    public function paymentCount($data = array()) {
        $result = $this->REPLICATION_DB
            ->select(array('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") AS DATA_DATE', 'PRODUCT_ID', 'COUNT(USER_ID) AS DATA_VALUE'))
            ->from('market_order')
            ->where('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") BETWEEN "' . $data['START_DATE'] . '" AND "' . $data['END_DATE'] . '"', null, false)
            ->group_by(array('DATA_DATE', 'PRODUCT_ID'))
            ->get();

        foreach ($result->result_array() as $row) {
            $this->updateStatQuery($row['DATA_DATE'], '201:' . $row['PRODUCT_ID'], $row['DATA_VALUE']);
        }
    }

    public function paymentUserCount($data = array()) {
        $result = $this->REPLICATION_DB
            ->select(array('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") AS DATA_DATE', 'PRODUCT_ID', 'COUNT(DISTINCT USER_ID) AS DATA_VALUE'))
            ->from('market_order')
            ->where('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") BETWEEN "' . $data['START_DATE'] . '" AND "' . $data['END_DATE'] . '"', null, false)
            ->group_by(array('DATA_DATE', 'PRODUCT_ID'))
            ->get();

        foreach ($result->result_array() as $row) {
            $this->updateStatQuery($row['DATA_DATE'], '202:' . $row['PRODUCT_ID'], $row['DATA_VALUE']);
        }
    }

    public function pointCount($data = array()) {
        $result = $this->LOG_DB
            ->select(array('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") AS DATA_DATE', 'POINT_TYPE', 'CATEGORY', 'COUNT(USER_ID) AS POINT_COUNT', 'SUM(POINT) AS POINT_SUM'))
            ->from('log_point_' . date('Ym', strtotime($data['START_DATE'])))
            ->where('DATE_FORMAT(`CREATE_DATE`, "%Y-%m-%d") BETWEEN "' . $data['START_DATE'] . '" AND "' . $data['END_DATE'] . '"', null, false)
            ->group_by(array('DATA_DATE', 'POINT_TYPE', 'CATEGORY'))
            ->get();

        foreach ($result->result_array() as $row) {
            $this->updateStatQuery($row['DATA_DATE'], '301:' . $row['POINT_TYPE'] . ':' . $row['CATEGORY'], $row['POINT_COUNT']);
            $this->updateStatQuery($row['DATA_DATE'], '302:' . $row['POINT_TYPE'] . ':' . $row['CATEGORY'], $row['POINT_SUM']);
        }
    }

    public function gameStage($data = array()) {
        $stages = array();
        for ($i =0; $i < 10; $i++) {
            $result = $this->REPLICATION_DB
                ->select(array('STAGE_ID', 'COUNT(USER_ID) AS DATA_VALUE'))
                ->from('users_stages_' . $i)
                ->where('USER_ID >', 0)
                ->group_by('STAGE_ID')
                ->get();

            foreach ($result->result_array() as $row) {
                if (isset($stages[$row['STAGE_ID']])) {
                    $stages[$row['STAGE_ID']] += $row['DATA_VALUE'];
                } else {
                    $stages[$row['STAGE_ID']] = $row['DATA_VALUE'];
                }
            }
        }

        foreach ($stages as $stage_id => $value) {
            $this->updateStatQuery(date('Y-m-d', strtotime($data['START_DATE'])), '401:' . $stage_id, $value);
        }
    }

    public function gameCharacter($data = array()) {
        $result = $this->REPLICATION_DB
            ->select(array('CHARACTER_INFO_ID', 'LEVEL', 'COUNT(USER_ID) AS DATA_VALUE'))
            ->from('users_characters')
            ->where('USER_ID >', 0)
            ->group_by(array('CHARACTER_INFO_ID', 'LEVEL'))
            ->get();

        foreach ($result->result_array() as $row) {
            $this->updateStatQuery(date('Y-m-d', strtotime($data['START_DATE'])), '402:' . $row['CHARACTER_INFO_ID'] . ':' . $row['LEVEL'], $row['DATA_VALUE']);
        }
    }

    public function gameSkill($data = array()) {
        $result = $this->REPLICATION_DB
            ->select(array('SKILL_INFO_ID', 'LEVEL', 'COUNT(USER_ID) AS DATA_VALUE'))
            ->from('users_skills')
            ->where('USER_ID >', 0)
            ->group_by(array('SKILL_INFO_ID', 'LEVEL'))
            ->get();

        foreach ($result->result_array() as $row) {
            $this->updateStatQuery(date('Y-m-d', strtotime($data['START_DATE'])), '403:' . $row['SKILL_INFO_ID'] . ':' . $row['LEVEL'], $row['DATA_VALUE']);
        }
    }

    public function gameTreasure($data = array()) {
        $result = $this->REPLICATION_DB
            ->select(array('TREASURE_INFO_ID', 'LEVEL', 'COUNT(USER_ID) AS DATA_VALUE'))
            ->from('users_treasures')
            ->where('USER_ID >', 0)
            ->where('IS_DELETE', 0)
            ->group_by(array('TREASURE_INFO_ID', 'LEVEL'))
            ->get();

        foreach ($result->result_array() as $row) {
            $this->updateStatQuery(date('Y-m-d', strtotime($data['START_DATE'])), '404:' . $row['TREASURE_INFO_ID'] . ':' . $row['LEVEL'], $row['DATA_VALUE']);
        }
    }

    public function gamePlayCount($data = array()) {
        $result = $this->LOG_DB
            ->select(array('DATE_FORMAT(`START_DATE`, "%Y-%m-%d") AS DATA_DATE', 'STAGE_ID', 'COUNT(USER_ID) AS DATA_VALUE'))
            ->from('log_game_start_' . date('Ym', strtotime($data['START_DATE'])))
            ->where('DATE_FORMAT(`START_DATE`, "%Y-%m-%d") BETWEEN "' . $data['START_DATE'] . '" AND "' . $data['END_DATE'] . '"', null, false)
            ->group_by(array('DATA_DATE', 'STAGE_ID'))
            ->get();

        foreach ($result->result_array() as $row) {
            $this->updateStatQuery($row['DATA_DATE'], '405:' . $row['STAGE_ID'], $row['DATA_VALUE']);
        }
    }

    private function updateStatQuery($date, $key, $value = 0) {
        $sql = 'INSERT INTO `op_stat_data`
                (`DATA_DATE`, `DATA_KEY`, `DATA_VALUE`)
                VALUES
                ("' . $date . '", "' . $key . '", ' . $value . ')
                ON DUPLICATE KEY
                UPDATE `DATA_VALUE` = ' . $value;
        return $this->LOG_DB->query($sql);
    }
}
