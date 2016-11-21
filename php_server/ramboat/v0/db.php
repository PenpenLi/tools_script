<?php
include_once 'helper.php';

class DB {
	private $db_name = "ramboat";
	private $db_ip = "127.0.0.1";
	private $db_user = "root";
	private $db_pw = "root123!";
	private $connect;

	function __construct() {
		$this->connectMysql();
	}

	function __destruct() {
		$this->closeMysql();
	}

	function connectMysql() {
		$this->connect = mysql_connect($this->db_ip, $this->db_user, $this->db_pw);
		if (!$this->connect) {
			helper_log("connect mysql fail");
			return false;
		}
		mysql_select_db($this->db_name, $this->connect);
		return true;
	}

	function closeMysql() {
		mysql_close($this->connect);
	}

	public function selectAllUser() {
		if (!$this->connect) {
			helper_log("connect invalid");
			return false;
		}
		$sql = "SELECT USERID FROM op_user";
		$result = mysql_query($sql, $this->connect);
		if (!$result) {
			helper_log("select failed");
			return false;
		}
		//$row = mysql_fetch_array($result, MYSQL_ASSOC);
		$rows=array();
		while($row=mysql_fetch_array($result)){
		   	$rows[]=$row;
		}
		return $rows;
	}

	public function selectUser($userInfo) {
		if (!$this->connect) {
			helper_log("connect invalid");
			return false;
		}
		// $sql = "SELECT USERID,NAME,PW,ICON,LEVEL,SCORE,LOGINTIMES,LOGINSTAMP FROM op_user WHERE USERID='$userInfo[USERID]'";
		$sql = "SELECT USERID,NAME,PW,ICON,LEVEL,SCORE,MAXSCORE,LASTLOGINDAY,LASTWEEKRANK FROM op_user WHERE USERID='$userInfo[USERID]'";
		$result = mysql_query($sql, $this->connect);
		if (!$result) {
			helper_log("select failed");
			return false;
		}
		$row = mysql_fetch_array($result, MYSQL_ASSOC);
		return $row;
	}

	public function selectUserByName($userInfo) {
		if (!$this->connect) {
			helper_log("connect invalid");
			return false;
		}
		// $sql = "SELECT USERID,NAME,PW,ICON,LEVEL,SCORE,LOGINTIMES,LOGINSTAMP FROM op_user WHERE USERID='$userInfo[USERID]'";
		$sql = "SELECT USERID,NAME,PW,ICON,LEVEL,SCORE,MAXSCORE,LASTLOGINDAY,LASTWEEKRANK FROM op_user WHERE NAME='$userInfo[USERNAME]'";
		$result = mysql_query($sql, $this->connect);
		if (!$result) {
			helper_log("select failed");
			return false;
		}
		$row = mysql_fetch_array($result, MYSQL_ASSOC);
		return $row;
	}

	public function selectUserRankByDate($day) {
		$rankName = 'RANK'.$day;
		if (!$this->connect) {
			helper_log("connect invalid");
			return false;
		}
		$sql = "SELECT USERID,NAME,$rankName,ICON FROM op_user ORDER BY $rankName LIMIT 0,100";
		$result = mysql_query($sql, $this->connect);
		if (!$result) {
			helper_log("select failed");
			return false;
		}
		
		$rows=array();
		while($row=mysql_fetch_array($result)){
		    $rows[]=$row;
		}
		return $rows;
	}

	public function selectUserRank($userId, $day) {
		$rankName = 'RANK'.$day;
		if (!$this->connect) {
			helper_log("connect invalid");
			return false;
		}
		$sql = "SELECT $rankName FROM op_user WHERE USERID='$userId'";
		$result = mysql_query($sql, $this->connect);
		if (!$result) {
			helper_log("select failed");
			return false;
		}
		$row = mysql_fetch_array($result, MYSQL_ASSOC);
		return $row;
	}

	public function insertUser($userInfo) {
		if (!$this->connect) {
			helper_log("connect invalid");
			return false;
		}
		$sql = helper_getInsertSQL("op_user", $userInfo);
		helper_log($sql);
		if (!mysql_query($sql, $this->connect)) {
			helper_log("insertUser failed");
			return false;
		}
		return true;
	}

	public function updateUser($userInfo) {
		if (!$userInfo['USERID']) {
			helper_log("param invalid");
			return false;
		}
		if (!$this->connect) {
			helper_log("connect invalid");
			return false;
		}
		$sql = helper_getUpdateSQL("op_user", "USERID", $userInfo);
		if (!mysql_query($sql, $this->connect)) {
			helper_log("updateUser failed");
			return false;
		}
		return true;
	}

	public function updateAllUser($userInfo) {

		if (!$this->connect) {
			helper_log("connect invalid");
			return false;
		}
		$sql = helper_getUpdateSQL("op_user", null, $userInfo);
		if (!mysql_query($sql, $this->connect)) {
			helper_log("updateUser failed");
			return false;
		}
		return true;
	}
}


?>

