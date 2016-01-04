<?php
ini_set('pcre.backtrack_limit', 999999999);
define("IN_PATH", "./in/");
define("OUT_PATH", "./out/");

$HEADER_RULES = array(
	'/#import/i' => '#include', 

	// interface to class
	'/@interface\s+(\w+)\s*:\s*(\w+)/i' => 'class ${1} : public ${2} {public:',

	// protocol
	'/@protocol\s+(\s*\w+\s*)\s+[<](\s*\w+\s*)[>]/i' => '',

	// function with 0 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*;/i' => '    ${1} ${2}();',

	// function with 1 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*:\s*\((\s*\w+\s*[*]{0,1})\)\s*(\s*\w+\s*[*]{0,1})\s*;/i' => '    ${1} ${2}(${3} ${4});',

	// function with 2 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*:\s*\((\s*\w+\s*[*]{0,1})\)\s*(\s*\w+\s*[*]{0,1})\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*);/i' => '    ${1} ${2}(${3} ${4}, ${5} ${6});',

	// function with 3 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*:\s*\((\s*\w+\s*[*]{0,1})\)\s*(\s*\w+\s*[*]{0,1})\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*);/i' => '    ${1} ${2}(${3} ${4}, ${5} ${6}, ${7} ${8});',

	// function with 4 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*:\s*\((\s*\w+\s*[*]{0,1})\)\s*(\s*\w+\s*[*]{0,1})\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s*;/i' => '    ${1} ${2}(${3} ${4}, ${5} ${6}, ${7} ${8}, ${9} ${10});',

	// function with 5 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*:\s*\((\s*\w+\s*[*]{0,1})\)\s*(\s*\w+\s*[*]{0,1})\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s*;/i' => '    ${1} ${2}(${3} ${4}, ${5} ${6}, ${7} ${8}, ${9} ${10}, ${11} ${12});',

	// property
	'/@property\s+(\s*\w+\s*[*]{0,1}\s*)\s+(\s*\w+\s*);/i' => '    ${1} ${2};',

	'/@end/i' => '};',

		// BOOL to bool
	'/([^a-zA-Z_0-9])BOOL([^a-zA-Z_0-9])/i' => '${1}bool${2}',

	// nil to NULL
	'/([^a-zA-Z_0-9])nil([^a-zA-Z_0-9])/i' => '${1}NULL${2}',

	// YES to true, NO to false
	'/([^a-zA-Z_0-9])YES([^a-zA-Z_0-9])/i' => '${1}true${2}',
	'/([^a-zA-Z_0-9])NO([^a-zA-Z_0-9])/i' => '${1}false${2}',

	'/([^a-zA-Z_0-9])CGSizeZero([^a-zA-Z_0-9])/i' => '${1}Size(0, 0)${2}',
	'/([^a-zA-Z_0-9])CGPoint([^a-zA-Z_0-9])/i' => '${1}Point${2}',
	'/([^a-zA-Z_0-9])CGSizeMake([^a-zA-Z_0-9])/i' => '${1}Size${2}',
	'/([^a-zA-Z_0-9])CGPointZero([^a-zA-Z_0-9])/i' => '${1}Point(0, 0)${2}',

	// NSString *xx to std::string xx
	'/[^a-zA-Z_0-9]NSString\s*\*\s*(\w+)/' => 'std::string ${1}',

	'/[^a-zA-Z_0-9]NSArray\s*\*\s*(\w+)/' => 'Vector<T>* ${1}',

	//'' => '',
	//'' => '',
	//'' => '',
	//'' => ''
);


