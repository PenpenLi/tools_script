require("Cocos2d")

local NPC = require "scene.battle2.actors.NPC"
local FightLayer = class("FightLayer", function()
    return cc.Layer:create() 
end)

-----------------------[[External API]]-----------------------------
function FightLayer:create()
    local layer = FightLayer.new()
    layer:initM()
    return layer
end

function FightLayer:actCMD(cmd)
    local handler =  self.cmdHandlers[cmd.t]
    assert(handler~=nil,"handler nil")
    handler(cmd)
end

------------------------[[Internal function]]------------------------
function FightLayer:ctor()
end

function FightLayer:onEnter()
end

function FightLayer:onExit()
end

function FightLayer:initM()
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)
    
    self.cmdHandlers = {
        ["createNpc"] = function (cmd) self:createNpc(cmd) end,
        ["removeNpc"] = function (cmd) self:removeNpc(cmd) end,
        ["stand"] = function (cmd) self:stand(cmd) end,
        ["walk"] = function (cmd) self:walk(cmd) end,
        ["move"] = function (cmd) self:move(cmd) end,
        ["atk_r"] = function (cmd) self:atk_r(cmd) end,
        ["atk_s"] = function (cmd) self:atk_s(cmd) end,
        ["atked"] = function (cmd) self:atked(cmd) end,
        ["small_r"] = function (cmd) self:small_r(cmd) end,
        ["small_s"] = function (cmd) self:small_s(cmd) end,
        ["smalled"] = function (cmd) self:smalled(cmd) end,
        ["big_r"] = function (cmd) self:big_r(cmd) end,
        ["big_s"] = function (cmd) self:big_s(cmd) end,
        ["biged"] = function (cmd) self:biged(cmd) end,
        ["die"] = function (cmd) self:die(cmd) end,
        ["persistent_in"] = function (cmd) self:persistent_in(cmd) end,
        ["yun_s"] = function (cmd) self:yun_s(cmd) end,
        ["yun_o"] = function (cmd) self:yun_o(cmd) end,
        ["tui_s"] = function (cmd) self:tui_s(cmd) end,
        ["tui_o"] = function (cmd) self:tui_o(cmd) end,
        ["global_s"] = function (cmd) self:global_s(cmd) end,
        ["global_in"] = function (cmd) self:global_in(cmd) end,
        ["globaled"] = function (cmd) self:globaled(cmd) end,
        ["addHp"] = function (cmd) self:addHp(cmd) end,
        ["ice_s"] = function (cmd) self:ice_s(cmd) end,
        ["ice_o"] = function (cmd) self:ice_o(cmd) end,
        ["silence_s"] = function (cmd) self:silence_s(cmd) end,
        ["silence_o"] = function (cmd) self:silence_o(cmd) end
    }
    
    self.attackHandlers = {
        ["Captain"] = function (param) self:attack_Captain(param) end,
        ["DuanGuZhe"] = function (param) self:attack_DuanGuZhe(param) end,
        ["FanKeLiFu"] = function (param) self:attack_FanKeLiFu(param) end,
        ["FengXing"] = function (param) self:attack_FengXing(param) end,
        ["HuaiTe"] = function (param) self:attack_HuaiTe(param) end,
        ["KuLouWang"] = function (param) self:attack_KuLouWang(param) end,
        ["LaZuoKeLao"] = function (param) self:attack_LaZuoKeLao(param) end,
        ["Lina"] = function (param) self:attack_Lina(param) end,
        ["XiaoHei"] = function (param) self:attack_XiaoHei(param) end,
        ["ZhuRiZhe"] = function (param) self:attack_ZhuRiZhe(param) end,
        ["Jaina"] = function (param) self:attack_Jaina(param) end,
        ["Tyrande"] = function (param) self:attack_Tyrande(param) end,
        ["DiXueLingZhu"] = function (param) self:attack_DiXueLingZhu(param) end,
        ["Kelthuzad"] = function (param) self:attack_Kelthuzad(param) end,
        ["YiLiDan"] = function (param) self:attack_YiLiDan(param) end,
        ["ShuiYuanSu"] = function (param) self:attack_ShuiYuanSu(param) end,
        ["ShahelasiZhuMu"] = function (param) self:attack_ShahelasiZhuMu(param) end,
        ["LaGeNaLuoSi"] = function (param) self:attack_LaGeNaLuoSi(param) end,
        ["AoNiKeXiYa"] = function (param) self:attack_AoNiKeXiYa(param) end,
        ["KeLuoMaGuSi"] = function (param) self:attack_KeLuoMaGuSi(param) end,
        ["MaNuoLuoSi"] = function (param) self:attack_MaNuoLuoSi(param) end,
        ["tonglingta"] = function (param) self:attack_tonglingta(param) end,
        ["Cannon"] = function (param) self:attack_Cannon(param) end
    }
    
    self.smallSkillHandlers = {
        ["Captain"] = function (param) self:smallSkill_Captain(param) end,
        ["DuanGuZhe"] = function (param) self:smallSkill_DuanGuZhe(param) end,
        ["FanKeLiFu"] = function (param) self:smallSkill_FanKeLiFu(param) end,
        ["FengXing"] = function (param) self:smallSkill_FengXing(param) end,
        ["HuaiTe"] = function (param) self:smallSkill_HuaiTe(param) end,
        ["KuLouWang"] = function (param) self:smallSkill_KuLouWang(param) end,
        ["LaZuoKeLao"] = function (param) self:smallSkill_LaZuoKeLao(param) end,
        ["Lina"] = function (param) self:smallSkill_Lina(param) end,
        ["XiaoHei"] = function (param) self:smallSkill_XiaoHei(param) end,
        ["ZhuRiZhe"] = function (param) self:smallSkill_ZhuRiZhe(param) end,
        ["Jaina"] = function (param) self:smallSkill_Jaina(param) end,
        ["Tyrande"] = function (param) self:smallSkill_Tyrande(param) end,
        ["DiXueLingZhu"] = function (param) self:smallSkill_DiXueLingZhu(param) end,
        ["Kelthuzad"] = function (param) self:smallSkill_Kelthuzad(param) end,
        ["YiLiDan"] = function (param) self:smallSkill_YiLiDan(param) end,
        ["ShahelasiZhuMu"] = function (param) self:smallSkill_ShahelasiZhuMu(param) end,
        ["LaGeNaLuoSi"] = function (param) self:smallSkill_LaGeNaLuoSi(param) end,
        ["AoNiKeXiYa"] = function (param) self:smallSkill_AoNiKeXiYa(param) end,
        ["KeLuoMaGuSi"] = function (param) self:smallSkill_KeLuoMaGuSi(param) end,
        ["MaNuoLuoSi"] = function (param) self:smallSkill_MaNuoLuoSi(param) end,
        ["tonglingta"] = function (param) self:smallSkill_tonglingta(param) end,
        ["Cannon"] = function (param) self:smallSkill_Cannon(param) end

    }
    
    self.bigSkillHandlers = {
        ["Captain"] = function (param) self:bigSkill_Captain(param) end,
        ["DuanGuZhe"] = function (param) self:bigSkill_DuanGuZhe(param) end,
        ["FanKeLiFu"] = function (param) self:bigSkill_FanKeLiFu(param) end,
        ["FengXing"] = function (param) self:bigSkill_FengXing(param) end,
        ["HuaiTe"] = function (param) self:bigSkill_HuaiTe(param) end,
        ["KuLouWang"] = function (param) self:bigSkill_KuLouWang(param) end,
        ["LaZuoKeLao"] = function (param) self:bigSkill_LaZuoKeLao(param) end,
        ["Lina"] = function (param) self:bigSkill_Lina(param) end,
        ["XiaoHei"] = function (param) self:bigSkill_XiaoHei(param) end,
        ["ZhuRiZhe"] = function (param) self:bigSkill_ZhuRiZhe(param) end,
        ["Jaina"] = function (param) self:bigSkill_Jaina(param) end,
        ["Tyrande"] = function (param) self:bigSkill_Tyrande(param) end,
        ["DiXueLingZhu"] = function (param) self:bigSkill_DiXueLingZhu(param) end,
        ["Kelthuzad"] = function (param) self:bigSkill_Kelthuzad(param) end,
        ["YiLiDan"] = function (param) self:bigSkill_YiLiDan(param) end,
        ["ShahelasiZhuMu"] = function (param) self:bigSkill_ShahelasiZhuMu(param) end,
        ["LaGeNaLuoSi"] = function (param) self:bigSkill_LaGeNaLuoSi(param) end,
        ["AoNiKeXiYa"] = function (param) self:bigSkill_AoNiKeXiYa(param) end,
        ["KeLuoMaGuSi"] = function (param) self:bigSkill_KeLuoMaGuSi(param) end,
        ["MaNuoLuoSi"] = function (param) self:bigSkill_MaNuoLuoSi(param) end,
        ["tonglingta"] = function (param) self:bigSkill_tonglingta(param) end,
        ["Cannon"] = function (param) self:bigSkill_Cannon(param) end

    }
    
    self.npcs = {}
    
    --遮罩
    self.colorLayer = cc.LayerColor:create(cc.c4b(0,0,0,220))
    self:addChild(self.colorLayer,5000)
    self.colorLayer:setVisible(false)    
