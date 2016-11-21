<?php
include_once 'helper.php';
include_once 'SSDB.php';
include_once 'db.php';

ini_set('date.timezone','Asia/Shanghai');

class GameData {
	private $USER_HASH = 'RAMBOAT_USER_HASH';
	private $PREFIX = '';
	private $SCORE_RANK = 'SCORE';
	private $YESTERDAY_SCORE_RANK = 'YESTERDAYSCORE';
	private $LAST_SCORE_RANK = 'LASTSCORE';
	private $MAX_SCORE_RANK = 'MAXSCORE';
	private $LOGIN_RANK = 'LOGIN';
	private $NAMELIST = 'NAMELIST';
	private $ssdb;
	private $FRESH_RECORD = 'FRESH_RECORD';

	function __construct() {
		$this->db = new DB();
		$this->ssdb = new SimpleSSDB('127.0.0.1', 8880);
	}
	function __destruct() {
	}

	public function addNewUser($userInfo) {
		$day = intval(date("w"));
		if ($day == 0) {$day = 7;}
		$dataArray = array(
			'USERID' => $userInfo['userId'],
			'NAME' => $userInfo['name'],
			'PW' => $userInfo['pw'],
			'ICON' => $userInfo['icon'],
			'LEVEL' => 0,
			'SCORE' => 0,
			'MAXSCORE' => 0,
			'LASTLOGINDAY' => $day,
			'MILITARY' => 1
		);
		if ($dataArray['ICON'] == "") {
			$dataArray['ICON'] = '0';
		}
		$this->ssdb->hset($this->USER_HASH, $this->PREFIX.$userInfo['userId'], json_encode($dataArray));
		$this->ssdb->hset($this->NAMELIST, $row['NAME'], 1);
	}

	public function userNameCheck($userName) {
		$userName =preg_replace("/\s|¡¡/","",$userName);
		if($userName == '') {
			return array ('code' => 1002);
		}
		if ($this->ssdb->hget($NAMELIST, $userName)) {
			return array ('code' => 1001);
		}
		$row = $this->db->selectUserByName(array('USERNAME' => $userName));
		if ($row) {
			$this->ssdb->hset($this->NAMELIST, $row['NAME'], 1);
			return array ('code' => 1001);
		}
		
		require_once('badword.src.php');
		$badword1 = array_combine($badword,array_fill(0,count($badword),'*'));
		$str = $userName;
		$str = strtr($str, $badword1);
		if ($str != $userName) {
			$pos = 0;
			for($i=1; $i<strlen($str); $i++) {
				if ($str[$i] == '*') {
					$pos = $i;
					break;
				}
			}
			$strSub = strlen(substr($str, $pos+1));
			$strBefore = strlen(substr($userName, 0, $pos));
			$strAfter = 0;
			if ($strSub != 0) {
				$strAfter = strlen(substr($userName, $strSub*-1));
			}
			$change = array (
				$strBefore => '',
				$strAfter => '');
			$strResult = substr($userName, $pos, strlen($userName)-$pos-$strAfter);
			return array ('code' => 1003, 'msg' => $strResult);
		}
		return false;
	}

	private function todayRank() {
		return "rank_".date("Y-m-d");
	}

	private function lastNDayRank($day) {
	   $time = time();
	   return date('Y-m-d', strtotime('+'.$day.' day',$time));
	}

	public function updateLoginInfo($userId, $timeStamp, $loginTimes) {
		$this->updateUserInfo(array('USERID' => $userId, 'LOGINTIMES' => $loginTimes, 'LOGINSTAMP' => $timeStamp));
	}

	public function modifyUserInfo($userId, $newname, $newicon) {
		$modifyInfo = array("USERID" => $userId);
		if ($newname) {
			$modifyInfo['NAME'] = $newname;
		}
		if ($newicon) {
			$modifyInfo['ICON'] = $newicon;
		}
		$this->updateUserInfo($modifyInfo);
	}

	public function updateUserScore($userId, $newscore, $newIcon, $military, $shipType) {
		$updateInfo = array("USERID" => $userId);
		if ($newscore) {
			$updateInfo['SCORE'] = $newscore;
		}
		$updateInfo['ICON'] = $newIcon;
		if ($military) {
			$updateInfo['MILITARY'] = $military;
		}
		$updateInfo['SHIPTYPE_WILL_BE_UNSET'] = $shipType;
		$userScore = $this->ssdb->zget($this->todayRank(), $userId);
		if ($updateInfo['SCORE'] <= $userScore) {
			return 2001;
		}
		$this->updateUserInfo($updateInfo);
		return 1;
	}

	private function updateUserInfo($updateInfo) {
		if (!$updateInfo || !isset($updateInfo['USERID'])) {
			helper_log("[updateUserInfo] param invalid");
			return;
		}
		$userId = $updateInfo['USERID'];

		// 1. update userinfo memcache
		$userInfo = $this->getUserInfo($userId);
		foreach ($updateInfo as $key => $value) {
			$userInfo[$key] = $value;
		}
		$this->ssdb->hset($this->USER_HASH, $this->PREFIX.$userId, json_encode($userInfo));

		// 2. update score rank memchache
		if ($updateInfo['SCORE']) {
			$this->ssdb->zset($this->todayRank(), $userId, intval($updateInfo['SCORE']));
		}
	}

