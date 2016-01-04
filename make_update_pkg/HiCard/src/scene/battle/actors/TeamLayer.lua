require("Cocos2d")

local NpcActor = require "scene.battle.actors.NpcActor"
local BattleBgLayer =  require "scene.battle.actors.BattleBgLayer"
local BattleUiLayer =  require "scene.battle.actors.BattleUILayer"

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

function TeamLayer:reset(param)
    self:removeAllChildren()
    self.teamLBook = param.teamLBook
    self.teamL = self:addTeamNpcsHelper(self.teamLBook)
    self.teamRBook = param.teamRBook
    self.teamR = self:addTeamNpcsHelper(self.teamRBook)
    
    self:registerHandlers()
end

function TeamLayer:addTeamNpcsHelper(teamBook)
    local team = {} 
    for key, npcBook in pairs(teamBook:getNpcBooks()) do
        if npcBook then
            team[npcBook.key] = NpcActor:create({npcBook=npcBook})
            self:addChild(team[npcBook.key])
        end
    end
    return team
end

function TeamLayer:calShootAngleHelper(actor, targetActor)
    local radian = cc.pGetAngle(cc.pSub(targetActor:getHurtPosition(), actor:getFirePosition()), {x=actor:getDirection(),y=0})
    return math.radian2angle(radian)
end

function TeamLayer:getNpcActor(key)
    return self.teamL[key] or self.teamR[key]
end

function TeamLayer:actCMD(CMD)
    local handler = self.cmdHandlers[CMD.cmdId]
    if handler then
        handler(CMD)
    end
end

