DROP TABLE IF EXISTS `op_weixin_notify`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE op_weixin_notify (
  `NOTIFY_ID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `APPID` varchar(128) NOT NULL,
  `ATTACH` varchar(128) NOT NULL,
  `BANK_TYPE` varchar(128) NOT NULL,
  `CASH_FEE` varchar(128) NOT NULL,
  `FEE_TYPE` varchar(128) NOT NULL,
  `IS_SUBSCRIBE` varchar(128) NOT NULL,
  `NONCE_STR` varchar(128) NOT NULL,
  `OPEN_ID` varchar(128) NOT NULL,
  `OUT_TRADE_NO` varchar(128) NOT NULL,
  `RESULT_CODE` varchar(128) NOT NULL,
  `RETURN_CODE` varchar(128) NOT NULL,
  `SIGN` varchar(128) NOT NULL,
  `TIME_END` varchar(128) NOT NULL,
  `TOTAL_FEE` varchar(128) NOT NULL,
  `TRADE_TYPE` varchar(128) NOT NULL,
  `TRANSACTION_ID` varchar(128) NOT NULL,
  PRIMARY KEY (`NOTIFY_ID`),
  KEY `TRANSACTION_ID` (`TRANSACTION_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS op_order_info;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `op_order_info` (
  `ORDER_ID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `CLIENT_ID` varchar(128) NOT NULL,
  `GAME_NAME` varchar(32) NOT NULL,
  `SDK_PAY_TYPE` varchar(32) NOT NULL,
  `TIME_STAMP` varchar(32) NOT NULL,
  `VERIFY_STATE` varchar(16) NOT NULL,
  `TRANSACTION_ID` varchar(128) NOT NULL,
  PRIMARY KEY (`ORDER_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `op_360_notify`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE op_360_notify (
  `NOTIFY_ID` bigint unsigned NOT NULL AUTO_INCREMENT,
  `APP_KEY` varchar(32) NOT NULL,
  `PRODUCT_ID` varchar(36) NOT NULL,
  `AMOUNT` int unsigned NOT NULL,
  `APP_UID` varchar(50) NOT NULL,
  `APP_EXT1` varchar(255) NOT NULL,
  `APP_EXT2` varchar(255) NOT NULL,
  `USER_ID` bigint unsigned NOT NULL,
  `ORDER_ID` bigint unsigned NOT NULL,
  `GATEWAY_FLAG` varchar(16) NOT NULL,
  `SIGN_TYPE` varchar(8) NOT NULL,
  `APP_ORDER_ID` varchar(64) NOT NULL,
  `SIGN_RETURN` varchar(32) NOT NULL,
  `SIGN` varchar(32) NOT NULL,
  PRIMARY KEY (`NOTIFY_ID`),
  KEY `TRANSACTION_ID` (`ORDER_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;