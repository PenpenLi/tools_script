<?php if (!defined('BASEPATH')) exit('No direct script access allowed');

class League_data extends CI_Model {
    public function __construct() {
        parent::__construct();

        $this->load->database($_SESSION['db_group']);
    }

    public function __destruct() {
        $this->db->close();
    }
    
    public function selectLeagueRewardInfo($data = array()) {
        $this->load->driver('cache', array('adapter' => 'apc', 'backup' => 'file'));

        $version = 'v'.SERVER_CURRENT_VERSION;
        $cacheName = 'ofdp-league_reward'.$version;


        if (!$return = $this->cache->get($cacheName)) {
            $this->load->library(array('Docs_reader', 'Curl'));
            $this->load->helper(array('file'));
            $this->config->load('docs');
            $docs = $this->config->item('newdocs');
            
            $this->curl->create($docs['baseurl'].$version.'/'.$docs['filename']['league_reward']);
            $contents = $this->curl->execute();
            write_file('/tmp/ofdp_docs_temp_file_league_reward'.$version, $contents);
            $docsRows = $this->docs_reader->parse_file('/tmp/ofdp_docs_temp_file_league_reward'.$version);
            
            $return = array();
            $reward_raw = array();
            foreach ($docsRows as $row) {
                for ($i = 1; $i <= 100; $i++) {
                    if ($i == 1) {
                        $reward_raw[$i] = $row['first'];
                    } else if ($i == 2) {
                        $reward_raw[$i] = $row['second'];
                    } else if ($i == 3) {
                        $reward_raw[$i] = $row['third'];
                    } else if ($i <= 10) {
                        $reward_raw[$i] = $row['tenthOver'];
                    } else if ($i <= 50) {
                        $reward_raw[$i] = $row['fiftiethOver'];
                    } else {
                        $reward_raw[$i] = $row['hundredthOver'];
                    }
                }
                
                $reward = array();
                foreach ($reward_raw as $rank => $value) {
                    list($reward_type, $reward_amount) = explode(' ', $value);
                    $item_id = null;
                    
                    if (strtolower($reward_type) == 'money') $type = 0;
                    else if (strtolower($reward_type) == 'cash') $type = 1;
                    else if (strpos(strtolower($reward_type), 'booster') === 0) {
                        $type = 8;
                        $item_id = substr($reward_type, 7);
                    }
                    
                    $reward[$rank] = array(
                            'TYPE' => $type,
                            'ITEM_ID' => $item_id,
                            'AMOUNT' => $reward_amount
                        );
                }
                
                $return[$row['index']] = array(
                        'GRADE' => $row['index'],
                        'REWARD' => $reward,
                    );
            }
            
            $this->cache->save($cacheName, $return, CACHE_REFRESH_TIME);
        }

        return $return;
    }
    
    public function selectUserLeague($data = array()) {
        $fields = array(
                'GRADE',
                'GROUP_ID',
                'SCORE',
                'REWARD_OK',
            );
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('leagues_ranking_' . LEAGUE_DATE)
            ->where($wheres)
            ->get();
        $return = $result->row_array();
        
        return $return;
    }
    
    public function selectUserLeagueLastWeek($data = array()) {
        $fields = array(
                'GRADE',
                'GROUP_ID',
                'SCORE',
                'REWARD_OK',
            );
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('leagues_ranking_' . LEAGUE_LAST_DATE)
            ->where($wheres)
            ->get();
        $return = $result->row_array();
        
        return $return;
    }
    
