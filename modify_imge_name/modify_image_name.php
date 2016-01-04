<?php


function getDictionaryFile($dir, $search) {
	$handler = opendir($dir);
	while (($filename = readdir($handler)) !== false) {
		if ($filename != "." && $filename != ".." && strpos($filename, $search)) {
			$files[] = $filename;
		}
	}
	closedir($handler);
	return $files;
}

function getOnlyFileName($fileName, $postfix) {
	$start_pos = 0;
	$end_pos = strrpos($fileName, $postfix);
	$onlyFileName = substr($fileName, $start_pos, $end_pos - $start_pos);
	return $onlyFileName;
}

function renamePngFile($dir, $postfix, &$allPngFileNameArray) {
	$handler = opendir($dir);
	while (($filename = readdir($handler)) !== false) {
		if (strpos($filename, ".png")) {
			array_push($allPngFileNameArray, $filename);
			$newfilename = str_replace(".png", $postfix, $filename);
			rename($dir."/".$filename, $dir."/".$newfilename);
		}
		if ($filename != "." && $filename != ".." && is_dir($dir."/".$filename)) {
			renamePngFile($dir."/".$filename, $postfix, $allPngFileNameArray);
		}
	}
	closedir($handler);
}


//0. postfix
$imagePostfix = "_".basename(dirname(__FILE__)).".png";
$jsonContentPostfix = "_".basename(dirname(__FILE__));

//1. modify png file name
$allPngFileNameArray = array();
renamePngFile("Resources", $imagePostfix, $allPngFileNameArray);

//2. replace postfix in json file
$jsonFileNames = getDictionaryFile("Json", ".json");
foreach ($jsonFileNames as $tmpFileName) {
	$fileStr = file_get_contents("Json/".$tmpFileName);
	//$fileStr = utf8_encode($fileStr);
	//$encode = mb_detect_encoding($fileStr, array(GB2312,UTF-8, GBK));
	//echo $encode;
	foreach ($allPngFileNameArray as $key => $value) {
		$onlyFileName = getOnlyFileName($value, ".png");
		$encode = mb_detect_encoding($onlyFileName, array("GB2312","GBK","UTF-8"));
		//echo "____________________".$encode."____________________\n";
		if($encode=="GB2312")
		{	
    		$onlyFileName = iconv("GB2312","UTF-8",$onlyFileName);
		}
		else if($encode=="GBK")
		{
    		$onlyFileName = iconv("GBK","UTF-8",$onlyFileName);
		}
		else if($encode=="EUC-CN")
		{
    		$onlyFileName = iconv("EUC-CN","UTF-8",$onlyFileName);
		}
		else//CP936
		{
    		//$q = iconv("GB2312","UTF-8",$q);
		}

		echo $onlyFileName." --> ".$onlyFileName.$jsonContentPostfix."\n";
		$fileStr = str_replace('"'.$onlyFileName.'"', '"'.$onlyFileName.$jsonContentPostfix.'"', $fileStr);
		$fileStr = str_replace('"'.$onlyFileName.".png".'"', '"'.$onlyFileName.$jsonContentPostfix.".png".'"', $fileStr);

		$fileStr = str_replace('/'.$onlyFileName.".png".'"', '/'.$onlyFileName.$jsonContentPostfix.".png".'"', $fileStr);
		$fileStr = str_replace('/'.$onlyFileName.'"', '/'.$onlyFileName.$jsonContentPostfix.'"', $fileStr);
	}
	$fileHandle = fopen("Json/".$tmpFileName, "w");
	fwrite($fileHandle, $fileStr);
	fclose($fileHandle);
}

//echo str_replace("傻逼", "超级笨蛋"."__反复", "傻逼.png");

?>