--[[CMD and Handler]]--
function TeamLayer:registerHandlers()
    --[[stand]]--
    local function stand(CMD)
        --print("stand handler!")
        local actor = self:getNpcActor(CMD.npcKey)
        if actor then
            actor:actCMD(CMD)
        end
    end
    
    --[[walk]]--
    local function walk(CMD)
        --print("walk_handler")
        local actor = self:getNpcActor(CMD.npcKey)
        actor:actCMD(CMD)
    end
    
    --[[die]]--
    local function die(CMD)
        --print("die_handler")
        local actor = self:getNpcActor(CMD.npcKey)
        actor:actCMD(CMD)
    end
    
    --[[dizzy]]--
    local function dizzy(CMD)
        --print("dizzy_handler")
        local actor = self:getNpcActor(CMD.npcKey)
        actor:actCMD(CMD)
    end
    
    --[[fallback]]--
    local function fallback(CMD)
        --print("fallback_handler")
        local actor = self:getNpcActor(CMD.npcKey)
        actor:actCMD(CMD)
    end
    
    --[[normalAttack]]--
    local function normalAttack(CMD)
        --print("normalAttack_handler")
        local actor = self:getNpcActor(CMD.npcKey)
        local name = actor.npcBook.name
        if name == "DuanGuZhe" then
            print("SUBHP:"..CMD.subHpVal)
        end
        --cclog("[TeamLayer]fetch normalAttack handler for "..name)
        local handler = self.attackHandlers[name]["normalAttack"]
        handler(CMD)
        --[[
        if handler then
            handler(CMD)
        else
            cclog("[TeamLayer]no normalAttack CMD handler for "..name)
        end
        ]]--
    end
    
    --[[SmallSkillAttack]]--
    local function smallSkillAttack(CMD)
        local actor = self:getNpcActor(CMD.npcKey)
        local name = actor.npcBook.name
        local handler = self.attackHandlers[name]["smallSkillAttack"]
        if handler then
            handler(CMD)
        else
            cclog("[TeamLayer]no smallSkillAttack CMD handler for "..name)
        end
    end
    
    --[[BigSkillAttack]]--
    local function bigSkillAttack(CMD)
        local actor = self:getNpcActor(CMD.npcKey)
        local name = actor.npcBook.name
        local handler = self.attackHandlers[name]["bigSkillAttack"]
        if handler then
            handler(CMD)
        else
            cclog("[TeamLayer]no bigSkillAttack CMD handler for "..name)
        end
    end
    
    --[[globalSkillAttack]]--
    local function globalSkillAttack(CMD)
        local visibleSize = cc.Director:getInstance():getVisibleSize()
        local colorLayer = BattleUiLayer:getCoverLayer()
        colorLayer:setVisible(true)
        print("colorLayer:setVisible(true)")
        
        local armature =nil
       
        local effectMusicPath = cc.FileUtils:getInstance():fullPathForFilename("sound/globalSkill_thunder.mp3") 
      
        --cc.SimpleAudioEngine:getInstance():preloadEffect(effectMusicPath)

        local function  func()
            --armature的tag 9999
            colorLayer:getChildByTag(9999):getAnimation():play("Animation1")
        end
        
        local function hurt()
            for i=1,CMD.duration do
                for _, actor in pairs(self.teamR) do
                    print("[globalSkill]normalAttackHurt")
                    actor:normalAttackHurt(CMD.subHpVal/CMD.duration,"thunder")
                end
                --[[
                self:runAction(cc.Sequence:create(cc.DelayTime:create(1), cc.CallFunc:create(function()
                    for _, actor in pairs(self.teamR) do
                        actor:normalAttackHurt(CMD.subHpVal/CMD.duration,"thunder")
                    end
                end)))
                ]]--
            end
        end
        
        local function colorFunc()
            colorLayer:setVisible(false)
        end
        
        local seq1 = cc.Sequence:create(cc.CallFunc:create(hurt),cc.DelayTime:create(1.0))
        local seq2 = cc.Repeat:create(seq1,CMD.duration)
        local sequence = cc.Sequence:create(cc.CallFunc:create(func),seq2,cc.CallFunc:create(colorFunc))
       
             
        local function coverAndHurt()
            colorLayer:runAction(sequence)         
            cc.SimpleAudioEngine:getInstance():playEffect(effectMusicPath)
            colorLayer:removeChild(armature)
        end
        
        local size = cc.size(1,1)
        local shake = cc.Shaky3D:create(CMD.duration/2,size,3, false)
        local seq = cc.Sequence:create(shake,shake:reverse())
        local spwan = cc.Spawn:create(cc.CallFunc:create(coverAndHurt),seq)
        BattleBgLayer:getNodeGrid():runAction(spwan)
        
        
        
        for _, actor in pairs(self.teamL) do
            actor:actCMD(CMD)
        end
        for _, actor in pairs(self.teamR) do
            actor:actCMD(CMD)
        end
    end
    
    self.cmdHandlers = {
        ["stand"] = stand,
        ["walk"] = walk,
        ["die"] = die,
        ["normalAttack"] = normalAttack,
        ["smallSkillAttack"] = smallSkillAttack,
        ["bigSkillAttack"] = bigSkillAttack,
        ["globalSkillAttack"] = globalSkillAttack,
        ["dizzy"] = dizzy,
        ["fallback"] = fallback
    }
    
    self:registerAttackHandlers()
end

