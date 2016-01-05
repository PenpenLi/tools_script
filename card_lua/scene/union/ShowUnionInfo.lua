--ShowUnionInfo.lua, created by php script. chris.li--
require "Cocos2d"

local ShowUnionInfo = class("ShowUnionInfo", function()
	return cc.Layer:create()
end)

function ShowUnionInfo:create()
	local layer = ShowUnionInfo.new()
	layer:initM()
	return layer
end

function ShowUnionInfo:ctor()
end

function ShowUnionInfo:onEnter()
end

function ShowUnionInfo:onExit()
end

function ShowUnionInfo:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("ShowUnionInfo.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_ShowUnionInfo: ImageView
	self.m_ShowUnionInfo = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShowUnionInfo")

	--m_UnionId: Text
	self.m_UnionId = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionId")

	--m_UnionPeopleNum: Text
	self.m_UnionPeopleNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionPeopleNum")

	--m_UnionLeaderName: Text
	self.m_UnionLeaderName = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionLeaderName")

	--m_UnionLimitNum: Text
	self.m_UnionLimitNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionLimitNum")

	--m_UnionLevelNum: Text
	self.m_UnionLevelNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionLevelNum")

	--m_ApplyUnionBtn: Button
	self.m_ApplyUnionBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_ApplyUnionBtn")
	self.m_ApplyUnionBtn:addTouchEventListener(touchEventHandler)

	--m_UnionPeopleFull: Button
	self.m_UnionPeopleFull = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionPeopleFull")
	self.m_UnionPeopleFull:addTouchEventListener(touchEventHandler)

	self:initUI()
end

function ShowUnionInfo:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_ApplyUnionBtn and event == cc.EventCode.ENDED then
		cclog("[ShowUnionInfo]--m_ApplyUnionBtn touched--")

	elseif sender == self.m_UnionPeopleFull and event == cc.EventCode.ENDED then
		cclog("[ShowUnionInfo]--m_UnionPeopleFull touched--")

	end
end

--init your ui here
function ShowUnionInfo:initUI()
end

--refresh all ui here
function ShowUnionInfo:refreshUI()
end

return ShowUnionInfo
