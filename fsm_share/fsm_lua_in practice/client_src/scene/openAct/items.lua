--items.lua, created by php script. chris.li--
require "Cocos2d"

local items = class("items", function()
	return cc.Layer:create()
end)

function items:create()
	local layer = items.new()
	layer:initM()
	return layer
end

function items:ctor()
end

function items:onEnter()
end

function items:onExit()
end

function items:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("items.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_paimingNum: TextAtlas
	self.m_paimingNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_paimingNum")

	--m_rechargeLable: Text
	self.m_rechargeLable = ccui.Helper:seekWidgetByName(self.rootNode, "m_rechargeLable")

	--m_receiveBtn: Button
	self.m_receiveBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_receiveBtn")
	self.m_receiveBtn:addTouchEventListener(touchEventHandler)

	--m_notPassBtn: Button
	self.m_notPassBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_notPassBtn")
	self.m_notPassBtn:addTouchEventListener(touchEventHandler)

	--m_alreadyReceiveBtn: Button
	self.m_alreadyReceiveBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_alreadyReceiveBtn")
	self.m_alreadyReceiveBtn:addTouchEventListener(touchEventHandler)

	--m_alreadyLable: Text
	self.m_alreadyLable = ccui.Helper:seekWidgetByName(self.rootNode, "m_alreadyLable")

	--m_playerName: Text
	self.m_playerName = ccui.Helper:seekWidgetByName(self.rootNode, "m_playerName")

	--m_alreadyReceiveBtnhui: Button
	self.m_alreadyReceiveBtnhui = ccui.Helper:seekWidgetByName(self.rootNode, "m_alreadyReceiveBtnhui")
	self.m_alreadyReceiveBtnhui:addTouchEventListener(touchEventHandler)

	--m_zuanshiNum: Text
	self.m_zuanshiNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_zuanshiNum")

	self:initUI()
end

function items:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_receiveBtn and event == cc.EventCode.ENDED then
		cclog("[items]--m_receiveBtn touched--")

	elseif sender == self.m_notPassBtn and event == cc.EventCode.ENDED then
		cclog("[items]--m_notPassBtn touched--")

	elseif sender == self.m_alreadyReceiveBtn and event == cc.EventCode.ENDED then
		cclog("[items]--m_alreadyReceiveBtn touched--")

	elseif sender == self.m_alreadyReceiveBtnhui and event == cc.EventCode.ENDED then
		cclog("[items]--m_alreadyReceiveBtnhui touched--")

	end
end

--init your ui here
function items:initUI()
end

--refresh all ui here
function items:refreshUI()
end

return items