--[[attack handler]]--
function TeamLayer:registerAttackHandlers()
    self.attackHandlers = {
        -----------------[[captain]]------------------------
        ["Captain"] = {
            ["normalAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
               
                local readyTime = actor:getNormalAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))
                
                local music = actor.npcBook.normalMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["smallSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                --targetActor:normalAttackHurt(CMD.subHpVal)
                

                local readyTime = actor:getSmallSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))
                
                local music = actor.npcBook.smallMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["bigSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local visibleSize = cc.Director:getInstance():getVisibleSize()
                local ship = cc.Sprite:create()
                ship:setScaleX(actor:getScaleX())
                ship:setScaleY(actor:getScaleY())
                local moveByPos = nil
                if  actor.npcBook.direction == 1 then
                    ship:setPosition(visibleSize.width*0.2,visibleSize.height*0.6)
                    --moveByPos = targetActor:getPositionX()-actor:getPositionX()+visibleSize.width*0.5
                    moveByPos = cc.p(visibleSize.width*0.8,visibleSize.height*0.6)
                else
                    ship:setPosition(visibleSize.width*0.8,visibleSize.height*0.6)
                    moveByPos = cc.p(visibleSize.width*0.2,visibleSize.height*0.6)
                    --moveByPos = targetActor:getPositionX()-actor:getPositionX()+visibleSize.width*0.5
                    --moveByPos = -moveByPos
                end 
                --ship:setRotation(self:calShootAngleHelper(actor,targetActor))
                ship:setVisible(false)
                self:addChild(ship,10000)

                Helper.addSpriteFrames("battle/skill/captain/bigSkillAttack_captain.plist","battle/skill/captain/bigSkillAttack_captain.png")
                local frameArr = Helper.newFrames("chuan000%d.png",1,7)
                local animation = Helper.newAnimation(frameArr,0.5/7)

                local readyTime = actor:getBigSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function()
                    if actor:isBigSkillAttackOk() then
                        ship:setVisible(true)
                    else
                        ship:stopAllActions()
                        self:removeChild(ship)
                    end
                end)
                --local moveBy = cc.MoveBy:create(CMD.duration-readyTime,cc.p(moveByPos,0))
                --local spwan = cc.Spawn:create(moveBy,cc.Animate:create(animation))
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime,moveByPos)
                local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
                local attackOver = cc.CallFunc:create(function()
                    ship:stopAllActions()
                    self:removeChild(ship)
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                ship:runAction(cc.Sequence:create(delay, attackStart, spwan, attackOver))
                
                local music = actor.npcBook.bigMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end
        },
        
        -----------------[[DuanGuZhe]]------------------------
        ["DuanGuZhe"] = {
            ["normalAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local readyTime = actor:getNormalAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                --self:runAction(cc.Sequence:create(delay, attackOver))
                if targetActor.npcBook.name == "Captain" then
                    print("Call normalAttackHurt for Captain......")
                end
                targetActor:normalAttackHurt(CMD.subHpVal)
                           
                local music = actor.npcBook.normalMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["smallSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local readyTime = actor:getSmallSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))
                
                local music = actor.npcBook.smallMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["bigSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local readyTime = actor:getBigSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))
                
                local music = actor.npcBook.bigMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end
        },
        
        -----------------[[FanKeLiFu]]------------------------
        ["FanKeLiFu"] = {
            ["normalAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local readyTime = actor:getNormalAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))

                local music = actor.npcBook.normalMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["smallSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local readyTime = actor:getSmallSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))
                
                local music = actor.npcBook.smallMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["bigSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local sprite = cc.Sprite:create()
                sprite:setScaleX(actor:getScaleX())
                sprite:setScaleY(actor:getScaleY())
                sprite:setPosition(targetActor:getFirePosition())
                sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
                sprite:setVisible(false)
                self:addChild(sprite, 10000)

                Helper.addSpriteFrames("battle/skill/FanKeLiFu/bigSkillAttack_fankelifu.plist","battle/skill/FanKeLiFu/bigSkillAttack_fankelifu.png")
                local frameArr = Helper.newFrames("dahzao%d.png",1,13)
                local animation = Helper.newAnimation(frameArr,0.4/13)

                local readyTime = actor:getBigSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
                local animation = cc.Animate:create(animation)
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(sprite)
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                sprite:runAction(cc.Sequence:create(delay, attackStart, animation, attackOver))
                
                local music = actor.npcBook.bigMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end
        },
        
    -----------------[[FengXing]]------------------------
        ["FengXing"] = {
            ["normalAttack"] = function (CMD)
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local arrow = cc.Sprite:create("image/battle/skill/FengXing/jian.png")
                arrow:setScaleX(actor:getScaleX())
                arrow:setScaleY(actor:getScaleY())
                arrow:setPosition(actor:getFirePosition())
                arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
                arrow:setVisible(false)
                arrow:setLocalZOrder(targetActor:getLocalZOrder())
                self:addChild(arrow, 10000)
                
                local readyTime = actor:getNormalAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetActor:getHurtPosition())
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(arrow)
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                arrow:runAction(cc.Sequence:create(delay, attackStart, moveTo, attackOver))
                
                local music = actor.npcBook.normalMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,
    
            ["smallSkillAttack"] = function (CMD)
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                for i=1,5 do
                    local arrow = cc.Sprite:create("image/battle/skill/FengXing/jian.png")  
                    arrow:setScaleX(actor:getScaleX())
                    arrow:setScaleY(actor:getScaleY())
                    arrow:setPosition(actor:getFirePosition())
                    arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
                    arrow:setVisible(false)
                    arrow:setLocalZOrder(targetActor:getLocalZOrder())
                    self:addChild(arrow, 10000)

                    local readyTime = actor:getSmallSkillAttackReadyTime()
                    local delay = cc.DelayTime:create(readyTime+0.1*i)
                    local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                    local moveTo = cc.MoveTo:create(CMD.duration-readyTime-0.1*i, targetActor:getHurtPosition())
                    local attackOver = cc.CallFunc:create(function()
                        self:removeChild(arrow)
                        targetActor:normalAttackHurt(CMD.subHpVal)
                    end)
                    arrow:runAction(cc.Sequence:create(delay, attackStart, moveTo, attackOver))
                    
                    local music = actor.npcBook.smallMusic
                    cc.SimpleAudioEngine:getInstance():playEffect(music)
                end
            end,
    
            ["bigSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local tmpX = 0
                if actor:getDirection() == 1 then
                    tmpX = WIN_SIZE.width
                else
                    tmpX = 0
                end
                local targetPos = {x=tmpX, y=actor:getPositionY()}
                local arrow = cc.Sprite:create()
                arrow:setScaleX(actor:getScaleX())
                arrow:setScaleY(actor:getScaleY())
                arrow:setPosition(actor:getFirePosition())
                --arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
                arrow:setVisible(false)
                self:addChild(arrow, 10000)
                
                Helper.addSpriteFrames("battle/skill/FengXing/bigSkillAttack_fengxing.plist","battle/skill/FengXing/bigSkillAttack_fengxing.png")
                local frameArr = Helper.newFrames("c000%d.png",1,8)
                local animation = Helper.newAnimation(frameArr,0.2/8)
                
                local readyTime = actor.npcBook.bigSkillReadyTime
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetPos)
                local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(arrow)
                end)
                arrow:runAction(cc.Sequence:create(delay, attackStart, spwan, attackOver))
                
                local music = actor.npcBook.bigMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end
        },
    
        -----------------[[HuaiTe]]---------------------------
        ["HuaiTe"] = {
            ["normalAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)

                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local sprite = cc.Sprite:create()
                sprite:setScaleX(actor:getScaleX())
                sprite:setScaleY(actor:getScaleY())
                sprite:setPosition(actor:getFirePosition())
                sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
                sprite:setVisible(false)
                sprite:setLocalZOrder(targetActor:getLocalZOrder())
                self:addChild(sprite, 10000)

                Helper.addSpriteFrames("battle/skill/huaite/normalSkillAttack_huaite.plist","battle/skill/huaite/normalSkillAttack_huaite.png")
                local frameArr = Helper.newFrames("guangqiu%d.png",0,5)
                local animation = Helper.newAnimation(frameArr,0.2/6)

                local readyTime = actor:getNormalAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetActor:getHurtPosition())
                local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(sprite)
                    targetActor:normalAttackHurt(CMD.subHpVal,"thunder")
                end)
                sprite:runAction(cc.Sequence:create(delay, attackStart, spwan, attackOver))
                
                local music = actor.npcBook.normalMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["smallSkillAttack"] = function (CMD)
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local actor = self:getNpcActor(CMD.npcKey)

                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local sprite = cc.Sprite:create()
                sprite:setScaleX(actor:getScaleX())
                sprite:setScaleY(actor:getScaleY())
                sprite:setPosition(actor:getFirePosition())
                sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
                sprite:setVisible(false)
                sprite:setLocalZOrder(targetActor:getLocalZOrder())
                self:addChild(sprite, 10000)
                
                local sprite2 = cc.Sprite:create()
                sprite2:setScaleX(actor:getScaleX())
                sprite2:setScaleY(actor:getScaleY())
                sprite2:setPosition(0,0)
                --actor.armature:getContentSize().height*targetActor.npcBook.scaleFactor*0.5
                sprite2:setRotation(self:calShootAngleHelper(actor,targetActor))
                --sprite2:setVisible(false)
                sprite2:setLocalZOrder(targetActor:getLocalZOrder())
                sprite:addChild(sprite2,1)
                
                Helper.addSpriteFrames("battle/skill/huaite/smallSkillAttack_huaite_daoguang.plist","battle/skill/huaite/smallSkillAttack_huaite_daoguang.png")
                local frameArr = Helper.newFrames("huaidaoguang%04d.png",1,5)
                local animation = Helper.newAnimation(frameArr,0.2/5)
                
                Helper.addSpriteFrames("battle/skill/huaite/smallSkillAttack_huaite_dimian.plist","battle/skill/huaite/smallSkillAttack_huaite_dimian.png")
                local frameArr2 = Helper.newFrames("HUOHUA%d.png",0,7)
                local animation2 = Helper.newAnimation(frameArr2,0.2/8)
                
                local readyTime = actor:getSmallSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
                local attackStart2 = cc.CallFunc:create(function() sprite2:setVisible(true) end)
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetActor:getHurtPosition())
                local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
                local spwan2 = cc.Spawn:create(moveTo,cc.Animate:create(animation2))
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(sprite)
                    targetActor:normalAttackHurt(CMD.subHpVal,"thunder")
                end)
                local attackOver2 = cc.CallFunc:create(function()
                    self:removeChild(sprite2)
                end)
                sprite2:runAction(cc.Animate:create(animation2))
                sprite:runAction(cc.Sequence:create(delay, attackStart, spwan, attackOver))
                
                local music = actor.npcBook.smallMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
                
            end,

            ["bigSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)

                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local sprite = cc.Sprite:create()
                sprite:setScaleX(actor:getScaleX())
                sprite:setScaleY(actor:getScaleY())
                sprite:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()-targetActor.armature:getContentSize().height*targetActor.npcBook.scaleFactor*0.5))
                sprite:setVisible(false)
                sprite:setAnchorPoint(cc.p(0.5,0))
                sprite:setLocalZOrder(targetActor:getLocalZOrder())
                self:addChild(sprite, 10000)
                
                local readyTime = actor:getBigSkillAttackReadyTime()
                Helper.addSpriteFrames("battle/skill/huaite/bigSkillAttack_huaite.plist","battle/skill/huaite/bigSkillAttack_huaite.png")
                local frameArr = Helper.newFrames("shandian%04d.png",1,25)
                local animation = Helper.newAnimation(frameArr,0.5/25)

                
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
                local anmi = cc.Animate:create(animation)
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(sprite)
                    targetActor:normalAttackHurt(CMD.subHpVal,"thunder")
                end)
                sprite:runAction(cc.Sequence:create(delay, attackStart, anmi, attackOver))
                
                local music = actor.npcBook.bigMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end       
        },

        -----------------[[KuLouWang]]---------------------------
        ["KuLouWang"] = {
            ["normalAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local readyTime = actor:getNormalAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))
                
                local music = actor.npcBook.normalMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["smallSkillAttack"] = function (CMD)
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local readyTime = actor:getSmallSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))
                
                local music = actor.npcBook.smallMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["bigSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local arrow = cc.Sprite:create()
                arrow:setScaleX(actor:getScaleX())
                arrow:setScaleY(actor:getScaleY())
                arrow:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()+targetActor.armature:getContentSize().height*(targetActor.npcBook.scaleFactor*0.5)))
                arrow:setAnchorPoint(cc.p(0.5,0.5))
                arrow:setVisible(false)
                self:addChild(arrow, 10000)

                Helper.addSpriteFrames("battle/skill/KuLouWang/dazhao.plist","battle/skill/KuLouWang/dazhao.png")
                local frameArr = Helper.newFrames("kulbao%d.png",9,12)
                local animation = Helper.newAnimation(frameArr,1.4/12)

                local readyTime = actor:getBigSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                --    local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetActor:getHurtPosition())
                --    local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(arrow)
                    targetActor:normalAttackHurt(CMD.subHpVal,"fire")
                end)
                arrow:runAction(cc.Sequence:create(delay, attackStart,cc.Animate:create(animation), attackOver))

                local music = actor.npcBook.bigMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end       
        },

        -----------------[[LaZuoKeLao]]---------------------------
        ["LaZuoKeLao"] = {
            ["normalAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local readyTime = actor:getNormalAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))
                
                local music = actor.npcBook.normalMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["smallSkillAttack"] = function (CMD)
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local readyTime = actor:getSmallSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))
                
                local music = actor.npcBook.smallMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["bigSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local readyTime = actor:getBigSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackOver = cc.CallFunc:create(function()
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                self:runAction(cc.Sequence:create(delay, attackOver))
                local music = actor.npcBook.bigMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end       
        },
        -----------------[[Lina]]---------------------------
        ["Lina"] = {
            ["normalAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local arrow = cc.Sprite:create()
                arrow:setScaleX(actor:getScaleX())
                arrow:setScaleY(actor:getScaleY())
                arrow:setPosition(actor:getFirePosition())
                arrow:setLocalZOrder(targetActor:getLocalZOrder())

                arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
                arrow:setVisible(false)
                self:addChild(arrow, 10000)

                Helper.addSpriteFrames("battle/skill/LiNa/xiaohuoqiu.plist","battle/skill/LiNa/xiaohuoqiu.png")
                local frameArr = Helper.newFrames("xiaohuoqiu%d.png",1,8)
                local animation = Helper.newAnimation(frameArr,0.2/8)

                local readyTime = actor:getNormalAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetActor:getHurtPosition())
                local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(arrow)
                    targetActor:normalAttackHurt(CMD.subHpVal,"fire")
                end)
                arrow:runAction(cc.Sequence:create(delay, attackStart, spwan, attackOver))
                
                local music = actor.npcBook.normalMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["smallSkillAttack"] = function (CMD)
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local arrow = cc.Sprite:create()
                arrow:setScaleX(actor:getScaleX())
                arrow:setScaleY(actor:getScaleY())
                arrow:setPosition(actor:getFirePosition())
                arrow:setLocalZOrder(targetActor:getLocalZOrder())

                arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
                arrow:setVisible(false)
                self:addChild(arrow, 10000)

                Helper.addSpriteFrames("battle/skill/LiNa/dahuoqiu.plist","battle/skill/LiNa/dahuoqiu.png")
                local frameArr = Helper.newFrames("huoqiu%d.png",1,16)
                local animation = Helper.newAnimation(frameArr,0.2/16)

                local readyTime = actor:getSmallSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetActor:getHurtPosition())
                local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(arrow)
                    targetActor:normalAttackHurt(CMD.subHpVal,"fire")
                end)
                arrow:runAction(cc.Sequence:create(delay, attackStart, spwan, attackOver))
                
                local music = actor.npcBook.smallMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["bigSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local arrow = cc.Sprite:create()
                arrow:setScaleX(actor:getScaleX())
                arrow:setScaleY(actor:getScaleY())
                arrow:setLocalZOrder(targetActor:getLocalZOrder())
                
                arrow:setPosition(actor:getFirePosition())
                
               
           --     arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
                arrow:setVisible(false)
           --     arrow:setScale(0.7)
                self:addChild(arrow, 10000)

                Helper.addSpriteFrames("battle/skill/LiNa/fenghuang.plist","battle/skill/LiNa/fenghuang.png")
                local frameArr = Helper.newFrames("fenghuang%d.png",13,12)
                local animation = Helper.newAnimation(frameArr,1/12)

                local readyTime = actor:getBigSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                local point = actor:getFirePosition()
                
                local visibleSize = cc.Director:getInstance():getVisibleSize()
                local moveByPos = nil
                if  actor.npcBook.direction == 1 then
                    moveByPos = 400
                else
                    moveByPos = -400
                end 

               
                local moveBy = cc.MoveBy:create(0.8, cc.p(moveByPos,0))
                local spwan = cc.Spawn:create(moveBy,cc.Animate:create(animation))
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(arrow)
                    targetActor:normalAttackHurt(CMD.subHpVal,"fire")
                end)
                arrow:runAction(cc.Sequence:create(delay, attackStart, spwan, attackOver))
                
                local music = actor.npcBook.bigMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end       
        },
        
        -----------------[[XiaoHei]]---------------------------
        ["XiaoHei"] = {
            ["normalAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local arrow = cc.Sprite:create("image/battle/skill/XiaoHei/jian.png")
                arrow:setScaleX(actor:getScaleX())
                arrow:setScaleY(actor:getScaleY())
                arrow:setPosition(actor:getFirePosition())
                arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
                arrow:setVisible(false)
                arrow:setLocalZOrder(targetActor:getLocalZOrder())
                self:addChild(arrow, 10000)

                local readyTime = actor:getNormalAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetActor:getHurtPosition())
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(arrow)
                    targetActor:normalAttackHurt(CMD.subHpVal)
                end)
                arrow:runAction(cc.Sequence:create(delay, attackStart, moveTo, attackOver))
                
                local music = actor.npcBook.normalMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["smallSkillAttack"] = function (CMD)
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local arrow = cc.Sprite:create()
                arrow:setScaleX(actor:getScaleX())
                arrow:setScaleY(actor:getScaleY())
                arrow:setPosition(actor:getFirePosition())
                arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
                arrow:setVisible(false)
                self:addChild(arrow, 10000)
                arrow:setLocalZOrder(targetActor:getLocalZOrder())
                arrow:setAnchorPoint(cc.p(1,0.5))

                Helper.addSpriteFrames("battle/skill/XiaoHei/zidan.plist","battle/skill/XiaoHei/zidan.png")
                local frameArr = Helper.newFrames("zidan%d.png",1,8)
                local animation = Helper.newAnimation(frameArr,0.2/8)

                local readyTime = actor:getSmallSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetActor:getHurtPosition())
                local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(arrow)
                    targetActor:normalAttackHurt(CMD.subHpVal,"ice")
                end)
                arrow:runAction(cc.Sequence:create(delay, attackStart, spwan, attackOver))
                
                local music = actor.npcBook.smallMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["bigSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                --箭的角度  左边 -60顺时针旋转120  右边-120逆时针旋转120
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                
                local rotateAngle = 0
                local arrowAngle = 0
                if  actor.npcBook.direction == 1 then 
                    arrowAngle = -60
                    rotateAngle = 120
                else
                    arrowAngle = -120
                    rotateAngle = -120
                end
                for i=1,5 do --弓箭数5
                    local arrow = cc.Sprite:create("image/battle/skill/XiaoHei/jian.png")  
                    --arrow:setScaleX(actor:getScaleX())
                    local firePosition = cc.p(actor:getFirePosition().x,actor:getFirePosition().y+20)
                    arrow:setScaleY(actor:getScaleY())
                    --arrow:setPosition(actor:getFirePosition())
                    arrow:setPosition(firePosition)
                    arrow:setVisible(false)
                    arrow:setLocalZOrder(targetActor:getLocalZOrder())
                    self:addChild(arrow, 10000)             
                    
                    
                    
                    local readyTime = actor:getSmallSkillAttackReadyTime()
                    local delay = cc.DelayTime:create(readyTime+0.2*i) --0.2箭的延迟时间
                    local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                    local moveTo = cc.MoveTo:create(CMD.duration-readyTime-0.2*i, targetActor:getHurtPosition())
                    
                    arrow:setRotation(arrowAngle)
                    local arrowParabola = MyDIYAction.arrowParabola(firePosition,targetActor:getHurtPosition(),rotateAngle,CMD.duration-readyTime-0.1*i,
                                                                    arrow:getContentSize().height,300)
                    
                    local attackOver = cc.CallFunc:create(function()
                        self:removeChild(arrow)
                        targetActor:normalAttackHurt(CMD.subHpVal)
                    end)
                    arrow:runAction(cc.Sequence:create(delay, attackStart, arrowParabola, attackOver))

                    local music = actor.npcBook.smallMusic
                    cc.SimpleAudioEngine:getInstance():playEffect(music)
                end
            end       
        },
    
        -----------------[[ZhuRiZhe]]---------------------------
        ["ZhuRiZhe"] = {
            ["normalAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local arrow = cc.Sprite:create()
                arrow:setScaleX(actor:getScaleX())
                arrow:setScaleY(actor:getScaleY())
                arrow:setPosition(actor:getFirePosition())
                arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
                arrow:setVisible(false)
                arrow:setLocalZOrder(targetActor:getLocalZOrder())
                self:addChild(arrow, 10000)

                Helper.addSpriteFrames("battle/skill/LiNa/xiaohuoqiu.plist","battle/skill/LiNa/xiaohuoqiu.png")
                local frameArr = Helper.newFrames("xiaohuoqiu%d.png",1,8)
                local animation = Helper.newAnimation(frameArr,0.2/8)

                local readyTime = actor:getNormalAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetActor:getHurtPosition())
                local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(arrow)
                    targetActor:normalAttackHurt(CMD.subHpVal,"fire")
                end)
                arrow:runAction(cc.Sequence:create(delay, attackStart, spwan, attackOver))
                
                local music = actor.npcBook.normalMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["smallSkillAttack"] = function (CMD)
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                if targetActor == nil then return end
                local arrow = cc.Sprite:create()
                --arrow:setAnchorPoint(cc.p(1,0.5))
                arrow:setScaleX(actor:getScaleX())
                arrow:setScaleY(actor:getScaleY())
                arrow:setPosition(actor:getFirePosition())
                arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
                arrow:setVisible(false)
                arrow:setLocalZOrder(targetActor:getLocalZOrder())
                self:addChild(arrow, 10000)

                Helper.addSpriteFrames("battle/skill/LiNa/dahuoqiu.plist","battle/skill/LiNa/dahuoqiu.png")
                local frameArr = Helper.newFrames("huoqiu%d.png",1,16)
                local animation = Helper.newAnimation(frameArr,0.2/16)
                
                local readyTime = actor:getSmallSkillAttackReadyTime()
                --让火球后点移动到发招点   火球长度:250
                --local delay = cc.MoveBy:create(readyTime,cc.p(250*actor.npcBook.direction,0))
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function()  arrow:setVisible(true) end)
                
                local moveTo = cc.MoveTo:create(CMD.duration-readyTime, targetActor:getHurtPosition())
                local flyTime = CMD.duration-readyTime
                local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(arrow)
                    targetActor:normalAttackHurt(CMD.subHpVal,"fire")
                end)
                arrow:runAction(cc.Sequence:create(delay, attackStart, spwan, attackOver))
                
                local music = actor.npcBook.smallMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end,

            ["bigSkillAttack"] = function (CMD) 
                local actor = self:getNpcActor(CMD.npcKey)
                actor:actCMD(CMD)
                --施法目标： 自己或队友 现在在敌方
                local targetActor = self:getNpcActor(CMD.targetNpcKey)
                --targetActor:normalAttackHurt(CMD.subHpVal)
                
                local sprite = cc.Sprite:create()
                sprite:setAnchorPoint(cc.p(0.5,0))
                sprite:setScaleX(actor:getScaleX())
                sprite:setScaleY(actor:getScaleY())
                sprite:setPosition(cc.p(actor:getPositionX(),actor:getPositionY()))
                --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
                sprite:setVisible(false)
                sprite:setLocalZOrder(targetActor:getLocalZOrder())
                self:addChild(sprite, 10000)

                Helper.addSpriteFrames("battle/skill/zhurizhe/bigSkillAttack_zhurizhe.plist","battle/skill/zhurizhe/bigSkillAttack_zhurizhe.png")
                local frameArr = Helper.newFrames("xunhuangdunp%d.png",1,8)
                local animation = Helper.newAnimation(frameArr,0.2/8)

                local readyTime = actor:getBigSkillAttackReadyTime()
                local delay = cc.DelayTime:create(readyTime)
                local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
                local attackOver = cc.CallFunc:create(function()
                    self:removeChild(sprite)
                    targetActor:normalAttackHurt(CMD.subHpVal,"ice")
                end)
                sprite:runAction(cc.Sequence:create(delay, attackStart,cc.Animate:create(animation),cc.FadeOut:create(CMD.duration-readyTime), attackOver))
            
                local music = actor.npcBook.bigMusic
                cc.SimpleAudioEngine:getInstance():playEffect(music)
            end            
        },
    
    -----------------[[add new hero here]]----------------
    
    } -- table
end -- funcion

return TeamLayer