end

function FightLayer:resetZOrder()
    table.sort(self.npcs, function(a, b) return math.floor(a:getPositionY()) > math.floor(b:getPositionY())+2 end)
    for i=1, #self.npcs do
        self.npcs[i]:setLocalZOrder(100+i)
    end
end

------------cmd handlers function define------------
function FightLayer:createNpc(cmd)
    local npc = NPC:create(cmd)
    self:addNpc(npc)
    -- check zOrder --
    table.insert(self.npcs, npc)
    self:resetZOrder()
        --local actor = param.fromNpc
    if npc == nil then return end

    local sprite1 = cc.Sprite:create()
    sprite1:setAnchorPoint(cc.p(0.5,0))
    sprite1:setPosition(cc.p(0,-40))
    --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite1:setVisible(false)
    sprite1:setLocalZOrder(npc:getLocalZOrder())
    npc:addChild(sprite1)

    Helper.addSpriteFrames("battle/skill/NPC/npc_showEffect.plist","battle/skill/NPC/npc_showEffect.pvr.ccz")
    local frameArr1 = Helper.newFrames("chuchang%04d.png",1,14)
    local animation1 = Helper.newAnimation(frameArr1,1.0/18)

    sprite1:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation1),cc.FadeOut:create(0.2), cc.RemoveSelf:create()))
    -- check zOrder --
end

function FightLayer:removeNpc(key)
    local npc = self:getNpc(key)
    assert(npc~=nil, "npc not exist")
    -- check zOrder --
    local idx = 0
    for i=1, #self.npcs do
        idx = idx + 1
        if self.npcs[i] == npc then
            break
        end
    end
    table.remove(self.npcs,idx)
    -- check zOrder --
    self:removeChild(npc, false)
end

