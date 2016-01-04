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

function my_print(&$output_str, $str)
{
	$output_str .= $str;
}

function my_println(&$output_str, $str)
{
	$output_str .= $str . "\n";
}

function createCPPWithJson($json_file_name)
{
	$ui_json_str = file_get_contents($json_file_name);

	$ui = json_decode($ui_json_str, true);

	$root_node = $ui["widgetTree"];

	// output *.cpp *.h name
	
	$start_pos = strrpos($json_file_name, "/") + 1;
	$end_pos = strrpos($json_file_name, ".json");
	$cpp_class_name = substr($json_file_name, $start_pos, $end_pos - $start_pos);
	//print($json_file_name . " --- " . $cpp_class_name . "\n");

	$export_cpp_file_name = $cpp_class_name . ".cpp";

	$export_header_file_name = $cpp_class_name . ".h";

	$export_cpp_file = fopen("cpp_output/".$export_cpp_file_name, "w");

	$export_header_file = fopen("cpp_output/".$export_header_file_name, "w");

	$cpp_file_output = "";

	// add header content to output string
	my_println($header_file_output, "//// " . $export_header_file_name . " created by php script. chris.li");

	// 1 - header start: macrio define and include other header files
	my_println($header_file_output, "#ifndef __HiFly__$cpp_class_name");

	my_println($header_file_output, "#define __HiFly__$cpp_class_name");

	my_println($header_file_output, '');

	my_println($header_file_output, '#include "cocos2d.h"');

	my_println($header_file_output, '#include "extensions/cocos-ext.h"');

	my_println($header_file_output, '#include "cocostudio/CocoStudio.h"');

	my_println($header_file_output, '#include "ui/CocosGUI.h"');

	my_println($header_file_output, "");

	my_println($header_file_output, "using namespace cocos2d;");

	my_println($header_file_output, "using namespace cocostudio;");

	my_println($header_file_output, "using namespace ui;");

	my_println($header_file_output, "");

	// 2 - class start: class define start
	my_println($header_file_output, "class $cpp_class_name : public cocos2d::Layer");

	my_println($header_file_output, "{");

	my_println($header_file_output, "public:");

	my_println($header_file_output, "\tCREATE_FUNC($cpp_class_name);");

	my_println($header_file_output, "private:");

	my_println($header_file_output, "\tbool init();");

	my_println($header_file_output, "\tvoid onEnter();");

	my_println($header_file_output, "\tvoid onExit();");

	my_println($header_file_output, "\tvoid loadUI();");

	my_println($header_file_output, "\tvoid handleTouchEvent(Ref *pSender, TouchEventType type);");

	my_println($header_file_output, "");

	my_println($header_file_output, "\t//get ui reference marked by ui_xxx ");

	// add cpp content to output string
	// cpp: include header.h
	my_println($cpp_file_output, "////$export_cpp_file_name created by php script. chris.li");

	my_println($cpp_file_output, "#include \"$export_header_file_name\"");

	my_println($cpp_file_output, "");

	// cpp: print init() function
	my_println($cpp_file_output, "bool $cpp_class_name::init()"); 
	my_println($cpp_file_output, "{");
	my_println($cpp_file_output, "\tif (Layer::init())");
	my_println($cpp_file_output, "\t{");
	my_println($cpp_file_output, "\t\tloadUI();");
	my_println($cpp_file_output, "\t\treturn true;");
	my_println($cpp_file_output, "\t}");
	my_println($cpp_file_output, "");
	my_println($cpp_file_output, "\treturn false;");
	my_println($cpp_file_output, "}");

	// cpp: print onEnter() function
	my_println($cpp_file_output, "void $cpp_class_name::onEnter()"); 
	my_println($cpp_file_output, "{");
	my_println($cpp_file_output, "\tLayer::onEnter();");
	my_println($cpp_file_output, "}");
	my_println($cpp_file_output, "");

	// cpp: print onExit() function
	my_println($cpp_file_output, "void $cpp_class_name::onExit()"); 
	my_println($cpp_file_output, "{");
	my_println($cpp_file_output, "\tLayer::onExit();");
	my_println($cpp_file_output, "}");
	my_println($cpp_file_output, "");

	$loadUI_func_str = "";
	my_println($loadUI_func_str, "\tauto rootNode = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(\"$cpp_class_name/$cpp_class_name.json\"));");
	my_println($loadUI_func_str, "");
	my_println($loadUI_func_str, "\tthis->addChild(rootNode);");
	my_println($loadUI_func_str, "");

	$handleTouchEvent_str = "";

	my_println($handleTouchEvent_str, "\tif (pSender == nullptr)");
	my_println($handleTouchEvent_str, "\t{");
	my_println($handleTouchEvent_str, "\t}");
	my_println($handleTouchEvent_str, "");

	$node_queue = array();

	array_push($node_queue, $root_node);

	while (count($node_queue) > 0)
	{
		$node = array_shift($node_queue);

		if ($node) 
		{
			$node_classname = $node["options"]["classname"];

			// replace Label to Text
			if ("Label" == $node_classname)
			{
				$node_classname = "Text";
			}

			if ("Panel" == $node_classname)
			{
				$node_classname = "Layout";
			}

			if ("LabelAtlas" == $node_classname)
			{
				$node_classname = "TextAtlas";
			}

			$node_name = $node["options"]["name"];

			// only process node with name containing "m_" header
			if (substr($node_name, 0, 2) == "m_" || true) 
			{
				my_println($header_file_output, "\t$node_classname* _$node_name;");

				my_println($header_file_output, "");

				my_println($loadUI_func_str, "\t_$node_name = static_cast<$node_classname*>(ui::Helper::seekWidgetByName(rootNode, \"$node_name\"));");	
				my_println($loadUI_func_str, "");

				if ($node_classname == "Button" || 
					$node_classname == "Layout" || 
					$node_classname == "ScrollView" || 
					$node_classname == "ListView" || 
					$node_classname == "PageView" ||
					$node_classname == "CheckBox" ||
					$node_classname == "ProgressBar" ||
					$node_classname == "Slider" ||
					$node_classname == "TextField")
				{
					my_println($loadUI_func_str, "\t_$node_name-> addTouchEventListener(this, toucheventselector($cpp_class_name::handleTouchEvent));");
					my_println($loadUI_func_str, "");

					my_println($handleTouchEvent_str, "\telse if (pSender == _$node_name && (TOUCH_EVENT_ENDED == type))");
					my_println($handleTouchEvent_str, "\t{");
					my_println($handleTouchEvent_str, "\t}");
					my_println($handleTouchEvent_str, "");
				}
			}
		}

		// add children to queue
		$children = $node["children"];

		foreach ($children as $child) 
		{
			array_push($node_queue, $child);
		}
	}

	// cpp: loadUI function
	my_println($cpp_file_output, "void $cpp_class_name::loadUI()");
	my_println($cpp_file_output, "{");
	my_println($cpp_file_output, $loadUI_func_str);
	my_println($cpp_file_output, "}");
	my_println($cpp_file_output, "");

	// cpp: handleTouchEvent
	my_println($cpp_file_output, "void $cpp_class_name::handleTouchEvent(Ref *pSender, TouchEventType type)");
	my_println($cpp_file_output, "{");
	my_println($cpp_file_output, $handleTouchEvent_str);
	my_println($cpp_file_output, "}");
	my_println($cpp_file_output, "");

	// 2- class end: class define end
	my_println($header_file_output, "};");

	// 1 - header end: macrio define end
	my_println($header_file_output, "#endif");

	fwrite($export_header_file, $header_file_output);

	fwrite($export_cpp_file, $cpp_file_output);

	fclose($export_header_file);

	fclose($export_cpp_file);

} // end function createCPP


$jsonFiles = getDictionaryFile("json", ".json");

foreach ($jsonFiles as $tmpFile) {
	createCPPWithJson("json/$tmpFile");
}

?>