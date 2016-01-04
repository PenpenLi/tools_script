<?php
ini_set('date.timezone','Asia/Shanghai');

$update_start_time = "";
$proj_src_dir_name = "HiCard/src";
$proj_res_dir_name = "HiCard/res";

$update_pkg_zip_name = "UPDATE_PKG";

function get_dictionary_files($dir) 
{
	$handler = opendir($dir);
	while (($filename = readdir($handler)) !== false) {
		if ($filename != "." && $filename != "..") {
			$files[] = $filename;
		}
	}
	closedir($handler);
	return $files;
}

function get_update_start_time() 
{
	fwrite(STDOUT, "Enter your update start time: (2014-12-17 15:11:20) \n");
	$start_time = strtotime(trim(fgets(STDIN)));
	return $start_time;
}

function get_update_files_path($dir, $start_time, &$path_list) 
{
	if (is_dir($dir) == false) {return;}
	$child_files = get_dictionary_files($dir);
	foreach ($child_files as $key => $filename) {
		$filepath = $dir."/".$filename;
		if (is_dir($filepath)) {
			get_update_files_path($filepath, $start_time, $path_list);
		} else {
			$file_modify_time = filemtime($filepath);
			if ($file_modify_time > $start_time) {
				$path_list[] = $filepath;
			}
			//echo date("Y-m-d H:i:s", $file_modify_time);echo "\n";
		}
	}
}

function remove_dir($dir) { 
   if (is_dir($dir)) { 
     $objects = scandir($dir); 
     foreach ($objects as $object) { 
       if ($object != "." && $object != "..") { 
         if (filetype($dir."/".$object) == "dir") remove_dir($dir."/".$object); else unlink($dir."/".$object); 
       } 
     } 
     reset($objects); 
     rmdir($dir); 
   } 
}

$path_list = [];

$update_start_time = get_update_start_time();

get_update_files_path($proj_src_dir_name, $update_start_time, $path_list);
get_update_files_path($proj_res_dir_name, $update_start_time, $path_list);

$update_pkg_dir_name = "update_output";
remove_dir($update_pkg_dir_name);

//print_r($path_list);
foreach ($path_list as $key => $path) {
	$dest_dir = $update_pkg_dir_name."/".$path;
	if (is_dir(dirname($dest_dir)) == false) {
		mkdir(dirname($dest_dir), 0777, true);
	}
	copy($path, $dest_dir);
}

shell_exec('cocos luacompile -s update_output/HiCard/src/ -d update_output/HiCard/src_luac -e -k beigaofeng -b yuwenxinxi');
shell_exec('rm -fr update_output/HiCard/src/');
shell_exec('mv update_output/HiCard/src_luac update_output/HiCard/src');
echo shell_exec('pwd');
//shell_exec('cd update_output/');
chdir('update_output');
echo shell_exec('pwd');
echo shell_exec('zip -r update_HiCard.zip HiCard');
chdir('..');
echo shell_exec('pwd');


?>