--HomeUILayer.lua, created by php script. chris.li--
require "Cocos2d"

local HomeUILayer = class("HomeUILayer", function()
	return cc.Layer:create()
end)

function HomeUILayer:create()
	local layer = HomeUILayer.new()
	layer:initM()
	return layer
end

function HomeUILayer:ctor()
end

function HomeUILayer:onEnter()
end

function HomeUILayer:onExit()
end

function HomeUILayer:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("HomeUILayer.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_chatBtn: Button
	self.m_chatBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_chatBtn")
	self.m_chatBtn:addTouchEventListener(touchEventHandler)

	--m_headImg: ImageView
	self.m_headImg = ccui.Helper:seekWidgetByName(self.rootNode, "m_headImg")

	--m_menuLayout: Layout
	self.m_menuLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_menuLayout")
	self.m_menuLayout:addTouchEventListener(touchEventHandler)

	--m_menuBtn: Button
	self.m_menuBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_menuBtn")
	self.m_menuBtn:addTouchEventListener(touchEventHandler)

	--m_addStrengthBtn: Button
	self.m_addStrengthBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_addStrengthBtn")
	self.m_addStrengthBtn:addTouchEventListener(touchEventHandler)

	--m_addDiamondBtn: Button
	self.m_addDiamondBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_addDiamondBtn")
	self.m_addDiamondBtn:addTouchEventListener(touchEventHandler)

	--m_addCoinBtn: Button
	self.m_addCoinBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_addCoinBtn")
	self.m_addCoinBtn:addTouchEventListener(touchEventHandler)

	--m_strengthLabel: Text
	self.m_strengthLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_strengthLabel")

	--m_diamondLabel: Text
	self.m_diamondLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_diamondLabel")

	--m_coinLabel: Text
	self.m_coinLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_coinLabel")

	--m_headBtn: Button
	self.m_headBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_headBtn")
	self.m_headBtn:addTouchEventListener(touchEventHandler)

	--m_startServerActBtn: Button
	self.m_startServerActBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_startServerActBtn")
	self.m_startServerActBtn:addTouchEventListener(touchEventHandler)

	--m_payBtn: Button
	self.m_payBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_payBtn")
	self.m_payBtn:addTouchEventListener(touchEventHandler)

	--m_tranHeroBtn: Button
	self.m_tranHeroBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_tranHeroBtn")
	self.m_tranHeroBtn:addTouchEventListener(touchEventHandler)

	--m_dailyBtn: Button
	self.m_dailyBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_dailyBtn")
	self.m_dailyBtn:addTouchEventListener(touchEventHandler)

	--m_bagBtn: Button
	self.m_bagBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_bagBtn")
	self.m_bagBtn:addTouchEventListener(touchEventHandler)

	--m_xinghunBtn: Button
	self.m_xinghunBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_xinghunBtn")
	self.m_xinghunBtn:addTouchEventListener(touchEventHandler)

	--m_levelLabel: Text
	self.m_levelLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelLabel")

	--m_nameLabel: Text
	self.m_nameLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_nameLabel")

	--m_viplevelLabel: Text
	self.m_viplevelLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_viplevelLabel")

	self:initUI()
end

function HomeUILayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_chatBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_chatBtn touched--")

	elseif sender == self.m_menuLayout and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_menuLayout touched--")

	elseif sender == self.m_menuBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_menuBtn touched--")

	elseif sender == self.m_addStrengthBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_addStrengthBtn touched--")

	elseif sender == self.m_addDiamondBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_addDiamondBtn touched--")

	elseif sender == self.m_addCoinBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_addCoinBtn touched--")

	elseif sender == self.m_headBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_headBtn touched--")

	elseif sender == self.m_startServerActBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_startServerActBtn touched--")

	elseif sender == self.m_payBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_payBtn touched--")

	elseif sender == self.m_tranHeroBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_tranHeroBtn touched--")

	elseif sender == self.m_dailyBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_dailyBtn touched--")

	elseif sender == self.m_bagBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_bagBtn touched--")

	elseif sender == self.m_xinghunBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_xinghunBtn touched--")

	end
end

--init your ui here
function HomeUILayer:initUI()
end

--refresh all ui here
function HomeUILayer:refreshUI()
end

return HomeUILayer
