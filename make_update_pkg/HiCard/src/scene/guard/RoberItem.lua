--RoberItem.lua, created by php script. chris.li--
require "Cocos2d"

local RoberItem = class("RoberItem", function()
	return cc.Layer:create()
end)

function RoberItem:create()
	local layer = RoberItem.new()
	layer:initM()
	return layer
end

function RoberItem:ctor()
end

function RoberItem:onEnter()
end

function RoberItem:onExit()
end

function RoberItem:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("RoberItem.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_RoberItem: ImageView
	self.m_RoberItem = ccui.Helper:seekWidgetByName(self.rootNode, "m_RoberItem")

	--m_Win: ImageView
	self.m_Win = ccui.Helper:seekWidgetByName(self.rootNode, "m_Win")

	--m_Failed: ImageView
	self.m_Failed = ccui.Helper:seekWidgetByName(self.rootNode, "m_Failed")

	--m_RoberName: Text
	self.m_RoberName = ccui.Helper:seekWidgetByName(self.rootNode, "m_RoberName")

	--m_Time: Text
	self.m_Time = ccui.Helper:seekWidgetByName(self.rootNode, "m_Time")

	--m_fuwuqi: Text
	self.m_fuwuqi = ccui.Helper:seekWidgetByName(self.rootNode, "m_fuwuqi")

	--m_baoxiang: ImageView
	self.m_baoxiang = ccui.Helper:seekWidgetByName(self.rootNode, "m_baoxiang")

	--m_bofang: ImageView
	self.m_bofang = ccui.Helper:seekWidgetByName(self.rootNode, "m_bofang")

	self:initUI()
end

function RoberItem:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	end
end

--init your ui here
function RoberItem:initUI()
end

--refresh all ui here
function RoberItem:refreshUI()
end

return RoberItem