function FightLayer:stand(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
end

function FightLayer:walk(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
end

function FightLayer:move(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
    --[[
    if cmd.z then
        for k, v in pairs(cmd.z) do
            local tmpNpc = self:getNpc(v.k)
            if tmpNpc then tmpNpc:setLocalZOrder(v.z) end
        end
    end
    ]]
    -- check zOrder --
    self:resetZOrder()
    -- check zOrder --
end

function FightLayer:atk_r(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
end

--[[{t="atk_s", key=self.key, tKey=self.attackTarget.key, dt=self:getAttackDt()}]]--
function FightLayer:atk_s(cmd)
    local npc = self:getNpc(cmd.key)
    local targetNpc = self:getNpc(cmd.tKey)
    if npc then npc:actCMD(cmd) end
    local attackHandler = self.attackHandlers[npc.npcBook.name]
    if attackHandler then
        attackHandler({fromNpc=npc, toNpc=targetNpc, dt=cmd.dt})
    else
        print("No attackHandler for "..npc.npcBook.name)
    end
end

function FightLayer:atked(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) npc:normalAttackHurt(cmd.hp,cmd.eft) end
end

function FightLayer:small_r(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
end

--[[{t="small_s", key=self.key, tKeys=target.key, dt=self:getSmallSkillDt()}]]--
function FightLayer:small_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
    if  cmd.row then npc.row = cmd.row end
    if  cmd.col then npc.col = cmd.col end
    local toNpcs = {}
    for _, v in pairs(cmd.tSet) do
        local item = {}
        item.npc = self:getNpc(v.key)
        item.dt = v.dt
        table.insert(toNpcs, item)
    end
    local attackHandler = self.smallSkillHandlers[npc.npcBook.name]
    attackHandler({fromNpc=npc, toNpcs=toNpcs})
end

function FightLayer:smalled(cmd)
    local fromnpc = self:getNpc(cmd.fKey)
    local npc = self:getNpc(cmd.key)
    if npc then
        local att = nil
        if fromnpc.npcBook.name == "Jaina" then
            att = "bingbao"
        elseif fromnpc.npcBook.name == "KeLuoMaGuSi" then
            att = "chishao"      
        end
        npc:actCMD(cmd) npc:normalAttackHurt(cmd.hp,cmd.eft,att)
    end
end

function FightLayer:big_r(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd)  end
end

function FightLayer:big_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
    if  cmd.row then npc.row = cmd.row end
    if  cmd.col then npc.col = cmd.col end
    local toNpcs = {}
    for _, v in pairs(cmd.tSet) do
        local item = {}
        item.npc = self:getNpc(v.key)
        item.dt = v.dt
        table.insert(toNpcs, item)
    end
    local attackHandler = self.bigSkillHandlers[npc.npcBook.name]
    attackHandler({fromNpc=npc, toNpcs=toNpcs})
end

function FightLayer:persistent_in(cmd)
    local npc = self:getNpc(cmd.key)
    print("persistent_in")
    if npc then
        npc:actCMD(cmd)
        npc:normalAttackHurt(cmd.hp)
    end
end

function FightLayer:biged(cmd)
    local fromnpc = self:getNpc(cmd.fKey)
    local npc = self:getNpc(cmd.key)
     if npc then
       local att = nil
       if fromnpc.npcBook.name == "XiaoHei" or fromnpc.npcBook.name == "DiXueLingZhu" then
          att = "bingbao"    
       end
       npc:actCMD(cmd) npc:normalAttackHurt(cmd.hp,cmd.eft,att)
    end
end

function FightLayer:die(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
    if npc.npcBook.treasuresNum == nil then return end  
    for i=1,npc.npcBook.treasuresNum do
        local sprite = cc.Sprite:create("battle/ui/baoxiang.png")
        sprite:setPosition(npc:getPosition())
        sprite:setLocalZOrder(npc:getLocalZOrder())
        
        local function addnum()
        	actorAgent:addTreasureNum()
        end
        local moveTo = cc.MoveTo:create(1.0,actorAgent:getTreasurePos())
        local action = cc.Sequence:create(moveTo,cc.RemoveSelf:create(),cc.CallFunc:create(addnum))
        sprite:runAction(action)
        self:addChild(sprite)
    end
    
    -- check zOrder --
    local idx = 0
    for i=1, #self.npcs do
        idx = idx + 1
        if self.npcs[i] == npc then
            break
        end
    end
    table.remove(self.npcs,idx)
    -- check zOrder --
end

function FightLayer:yun_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then  npc:actCMD(cmd) npc:setBuffState("yun") end
end

function FightLayer:yun_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) npc:removeBuffState() end
end

function FightLayer:tui_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
       npc:actCMD(cmd)
       local tPos = cmd.tPos
       if tPos.x < 0 then
            tPos.x = 0
       elseif  tPos.x > WIN_SIZE.width then
           tPos.x = WIN_SIZE.width
       end
       local action = cc.Sequence:create(cc.MoveTo:create(cmd.dt/30,tPos))
       npc:runAction(action)
    end
end

function FightLayer:tui_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
end

function FightLayer:global_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) npc:removeBuffState() end
    if  self.globals_seq ~= cmd.seq then
        self.globals_seq = cmd.seq
        self:globalSkillBegin(cmd.skillId,cmd.dt)
    end
    if  npc:getDirection() == -1 then
        npc:setLocalZOrder(npc:getLocalZOrder()+5000)
    end
    
end

function FightLayer:global_in(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
       npc:actCMD(cmd)
       npc:normalAttackHurt(cmd.hp)
    end
end

function FightLayer:globaled(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
    if  self.globaled_seq ~= cmd.seq then
        self.globaled_seq = cmd.seq
        self:globalSkillEnd()
    end
    if  npc:getDirection() == -1 then
        npc:setLocalZOrder(npc:getLocalZOrder()-5000)
    end
end

function FightLayer:addHp(cmd)
    local npc = self:getNpc(cmd.tkey)
    if npc then npc:actCMD(cmd) npc:normalAttackHurt(cmd.hp) end
end

function FightLayer:ice_s(cmd)
    print("ice_s")
    local npc = self:getNpc(cmd.key)
    if npc then  npc:actCMD(cmd) npc:setBuffState("yun") end
end

function FightLayer:ice_o(cmd)
    print("ice_o")
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) npc:removeBuffState() end
end

function FightLayer:silence_s(cmd)
    print("silence_s")
end

function FightLayer:silence_o(cmd)
    print("silence_o")
end

------------attack handlers function define------------
function FightLayer:attack_Captain(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_DuanGuZhe(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_FanKeLiFu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_FengXing(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local arrow = cc.Sprite:create("image/battle/skill/FengXing/jian.png")
    arrow:setScaleX(actor:getScaleX())
    arrow:setScaleY(actor:getScaleY())
    arrow:setPosition(actor:getFirePosition())
    arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
    arrow:setVisible(false)
    arrow:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(arrow)
    local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(arrow)
    end)
    arrow:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))

    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]

end

function FightLayer:attack_HuaiTe(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/huaite/normalSkillAttack_huaite.png")
    sprite:setScaleX(actor:getScaleX())
    sprite:setScaleY(actor:getScaleY())
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)
    
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))

    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
    
end

function FightLayer:attack_KuLouWang(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_LaZuoKeLao(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_Lina(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/LiNa/xiaohuoqiu.png")
    sprite:setScaleX(actor:getScaleX())
    sprite:setScaleY(actor:getScaleY())
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)
    
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_XiaoHei(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local arrow = cc.Sprite:create("image/battle/skill/XiaoHei/jian.png")
    arrow:setScaleX(actor:getScaleX())
    arrow:setScaleY(actor:getScaleY())
    arrow:setPosition(actor:getFirePosition())
    arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
    arrow:setVisible(false)
    arrow:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(arrow)

    local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(arrow)
    end)
    arrow:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
    
end

function FightLayer:attack_ZhuRiZhe(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/LiNa/xiaohuoqiu.png")
    sprite:setScaleX(actor:getScaleX())
    sprite:setScaleY(actor:getScaleY())
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)

    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
    
end

function FightLayer:attack_Jaina(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/Jaina/normalSkillAttack_jaina.png")
    sprite:setScaleX(actor:getScaleX())
    sprite:setScaleY(actor:getScaleY())
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
    
end

function FightLayer:attack_Tyrande(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local arrow = cc.Sprite:create("image/battle/skill/Tyrande/mofadan.png")

    arrow:setPosition(actor:getFirePosition())
    arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
    arrow:setVisible(false)
    arrow:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(arrow)

    local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(arrow)
    end)
    arrow:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_DiXueLingZhu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_Kelthuzad(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/Jaina/normalSkillAttack_jaina.png")
    sprite:setScaleX(actor:getScaleX())
    sprite:setScaleY(actor:getScaleY())
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
    
end

function FightLayer:attack_YiLiDan(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_ShuiYuanSu(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/ShuiYuanSu/normalSkillAttack_shuiyuansu.png")
    sprite:setScaleX(actor:getScaleX())
    sprite:setScaleY(actor:getScaleY())
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
    
end

function FightLayer:attack_ShahelasiZhuMu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_LaGeNaLuoSi(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/LiNa/xiaohuoqiu.png")
    sprite:setScaleX(actor:getScaleX())
    sprite:setScaleY(actor:getScaleY())
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)

    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_AoNiKeXiYa(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/AoNiKeXiYa/normalSkillAttack_aonikexiya.png")
    sprite:setScaleX(actor:getScaleX())
    sprite:setScaleY(actor:getScaleY())
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))

    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_LaSuoLiAn(param)

end

function FightLayer:attack_KeLuoMaGuSi(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    if targetActor == nil then return end
    local dt = param.dt
    local sprite = cc.Sprite:create("battle/skill/Jaina/normalSkillAttack_jaina.png")
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)
    
    local sprite2 = cc.Sprite:create("battle/skill/LiNa/xiaohuoqiu.png")
    sprite2:setPosition(cc.p(actor:getFirePosition().x,actor:getFirePosition().y-20))
    sprite2:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite2:setVisible(false)
    sprite2:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite2)
    
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local moveTo1 = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local moveTo2 = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, moveTo1, attackOver))
    sprite2:runAction(cc.Sequence:create(cc.DelayTime:create(0.125),cc.Show:create(),moveTo2,cc.RemoveSelf:create()))
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:attack_MaNuoLuoSi(param)
    
end

function FightLayer:attack_AErSaSi(param)

end

function FightLayer:attack_AKeMengDe(param)

end

function FightLayer:attack_WaSiQi(param)

end

function FightLayer:attack_XianXueNvWangLan(param)

end

function FightLayer:attack_KaZhaKe(param)

end

function FightLayer:attack_WoJin(param)

end

function FightLayer:attack_LuKui(param)

end

function FightLayer:attack_tonglingta(param)
    
end

function FightLayer:attack_Cannon(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local endAngle = 0
    local startAngle = 0
    if  actor.npcBook.direction == 1 then 
        startAngle = -30
        endAngle = 30
    else
        startAngle = -150
        endAngle = -260
    end
    local sprite = cc.Sprite:create("battle/skill/Cannon/normalSkillAttack_cannon.png")
    sprite:setScale(0.5)
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)
    local spriteHeight = sprite:getContentSize()
    
    sprite:setRotation(startAngle)
    
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local arrowParabola = MyDIYAction.MissleParabola(actor:getFirePosition(),cc.p(targetActor:getHurtPosition().x,targetActor:getPositionY()),startAngle,endAngle,dt,
        spriteHeight.height,100)
    local attackOver = cc.CallFunc:create(function()
        Helper.addSpriteFrames("battle/hurt/paodan_hurt.plist","battle/hurt/paodan_hurt.pvr.ccz")
        local frameArr = Helper.newFrames("paodan%d.png",5,10)
        local animation = Helper.newAnimation(frameArr,0.5/10)
        local effectSprite = cc.Sprite:create()
        effectSprite:setAnchorPoint(cc.p(0.5,0))
        self:addChild(effectSprite)
        effectSprite:setLocalZOrder(targetActor:getLocalZOrder())
        effectSprite:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()-targetActor.armature:getContentSize().width/2))
        effectSprite:runAction(cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create()))
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, arrowParabola, attackOver))
    --[[
    local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)
    ]]
end

