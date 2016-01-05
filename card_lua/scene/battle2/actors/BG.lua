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
    self.bg:setPosition(self.bg:getPositionX()+shiftX, 0)
    self.bg2:setPosition(self.bg2:getPositionX()+shiftX, 0)
    if self.bg:getPositionX() < -WIN_SIZE.width then
        self.bg:setPosition(self.bg2:getPositionX()+WIN_SIZE.width, 0)
        local tmp = self.bg
        self.bg = self.bg2
        self.bg2 = tmp
    end
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
    local bgImgNum = math.random(1,9)
    if param.isEmulate then
        bgImgNum = 8
    end
    
    local bgName = string.format("image/battle/bg/battle_bg_%d.jpg", bgImgNum)
    
    if param then
        if  param.battleType == 1 then 
            bgName = string.format("image/battle/bg/battle_sport.jpg")
        elseif  param.battleType == 5 then 
            bgName = string.format("image/battle/bg/battle_guild.jpg")
        elseif  param.battleType == 6 then 
            bgName = string.format("image/battle/bg/battle_convoy.jpg")
        end
    end
    
    --local bgName = string.format("image/battle/bg/battle_bg_%d.jpg",5)
    self.bg = cc.Sprite:create(bgName)
    self.bg:setAnchorPoint(0,0)
    self.bg:setPosition(0, 0)
    self.bg:setScaleX(WIN_SIZE.width/960)
    self.bg:setScaleY(WIN_SIZE.height/640)
    
    self.bg2 = cc.Sprite:create(bgName)
    self.bg2:setAnchorPoint(0,0)
    self.bg2:setScaleX(WIN_SIZE.width/960)
    self.bg2:setScaleY(WIN_SIZE.height/640)
    self.bg2:setPosition(self.bg:getPositionX()+WIN_SIZE.width, 0)
    
    self:addChild(self.bg)
    self:addChild(self.bg2)
end

function BG:reset(param)
    local bgName = string.format("image/battle/bg/battle_bg_%d.jpg", param.bgID)
    --self.bg:setTexture(bgName)
end

function BG:pause()
    local shader = cc.GLProgramCache:getInstance():getGLProgram("bigskill")
    if self.bg:getGLProgram() ~= shader then
        self.bg.preShader = self.bg:getGLProgram()
        self.bg:setGLProgram(shader)
        
        self.bg2.preShader = self.bg2:getGLProgram()
        self.bg2:setGLProgram(shader)
    end
    --[[
    Helper.clearTimeout(self)
    Helper.setTimeout(self, 20, function()
        self:resume()
    end)
    ]]
end

function BG:resume()
    self.bg:setGLProgram(self.bg.preShader)
    self.bg2:setGLProgram(self.bg2.preShader)
end

function BG:getNodeGrid()
    return nodeGrid
end

return BG