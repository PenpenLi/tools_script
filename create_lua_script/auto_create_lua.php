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

function createLUAWithJson($json_file_name)
{
	$ui_json_str = file_get_contents($json_file_name);

	$ui = json_decode($ui_json_str, true);

	$root_node = $ui["widgetTree"];

	
	$start_pos = strrpos($json_file_name, "/") + 1;
	$end_pos = strrpos($json_file_name, ".ExportJson");
	$lua_class_name = substr($json_file_name, $start_pos, $end_pos - $start_pos);

	$export_lua_file_name = $lua_class_name . ".lua";
	$export_lua_file = fopen("lua_output/$export_lua_file_name", "w");


	$lua_file_content = "";
	my_println($lua_file_content, "--$export_lua_file_name, created by php script. chris.li--");
	my_println($lua_file_content, 'require "Cocos2d"');
	my_println($lua_file_content, "");

	my_println($lua_file_content, "local $lua_class_name = class(\"$lua_class_name\", function()");
	my_println($lua_file_content, "\treturn cc.Layer:create()");
	my_println($lua_file_content, 'end)');
	my_println($lua_file_content, "");

	my_println($lua_file_content, "function $lua_class_name:create()");
	my_println($lua_file_content, "\tlocal layer = $lua_class_name.new()");
	my_println($lua_file_content, "\tlayer:initM()");
	my_println($lua_file_content, "\treturn layer");
	my_println($lua_file_content, "end");
	my_println($lua_file_content, "");

	my_println($lua_file_content, "function $lua_class_name:ctor()");
	my_println($lua_file_content, "end");
	my_println($lua_file_content, "");

	my_println($lua_file_content, "function $lua_class_name:onEnter()");
	my_println($lua_file_content, "end");
	my_println($lua_file_content, "");

	my_println($lua_file_content, "function $lua_class_name:onExit()");
	my_println($lua_file_content, "end");
	my_println($lua_file_content, "");

	$loadUI_func_str = "";

	//register onEnter and onExit event
	my_println($loadUI_func_str, "\tlocal function onNodeEvent(event)");
	my_println($loadUI_func_str, "\t\tif event == \"enter\" then self:onEnter() ");
	my_println($loadUI_func_str, "\t\telseif event == \"exit\" then self:onExit() end");
	my_println($loadUI_func_str, "\tend");
	my_println($loadUI_func_str, "\tself:registerScriptHandler(onNodeEvent)");
	my_println($loadUI_func_str, "");

	//binding UI control
	my_println($loadUI_func_str, "\tself.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile(\"$lua_class_name.csb\")");
	my_println($loadUI_func_str, "\tself:addChild(self.rootNode)");
	my_println($loadUI_func_str, "");

	my_println($loadUI_func_str, "\tlocal function touchEventHandler(sender, event)");
	my_println($loadUI_func_str, "\t\tself:handleTouchEvent(sender, event)");
	my_println($loadUI_func_str, "\tend");
	my_println($loadUI_func_str, "");

	$handleTouchEvent_str = "";

	my_println($handleTouchEvent_str, "\tif sender == nil then");
	my_println($handleTouchEvent_str, "\t\tcclog(\"[ERR]unknown sender\")");
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
			if (substr($node_name, 0, 2) == "m_") 
			{
				my_println($loadUI_func_str, "\t--$node_name: $node_classname");
				my_println($loadUI_func_str, "\tself.$node_name = ccui.Helper:seekWidgetByName(self.rootNode, \"$node_name\")");
				//my_println($loadUI_func_str, "");

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
					my_println($loadUI_func_str, "\tself.$node_name:addTouchEventListener(touchEventHandler)");
					//my_println($loadUI_func_str, "");

					my_println($handleTouchEvent_str, "\telseif sender == self.$node_name and event == cc.EventCode.ENDED then");
					my_println($handleTouchEvent_str, "\t\tcclog(\"[$lua_class_name]--$node_name touched--\")");
					my_println($handleTouchEvent_str, "");
				}

				my_println($loadUI_func_str, "");

			}
		}

		// add children to queue
		$children = $node["children"];

		foreach ($children as $child) 
		{
			array_push($node_queue, $child);
		}
	}

	my_println($handleTouchEvent_str, "\tend");
	$handleTouchEvent_str = substr($handleTouchEvent_str, 0, strlen($handleTouchEvent_str)-1);

	// lua: initM function
	my_println($lua_file_content, "function $lua_class_name:initM()");
	$loadUI_func_str = substr($loadUI_func_str, 0, strlen($loadUI_func_str)-1);
	my_println($lua_file_content, $loadUI_func_str);
	my_println($lua_file_content, "\tself:initUI()");
	my_println($lua_file_content, "end");
	my_println($lua_file_content, "");

	// lua: handleTouchEvent
	my_println($lua_file_content, "function $lua_class_name:handleTouchEvent(sender, event)");
	my_println($lua_file_content, $handleTouchEvent_str);
	my_println($lua_file_content, "end");
	my_println($lua_file_content, "");

	// lua: initUI
	my_println($lua_file_content, "--init your ui here");
	my_println($lua_file_content, "function $lua_class_name:initUI()");
	my_println($lua_file_content, "end");
	my_println($lua_file_content, "");

	// lua:refreshUI
	my_println($lua_file_content, "--refresh all ui here");
	my_println($lua_file_content, "function $lua_class_name:refreshUI()");
	my_println($lua_file_content, "end");
	my_println($lua_file_content, "");

	my_println($lua_file_content, "return $lua_class_name");

	fwrite($export_lua_file, $lua_file_content);
	fclose($export_lua_file);

} // end function createLUA


$jsonFiles = getDictionaryFile("json", ".ExportJson");

foreach ($jsonFiles as $tmpFile) {
	createLUAWithJson("json/$tmpFile");
}

?>