------------small skill handlers function define------------
function FightLayer:smallSkill_Captain(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_DuanGuZhe(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_FanKeLiFu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_FengXing(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local arrow = cc.Sprite:create("image/battle/skill/FengXing/jian.png")
        arrow:setScaleX(actor:getScaleX())
        arrow:setScaleY(actor:getScaleY())
        arrow:setPosition(actor:getFirePosition())
        arrow:setRotation(self:calShootAngleHelper(actor,targetActor))
        arrow:setVisible(false)
        arrow:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(arrow)

        --local delay = cc.DelayTime:create(0.1*i)
        local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(arrow)
        end)
        arrow:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    end

   
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
    
end

function FightLayer:smallSkill_HuaiTe(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setScaleX(actor:getScaleX())
        sprite:setScaleY(actor:getScaleY())
        sprite:setPosition(actor:getFirePosition())
        sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite)
        --[[
        local sprite2 = cc.Sprite:create()
        sprite2:setScaleX(actor:getScaleX())
        sprite2:setScaleY(actor:getScaleY())
        sprite2:setPosition(0,0)
        --actor.armature:getContentSize().height*targetActor.npcBook.scaleFactor*0.5
        sprite2:setRotation(self:calShootAngleHelper(actor,targetActor))
        --sprite2:setVisible(false)
        sprite2:setLocalZOrder(targetActor:getLocalZOrder())
        sprite:addChild(sprite2,1)
        ]]

        Helper.addSpriteFrames("battle/skill/huaite/smallSkillAttack_huaite_daoguang.plist","battle/skill/huaite/smallSkillAttack_huaite_daoguang.pvr.ccz")
        local frameArr = Helper.newFrames("huaidaoguang%04d.png",1,5)
        local animation = Helper.newAnimation(frameArr,0.2/5)
        --[[
        Helper.addSpriteFrames("battle/skill/huaite/smallSkillAttack_huaite_dimian.plist","battle/skill/huaite/smallSkillAttack_huaite_dimian.pvr.ccz")
        local frameArr2 = Helper.newFrames("HUOHUA%d.png",0,7)
        local animation2 = Helper.newAnimation(frameArr2,0.2/8)
        ]]

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        --local attackStart2 = cc.CallFunc:create(function() sprite2:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        --local spwan2 = cc.Spawn:create(moveTo,cc.Animate:create(animation2))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        local attackOver2 = cc.CallFunc:create(function()
            --self:removeChild(sprite2)
        end)
        --sprite2:runAction(cc.Animate:create(animation2))
        sprite:runAction(cc.Sequence:create(attackStart, spwan, attackOver))
    end

    
    --[[local music = actor.npcBook.normalMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
    
end

function FightLayer:smallSkill_KuLouWang(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_LaZuoKeLao(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_Lina(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setScaleX(actor:getScaleX())
        sprite:setScaleY(actor:getScaleY())
        sprite:setPosition(actor:getFirePosition())
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/LiNa/dahuoqiu.plist","battle/skill/LiNa/dahuoqiu.pvr.ccz")
        local frameArr = Helper.newFrames("huoqiu%d.png",1,16)
        local animation = Helper.newAnimation(frameArr,0.2/16)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, spwan, attackOver))
    end

    
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
   
end

function FightLayer:smallSkill_XiaoHei(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create("battle/skill/XiaoHei/zidan.png")
        sprite:setScaleX(actor:getScaleX())
        sprite:setScaleY(actor:getScaleY())
        sprite:setPosition(actor:getFirePosition())
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)
        --[[
        Helper.addSpriteFrames("battle/skill/XiaoHei/zidan.plist","battle/skill/XiaoHei/zidan.pvr.ccz")
        local frameArr = Helper.newFrames("zidan%d.png",1,8)
        local animation = Helper.newAnimation(frameArr,0.2/8)
        ]]
        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        --local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    end

    
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
    
end

function FightLayer:smallSkill_ZhuRiZhe(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setScaleX(actor:getScaleX())
        sprite:setScaleY(actor:getScaleY())
        sprite:setPosition(actor:getFirePosition())
        sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/LiNa/dahuoqiu.plist","battle/skill/LiNa/dahuoqiu.pvr.ccz")
        local frameArr = Helper.newFrames("huoqiu%d.png",1,16)
        local animation = Helper.newAnimation(frameArr,0.2/16)

        local attackStart = cc.CallFunc:create(function()  sprite:setVisible(true) end)
        
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, spwan, attackOver))
        

    end

    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_Jaina(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    local rotateAngle = 0
    local arrowAngle = 0
    if  actor.npcBook.direction == 1 then 
        arrowAngle = -60
        rotateAngle = 120
    else
        arrowAngle = -120
        rotateAngle = -120
    end
    local direction = actor:getDirection()
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        --print("dt:"..dt)
        if targetActor == nil then return end
        local arrow = cc.Sprite:create("battle/skill/Jaina/smalllSkillAttack_jaina.png")  
        --arrow:setScaleX(actor:getScaleX())
        local firePosition = cc.p(actor:getFirePosition().x,actor:getFirePosition().y+20)
        arrow:setScaleY(actor:getScaleY())
        --arrow:setPosition(actor:getFirePosition())
        arrow:setPosition(firePosition)
        arrow:setVisible(false)
        --arrow:setAnchorPoint(cc.p(1,0))
        arrow:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(arrow)             
        arrow:setLocalZOrder(targetActor:getLocalZOrder())
        local spriteHeight = {width =193,height =79}
        local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())

        arrow:setRotation(arrowAngle)
        local arrowParabola = MyDIYAction.arrowShotDown(firePosition,targetActor:getHurtPosition(),dt,
            direction)

        local attackOver = cc.CallFunc:create(function()
            self:removeChild(arrow)
        end)
        arrow:runAction(cc.Sequence:create(attackStart, arrowParabola,attackOver))
    end

    
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
   
end

function FightLayer:smallSkill_Tyrande(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end

        local sprite1 = cc.Sprite:create()
        sprite1:setAnchorPoint(cc.p(0.5,0))
        sprite1:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite1:setVisible(false)
        sprite1:setLocalZOrder(targetActor:getLocalZOrder()-1)
        self:addChild(sprite1)
        sprite1:setColor(cc.c3b(255,252,112))

        local sprite2 = cc.Sprite:create()
        sprite2:setAnchorPoint(cc.p(0.5,0))
        sprite2:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite2:setVisible(false)
        sprite2:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite2)
        sprite2:setColor(cc.c3b(255,252,112))
        
        local sprite3 = cc.Sprite:create()
        sprite3:setAnchorPoint(cc.p(0.5,0))
        sprite3:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite3:setVisible(false)
        sprite3:setLocalZOrder(targetActor:getLocalZOrder()+1)
        self:addChild(sprite3)
        
        local sprite4 = cc.Sprite:create()
        sprite4:setAnchorPoint(cc.p(0.5,0))
        sprite4:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite4:setVisible(false)
        sprite4:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite4)
        --sprite4:setColor(cc.c3b(255,252,112))
        
        Helper.addSpriteFrames("battle/skill/Tyrande/smallSkillAttack_Tyrande_baihuguang.plist","battle/skill/Tyrande/smallSkillAttack_Tyrande_baihuguang.pvr.ccz")
        local frameArr1 = Helper.newFrames("baihuguang_%d_Tyrande.png",1,6)
        local animation1 = Helper.newAnimation(frameArr1,dt/6)
        
        Helper.addSpriteFrames("battle/skill/Tyrande/smallSkillAttack_Tyrande_buxue.plist","battle/skill/Tyrande/smallSkillAttack_Tyrande_buxue.pvr.ccz")
        local frameArr2 = Helper.newFrames("buxue_%d_Tyrande.png",0,4)
        local animation2 = Helper.newAnimation(frameArr2,dt/4)
        
        Helper.addSpriteFrames("battle/skill/Tyrande/smallSkillAttack_Tyrande_oye.plist","battle/skill/Tyrande/smallSkillAttack_Tyrande_oye.pvr.ccz")
        local frameArr3 = Helper.newFrames("oye_0_%d_Tyrande.png",0,4)
        local animation3 = Helper.newAnimation(frameArr3,dt/4)
        
        Helper.addSpriteFrames("battle/skill/Tyrande/smallSkillAttack_Tyrande_buxuelizi.plist","battle/skill/Tyrande/smallSkillAttack_Tyrande_buxuelizi.pvr.ccz")
        local frameArr4 = Helper.newFrames("buxuelizi_%d_Tyrande.png",0,4)
        local animation4 = Helper.newAnimation(frameArr4,dt/4)

        sprite1:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation1),cc.FadeOut:create(dt), cc.RemoveSelf:create()))
        sprite2:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation2),cc.FadeOut:create(dt), cc.RemoveSelf:create()))
        sprite3:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation3),cc.FadeOut:create(dt), cc.RemoveSelf:create()))
        sprite4:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation4),cc.FadeOut:create(dt), cc.RemoveSelf:create()))


        
    end

    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_DiXueLingZhu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_Kelthuzad(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setScaleX(actor:getScaleX())
        sprite:setScaleY(actor:getScaleY())
        sprite:setPosition(actor:getFirePosition())
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/Kelthuzad/smallSkillAttack_Kelthuzad.plist","battle/skill/Kelthuzad/smallSkillAttack_Kelthuzad.pvr.ccz")
        local frameArr = Helper.newFrames("binghua_%d.png",0,8)
        local animation = Helper.newAnimation(frameArr,dt/8)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, spwan, attackOver))
    end

    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_YiLiDan(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_ShahelasiZhuMu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_LaGeNaLuoSi(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = 0.33
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setPosition(actor:getFirePosition())
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite)
        
        local sprite2 = cc.Sprite:create()
        sprite2:setPosition(actor:getFirePosition())
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite2:setVisible(false)
        sprite2:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite2)
        
        local sprite3 = cc.Sprite:create()
        sprite3:setPosition(actor:getFirePosition())
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite3:setVisible(false)
        sprite3:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite3)
       
        
        Helper.addSpriteFrames("battle/skill/LaGeNaLuoSi/smallSkillAttack_LaGeNaLuoSi.plist","battle/skill/LaGeNaLuoSi/smallSkillAttack_LaGeNaLuoSi.pvr.ccz")
        local frameArr = Helper.newFrames("feilun_%d.png",0,6)
        local animation = Helper.newAnimation(frameArr,dt/6)
        
        
        local attackStart = cc.CallFunc:create(function()  sprite:setVisible(true) end)
        
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, spwan, attackOver))
        sprite2:runAction(cc.Sequence:create(cc.DelayTime:create(dt),cc.Show:create(), spwan, cc.RemoveSelf:create()))
        sprite3:runAction(cc.Sequence:create(cc.DelayTime:create(dt*2),cc.Show:create(), spwan, cc.RemoveSelf:create()))
        

    end
    --[[local music = actor.npcBook.smallMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_AoNiKeXiYa(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()-targetActor.armature:getContentSize().height*targetActor.npcBook.scaleFactor*0.5-10))
        sprite:setVisible(false)
        sprite:setAnchorPoint(cc.p(0.5,0))
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/AoNiKeXiYa/smallSkillAttack_aonikexiya.plist","battle/skill/AoNiKeXiYa/smallSkillAttack_aonikexiya.pvr.ccz")
        local frameArr = Helper.newFrames("baoyan_%d.png",3,6)
        local animation = Helper.newAnimation(frameArr,dt/6)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart,cc.Animate:create(animation), attackOver))
    end
    
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)
    ]]
