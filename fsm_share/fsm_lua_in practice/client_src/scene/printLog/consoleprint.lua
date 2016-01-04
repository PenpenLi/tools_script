--consoleprint.lua, created by php script. chris.li--
require "Cocos2d"

local consoleprint = class("consoleprint", function()
	return cc.Layer:create()
end)

function consoleprint:create()
	local layer = consoleprint.new()
	layer:initM()
	return layer
end

function consoleprint:ctor()
end

function consoleprint:onEnter()
end

function consoleprint:onExit()
end

function consoleprint:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("consoleprint.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	self:initUI()
end

function consoleprint:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	end
end

--init your ui here
function consoleprint:initUI()
end

--refresh all ui here
function consoleprint:refreshUI()
end

return consoleprint
