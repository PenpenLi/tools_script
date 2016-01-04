require("Cocos2d")
local nodeGrid = nil
local BattleBgLayer = class("BattleBgLayer", function()
    return cc.Layer:create() 
end)

--[[
param:{
    bgID = xxx,
}
]]--
function BattleBgLayer:create()
    local layer = BattleBgLayer.new()
    layer:initM()
    nodeGrid = cc.NodeGrid:create()
    nodeGrid:addChild(layer)
    return layer
end

function BattleBgLayer:ctor()
end

function BattleBgLayer:onEnter()
end

function BattleBgLayer:onExit()
end

function BattleBgLayer:initM()
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)

    local bgName = string.format("image/battle/bg/battle_bg_%d.jpg", 5)
    self.bg = cc.Sprite:create(bgName)
    self.bg:setAnchorPoint(0,0)
    self.bg:setScaleX(WIN_SIZE.width/960)
    self:addChild(self.bg)
    self:setAnchorPoint(0,0)
end

function BattleBgLayer:reset(param)
    local bgName = string.format("image/battle/bg/battle_bg_%d.jpg", param.bgID)
    self.bg:setTexture(bgName)
end

function BattleBgLayer:getNodeGrid()
    return nodeGrid
end

return BattleBgLayer
