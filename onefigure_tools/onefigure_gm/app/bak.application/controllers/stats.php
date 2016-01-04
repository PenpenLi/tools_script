<?php if ( ! defined('BASEPATH')) exit('No direct script access allowed');

class Stats extends CI_Controller {
    public function __construct() {
        parent::__construct();

        $this->load->model(array('Stat_data'));
        $this->load->helper(array('json'));
    }

    public function user() {
        $data = array();

        $this->viewer->all('stats/user', $data);
    }

    public function ajaxSelectUserStat() {
        $data = array(
            'DATA_KEY' => 101,
            'START_DATE' => $this->input->post('start_date'),
            'END_DATE' => $this->input->post('end_date'),
        );
        $userCountRows = $this->Stat_data->selectStat($data);

        $data['DATA_KEY'] = 102;
        $joinCountRows = $this->Stat_data->selectStat($data);

        $data['DATA_KEY'] = 103;
        $loginCountRows = $this->Stat_data->selectStat($data);

        $total_count = count($userCountRows);

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        foreach ($userCountRows as $index => $row) {
            $arr = array(
                'id' => $row['DATA_DATE'],
                'cell' => array(
                    $row['DATA_DATE'],
                    $row['DATA_VALUE'],
                    $joinCountRows[$row['DATA_DATE']]['DATA_VALUE'],
                    $loginCountRows[$row['DATA_DATE']]['DATA_VALUE']
                )
            );

            $response['rows'][] = $arr;
        }

        json_output($response, true);
    }

    public function payment() {
        $this->load->model(array('Shop_data'));

        $shopItemInfoRows = $this->Shop_data->selectShopItemInfo();

        $data = array(
            'SHOP_ITEM_ARRAY' => $shopItemInfoRows
        );

        $this->viewer->all('stats/payment', $data);
    }

    public function ajaxSelectPaymentStat() {
        $this->load->model(array('Shop_data'));

        $shopItemInfoRows = $this->Shop_data->selectShopItemInfo();

        $data = array(
            'DATA_KEY' => 201,
            'START_DATE' => $this->input->post('start_date'),
            'END_DATE' => $this->input->post('end_date'),
        );
        $paymentCountRows = $this->Stat_data->selectStat($data);

        $data['DATA_KEY'] = 202;
        $paymentUserCountRows = $this->Stat_data->selectStat($data);

        $total_count = count($paymentCountRows);

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        $rows = array();
        foreach ($paymentCountRows as $date => $_rows1) {
            $rows[$date]['TOTAL_COUNT'] = 0;
            $rows[$date]['TOTAL_PRICE'] = 0;

            foreach ($_rows1 as $product_id => $row) {
                $rows[$date]['VALUE_' . $product_id . '_COUNT'] = $row['DATA_VALUE'];
                $rows[$date]['VALUE_' . $product_id . '_PRICE'] = $shopItemInfoRows[$product_id]['PRICE_US'] * $row['DATA_VALUE'];

                $rows[$date]['TOTAL_COUNT'] += $row['DATA_VALUE'];
                $rows[$date]['TOTAL_PRICE'] += $shopItemInfoRows[$product_id]['PRICE_US'] * $row['DATA_VALUE'];
            }
        }

        foreach ($paymentUserCountRows as $date => $_rows1) {
            $rows[$date]['TOTAL_USERS'] = 0;

            foreach ($_rows1 as $product_id => $row) {
                $rows[$date]['VALUE_' . $product_id . '_USERS'] = $row['DATA_VALUE'];

                $rows[$date]['TOTAL_USERS'] += $row['DATA_VALUE'];
            }
        }

        foreach ($rows as $date => $row) {
            $row['DATA_DATE'] = $date;
            $response['rows'][] = $row;
        }

        json_output($response, true);
    }

    public function point() {
        $category_name = array(
            #1 => '보석 결제',
            1 => '금화 구매',
            2 => '결제 프로모션',
            3 => '은화 구입',
            #4 => '이벤트',
            4 => '튜토리얼',
            5 => '게임 보상',
            6 => '랭킹 보상',
            #7 => '튜토리얼',
            7 => '리뷰',
            #8 => '리뷰 보상',
            8 => '메시지함',
            9 => '초대 보상',
            10 => '일간 미션 완료',
            11 => '주간 미션 완료',
            12 => '보물 판매',
            13 => '뽑기',
            14 => '레벨업 보상',
            15 => '스테이지 등급 보상',
            16 => '출석 보상',
            17 => '패키지 구입',
            #18 => '캐릭터 구입 대체',
            18 => '업적보상',
            99 => '관리자 지급',
            -1 => '국수 구입',
            -2 => '은화 구입',
            -3 => '아이템 구입',
            -4 => '캐릭터 구입',
            -5 => '캐릭터 강화',
            -6 => '비법서 구입',
            -7 => '비법서 강화',
            -8 => '보물 구입',
            -9 => '보물 강화',
            -10 => '보물함 확장',
            -11 => '슬롯 구입',
            -12 => '뽑기',
            -13 => '이어하기',
            -14 => '패키지 구입',
            -99 => '관리자 차감',
        );

        $data = array(
            'CATEGORY_ARRAY' => $category_name
        );

        $this->viewer->all('stats/point', $data);
    }