$OC_RULES = array(

	'/#import/i' => '#include',

	'/#pragma\s*\w+.*/i' => '',

	// interface to class
	'/@interface\s+(\w+)\s*:\s*(\w+)/i' => 'class ${1} : public ${2} {public:',

	// protocol
	'/@protocol\s+(\s*\w+\s*)\s+[<](\s*\w+\s*)[>]/i' => '',

	// property
	'/@property\s+(\s*\w+\s*[*]{0,1}\s*)\s+(\s*\w+\s*);/i' => '    ${1} ${2};',

	'/@implementation \s*Level\s*/i' => "",



	// function with 0 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*[{]{0,1}\s*([\n])/i' => '${1} ___className___::${2}() ${3}{${3}',

	// function with 1 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*:\s*\((\s*\w+\s*[*]{0,1})\)\s*(\s*\w+\s*)\s*[{]{0,1}\s*([\n])/i' => '${1} ___className___::${2}($3 $4) ${5}{${5}',

	// function with 2 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*:\s*\((\s*\w+\s*[*]{0,1})\)\s*(\s*\w+\s*[*]{0,1})\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s*[{]{0,1}\s*([\n])/i' => '${1} ___className___::${2}($3 $4, $5 $6) ${7} {${7}',

	// function with 3 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*:\s*\((\s*\w+\s*[*]{0,1})\)\s*(\s*\w+\s*[*]{0,1})\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s*[{]{0,1}\s*([\n])/i' => '${1} ___className___::${2}($3 $4, $5 $6, $7 $8) ${9} {${9}',

	// function with 4 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*:\s*\((\s*\w+\s*[*]{0,1})\)\s*(\s*\w+\s*[*]{0,1})\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s*[{]{0,1}\s*([\n])/i' => '${1} ___className___::${2}($3 $4, $5 $6, $7 $8, $9 $10) ${11} {${11}',

	// function with 5 parameter
	'/[+-]\((\s*\w+\s*[*]{0,1})\)(\s*\w+\s*)\s*:\s*\((\s*\w+\s*[*]{0,1})\)\s*(\s*\w+\s*[*]{0,1})\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s+\w+\s*:\s*\((\s*\w+\s*[*]{0,1}\s*)\)(\s*\w+\s*)\s*[{]{0,1}\s*([\n])/i' => '${1} ___className___::${2}($3 $4, $5 $6, $7 $8, $9 $10, $11 $12) ${13} {${13}',


	
	// NSString to std::string
	'/\[\[NSString\s+alloc]initWithString:\s*(NSLocalizedString)\s*\(@"(\w+)"\s*,\s*nil\s*\)\s*\]/i' => '${1}("${2}")',

	// NSString format to StringUitl::format
	'/\[NSString stringWithFormat\s*:\s*@"(.+)"\s*,\s*(.+)\s*\]/i' => 'StringUtils::format("${1}", ${2})',

	'/\[\[\[\[\[(\w+)\s+(\w+)\](\w+):@"(\w+)"\](\w+):@"(\w+)"\](\w+):@"(\w+)"\](\w+)\]/' => '${1}::${2}()->${3}("${4}")->${5}("${6}")->${7}("${8}")->${9}()',

	// this item has bug!!!!
	'/\s*\[\[\s*(\w+)\s+(\w+)\s*:\s*\[(\w+)\s+(\w+)\]\](\w+)\]/i' => "${1}::${2}(${3}->${4}())->${5}()",

	'/\[\[(\w+) alloc](\w+):(.+)\]/' => '${1}::${2}(${3})',

	'/\[\[\[(\w+)\s+alloc]init]autorelease]/' => '${1}::create()',

	// CCxxx instance
	'/\[\[([A-Z])(\w+)\s+(\w+)+\]\s*(\w+)\s*:\s*(.+)\s*\]/' => '${1}${2}::${3}()->${4}(${5})',

	// instance with 0 param
	'/\[\[([A-Z])(\w+)\s+(\w+)+\]\s*(\w+)\s*\]/' => '${1}${2}->${3}()->${4}(${5})',

	// Factory function
	'/\[([A-Z])(\w+)\s+(\w+)\s*:\s*(.+)\s*\]/' => '${1}${2}::${3}(${4})',

	'/\[([A-Z])(\w*)\s+(\w+)\s*\]/' => '${1}${2}::${3}()',

	//[[a b]c:xxx]
	'/\[\[([a-z])(\w+)\s+(\w+)\s*\](\w+)\s*:(.+)\]/' => '${1}${2}->${3}()->${4}(${5})',

	// [this xxx] to this->xxx()
	'/\[([a-z_])(\w+)\s+(\w+)\s*\]/' => '${1}${2}->${3}(${4})',

	// [a b:xx]
	'/\[([a-z_])(\w+)\s+(\w+)\s*:\s*(.+)\s*\]/' => '${1}${2}->${3}(${4})',

	// (a, para1:b, para2:c)
	'/\((.+)\s+\w+\s*:(.+)\s+\w+\s*\w+:\s*(.+)\)/' => '(${1}, ${2}, ${3})',

	// (a, para1:b)
	'/\((.+)\s+\w+\s*:(.+)\s*\)/' => '(${1}, ${2})',


	'/[^a-zA-Z_0-9](\w+)\.contentSize\s+=\s*(.+)\s*;/' => '${1}->setContentSize(${2});',
	'/[^a-zA-Z_0-9](\w+)\.rotation\s+=\s*(.+)\s*;/' => '${1}->setRotation(${2});',
	'/[^a-zA-Z_0-9](\w+)\.anchorPoint\s+=\s*(.+)\s*;/' => '${1}->setAnchorPoint(${2});',
	'/[^a-zA-Z_0-9](\w+)\.position\s+=\s*(.+)\s*;/' => '${1}->setPosition(${2});',
	'/[^a-zA-Z_0-9](\w+)\.scale\s+=\s*(.+)\s*;/' => '${1}->setScale(${2});',
	'/[^a-zA-Z_0-9](\w+)\.blendFunc\s+=\s*(.+)\s*;/' => '${1}->setBlendFunc(${2});',
	'/[^a-zA-Z_0-9](\w+)\.opacity\s+=\s*(.+)\s*;/' => '${1}->setOpacity(${2});',
	'/[^a-zA-Z_0-9](\w+)\.autoRemoveOnFinish\s+=\s*(.+)\s*;/' => '${1}->setAutoRemoveOnFinish(${2});',



	'/(\w+)\.contentSize/' => '${1}->getContentSize()',
	'/(\w+)\.rotation/' => '${1}->getRotation()',
	'/(\w+)\.anchorPoint/' => '${1}->getAnchorPoint()',
	'/(\w+)\.position/' => '${1}->getPosition()',
	'/(\w+)\.scale/' => '${1}->getScale()',
	'/(\w+)\.blendFunc/' => '${1}->getBlendFunc()',
	'/(\w+)\.opacity/' => '${1}->getOpacity()',
	'/(\w+)\.autoRemoveOnFinish/' => '${1}->getAutoRemoveOnFinish()',

	// a.size = size to set function
	//'/[^a-zA-Z_0-9](\w+)\.(\w+)\s*=\s*(.+)\s*;/' => '${1}->set_${2}(${3});',

	// a.scale to  a->getScale()
	// a.contentSize to a->getContentSize()

	//CallBlock to CallFunc
	'/CCCallBlock \s*\*\s*(\w+).+:\^{/' => 'CallFunc *${1} = CallFunc::create([this]() {',
	'/}]/' => '})',

	'/@"/' => '"',

	'/CC(\w+)/' => '${1}',

	// self to this
	'/([^a-zA-Z_0-9])self([^a-zA-Z_0-9])/i' => '${1}this${2}',

	// BOOL to bool
	'/([^a-zA-Z_0-9])BOOL([^a-zA-Z_0-9])/i' => '${1}bool${2}',

	// nil to NULL
	'/([^a-zA-Z_0-9])nil([^a-zA-Z_0-9])/i' => '${1}NULL${2}',

	// YES to true, NO to false
	'/([^a-zA-Z_0-9])YES([^a-zA-Z_0-9])/i' => '${1}true${2}',
	'/([^a-zA-Z_0-9])NO([^a-zA-Z_0-9])/i' => '${1}false${2}',

	'/([^a-zA-Z_0-9])CGSizeZero([^a-zA-Z_0-9])/i' => '${1}Size(0, 0)${2}',
	'/([^a-zA-Z_0-9])CGPoint([^a-zA-Z_0-9])/i' => '${1}Point${2}',
	'/([^a-zA-Z_0-9])CGSizeMake([^a-zA-Z_0-9])/i' => '${1}Size${2}',
	'/([^a-zA-Z_0-9])CGPointZero([^a-zA-Z_0-9])/i' => '${1}Point(0, 0)${2}',
	'/([^a-zA-Z_0-9])spriteWithSpriteFrameName([^a-zA-Z_0-9])/i' => '${1}createWithSpriteFrameName${2}',
	'/([^a-zA-Z_0-9])actionWithDuration([^a-zA-Z_0-9])/i' => '${1}create${2}',
	'/([^a-zA-Z_0-9])actionWithAction([^a-zA-Z_0-9])/i' => '${1}create${2}',
	'/::actions\(/i' => '::create(',



	'/@end/' => '',

	// NSString *xx to std::string xx
	'/[^a-zA-Z_0-9]NSString\s*\*\s*(\w+)/' => 'std::string ${1}',

	// NSSArray to Vector<T>
	'/[^a-zA-Z_0-9]NSMutableArray\s*\*(\w+)\s*=\s*NSMutableArray::.*;/' => ' auto ${1} = new Vector<T>;',

	'/([^a-zA-Z_0-9])addObject([^a-zA-Z_0-9])/i' => '${1}pushBack${2}',

	'/[^a-zA-Z_0-9]NSArray\s*\*\s*(\w+)/' => 'Vector<T>* ${1}',

	// self.xxx to this->xxx
	'/([^a-zA-Z_0-9])this.(\w+)/' => '${1}this->${2}',




	// [[a b]c]
	//'/\[\[(\s*\w+\s*)\s+(\s*\w+\s*)\](\s*\w+\s*)\]/i' => '${1}->${2}()->${3}()',

	// [[[a b]c]d]
	//'/\[\[\[(\s*\w+\s*)\s+(\s*\w+\s*)\](\s*\w+\s*)\](\s*\w+\s*)\]/i' => '${1}->${2}()->${3}()->${4}()',

	// [[[[a b]c]d]e]
	//'/\[\[\[\[(\s*\w+\s*)\s+(\s*\w+\s*)\](\s*\w+\s*)\](\s*\w+\s*)\](\s*\w+\s*)\]/i' => '${1}->${2}()->${3}()->${4}()->${5}'

	// 
);