end

function FightLayer:smallSkill_LaSuoLiAn(param)

end

function FightLayer:smallSkill_KeLuoMaGuSi(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    local dt = targetActors[1].dt
    local rangeCol = 2
    local skillPos = self:getAttackPosByRange(actor.row+actor.npcBook.direction,actor.col,5,2,actor.npcBook.direction)
    for i=1,#skillPos do
        local singlePos = skillPos[i]
        local delayTime = i/rangeCol*0.1
        local sprite1 = cc.Sprite:create()
        sprite1:setAnchorPoint(cc.p(0.5,0))
        sprite1:setPosition(singlePos)
        sprite1:setVisible(false)
        sprite1:setLocalZOrder(actor:getLocalZOrder())
        self:addChild(sprite1)

        Helper.addSpriteFrames("battle/skill/KeLuoMaGuSi/smallSkillAttack_KeLuoMaGuSi.plist","battle/skill/KeLuoMaGuSi/smallSkillAttack_KeLuoMaGuSi.pvr.ccz")
        local frameArr1 = Helper.newFrames("huoyanbage_%d.png",0,12)
        local animation1 = Helper.newAnimation(frameArr1,dt/12)     

        sprite1:runAction(cc.Sequence:create(cc.DelayTime:create(delayTime),cc.Show:create(),cc.Animate:create(animation1),cc.RemoveSelf:create()))
    end
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:smallSkill_MaNuoLuoSi(param)

end

function FightLayer:smallSkill_AErSaSi(param)

end

function FightLayer:smallSkill_AKeMengDe(param)

end

function FightLayer:smallSkill_WaSiQi(param)

end

function FightLayer:smallSkill_XianXueNvWangLan(param)

end

function FightLayer:smallSkill_KaZhaKe(param)

end

function FightLayer:smallSkill_WoJin(param)

end

function FightLayer:smallSkill_LuKui(param)

end

function FightLayer:smallSkill_tonglingta(param)
end

function FightLayer:smallSkill_Cannon(param)
end

------------big skill handlers function define------------
function FightLayer:bigSkill_Captain(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    if #targetActors == 0 then return end
    local targetActor = targetActors[1].npc
    if targetActor == nil then return end
    local dt =targetActors[1].dt
    local ship = cc.Sprite:create()
    ship:setAnchorPoint(cc.p(1,0.5))
    ship:setScaleX(actor:getScaleX())
    ship:setScaleY(actor:getScaleY())
    local moveToPos = nil
    local pos1 = nil
    if  actor.npcBook.direction == 1 then
        ship:setPosition(WIN_SIZE.width*0,WIN_SIZE.height*0.6)
        moveToPos = cc.p(WIN_SIZE.width*0.75,WIN_SIZE.height*0.6)
        local maxX = WIN_SIZE.width*0.75
        for i=1,#targetActors do
            local targetActor = targetActors[i].npc
            if  targetActor:getHurtPosition().x > maxX then
                maxX = targetActor:getHurtPosition().x
            end
        end
        moveToPos.x = maxX
    else
        ship:setPosition(WIN_SIZE.width*1.0,WIN_SIZE.height*0.6)
        moveToPos = cc.p(WIN_SIZE.width*0.25,WIN_SIZE.height*0.6)
        local minX = WIN_SIZE.width*0.25
        for i=1,#targetActors do
            local targetActor = targetActors[i].npc
            if  targetActor:getHurtPosition().x < minX then
                minX = targetActor:getHurtPosition().x
            end
        end
        moveToPos.x = minX
    end 
    ship:setVisible(false)
    self:addChild(ship,10000)

    Helper.addSpriteFrames("battle/skill/captain/bigSkillAttack_captain.plist","battle/skill/captain/bigSkillAttack_captain.pvr.ccz")
    local frameArr = Helper.newFrames("chuan%d.png",1,4)
    local animation = Helper.newAnimation(frameArr,0.2/4)
    animation:setLoops(dt/0.2)
    local attackStart = cc.CallFunc:create(function()
        ship:setVisible(true)
    end)
    local fadeout = cc.FadeOut:create(0.5)
    local moveTo = cc.MoveTo:create(dt,moveToPos)
    local easeSineOut = cc.EaseSineInOut:create(moveTo)
    local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
    local attackOver = cc.CallFunc:create(function()
        ship:stopAllActions()
        self:removeChild(ship)
    end)
    ship:runAction(cc.Sequence:create(attackStart,spwan,fadeout,attackOver))

    
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
    
end

function FightLayer:bigSkill_DuanGuZhe(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_FanKeLiFu(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()

        sprite:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()+targetActor.armature:getContentSize().height*targetActor.npcBook.scaleFactor*0.5))
        sprite:setVisible(false)
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/FanKeLiFu/bigSkillAttack_fankelifu.plist","battle/skill/FanKeLiFu/bigSkillAttack_fankelifu.pvr.ccz")
        local frameArr = Helper.newFrames("dahzao%d.png",1,13)
        local animation = Helper.newAnimation(frameArr,dt/13)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart,cc.Animate:create(animation), attackOver))
    end

    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_FengXing(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    --table.sort(targetActors,function(a,b) return a.dt < b.dt end)
    --[[
    Helper.addSpriteFrames("battle/skill/FengXing/bigSkillAttack_fengxing.plist","battle/skill/FengXing/bigSkillAttack_fengxing.pvr.ccz")
    local frameArr = Helper.newFrames("c000%d.png",1,8)
    local animation = Helper.newAnimation(frameArr,0.2/8)
    ]]
    local arrow = cc.Sprite:create("battle/skill/FengXing/bigSkillAttack_fengxing.png")
    self:addChild(arrow)
    arrow:setLocalZOrder(actor:getLocalZOrder())
    arrow:setVisible(false)
    arrow:setScaleX(actor.npcBook.direction)
    arrow:setPosition(actor:getFirePosition())
    local moveTotab = {}
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        local arrowDt = nil
        if  i >1 then
            arrowDt = targetActors[i].dt - targetActors[i-1].dt
        else
            arrowDt = targetActors[i].dt
        end
        local arrowPos = cc.p(targetActor:getHurtPosition().x,actor:getFirePosition().y)
        local moveTo = cc.MoveTo:create(arrowDt,arrowPos)
        table.insert(moveTotab,moveTo)
    end
    local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
    local sequence = cc.Sequence:create(moveTotab)
    --local spwan = cc.Spawn:create(sequence,cc.Animate:create(animation))
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(arrow)
    end)
    arrow:runAction(cc.Sequence:create(attackStart, sequence, attackOver))
    
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_HuaiTe(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    
    local dt = 0
    local index = 1
    for i=1,#targetActors do
        local time = targetActors[i].dt
        if  time > dt then
            dt = time
            index = i
        end
    end
        local targetActor = targetActors[index].npc
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setScaleX(actor:getScaleX())
        sprite:setScaleY(actor:getScaleY())
        sprite:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()-targetActor.armature:getContentSize().height*targetActor.npcBook.scaleFactor*0.5))
        sprite:setVisible(false)
        sprite:setAnchorPoint(cc.p(0.5,0))
        sprite:setLocalZOrder(targetActor:getLocalZOrder()+300)
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/huaite/bigSkillAttack_huaite.plist","battle/skill/huaite/bigSkillAttack_huaite.pvr.ccz")
        local frameArr = Helper.newFrames("shandian%04d.png",1,25)
        local animation = Helper.newAnimation(frameArr,0.5/25)


        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local anmi = cc.Animate:create(animation)
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
     
        end)
        sprite:runAction(cc.Sequence:create(attackStart, anmi, attackOver))



    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_KuLouWang(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        local animationHeight = 234
        sprite:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()+animationHeight*0.6))
        sprite:setVisible(false)
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/KuLouWang/dazhao.plist","battle/skill/KuLouWang/dazhao.pvr.ccz")
        local frameArr = Helper.newFrames("kulbao%d.png",9,12)
        local animation = Helper.newAnimation(frameArr,dt/12)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart,cc.Animate:create(animation), attackOver))
    end

    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_LaZuoKeLao(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_Lina(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setAnchorPoint(cc.p(0,0.5))
        sprite:setScaleX(actor:getScaleX())
        sprite:setScaleY(actor:getScaleY())
        sprite:setPosition(actor:getFirePosition())
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)
        local spriteWidth = 745*actor:getScaleX()
        local moveToPos = targetActor:getHurtPosition()
        local abs = math.abs(targetActor:getHurtPosition().x - actor:getFirePosition().x)
        if  abs<300 then
            moveToPos.x = actor:getFirePosition().x + 300*actor.npcBook.direction
        end
        Helper.addSpriteFrames("battle/skill/LiNa/fenghuang.plist","battle/skill/LiNa/fenghuang.pvr.ccz")
        local frameArr = Helper.newFrames("fenghuang%d.png",13,6)
        local animation = Helper.newAnimation(frameArr,dt*0.8/6)
        local frameArr2 = Helper.newFrames("fenghuang%d.png",19,6)
        local animation2 = Helper.newAnimation(frameArr2,dt*0.2/6)
        local animate2 = cc.Animate:create(animation2)
        local spwan2 = cc.Spawn:create(cc.FadeOut:create(dt*0.2),animate2)
        local fadeout = cc.FadeOut:create(0.5)
        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt*0.8,cc.p(moveToPos.x-spriteWidth/2,actor:getFirePosition().y))
        local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, spwan,spwan2,attackOver))
    end

    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_XiaoHei(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    local rotateAngle = 0
    local arrowAngle = 0
    if  actor.npcBook.direction == 1 then 
        arrowAngle = -60
        rotateAngle = 120
    else
        arrowAngle = -120
        rotateAngle = -120
    end
    local direction = actor:getDirection()
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        
        if targetActor == nil then return end
        local arrow = cc.Sprite:create("battle/skill/XiaoHei/zidan.png")  
        --arrow:setScaleX(actor:getScaleX())
        local firePosition = cc.p(actor:getFirePosition().x,actor:getFirePosition().y+20)
        arrow:setScaleY(actor:getScaleY())
        --arrow:setPosition(actor:getFirePosition())
        arrow:setPosition(firePosition)
        arrow:setVisible(false)
        arrow:setAnchorPoint(cc.p(1,0))
        arrow:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(arrow)             
        arrow:setLocalZOrder(targetActor:getLocalZOrder())
        local spriteHeight = {width =193,height =79}
        
        --[[
        Helper.addSpriteFrames("battle/skill/XiaoHei/zidan.plist","battle/skill/XiaoHei/zidan.pvr.ccz")
        local frameArr = Helper.newFrames("zidan%d.png",1,8)
        local animation = Helper.newAnimation(frameArr,0.2/8)
        animation:setLoops(dt/0.2)
        ]]
        
        local attackStart = cc.CallFunc:create(function() arrow:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())

        arrow:setRotation(arrowAngle)
        local arrowParabola = MyDIYAction.arrowShotDown(firePosition,targetActor:getHurtPosition(),dt,
            direction)
        --local arrowParabola = MyDIYAction.arrowParabola(firePosition,targetActor:getHurtPosition(),rotateAngle,dt,
          --  spriteHeight.height,direction,WIN_SIZE.height*1.2)
        --[[
        Helper.addSpriteFrames("battle/skill/XiaoHei/bingbao.plist","battle/skill/XiaoHei/bingbao.pvr.ccz")
        local frameArr2 = Helper.newFrames("bingbao%d.png",7,7)
        local animation2 = Helper.newAnimation(frameArr2,0.2/7)
        local hurtSprit = cc.Sprite:create()
        targetActor:addChild(hurtSprit)
        hurtSprit:setVisible(false)
        local hurtAction = cc.Animate:create("animation2")
        ]]
        
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(arrow)
            --[[
            hurtSprit:setVisible(true)
            hurtSprit:setPosition(cc.p(targetActor:getHurtPosition().x,0))
            hurtSprit:runAction(cc.Sequence:create(hurtAction))
            ]]
        end)
        arrow:runAction(cc.Sequence:create(attackStart, arrowParabola,attackOver))
    end

    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_ZhuRiZhe(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        
        local sprite = cc.Sprite:create()
        sprite:setAnchorPoint(cc.p(0.5,0))
        sprite:setScaleX(actor:getScaleX())
        sprite:setScaleY(actor:getScaleY())
        sprite:setPosition(cc.p(actor:getPositionX(),actor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite, 10000)

        Helper.addSpriteFrames("battle/skill/zhurizhe/bigSkillAttack_zhurizhe.plist","battle/skill/zhurizhe/bigSkillAttack_zhurizhe.pvr.ccz")
        local frameArr = Helper.newFrames("xunhuangdunp%d.png",1,8)
        local animation = Helper.newAnimation(frameArr,0.2/8)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart,cc.Animate:create(animation),cc.FadeOut:create(dt), attackOver))

    end

    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_Jaina(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_Tyrande(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    local dt = actor.npcBook.persistentSkillTime
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        if  dt == nil then dt =0.625 end
        if targetActor == nil then return end

        local sprite1 = cc.Sprite:create()
        sprite1:setAnchorPoint(cc.p(0.5,0))
        sprite1:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite1:setVisible(false)
        sprite1:setLocalZOrder(targetActor:getLocalZOrder()-1)
        self:addChild(sprite1)

        local sprite2 = cc.Sprite:create()
        sprite2:setAnchorPoint(cc.p(0.5,0))
        sprite2:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite2:setVisible(false)
        sprite2:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite2)
        
        local sprite3 = cc.Sprite:create()
        sprite3:setAnchorPoint(cc.p(0.5,0))
        sprite3:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite3:setVisible(false)
        sprite3:setLocalZOrder(targetActor:getLocalZOrder()+1)
        self:addChild(sprite3)
        
        local sprite4 = cc.Sprite:create()
        sprite4:setAnchorPoint(cc.p(0.5,0))
        sprite4:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite4:setVisible(false)
        sprite4:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite4)
        --sprite4:setColor(cc.c3b(255,252,112))

        Helper.addSpriteFrames("battle/skill/Tyrande/smallSkillAttack_Tyrande_baihuguang.plist","battle/skill/Tyrande/smallSkillAttack_Tyrande_baihuguang.pvr.ccz")
        local frameArr1 = Helper.newFrames("baihuguang_%d_Tyrande.png",1,6)
        local animation1 = Helper.newAnimation(frameArr1,dt/12)
        animation1:setLoops(8)

        Helper.addSpriteFrames("battle/skill/Tyrande/smallSkillAttack_Tyrande_buxue.plist","battle/skill/Tyrande/smallSkillAttack_Tyrande_buxue.pvr.ccz")
        local frameArr2 = Helper.newFrames("buxue_%d_Tyrande.png",0,4)
        local animation2 = Helper.newAnimation(frameArr2,dt/8)
        animation2:setLoops(8)

        Helper.addSpriteFrames("battle/skill/Tyrande/smallSkillAttack_Tyrande_oye.plist","battle/skill/Tyrande/smallSkillAttack_Tyrande_oye.pvr.ccz")
        local frameArr3 = Helper.newFrames("oye_0_%d_Tyrande.png",0,4)
        local animation3 = Helper.newAnimation(frameArr3,dt/8)
        animation3:setLoops(8)

        Helper.addSpriteFrames("battle/skill/Tyrande/smallSkillAttack_Tyrande_buxuelizi.plist","battle/skill/Tyrande/smallSkillAttack_Tyrande_buxuelizi.pvr.ccz")
        local frameArr4 = Helper.newFrames("buxuelizi_%d_Tyrande.png",0,4)
        local animation4 = Helper.newAnimation(frameArr4,dt/4/2)
        animation4:setLoops(4*2)

        sprite1:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation1),cc.FadeOut:create(dt), cc.RemoveSelf:create()))
        sprite2:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation2),cc.FadeOut:create(dt), cc.RemoveSelf:create()))
        sprite3:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation3),cc.FadeOut:create(dt), cc.RemoveSelf:create()))
        sprite4:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation4),cc.FadeOut:create(dt), cc.RemoveSelf:create()))
    end
    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_DiXueLingZhu(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    local dt = 0
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        dt = targetActors[i].dt
        if targetActor ~= nil then 
            local actorMove = cc.MoveBy:create(dt/2,cc.p(0,100))
            targetActor:runAction(cc.Sequence:create(actorMove,actorMove:reverse()))
        end
    end
    local skillPos = self:getAttackPosByRange(actor.row,actor.col,5,2,actor.npcBook.direction)
    for i=1,#skillPos do
        local singlePos = skillPos[i]
        local sprite1 = cc.Sprite:create()
        sprite1:setAnchorPoint(cc.p(0.5,0))
        sprite1:setPosition(singlePos)
        sprite1:setVisible(false)
        sprite1:setLocalZOrder(10000)
        self:addChild(sprite1)

        local sprite2 = cc.Sprite:create()
        sprite2:setAnchorPoint(cc.p(0.5,0))
        sprite2:setPosition(singlePos)
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite2:setVisible(false)
        sprite2:setLocalZOrder(10000)
        self:addChild(sprite2)

        local sprite3 = cc.Sprite:create()
        sprite3:setAnchorPoint(cc.p(0.5,0))
        sprite3:setPosition(singlePos)
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite3:setVisible(false)
        sprite3:setLocalZOrder(10000)
        self:addChild(sprite3)
        
        Helper.addSpriteFrames("battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Di.plist","battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Di.pvr.ccz")
        local frameArr1 = Helper.newFrames("dimian_%d.png",0,8)
        local animation1 = Helper.newAnimation(frameArr1,dt/8)
        
        Helper.addSpriteFrames("battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Quan.plist","battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Quan.pvr.ccz")
        local frameArr2 = Helper.newFrames("diliequan_%d.png",0,6)
        local animation2 = Helper.newAnimation(frameArr2,dt/6)
        
        Helper.addSpriteFrames("battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Ci.plist","battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Ci.pvr.ccz")
        local frameArr3 = Helper.newFrames("dicidici_%d.png",0,5)
        local animation3 = Helper.newAnimation(frameArr3,dt/5)

        sprite1:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation1),cc.RemoveSelf:create()))
        sprite2:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation2),cc.RemoveSelf:create()))
        sprite3:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation3),cc.RemoveSelf:create()))
    end
    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_Kelthuzad(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setScaleX(actor:getScaleX())
        sprite:setScaleY(actor:getScaleY())
        sprite:setPosition(actor:getFirePosition())
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/Kelthuzad/bigSkillAttack_Kelthuzad.plist","battle/skill/Kelthuzad/bigSkillAttack_Kelthuzad.pvr.ccz")
        local frameArr = Helper.newFrames("dazhao_%d.png",0,4)
        local animation = Helper.newAnimation(frameArr,dt/4)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, spwan, attackOver))
    end

    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_YiLiDan(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_ShahelasiZhuMu(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()-20))
        sprite:setVisible(false)
        sprite:setAnchorPoint(cc.p(0.5,0))
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/ShahelasiZhuMu/bigSkillAttack_ShahelasiZhuMu.plist","battle/skill/ShahelasiZhuMu/bigSkillAttack_ShahelasiZhuMu.pvr.ccz")
        local frameArr = Helper.newFrames("jiguang%d.png",1,6)
        local animation = Helper.newAnimation(frameArr,dt/6)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart,cc.Animate:create(animation), attackOver))
    end

    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_LaGeNaLuoSi(param)
        local actor = param.fromNpc
        local targetActors = param.toNpcs

        for i=1,#targetActors do
            local targetActor = targetActors[i].npc
            local dt = targetActors[i].dt
            if targetActor == nil then return end
            local sprite = cc.Sprite:create()
            sprite:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()-targetActor.armature:getContentSize().height*targetActor.npcBook.scaleFactor*0.5-10))
            sprite:setVisible(false)
            sprite:setAnchorPoint(cc.p(0.5,0))
            sprite:setLocalZOrder(targetActor:getLocalZOrder())
            self:addChild(sprite)

            Helper.addSpriteFrames("battle/skill/LaGeNaLuoSi/bigSkillAttack_LaGeNaLuoSi.plist","battle/skill/LaGeNaLuoSi/bigSkillAttack_LaGeNaLuoSi.pvr.ccz")
            local frameArr = Helper.newFrames("dashou_%d.png",0,16)
            local animation = Helper.newAnimation(frameArr,dt/16)

            local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
            local attackOver = cc.CallFunc:create(function()
                self:removeChild(sprite)
            end)
            sprite:runAction(cc.Sequence:create(attackStart,cc.Animate:create(animation), attackOver))
        end

    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_AoNiKeXiYa(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()-targetActor.armature:getContentSize().height*targetActor.npcBook.scaleFactor*0.5-10))
        sprite:setVisible(false)
        sprite:setAnchorPoint(cc.p(0.5,0))
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(sprite)

        local sprite2 = cc.Sprite:create()       
        sprite2:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()-targetActor.armature:getContentSize().height*targetActor.npcBook.scaleFactor*0.5-10))
        sprite2:setVisible(false)
        sprite2:setAnchorPoint(cc.p(0.5,0))
        sprite2:setLocalZOrder(targetActor:getLocalZOrder()-1)
        self:addChild(sprite2)
        
        Helper.addSpriteFrames("battle/skill/AoNiKeXiYa/bigSkillAttack_aonikexiya_long.plist","battle/skill/AoNiKeXiYa/bigSkillAttack_aonikexiya_long.pvr.ccz")
        local frameArr = Helper.newFrames("long_%d.png",3,12)
        local animation = Helper.newAnimation(frameArr,dt/12)
       
        
        Helper.addSpriteFrames("battle/skill/AoNiKeXiYa/bigSkillAttack_aonikexiya_di.plist","battle/skill/AoNiKeXiYa/bigSkillAttack_aonikexiya_di.pvr.ccz")
        local frameArr2 = Helper.newFrames("mofazhen_%d.png",0,15)
        local animation2 = Helper.newAnimation(frameArr2,dt/15)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart,cc.Animate:create(animation), attackOver))
        sprite2:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation2), cc.RemoveSelf:create()))
    end

    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_LaSuoLiAn(param)

