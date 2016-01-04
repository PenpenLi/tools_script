--NewHero_Main.lua, created by php script. chris.li--
require "Cocos2d"

local NewHero_Main = class("NewHero_Main", function()
	return cc.Layer:create()
end)

function NewHero_Main:create()
	local layer = NewHero_Main.new()
	layer:initM()
	return layer
end

function NewHero_Main:ctor()
end

function NewHero_Main:onEnter()
end

function NewHero_Main:onExit()
end

function NewHero_Main:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("NewHero_Main.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_mainPanel: Layout
	self.m_mainPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_mainPanel")
	self.m_mainPanel:addTouchEventListener(touchEventHandler)

	--m_backbtn: Button
	self.m_backbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_backbtn")
	self.m_backbtn:addTouchEventListener(touchEventHandler)

	--m_heropic: ImageView
	self.m_heropic = ccui.Helper:seekWidgetByName(self.rootNode, "m_heropic")

	--m_namelabel: Text
	self.m_namelabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_namelabel")

	--m_atk: Text
	self.m_atk = ccui.Helper:seekWidgetByName(self.rootNode, "m_atk")

	--m_def: Text
	self.m_def = ccui.Helper:seekWidgetByName(self.rootNode, "m_def")

	--m_hp: Text
	self.m_hp = ccui.Helper:seekWidgetByName(self.rootNode, "m_hp")

	--m_zhili: Text
	self.m_zhili = ccui.Helper:seekWidgetByName(self.rootNode, "m_zhili")

	--m_agl: Text
	self.m_agl = ccui.Helper:seekWidgetByName(self.rootNode, "m_agl")

	--m_dex: Text
	self.m_dex = ccui.Helper:seekWidgetByName(self.rootNode, "m_dex")

	--m_cri: Text
	self.m_cri = ccui.Helper:seekWidgetByName(self.rootNode, "m_cri")

	--m_block: Text
	self.m_block = ccui.Helper:seekWidgetByName(self.rootNode, "m_block")

	--m_btn3: Button
	self.m_btn3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_btn3")
	self.m_btn3:addTouchEventListener(touchEventHandler)

	--m_btn1: Button
	self.m_btn1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_btn1")
	self.m_btn1:addTouchEventListener(touchEventHandler)

	--m_btn0: Button
	self.m_btn0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_btn0")
	self.m_btn0:addTouchEventListener(touchEventHandler)

	--m_suipiannum: Text
	self.m_suipiannum = ccui.Helper:seekWidgetByName(self.rootNode, "m_suipiannum")

	--m_btn2: Button
	self.m_btn2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_btn2")
	self.m_btn2:addTouchEventListener(touchEventHandler)

	--m_eqpic_0: ImageView
	self.m_eqpic_0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_eqpic_0")

	--m_eqpic_1: ImageView
	self.m_eqpic_1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_eqpic_1")

	--m_eqpic_2: ImageView
	self.m_eqpic_2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_eqpic_2")

	--m_eqpic_3: ImageView
	self.m_eqpic_3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_eqpic_3")

	--m_level: Text
	self.m_level = ccui.Helper:seekWidgetByName(self.rootNode, "m_level")

	--m_zhanli: Text
	self.m_zhanli = ccui.Helper:seekWidgetByName(self.rootNode, "m_zhanli")

	--m_suipianjindu: ImageView
	self.m_suipianjindu = ccui.Helper:seekWidgetByName(self.rootNode, "m_suipianjindu")

	--m_heroplace: ImageView
	self.m_heroplace = ccui.Helper:seekWidgetByName(self.rootNode, "m_heroplace")

	--m_eqpinzhi_0: ImageView
	self.m_eqpinzhi_0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_eqpinzhi_0")

	--m_eqpinzhi_1: ImageView
	self.m_eqpinzhi_1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_eqpinzhi_1")

	--m_eqpinzhi_2: ImageView
	self.m_eqpinzhi_2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_eqpinzhi_2")

	--m_eqpinzhi_3: ImageView
	self.m_eqpinzhi_3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_eqpinzhi_3")

	self:initUI()
end

function NewHero_Main:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_mainPanel and event == cc.EventCode.ENDED then
		cclog("[NewHero_Main]--m_mainPanel touched--")

	elseif sender == self.m_backbtn and event == cc.EventCode.ENDED then
		cclog("[NewHero_Main]--m_backbtn touched--")

	elseif sender == self.m_btn3 and event == cc.EventCode.ENDED then
		cclog("[NewHero_Main]--m_btn3 touched--")

	elseif sender == self.m_btn1 and event == cc.EventCode.ENDED then
		cclog("[NewHero_Main]--m_btn1 touched--")

	elseif sender == self.m_btn0 and event == cc.EventCode.ENDED then
		cclog("[NewHero_Main]--m_btn0 touched--")

	elseif sender == self.m_btn2 and event == cc.EventCode.ENDED then
		cclog("[NewHero_Main]--m_btn2 touched--")

	end
end

--init your ui here
function NewHero_Main:initUI()
end

--refresh all ui here
function NewHero_Main:refreshUI()
end

return NewHero_Main
