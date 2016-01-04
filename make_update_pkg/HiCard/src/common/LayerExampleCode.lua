require("Cocos2d")

local TeamLayer = class("TeamLayer", function()
    return cc.Layer:create() 
end)

function TeamLayer:create()
    local layer = TeamLayer.new()
    layer:initM()
    return layer
end

function TeamLayer:ctor()
end

function TeamLayer:onEnter()
end

function TeamLayer:onExit()
end

function TeamLayer:initM()
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)
end

return TeamLayer