	public function getUserInfo($userId) {
		$userInfoStr = $this->ssdb->hget($this->USER_HASH, $this->PREFIX.$userId);
		if ($userInfoStr) {
			return json_decode($userInfoStr, true);
		}
		$row = $this->db->selectUser(array('USERID' => $userId));
		if ($row) {
			$this->ssdb->hset($this->USER_HASH, $this->PREFIX . $userId, json_encode($row));
			$this->ssdb->hset($this->NAMELIST, $row['NAME'], 1);
		}
		return $row;
	}

	public function getScoreRank($userId) {
		$rankName = $this->todayRank();
		return $this->getScoreRankByName($rankName, $userId);
	}

	public function getLastDayRank($userId) {
		$rankName = $this->lastNDayRank(1);
		return $this->getScoreRankByName($rankName, $userId);
	}

	private function getScoreRankByName($rankName, $userId) {
        $rankList = array();
        $userList = $this->ssdb->zrrange($rankName, 0, 98);
		$rankNum = 0;
        foreach ($userList as $key => $value) {
			if (!$key) {continue;}
			$rankNum = $rankNum + 1;
	        $userInfo = $this->getUserInfo($key);
	        $userScore = $this->ssdb->zget($rankName, $key);
	        $rankInfo = array(
                "uid" => $userInfo['USERID'],
                "name" => $userInfo['NAME'],
                "icon" => strval($userInfo['ICON']),
                "score" => $userScore,
				"military" => intval($userInfo['MILITARY']),
				"ship" => intval($userInfo['SHIPTYPE_WILL_BE_UNSET']),
                "rank" => $rankNum
	        );
	        if ($rankInfo["uid"] && $rankInfo["name"]) {
	            array_push($rankList, $rankInfo);
	        }
        }
        if (!isset($userList[$userId])) {
            $userInfo = $this->getUserInfo($userId);
            $userScore = $this->ssdb->zget($rankName, $userId);
            $rankNum = $this->ssdb->zrrank($rankName, $userId) + 1;
            if (!$userScore) {
                $userScore = 0;
                $rankNum = $this->ssdb->zsize($rankName) + 1;
            }
            $rankInfo = array(
                "uid" => $userInfo['USERID'],
                "name" => $userInfo['NAME'],
                "icon" => strval($userInfo['ICON']),
                "score" => $userScore,
				"military" => intval($userInfo['MILITARY']),
				"ship" =>intval($userInfo['SHIPTYPE_WILL_BE_UNSET']),
                "rank" => $rankNum
            );
            if ($rankInfo["uid"] && $rankInfo["name"]) {
                array_push($rankList, $rankInfo);
            }
        }
        return $rankList;
    }

	public function getSelfScoreRank($userId) {
		$rankName = $this->todayRank();
		$userInfo = $this->getUserInfo($userId);
		$userScore = $this->ssdb->zget($rankName, $userId);
		$rankNum = $this->ssdb->zrrank($rankName, $userId) + 1;
		if (!$userScore) {
            $userScore = 0;
            $rankNum = $this->ssdb->zsize($rankName) + 1;
        }
		if(!$userInfo['SHIPTYPE_WILL_BE_UNSET']) {
			$userInfo['SHIPTYPE_WILL_BE_UNSET'] = 0;
		}
		$rankInfo = array(
			"uid" => $userInfo['USERID'],
			"name" => $userInfo['NAME'],
			"icon" => strval($userInfo['ICON']),
			"score" => $userScore,
			"military" => intval($userInfo['MILITARY']),
			"ship" => intval($userInfo['SHIPTYPE_WILL_BE_UNSET']),
			"rank" => $rankNum
		);
		return $rankInfo;
	}

	public function getReward($userId) {
		date_default_timezone_set("PRC");
		$userInfo = $this->getUserInfo($userId);
		$day = intval(date("w"));
		if ($day == 0) {$day = 7;}
		$lastDay = $userInfo['LASTLOGINDAY'];
		if ($day != $lastDay) {
			$updateInfo = array("USERID" => $userId, 'LASTLOGINDAY' => $day);
			$this->updateUserInfo($updateInfo);
			$rankInfo = $this->getLastDayRank($userId);
			foreach ($rankInfo as $value) {
				if ($value['uid'] == $userId) {
					$rank = $value['rank'];
					$reward = $this->getRewardInfo($rank);
					return $reward;
				}
			}
		}
		return null;
	}

	public function getRewardInfo($rank) {
		require_once('rewardconfig.php');
		foreach ($ramboatDailyRewardConfig as $key => $value) {
			if ($rank >= $value['start'] && $rank <= $value['end']) {
				$reward = array ();
				$reward['item'] = $value['reward'];
				$reward['count'] = $value['count'];
				$reward['type'] = intval($value['type']);
				$reward['rank'] = $rank;
				return $reward;
			}
		}
		return null;
	}
}

?>