    public function ajaxSelectPointStat() {
        $data = array(
            'DATA_KEY' => 302,
            'START_DATE' => $this->input->post('start_date'),
            'END_DATE' => $this->input->post('end_date'),
        );
        $pointAmountRows = $this->Stat_data->selectStat($data);

        $total_count = count($pointAmountRows);

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        $rows = array();
        foreach ($pointAmountRows as $date => $_rows1) {
            foreach ($_rows1 as $point_type => $_rows2) {
                foreach ($_rows2 as $category => $row) {
                    $rows[$date]['VALUE_' . $point_type . '_' . $category] = $row['DATA_VALUE'];
                }
            }
        }

        foreach ($rows as $date => $row) {
            $row['DATA_DATE'] = $date;
            $response['rows'][] = $row;
        }

        json_output($response, true);
    }

    public function game() {
        $this->load->model(array('Game_data', 'Character_data', 'Skill_data', 'Treasure_data'));

        $stageInfoRows = $this->Game_data->selectStageInfo();
        $characterInfoRows = $this->Character_data->selectCharacterInfo();
        $skillInfoRows = $this->Skill_data->selectSkillInfo();
        $treasureInfoRows = $this->Treasure_data->selectTreasureInfo();

        $data = array(
            'STAGE_ARRAY' => $stageInfoRows,
            'CHARACTER_ARRAY' => $characterInfoRows,
            'SKILL_ARRAY' => $skillInfoRows,
            'TREASURE_ARRAY' => $treasureInfoRows,
        );

        $this->viewer->all('stats/game', $data);
    }

    public function ajaxSelectStageStat() {
        $data = array(
            'DATA_KEY' => 401,
            'START_DATE' => $this->input->post('start_date'),
            'END_DATE' => $this->input->post('end_date'),
        );
        $stageRows = $this->Stat_data->selectStat($data);

        $total_count = count($stageRows);

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        $rows = array();
        foreach ($stageRows as $date => $_rows) {
            foreach ($_rows as $stage_id => $row) {
                $rows[$date]['VALUE_' . $stage_id] = $row['DATA_VALUE'];
            }
        }

        foreach ($rows as $date => $row) {
            $row['DATA_DATE'] = $date;
            $response['rows'][] = $row;
        }

        json_output($response, true);
    }

    public function ajaxSelectCharacterStat() {
        $data = array(
            'DATA_KEY' => 402,
            'START_DATE' => $this->input->post('start_date'),
            'END_DATE' => $this->input->post('end_date'),
        );
        $characterRows = $this->Stat_data->selectStat($data);

        $total_count = count($characterRows);

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        $rows = array();
        foreach ($characterRows as $date => $_rows1) {
            foreach ($_rows1 as $character_id => $_rows2) {
                foreach ($_rows2 as $level => $row) {
                    $rows[$date]['VALUE_' . $character_id . '_' . $level] = $row['DATA_VALUE'];
                }
            }
        }

        foreach ($rows as $date => $row) {
            $row['DATA_DATE'] = $date;
            $response['rows'][] = $row;
        }

        json_output($response, true);
    }

    public function ajaxSelectSkillStat() {
        $data = array(
            'DATA_KEY' => 403,
            'START_DATE' => $this->input->post('start_date'),
            'END_DATE' => $this->input->post('end_date'),
        );
        $skillRows = $this->Stat_data->selectStat($data);

        $total_count = count($skillRows);

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        $rows = array();
        foreach ($skillRows as $date => $_rows1) {
            foreach ($_rows1 as $skill_id => $_rows2) {
                foreach ($_rows2 as $level => $row) {
                    $rows[$date]['VALUE_' . $skill_id . '_' . $level] = $row['DATA_VALUE'];
                }
            }
        }

        foreach ($rows as $date => $row) {
            $row['DATA_DATE'] = $date;
            $response['rows'][] = $row;
        }

        json_output($response, true);
    }

    public function ajaxSelectTreasureStat() {
        $data = array(
            'DATA_KEY' => 404,
            'START_DATE' => $this->input->post('start_date'),
            'END_DATE' => $this->input->post('end_date'),
        );
        $treasureRows = $this->Stat_data->selectStat($data);

        $total_count = count($treasureRows);

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        $rows = array();
        foreach ($treasureRows as $date => $_rows1) {
            foreach ($_rows1 as $treasure_id => $_rows2) {
                foreach ($_rows2 as $level => $row) {
                    $rows[$date]['VALUE_' . $treasure_id . '_' . $level] = $row['DATA_VALUE'];
                }
            }
        }

        foreach ($rows as $date => $row) {
            $row['DATA_DATE'] = $date;
            $response['rows'][] = $row;
        }

        json_output($response, true);
    }