function getDictionaryFile($dir, $postfix) 
{
	$handler = opendir($dir);

	while (($filename = readdir($handler)) !== false) 
	{
		if ($filename != "." && $filename != ".." && strpos($filename, $postfix)) 
		{
			$files[] = $filename;
		}
	}

	closedir($handler);

	return $files;
}

function getOnlyFileName($fileName, $postfix) 
{
	$start_pos = 0;

	$end_pos = strrpos($fileName, $postfix);

	$onlyFileName = substr($fileName, $start_pos, $end_pos - $start_pos);

	return $onlyFileName;
}

function processHeaderFileContent($headerRules, $headerFileContent, $className)
{
	foreach ($headerRules as $key => $value) 
	{
		//print_r($key);print_r('\n');print_r($value);
		$headerFileContent = preg_replace($key, $value, $headerFileContent);
	}

	$headerFileContent = preg_replace("/___className___/i", $className, $headerFileContent);
	$headerFileContent = preg_replace("/[^a-zA-Z_0-9]id\s+/", "static ".$className."* ", $headerFileContent);

	return $headerFileContent;
}

function processOCFileContent($ocRules, $ocFileContent, $className)
{
	foreach ($ocRules as $key => $value) 
	{
		//print_r($key);print_r('\n');print_r($value);
		$ocFileContent = preg_replace($key, $value, $ocFileContent);
	}
	$ocFileContent = preg_replace("/___className___/i", $className, $ocFileContent);
	$ocFileContent = preg_replace("/[^a-zA-Z_0-9]id\s+/", $className."* ", $ocFileContent);

	return $ocFileContent;
}



