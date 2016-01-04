require("Cocos2d")

local BG =  require "scene.battle2.actors.BG"
local FightLayer =  require "scene.battle2.actors.FightLayer"
local UI = require "scene.battle2.actors.UI"
local BattleResultLayer =  require "scene.battle2.actors.BattleResultUILayer"

local ActorAgent = class("ActorAgent", function()
    return cc.Layer:create()
end)

-----------------------[[External API]]-----------------------------
function ActorAgent:create(param)
    local actorAgent = ActorAgent.new()
    actorAgent:initM(param)
    return actorAgent
end

function ActorAgent:updateM()
end

function ActorAgent:actCMD(cmd)
    if self.isBattleOver == true then return end
    
    if cmd.t == "move" and cmd.key == 99999 then
        self.bg:move(cmd.dx, cmd.dy)
        return
    end
    
    if cmd.t == "global_s" then
        self.ui:setVisible(false)
    elseif cmd.t == "globaled" then
        self.ui:setVisible(true)
    end
    
    self.fightLayer:actCMD(cmd)
end

function ActorAgent:onBattleOver(param)
    print("[ActorAgent] Enter onBattleOver")
    self:stopJX()
    self.isBattleOver = true
    local keys = param.aliveKey
    if keys ~= nil then
    	for i=1,#keys do
    	   local cmd = {}
    	   cmd.key = keys[i]
    	   cmd.t = "stand"
           self.fightLayer:actCMD(cmd)
    	end
    end
    --print("[ActorAgent] Start NewBattleFinish")
    Http: req("NewBattleFinish",
        {endId = math.floor((self.fId-34)/31) ,leftAtk = param.leftAtks,leftDef = param.leftDefs,leftSeconds = 1},
        function(res)
            if res.err then cclog("err!!!") return end
            --local winnner = nil
            print("[ActorAgent] Finish NewBattleFinish")
            self.resultLayer = BattleResultLayer:create(res.data)      
            self.ui:removeFromParent()
            self.fightLayer:hideColorLayer()
            print("[ActorAgent] Enter resultLayer")
            self:addChild(self.resultLayer)
        end)
            --[[
            local winnner = DIRECTOR_AGENT:getWinner()
            self.resultLayer = BattleResultLayer:create(winnner)
            self:addChild(self.resultLayer)
            ]]
            --cc.Director:getInstance():popScene()

end

function ActorAgent:onBattleOverPK(param)
    if  self.isBattleOver == false then
        local resultLayer = BattleResultLayer:create(param)      
        self:addChild(resultLayer)
        self.isBattleOver = true
    end
    --cc.Director:getInstance():popScene()
end

function ActorAgent:startJX()
    if self.isBattleOver == false then
        self.ui:startJX()
    end
    
end

function ActorAgent:stopJX()
    if self.ui then
        self.ui:stopJX()
    end
end

function ActorAgent:skillBegin()
    self.ui:skillBegin()
end

function ActorAgent:skillEnd()
    if self.ui == nil then return end
    self.ui:skillEnd()
end

function ActorAgent:getTreasurePos()
    return self.ui:getTreasurePos()
end

function ActorAgent:addTreasureNum()
    if self.ui == nil then return end
    return self.ui:addTreasureNum()
end

------------------------[[Internal function]]------------------------
function ActorAgent:ctor()
end

function ActorAgent:onEnter()
end

function ActorAgent:onExit()
end

function ActorAgent:initM(param)
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)
    
    --1. background layer
    if  param then
        if  param.isPvp == true then
            self.bg = BG:create(param)
            self:addChild(self.bg:getNodeGrid())
        else
            self.bg = BG:create()
            self:addChild(self.bg:getNodeGrid())
        end
    else
        self.bg = BG:create()
        self:addChild(self.bg:getNodeGrid())
    end
    
    
    --2. team layer
    self.fightLayer = FightLayer:create()
    self:addChild(self.fightLayer)
    
    --3. UI layer
    self.fId = 1
    if param and param.isPvp ~= true then
        self.ui = UI:create(param)
        self:addChild(self.ui)
        self.fId = param.fId --fieldId
    end

    self.isBattleOver = false --battle over tag
end

return ActorAgent
