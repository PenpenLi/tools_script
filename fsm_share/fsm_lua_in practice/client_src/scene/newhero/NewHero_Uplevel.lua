--NewHero_Uplevel.lua, created by php script. chris.li--
require "Cocos2d"

local NewHero_Uplevel = class("NewHero_Uplevel", function()
	return cc.Layer:create()
end)

function NewHero_Uplevel:create()
	local layer = NewHero_Uplevel.new()
	layer:initM()
	return layer
end

function NewHero_Uplevel:ctor()
end

function NewHero_Uplevel:onEnter()
end

function NewHero_Uplevel:onExit()
end

function NewHero_Uplevel:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("NewHero_Uplevel.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_upPanel: Layout
	self.m_upPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_upPanel")
	self.m_upPanel:addTouchEventListener(touchEventHandler)

	--m_backbtn: Button
	self.m_backbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_backbtn")
	self.m_backbtn:addTouchEventListener(touchEventHandler)

	--m_heropic: ImageView
	self.m_heropic = ccui.Helper:seekWidgetByName(self.rootNode, "m_heropic")

	--m_namelabel: Text
	self.m_namelabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_namelabel")

	--m_leveljindu: ImageView
	self.m_leveljindu = ccui.Helper:seekWidgetByName(self.rootNode, "m_leveljindu")

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

	--m_upbtn: Button
	self.m_upbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_upbtn")
	self.m_upbtn:addTouchEventListener(touchEventHandler)

	--m_atkadd: Text
	self.m_atkadd = ccui.Helper:seekWidgetByName(self.rootNode, "m_atkadd")

	--m_defadd: Text
	self.m_defadd = ccui.Helper:seekWidgetByName(self.rootNode, "m_defadd")

	--m_hpadd: Text
	self.m_hpadd = ccui.Helper:seekWidgetByName(self.rootNode, "m_hpadd")

	--m_zhiliadd: Text
	self.m_zhiliadd = ccui.Helper:seekWidgetByName(self.rootNode, "m_zhiliadd")

	--m_agladd: Text
	self.m_agladd = ccui.Helper:seekWidgetByName(self.rootNode, "m_agladd")

	--m_dexadd: Text
	self.m_dexadd = ccui.Helper:seekWidgetByName(self.rootNode, "m_dexadd")

	--m_criadd: Text
	self.m_criadd = ccui.Helper:seekWidgetByName(self.rootNode, "m_criadd")

	--m_blockadd: Text
	self.m_blockadd = ccui.Helper:seekWidgetByName(self.rootNode, "m_blockadd")

	--m_yaoshui: Text
	self.m_yaoshui = ccui.Helper:seekWidgetByName(self.rootNode, "m_yaoshui")

	--m_levelpic: ImageView
	self.m_levelpic = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelpic")

	--m_levelnum: Text
	self.m_levelnum = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelnum")

	self:initUI()
end

function NewHero_Uplevel:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_upPanel and event == cc.EventCode.ENDED then
		cclog("[NewHero_Uplevel]--m_upPanel touched--")

	elseif sender == self.m_backbtn and event == cc.EventCode.ENDED then
		cclog("[NewHero_Uplevel]--m_backbtn touched--")

	elseif sender == self.m_upbtn and event == cc.EventCode.ENDED then
		cclog("[NewHero_Uplevel]--m_upbtn touched--")

	end
end

--init your ui here
function NewHero_Uplevel:initUI()
end

--refresh all ui here
function NewHero_Uplevel:refreshUI()
end

return NewHero_Uplevel
