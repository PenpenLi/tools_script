--Information_tip.lua, created by php script. chris.li--
require "Cocos2d"

local Information_tip = class("Information_tip", function()
	return cc.Layer:create()
end)

function Information_tip:create()
	local layer = Information_tip.new()
	layer:initM()
	return layer
end

function Information_tip:ctor()
end

function Information_tip:onEnter()
end

function Information_tip:onExit()
end

function Information_tip:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("Information_tip.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	self:initUI()
end

function Information_tip:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	end
end

--init your ui here
function Information_tip:initUI()
end

--refresh all ui here
function Information_tip:refreshUI()
end

return Information_tip