end

function FightLayer:bigSkill_KeLuoMaGuSi(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    local dt = targetActors[1].dt
    local rangeCol = 2
    local skillPos = self:getAttackPosByRange(actor.row+actor.npcBook.direction,actor.col,5,2,actor.npcBook.direction)
    for i=1,#skillPos do
        local singlePos = skillPos[i]
        local delayTime = i/rangeCol*0.1
        local sprite1 = cc.Sprite:create()
        sprite1:setAnchorPoint(cc.p(0.5,0))
        sprite1:setPosition(singlePos)
        sprite1:setVisible(false)
        sprite1:setLocalZOrder(actor:getLocalZOrder())
        self:addChild(sprite1)
        
        local sprite2 = cc.Sprite:create("battle/skill/KeLuoMaGuSi/bigSkillAttack_KeLuoMaGuSi_Di.png")
        sprite1:addChild(sprite2)
        sprite2:setAnchorPoint(cc.p(0,0))
        --sprite2:setPosition(0,0)

        Helper.addSpriteFrames("battle/skill/KeLuoMaGuSi/bigSkillAttack_KeLuoMaGuSi.plist","battle/skill/KeLuoMaGuSi/bigSkillAttack_KeLuoMaGuSi.pvr.ccz")
        local frameArr1 = Helper.newFrames("bingfengwangzuo_%d.png",1,11)
        local animation1 = Helper.newAnimation(frameArr1,dt/11)     

        sprite1:runAction(cc.Sequence:create(cc.DelayTime:create(delayTime),cc.Show:create(),cc.Animate:create(animation1),cc.FadeOut:create(math.max((0.8-delayTime),0)),cc.RemoveSelf:create()))
    end
    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_MaNuoLuoSi(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    local targetRow =  actor.row+actor.npcBook.direction*3
    local targetCol = actor.col
    local targetPos = directorAgent:calPosition(targetRow, targetCol)

    local targetActor = targetActors[1].npc
    if targetActor == nil then return end
    local dt = targetActors[1].dt
    
    local armatureFile = "battle/skill/MaNuoLuoSi/MaNuoLuoSi_HuoYU.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    local armature = ccs.Armature:create("MaNuoLuoSi_HuoYU")
    
    armature:getAnimation():play("attack_2_1")
    
    armature:setPosition(targetPos)
    armature:setLocalZOrder(targetActor:getLocalZOrder()+1)
    self:addChild(armature)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)


    --[[local music = actor.npcBook.bigMusic
    cc.SimpleAudioEngine:getInstance():playEffect(music)]]
end

function FightLayer:bigSkill_AErSaSi(param)

end

function FightLayer:bigSkill_AKeMengDe(param)

end

function FightLayer:bigSkill_WaSiQi(param)

end

function FightLayer:bigSkill_XianXueNvWangLan(param)

end

function FightLayer:bigSkill_KaZhaKe(param)

end

function FightLayer:bigSkill_WoJin(param)

end

function FightLayer:bigSkill_LuKui(param)

end

function FightLayer:bigSkill_tonglingta(param)
end

function FightLayer:bigSkill_Cannon(param)
end

------------color function define------------
function FightLayer:globalSkillBegin(skillId,dt)
    --roll
    local armatureFile = nil
    local armature = nil
    if  skillId == 2 then
        armatureFile = "battle/ui/BingYu/BingYu.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        armature = ccs.Armature:create("BingYu")
    elseif skillId == 3 then
        armatureFile = "battle/ui/HuoYuUp/HuoYuUp.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        armature = ccs.Armature:create("HuoYuUp")
        local armatureFile2 = "battle/ui/HuoYuDown/HuoYuDown.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile2)
        local armature2 = ccs.Armature:create("HuoYuDown")
        armature2:setPosition(WIN_SIZE.width/2,WIN_SIZE.height/2)
        armature2:setScaleX(WIN_SIZE.width*1.1/960)
        armature2:setScaleY(WIN_SIZE.height*1.1/640)
        armature2:getAnimation():play("Animation1")
        self.colorLayer:addChild(armature2)
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile2)
    else
        armatureFile = "battle/ui/leiyun/leiyun.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        armature = ccs.Armature:create("leiyun")
    end
    armature:setPosition(WIN_SIZE.width/2,WIN_SIZE.height/2)
    armature:setScaleX(WIN_SIZE.width*1.1/960)
    armature:setScaleY(WIN_SIZE.height*1.1/640)
    armature:getAnimation():play("Animation1")
    self.colorLayer:addChild(armature)
    
    self.colorLayer:setVisible(true)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
    
    actorAgent:skillBegin()
    
    local bg = require"scene.battle2.actors.BG"

    local size = cc.size(1,1)
    local shake = cc.Shaky3D:create(dt/2,size,3, false)
    local seq = cc.Sequence:create(shake,shake:reverse())
    bg:getNodeGrid():runAction(seq)
    