// get all .h file names
$headerFileNames = getDictionaryFile(IN_PATH, ".h");
//var_dump($headerFileNames);

foreach ($headerFileNames as $key => $headerFileName) 
{
	// 1. process this header file
	//print_r(getOnlyFileName($headerFileName, ".h") . "\n");
	$outHeaderFileName = $headerFileName;
	$outCPPFileName = "";

	$outHeaderFileContent = "";
	$headerFileContent = file_get_contents(IN_PATH . $headerFileName);
	//print_r($headerFileContent);
	$outHeaderFileContent = processHeaderFileContent($HEADER_RULES, $headerFileContent, getOnlyFileName($headerFileName, ".h"));
	//print_r($outHeaderFileContent);
	$outHeaderFile = fopen(OUT_PATH . $outHeaderFileName, "w");
	fwrite($outHeaderFile, $outHeaderFileContent);
	fclose($outHeaderFile);


	// 2. process this oc file
	$ocFileName = getOnlyFileName($headerFileName, ".h") . ".mm";
	if (file_exists(IN_PATH . $ocFileName) == false)
	{
		$ocFileName = getOnlyFileName($headerFileName, ".h") . ".m";
	}

	if (file_exists(IN_PATH . $ocFileName) == true)
	{
		$outCPPFileName = getOnlyFileName($headerFileName, ".h") . ".cpp";
		$outCPPFileContent = "";

		$ocFileContent = file_get_contents(IN_PATH . $ocFileName);
		//print_r($ocFileContent);

		$outCPPFileContent = processOCFileContent($OC_RULES, $ocFileContent, getOnlyFileName($headerFileName, ".h"));
		//print_r($outCPPFileContent);
		$outOCFile = fopen(OUT_PATH . $outCPPFileName, "w");
		fwrite($outOCFile, $outCPPFileContent);
		fclose($outOCFile);
	}
}

?>