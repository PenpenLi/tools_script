<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class Game_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->load->database();
    }

    public function __destruct() {
        $this->db->close();
    }

    public function _selectUserLevelInfo($data = array()) {
        $fields = array(
            'LEVEL',
            'MAX_EXP_POINT',
            'REWARD'
        );
        $wheres = array();

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('game_user_level')
            ->where($wheres)
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            $row['REWARD'] = json_decode($row['REWARD'], true);

            $return[$row['LEVEL']] = $row;
        }

        return $return;
    }

    public function selectUserLevelInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));

        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-user_level'.$version;

        if (!$return = $this->cache->get($cacheName)) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('newdocs');

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['user_level']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_user_level'.$version, $contents);
            $docsRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_user_level'.$version);

            $return = array();
            foreach ($docsRows as $row) {
                $return[$row['level']] = array(
                    'LEVEL' => $row['level'],
                    'MAX_EXP_POINT' => $row['nextExp'],
                    'SCORE_BONUS_RATE' => $row['scoreBonusRate'],
                    'REWARD' => array(
                        'MONEY' => $row['rewardValueMoney'],
                        'CASH' => $row['rewardValueCash'],
                    ),
                );
            }

            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }

    public function _selectStageInfo($data = array()) {
        $fields = array(
            'STAGE_ID',
            'GAME_MODE',
            'REWARD'
        );
        $wheres = array(
            'STAGE_ID' => $data['STAGE_ID']
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('game_stage')
            ->where($wheres)
            ->get();

        $return = $result->row_array();
        if (!empty($return['REWARD'])) {
            $return['REWARD'] = json_decode($return['REWARD'], true);
        }

        return $return;
    }

    public function selectStageInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));

        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-stage'.$version;

        if (!$return = $this->cache->get($cacheName)) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('newdocs');

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['stage']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_stage'.$version, $contents);
            $docsRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_stage'.$version);

            $return = array();
            foreach ($docsRows as $row) {
                $grade_reward = array();
                for ($i = 1; $i <= 3; $i++) {
                    if (strtolower($row['gradeRewardType' . $i]) == 'exp') {
                        $grade_reward[$i] = array('EXP_POINT' => $row['gradeRewardValue' . $i]);
                    } else if (strtolower($row['gradeRewardType' . $i]) == 'stamina') {
                        $grade_reward[$i] = array('HEART' => $row['gradeRewardValue' . $i]);
                    } else if (strtolower($row['gradeRewardType' . $i]) == 'gold') {
                        $grade_reward[$i] = array('MONEY' => $row['gradeRewardValue' . $i]);
                    } else if (strtolower($row['gradeRewardType' . $i]) == 'cash') {
                        $grade_reward[$i] = array('CASH' => $row['gradeRewardValue' . $i]);
                    } else if (strtolower($row['gradeRewardType' . $i]) == 'lotterypoint') {
                        $grade_reward[$i] = array('LOTTERY_POINT' => $row['gradeRewardValue' . $i]);
                    } else if (strtolower($row['gradeRewardType' . $i]) == 'highticket') {
                        $grade_reward[$i] = array('LOTTERY_HIGH_COUPON' => $row['gradeRewardValue' . $i]);
                    } else if (strtolower($row['gradeRewardType' . $i]) == 'ticket') {
                        $grade_reward[$i] = array('LOTTERY_COUPON' => $row['gradeRewardValue' . $i]);
                    } else if (strtolower($row['gradeRewardType' . $i]) == 'item') {
                        $grade_reward[$i] = array('ITEM' => $row['gradeRewardValue' . $i]);
                    } else if (strpos(strtolower($row['gradeRewardType' . $i]), 'booster') === 0) {
                        $grade_reward[$i] = array(strtoupper($row['gradeRewardType' . $i]) => $row['gradeRewardValue' . $i]);
                    }
                }

                $return[$row['index']] = array(
                    'STAGE_ID' => $row['index'],
                    'GAME_MODE' => $row['gameMode'],
                    'STAMINA' => $row['stamina'],
                    'REWARD' => array(
                        'MONEY' => $row['rewardValue1'],
                        'EXP_POINT' => $row['rewardValue2'],
                    ),
                    'GRADE_REWARD' => $grade_reward,
                    'EST_EXP' => $row['estimatedExp'],
                    'EST_SILVER' => $row['estimatedSilver'],
                    'EST_SCORE' => $row['estimatedScore'],

                );
            }

            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }

    public function selectStageInfoCount($data = array()) {
        return $this->db->count_all('game_stage');
    }

    public function selectMissionInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));

        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-mission'.$version;

        if (!$return = $this->cache->get($cacheName)) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('newdocs');

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['mission']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_mission'.$version, $contents);
            $docsRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_mission'.$version);

            $return = array();
            foreach ($docsRows as $row) {
                if ($row['rewardType'] == 'Money') $reward_type = 0;
                else if ($row['rewardType'] == 'Cash') $reward_type = 1;
                else if ($row['rewardType'] == 'Stamina') $reward_type = 2;
                else if ($row['rewardType'] == 'LotteryPoint') $reward_type = 6;

                $return[$row['index']] = array(
                    'MISSION_INFO_ID' => $row['index'],
                    'CYCLE_TYPE' => $row['takeType'],
                    'TYPE1' => $row['missionType1'],
                    'TYPE2' => $row['missionType2'],
                    'REWARD_TYPE' => $reward_type,
                    'REWARD_AMOUNT' => $row['rewardValue1'],
                );
            }

            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }

    public function _selectMissionInfo($data = array()) {
        $fields = array(
            'MISSION_INFO_ID',
            'MISSION_TYPE',
            'MISSION_MODE',
            'CYCLE_TYPE',
            'REWARD'
        );
        $wheres = array(
            'MISSION_INFO_ID' => $data['MISSION_INFO_ID']
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('game_mission')
            ->where($wheres)
            ->get();

        $return = $result->row_array();
        if (!empty($return['REWARD'])) {
            $return['REWARD'] = json_decode($return['REWARD'], true);
        }

        return $return;
    }

    public function selectUserMission($data = array()) {
        $fields = array(
            'MISSION_ID',
            'MISSION_INFO_ID',
            'IS_CLEAR',
            'UPDATE_DATE',
        );
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
        );

        if (isset($data['MISSION_ID'])) {
            $wheres['MISSION_ID'] = $data['MISSION_ID'];
        }

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_missions_' . LOG_TABLE_DATE)
            ->where($wheres)
            ->get();
        $return = $result->row_array();

        return $return;
    }

    public function selectUserMissions($data = array(), $return_type = 'json') {
        $fields = array(
            'MISSION_ID',
            'MISSION_INFO_ID',
            'IS_CLEAR',
            'UPDATE_DATE',
        );
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_missions_' . LOG_TABLE_DATE)
            ->where($wheres)
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            if ($return_type == 'json') {
                $return[] = array(
                    'm' => $row['MISSION_ID'],
                    'i' => $row['MISSION_INFO_ID'],
                    'c' => $row['IS_CLEAR']
                );
            } else {
                $return[] = $row;
            }
        }

        return $return;
    }

    public function insertRandomMission($data = array()) {
        $missionInfoRows = $this->selectMissionInfo($data);

        $mission_type = array();
        foreach ($missionInfoRows as $row) {
            if ($row['TYPE2'] == 'Attendance') continue;
            $mission_type[$row['TYPE2']][] = $row['MISSION_INFO_ID'];
        }

        foreach ($mission_type as $r) {
            $mission_info_id = $r[array_rand($r)];

            $insert = array(
                'MISSION_INFO_ID' => $mission_info_id,
                'USER_ID' => $data['USER_ID'],
                'IS_CLEAR' => 0,
            );

            $this->db->insert('users_missions_' . LOG_TABLE_DATE, $insert);
        }
    }

    public function updateUserMission($data = array()) {
        $update = array(
            'IS_CLEAR' => 1,
            'UPDATE_DATE' => date('Y-m-d H:i:s')
        );

        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'MISSION_ID' => $data['MISSION_ID'],
            'IS_CLEAR' => 0,
        );

        $this->db
            ->where($wheres)
            ->update('users_missions_' . LOG_TABLE_DATE, $update);

        if ($this->db->affected_rows() > 0) {
            return true;
        } else {
            return false;
        }
    }

    public function selectUserStages($data = array(), $return_type = 'json') {
        $fields = array(
            'STAGE_ID',
            'USER_ID',
            'CLEAR_TYPE',
            'BEST_SCORE',
            'CLEAR_COUNT',
            'CREATE_DATE',
            'UPDATE_DATE',
            'PERFECT',
        );
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_stages_' . substr($data['USER_ID'], -1))
            ->where($wheres)
            ->get();
        $stageInfoRows = $this->selectStageInfo($data);
        $return = array();
        foreach ($result->result_array() as $row) {
            if ($return_type == 'json') {
                $return[] = array(
                    'i' => $row['STAGE_ID'],
                    'c' => $row['CLEAR_TYPE'],
                    'cc' => $row['CLEAR_COUNT'],
                    's' => $row['BEST_SCORE'],
                    'p' => $row['PERFECT'],
                );
            } else {
                $stage_info = $stageInfoRows[$row['STAGE_ID']];
                $row['NAME'] = $stage_info['GAME_MODE'] . ' (' . $row['STAGE_ID'] . ')';

                $return[] = $row;
            }
        }

        return $return;
    }

    public function selectUserStage($data = array()) {
        $fields = array(
            'STAGE_ID',
            'USER_ID',
            'CLEAR_TYPE',
            'BEST_SCORE',
            'CLEAR_COUNT',
            'CREATE_DATE',
            'UPDATE_DATE',
        );
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'STAGE_ID' => $data['STAGE_ID']
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_stages_' . substr($data['USER_ID'], -1))
            ->where($wheres)
            ->get();
        $return = $result->row_array();

        return $return;
    }

    public function insertUserStage($data = array()) {
        $sql = 'INSERT INTO `op_users_stages_' . substr($data['USER_ID'], -1) . '`
                (`STAGE_ID`, `USER_ID`, `CLEAR_TYPE`, `BEST_SCORE`, `CLEAR_COUNT`, `CREATE_DATE`, `PERFECT`)
                VALUES
                (' . $data['STAGE_ID'] . ', ' . $data['USER_ID'] . ', ' . $data['CLEAR_TYPE'] . ', ' . $data['SCORE'] . ', '.$data['CLEAR_COUNT'].', "' . date('Y-m-d H:i:s') .'", '. $data['PERFECT'] .')
                ON DUPLICATE KEY
                UPDATE `CLEAR_TYPE` = IF(`CLEAR_TYPE` > ' . $data['CLEAR_TYPE'] . ', `CLEAR_TYPE`, ' . $data['CLEAR_TYPE'] . '), `BEST_SCORE` = IF(`BEST_SCORE` > ' . $data['SCORE'] . ', `BEST_SCORE`, ' . $data['SCORE'] . '), `CLEAR_COUNT` = `CLEAR_COUNT` + '.$data['CLEAR_COUNT'].', `UPDATE_DATE` = "' . date('Y-m-d H:i:s') . '", `PERFECT` = IF(`PERFECT` > ' . $data['PERFECT'] . ', `PERFECT`, ' . $data['PERFECT'] . ')';
        $this->db->query($sql);
    }

    public function _insertUserStage($data = array()) {
        $insert = array(
            'STAGE_ID' => $data['STAGE_ID'],
            'USER_ID' => $data['USER_ID'],
            'CLEAR_TYPE' => $data['CLEAR_TYPE'],
            'BEST_SCORE' => $data['SCORE'],
            'CLEAR_COUNT' => 1,
            'CREATE_DATE' => date('Y-m-d H:i:s'),
            'PERFECT' => $data['PERFECT']
        );

        $this->db->insert('users_stages_' . substr($data['USER_ID'], -1), $insert);

        if ($this->db->affected_rows() > 0) {
            return $this->db->insert_id();
        } else {
            return -1;
        }
    }

    public function updateUserStage($data = array()) {
        $update = array(
            'UPDATE_DATE' => date('Y-m-d H:i:s')
        );

        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'STAGE_ID' => $data['STAGE_ID']
        );

        $this->db
            ->set('CLEAR_TYPE', 'IF(CLEAR_TYPE > ' . $data['CLEAR_TYPE'] . ', CLEAR_TYPE, ' . $data['CLEAR_TYPE'] . ')', false)
            ->set('BEST_SCORE', 'IF(BEST_SCORE > ' . $data['SCORE'] . ', BEST_SCORE, ' . $data['SCORE'] . ')', false)
            ->set('CLEAR_COUNT', 'CLEAR_COUNT + '.$data['CLEAR_COUNT'], false)
            ->set('PERFECT', 'IF(PERFECT > ' . $data['PERFECT'] . ', PERFECT, ' . $data['PERFECT'] . ')', false)
            ->where($wheres)
            ->update('users_stages_' . substr($data['USER_ID'], -1), $update);
    }

    public function selectPlayCode($data = array()) {
        $fields = array(
            'USER_ID',
            'STAGE_ID',
            'FRIEND_USER_ID',
            'CONTINUE_COUNT',
            'IS_FINISH',
        );
        $wheres = array(
            'PLAY_CODE' => $data['PLAY_CODE']
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('play_code_' . LOG_TABLE_DATE)
            ->where($wheres)
            ->get();
        $return = $result->row_array();

        if (!isset($return['USER_ID'])) {
            $result = $this->db
                ->select(implode(', ', $fields))
                ->from('play_code_' . LOG_TABLE_DATE_YESTERDAY)
                ->where($wheres)
                ->get();
            $return = $result->row_array();
        }

        return $return;
    }

    public function insertPlayCode($data = array()) {
        $insert = array(
            'USER_ID' => $data['USER_ID'],
            'STAGE_ID' => $data['STAGE_ID'],
            'FRIEND_USER_ID' => $data['FRIEND_USER_ID'],
            'IS_FINISH' => 0,
            'CREATE_DATE' => date('Y-m-d H:i:s')
        );

        $this->db->insert('play_code_' . LOG_TABLE_DATE, $insert);

        if ($this->db->affected_rows() > 0) {
            return $this->db->insert_id();
        } else {
            return -1;
        }
    }

    public function updatePlayCode($data = array()) {
        $update = array();

        if (isset($data['IS_FINISH'])) {
            $update['IS_FINISH'] = 1;
        }

        if (isset($data['CONTINUE_COUNT']) && is_numeric($data['CONTINUE_COUNT'])) {
            $update['CONTINUE_COUNT'] = $data['CONTINUE_COUNT'];
        }

        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'PLAY_CODE' => $data['PLAY_CODE'],
        );

        if ($this->db->from('play_code_' . LOG_TABLE_DATE)->where($wheres)->count_all_results() > 0) {
            $this->db
                ->where($wheres)
                ->update('play_code_' . LOG_TABLE_DATE, $update);
        } else {
            $this->db
                ->where($wheres)
                ->update('play_code_' . LOG_TABLE_DATE_YESTERDAY, $update);
        }
    }

    public function selectAchievementInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));

        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-achievement'.$version;

        if (!$return = $this->cache->get($cacheName)) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('newdocs');

            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['achievement']);
            $contents = $this->curl->execute();
            write_file('/tmp/docs_temp_file'.$version, $contents);
            $docsRows = $this->docs_reader->parse_file('/tmp/docs_temp_file'.$version);

            $return = array();
            foreach ($docsRows as $row) {
                $return[$row['index']] = array(
                    'ACHIEVEMENT_INFO_ID' => $row['index'],
                    'NAME' => $row['name'],
                    'TYPE' => $row['type'],
                    'REWARD_TYPE' => 1,
                    'REWARD_AMOUNT' => $row['reward'],
                    'MAX_COUNT' => $row['step'],
                    'UNLOCK_CASH' => $row['unlockcash'],
                );
            }

            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }

    public function selectUserAchievement($data = array()) {
        $fields = array(
            'ACHIEVEMENT_INFO_ID',
            'PROGRESS',
            'REWARD_OK',
            'CREATE_DATE',
        );
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'ACHIEVEMENT_INFO_ID' => $data['ACHIEVEMENT_INFO_ID'],
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_achievements_' . substr($data['USER_ID'], -1))
            ->where($wheres)
            ->get();
        $return = $result->row_array();

        return $return;
    }

    public function selectUserAchievements($data = array(), $return_type = 'json') {
        $fields = array(
            'ACHIEVEMENT_INFO_ID',
            'USER_ID',
            'PROGRESS',
            'REWARD_OK',
            'CREATE_DATE',
        );
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_achievements_' . substr($data['USER_ID'], -1))
            ->where($wheres)
            ->get();
        $achievementInfoRows = $this->selectAchievementInfo($data);
        $return = array();
        foreach ($result->result_array() as $row) {
            if ($return_type == 'json') {
                $return[] = array(
                    'a' => $row['ACHIEVEMENT_INFO_ID'],
                    'p' => $row['PROGRESS'],
                    'r' => $row['REWARD_OK']
                );
            } else {
                $achievement_info = $achievementInfoRows[$row['ACHIEVEMENT_INFO_ID']];
                $row['NAME'] = $achievement_info['NAME'];
                $row['MAX_COUNT'] = $achievement_info['MAX_COUNT'];

                $return[] = $row;
            }
        }

        return $return;
    }

    public function unlockUserAchievement($data = array()) {
        $achieveInfoRows = $this->selectAchievementInfo($data);
        $achieve_info = $achieveInfoRows[$data['ACHIEVEMENT_INFO_ID']];

//        $wheres = array(
//            'USER_ID' => $data['USER_ID'],
//            'ACHIEVEMENT_INFO_ID' => $data['ACHIEVEMENT_INFO_ID'],
//        );
//
//
//        $insert = array(
//            'ACHIEVEMENT_INFO_ID' => $data['ACHIEVEMENT_INFO_ID'],
//            'USER_ID' => $data['USER_ID'],
//            'PROGRESS' => 0,
//            'REWARD_OK' => 1,
//            'CREATE_DATE' => date('Y-m-d H:i:s'),
//        );
//
//        $this->db->insert('users_achievements_' . substr($data['USER_ID'], -1), $insert);

        $sql = 'INSERT INTO `op_users_achievements_' . substr($data['USER_ID'], -1) . '`
                    (`ACHIEVEMENT_INFO_ID`, `USER_ID`, `PROGRESS`, `REWARD_OK`, `CREATE_DATE`)
                    VALUES
                    (' . $data['ACHIEVEMENT_INFO_ID'] . ', ' . $data['USER_ID'] . ', ' . 0 . ', 1, "' . date('Y-m-d H:i:s') . '")
                    ON DUPLICATE KEY
                    UPDATE `REWARD_OK` = ' . 1;
        $this->db->query($sql);

        if ($this->db->affected_rows() > 0) {
            return true;
        } else {
            return false;
        }
    }

    public function updateUserAchievement($data = array()) {
        $achieveInfoRows = $this->selectAchievementInfo($data);
        $achieve_info = $achieveInfoRows[$data['ACHIEVEMENT_INFO_ID']];

        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'ACHIEVEMENT_INFO_ID' => $data['ACHIEVEMENT_INFO_ID'],
        );

        if ($this->db->from('users_achievements_' . substr($data['USER_ID'], -1))->where($wheres)->count_all_results() > 0) {
            if (isset($data['PROGRESS'])) {
                if ($data['PROGRESS'] > $achieve_info['MAX_COUNT']) $data['PROGRESS'] = $achieve_info['MAX_COUNT'];
                $this->db->set('PROGRESS', $data['PROGRESS']);
            }

            if (isset($data['PROGRESS_PLUS'])) {
                $this->db->set('PROGRESS', 'IF((PROGRESS + ' . $data['PROGRESS_PLUS'] . ') < ' . $achieve_info['MAX_COUNT'] . ', PROGRESS + ' . $data['PROGRESS_PLUS'] . ', ' . $achieve_info['MAX_COUNT'] . ')', false);
            }

            $this->db->where($wheres)->update('users_achievements_' . substr($data['USER_ID'], -1));
        } else {
            $insert = array(
                'ACHIEVEMENT_INFO_ID' => $data['ACHIEVEMENT_INFO_ID'],
                'USER_ID' => $data['USER_ID'],
                'PROGRESS' => 0,
                'REWARD_OK' => 0,
                'CREATE_DATE' => date('Y-m-d H:i:s'),
            );

            if (isset($data['PROGRESS'])) {
                if ($data['PROGRESS'] > $achieve_info['MAX_COUNT']) $data['PROGRESS'] = $achieve_info['MAX_COUNT'];
                $this->db->set('PROGRESS', $data['PROGRESS']);
                unset($insert['PROGRESS']);
            }

            if (isset($data['PROGRESS_PLUS'])) {
                if ($data['PROGRESS_PLUS'] > $achieve_info['MAX_COUNT']) $data['PROGRESS_PLUS'] = $achieve_info['MAX_COUNT'];
                $this->db->set('PROGRESS', $data['PROGRESS_PLUS']);
                unset($insert['PROGRESS']);
            }

            $this->db->insert('users_achievements_' . substr($data['USER_ID'], -1), $insert);

            if ($this->db->affected_rows() > 0) {
                return true;
            } else {
                return false;
            }
        }
    }



    public function updateUserAchievements($data = array()) {
        $types = array();
        foreach ($data['PROGRESS'] as $row) {
            $types[$row['t']] = $row['c'];
        }

        $achieve_progress = array();
        $achieveInfoRows = $this->selectAchievementInfo($data);
        foreach ($achieveInfoRows as $row) {
            if (isset($types[$row['TYPE']])) {
                $achieve_progress[$row['ACHIEVEMENT_INFO_ID']] = ($row['MAX_COUNT'] > $types[$row['TYPE']]) ? $types[$row['TYPE']] : $row['MAX_COUNT'];
            }
        }

        foreach ($achieve_progress as $achieve_info_id => $count) {
            $sql = 'INSERT INTO `op_users_achievements_' . substr($data['USER_ID'], -1) . '`
                    (`ACHIEVEMENT_INFO_ID`, `USER_ID`, `PROGRESS`, `REWARD_OK`, `CREATE_DATE`)
                    VALUES
                    (' . $achieve_info_id . ', ' . $data['USER_ID'] . ', ' . $count . ', 0, "' . date('Y-m-d H:i:s') . '")
                    ON DUPLICATE KEY
                    UPDATE `PROGRESS` = ' . $count;
            $this->db->query($sql);
        }
    }

    public function _updateUserAchievements($data = array()) {
        $types = array();
        foreach ($data['PROGRESS'] as $row) {
            $types[$row['type']] = $row['count'];
        }

        $achieve_ids = array();
        $achieve_progress = array();
        $achieveInfoRows = $this->selectAchievementInfo($data);
        foreach ($achieveInfoRows as $row) {
            if (isset($types[$row['TYPE']])) {
                $achieve_progress[$row['ACHIEVEMENT_INFO_ID']] = ($row['MAX_COUNT'] > $types[$row['TYPE']]) ? $types[$row['TYPE']] : $row['MAX_COUNT'];
                $achieve_ids[] = $row['ACHIEVEMENT_INFO_ID'];
            }
        }

        $result = $this->db
            ->select('ACHIEVEMENT_INFO_ID')
            ->from('users_achievements_' . substr($data['USER_ID'], -1))
            ->where('USER_ID', $data['USER_ID'])
            ->where_in('ACHIEVEMENT_INFO_ID', $achieve_ids)
            ->get();

        $db_achieve_ids = array();
        foreach ($result->result_array() as $row) {
            $db_achieve_ids[] = $row['ACHIEVEMENT_INFO_ID'];
        }

        foreach ($achieve_ids as $achieve_id) {
            if (in_array($achieve_id, $db_achieve_ids)) {
                $this->db
                    ->set('PROGRESS', $achieve_progress[$achieve_id])
                    ->where('USER_ID', $data['USER_ID'])
                    ->where('ACHIEVEMENT_INFO_ID', $achieve_id)
                    ->update('users_achievements_' . substr($data['USER_ID'], -1));
            } else {
                $insert = array(
                    'ACHIEVEMENT_INFO_ID' => $achieve_id,
                    'USER_ID' => $data['USER_ID'],
                    'PROGRESS' => $achieve_progress[$achieve_id],
                    'REWARD_OK' => 0,
                    'CREATE_DATE' => date('Y-m-d H:i:s'),
                );

                $this->db->insert('users_achievements_' . substr($data['USER_ID'], -1), $insert);
            }
        }
    }

    public function updateUserAchievementReward($data = array()) {
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'ACHIEVEMENT_INFO_ID' => $data['ACHIEVEMENT_INFO_ID'],
        );

        $this->db->set('REWARD_OK', 1);
        $this->db->where($wheres)->update('users_achievements_' . substr($data['USER_ID'], -1));
    }

    public function updateHeart($data = array()) {
        if (MAX_USER_HEART <= $data['HEART']) {
            $data['HEART_TIME'] = null;
        }

        $update = array(
            'HEART' => $data['HEART'],
            'HEART_TIME' => $data['HEART_TIME']
        );

        $wheres = array(
            'USER_ID' => $data['USER_ID']
        );

        $this->db
            ->where($wheres)
            ->update('users', $update);
    }

    public function updateVIPEndDate($data = array()) {
        $update = array(
            'VIP_END_DATE' => $data['VIP_END_DATE'],
        );

        $wheres = array(
            'USER_ID' => $data['USER_ID']
        );

        $this->db
            ->where($wheres)
            ->update('users', $update);
    }

    public function updateUserBlock($data = array()) {


        $update = array(
            'STATUS' => 2
        );

        $wheres = array(
            'USER_ID' => $data['USER_ID']
        );

        $this->db
            ->where($wheres)
            ->update('users', $update);
    }

    public function increaseHackCount($data = array()) {
        $wheres = array(
            'USER_ID' => $data['USER_ID']
        );

        $this->db
            ->where($wheres)
            ->set( 'HACK_COUNT', 'HACK_COUNT+1', false)
            ->update('users');
    }

    public function selectUserRanking($data = array()) {
        $sql = 'SELECT
                    RANK, U.USER_ID, SCORE, U.LEVEL, CHARACTER_INFO_ID
                FROM (
                    SELECT R.*, (@rownum:=@rownum+1) RANK
                    FROM op_users_ranking_' . LOG_TABLE_DATE . ' R, (SELECT @rownum:=0 FROM DUAL) D, op_users U, op_users_characters C
                    WHERE RANK_TABLE.USER_ID=U.USER_ID
                    AND U.USER_ID=C.USER_ID
                    AND U.CHARACTER_ID=C.CHARACTER_ID
                    ORDER BY R.SCORE DESC, R.USER_ID
                ) RANK_TABLE, op_users U, op_users_characters C
                WHERE RANK_TABLE.USER_ID=U.USER_ID
                    AND U.USER_ID=C.USER_ID
                    AND U.CHARACTER_ID=C.CHARACTER_ID
                    AND (SCORE >= (SELECT SCORE FROM op_users_ranking_' . LOG_TABLE_DATE . ' WHERE USER_ID=' . $this->db->escape($data['USER_ID']) . ') OR RANK <= ' . MAX_DISPLAY_USER_RANKING . ')';
        //$query = $this->db->query($sql);

        $fields = array(
            'U.USER_ID',
            'SCORE',
            'U.LEVEL',
            'U.NICKNAME',
            'C.CHARACTER_ID',
            'C.CHARACTER_INFO_ID',
            'C.LEVEL AS CHARACTER_LEVEL',
        );
        $wheres = array();

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users_ranking_' . LOG_TABLE_DATE . ' R')
            ->join('users U', 'U.USER_ID=R.USER_ID')
            ->join('op_users_characters C', 'U.USER_ID=C.USER_ID AND U.CHARACTER_ID=C.CHARACTER_ID')
            //->where('(SCORE >= (SELECT SCORE FROM op_users_ranking_' . LOG_TABLE_DATE . ' WHERE USER_ID=' . $this->db->escape($data['USER_ID']) . '))', null, false)
            ->order_by('SCORE', 'DESC')
            ->order_by('USER_ID', 'ASC')
            ->get();

        $return = array('rank' => array());
        $rank = 0;
        foreach ($result->result_array() as $row) {
            $rank++;

            if ($rank > MAX_DISPLAY_USER_RANKING && $row['USER_ID'] != $data['USER_ID']) {
                continue;
            } else {
                if ($row['USER_ID'] == $data['USER_ID']) {
                    $return['my'] = array(
                        'rank' => $rank,
                        'userID' => $row['USER_ID'],
                        'score' => $row['SCORE'],
                        //'level' => $row['LEVEL'],
                        //'characterOriginalID' => $row['CHARACTER_INFO_ID'],
                    );
                }

                $return['rank'][] = array(
                    'rank' => $rank,
                    'userID' => $row['USER_ID'],
                    'nickname' => $row['NICKNAME'],
                    'score' => $row['SCORE'],
                    'level' => $row['LEVEL'],
                    'character' => array(
                        'i' => $row['CHARACTER_ID'],
                        'o' => $row['CHARACTER_INFO_ID'],
                        'l' => $row['CHARACTER_LEVEL'],
                    ),
                );
            }
        }

        return $return;
    }

    public function updateUserRanking($data = array()) {
        $insert = array(
            'USER_ID' => $data['USER_ID'],
            'SCORE' => $data['SCORE'],
        );

        $update = array(
            'SCORE' => $data['SCORE'],
        );

        $wheres = array(
            'USER_ID' => $data['USER_ID'],
        );

        if ($this->db->from('users_ranking_' . LOG_TABLE_DATE)->where($wheres)->count_all_results() > 0) {
            $where['SCORE <'] = $data['SCORE'];
            $this->db
                ->where($wheres)
                ->update('users_ranking_' . LOG_TABLE_DATE, $update);
        } else {
            $this->db->insert('users_ranking_' . LOG_TABLE_DATE, $insert);
        }
    }

    public function selectUserDetail($data = array(), $return_type = 'json') {
        $fields = array(
            'U.USER_ID',
            'NICKNAME',
            'U.LEVEL',
            'LOGIN_DATE',
            'C.CHARACTER_ID',
            'C.CHARACTER_INFO_ID',
            'C.LEVEL AS CHARACTER_LEVEL',
            'SCORE',
            'R.GRADE',
            'BEST_SCORE',
        );
        $wheres = array(
            'U.USER_ID' => $data['USER_ID'],
            'U.STATUS' => 1,
        );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('users U')
            ->join('users_characters C', 'C.CHARACTER_ID=U.CHARACTER_ID')
            ->join('leagues_ranking_' . LEAGUE_DATE . ' R', 'R.USER_ID=U.USER_ID', 'LEFT OUTER')
            ->where($wheres)
            ->get();

        $detail = $result->row_array();
        $detail['SKILLS'] = array();
        $detail['TREASURES'] = array();

        $fields = array(
            'SKILL_ID',
            'SKILL_INFO_ID',
            'USER_ID',
            'LEVEL',
            'SLOT_NUMBER',
        );
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'SLOT_NUMBER > ' => 0,
        );

        $skills = $this->db
            ->select(implode(', ', $fields))
            ->from('users_skills')
            ->where($wheres)
            ->get();

        foreach ($skills->result_array() as $row) {
            $detail['SKILLS'][] = $row;
        }

        $fields = array(
            'TREASURE_ID',
            'TREASURE_INFO_ID',
            'USER_ID',
            'LEVEL',
            'SLOT_NUMBER',
        );
        $wheres = array(
            'USER_ID' => $data['USER_ID'],
            'SLOT_NUMBER > ' => 0,
            'IS_DELETE' => 0,
        );

        $treasures = $this->db
            ->select(implode(', ', $fields))
            ->from('users_treasures')
            ->where($wheres)
            ->get();

        foreach ($treasures->result_array() as $row) {
            $detail['TREASURES'][] = $row;
        }

        $return = array();
        if ($return_type == 'json') {
            $skill = array();
            foreach ($detail['SKILLS'] as $s) {
                $skill[] = array(
                    'i' => $s['SKILL_ID'],
                    'o' => $s['SKILL_INFO_ID'],
                    'l' => $s['LEVEL'],
                    's' => $s['SLOT_NUMBER']
                );
            }

            $treasure = array();
            foreach ($detail['TREASURES'] as $s) {
                $treasure[] = array(
                    'i' => $s['TREASURE_ID'],
                    'o' => $s['TREASURE_INFO_ID'],
                    'l' => $s['LEVEL'],
                    's' => $s['SLOT_NUMBER']
                );
            }

            $return = array(
                'nickname' => $detail['NICKNAME'],
                'level' => $detail['LEVEL'],
                'leagueGrade' => (!$detail['GRADE']) ? 10 : $detail['GRADE'],
                'score' => $detail['SCORE'],
                'bestCcore' => $detail['BEST_SCORE'],
                'loginDate' => $detail['LOGIN_DATE'],
                'loginDateTime' => (time() - strtotime($detail['LOGIN_DATE'])),
                'character' => array(
                    'i' => $detail['CHARACTER_ID'],
                    'o' => $detail['CHARACTER_INFO_ID'],
                    'l' => $detail['CHARACTER_LEVEL'],
                ),
                'skills' => $skill,
                'treasures' => $treasure,
            );
        } else {
            $return[] = $row;
        }

        return $return;
    }
}