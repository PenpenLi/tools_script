require("Cocos2d")

local BG =  require "scene.battle2.actors.BG"
local FightLayer =  require "scene.battle2.actors.FightLayer"
local UI = require "scene.battle2.actors.UI2"
local BattleResultLayer =  require "scene.battle2.actors.BattleResultUILayer"
local ActorAgent = class("ActorAgent", function()
    return cc.Layer:create()
end)

-----------------------[[External API]]-----------------------------
function ActorAgent:create(param)
    local actorAgent = ActorAgent.new()
    actorAgent:initFirst(param)
    return actorAgent
end

function ActorAgent:updateM()
    if self.isBattleOver == true then return end
    if self.ui then self.ui:updateM() end
end

function ActorAgent:actCMD(cmd)
    if self.isBattleOver == true then return end
    
    if cmd.t == "init_actor_agent" then
        printJSON(cmd)
        self:initM(cmd.data)
        return
    end
    
    if cmd.t == "move" and cmd.key == 99999 then
        self.bg:move(cmd.dx, cmd.dy)
        self.fightLayer:moveWithBG(cmd.dx, cmd.dy)
        return
    end
    
    if cmd.t == "jx" then
        self.ui:addJXPower(cmd)
        return
    end
    
    if cmd.t == "global_s" then
        self.ui:setVisible(false)
    elseif cmd.t == "globaled" then
        self.ui:setVisible(true)
    end
    
    if cmd.t == "atk_s" and cmd.sid then
        self.ui:addBigSkillPower({soulId=cmd.sid, powerRate=cmd.pr})
    end
    
    if cmd.t == "silence_s" then
        self.ui:startSilence(cmd.sid)
    end
    
    if cmd.t == "silence_o" then
        self.ui:stopSilence(cmd.sid)
    end
    
    if cmd.t == "die" and cmd.direction==1 then
        self.ui:die(cmd.sid)
    end
    
    if cmd.t == "big_r" and cmd.direction==1 then
        self.ui:disableAllBigSkill()
    end
    
    if cmd.t == "big_s" and cmd.direction==1 then
        self.ui:resetBigSkillPower(cmd.sid)
        --self.ui:disableAllBigSkill()
    end
    
    if cmd.t == "big_o" then
        self.ui:enableAllBigSkill()
        return
    end
    
    if cmd.t == "walk" then
        --self.ui:disableBigSkill(cmd.sid)
    end
    
    if cmd.t == "walk_o" then
        --self.ui:enableBigSkill(cmd.sid)
        return
    end
 
    if cmd.t == "suspend_s" then
        self.bg:pause()
        self.ui:disableBigSkill(cmd.sid)
    end
    
    if cmd.t == "suspend_o" then
        self.bg:resume()
        self.ui:enableBigSkill(cmd.sid)
    end
    
    self.fightLayer:actCMD(cmd)
end

function ActorAgent:onBattleOver(param)
    print("[ActorAgent] Enter onBattleOver")
    self.ui:setTouchEnabled(false)
    self.ui:setVisible(false)
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
    local pauseBtn = self.ui:getChildByName("pauseBtn")
    pauseBtn:setEnabled(false)
    --print("[ActorAgent] Start NewBattleFinish")
    
    -- if emulate battle mode, then return back to home scene
    if self.isEmulate == true then
        Helper.setTimeout(self, 30, function()
            local scene = require("scene.home.HomeScene"):create()
            cc.Director:getInstance():replaceScene(scene) 

            --guard tag--
            guardCenter:jumpNextGuardStage()
            guardCenter:isJump2Stage()
        end)

        return
    end
    
    Http: req("NewBattleFinish",
        {endId = math.floor((self.fId-34)/31) ,leftAtk = param.leftAtks,leftDef = param.leftDefs,leftSeconds = param.leftTime},
        function(res)
            if res.err then cclog("err!!!") return end
            --local winnner = nil
            print("[ActorAgent] Finish NewBattleFinish")
            self.resultLayer = BattleResultLayer:create(res.data)
            self.ui:removeFromParent()
            self.ui = nil
            self.fightLayer:hideColorLayer()
            print("[ActorAgent] Enter resultLayer")
            --[[local PveStageGuild = require "scene.pve.PVEUILayer"
            local stageTag = PveStageGuild:getChallengeStageId()
            print("stageTag_"..stageTag)
            self.pStage = Roler:get("roleId").."_integerTag".."_pStage"
            local pvestage = cc.UserDefault:getInstance():getIntegerForKey(self.pStage)
            if pvestage == nil or pvestage == "" then
                cc.UserDefault:getInstance():setIntegerForKey(self.pStage,1)
                pvestage = cc.UserDefault:getInstance():getIntegerForKey(self.pStage)
            end
            self.pveNum = pvestage
            print("pvenum_n"..self.pveNum)
            if self.pveNum == stageTag then
                self.pveNum = self.pveNum + 1
                cc.UserDefault:getInstance():setIntegerForKey(self.pStage,self.pveNum)
            end
            print("pvenum_"..self.pveNum)
            --关卡 判断是否首次通过
            if res.data.win == 1 and stageTag == 3 then
                if self.pveNum == 4 then
                    guardCenter:guildPveStage()
                end
            elseif  res.data.win == 1 and stageTag == 4 then 
                if self.pveNum == 5 then
                    guardCenter:guildPveStage()
                end 
            elseif  res.data.win == 1 and stageTag == 5 then 
                if self.pveNum == 6 then
                    guardCenter:guildPveStage()
                end 
            elseif  res.data.win == 1 and stageTag == 6 then 
                if self.pveNum == 7 then
                    guardCenter:guildPveStage()
                end
            end--]]
            
            if guardCenter:canEnterGuardMode("pve") == true
                or guardCenter:canEnterGuardMode("pve2") == true
                then
                guardCenter:isJump2Stage()
            end
            
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
        self.ui:setVisible(false)
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

function ActorAgent:setUIHp(soulId, hpRate)
    self.ui:setHp(soulId, hpRate)
end

------------------------[[Internal function]]------------------------
function ActorAgent:ctor()
end

function ActorAgent:onEnter()
end

function ActorAgent:onExit()
end

function ActorAgent:initFirst(param)
    self.bg = BG:create(param)
    self:addChild(self.bg:getNodeGrid())
end

function ActorAgent:initM(param)
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)
    
    --1. background layer
    --[[
    if  param then
        self.bg = BG:create(param)
        self:addChild(self.bg:getNodeGrid())
    else
        self.bg = BG:create()
        self:addChild(self.bg:getNodeGrid())
    end
    ]]
    
    --2. team layer
    self.fightLayer = FightLayer:create()
    self:addChild(self.fightLayer:getNodeGrid())
    
    --3. UI layer
    self.fId = param.fId or 1
    self.isEmulate = param.isEmulate or false
    self.ui = UI:create(param)
    self:addChild(self.ui)

    --[[
    if param and param.fightMode ~= true then
        self.ui = UI:create(param)
        self:addChild(self.ui)
        self.fId = param.fId --fieldId
    end
    ]]
    self.isBattleOver = false --battle over tag
end

return ActorAgent
