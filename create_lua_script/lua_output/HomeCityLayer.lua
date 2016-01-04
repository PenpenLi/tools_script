--HomeCityLayer.lua, created by php script. chris.li--
require "Cocos2d"

local HomeCityLayer = class("HomeCityLayer", function()
	return cc.Layer:create()
end)

function HomeCityLayer:create()
	local layer = HomeCityLayer.new()
	layer:initM()
	return layer
end

function HomeCityLayer:ctor()
end

function HomeCityLayer:onEnter()
end

function HomeCityLayer:onExit()
end

function HomeCityLayer:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("HomeCityLayer.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_scrollView: ScrollView
	self.m_scrollView = ccui.Helper:seekWidgetByName(self.rootNode, "m_scrollView")
	self.m_scrollView:addTouchEventListener(touchEventHandler)

	--m_mailCity: Button
	self.m_mailCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_mailCity")
	self.m_mailCity:addTouchEventListener(touchEventHandler)

	--m_activityCity: Button
	self.m_activityCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_activityCity")
	self.m_activityCity:addTouchEventListener(touchEventHandler)

	--m_sportCity: Button
	self.m_sportCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_sportCity")
	self.m_sportCity:addTouchEventListener(touchEventHandler)

	--m_tieCity: Button
	self.m_tieCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_tieCity")
	self.m_tieCity:addTouchEventListener(touchEventHandler)

	--m_pvpCity: Button
	self.m_pvpCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_pvpCity")
	self.m_pvpCity:addTouchEventListener(touchEventHandler)

	--m_clubCity: Button
	self.m_clubCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_clubCity")
	self.m_clubCity:addTouchEventListener(touchEventHandler)

	--m_shopCity: Button
	self.m_shopCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_shopCity")
	self.m_shopCity:addTouchEventListener(touchEventHandler)

	--m_yewaiCity: Button
	self.m_yewaiCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_yewaiCity")
	self.m_yewaiCity:addTouchEventListener(touchEventHandler)

	--m_heroCity: Button
	self.m_heroCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_heroCity")
	self.m_heroCity:addTouchEventListener(touchEventHandler)

	--m_scienceCity: Button
	self.m_scienceCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_scienceCity")
	self.m_scienceCity:addTouchEventListener(touchEventHandler)

	--m_taskCity: Button
	self.m_taskCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_taskCity")
	self.m_taskCity:addTouchEventListener(touchEventHandler)

	--m_pickCardCity: Button
	self.m_pickCardCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_pickCardCity")
	self.m_pickCardCity:addTouchEventListener(touchEventHandler)

	self:initUI()
end

function HomeCityLayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_scrollView and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_scrollView touched--")

	elseif sender == self.m_mailCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_mailCity touched--")

	elseif sender == self.m_activityCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_activityCity touched--")

	elseif sender == self.m_sportCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_sportCity touched--")

	elseif sender == self.m_tieCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_tieCity touched--")

	elseif sender == self.m_pvpCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_pvpCity touched--")

	elseif sender == self.m_clubCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_clubCity touched--")

	elseif sender == self.m_shopCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_shopCity touched--")

	elseif sender == self.m_yewaiCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_yewaiCity touched--")

	elseif sender == self.m_heroCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_heroCity touched--")

	elseif sender == self.m_scienceCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_scienceCity touched--")

	elseif sender == self.m_taskCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_taskCity touched--")

	elseif sender == self.m_pickCardCity and event == cc.EventCode.ENDED then
		cclog("[HomeCityLayer]--m_pickCardCity touched--")

	end
end

--init your ui here
function HomeCityLayer:initUI()
end

--refresh all ui here
function HomeCityLayer:refreshUI()
end

return HomeCityLayer
