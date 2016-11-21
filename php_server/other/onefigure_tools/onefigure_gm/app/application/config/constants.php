<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');

/*
|--------------------------------------------------------------------------
| File and Directory Modes
|--------------------------------------------------------------------------
|
| These prefs are used when checking and setting modes when working
| with the file system.  The defaults are fine on servers with proper
| security, but you may wish (or even need) to change the values in
| certain environments (Apache running a separate process for each
| user, PHP under CGI with Apache suEXEC, etc.).  Octal values should
| always be used to set the mode correctly.
|
*/
define('FILE_READ_MODE', 0644);
define('FILE_WRITE_MODE', 0666);
define('DIR_READ_MODE', 0755);
define('DIR_WRITE_MODE', 0777);

/*
|--------------------------------------------------------------------------
| File Stream Modes
|--------------------------------------------------------------------------
|
| These modes are used when working with fopen()/popen()
|
*/

define('FOPEN_READ',							'rb');
define('FOPEN_READ_WRITE',						'r+b');
define('FOPEN_WRITE_CREATE_DESTRUCTIVE',		'wb'); // truncates existing file data, use with care
define('FOPEN_READ_WRITE_CREATE_DESTRUCTIVE',	'w+b'); // truncates existing file data, use with care
define('FOPEN_WRITE_CREATE',					'ab');
define('FOPEN_READ_WRITE_CREATE',				'a+b');
define('FOPEN_WRITE_CREATE_STRICT',				'xb');
define('FOPEN_READ_WRITE_CREATE_STRICT',		'x+b');

/*
|--------------------------------------------------------------------------
| Game Variables
|--------------------------------------------------------------------------
|
|
|
*/

define('MAX_CHARACTER_LEVEL',           8);
define('MAX_SKILL_LEVEL',               8);
define('MAX_TREASURE_LEVEL',            8);
define('MAX_SKILL_SLOT',                3);
define('MAX_TREASURE_SLOT',             3);
define('MAX_TREASURE_INVENTORY',        150);
define('MAX_INSTANT_ITEM_AMOUNT',       99);
define('MAX_FRIEND_COUNT',              40);
define('MAX_LOTTERY_POINT',             1000);

define('SERVER_CURRENT_VERSION',        22);
/*
|--------------------------------------------------------------------------
| Database Variables
|--------------------------------------------------------------------------
|
|
|
*/

define('LOG_TABLE_PAY',                         'dh_market_order');
define('LOG_TABLE_HOURLY_DATA',                 'stats_hourly_data');
define('LOG_TABLE_DAILY_DEPTH_DATA',            'stats_daily_depth_data');
define('LOG_TABLE_DAILY_DEPTH_COUNTRY_DATA',    'stats_daily_depth_country_data');
define('LOG_TABLE_DAILY_ITEM_USED_DATA',        'stats_daily_item_used_data');

define('LOG_TABLE_DATE',                date('Ymd'));
define('LOG_TABLE_DATE_YESTERDAY',      date('Ymd', strtotime('-1 day')));

define('TODAY_MONTH', date('Ym'));
define('TODAY_DATE', date('Ymd'));
define('YESTERDAY_MONTH', date('Ym', strtotime('-1 day')));
define('YESTERDAY_DATE', date('Ymd', strtotime('-1 day')));
define('2DAYS_AGO_MONTH', date('Ym', strtotime('-2 days')));
define('2DAYS_AGO_DATE', date('Ymd', strtotime('-2 days')));

define('CACHE_REFRESH_TIME',            86400);

/* End of file constants.php */
/* Location: ./application/config/constants.php */