    public function insertUserLeague($data = array()) {
        $fields = array(
                'USER_ID',
                'GRADE',
                'GROUP_ID',
                'SCORE',
                'REWARD_OK',
            );
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('leagues_ranking_' . LEAGUE_LAST_DATE)
            ->where($wheres)
            ->get();
        $userRankingRow = $result->row_array();
        
        if (empty($userRankingRow['USER_ID'])) { // 지난주 랭킹 없음
            $grade = $userRankingRow['GRADE'] + 2;

            $result = $this->db
                ->select(implode(', ', $fields))
                ->from('leagues_ranking_' . LEAGUE_2WEEKS_DATE)
                ->where($wheres)
                ->get();
            $userRankingRow = $result->row_array();

            if (empty($userRankingRow['USER_ID'])) { // 랭킹 없음
                $grade = 10;
            }
        } else { // 지난주 랭킹 있음
            $fields = array(
                    'COUNT(*) + 1 AS RANK',
                );
            $wheres = array(
                    'GRADE' => $userRankingRow['GRADE'],
                    'GROUP_ID' => $userRankingRow['GROUP_ID'],
                    'SCORE > ' => $userRankingRow['SCORE'],
                );
    
            $result = $this->db
                ->select(implode(', ', $fields))
                ->from('leagues_ranking_' . LEAGUE_LAST_DATE)
                ->where($wheres)
                ->get();
            $rankingRow = $result->row_array();
            $rank = $rankingRow['RANK'];
            
            $grade = $userRankingRow['GRADE'];
            if ($rank < 46) $grade = $grade - 1;
            else if ($rank > 55) $grade = $grade + 1;
        }
        
        if ($grade < 1) $grade = 1;
        else if ($grade > 10) $grade = 10;
        
        $fields = array(
                'GROUP_ID',
                'COUNT(*) AS USER_COUNT',
            );
        $wheres = array(
                'GRADE' => $grade,
                'USER_ID > ' => 0,
            );

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('leagues_ranking_' . LEAGUE_DATE)
            ->where($wheres)
            ->group_by(array('GROUP_ID'))
            ->order_by('GROUP_ID', 'DESC')
            ->get();
        $userCountRow = $result->row_array();

        $insert = array();
        if (empty($userCountRow['GROUP_ID'])) { // 그룹이 하나도 없음
            $group_id = 1;

            $dummy_last_id = $this->User_data->selectLastDummyID($data);
            $dummy_range = range($dummy_last_id, -1);
            $dummy_selected = array_rand($dummy_range, 99);

            foreach ($dummy_selected as $index => $key) {
                $insert[] = array(
                    'USER_ID' => $dummy_range[$key],
                    'GRADE' => $grade,
                    'GROUP_ID' => $group_id,
                    'SCORE' => 0,
                    'REWARD_OK' => 0,
                );
            }
        } else {
            if ($userCountRow['USER_COUNT'] >= 100) { // 기존 그룹이 꽉 찼음
                $group_id = $userCountRow['GROUP_ID'] + 1;

                $this->db
                    ->where('GRADE', $grade)
                    ->where('USER_ID < ', 0)
                    ->set('GROUP_ID', $group_id)
                    ->update('leagues_ranking_' . LEAGUE_DATE);
            } else {
                $group_id = $userCountRow['GROUP_ID'];

                $this->db
                    ->where('GRADE', $grade)
                    ->where('USER_ID < ', 0)
                    ->order_by('SCORE', 'ASC')
                    ->limit(1)
                    ->set('GROUP_ID', 0)
                    ->update('leagues_ranking_' . LEAGUE_DATE);
            }
        }

        $insert[] = array(
                'USER_ID' => $data['USER_ID'],
                'GRADE' => $grade,
                'GROUP_ID' => $group_id,
                'SCORE' => 0,
                'REWARD_OK' => 0,
            );

        $this->db->insert_batch('leagues_ranking_' . LEAGUE_DATE, $insert);
    }
    
    public function selectLeagueRanking($data = array(), $return_type = 'json') {
        $fields = array(
                'U.USER_ID',
                'SCORE',
                'U.LEVEL',
                'U.NICKNAME',
                'C.CHARACTER_ID',
                'C.CHARACTER_INFO_ID',
                'C.LEVEL AS CHARACTER_LEVEL',
            );
        $wheres = array(
                'GRADE' => $data['GRADE'],
                'GROUP_ID' => $data['GROUP_ID'],
                'STATUS' => 1,
            );

        $league_date = isset($data['LEAGUE_DATE']) ? date('Ymd', strtotime($data['LEAGUE_DATE'])) : LEAGUE_DATE;

        $result = $this->db
            ->select(implode(', ', $fields))
            ->from('leagues_ranking_' . $league_date . ' R')
            ->join('users U', 'U.USER_ID=R.USER_ID')
            ->join('users_characters C', 'U.USER_ID=C.USER_ID AND U.CHARACTER_ID=C.CHARACTER_ID')
            ->where($wheres)
            ->order_by('SCORE', 'DESC')
            ->order_by('USER_ID', 'DESC')
            ->limit(100)
            ->get();

        $return = array();
        foreach ($result->result_array() as $row) {
            if ($return_type == 'json') {
                $return[] = array(
                        'userID' => $row['USER_ID'],
                        'nickname' => $row['NICKNAME'],
                        'score' => $row['SCORE'],
                        'level' => $row['LEVEL'],
                        'character' => array(
                                'itemID' => $row['CHARACTER_ID'],
                                'itemOriginalID' => $row['CHARACTER_INFO_ID'],
                                'level' => $row['CHARACTER_LEVEL'],
                            ),
                    );
            } else {
                $return[] = $row;
            }
        }
        
        return $return;
    }
    
    public function updateUserLeagueRanking($data = array()) {
        $update = array();
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
            );

        if (isset($data['SCORE'])) {
            $update['SCORE'] = $data['SCORE'];
            $wheres['SCORE < '] = $data['SCORE'];
        }

        $this->db
            ->where($wheres)
            ->update('leagues_ranking_' . LEAGUE_DATE, $update);
    }
    
    public function updateUserLeagueLastWeekRanking($data = array()) {
        $update = array();
        $wheres = array(
                'USER_ID' => $data['USER_ID'],
            );

        if (!empty($data['REWARD_OK'])) {
            $update['REWARD_OK'] = 1;
        }
        
        $this->db
            ->where($wheres)
            ->update('leagues_ranking_' . LEAGUE_LAST_DATE, $update);
    }
}