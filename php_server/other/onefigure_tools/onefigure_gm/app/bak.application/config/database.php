<?php  if ( ! defined('BASEPATH')) exit('No direct script access allowed');
/*
| -------------------------------------------------------------------
| DATABASE CONNECTIVITY SETTINGS
| -------------------------------------------------------------------
| This file will contain the settings needed to access your database.
|
| For complete instructions please consult the 'Database Connection'
| page of the User Guide.
|
| -------------------------------------------------------------------
| EXPLANATION OF VARIABLES
| -------------------------------------------------------------------
|
|	['hostname'] The hostname of your database server.
|	['username'] The username used to connect to the database
|	['password'] The password used to connect to the database
|	['database'] The name of the database you want to connect to
|	['dbdriver'] The database type. ie: mysql.  Currently supported:
				 mysql, mysqli, postgre, odbc, mssql, sqlite, oci8
|	['dbprefix'] You can add an optional prefix, which will be added
|				 to the table name when using the  Active Record class
|	['pconnect'] TRUE/FALSE - Whether to use a persistent connection
|	['db_debug'] TRUE/FALSE - Whether database errors should be displayed.
|	['cache_on'] TRUE/FALSE - Enables/disables query caching
|	['cachedir'] The path to the folder where cache files should be stored
|	['char_set'] The character set used in communicating with the database
|	['dbcollat'] The character collation used in communicating with the database
|				 NOTE: For MySQL and MySQLi databases, this setting is only used
| 				 as a backup if your server is running PHP < 5.2.3 or MySQL < 5.0.7
|				 (and in table creation queries made with DB Forge).
| 				 There is an incompatibility in PHP with mysql_real_escape_string() which
| 				 can make your site vulnerable to SQL injection if you are using a
| 				 multi-byte character set and are running versions lower than these.
| 				 Sites using Latin-1 or UTF-8 database character set and collation are unaffected.
|	['swap_pre'] A default table prefix that should be swapped with the dbprefix
|	['autoinit'] Whether or not to automatically initialize the database.
|	['stricton'] TRUE/FALSE - forces 'Strict Mode' connections
|							- good for ensuring strict SQL while developing
|
| The $active_group variable lets you choose which connection group to
| make active.  By default there is only one group (the 'default' group).
|
| The $active_record variables lets you determine whether or not to load
| the active record class
*/

$active_group = 'service';
$active_record = TRUE;


######Manager DB Connect#####################
$db['dev']['hostname'] = '127.0.0.1';
$db['dev']['username'] = 'root';
$db['dev']['password'] = 'root123!';
$db['dev']['database'] = 'ofdp_360';
$db['dev']['dbdriver'] = 'mysqli';
$db['dev']['dbprefix'] = 'op_';
$db['dev']['pconnect'] = TRUE;
$db['dev']['db_debug'] = TRUE;
$db['dev']['cache_on'] = FALSE;
$db['dev']['cachedir'] = '';
$db['dev']['char_set'] = 'utf8';
$db['dev']['dbcollat'] = 'utf8_general_ci';
$db['dev']['swap_pre'] = '';
$db['dev']['autoinit'] = TRUE;
$db['dev']['stricton'] = FALSE;

######Real Service DB Connect#####################
$db['service']['hostname'] = '127.0.0.1';
$db['service']['username'] = 'root';
$db['service']['password'] = 'root123!';
$db['service']['database'] = 'ofdp_360';
$db['service']['dbdriver'] = 'mysqli';
$db['service']['dbprefix'] = 'op_';
$db['service']['pconnect'] = TRUE;
$db['service']['db_debug'] = TRUE;
$db['service']['cache_on'] = FALSE;
$db['service']['cachedir'] = '';
$db['service']['char_set'] = 'utf8';
$db['service']['dbcollat'] = 'utf8_general_ci';
$db['service']['swap_pre'] = '';
$db['service']['autoinit'] = TRUE;
$db['service']['stricton'] = FALSE;



