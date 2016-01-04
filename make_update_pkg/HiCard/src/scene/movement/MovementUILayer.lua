--MovementUILayer.lua, created by php script. chris.li--
require "Cocos2d"

local Roler = require("data.Roler")

local MovementUILayer = class("MovementUILayer", function()
	return cc.Layer:create()
end)

function MovementUILayer:create()
	local layer = MovementUILayer.new()
	layer:initM()
	return layer
end

function MovementUILayer:ctor()
end

function MovementUILayer:onEnter()
end

function MovementUILayer:onExit()
end

function MovementUILayer:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("MovementUILayer.csb")
	self:addChild(self.rootNode)
	
    self.rootNode:setBackGroundImage("ditu.jpg")

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_movementLayout: Layout
	self.m_movementLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_movementLayout")
	self.m_movementLayout:addTouchEventListener(touchEventHandler)

	--m_closeBtn: Button
	self.m_closeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeBtn")
	self.m_closeBtn:addTouchEventListener(touchEventHandler)

	--m_bookItemLayout: Layout
	self.m_bookItemLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_bookItemLayout")
	
	--m_coinsItemLayout: Layout
	self.m_coinsItemLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_coinsItemLayout")
	
	--m_other1ItemLayout: Layout
	self.m_other1ItemLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_other1ItemLayout")
	
	--m_other2ItemLayout: Layout
	self.m_other2ItemLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_other2ItemLayout")
	
	--m_coinscntLabel: Text
	self.m_coinscntLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_coinscntLabel")
	
    self.m_gobtn1 = ccui.Helper:seekWidgetByName(self.m_bookItemLayout, "m_gobtn1")
    self.m_gobtn1:addTouchEventListener(touchEventHandler)
    
    self.m_gobtn2 = ccui.Helper:seekWidgetByName(self.m_coinsItemLayout, "m_gobtn2")
    self.m_gobtn2:addTouchEventListener(touchEventHandler)
    
    self.m_gobtn3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_gobtn3")
    self.m_gobtn3:addTouchEventListener(touchEventHandler)

	self:initUI()
end

function MovementUILayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_movementLayout and event == cc.EventCode.ENDED then
		cclog("[MovementUILayer]--m_movementLayout touched--")

	elseif sender == self.m_closeBtn and event == cc.EventCode.ENDED then
		cclog("[MovementUILayer]--m_closeBtn touched--")
        local scene = require("scene.home.HomeScene") : create()
        cc.Director:getInstance():replaceScene(scene)
        
	elseif sender == self.m_bookItemLayout and event == cc.EventCode.ENDED then
		cclog("[MovementUILayer]--m_bookItemLayout touched--")
        
        
	elseif sender == self.m_coinsItemLayout and event == cc.EventCode.ENDED then
		cclog("[MovementUILayer]--m_coinsItemLayout touched--")
        
        
	elseif sender == self.m_other1ItemLayout and event == cc.EventCode.ENDED then
		cclog("[MovementUILayer]--m_other1ItemLayout touched--")

	elseif sender == self.m_other2ItemLayout and event == cc.EventCode.ENDED then
		cclog("[MovementUILayer]--m_other2ItemLayout touched--")

    elseif sender == self.m_gobtn1 and event == cc.EventCode.ENDED then
        cclog("[MovementUILayer]--m_gobtn1 touched--")
        if Roler:get("level") < 12 then
            PopDialog:getInstance():popText("夺宝12级开放")
            return
        end
        self:enterBookUp()
        
    elseif sender == self.m_gobtn2 and event == cc.EventCode.ENDED then
        cclog("[MovementUILayer]--m_gobtn2 touched--")
        if Roler:get("level") < 12 then
            PopDialog:getInstance():popText("金币BOSS10级开放")
            return
        end
        self:enterCoinsEctype()
        
    elseif sender == self.m_gobtn3 and event == cc.EventCode.ENDED then
        cclog("[MovementUILayer]--m_gobtn3 touched--")
        self:enterGuard()
	end
end

--init your ui here
function MovementUILayer:initUI()
    
    Http: req("GetMovement",
              nil,
              function(res) 
                 if res.err then cclog("err!!!") return end
                 
                 self.m_coinscntLabel:setString(res.data["coinsEctypeMaxCount"]-res.data["coinsEctypeCount"].."/"
                                        ..res.data["coinsEctypeMaxCount"])
                    
    end)
end

--refresh all ui here
function MovementUILayer:refreshUI()
end

function MovementUILayer:enterCoinsEctype()
    local FormationUILayer =  require "scene.formation.FormationUILayer"

    --FormationUILayer:setBeforeSceneType("coinsEctype")

    local scence = require("scene.formation.FormationScene"):create({BattleType="coinsEctype"})
    cc.Director:getInstance():pushScene(scence)
    
end

function MovementUILayer:enterBookUp()
--夺宝
    local scene = require("scene.indiana.IndianaScene"):create()
    cc.Director:getInstance():pushScene(scene)
end

function MovementUILayer:checkResult(_result)
    local bRet=false
    if _result== 0 then 
        cclog("MOVEMENT_SUCCESS")
        bRet = true
    elseif  _result == 1 then
        cclog("MOVEMENT_NOT_ENOUGH")
    end
    return bRet
end

--进入押镖界面
function MovementUILayer:enterGuard()
    local scene =  require "scene.guard.GuardScene":create()
    cc.Director:getInstance():pushScene(scene)
end

return MovementUILayer
