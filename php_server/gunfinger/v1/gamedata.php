<?php
include_once 'helper.php';
include_once 'db.php';
include_once 'config.php';

class GameData {
	private $USER_HASH = 'USER_HASH';
	private $SCORE_RANK = 'SCORE';
	private $SCORE_RANK_BACKUP = 'SCORE_BACKUP';
	private $LOGIN_RANK = 'LOGIN';
	// private $LOGIN_RANK_BACKUP = 'LOGIN_BACKUP';
	private $db;
	private $redis;

	function __construct() {
		$this->db = new DB();
		$this->redis = new Redis();
		$this->redis->connect('127.0.0.1', Config::REDIS_PORT);
	}
	function __destruct() {
	}

	public function addNewUser($userInfo) {
		$dataArray = array(
			'USERID' => $userInfo['userId'],
			'NAME' => $userInfo['name'],
			'PW' => $userInfo['pw'],
			'ICON' => $userInfo['icon'],
			'LEVEL' => 0,
			'SCORE' => 0,
			'LOGINTIMES' => 0,
			'LOGINSTAMP' => 0
		);
		$ret = $this->db->insertUser($dataArray);
	}

	public function getAllUserId() {
		return $this->redis->hKeys($this->USER_HASH);
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

	public function updateUserScore($userId, $newscore, $level) {
		$updateInfo = array("USERID" => $userId);
		if ($newscore) {
			$updateInfo['SCORE'] = $newscore;
		}
		if ($level) {
			$updateInfo['LEVEL'] = $level;
		}
		$this->updateUserInfo($updateInfo);
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
		$this->redis->hSet($this->USER_HASH, $userId, json_encode($userInfo));

		// 2. update score rank memchache
		if ($updateInfo['SCORE']) {
		    $userScore = $this->redis->zScore($this->SCORE_RANK, $userId);
            if ($updateInfo['SCORE'] > $userScore) {
                $this->redis->zAdd($this->SCORE_RANK, intval($updateInfo['SCORE']), $userId);
            }
		}

		// 3. update login rank memcache
		if ($updateInfo['LOGINTIMES']) {
			$this->redis->zAdd($this->LOGIN_RANK, intval($updateInfo['LOGINTIMES']), $userId);
		}

		// 4. update mysql async
		$this->redis->publish('userinfo_chan', json_encode($updateInfo));
	}

	public function getUserInfo($userId) {
		$userInfoStr = $this->redis->hGet($this->USER_HASH, $userId);
		if ($userInfoStr) {
			return json_decode($userInfoStr, true);
		} else {
			$row = $this->db->selectUser(array('USERID' => $userId));
			if ($row) {
				$this->redis->hSet($this->USER_HASH, $userId, json_encode($row));
				//$this->redis->zAdd($this->SCORE_RANK, intval($row['SCORE']), $userId);
			}
			return $row;
		}
	}

	public function getScoreRank($userId) {
		$rankList = array();
		$userList = $this->redis->zRevRange($this->SCORE_RANK, 0, 99, true);
		$rankNum = 0;
		foreach ($userList as $key => $value) {
			if (!$key) {continue;}
			$userInfo = $this->getUserInfo($key);
			if (!$userInfo) {continue;}
			$userScore = $this->redis->zScore($this->SCORE_RANK, $key);
			$rankNum = $rankNum + 1;
			$rankInfo = array(
				"uid" => $userInfo['USERID'],
				"name" => $userInfo['NAME'],
				"icon" => $userInfo['ICON'],
				"level" => intval($userInfo['LEVEL']),
				"score" => $userScore,
				"rank" => $rankNum
			);
			array_push($rankList, $rankInfo);
		}
		if (!isset($userList[$userId])) {
			$userInfo = $this->getUserInfo($userId);
			if (!$userInfo) {return $rankList;}
			$userScore = $this->redis->zScore($this->SCORE_RANK, $key);
			$rankNum = $this->redis->zRevRank($this->SCORE_RANK, $userId) + 1;
			if (!is_integer($userScore)) {
                $userScore = 0;
                $rankNum = $this->redis->zSize($this->SCORE_RANK) + 1;
            }
			$rankInfo = array(
				"uid" => $userInfo['USERID'],
				"name" => $userInfo['NAME'],
				"icon" => $userInfo['ICON'],
				"level" => intval($userInfo['LEVEL']),
				"score" => $userScore,
				"rank" => $rankNum
			);
			array_push($rankList, $rankInfo);
		}

		return $rankList;
	}

	public function getLoginRank($userId) {
		$rankList = array();
		$userList = $this->redis->zRevRange($this->LOGIN_RANK, 0, 99, true);
		$rankNum = 0;
		foreach ($userList as $key => $value) {
			if (!$key) {continue;}
			$userInfo = $this->getUserInfo($key);
			if (!$userInfo) {continue;}
			$rankNum = $rankNum + 1;
			$rankInfo = array(
				"uid" => $userInfo['USERID'],
				"name" => $userInfo['NAME'],
				"icon" => $userInfo['ICON'],
				"level" => intval($userInfo['LEVEL']),
				"time" => intval($value),
				"rank" => $rankNum
			);
			array_push($rankList, $rankInfo);
		}
		if (!isset($userList[$userId])) {
			$userInfo = $this->getUserInfo($userId);
			if (!$userInfo) {return $rankList;}
			$rankNum = $this->redis->zRevRank($this->LOGIN_RANK, $userId) + 1;
			$rankInfo = array(
				"uid" => $userInfo['USERID'],
				"name" => $userInfo['NAME'], 
				"icon" => $userInfo['ICON'],
				"level" => intval($userInfo['LEVEL']),
				"time" => intval($value),
				"rank" => $rankNum
			);
			array_push($rankList, $rankInfo);
		}
		return $rankList;
	}

	// public function getLastRank($userId, $day) {
	// 	$today = intval(date("w"));
	// 	if (0 == $today) {$today = 7;}
	// 	$selectDay = ($today - $day);
	// 	if ($selectDay <= 0) {$selectDay = $selectDay + 7;}
	// 	$row = $this->db->selectUserRank($userId, $selectDay);
	// 	$rankJson = $row['RANK'.$selectDay];
	// 	return json_decode($rankJson, true);
	// }

	public function getReward($userId) {
		$reward = array();
		$rankNum = $this->redis->zRevRank($this->SCORE_RANK_BACKUP, $userId);
		if (!is_integer($rankNum)) {return $reward;}
		$rankNum = $rankNum + 1;
		$totalPlayer = $this->redis->zCount($this->SCORE_RANK_BACKUP, 0, 2147483647);
		$percent = floor($rankNum*100/$totalPlayer);
		if ($percent <= 1) {
			array_push($reward, array("item"=>"mod", "count"=>6));
			// array_push($reward, array("item"=>"gold", "count"=>0));
			// array_push($reward, array("item"=>"coins", "count"=>0));
		} else if ($percent <= 10) {
			array_push($reward, array("item"=>"mod", "count"=>3));
			// array_push($reward, array("item"=>"gold", "count"=>0));
			// array_push($reward, array("item"=>"coins", "count"=>0));
		} else if ($percent <= 25) {
			array_push($reward, array("item"=>"mod", "count"=>2));
			// array_push($reward, array("item"=>"gold", "count"=>0));
			// array_push($reward, array("item"=>"coins", "count"=>0));
		} else if ($percent <= 40) {
			array_push($reward, array("item"=>"mod", "count"=>1));
			// array_push($reward, array("item"=>"gold", "count"=>0));
			// array_push($reward, array("item"=>"coins", "count"=>0));
		}
		return $reward;
	}

	// public function backupScoreRank($userId) {
	// 	$scoreRank = $this->getScoreRank($userId);
	// 	$thisDayRank = json_encode($scoreRank);
	// 	$day = intval(date("w"));
	// 	if ($day == 0) {$day = 7;}
	// 	$rankName = "RANK".$day;
	// 	$updateInfo = array("USERID" => $userId, $rankName => $thisDayRank);
	// 	$this->db->updateUser($updateInfo);
	// }

	public function getTotalScoreRank() {
		return $this->redis->zRevRange($this->SCORE_RANK, 0, -1, true);
	}

	public function backupScoreRank_redis() {
		$scoreRankKeyValue_backup = $this->redis->zRevRange($this->SCORE_RANK_BACKUP, 0, -1, true);
		foreach ($scoreRankKeyValue_backup as $key => $value) {
			$this->redis->zDelete($this->SCORE_RANK_BACKUP, $key);
		}

		$scoreRankKeyValue = $this->redis->zRevRange($this->SCORE_RANK, 0, -1, true);
		foreach ($scoreRankKeyValue as $key => $value) {
			if ($key) {
				$this->redis->zAdd($this->SCORE_RANK_BACKUP, intval($value), $key);
			}
		}
	}

	public function clearScoreRank_redis() {
		$scoreRankKeyValue = $this->redis->zRevRange($this->SCORE_RANK, 0, -1, true);
        foreach ($scoreRankKeyValue as $key => $value) {
        	$this->redis->zDelete($this->SCORE_RANK, $key);
        }
	}

	// public function backupLoginRank_redis() {
	// 	$loginRankKeyValue_backup = $this->redis->zRevRange($this->LOGIN_RANK_BACKUP, 0, -1, true);
	// 	foreach ($loginRankKeyValue_backup as $key => $value) {
	// 		$this->redis->zDelete($this->LOGIN_RANK_BACKUP, $key);
	// 	}

	// 	$loginRankKeyValue = $this->redis->zRevRange($this->LOGIN_RANK, 0, -1, true);
	// 	foreach ($loginRankKeyValue as $key => $value) {
	// 		$this->redis->zAdd($this->LOGIN_RANK_BACKUP, intval($value), $key);
	// 	}
	// }	

	public function getLastDayRank_redis($userId) {
		$rankList = array();
		$userScore = $this->redis->zScore($this->SCORE_RANK_BACKUP, $userId);
		if (!$userScore) {return $rankList;}
		$userList = $this->redis->zRevRange($this->SCORE_RANK_BACKUP, 0, 99, true);
		foreach ($userList as $key => $value) {
			$userInfo = $this->getUserInfo($key);
			$userScore = intval($value);
			$rankNum = $this->redis->zRevRank($this->SCORE_RANK_BACKUP, $key) + 1;
			$rankInfo = array(
				"uid" => $userInfo['USERID'],
				"name" => $userInfo['NAME'],
				"icon" => strval($userInfo['ICON']),
				"level" => intval($userInfo['LEVEL']),
				"score" => $userScore,
				"rank" => $rankNum
			);
			if ($rankInfo["uid"] && $rankInfo["name"]) {
				array_push($rankList, $rankInfo);
			}
		}
		if (!isset($userList[$userId])) {
			$userInfo = $this->getUserInfo($userId);
			$userScore = $this->redis->zScore($this->SCORE_RANK_BACKUP, $userId);
			$rankNum = $this->redis->zRevRank($this->SCORE_RANK_BACKUP, $userId) + 1;
			$rankInfo = array(
				"uid" => $userInfo['USERID'],
				"name" => $userInfo['NAME'],
				"icon" => strval($userInfo['ICON']),
				"level" => intval($userInfo['LEVEL']),
				"score" => $userScore,
				"rank" => $rankNum
			);
			if ($rankInfo["uid"] && $rankInfo["name"]) {
				array_push($rankList, $rankInfo);
			}
		}

		return $rankList;
	}
}

?>


