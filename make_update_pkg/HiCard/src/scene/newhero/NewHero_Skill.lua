--NewHero_Skill.lua, created by php script. chris.li--
require "Cocos2d"

local NewHero_Skill = class("NewHero_Skill", function()
	return cc.Layer:create()
end)

function NewHero_Skill:create()
	local layer = NewHero_Skill.new()
	layer:initM()
	return layer
end

function NewHero_Skill:ctor()
end

function NewHero_Skill:onEnter()
end

function NewHero_Skill:onExit()
end

function NewHero_Skill:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("NewHero_Skill.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_skillPanel: Layout
	self.m_skillPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_skillPanel")
	self.m_skillPanel:addTouchEventListener(touchEventHandler)

	--m_ListView: ListView
	self.m_ListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_ListView")
	self.m_ListView:addTouchEventListener(touchEventHandler)

	--m_checkname: Text
	self.m_checkname = ccui.Helper:seekWidgetByName(self.rootNode, "m_checkname")

	--m_skilldesc: Text
	self.m_skilldesc = ccui.Helper:seekWidgetByName(self.rootNode, "m_skilldesc")

	--m_nowlevel: TextAtlas
	self.m_nowlevel = ccui.Helper:seekWidgetByName(self.rootNode, "m_nowlevel")

	--m_nextlevel: TextAtlas
	self.m_nextlevel = ccui.Helper:seekWidgetByName(self.rootNode, "m_nextlevel")

	--m_upbtn: Button
	self.m_upbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_upbtn")
	self.m_upbtn:addTouchEventListener(touchEventHandler)

	--m_yaoshui: Text
	self.m_yaoshui = ccui.Helper:seekWidgetByName(self.rootNode, "m_yaoshui")

	--m_jinbi: Text
	self.m_jinbi = ccui.Helper:seekWidgetByName(self.rootNode, "m_jinbi")

	--m_backbtn: Button
	self.m_backbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_backbtn")
	self.m_backbtn:addTouchEventListener(touchEventHandler)

	self:initUI()
end

function NewHero_Skill:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_skillPanel and event == cc.EventCode.ENDED then
		cclog("[NewHero_Skill]--m_skillPanel touched--")

	elseif sender == self.m_ListView and event == cc.EventCode.ENDED then
		cclog("[NewHero_Skill]--m_ListView touched--")

	elseif sender == self.m_upbtn and event == cc.EventCode.ENDED then
		cclog("[NewHero_Skill]--m_upbtn touched--")

	elseif sender == self.m_backbtn and event == cc.EventCode.ENDED then
		cclog("[NewHero_Skill]--m_backbtn touched--")

	end
end

--init your ui here
function NewHero_Skill:initUI()
end

--refresh all ui here
function NewHero_Skill:refreshUI()
end

return NewHero_Skill
