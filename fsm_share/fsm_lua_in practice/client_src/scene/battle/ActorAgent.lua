require("Cocos2d")

local BattleBgLayer =  require "scene.battle.actors.BattleBgLayer"
local TeamLayer = require "scene.battle.actors.TeamLayer"
local BattleUILayer =  require "scene.battle.actors.BattleUILayer"
local BattleResultLayer =  require "scene.battle.actors.BattleResultUILayer"
local FormationUILayer =  require "scene.formation.FormationUILayer"

local CMDHandlers = {
}

local ActorAgent = class("ActorAgent", function()
    return cc.Layer:create() 
end)

function ActorAgent:create()
    local layer = ActorAgent.new()
    layer:initM()
    layer:retain()
    return layer
end

function ActorAgent:ctor()
end

function ActorAgent:onEnter()
end

function ActorAgent:onExit()
    self:release()
end

function ActorAgent:initM()
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)
    
    self.bgLayer = BattleBgLayer:create()
    self:addChild(self.bgLayer:getNodeGrid())
    self.teamLayer = TeamLayer:create()
    self:addChild(self.teamLayer)
    self.uiLayer = BattleUILayer:create({})
    self:addChild(self.uiLayer)
    
    local effectMusicPath = cc.FileUtils:getInstance():fullPathForFilename("sound/globalSkill_thunder.mp3") 

    cc.SimpleAudioEngine:getInstance():preloadEffect(effectMusicPath)
end

--[[
Reset function is called by DirectorAgent
config: {
    teamLBook = {{npcBook}, {npcBook}, {npcBook}, {npcBook}, {npcBook}},
    teamRBook = {{npcBook}, {npcBook}, {npcBook}, {npcBook}, {npcBook}},
    bgID = 1,
}
]]--
function ActorAgent:reset(config)
    self.uiLayer:stopJX()
    
    printJSON(config)
    self.bgLayer:reset({bgID=config.bgID})
    self.teamLayer:reset({teamLBook=config.teamLBook, teamRBook=config.teamRBook})
    --self.uiLayer:reset({})
end

--[[
All CMD lists:
{cmdId="walk", npcKey=npcDirector.npcKey, duration=self.walkTimer}
{cmdId="stand", npcKey=npcDirector.key}
{cmdId="normalAttack", npcKey=npcDirector.key, targetNpcKey=self.nearestAttackNpc.key, subHp=100}
{cmdId="smallSkillAttack", npcKey=npcDirector.key, targetNpcKey=self.nearestAttackNpc.key, subHp=100}
{cmdId="bigSkillAttack", npcKey=npcDirector.key, targetNpcKey=self.nearestAttackNpc.key, subHp=100}
{cmdId = "dizzy", npcKey = npcDirector.key,targetNpcKey = npcDirector.key,subHpVal = self.tempSubHpVal,duration = self.timer/30}
{cmdId = "fallback", npcKey = npcDirector.key,targetNpcKey = npcDirector.key,subHpVal = self.tempSubHpVal,duration = self.timer/30}
{cmdId="globalSkill", level=data.skillid}
]]--
function ActorAgent:actCMD(CMD)
    --printJSON(CMD)
    self.teamLayer:actCMD(CMD)
    --other layers
end

function ActorAgent:startJX()
    self.uiLayer:startJX()
end

function ActorAgent:stopJX()
    self.uiLayer:stopJX()
end

function ActorAgent:updateUI(data)
    self.uiLayer:updateUI(data)
end

function ActorAgent:onBattleOver()
    self:stopJX()
    self:runAction(cc.Sequence:create(cc.DelayTime:create(2), cc.CallFunc:create(
            function()

            Http: req("NewBattleFinish",
                {endId = math.floor((FormationUILayer:getBattleFieldId()-34)/31) ,leftAtks = 5,leftDefs = 0,usedSeconds = 0,treasures =0},
                function(res)
                    if res.err then cclog("err!!!") return end
                    --local winnner = nil

                    self.resultLayer = BattleResultLayer:create(res.data)
                    self:addChild(self.resultLayer)
                end)
            --[[
            local winnner = DIRECTOR_AGENT:getWinner()
            self.resultLayer = BattleResultLayer:create(winnner)
            self:addChild(self.resultLayer)
            ]]
        --cc.Director:getInstance():popScene()
    end)))
end

function ActorAgent:delayAndReset()
    self:stopJX()
    self:runAction(cc.Sequence:create(cc.DelayTime:create(2),cc.CallFunc:create(function()
        DIRECTOR_AGENT:reset()
    end)))
end

return ActorAgent