end

function FightLayer:globalSkillEnd()
    self.colorLayer:setVisible(false)
    self.colorLayer:removeAllChildren()
    actorAgent:skillEnd()
end

function FightLayer:showColorLayer()
    self.colorLayer:setVisible(true)
end

function FightLayer:hideColorLayer()
    self.colorLayer:setVisible(false)
end

------------helper function define------------
function FightLayer:addNpc(npc)
    self:addChild(npc)
end

function FightLayer:getNpc(key)
    return self:getChildByTag(key)
end

function FightLayer:calShootAngleHelper(actor, targetActor)
    local radian = cc.pGetAngle(cc.pSub(targetActor:getHurtPosition(), actor:getFirePosition()), {x=actor:getDirection(),y=0})
    return math.radian2angle(radian)
end

function FightLayer:getAttackPosByRange(fromRow,fromCol,rangeRow,rangeCol,direction)
    local posTab = {}
    for row=fromRow+direction,fromRow+(rangeRow+1)*direction,direction do
        for col=fromCol-math.floor(rangeCol/2),fromCol+math.floor(rangeCol/2) do
            if  row>0 and row <24 and col>0 and col <10 then
                local pos = directorAgent:calPosition(row, col)
                table.insert(posTab,pos)
            end
        end
    end
    return posTab
end

return FightLayer

