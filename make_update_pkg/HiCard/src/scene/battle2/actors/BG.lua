require("Cocos2d")
local nodeGrid =nil
local BG = class("BG", function()
    return cc.Layer:create() 
end)

function BG:create(param)
    local layer = BG.new()
    layer:initM(param)
    nodeGrid = cc.NodeGrid:create()
    nodeGrid:addChild(layer)
    return layer
end

function BG:move(shiftX, shiftY)
    self:setPosition(self:getPositionX()+shiftX, self:getPositionY()+shiftY)
end

function BG:ctor()
end

function BG:onEnter()
end

function BG:onExit()
end

function BG:initM(param)
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)
     
    local bgName = string.format("image/battle/bg/battle_bg_%d.jpg", math.random(1,10))
    --[[
    if param then
        if  param.isPvp == true then 
            bgName = string.format("image/battle/bg/battle_bg_%d.jpg", 11)
        end
    end
    ]]
    
    --local bgName = string.format("image/battle/bg/battle_bg_%d.jpg",5)
    self.bg = cc.Sprite:create(bgName)
    self.bg:setAnchorPoint(0,0)
    self.bg:setScaleX(WIN_SIZE.width/960)
    self.bg:setScaleY(WIN_SIZE.height/640)
    
    self.bg2 = cc.Sprite:create(bgName)
    self.bg2:setAnchorPoint(0,0)
    self.bg2:setScaleX(WIN_SIZE.width/960)
    self.bg2:setScaleY(WIN_SIZE.height/640)
    self.bg2:setPositionX(self.bg:getPositionX()+WIN_SIZE.width)
    
    self.bg3 = cc.Sprite:create(bgName)
    self.bg3:setAnchorPoint(0,0)
    self.bg3:setScaleX(WIN_SIZE.width/960)
    self.bg3:setScaleY(WIN_SIZE.height/640)
    self.bg3:setPositionX(self.bg2:getPositionX()+WIN_SIZE.width)
    
    self:addChild(self.bg)
    self:addChild(self.bg2)
    self:addChild(self.bg3)
end

function BG:reset(param)
    local bgName = string.format("image/battle/bg/battle_bg_%d.jpg", param.bgID)
    --self.bg:setTexture(bgName)
end

function BG:getNodeGrid()
    return nodeGrid
end

return BG