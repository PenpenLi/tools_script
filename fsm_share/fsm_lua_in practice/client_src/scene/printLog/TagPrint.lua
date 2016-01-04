--TagPrint.lua, created by php script. chris.li--
require "Cocos2d"

local lines = 10

local TagPrint = class("TagPrint", function()
	return cc.Layer:create()
end)

function TagPrint:create()
	local layer = TagPrint.new()
	layer:initM()
	return layer
end

function TagPrint:ctor()
end

function TagPrint:onEnter()
end

function TagPrint:onExit()
end

function TagPrint:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("TagPrint.csb")
	self:addChild(self.rootNode)
    
	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end
	
    self.m_closeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeBtn")
    self.m_closeBtn:addTouchEventListener(touchEventHandler)
    
    self.m_deleteBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_delete")
    self.m_deleteBtn:addTouchEventListener(touchEventHandler)
    
    self.scrollView = ccui.Helper:seekWidgetByName(self.rootNode, "ScrollView_4")
    self.scrollView:addTouchEventListener(touchEventHandler)

    self.scrollView:setBounceEnabled(false)
    self.scrollView:setDirection(3)
	self:initUI()
end

function TagPrint:handleTouchEvent(sender, event)
	if sender == nil then
		--cclog("[ERR]unknown sender")
    elseif sender == self.m_closeBtn and event == cc.EventCode.ENDED then
        --cclog("--m_closeBtn touched--")
        --local scene = require("scene.movement.MovementScene") : create()
        log_remove_logstring()
        cc.Director:getInstance():popScene()
        
    elseif sender == self.m_deleteBtn and event == cc.EventCode.ENDED then
        --cclog("--m_delete touched--")
        log_remove_logstring()
    elseif sender == self.m_scrollView and event == cc.EventCode.ENDED then
        --cclog("--m_scrollView touched--")

	end
end

--init your ui here
function TagPrint:initUI()
    print_raw("~~~~~TagPrint:initUI~~~~~~~")
    local linestrs = log_get_logstring()
    --local linestrs = {"sdfdfd"}
    lines = #linestrs
    local size = math.min(lines, 500)
    self.scrollView:setInnerContainerSize(cc.size(10*960,24*size))
    
    local count = 1
    for i=lines, 1, -1 do
        if count > 500 then return end
        local text = ccui.Text:create()
        text:setString(linestrs[lines-i+1])
        text:setAnchorPoint(0,0)
        text:setPosition(0, count*24)
        self.scrollView:addChild(text)
        count = count + 1
    end
end

--refresh all ui here
function TagPrint:refreshUI()
end

return TagPrint