    public function ajaxSelectPlayCountStat() {
        $data = array(
            'DATA_KEY' => 405,
            'START_DATE' => $this->input->post('start_date'),
            'END_DATE' => $this->input->post('end_date'),
        );
        $playCountRows = $this->Stat_data->selectStat($data);

        $total_count = count($playCountRows);

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        $rows = array();
        foreach ($playCountRows as $date => $_rows) {
            $rows[$date]['TOTAL_COUNT'] = 0;
            foreach ($_rows as $stage_id => $row) {
                $rows[$date]['VALUE_' . $stage_id] = $row['DATA_VALUE'];
                $rows[$date]['TOTAL_COUNT'] += $row['DATA_VALUE'];
            }
        }

        foreach ($rows as $date => $row) {
            $row['DATA_DATE'] = $date;
            $response['rows'][] = $row;
        }

        json_output($response, true);
    }

    function index() {
        $this->hourly();
    }

    function hourly() {
        $data = array();

        $this->viewer->all('stats/hourly', $data);
    }

    function item() {
        $runeInfoRows = $this->Log_data->getRuneInfo();
        $stageInfoRows = $this->Log_data->getStageInfo();

        $this->load->config('log');
        $coin_log_info_array = $this->config->item('coin_log_info');

        $data = array(
                'RUNE_ARRAY' => $runeInfoRows,
                'STAGE_ARRAY' => $stageInfoRows,
                'COIN_LOG_INFO_ARRAY' => $coin_log_info_array,
            );

        $this->viewer->all('stats/item', $data);
    }

    function social() {
        $data = array();

        $this->viewer->all('stats/social', $data);
    }

    function os() {
        $osNameArray = array('', 'iOS', 'Android');
        
        $data = array(
                'OS_NAME_ARRAY' => $osNameArray
            );

        $this->viewer->all('stats/os', $data);
    }

    function country() {
        $countryNameRows = $this->Log_data->getStatsCountryNameRows();
        
        $data = array(
                'COUNTRY_NAME_ARRAY' => $countryNameRows
            );

        $this->viewer->all('stats/country', $data);
    }

    function score() {
        $data = array();

        $this->viewer->all('stats/score', $data);
    }

    function sale() {
        $this->load->config('solipop');
        $item_name_array = $this->config->item('item_name');
        $item_price_array = $this->config->item('item_price');

        $item_array = array();
        foreach ($item_price_array as $item_code => $item_price) {
            $item_array[$item_code] = $item_name_array[$item_code];
        }
        ksort($item_array);

        $data = array(
                'ITEM_NAME_ARRAY' => $item_array
            );

        $this->viewer->all('stats/sale', $data);
    }

    function ajaxGetStatsHourlyData($stat_type = 1) {
        $data = array(
                'STAT_TYPE' => $stat_type,
                'START_DATE' => $this->input->post('start_date'),
                'END_DATE' => $this->input->post('end_date'),
            );

        $dataArray = $this->Log_data->getStatsHourlyRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        $response = array();
        $response['page'] = 1;
        $response['total'] = 1;
        $response['records'] = $total_count;
        $index = 0;
        foreach ($statRows as $stat_date => $rows) {
            $response['rows'][$index]['id'] = $stat_date;
            $response['rows'][$index]['cell'][] = $stat_date;
            $response['rows'][$index]['cell'][] = 0;

            $total_value = 0;
            foreach (range(0, 23) as $hour) {
                $value = isset($rows[$hour]) ? $rows[$hour] : 0;

                $response['rows'][$index]['cell'][] = $value;
                $total_value += $value;
            }

            $response['rows'][$index]['cell'][1] = $total_value;
            $index++;
        }

        json_output($response, true);
    }

    function downloadGetStatsHourlyData($stat_type = 1, $start_date, $end_date) {
        header("Content-type:application/vnd.ms-excel;charset=UTF-8");
        header("Content-Disposition:attachment; filename=Stats_Hourly_Data_" . date("Ymd") . ".xls");
        header("Expires:0");
        header("Cache-Control:must-revalidate,post-check=0,pre-check=0");
        header("Pragma:public");

        $data = array(
                'STAT_TYPE' => $stat_type,
                'START_DATE' => $start_date,
                'END_DATE' => $end_date,
            );

        $dataArray = $this->Log_data->getStatsHourlyRows($data);
        $statRows = $dataArray['DATA'];
        $total_count = $dataArray['TOTAL_COUNT'];

        echo "Date";
        foreach (range(0, 23) as $hour) {
            echo "\t" . $hour . "H";
        }
        echo "\n";

        $index = 0;
        foreach ($statRows as $stat_date => $rows) {
            echo $stat_date;
            foreach (range(0, 23) as $hour) {
                $value = isset($rows[$hour]) ? $rows[$hour] : 0;
                echo "\t" . $value;
            }
            echo "\n";
        }
    }
}
