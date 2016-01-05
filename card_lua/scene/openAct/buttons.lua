--buttons.lua, created by php script. chris.li--
require "Cocos2d"

local buttons = class("buttons", function()
	return cc.Layer:create()
end)

function buttons:create()
	local layer = buttons.new()
	layer:initM()
	return layer
end

function buttons:ctor()
end

function buttons:onEnter()
end

function buttons:onExit()
end

function buttons:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("buttons.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_buttonListPanel: Layout
	self.m_buttonListPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_buttonListPanel")
	self.m_buttonListPanel:addTouchEventListener(touchEventHandler)

	--m_button: Button
	self.m_button = ccui.Helper:seekWidgetByName(self.rootNode, "m_button")
	self.m_button:addTouchEventListener(touchEventHandler)

	--m_redDot: ImageView
	self.m_redDot = ccui.Helper:seekWidgetByName(self.rootNode, "m_redDot")

	self:initUI()
end

function buttons:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_buttonListPanel and event == cc.EventCode.ENDED then
		cclog("[buttons]--m_buttonListPanel touched--")

	elseif sender == self.m_button and event == cc.EventCode.ENDED then
		cclog("[buttons]--m_button touched--")

	end
end

--init your ui here
function buttons:initUI()
end

--refresh all ui here
function buttons:refreshUI()
end

return buttons
