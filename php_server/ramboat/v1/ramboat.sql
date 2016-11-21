DROP TABLE IF EXISTS op_user;
CREATE TABLE `op_user` (
  `UID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `USERID` varchar(32) NOT NULL,
  `NAME` varchar(32) NOT NULL,
  `PW` varchar(32) NOT NULL,
  `ICON` varchar(32) NOT NULL,
  `LEVEL` int unsigned NOT NULL,
  `SCORE` int unsigned NOT NULL,
  `MAXSCORE` int unsigned NOT NULL,
  `LASTLOGINDAY` int unsigned NOT NULL,
  `LASTWEEKRANK` varchar(102400) NOT NULL,
  `RANK1` varchar(102400) NOT NULL,
  `RANK2` varchar(102400) NOT NULL,
  `RANK3` varchar(102400) NOT NULL,
  `RANK4` varchar(102400) NOT NULL,
  `RANK5` varchar(102400) NOT NULL,
  `RANK6` varchar(102400) NOT NULL,
  `RANK7` varchar(102400) NOT NULL,
   PRIMARY KEY (`UID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
