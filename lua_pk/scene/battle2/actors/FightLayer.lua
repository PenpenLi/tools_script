require("Cocos2d")
local nodeGrid =nil
local NPC = require "scene.battle2.actors.NPC"
local FightLayer = class("FightLayer", function()
    return cc.Layer:create() 
end)

-----------------------[[External API]]-----------------------------
function FightLayer:create()
    local layer = FightLayer.new()
    layer:initM()
    nodeGrid = cc.NodeGrid:create()
    nodeGrid:addChild(layer)
    return layer
end

function FightLayer:actCMD(cmd)
    local handler =  self.cmdHandlers[cmd.t]
    if handler == nil then
        print("[FightLayer]handler nil")
        printJSON(cmd)
    end
    assert(handler~=nil,"handler nil")
    handler(cmd)
end

function FightLayer:moveWithBG(dx, dy)
    local children = self:getChildren()
    for _, child in pairs(children) do
        if child.npcBook == nil and child ~= self.colorLayer and child ~= self.maskLayer then
            local pos = {x=child:getPositionX(), y=child:getPositionY()}
            pos = {x=pos.x+dx, y=pos.y+dy}
            child:setPosition(pos)
        end
    end
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
    --[[
    local shader = cc.GLProgram:create("shader/InvisibleShader.vsh", "shader/InvisibleShader.fsh")
    --shader:retain()
    shader:bindAttribLocation(cc.ATTRIBUTE_NAME_POSITION, cc.VERTEX_ATTRIB_POSITION)
    shader:bindAttribLocation(cc.ATTRIBUTE_NAME_COLOR, cc.VERTEX_ATTRIB_COLOR)
    shader:bindAttribLocation(cc.ATTRIBUTE_NAME_TEX_COORD, cc.VERTEX_ATTRIB_TEX_COORDS)
    shader:retain()
    shader:link()
    shader:updateUniforms()
    cc.GLProgramCache:getInstance():addGLProgram(shader, "bigskill")
    ]]
    
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
        ["addHp2"] = function (cmd) self:addHp2(cmd) end,
        ["ice_s"] = function (cmd) self:ice_s(cmd) end,
        ["ice_o"] = function (cmd) self:ice_o(cmd) end,
        ["bound_s"] = function (cmd) self:bound_s(cmd) end,
        ["bound_o"] = function (cmd) self:bound_o(cmd) end,
        ["interrupt_s"] = function (cmd) self:interrupt_s(cmd) end,
        ["interrupt_o"] = function (cmd) self:interrupt_o(cmd) end,
        ["xuruo_s"] = function (cmd) self:xuruo_s(cmd) end, --skillid 13
        ["xuruo_o"] = function (cmd) self:xuruo_o(cmd) end,
        ["shengguang_s"] = function (cmd) self:shengguang_s(cmd) end, --skillid 12
        ["shengguang_o"] = function (cmd) self:shengguang_o(cmd) end,
        ["tuteng_s"] = function (cmd) self:tuteng_s(cmd) end, --skilid 9
        ["tuteng_o"] = function (cmd) self:tuteng_o(cmd) end,
        ["jiedu_s"] = function (cmd) self:jiedu_s(cmd) end, --skillid 11
        ["jiedu_o"] = function (cmd) self:jiedu_o(cmd) end,
        ["cibao_s"] = function (cmd) self:cibao_s(cmd) end, --skillid 1
        ["cibao_o"] = function (cmd) self:cibao_o(cmd) end,
        ["hongzha_s"] = function (cmd) self:hongzha_s(cmd) end, --skillid 2
        ["hongzha_o"] = function (cmd) self:hongzha_o(cmd) end,
        ["zhaohuanjianta_s"] = function (cmd) self:zhaohuanjianta_s(cmd) end, --skillid 3
        ["zhaohuanjianta_o"] = function (cmd) self:zhaohuanjianta_o(cmd) end,
        ["zhaohuanmofata_s"] = function (cmd) self:zhaohuanmofata_s(cmd) end, --skillid 4
        ["zhaohuanmofata_o"] = function (cmd) self:zhaohuanmofata_o(cmd) end,
        ["yanwu_s"] = function (cmd) self:yanwu_s(cmd) end, --skillid 5
        ["yanwu_o"] = function (cmd) self:yanwu_o(cmd) end,
        ["zhaohuanfeiting_s"] = function (cmd) self:zhaohuanfeiting_s(cmd) end, --skillid 6
        ["zhaohuanfeiting_o"] = function (cmd) self:zhaohuanfeiting_o(cmd) end,
        ["zhaohuanfazhen_s"] = function (cmd) self:zhaohuanfazhen_s(cmd) end, --skillid 7
        ["zhaohuanfazhen_o"] = function (cmd) self:zhaohuanfazhen_o(cmd) end,
        ["weixingdingwei_s"] = function (cmd) self:weixingdingwei_s(cmd) end, --skillid 8
        ["weixingdingwei_o"] = function (cmd) self:weixingdingwei_o(cmd) end,
        ["silence_s"] = function (cmd) self:silence_s(cmd) end,
        ["silence_o"] = function (cmd) self:silence_o(cmd) end,
        ["sheep_s"] = function (cmd) self:sheep_s(cmd) end,
        ["sheep_o"] = function (cmd) self:sheep_o(cmd) end,
        ["AErSaSi_shield_s"] = function (cmd) self:AErSaSi_shield_s(cmd) end,
        ["AErSaSi_shield_o"] = function (cmd) self:AErSaSi_shield_o(cmd) end,
        ["Fan_s"] = function(cmd) self:Fan_s(cmd) end,
        ["Fan_o"] = function(cmd) self:Fan_o(cmd) end,
        ["suspend_s"] = function(cmd) self:suspend_s(cmd) end,
        ["suspend_o"] = function(cmd) self:suspend_o(cmd) end,
        ["fucked_s"] = function(cmd) self:fucked_s(cmd) end,
        ["fucked_o"] = function(cmd) self:fucked_o(cmd) end,
        ["jianfang_s"] = function(cmd) self:jianfang_s(cmd) end,
    }
    
    self.attackHandlers = {
        ["Captain"] = function (param) self:attack_Captain(param) end,
        ["DuanGuZhe"] = function (param) self:attack_DuanGuZhe(param) end,
        ["HuoGe"] = function (param) self:attack_DuanGuZhe(param) end,
        ["KuangGongXiaoBing"] = function (param) self:attack_DuanGuZhe(param) end,
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
        ["ShahelasiZhuMu"] = function (param) self:attack_ShahelasiZhuMu(param) end,
        ["LaGeNaLuoSi"] = function (param) self:attack_LaGeNaLuoSi(param) end,
        ["AoNiKeXiYa"] = function (param) self:attack_AoNiKeXiYa(param) end,
        ["KeLuoMaGuSi"] = function (param) self:attack_KeLuoMaGuSi(param) end,
        ["MaNuoLuoSi"] = function (param) self:attack_MaNuoLuoSi(param) end,
        ["AKeMengDe"] = function (param) self:attack_AKeMengDe(param) end,
        ["WoJin"] = function (param) self:attack_WoJin(param) end,
        ["XianXueNvWangLan"] = function (param) self:attack_XianXueNvWangLan(param) end,
        ["LaSuoLiAn"] = function (param) self:attack_LaSuoLiAn(param) end,
        ["WaSiQi"] = function (param) self:attack_WaSiQi(param) end,
        ["KaZhaKe"] = function (param) self:attack_KaZhaKe(param) end,
        ["LuKui"] = function (param) self:attack_LuKui(param) end,
        ["AErSaSi"] = function (param) self:attack_AErSaSi(param) end,
        ["ShuiYuanSu"] = function (param) self:attack_ShuiYuanSu(param) end,
        ["FaShi"] = function (param) self:attack_FaShi(param) end,
        ["GongJianShou"] = function (param) self:attack_GongJianShou(param) end,
        ["KeDuoShou"] = function (param) self:attack_KeDuoShou(param) end,
        ["NvYao"] = function (param) self:attack_NvYao(param) end,
        ["XiaoYuRen"] = function (param) self:attack_XiaoYuRen(param) end,
        ["tonglingta"] = function (param) self:attack_tonglingta(param) end,
        ["Cannon"] = function (param) self:attack_Cannon(param) end,
        ["JuDuZhanChe"] = function (param) self:attack_JuDuZhanChe (param) end,
        ["MoFaZhanChe"] = function (param) self:attack_MoFaZhanChe (param) end,
        ["PuTongTanKe"] = function (param) self:attack_PuTongTanKe (param) end,
        ["ZhongZhuangTanKe"] = function (param) self:attack_ZhongZhuangTanKe (param) end
    }
    
    self.smallSkillHandlers = {
        ["Captain"] = function (param) self:smallSkill_Captain(param) end,
        ["DuanGuZhe"] = function (param) self:smallSkill_DuanGuZhe(param) end,
        ["HuoGe"] = function (param) self:smallSkill_DuanGuZhe(param) end,
        ["KuangGongXiaoBing"] = function (param) self:smallSkill_DuanGuZhe(param) end,
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
        ["AKeMengDe"] = function (param) self:smallSkill_AKeMengDe(param) end,
        ["WoJin"] = function (param) self:smallSkill_WoJin(param) end,
        ["XianXueNvWangLan"] = function (param) self:smallSkill_XianXueNvWangLan(param) end,
        ["LaSuoLiAn"] = function (param) self:smallSkill_LaSuoLiAn(param) end,
        ["WaSiQi"] = function (param) self:smallSkill_WaSiQi(param) end,
        ["KaZhaKe"] = function (param) self:smallSkill_KaZhaKe(param) end,
        ["LuKui"] = function (param) self:smallSkill_LuKui(param) end,
        ["AErSaSi"] = function (param) self:smallSkill_AErSaSi(param) end,
        ["FaShi"] = function (param) self:smallSkill_FaShi(param) end,
        ["GongJianShou"] = function (param) self:smallSkill_GongJianShou(param) end,
        ["KeDuoShou"] = function (param) self:smallSkill_KeDuoShou(param) end,
        ["NvYao"] = function (param) self:smallSkill_NvYao(param) end,
        ["tonglingta"] = function (param) self:smallSkill_tonglingta(param) end,
        ["Cannon"] = function (param) self:smallSkill_Cannon(param) end

    }
    
    self.bigSkillHandlers = {
        ["Captain"] = function (param) self:bigSkill_Captain(param) end,
        ["DuanGuZhe"] = function (param) self:bigSkill_DuanGuZhe(param) end,
        ["HuoGe"] = function (param) self:bigSkill_DuanGuZhe(param) end,
        ["KuangGongXiaoBing"] = function (param) self:bigSkill_DuanGuZhe(param) end,
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
        ["AKeMengDe"] = function (param) self:bigSkill_AKeMengDe(param) end,
        ["WoJin"] = function (param) self:bigSkill_WoJin(param) end,
        ["XianXueNvWangLan"] = function (param) self:bigSkill_XianXueNvWangLan(param) end,
        ["LaSuoLiAn"] = function (param) self:bigSkill_LaSuoLiAn(param) end,
        ["WaSiQi"] = function (param) self:bigSkill_WaSiQi(param) end,
        ["KaZhaKe"] = function (param) self:bigSkill_KaZhaKe(param) end,
        ["LuKui"] = function (param) self:bigSkill_LuKui(param) end,
        ["AErSaSi"] = function (param) self:bigSkill_AErSaSi(param) end,
        ["tonglingta"] = function (param) self:bigSkill_tonglingta(param) end,
        ["Cannon"] = function (param) self:bigSkill_Cannon(param) end,
        ["ZhaoHuanFaZhen_juexingSkill"] = function (param) self:bigSkill_ZhaoHuanFaZhen_juexingSkill(param) end,

    }
    
    self.npcs = {}
    self.resetZOrderCount = 1
    --遮罩
    self.colorLayer = cc.LayerColor:create(cc.c4b(0,0,0,220))
    self:addChild(self.colorLayer,5000)
    self.colorLayer:setVisible(false)
    
    cc.Texture2D:setDefaultAlphaPixelFormat(cc.TEXTURE2_D_PIXEL_FORMAT_AUTO)    
end

function FightLayer:resetZOrder()
    table.sort(self.npcs, function(a, b) return math.floor(a:getPositionY()) > math.floor(b:getPositionY())+8 end)
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
    sprite1:setPosition(cc.p(0,-50))
    --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite1:setVisible(false)
    sprite1:setLocalZOrder(npc:getLocalZOrder()-1000)
    npc:addChild(sprite1)

    Helper.addSpriteFrames("battle/skill/NPC/npc_showEffect.plist","battle/skill/NPC/npc_showEffect.pvr.ccz")
    local frameArr1 = Helper.newFrames("chuchang%04d.png",1,14)
    local animation1 = Helper.newAnimation(frameArr1,1.0/14)
    if  cmd.npcBook.npcId == 20011 then
        -- 126 80  100 63
        local spriteScaleX = 126/100
        local spriteScaleY = 80/63
        Helper.addSpriteFrames("battle/skill/Jaina/ShuiYuanSu_Display_fazhen.plist","battle/skill/Jaina/ShuiYuanSu_Display_fazhen.pvr.ccz")
        frameArr1 = Helper.newFrames("effect%04d_Jaina.png",1,7)
        animation1 = Helper.newAnimation(frameArr1,1.0/7)
        animation1:setLoops(3)
        sprite1:setScale(spriteScaleX,spriteScaleY)
        sprite1:setPosition(cc.p(0,-25))
    end

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
    if npc then 
        --npc:resume() 
        npc:actCMD(cmd) 
    end
end

function FightLayer:move(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then 
        --npc:resume() 
        npc:actCMD(cmd) 
    end
    
    if cmd.z then
        for k, v in pairs(cmd.z) do
            local tmpNpc = self:getNpc(v.k)
            if tmpNpc then tmpNpc:setLocalZOrder(v.z) end
        end
    end
    
    -- check zOrder --
    self.resetZOrderCount = self.resetZOrderCount + 1
    if self.resetZOrderCount % 10 == 0 then
        self:resetZOrder()
    end
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
    local fnpc = self:getNpc(cmd.fkey)
    if npc and fnpc then
        if fnpc.npcBook.name == "KeLuoMaGuSi" then
            local function func()
                local hp = cmd.hp
                hp = math.max(math.floor(cmd.hp/2),1)
                npc:actCMD(cmd) 
                npc:normalAttackHurt(hp,cmd.eft)
            end
            self:runAction(cc.Sequence:create(cc.CallFunc:create(func),cc.DelayTime:create(0.1),cc.CallFunc:create(func)))
        else
            npc:actCMD(cmd) npc:normalAttackHurt(cmd.hp,cmd.eft) 
        end     
    end
end

function FightLayer:small_r(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
end

--[[{t="small_s", key=self.key, tKeys=target.key, dt=self:getSmallSkillDt()}]]--
function FightLayer:small_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
    local dt = 0.6
    if  cmd.row then npc.row = cmd.row end
    if  cmd.col then npc.col = cmd.col end
    local toNpcs = {}
    for _, v in pairs(cmd.tSet) do
        local item = {}
        item.npc = self:getNpc(v.key)
        item.dt = v.dt
        dt = v.dt
        table.insert(toNpcs, item)
    end
    npc:playSkillTriggerBuff(dt)
    local attackHandler = self.smallSkillHandlers[npc.npcBook.name]
    attackHandler({fromNpc=npc, toNpcs=toNpcs})
end

function FightLayer:smalled(cmd)
    local fromnpc = self:getNpc(cmd.fKey)
    local npc = self:getNpc(cmd.key)
    --printJSON(cmd)
    print(fromnpc.npcBook.name)
    if npc then
        local att = nil
        if fromnpc.npcBook.name == "Jaina" or fromnpc.npcBook.name == "Kelthuzad" then
            att = "bingbao"
        elseif fromnpc.npcBook.name == "KeLuoMaGuSi" or fromnpc.npcBook.name == "MaNuoLuoSi" then
            att = "chishao"
        elseif fromnpc.npcBook.name == "WaSiQi" then
            att = "yan_wasiqi"     
       end
        npc:actCMD(cmd) npc:normalAttackHurt(cmd.hp,cmd.eft,att)
    end
end

function FightLayer:big_r(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end

    if self.maskLayer==nil then
        self.maskLayer = cc.LayerColor:create(cc.c4b(0, 0, 0, 200), WIN_SIZE.width, WIN_SIZE.height)
        self.maskLayer:setLocalZOrder(10000)
        self:addChild(self.maskLayer)
        self.maskLayer:setVisible(false) 
    end
    Helper.setTimeout(self, 10, function()
        self.maskLayer:setVisible(false) 
        npc:setLocalZOrder(npc.preZOrder)
        --npc:setPosition(npc.prePosition)
    end)

    --big skill mask effect
    npc.prePosition = {x=npc:getPositionX(), y=npc:getPositionY()}
    --npc:setPosition(npc.prePosition.x-npc.npcBook.direction*60, npc.prePosition.y)
    local children = self:getChildren()
    for _, child in pairs(children) do
        if child.npcBook ~= nil and child ~= npc then
            --child:pause()
        end
    end
    self.maskLayer:setVisible(false)
    npc.preZOrder = npc:getLocalZOrder()
    npc:setLocalZOrder(self.maskLayer:getLocalZOrder()+10000)

    --[[local clipNode = cc.ClippingNode:create()
    clipNode:setInverted(true)
    clipNode:addChild(colorLayer)
    clipNode:setStencil(npc.armature)
    self:addChild(clipNode)
    self.clipNode = clipNode]]
    --self.colorLayer:setVisible(true)
    --npc.preZOrder = npc:getZOrder()
    --npc:setZOrder(self.colorLayer:getZOrder()+1)
    --npc.preScale = npc:getScale()
    --npc:setScale(npc:getScale()*1.2)
    --npc.hpSlider:setBlendFunc(gl.ONE, gl.ZERO)
    --Helper.setTimeout(npc, 20, function()
        --self.colorLayer:setVisible(false)
        --npc:setZOrder(npc.preZOrder)
        --npc:setScale(npc.preScale)
    --end)
end

function FightLayer:big_s(cmd)
    --[[
    if self.clipNode then
        self:removeChild(self.clipNode)
        self.clipNode = nil
    end
    ]]
    
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
    
    if self.maskLayer then
        self.maskLayer:setVisible(false)
        npc:setLocalZOrder(npc.preZOrder)
        --npc:setPosition(npc.prePosition)
    end
    
    if  npc.npcBook.name == "HuaiTe" then
        printJSON(cmd)
    end
    
    local dt = 0.6
    if  cmd.row then npc.row = cmd.row end
    if  cmd.col then npc.col = cmd.col end
    local toNpcs = {}
    for _, v in pairs(cmd.tSet) do
        local item = {}
        item.npc = self:getNpc(v.key)
        item.dt = v.dt
        table.insert(toNpcs, item)
    end
    npc:playSkillTriggerBuff(dt)    
    local attackHandler = self.bigSkillHandlers[npc.npcBook.name]
    attackHandler({fromNpc=npc, toNpcs=toNpcs})
    
    --big skill mask effect
    --[[
    local children = self:getChildren()
    for _, child in pairs(children) do
        if child.npcBook ~= nil and child ~= npc then
            --child:resume()
        end
    end
    ]]
end

function FightLayer:persistent_in(cmd)
    local npc = self:getNpc(cmd.key)
    local fromnpc = self:getNpc(cmd.fKey)
    --print("persistent_in")
    if npc and fromnpc then
        npc:actCMD(cmd)
        local att = nil
        if  fromnpc then
            if fromnpc.npcBook.name == "WaSiQi" then
                att = "du_wasiqi" 
            end  
        end
        npc:normalAttackHurt(cmd.hp,1,att)
    end
end

function FightLayer:biged(cmd)
    local fromnpc = self:getNpc(cmd.fKey)
    local npc = self:getNpc(cmd.key)
     if npc then
       local att = nil
       if fromnpc.npcBook.name == "XiaoHei" then
          att = "bingbao"
        elseif fromnpc.npcBook.name == "Jaina" then
            att = "bingbao_jaina"  
       elseif fromnpc.npcBook.name == "Kelthuzad" then
          att = "bingbao_kelthuzad"
        elseif fromnpc.npcBook.name == "WaSiQi" then
            att = "du_wasiqi"
       end
       npc:actCMD(cmd) 
       npc:normalAttackHurt(cmd.hp,cmd.eft,att)
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
    if npc then npc:actCMD(cmd) npc:removeBuffState("yun") end
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
        local action = cc.Sequence:create(cc.MoveTo:create(cmd.dt/30*(4/5),tPos),cc.DelayTime:create(cmd.dt/30*(1/5)))
       npc:runAction(action)
    end
end

function FightLayer:tui_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
end

function FightLayer:global_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) npc:removeBuffState("global") end
    if  self.globals_seq ~= cmd.seq then
        self.globals_seq = cmd.seq
        self.globals_num = 1
        self:globalSkillBegin(cmd.skillId,cmd.dt)
    else
        self.globals_num = self.globals_num + 1
    end
    --print(cmd.seq.."~~~~global_s~~~~~~~~~~~~globals_num:"..self.globals_num)
    
    self:globalSkillBegin(cmd.skillId,cmd.dt)
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
    if self.globals_seq == cmd.seq then
        self.globals_num = self.globals_num - 1
        if self.globals_num <= 0 then
            self:globalSkillEnd()
            self.globals_seq = nil
        end
    end
    --print(cmd.seq.."~~~~globaled~~~~~~~~~~~~globals_num:"..self.globals_num)
    if  npc:getDirection() == -1 then
        npc:setLocalZOrder(npc:getLocalZOrder()-5000)
    end
end

function FightLayer:addHp(cmd)
    local npc = self:getNpc(cmd.tkey)
    if npc then npc:actCMD(cmd) npc:normalAttackHurt(cmd.hp) end
end

function FightLayer:addHp2(cmd)
    local npc = self:getNpc(cmd.tkey)
    if npc then
        local armatureFile1 = "battle/ui/jiaxue1_juexingSkill/jiaxue1_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("jiaxue1_juexingSkill")
        --armature1:getAnimation():play("Animation1")
        local function func1()
            armature1:getAnimation():play("Animation1")
        end
        armature1:setLocalZOrder(npc:getLocalZOrder()+1)
        npc:addChild(armature1)
        
        local armatureFile2 = "battle/ui/jiaxue2_juexingSkill/jiaxue2_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile2)
        local armature2 = ccs.Armature:create("jiaxue2_juexingSkill")
        --armature2:getAnimation():play("Animation1")
        armature1:setLocalZOrder(npc:getLocalZOrder()+1)
        npc:addChild(armature2)
        local function func2()
            armature2:getAnimation():play("Animation1")
        end
       
        armature1:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(0.75),cc.RemoveSelf:create()))
        armature2:runAction(cc.Sequence:create(cc.DelayTime:create(0.75),cc.CallFunc:create(func2),cc.DelayTime:create(0.75),cc.RemoveSelf:create()))
        npc:actCMD(cmd)
        local function hurtFunc()
            npc:normalAttackHurt(cmd.hp)
        end
        self:runAction(cc.Sequence:create(cc.DelayTime:create(1.5),cc.CallFunc:create(hurtFunc)))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile2)
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    end
    local music = "sound/juexingSkill/addhp.mp3"
    Helper.PlayEffectMusic(music)  
end

function FightLayer:xuruo_s(cmd)
    print("xuruo_s")
    local npc = self:getNpc(cmd.key)
    if npc then
        local armatureFile1 = "battle/juexingSkill/xuruo_di_juexingSkill/xuruo_di_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("xuruo_di_juexingSkill")
        armature1:setVisible(false)
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        armature1:setLocalZOrder(npc:getLocalZOrder()-1)
        npc:addChild(armature1)

        local armatureFile2 = "battle/juexingSkill/xuruo_guangx_juexingSkill/xuruo_guangx_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile2)
        local armature2 = ccs.Armature:create("xuruo_guangx_juexingSkill")
        armature2:setVisible(false)
        armature2:setLocalZOrder(npc:getLocalZOrder()+1)
        npc:addChild(armature2)
        local function func2()
            armature2:setVisible(true)
            armature2:getAnimation():play("Animation1")
        end
        
        local armatureFile3 = "battle/juexingSkill/xuruo_kulou_juexingSkill/xuruo_kulou_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile3)
        local armature3 = ccs.Armature:create("xuruo_kulou_juexingSkill")
        --armature3:getAnimation():play("Animation1")
        armature3:setLocalZOrder(npc:getLocalZOrder())
        armature3:setName("kulou")
        armature3:setVisible(false)
        armature3:setScaleX(npc:getScaleX()/npc.npcBook.scaleFactor*1.2)
        armature3:setScaleY(npc:getScaleY()/npc.npcBook.scaleFactor*1.2)
        armature3:setPositionY(npc.armature:getContentSize().height+20)
        npc:addChild(armature3)
        local function func3()
            armature3:setVisible(true)
            armature3:getAnimation():play("Animation1")
        end

        armature1:runAction(cc.Sequence:create(cc.DelayTime:create(1.0),cc.CallFunc:create(func1),cc.DelayTime:create(1.2),cc.FadeOut:create(0.5),cc.RemoveSelf:create()))
        armature2:runAction(cc.Sequence:create(cc.CallFunc:create(func2),cc.DelayTime:create(1.0),cc.FadeIn:create(0.2),cc.RemoveSelf:create()))
        armature3:runAction(cc.Sequence:create(cc.DelayTime:create(1.0),cc.FadeIn:create(0.2),cc.CallFunc:create(func3)))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile2)
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile3)
    end
    local music = "sound/juexingSkill/xuruo.mp3"
    Helper.PlayEffectMusic(music)  
end

function FightLayer:xuruo_o(cmd)
    print("xuruo_o")
    local npc = self:getNpc(cmd.key)
    if npc then
        local kulou = npc:getChildByName("kulou")
        if  kulou then
            kulou:removeFromParent()
        end 
    end
end

function FightLayer:jiedu_s(cmd)
    print("jiedu_s")
    local npc = self:getNpc(cmd.key)
    if npc then
        local armatureFile1 = "battle/juexingSkill/jiedu_juexingSkill/jiedu_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("jiedu_juexingSkill")
        armature1:setVisible(false)
        armature1:setAnchorPoint(cc.p(0.5,0.25))
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        npc:addChild(armature1)

        armature1:runAction(cc.Sequence:create(cc.DelayTime:create(0),cc.CallFunc:create(func1),cc.DelayTime:create(1.2),cc.RemoveSelf:create()))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    end
    local music = "sound/juexingSkill/jiedu.mp3"
    Helper.PlayEffectMusic(music)  
end

function FightLayer:jiedu_o(cmd)
    print("jiedu_o")
end

function FightLayer:jinghua_s(cmd)
    print("jinghua_s")
    local npc = self:getNpc(cmd.key)
    if npc then
        local armatureFile1 = "battle/juexingSkill/jinghua_juexingSkill/jinghua_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("jinghua_juexingSkill")
        armature1:setVisible(false)
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        npc:addChild(armature1)

        armature1:runAction(cc.Sequence:create(cc.DelayTime:create(0),cc.CallFunc:create(func1),cc.DelayTime:create(0.8),cc.RemoveSelf:create()))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    end
    local music = "sound/juexingSkill/jiedu.mp3"
    Helper.PlayEffectMusic(music)  
end

function FightLayer:jinghua_o(cmd)
    print("jinghua_o")
end

function FightLayer:shengguang_s(cmd)
    print("shengguang_s")
    if  self.globals_seq ~= cmd.seq then
        self.globals_seq = cmd.seq
        local armatureFile = "battle/juexingSkill/shengguang_juexingSkill/shengguang_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        local armature = ccs.Armature:create("shengguang_juexingSkill")
        armature:setVisible(false)
        local function funcPlay()
            armature:setVisible(true)
            armature:getAnimation():play("Animation1")
        end
        armature:setLocalZOrder(5000)
        armature:setPosition(WIN_SIZE.width/2,WIN_SIZE.height/2)
        armature:runAction(cc.Sequence:create(cc.CallFunc:create(funcPlay),cc.DelayTime:create(1.2),cc.FadeOut:create(0.3),cc.RemoveSelf:create()))
        self:addChild(armature)
    end
    local npc = self:getNpc(cmd.key)
    if npc then
        local armatureFile1 = "battle/juexingSkill/shengguang_fazhen_juexingSkill/shengguang_fazhen_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("shengguang_fazhen_juexingSkill")
        --armature1:getAnimation():play("Animation1")
        armature1:setVisible(false)
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        armature1:setScaleX(npc:getScaleX()/npc.npcBook.scaleFactor*1.4)
        armature1:setScaleY(npc:getScaleY()/npc.npcBook.scaleFactor*1.4)
        --armature1:setLocalZOrder(npc:getLocalZOrder()-100)
        armature1:setName("shengguangFazhen")
        npc:addChild(armature1,-100)

        local armatureFile3 = "image/battle/juexingSkill/shengguang_chibang_juexingSkill/shengguang_chibang_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile3)
        local armature3 = ccs.Armature:create("shengguang_chibang_juexingSkill")
        --armature3:getAnimation():play("Animation1")
        armature3:setName("chibang")
        armature3:setScaleX(npc:getScaleX()/npc.npcBook.scaleFactor*1.4)
        armature3:setScaleY(npc:getScaleY()/npc.npcBook.scaleFactor*1.4)
        armature3:setPositionY(npc.armature:getContentSize().height+20)
        npc:addChild(armature3)
        armature3:setVisible(false)
        local function func3()
            armature3:setVisible(true)
            armature3:getAnimation():play("Animation1")
        end

        armature1:runAction(cc.Sequence:create(cc.DelayTime:create(1.5),cc.CallFunc:create(func1)))
        armature3:runAction(cc.Sequence:create(cc.DelayTime:create(1.5),cc.CallFunc:create(func3)))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile3)
    end
    local music = "sound/juexingSkill/shengguang.mp3"
    Helper.PlayEffectMusic(music)  
end

function FightLayer:shengguang_o(cmd)
    print("shengguang_o")
    local npc = self:getNpc(cmd.key)
    if npc then
        local chibang = npc:getChildByName("chibang")
        if  chibang then
            chibang:removeFromParent()
        end
        local shengguangFazhen = npc:getChildByName("shengguangFazhen")
        if  shengguangFazhen then
            shengguangFazhen:removeFromParent()
        end  
    end
end

--didn't finish cmd
function FightLayer:cibao_s(cmd)
    print("cibao_s")
    if  self.globals_seq ~= cmd.seq then
        self.globals_seq = cmd.seq
        local armatureFile = "battle/juexingSkill/diancizhendang_juexingSkill/diancizhendang_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        local armature = ccs.Armature:create("diancizhendang_juexingSkill")
        armature:setVisible(false)
        local function funcPlay()
            armature:setVisible(true)
            armature:getAnimation():play("Animation1")
        end
        armature:setLocalZOrder(5000)
        armature:setPosition(WIN_SIZE.width/2,WIN_SIZE.height*0.5)
        armature:setScaleX(2.2)
        armature:setScaleY(2.0)
        armature:runAction(cc.Sequence:create(cc.CallFunc:create(funcPlay),cc.DelayTime:create(1.2),cc.FadeOut:create(0.3),cc.RemoveSelf:create()))
        self:addChild(armature)
        

        local bg = require"scene.battle2.actors.BG"

        local size = cc.size(5,5)
        local shake = cc.Shaky3D:create(1.2/2,size,3, false)
        local seq1 = cc.Sequence:create(shake,shake:reverse())
        local seq2 = cc.Sequence:create(shake,shake:reverse())
        nodeGrid:runAction(seq1)
        bg:getNodeGrid():runAction(seq2)
    end
    local npc = self:getNpc(cmd.key)
    if npc then
        local armatureFile1 = "battle/juexingSkill/dcshouji_juexingSkill/dcshouji_juexingSkill.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("dcshouji_juexingSkill")
        --armature1:getAnimation():play("Animation1")
        armature1:setVisible(false)
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        armature1:setLocalZOrder(npc:getLocalZOrder()+100)
        armature1:setName("cibao")
        armature1:setScaleX(npc:getScaleX()/npc.npcBook.scaleFactor*1.2)
        armature1:setScaleY(npc:getScaleY()/npc.npcBook.scaleFactor*1.2)
        npc:addChild(armature1)

        armature1:runAction(cc.Sequence:create(cc.DelayTime:create(1.2),cc.CallFunc:create(func1)))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    end
    local music = "sound/juexingSkill/diancizhendang.mp3"
    Helper.PlayEffectMusic(music)  
end

function FightLayer:cibao_o(cmd)
    print("cibao_o")
    local npc = self:getNpc(cmd.key)
    if npc then
        local cibaoFazhen = npc:getChildByName("cibao")
        if  cibaoFazhen then
            cibaoFazhen:removeFromParent()
        end  
    end
end


function FightLayer:tuteng_s(cmd)
    print("tuteng_s")
    local npc = self:getNpc(cmd.key)
    --local pos = cc.p(WIN_SIZE.width/2,WIN_SIZE.height/2)--get pos from cmd
    
        if npc then
            local armatureFile = "battle/juexingSkill/TuTeng_Show_juexingSkill/TuTeng_Show_juexingSkill.csb"
            ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
            local armature = ccs.Armature:create("TuTeng_Show_juexingSkill")
            armature:getAnimation():play("Animation1")
            armature:setLocalZOrder(npc:getLocalZOrder())
            armature:setPositionX(-50)
            npc:addChild(armature)

            local armatureFile1 = "battle/juexingSkill/TuTeng_juexingSkill/TuTeng_juexingSkill.csb"
            ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
            local armature1 = ccs.Armature:create("TuTeng_juexingSkill")
            armature1:setVisible(false)
            local function func1()
                armature:removeFromParent()
                armature1:setVisible(true)
                armature1:getAnimation():play("Animation1")
            end
            npc:addChild(armature1,-100)
            armature1:setName("tuteng")
            armature1:setPositionX(-50)

            armature1:runAction(cc.Sequence:create(cc.DelayTime:create(1.0),cc.CallFunc:create(func1)))
            ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
            ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
        end
    local music = "sound/juexingSkill/tuteng.mp3"
    Helper.PlayEffectMusic(music)  

end

function FightLayer:tuteng_o(cmd)
    print("tuteng_o")
    local npc = self:getNpc(cmd.key)
    local fadeOut = cc.FadeOut:create(0.5)
    if npc then
        local tuteng = npc:getChildByName("tuteng")      
        if  tuteng then
            tuteng:runAction(cc.Sequence:create(fadeOut,cc.RemoveSelf:create()))
        end
    end
end

function FightLayer:hongzha_s(cmd)
    local  armatureFile = "battle/juexingSkill/HuoJianHongPao_juexingSkill/HuoJianHongPao_juexingSkill.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    local  armature = ccs.Armature:create("HuoJianHongPao_juexingSkill")
    local armatureFile2 = "battle/juexingSkill/HuoJianHongPao_DiMian_juexingSkill/HuoJianHongPao_DiMian_juexingSkill.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile2)
    local armature2 = ccs.Armature:create("HuoJianHongPao_DiMian_juexingSkill")
    armature2:setPosition(WIN_SIZE.width/2,WIN_SIZE.height/2)
    armature2:setScaleX(1*cmd.direction*WIN_SIZE.width*1.1/960)
    armature2:setScaleY(WIN_SIZE.height*1.1/640)
    armature2:setLocalZOrder(5000)
    armature:setLocalZOrder(5000)
    armature2:getAnimation():play("Animation1")
    --self.colorLayer:addChild(armature2)
    self:addChild(armature2)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile2)
    armature:setPosition(WIN_SIZE.width/2,WIN_SIZE.height/2)
    armature:setScaleX(1*cmd.direction*WIN_SIZE.width*1.1/960)
    armature:setScaleY(WIN_SIZE.height*1.1/640)
    armature:getAnimation():play("Animation1")
    --self.colorLayer:addChild(armature)
    self:addChild(armature)

    --self.colorLayer:setVisible(true)
    local function func()
       self.colorLayer:setVisible(false)
       armature:removeFromParent()
       armature2:removeFromParent()
    end
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
    self:runAction(cc.Sequence:create(cc.DelayTime:create(2.0),cc.CallFunc:create(func)))
    local music = "sound/juexingSkill/hongzha.mp3"
    Helper.PlayEffectMusic(music)  
end

function FightLayer:zhaohuanjianta_s(cmd)
    --local pos = cc.p(WIN_SIZE.width/2,WIN_SIZE.height/2) --set position here
    local pos = cmd.pos
    local  armatureFile = "battle/juexingSkill/JianTaZhanChe_Guang/JianTaZhanChe_Guang.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    local  armature = ccs.Armature:create("JianTaZhanChe_Guang")
    local armatureFile2 = "battle/juexingSkill/JianTaZhanChe_FaZhen/JianTaZhanChe_FaZhen.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile2)
    local armature2 = ccs.Armature:create("JianTaZhanChe_FaZhen")
    armature2:setPosition(pos)
    armature2:setVisible(false)
    self:addChild(armature2)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile2)
    self:addChild(armature)
    armature:setPosition(pos)

    --self.colorLayer:setVisible(true)
    
    local function func1()
        armature:getAnimation():play("Animation1")
    end
    
    local function func2()
        --self.colorLayer:setVisible(false)
        armature2:setVisible(true)
        armature2:getAnimation():play("Animation1")
    end
    
    local function funcend()
        armature:removeFromParent()
        armature2:removeFromParent()
    end
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
    self:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(1.0),cc.CallFunc:create(func2),cc.DelayTime:create(1.0),cc.CallFunc:create(funcend)))
    local music = "sound/juexingSkill/zhaohuan.mp3"
    Helper.PlayEffectMusic(music)  
end

function FightLayer:zhaohuanjianta_o(cmd)

end

function FightLayer:zhaohuanmofata_s(cmd)
    --local pos = cc.p(WIN_SIZE.width/2,WIN_SIZE.height/2) --set position here
    local pos = cmd.pos
    local  armatureFile = "battle/juexingSkill/MoFaChe_Guang/MoFaChe_Guang.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    local  armature = ccs.Armature:create("MoFaChe_Guang")
    local armatureFile2 = "battle/juexingSkill/MoFaChe_FaZhen/MoFaChe_FaZhen.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile2)
    local armature2 = ccs.Armature:create("MoFaChe_FaZhen")
    armature2:setPosition(pos)
    armature2:setVisible(false)
    self:addChild(armature2)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile2)
    self:addChild(armature)
    armature:setPosition(pos)

    --self.colorLayer:setVisible(true)
    
    local function func1()
        armature:getAnimation():play("Animation1")
    end
    
    local function func2()
        --self.colorLayer:setVisible(false)
        armature2:setVisible(true)
        armature2:getAnimation():play("Animation1")
    end
    
    local function funcend()
        armature:removeFromParent()
        armature2:removeFromParent()
    end
    
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
    self:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(1.0),cc.CallFunc:create(func2),cc.DelayTime:create(1.0),cc.CallFunc:create(funcend)))
    
    local music = "sound/juexingSkill/zhaohuan.mp3"
    Helper.PlayEffectMusic(music)  
end

function FightLayer:zhaohuanmofata_o(cmd)

end

function FightLayer:yanwu_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        --local pos =cc.p(npc:getHurtPosition().x,npc:getPositionY())

        local  armatureFile = "battle/juexingSkill/YanWu_Show/YanWu_Show.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        local  armature = ccs.Armature:create("YanWu_Show")
        local armatureFile2 = "battle/juexingSkill/YanWu_Quan/YanWu_Quan.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile2)
        local armature2 = ccs.Armature:create("YanWu_Quan")
        armature2:setVisible(false)
        armature2:setName("yanwu")
        armature2:setAnchorPoint(cc.p(0.5,0))
        armature2:setPosition(0,-20)
        armature2:setLocalZOrder(npc:getLocalZOrder()+100)
        npc:addChild(armature2)
        npc:addChild(armature)


        local function func1()
            armature:getAnimation():play("Animation1")
        end

        local function func2()
            armature2:setVisible(true)
            armature2:getAnimation():play("Animation1")
        end

        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile2)
        self:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(1.0),cc.CallFunc:create(func2)))
        
        local music = "sound/juexingSkill/zhaohuan.mp3"
        Helper.PlayEffectMusic(music)  
    end
    
end

function FightLayer:yanwu_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        local yanwu = npc:getChildByName("yanwu")
        if  yanwu then
            yanwu:removeFromParent()
        end  
    end
end

function FightLayer:zhaohuanfeiting_s(cmd)
    local npc = self:getNpc(cmd.key)
    local attackReadyTime = 3.0
    local attackDuration = 1.0
    local fireHeight = 0.4*WIN_SIZE.height
    if npc then
        --local pos =cc.p(npc:getHurtPosition().x,npc:getPositionY())
        local posX = npc:getPositionX()
        local posY = npc:getPositionY()
        local  armatureFile = "battle/juexingSkill/FeiTing/FeiTing.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        local  armature = ccs.Armature:create("FeiTing")
        
        armature:setLocalZOrder(10000)
        armature:setPosition(cc.p(npc:getPositionX(),WIN_SIZE.height/2))
        armature:setName("feiting")
        
        
        local armatureFile2 = "battle/juexingSkill/FeiTing_Missile/FeiTing_Missile.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile2)
        local armature2 = ccs.Armature:create("FeiTing_Missile")
        armature2:setVisible(false)
        armature2:getAnimation():play("move")
        armature2:setPosition(cc.p(npc:getPositionX(),fireHeight+WIN_SIZE.height/2))
        armature2:setName("feiting_missile")
        armature2:setAnchorPoint(cc.p(0.5,0))
        --armature2:setLocalZOrder(npc:getLocalZOrder()+100)



        self:addChild(armature2)
        self:addChild(armature)


        local function func1()
            armature:getAnimation():play("attack")
        end
        
        local function func3()
            armature2:getAnimation():play("end")
        end
        
        local function func2()
            if  armature2 then
                armature2:setVisible(true)
                armature2:runAction(cc.Sequence:create(cc.MoveTo:create(attackDuration,cc.p(posX,posY)),cc.CallFunc:create(func3)))
            end
        end
        


        self:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(attackReadyTime),cc.CallFunc:create(func2)))

        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
        
        local music = "sound/juexingSkill/zhaohuan.mp3"
        Helper.PlayEffectMusic(music)  
    end
end

function FightLayer:zhaohuanfeiting_o(cmd)
    local feiting = self:getChildByName("feiting")
    local missile = self:getChildByName("feiting_missile")
    if  feiting then
        feiting:removeFromParent()
    end
    if  missile then
        missile:removeFromParent()
    end
end

function FightLayer:weixingdingwei_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        --local pos =cc.p(npc:getHurtPosition().x,npc:getPositionY())

        local  armatureFile = "battle/juexingSkill/WeiXingDingWei_Guang/WeiXingDingWei_Guang.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        local  armature = ccs.Armature:create("WeiXingDingWei_Guang")
        local armatureFile2 = "battle/juexingSkill/WeiXingDingWei_Quan/WeiXingDingWei_Quan.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile2)
        local armature2 = ccs.Armature:create("WeiXingDingWei_Quan")
        armature2:setVisible(false)
        
        armature2:setPosition(cc.p(0,-20))
        armature2:setName("dingwei")
        armature2:setAnchorPoint(cc.p(0.5,0))
        armature2:setLocalZOrder(npc:getLocalZOrder()+100)
        
        armature:setLocalZOrder(npc:getLocalZOrder()+100)
        armature:setPosition(cc.p(0,npc.armature:getContentSize().height/2))
        
        npc:addChild(armature2)
        npc:addChild(armature)


        local function func1()
            armature:getAnimation():play("Animation1")
        end

        local function func2()
            armature2:setVisible(true)
            armature2:getAnimation():play("Animation1")
        end

        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile2)
        self:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(0.8),cc.CallFunc:create(func2)))
        
        local music = "sound/juexingSkill/weixingdingwei.mp3"
        Helper.PlayEffectMusic(music)  
    end
end

function FightLayer:weixingdingwei_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        local dingwei = npc:getChildByName("dingwei")
        if  dingwei then
            dingwei:removeFromParent()
        end  
    end
end

function FightLayer:zhaohuanfazhen_s(cmd)
    --local pos = cc.p(WIN_SIZE.width/2,WIN_SIZE.height/2)
    local pos = cmd.pos
    --[[
    local armatureFile = "battle/juexingSkill/ZhaoHuanFaZhen_juexingSkill/ZhaoHuanFaZhen_juexingSkill.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    local armature = ccs.Armature:create("ZhaoHuanFaZhen_juexingSkill")
    armature:setName("zhaohuanzhen")
    armature:getAnimation():play("Animation1")
    armature:setPosition(pos)
    self:addChild(armature)

    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
    
    local music = "sound/juexingSkill/zhaohuanfazhen.mp3"
    Helper.PlayEffectMusic(music)  
    ]]
end

function FightLayer:zhaohuanfazhen_o(cmd)
    local fazhen = self:getChildByName("zhaohuanzhen")
    if  fazhen then
        fazhen:removeFromParent()
    end
end

-- -----------------------
function FightLayer:ice_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then  npc:actCMD(cmd) npc:setBuffState("ice") end
end

function FightLayer:ice_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) npc:removeBuffState("ice") end
end

function FightLayer:bound_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        local armatureFile1 = "battle/skill/LuKui/bigSkillAttack_LuKui_ShuTeng/bigSkillAttack_LuKui_ShuTeng.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("bigSkillAttack_LuKui_ShuTeng")
        armature1:setVisible(false)
        armature1:setName("shuTeng")
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        npc:addChild(armature1)

        armature1:runAction(cc.Sequence:create(cc.DelayTime:create(0),cc.CallFunc:create(func1)))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    end
end

function FightLayer:bound_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        local shuTeng = npc:getChildByName("shuTeng")
        if  shuTeng then
            shuTeng:removeFromParent()
        end  
    end
end

function FightLayer:interrupt_s(cmd)
    print("interrupt_s")
    local npc = self:getNpc(cmd.key)
    if npc then  npc:actCMD(cmd) end
end

function FightLayer:interrupt_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
end

function FightLayer:silence_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then  npc:actCMD(cmd) npc:setBuffState("silence") end
end

function FightLayer:silence_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) npc:removeBuffState("silence") end
end

function FightLayer:sheep_s(cmd)
    --print("sheep_s")
    local npc = self:getNpc(cmd.key)
    if  npc.isSheepState == nil then
        npc.isSheepState = 0
    end
    npc.isSheepState = npc.isSheepState +1
        
    if  npc.isSheepState == 1 then
        local armatureFile = "image/battle/skill/WoJin/bigSkillAttack_WoJin_Sheep/bigSkillAttack_WoJin_Sheep.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        local armature = ccs.Armature:create("bigSkillAttack_WoJin_Sheep")
        --armature3:getAnimation():play("Animation1")
        --armature:setPosition(npc:getPosition())
        armature:setScaleX(-math.abs(npc:getScaleX()/npc.npcBook.scaleFactor)*0.66)
        armature:setScaleY(npc:getScaleY()/npc.npcBook.scaleFactor*0.66)
        armature:setName("sheep")
        armature:setLocalZOrder(npc.armature:getLocalZOrder())
        npc:addChild(armature)
        armature:setVisible(false)
        local function func()
            armature:setVisible(true)
            armature:getAnimation():play("Animation1")
            npc.armature:setVisible(false)
        end
        armature:runAction(cc.Sequence:create(cc.CallFunc:create(func)))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
    end
end

function FightLayer:sheep_o(cmd)
    local npc = self:getNpc(cmd.key)
    if  npc.isSheepState == nil then
        npc.isSheepState = 0
    end
    npc.isSheepState = npc.isSheepState -1
    if  npc and npc.isSheepState == 0 then
        npc.armature:setVisible(true)
        local sheep = npc:getChildByName("sheep")
        if sheep then
           sheep:removeFromParent()
        end
        npc.armature:setVisible(true)
    end
end

function FightLayer:AErSaSi_shield_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc == nil then return end
    npc:setShield(cmd.hp)
    print("[AErSaSi_shield_s]"..npc.npcBook.name)
    local armatureFile1 = "battle/skill/AErSaSi/smallSkillAttack_AErSaSi_TianshiBuffFaZhen/smallSkillAttack_AErSaSi_TianshiBuffFaZhen.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
    local armature1 = ccs.Armature:create("smallSkillAttack_AErSaSi_TianshiBuffFaZhen")
    --armature1:getAnimation():play("Animation1")
    armature1:setVisible(false)
    armature1:getAnimation():setSpeedScale(0.5)
    local function func1()
        armature1:setVisible(true)
        armature1:getAnimation():play("Animation1")
    end
    npc:addChild(armature1,-1)

    local armatureFile2 = "battle/skill/AErSaSi/smallSkillAttack_AErSaSi_TianshiBuff/smallSkillAttack_AErSaSi_TianshiBuff.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile2)
    local armature2 = ccs.Armature:create("smallSkillAttack_AErSaSi_TianshiBuff")
    --armature1:getAnimation():play("Animation1")
    armature2:setVisible(false)
    armature2:getAnimation():setSpeedScale(0.5)
    armature2:setPositionY(-10)
    local function func2()
        armature2:setVisible(true)
        armature2:getAnimation():play("Animation1")
    end
    npc:addChild(armature2,-1)

    local armatureFile3 = "battle/skill/AErSaSi/smallSkillAttack_AErSaSi_TianshiBuffDunPai/smallSkillAttack_AErSaSi_TianshiBuffDunPai.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile3)
    local armature3 = ccs.Armature:create("smallSkillAttack_AErSaSi_TianshiBuffDunPai")
    armature3:getAnimation():setSpeedScale(0.5)
    armature3:setPositionY(35)
    armature3:setPositionX(npc.npcBook.hpPosition.x)
    --armature3:getAnimation():play("Animation1")

    npc:addChild(armature3,-1)
    armature3:setVisible(false)
    local function func3()
        armature3:setVisible(true)
        --armature3:getAnimation():play("Animation1")
    end
    
    armature1:setScaleX(npc:getScaleX()/npc.npcBook.scaleFactor*1.2)
    armature1:setScaleY(npc:getScaleY()/npc.npcBook.scaleFactor*1.2)
    armature2:setScaleX(npc:getScaleX()/npc.npcBook.scaleFactor*0.8)
    armature2:setScaleY(npc:getScaleY()/npc.npcBook.scaleFactor*0.8)
    armature3:setScaleX(npc:getScaleX()/npc.npcBook.scaleFactor*0.7)
    armature3:setScaleY(npc:getScaleY()/npc.npcBook.scaleFactor*0.7)
    
    armature1:setName("AErSaSi_shield1")
    armature2:setName("AErSaSi_shield2")
    armature3:setName("AErSaSi_shield3")
    armature1:runAction(cc.Sequence:create(cc.CallFunc:create(func1)))
    armature2:runAction(cc.Sequence:create(cc.CallFunc:create(func2)))
    armature3:runAction(cc.Sequence:create(cc.CallFunc:create(func3)))
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile2)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile3)
end

function FightLayer:AErSaSi_shield_o(cmd)
    local npc = self:getNpc(cmd.key)
    if  npc then
        local shield1 = npc:getChildByName("AErSaSi_shield1")
        local shield2 = npc:getChildByName("AErSaSi_shield2")
        local shield3 = npc:getChildByName("AErSaSi_shield3")
        if  shield1 and shield2 and shield3 then
            shield1:runAction(cc.Sequence:create(cc.FadeOut:create(0.5),cc.RemoveSelf:create()))
            shield2:runAction(cc.Sequence:create(cc.FadeOut:create(0.5),cc.RemoveSelf:create()))
            shield3:runAction(cc.Sequence:create(cc.FadeOut:create(0.5),cc.RemoveSelf:create()))
        end
        npc:hideShield()
    end
    print("[AErSaSi_shield_o]"..npc.npcBook.name)
end

function FightLayer:Fan_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        npc.armature:setScaleX(npc.armature:getScaleX()*-1)
    end
end

function FightLayer:Fan_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        npc.armature:setScaleX(npc.armature:getScaleX()*-1)
    end
end

function FightLayer:suspend_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        npc:pause()
    end
end

function FightLayer:suspend_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        npc:resume()
    end
end

function FightLayer:fucked_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
end

function FightLayer:fucked_o(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then npc:actCMD(cmd) end
end

function FightLayer:jianfang_s(cmd)
    local npc = self:getNpc(cmd.key)
    if npc then
        npc:normalAttackHurt(cmd.val,"jianfang")
    end
end

------------attack handlers function define------------
function FightLayer:attack_Captain(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_DuanGuZhe(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_FanKeLiFu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_FengXing(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local arrow = cc.Sprite:create("image/battle/skill/FengXing/jian.png")
    arrow:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    arrow:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then arrow:setScaleX(actor:getScaleX()) arrow:setScaleY(actor:getScaleY()) end
    if  actor.npcBook.boss == 1 then
        arrow:setScaleX(actor:getScaleX()) arrow:setScaleY(actor:getScaleY())
    end
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
    Helper.PlayEffectMusic(music)]]

end

function FightLayer:attack_HuaiTe(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/huaite/normalSkillAttack_huaite.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end

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
    Helper.PlayEffectMusic(music)]]
    
end

function FightLayer:attack_KuLouWang(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_LaZuoKeLao(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_Lina(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/LiNa/xiaohuoqiu.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_XiaoHei(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local arrow = cc.Sprite:create("image/battle/skill/XiaoHei/jian.png")
    arrow:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    arrow:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then arrow:setScaleX(actor:getScaleX()) arrow:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
    
end

function FightLayer:attack_ZhuRiZhe(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/LiNa/xiaohuoqiu.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end

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
    Helper.PlayEffectMusic(music)]]
    
end

function FightLayer:attack_Jaina(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    --130 67   190 98
    local spriteScaleX = 190/130
    local spriteScaleY = 98/67
    local sprite = cc.Sprite:create("battle/skill/Jaina/normalSkillAttack_jaina.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX())*spriteScaleX)
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())*spriteScaleY)

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
    Helper.PlayEffectMusic(music)]]
    
end

function FightLayer:attack_Tyrande(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local arrow = cc.Sprite:create("image/battle/skill/Tyrande/mofadan.png")
    if  actor.npcBook.boss == 1 then arrow:setScaleX(actor:getScaleX()) arrow:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_DiXueLingZhu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_Kelthuzad(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local spriteScaleX = 190/130
    local spriteScaleY = 98/67
    local sprite = cc.Sprite:create("battle/skill/Jaina/normalSkillAttack_jaina.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX())*spriteScaleX)
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())*spriteScaleY)
    if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
    
end

function FightLayer:attack_YiLiDan(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_ShuiYuanSu(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/ShuiYuanSu/normalSkillAttack_shuiyuansu.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
    
end

function FightLayer:attack_ShahelasiZhuMu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_LaGeNaLuoSi(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/LiNa/xiaohuoqiu.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_AoNiKeXiYa(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/AoNiKeXiYa/normalSkillAttack_aonikexiya.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_LaSuoLiAn(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_KeLuoMaGuSi(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    if targetActor == nil then return end
    local dt = param.dt
    local spriteScaleX = 190/130
    local spriteScaleY = 98/67
    local sprite = cc.Sprite:create("battle/skill/Jaina/normalSkillAttack_jaina.png")
    sprite:setScaleX(actor:getScaleX()*spriteScaleX)
    sprite:setScaleY(actor:getScaleY()*spriteScaleY)
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_MaNuoLuoSi(param)
    
end

function FightLayer:attack_AErSaSi(param)

end

function FightLayer:attack_AKeMengDe(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/AKeMengDe/normalSkillAttack_AKeMengDe.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_WaSiQi(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local arrow = cc.Sprite:create("image/battle/skill/WaSiQi/normalSkillAttack_WaSiQi_arrow.png")
    arrow:setScaleY(actor:getScaleX()/math.abs(actor:getScaleX()))
    arrow:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then arrow:setScaleX(actor:getScaleX()) arrow:setScaleY(actor:getScaleY()) end
    
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_XianXueNvWangLan(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local spriteScaleX = 190/130
    local spriteScaleY = 98/67
    local sprite = cc.Sprite:create("battle/skill/Jaina/normalSkillAttack_jaina.png")
    sprite:setScaleX(actor:getScaleX()*spriteScaleX)
    sprite:setScaleY(actor:getScaleY()*spriteScaleY)
    sprite:setColor(cc.c3b(17,155,55))
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_KaZhaKe(param)

end

function FightLayer:attack_WoJin(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    
    if targetActor == nil then return end
    local sprite = cc.Sprite:create()
    sprite:setPosition(actor:getFirePosition())
    sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)
    if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end

    Helper.addSpriteFrames("battle/skill/WoJin/normalSkillAttack_WoJin.plist","battle/skill/WoJin/normalSkillAttack_WoJin.pvr.ccz")
    local frameArr = Helper.newFrames("feidao1_%d_WoJin.png",21,4)
    local animation = Helper.newAnimation(frameArr,0.2/4)
    animation:setLoops(dt/0.2)
    
    local attackStart = cc.CallFunc:create(function()  sprite:setVisible(true) end)

    local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
    local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, spwan, attackOver))
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_LuKui(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/LuKui/normalSkillAttack_LuKui.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end

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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_FaShi(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/FaShi/normalSkillAttack_FaShi.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_GongJianShou(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/GongJianShou/normalSkillAttack_GongJianShou.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_KeDuoShou(param)
    
end

function FightLayer:attack_NvYao(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/NvYao/normalSkillAttack_NvYao.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_XiaoYuRen(param)
    
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
    Helper.PlayEffectMusic(music)
    ]]
end

function FightLayer:attack_JuDuZhanChe(param)
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
        endAngle = -210
    end
    endAngle = endAngle + 90
    startAngle = startAngle +90

    local lizi = cc.Node:create()
    local ParticleSystemQuad = cc.ParticleSystemQuad:create("battle/skill/JuDuZhanCheMissile/missilesmoke.plist")
    lizi:setPosition(actor:getFirePosition())
    lizi:addChild(ParticleSystemQuad)
    self:addChild(lizi)

    local armatureFile1 = "battle/skill/JuDuZhanCheMissile_BaoZha/JuDuZhanCheMissile_BaoZha.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
    local armature1 = ccs.Armature:create("JuDuZhanCheMissile_BaoZha")

    self:addChild(armature1)
    armature1:setPosition(cc.p(targetActor:getHurtPosition().x,targetActor:getPositionY()))
    armature1:setVisible(false)
    armature1:setLocalZOrder(targetActor:getLocalZOrder())


    local copy = cc.Sprite:create("battle/skill/JuDuZhanCheMissile/missile.png")

    copy:setPosition(actor:getFirePosition())
    copy:setVisible(false)
    copy:setLocalZOrder(targetActor:getLocalZOrder())
    copy:setRotation(startAngle)
    --armature1:getAnimation():play("Animation1")
    --armature1:setAnchorPoint(cc.p(0.5,0.25))
    self:addChild(copy)

    if  actor.npcBook.boss == 1 then
        copy:setScaleX(actor:getScaleX()) copy:setScaleY(actor:getScaleY())
        copy:setScaleX(actor:getScaleX()) copy:setScaleY(actor:getScaleY())
    end

    local spriteHeight = copy:getContentSize()

    local arrowParabola = MyDIYAction.MissleParabola_NoRotate(actor:getFirePosition(),cc.p(targetActor:getHurtPosition().x,targetActor:getPositionY()),startAngle,endAngle,dt,
        spriteHeight.height,100)
    local arrowParabola2 = MyDIYAction.MissleParabola(actor:getFirePosition(),cc.p(targetActor:getHurtPosition().x,targetActor:getPositionY()),startAngle,endAngle,dt,
        spriteHeight.height,100)
    local attackOver = cc.CallFunc:create(function()
        armature1:getAnimation():play("end")
    end)
    armature1:runAction(cc.Sequence:create(cc.DelayTime:create(dt),cc.Show:create(),attackOver,cc.DelayTime:create(1.0),cc.RemoveSelf:create()))
    lizi:runAction(cc.Sequence:create(arrowParabola,cc.RemoveSelf:create()))
    copy:runAction(cc.Sequence:create(cc.Show:create(), arrowParabola2, cc.RemoveSelf:create()))
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)

    --[[
    local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)
    ]]
end

function FightLayer:attack_MoFaZhanChe(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    local direction = actor.npcBook.direction
    if targetActor == nil then return end
    local sprite = cc.Sprite:create("battle/skill/MoFaZhanChe/laser1_MoFaZhanChe.png")
    sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
    sprite:setPosition(actor:getFirePosition())
    sprite:setAnchorPoint(cc.p(0,0.5))
    
    local sprite2 = cc.Sprite:create("battle/skill/MoFaZhanChe/laser2_MoFaZhanChe.png")
    sprite2:setPosition(cc.p(sprite:getContentSize().width,0))
    sprite2:setAnchorPoint(cc.p(0,0))
    sprite2:setScaleX(WIN_SIZE.width/47)
    
    sprite:addChild(sprite2)
    
    sprite:setVisible(false)
    sprite:setLocalZOrder(targetActor:getLocalZOrder())
    self:addChild(sprite)
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local moveTo = cc.DelayTime:create(dt)
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))

    --[[local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:attack_PuTongTanKe(param)
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
        endAngle = -210
    end

    local lizi = cc.Node:create()
    local ParticleSystemQuad = cc.ParticleSystemQuad:create("battle/skill/PuTongTanKeMissile/tuowei.plist")
    lizi:setPosition(actor:getFirePosition())
    lizi:addChild(ParticleSystemQuad)
    self:addChild(lizi)

    local armatureFile1 = "battle/skill/PuTongTanKeMissile_BaoZha/PuTongTanKeMissile_BaoZha.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
    local armature1 = ccs.Armature:create("PuTongTanKeMissile_BaoZha")

    self:addChild(armature1)
    armature1:setPosition(cc.p(targetActor:getHurtPosition().x,targetActor:getPositionY()))
    armature1:setVisible(false)
    armature1:setLocalZOrder(targetActor:getLocalZOrder())


    local copy = cc.Sprite:create("battle/skill/PuTongTanKeMissile/tieqiu.png")

    copy:setPosition(actor:getFirePosition())
    copy:setVisible(false)
    copy:setLocalZOrder(targetActor:getLocalZOrder())

    --armature1:getAnimation():play("Animation1")
    --armature1:setAnchorPoint(cc.p(0.5,0.25))
    self:addChild(copy)

    if  actor.npcBook.boss == 1 then
        copy:setScaleX(actor:getScaleX()) copy:setScaleY(actor:getScaleY())
        copy:setScaleX(actor:getScaleX()) copy:setScaleY(actor:getScaleY())
    end

    local spriteHeight = copy:getContentSize()


    local arrowParabola = MyDIYAction.MissleParabola_NoRotate(actor:getFirePosition(),cc.p(targetActor:getHurtPosition().x,targetActor:getPositionY()),startAngle,endAngle,dt,
        spriteHeight.height,100)
    local arrowParabola2 = MyDIYAction.MissleParabola(actor:getFirePosition(),cc.p(targetActor:getHurtPosition().x,targetActor:getPositionY()),startAngle,endAngle,dt,
        spriteHeight.height,100)
    local attackOver = cc.CallFunc:create(function()
        armature1:getAnimation():play("end")
    end)
    armature1:runAction(cc.Sequence:create(cc.DelayTime:create(dt),cc.Show:create(),attackOver,cc.DelayTime:create(1.0),cc.RemoveSelf:create()))
    lizi:runAction(cc.Sequence:create(arrowParabola,cc.RemoveSelf:create()))
    copy:runAction(cc.Sequence:create(cc.Show:create(), arrowParabola2, cc.RemoveSelf:create()))
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    
    --[[
    local music = actor.npcBook.normalMusic
    Helper.PlayEffectMusic(music)
    ]]
end

function FightLayer:attack_ZhongZhuangTanKe(param)
    local actor = param.fromNpc
    local targetActor = param.toNpc
    local dt = param.dt
    if targetActor == nil then return end 
    
    --[[
    local armatureFile1 = "battle/skill/ZhongZhuangTanKeAtk/ZhongZhuangTanKeAtk.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
    local armature1 = ccs.Armature:create("ZhongZhuangTanKeAtk")
    
    local point = {}
    table.merge(point,actor.npcBook.firePosition)
    --point.x = point.x*actor.npcBook.direction
    local scale = actor.npcBook.scaleFactor
    point =cc.p(point.x*scale,point.y*scale)
    armature1:setScale(0.8)
    armature1:setPosition(point)
    armature1:setRotation(-5)
    
    --armature1:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    --armature1:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY()))
    
    --armature1:setVisible(false)
    actor:addChild(armature1)
    local attackStart = cc.CallFunc:create(function() armature1:setVisible(true) end)
    local function func1()
        armature1:getAnimation():play("attack")
    end
    local function func2()
        armature1:getAnimation():play("loop")
    end
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(armature1)
    end)
    armature1:runAction(cc.Sequence:create(attackStart, cc.CallFunc:create(func1),cc.DelayTime:create(0.3),cc.CallFunc:create(func2),cc.DelayTime:create(2.0), attackOver))
        ]]
end

------------small skill handlers function define------------
function FightLayer:smallSkill_Captain(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_DuanGuZhe(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_FanKeLiFu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_FengXing(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local arrow = cc.Sprite:create("image/battle/skill/FengXing/jian.png")
        arrow:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
        arrow:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then arrow:setScaleX(actor:getScaleX()) arrow:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
    
end

function FightLayer:smallSkill_HuaiTe(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
        sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
    
end

function FightLayer:smallSkill_KuLouWang(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_LaZuoKeLao(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_Lina(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
        sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
   
end

function FightLayer:smallSkill_XiaoHei(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create("battle/skill/XiaoHei/zidan.png")
        sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
        sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
    
end

function FightLayer:smallSkill_ZhuRiZhe(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
        sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
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
    local spriteScaleX = 532/320
     local spriteScaleY = 136/82
    local direction = actor:getDirection()
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        --print("dt:"..dt)
        if targetActor == nil then return end
        local arrow = cc.Sprite:create("battle/skill/Jaina/smallSkillAttack_Jaina.png")  
        --arrow:setScaleX(actor:getScaleX())
        local firePosition = cc.p(actor:getFirePosition().x,actor:getFirePosition().y+20)
        arrow:setScaleY(actor:getScaleY())
        --arrow:setPosition(actor:getFirePosition())
        arrow:setPosition(firePosition)
        arrow:setVisible(false)
        --arrow:setAnchorPoint(cc.p(1,0))
        arrow:setLocalZOrder(targetActor:getLocalZOrder())
        self:addChild(arrow)             
        arrow:setScale(spriteScaleX,spriteScaleY)
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
    Helper.PlayEffectMusic(music)]]
   
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
        --sprite1:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite1:setVisible(false)
        sprite1:setLocalZOrder(targetActor:getLocalZOrder()-1)
        targetActor:addChild(sprite1)
        sprite1:setColor(cc.c3b(255,252,112))

        local sprite2 = cc.Sprite:create()
        sprite2:setAnchorPoint(cc.p(0.5,0))
        --sprite2:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite2:setVisible(false)
        sprite2:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(sprite2)
        sprite2:setColor(cc.c3b(255,252,112))
        
        local sprite3 = cc.Sprite:create()
        sprite3:setAnchorPoint(cc.p(0.5,0))
        --sprite3:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite3:setVisible(false)
        sprite3:setLocalZOrder(targetActor:getLocalZOrder()+1)
        targetActor:addChild(sprite3)
        
        local sprite4 = cc.Sprite:create()
        sprite4:setAnchorPoint(cc.p(0.5,0))
        --sprite4:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite4:setVisible(false)
        sprite4:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(sprite4)
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_DiXueLingZhu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_Kelthuzad(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create("battle/skill/Kelthuzad/smallSkillAttack_Kelthuzad.png")
        sprite:setPosition(targetActor.npcBook.hurtPosition)
        sprite:setScale(0.2)
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        targetActor:addChild(sprite)
        local spwan = cc.Spawn:create(cc.FadeIn:create(dt/2),cc.ScaleTo:create(dt,1.5))
        sprite:runAction(cc.Sequence:create(cc.Show:create(),spwan, cc.RemoveSelf:create()))
    end

    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_YiLiDan(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_ShahelasiZhuMu(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_LaGeNaLuoSi(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    local spriteScale = 200/110 
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
        local frameArr = Helper.newFrames("feilun_%d_LaGeNaLuoSi.png",0,6)
        local animation = Helper.newAnimation(frameArr,dt/6)
        sprite:setScale(spriteScale)
        
        local attackStart = cc.CallFunc:create(function()  sprite:setVisible(true) end)
        
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, spwan, attackOver))
        --sprite2:runAction(cc.Sequence:create(cc.DelayTime:create(dt),cc.Show:create(), spwan, cc.RemoveSelf:create()))
        --sprite3:runAction(cc.Sequence:create(cc.DelayTime:create(dt*2),cc.Show:create(), spwan, cc.RemoveSelf:create()))
        

    end
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_AoNiKeXiYa(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    
    local picScaleX = 329/180
    local picScaleY = 332/182
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setPosition(cc.p(0,-50))
        --targetActor.armature:getContentSize().height*targetActor.npcBook.scaleFactor*0.5-10
        sprite:setVisible(false)
        sprite:setAnchorPoint(cc.p(0.5,0))
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(sprite)
        sprite:setScaleX(picScaleX)
        sprite:setScaleY(picScaleY)

        Helper.addSpriteFrames("battle/skill/AoNiKeXiYa/smallSkillAttack_aonikexiya.plist","battle/skill/AoNiKeXiYa/smallSkillAttack_aonikexiya.pvr.ccz")
        local frameArr = Helper.newFrames("baoyan_%d_AoNiKeXiYa.png",4,5)
        local animation = Helper.newAnimation(frameArr,dt/5)


        sprite:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation), cc.RemoveSelf:create()))
    end
    
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)
    ]]
end

function FightLayer:smallSkill_LaSuoLiAn(param)
    
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local armatureFile1 = "battle/skill/LaSuoLiAn/smallSkillAttack_LaSuoLiAn_HuDun/smallSkillAttack_LaSuoLiAn_HuDun.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("smallSkillAttack_LaSuoLiAn_HuDun")
        armature1:setName("HuDun_LaSuoLiAn")
        armature1:setScaleX(targetActor:getScaleX()/math.abs(targetActor:getScaleX()))
        armature1:setScaleY(targetActor:getScaleY()/math.abs(targetActor:getScaleY()))
        armature1:setVisible(false)
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        targetActor:addChild(armature1)

        armature1:runAction(cc.Sequence:create(cc.DelayTime:create(0),cc.CallFunc:create(func1),cc.DelayTime:create(6.0),cc.RemoveSelf:create()))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    end

    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]    
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
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_MaNuoLuoSi(param)

end

function FightLayer:smallSkill_AErSaSi(param)
        local actor = param.fromNpc
    local targetActors = param.toNpcs
    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
   
end

function FightLayer:smallSkill_AKeMengDe(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local armatureFile1 = "battle/skill/AKeMengDe/smallSkillAttack_AKeMengDe/smallSkillAttack_AKeMengDe.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("smallSkillAttack_AKeMengDe")
        armature1:setVisible(false)
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        targetActor:addChild(armature1)

        armature1:runAction(cc.Sequence:create(cc.DelayTime:create(0),cc.CallFunc:create(func1),cc.DelayTime:create(1.5),cc.RemoveSelf:create()))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    end

    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_WaSiQi(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create("image/battle/skill/WaSiQi/smallSkillAttack_WaSiQi_arrow.png")
        sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
        sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_XianXueNvWangLan(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create("battle/skill/XianXueNvWangLan/smallSkillAttack_XianXueNvWangLan.png")
        sprite:setPosition(actor:getFirePosition())
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        local spwan = cc.Spawn:create(moveTo)
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, spwan, attackOver))
    end


    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_KaZhaKe(param)

end

function FightLayer:smallSkill_WoJin(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    --206 167 120 97
    local spriteScaleX = 206/120
    local spriteScaleY = 167/97

    local sprite = cc.Sprite:create()
    sprite:setPosition(actor:getFirePosition())
    sprite:setVisible(false)
    sprite:setScaleX(spriteScaleX) sprite:setScaleY(spriteScaleY)
    if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
    sprite:setLocalZOrder(actor:getLocalZOrder())
    self:addChild(sprite)
    
    Helper.addSpriteFrames("battle/skill/WoJin/smallSkillAttack_WoJin.plist","battle/skill/WoJin/smallSkillAttack_WoJin.pvr.ccz")
    local frameArr = Helper.newFrames("feidao2_%02d_WoJin.png",1,4)
    local animation = Helper.newAnimation(frameArr,0.2/4)
    
    
    local dt = 0
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
        dt = dt+arrowDt
        local arrowPos = cc.p(targetActor:getHurtPosition().x,actor:getFirePosition().y)
        local moveTo = cc.MoveTo:create(arrowDt,arrowPos)
        table.insert(moveTotab,moveTo)
    end
    animation:setLoops(dt/0.2)
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local sequence = cc.Sequence:create(moveTotab)
    local spwan = cc.Spawn:create(sequence,cc.Animate:create(animation))
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart,spwan, attackOver))

end

function FightLayer:smallSkill_LuKui(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        
        local armatureFile1 = "battle/skill/LuKui/smallSkillAttack_LuKui_GuangWen/smallSkillAttack_LuKui_GuangWen.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("smallSkillAttack_LuKui_GuangWen")
        --armature1:getAnimation():play("Animation1")
        armature1:setVisible(false)
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        targetActor:addChild(armature1)

        local armatureFile3 = "battle/skill/LuKui/smallSkillAttack_LuKui_FuWen/smallSkillAttack_LuKui_FuWen.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile3)
        local armature3 = ccs.Armature:create("smallSkillAttack_LuKui_FuWen")
        --armature3:getAnimation():play("Animation1")

        targetActor:addChild(armature3)
        armature3:setVisible(false)
        local function func3()
            armature3:setVisible(true)
            armature3:getAnimation():play("Animation1")
        end

        armature1:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(dt),cc.RemoveSelf:create()))
        armature3:runAction(cc.Sequence:create(cc.CallFunc:create(func3),cc.DelayTime:create(dt),cc.RemoveSelf:create()))
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile3)
        
    end

    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)
    ]]
end

function FightLayer:smallSkill_FaShi(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create("battle/skill/FaShi/smallSkillAttack_FaShi.png")
        sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
        sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
        sprite:setPosition(actor:getFirePosition())
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        --local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    end


    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_GongJianShou(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create("battle/skill/XiaoHei/zidan.png")
        sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
        sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
        sprite:setPosition(actor:getFirePosition())
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        --local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    end


    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_KeDuoShou(param)

end

function FightLayer:smallSkill_NvYao(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create("battle/skill/NvYao/smallSkillAttack_NvYao.png")
        sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
        sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
        sprite:setPosition(actor:getFirePosition())
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)

        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local moveTo = cc.MoveTo:create(dt, targetActor:getHurtPosition())
        --local spwan = cc.Spawn:create(moveTo,cc.Animate:create(animation))
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(attackStart, moveTo, attackOver))
    end


    --[[local music = actor.npcBook.smallMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:smallSkill_tonglingta(param)
end

function FightLayer:smallSkill_Cannon(param)
end

------------big skill handlers function define------------
function FightLayer:bigSkill_Captain(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    --[[
    if #targetActors == 0 then return end
    local targetActor = targetActors[1].npc
    if targetActor == nil then return end
    ]]
    --local dt = targetActors[1].dt
    local dt = 1.5
    if targetActors[1] then
        dt = targetActors[1].dt
    end
    local ship = cc.Sprite:create()
    ship:setLocalZOrder(actor:getLocalZOrder())
    ship:setAnchorPoint(cc.p(0.5,0.5))
    ship:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
    ship:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY()))
    local moveToPos = nil
    local pos1 = nil
    if  actor.npcBook.direction == 1 then
        ship:setPosition(WIN_SIZE.width*0,WIN_SIZE.height*0.7)
        moveToPos = cc.p(WIN_SIZE.width*0.75,WIN_SIZE.height*0.7)
        local maxX = WIN_SIZE.width*1.0
        for i=1,#targetActors do
            local targetActor = targetActors[i].npc
            if  targetActor and targetActor:getHurtPosition().x < maxX then
                maxX = targetActor:getHurtPosition().x
            end
        end
        moveToPos.x = maxX
    else
        ship:setPosition(WIN_SIZE.width*1.0,WIN_SIZE.height*0.7)
        moveToPos = cc.p(WIN_SIZE.width*0.25,WIN_SIZE.height*0.7)
        local minX = WIN_SIZE.width*0
        for i=1,#targetActors do
            local targetActor = targetActors[i].npc
            if  targetActor:getHurtPosition().x > minX then
                minX = targetActor:getHurtPosition().x
            end
        end
        moveToPos.x = minX
    end 
    ship:setVisible(false)
    self:addChild(ship)

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
    
    local function shipShuiHua()
        for i=1,#targetActors do
            local targetActor = targetActors[i].npc
            local armatureFile1 = "battle/skill/captain/bigSkillAttack_Captain_ShanDian/bigSkillAttack_Captain_ShanDian.csb"
            ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
            local armature1 = ccs.Armature:create("bigSkillAttack_Captain_ShanDian")
            armature1:setVisible(false)
            local function func1()
                armature1:setVisible(true)
                armature1:setScaleX(-0.5)
                armature1:setScaleY(0.5)
                armature1:getAnimation():play("Animation1")
            end
            --armature1:setLocalZOrder(targetActor:getLocalZOrder())
            targetActor:addChild(armature1)



            armature1:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(0.6),cc.RemoveSelf:create()))

            ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
        end
        
    end
    
    
    ship:runAction(cc.Sequence:create(attackStart,spwan,cc.CallFunc:create(shipShuiHua),fadeout,attackOver))

    
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
    
end

function FightLayer:bigSkill_DuanGuZhe(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
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
    Helper.PlayEffectMusic(music)]]
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
    arrow:setScaleX(actor.npcBook.direction*math.abs(actor:getScaleX()))
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_HuaiTe(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    local dt = 0
    local index = nil
    for i=1,#targetActors do
        local time = targetActors[i].dt
        if  time > dt then
            dt = time
            index = i
        end
    end
    if  index == nil then return end
    local targetActor = targetActors[index].npc
    if targetActor == nil then return end

    local armatureFile1 = "battle/skill/huaite/bigSkillAttack_HuaiTe_ShanDian/bigSkillAttack_HuaiTe_ShanDian.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
    local armature1 = ccs.Armature:create("bigSkillAttack_HuaiTe_ShanDian")
    armature1:setVisible(false)
    local function func1()
        armature1:setVisible(true)
        armature1:getAnimation():play("Animation1")
    end
    --armature1:setLocalZOrder(targetActor:getLocalZOrder())
    targetActor:addChild(armature1)



    armature1:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(dt),cc.RemoveSelf:create()))

    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)



    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_LaZuoKeLao(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_Lina(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    local spriteScaleX = 745/320
    local spriteScaleY = 334/142
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setAnchorPoint(cc.p(0,0.5))
        sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX())*spriteScaleX)
        sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())*spriteScaleY) --if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
        sprite:setPosition(actor:getFirePosition().x,actor:getFirePosition().y+20)
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)
        local spriteWidth = 320*actor:getScaleX()/math.abs(actor:getScaleX())
        local moveToPos = targetActor:getHurtPosition()
        local abs = math.abs(targetActor:getHurtPosition().x - actor:getFirePosition().x)
        if abs<300 then
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
    Helper.PlayEffectMusic(music)]]
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_ZhuRiZhe(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        
        local armatureFile1 = "battle/skill/zhurizhe/bigSkillAttack_ZhuRiZhe_DunPai/bigSkillAttack_ZhuRiZhe_DunPai.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("bigSkillAttack_ZhuRiZhe_DunPai")
        armature1:setPositionX(60)
        armature1:setVisible(false)
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        --armature1:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(armature1)



        armature1:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(dt/4*3),cc.FadeOut:create(dt/4),cc.RemoveSelf:create()))
    end

    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_Jaina(param)
    local actor = param.fromNpc
    local sprite = cc.Sprite:create()
    local showPos = self:calPosition(actor.row+actor.npcBook.direction, actor.col)
    --local showPos = {x=actor:getPositionX(), y=actor:getPositionY()}
    sprite:setAnchorPoint(cc.p(0.5,0))
    sprite:setPosition(showPos)
    --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
    sprite:setVisible(false)
    sprite:setLocalZOrder(actor:getLocalZOrder())
    self:addChild(sprite)
    --126 80 100 63
    local spriteScaleX = 126/100
    local spriteScaleY = 80/63
    sprite:setScale(spriteScaleX,spriteScaleY)
    Helper.addSpriteFrames("battle/skill/Jaina/bigSkillAttack_Jaina_fazhen.plist","battle/skill/Jaina/bigSkillAttack_Jaina_fazhen.pvr.ccz")
    local frameArr = Helper.newFrames("effect-h%04d_Jaina.png",1,6)
    local animation = Helper.newAnimation(frameArr,0.25/6)
    
    Helper.addSpriteFrames("battle/skill/Jaina/ShuiYuanSu_Display_fazhen.plist","battle/skill/Jaina/ShuiYuanSu_Display_fazhen.pvr.ccz")
    local frameArr1 = Helper.newFrames("effect%04d_Jaina.png",1,7)
    local animation1 = Helper.newAnimation(frameArr1,0.25/7)
    
    local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
    local attackOver = cc.CallFunc:create(function()
        self:removeChild(sprite)
    end)
    sprite:runAction(cc.Sequence:create(attackStart,cc.Animate:create(animation),cc.Animate:create(animation1),cc.FadeOut:create(0.2),attackOver))
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
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
        --sprite1:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite1:setVisible(false)
        sprite1:setLocalZOrder(targetActor:getLocalZOrder()-1)
        targetActor:addChild(sprite1)

        local sprite2 = cc.Sprite:create()
        sprite2:setAnchorPoint(cc.p(0.5,0))
        --sprite2:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite2:setVisible(false)
        sprite2:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(sprite2)
        
        local sprite3 = cc.Sprite:create()
        sprite3:setAnchorPoint(cc.p(0.5,0))
        --sprite3:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite3:setVisible(false)
        sprite3:setLocalZOrder(targetActor:getLocalZOrder()+1)
        targetActor:addChild(sprite3)
        
        local sprite4 = cc.Sprite:create()
        sprite4:setAnchorPoint(cc.p(0.5,0))
        --sprite4:setPosition(cc.p(targetActor:getPositionX(),targetActor:getPositionY()))
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite4:setVisible(false)
        sprite4:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(sprite4)
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_DiXueLingZhu(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    local dt = 0
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        dt = targetActors[i].dt
        if targetActor ~= nil then 
            if  targetActor.npcBook.npcId < 20000 or targetActor.npcBook.npcId > 20002 then
                local actorMove = cc.MoveBy:create(dt/2,cc.p(0,150))
                targetActor:runAction(cc.Sequence:create(actorMove,actorMove:reverse()))
            end
            local sprite1 = cc.Sprite:create()
            sprite1:setAnchorPoint(cc.p(0.5,0))
            sprite1:setPosition(targetActor:getPosition())
            sprite1:setVisible(false)
            self:addChild(sprite1)

            local sprite2 = cc.Sprite:create()
            sprite2:setAnchorPoint(cc.p(0.5,0))
            --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
            sprite2:setVisible(false)
            sprite2:setPosition(targetActor:getPosition())
            self:addChild(sprite2)

            local sprite3 = cc.Sprite:create()
            sprite3:setAnchorPoint(cc.p(0.5,0))
            --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
            sprite3:setVisible(false)
            sprite3:setPosition(targetActor:getPosition())
            self:addChild(sprite3)

            Helper.addSpriteFrames("battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Di.plist","battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Di.pvr.ccz")
            local frameArr1 = Helper.newFrames("dimian_%d_DiXueLingZhu.png",0,8)
            local animation1 = Helper.newAnimation(frameArr1,dt/8)

            Helper.addSpriteFrames("battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Quan.plist","battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Quan.pvr.ccz")
            local frameArr2 = Helper.newFrames("diliequan_%d_DiXueLingZhu.png",0,6)
            local animation2 = Helper.newAnimation(frameArr2,dt/6)

            Helper.addSpriteFrames("battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Ci.plist","battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Ci.pvr.ccz")
            local frameArr3 = Helper.newFrames("dicidici_%d_DiXueLingZhu.png",0,5)
            local animation3 = Helper.newAnimation(frameArr3,dt/5)

            sprite1:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation1),cc.RemoveSelf:create()))
            sprite2:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation2),cc.RemoveSelf:create()))
            sprite3:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation3),cc.RemoveSelf:create()))
        end
    end
    local skillPos = self:getAttackPosByRangeRow(actor.row+actor.npcBook.direction,actor.col,12,actor.npcBook.direction)
    for i=1,#skillPos do
        local singlePos = skillPos[i]
        local offect = 10*math.pow(-1,i-1)
        local sprite1 = cc.Sprite:create("battle/skill/DiXueLingZhu/dicixiao_DiXueLingZhu.png")
        --sprite1:setAnchorPoint(cc.p(0.5,0))
        sprite1:setPosition(cc.p(singlePos.x,singlePos.y+offect))
        sprite1:setVisible(false)
        sprite1:setLocalZOrder(0)
        self:addChild(sprite1)
        --[[
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
        local frameArr1 = Helper.newFrames("dimian_%d_DiXueLingZhu.png",0,8)
        local animation1 = Helper.newAnimation(frameArr1,dt/8)
        
        Helper.addSpriteFrames("battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Quan.plist","battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Quan.pvr.ccz")
        local frameArr2 = Helper.newFrames("diliequan_%d_DiXueLingZhu.png",0,6)
        local animation2 = Helper.newAnimation(frameArr2,dt/6)
        
        Helper.addSpriteFrames("battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Ci.plist","battle/skill/DiXueLingZhu/bigSkillAttack_DiXueLingZhu_Ci.pvr.ccz")
        local frameArr3 = Helper.newFrames("dicidici_%d_DiXueLingZhu.png",0,5)
        local animation3 = Helper.newAnimation(frameArr3,dt/5)

        
        sprite2:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation2),cc.RemoveSelf:create()))
        sprite3:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation3),cc.RemoveSelf:create()))
        ]]
        sprite1:runAction(cc.Sequence:create(cc.Show:create(),cc.DelayTime:create(dt),cc.RemoveSelf:create()))
    end
    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_Kelthuzad(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs

    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setScaleX(actor:getScaleX()/math.abs(actor:getScaleX()))
        sprite:setScaleY(actor:getScaleY()/math.abs(actor:getScaleY())) if  actor.npcBook.boss == 1 then sprite:setScaleX(actor:getScaleX()) sprite:setScaleY(actor:getScaleY()) end
        sprite:setPosition(actor:getFirePosition())
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        --sprite:setRotation(self:calShootAngleHelper(actor,targetActor))
        sprite:setVisible(false)
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/Kelthuzad/bigSkillAttack_Kelthuzad.plist","battle/skill/Kelthuzad/bigSkillAttack_Kelthuzad.pvr.ccz")
        local frameArr = Helper.newFrames("dazhao_%d_Kelthuzad.png",0,4)
        local animation = Helper.newAnimation(frameArr,0.5/4)
        animation:setLoops(dt/0.5)
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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_YiLiDan(param)
    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_ShahelasiZhuMu(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    --330 420 180 229

    local spriteScaleX = 330/180
    local spriteScaleY = 420/229
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local spriteHeight = WIN_SIZE.height - targetActor:getPositionY()
        spriteScaleY = math.max(spriteScaleY,spriteHeight/229)
        local sprite = cc.Sprite:create()
        sprite:setPosition(cc.p(0,-40))
        sprite:setVisible(false)
        sprite:setAnchorPoint(cc.p(0.5,0))
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(sprite)
        sprite:setScale(spriteScaleX,spriteScaleY)
        Helper.addSpriteFrames("battle/skill/ShahelasiZhuMu/bigSkillAttack_ShahelasiZhuMu.plist","battle/skill/ShahelasiZhuMu/bigSkillAttack_ShahelasiZhuMu.pvr.ccz")
        local frameArr = Helper.newFrames("%d_ShahelasiZhuMu.png",1,9)
        local animation = Helper.newAnimation(frameArr,dt/9)


        sprite:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation),cc.FadeOut:create(0.2), cc.RemoveSelf:create()))
    end

    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_LaGeNaLuoSi(param)
        local actor = param.fromNpc
        local targetActors = param.toNpcs
        --520 381 300 220
        local spriteScaleX = 520/300
        local spriteScaleY = 381/200
        local spriteHeight = 200
        for i=1,#targetActors do
            local targetActor = targetActors[i].npc
            local dt = targetActors[i].dt
            if targetActor == nil then return end
            local sprite = cc.Sprite:create()
            sprite:setPosition(0,-0.45*spriteHeight)
            sprite:setVisible(false)
            sprite:setAnchorPoint(0.48,0)
            targetActor:addChild(sprite)
            sprite:setScale(spriteScaleX,spriteScaleY)

            Helper.addSpriteFrames("battle/skill/LaGeNaLuoSi/bigSkillAttack_LaGeNaLuoSi.plist","battle/skill/LaGeNaLuoSi/bigSkillAttack_LaGeNaLuoSi.pvr.ccz")
            local frameArr = Helper.newFrames("dashou_%d_LaGeNaLuoSi.png",1,10)
            local animation = Helper.newAnimation(frameArr,dt/10)

            local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
            local attackOver = cc.CallFunc:create(function()
                self:removeChild(sprite)
            end)
            sprite:runAction(cc.Sequence:create(attackStart,cc.Animate:create(animation), cc.RemoveSelf:create()))
        end

    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_AoNiKeXiYa(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    
    local picScaleX_long = 400/200
    local picScaleY_long = 428/214
    
    local picScaleX_di = 381/200
    local picScaleY_di = 157/82
    
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setPosition(cc.p(0,-85))
        sprite:setVisible(false)
        sprite:setAnchorPoint(cc.p(0.5,0))
        sprite:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(sprite)
        sprite:setScale(picScaleX_long,picScaleY_long)

        local sprite2 = cc.Sprite:create()       
        sprite2:setPosition(cc.p(0,-95))
        sprite2:setVisible(false)
        sprite2:setAnchorPoint(cc.p(0.5,0))
        sprite2:setLocalZOrder(targetActor:getLocalZOrder()-1)
        targetActor:addChild(sprite2)
        sprite2:setScale(picScaleX_di,picScaleY_di)
        
        Helper.addSpriteFrames("battle/skill/AoNiKeXiYa/bigSkillAttack_aonikexiya_long.plist","battle/skill/AoNiKeXiYa/bigSkillAttack_aonikexiya_long.pvr.ccz")
        local frameArr = Helper.newFrames("long_%d_AoNiKeXiYa.png",3,8)
        local animation = Helper.newAnimation(frameArr,dt/8)
       
        
        Helper.addSpriteFrames("battle/skill/AoNiKeXiYa/bigSkillAttack_aonikexiya_di.plist","battle/skill/AoNiKeXiYa/bigSkillAttack_aonikexiya_di.pvr.ccz")
        local frameArr2 = Helper.newFrames("mofazhen_%d_AoNiKeXiYa.png",0,8)
        local animation2 = Helper.newAnimation(frameArr2,dt/8)


        sprite:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation), cc.RemoveSelf:create()))
        sprite2:runAction(cc.Sequence:create(cc.Show:create(),cc.Animate:create(animation2), cc.RemoveSelf:create()))
    end

    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
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
        local sprite1 = cc.Sprite:create("battle/skill/KeLuoMaGuSi/bigSkillAttack_KeLuoMaGuSi_Di.png")
        sprite1:setAnchorPoint(cc.p(0.5,0))
        sprite1:setPosition(cc.p(singlePos.x,singlePos.y-20))
        sprite1:setVisible(false)
        sprite1:setLocalZOrder(actor:getLocalZOrder())
        self:addChild(sprite1)
        
        local sprite2 = cc.Sprite:create()
        sprite1:addChild(sprite2)
        sprite2:setAnchorPoint(cc.p(0,0))
        sprite2:setPosition(0,20)
        sprite2:setVisible(false)
        --sprite2:setPosition(0,0)
        
        Helper.addSpriteFrames("battle/skill/KeLuoMaGuSi/bigSkillAttack_KeLuoMaGuSi.plist","battle/skill/KeLuoMaGuSi/bigSkillAttack_KeLuoMaGuSi.pvr.ccz")
        local frameArr1 = Helper.newFrames("bingfengwangzuo_%d.png",1,11)
        local animation1 = Helper.newAnimation(frameArr1,dt/11)   
        
        sprite1:runAction(cc.Sequence:create(cc.DelayTime:create(delayTime),cc.Show:create(),cc.FadeOut:create(math.max((1.8-delayTime),0)),cc.RemoveSelf:create()))
        sprite2:runAction(cc.Sequence:create(cc.DelayTime:create(delayTime+0.1),cc.Show:create(),cc.Animate:create(animation1)))
    end
 
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_MaNuoLuoSi(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    if  targetActors == nil then return end
    local targetRow =  actor.row+actor.npcBook.direction*3
    local targetCol = actor.col
    local targetPos = self:calPosition(targetRow, targetCol)

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
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_AErSaSi(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()

        local armatureFile1 = "battle/skill/AErSaSi/bigSkillAttack_AErSaSi_HuangSe/bigSkillAttack_AErSaSi_HuangSe.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("bigSkillAttack_AErSaSi_HuangSe")
        armature1:setVisible(false)
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        --armature1:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(armature1)



        armature1:runAction(cc.Sequence:create(cc.DelayTime:create(0),cc.CallFunc:create(func1)))

        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)

    end

    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_AKeMengDe(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    
    local skillFirePosition = cc.p(108,91.6)
    skillFirePosition = cc.p(skillFirePosition.x*actor.npcBook.scaleFactor,skillFirePosition.y*actor.npcBook.scaleFactor)
    local xianWidth = 209
    local scaleY_xian = 114/59
    local scaleX_qiu = 288/188
    local scaleY_qiu = 286/187
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setAnchorPoint(cc.p(0,0.5))
        skillFirePosition.x = skillFirePosition.x*actor.npcBook.direction
        local firePosition =  cc.pAdd(cc.p(actor:getPositionX(),actor:getPositionY()), skillFirePosition)
        sprite:setPosition(firePosition)
        local betweenWidth = math.abs(targetActor:getPositionX()-firePosition.x)
        sprite:setScaleX(betweenWidth/xianWidth)
        sprite:setScaleY(scaleY_xian)
        sprite:setVisible(false)
        sprite:setLocalZOrder(actor:getLocalZOrder())
        local angle = PositionToAngle(firePosition,targetActor:getHurtPosition())
        sprite:setRotation(angle)
        self:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/AKeMengDe/bigSkillAttack_AKeMengDe_xian.plist","battle/skill/AKeMengDe/bigSkillAttack_AKeMengDe_xian.pvr.ccz")
        local frameArr = Helper.newFrames("shandian_%d_AKeMengDe.png",1,5)
        local animation = Helper.newAnimation(frameArr,dt/5)
        
        local sprite2 = cc.Sprite:create()
        sprite2:setPosition(targetActor.npcBook.hurtPosition)
        sprite2:setVisible(false)
        sprite2:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(sprite2)
        sprite2:setScaleX(scaleX_qiu)
        sprite2:setScaleY(scaleY_qiu)
        
        Helper.addSpriteFrames("battle/skill/AKeMengDe/bigSkillAttack_AKeMengDe_qiu.plist","battle/skill/AKeMengDe/bigSkillAttack_AKeMengDe_qiu.pvr.ccz")
        local frameArr2 = Helper.newFrames("shouji_%d_AKeMengDe.png",1,4)
        local animation2 = Helper.newAnimation(frameArr2,dt/4)
        
        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local attackStart2 = cc.CallFunc:create(function() sprite2:setVisible(true) end)
        local attackOver = cc.CallFunc:create(function() self:removeChild(sprite) end)
        sprite:runAction(cc.Sequence:create(attackStart,cc.Animate:create(animation), attackOver))
        sprite2:runAction(cc.Sequence:create(attackStart2,cc.Animate:create(animation2), cc.RemoveSelf:create()))
    end

    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
end

function FightLayer:bigSkill_WaSiQi(param)
end

function FightLayer:bigSkill_ZhaoHuanFaZhen_juexingSkill(param)
end

function FightLayer:bigSkill_XianXueNvWangLan(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    
    local skillFirePosition = cc.p(135,88)
    skillFirePosition = cc.p(skillFirePosition.x*actor.npcBook.scaleFactor,skillFirePosition.y*actor.npcBook.scaleFactor)
    local xianWidth = 258
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        sprite:setAnchorPoint(cc.p(1,0.5))
        skillFirePosition.x = skillFirePosition.x*actor.npcBook.direction
        local firePosition =  cc.pAdd(cc.p(actor:getPositionX(),actor:getPositionY()), skillFirePosition)
        sprite:setPosition(targetActor.npcBook.hurtPosition)
        local betweenWidth = math.abs(targetActor:getPositionX()-firePosition.x)
        sprite:setScaleX((betweenWidth/xianWidth)*actor.npcBook.direction)
        sprite:setVisible(false)
        sprite:setLocalZOrder(actor:getLocalZOrder())
        local angle = PositionToAngle(targetActor:getHurtPosition(),firePosition)
        sprite:setRotation(-angle*actor.npcBook.direction)
        targetActor:addChild(sprite)

        Helper.addSpriteFrames("battle/skill/XianXueNvWangLan/bigSkillAttack_XianXueNvWangLan_xian.plist","battle/skill/XianXueNvWangLan/bigSkillAttack_XianXueNvWangLan_xian.pvr.ccz")
        local frameArr = Helper.newFrames("xixue_%d_XianXueNvWangLan.png",15,4)
        local animation = Helper.newAnimation(frameArr,dt/4/3*2)

        
        local sprite2 = cc.Sprite:create()
        sprite2:setPosition(targetActor.npcBook.hurtPosition)
        sprite2:setVisible(false)
        sprite2:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(sprite2)
        
        Helper.addSpriteFrames("battle/skill/XianXueNvWangLan/bigSkillAttack_XianXueNvWangLan_qiu.plist","battle/skill/XianXueNvWangLan/bigSkillAttack_XianXueNvWangLan_qiu.pvr.ccz")
        local frameArr2 = Helper.newFrames("xuebao_%d_XianXueNvWangLan.png",17,4)
        local animation2 = Helper.newAnimation(frameArr2,dt/4)
        animation2:setLoops(dt/0.2*(3/4))
        
        local attackStart = cc.CallFunc:create(function() sprite:setVisible(true) end)
        local attackStart2 = cc.CallFunc:create(function() sprite2:setVisible(true) end)
        local attackOver = cc.CallFunc:create(function()
            self:removeChild(sprite)
        end)
        sprite:runAction(cc.Sequence:create(cc.DelayTime:create(dt/3),attackStart,cc.Animate:create(animation),cc.RemoveSelf:create()))
        sprite2:runAction(cc.Sequence:create(attackStart2,cc.Animate:create(animation2),cc.RemoveSelf:create()))
    end
end

function FightLayer:bigSkill_KaZhaKe(param)

end

function FightLayer:bigSkill_WoJin(param)
    local actor = param.fromNpc
    local targetActors = param.toNpcs
    --print("big_s")
    for i=1,#targetActors do
        local targetActor = targetActors[i].npc
        local dt = targetActors[i].dt
        if targetActor == nil then return end
        local sprite = cc.Sprite:create()
        
        local armatureFile1 = "image/battle/skill/WoJin/bigSkillAttack_WoJin_sheepGuang/bigSkillAttack_WoJin_sheepGuang.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
        local armature1 = ccs.Armature:create("bigSkillAttack_WoJin_sheepGuang")
        armature1:setVisible(false)
        local function func1()
            armature1:setVisible(true)
            armature1:getAnimation():play("Animation1")
        end
        --armature1:setLocalZOrder(targetActor:getLocalZOrder())
        targetActor:addChild(armature1)



        armature1:runAction(cc.Sequence:create(cc.DelayTime:create(0),cc.CallFunc:create(func1)))

        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)

    end

    local actor = param.fromNpc
    --[[local music = actor.npcBook.bigMusic
    Helper.PlayEffectMusic(music)]]
    

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
    if  skillId == 14 then
        armatureFile = "battle/ui/BingYu/BingYu.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        armature = ccs.Armature:create("BingYu")
        local music = "sound/juexingSkill/bingxue.mp3"
        Helper.PlayEffectMusic(music)  
    elseif skillId == 15 then
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
        local music = "sound/juexingSkill/huoyu.mp3"
        Helper.PlayEffectMusic(music)  
    else
        armatureFile = "battle/ui/leiyun/leiyun.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        armature = ccs.Armature:create("leiyun")
        local music = "sound/juexingSkill/thunder.mp3"
        Helper.PlayEffectMusic(music)  
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

    local size = cc.size(5,5)
    local shake = cc.Shaky3D:create(dt/2,size,3, false)
    local seq1 = cc.Sequence:create(shake,shake:reverse())
    local seq2 = cc.Sequence:create(shake,shake:reverse())
    nodeGrid:runAction(seq1)
    bg:getNodeGrid():runAction(seq2)
    
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

function FightLayer:getNodeGrid()
    return nodeGrid
end

function FightLayer:calShootAngleHelper(actor, targetActor)
    local radian = cc.pGetAngle(cc.pSub(targetActor:getHurtPosition(), actor:getFirePosition()), {x=actor:getDirection(),y=0})
    return math.radian2angle(radian)
end

function FightLayer:getAttackPosByRange(fromRow,fromCol,rangeRow,rangeCol,direction)
    local posTab = {}
    for row=fromRow+direction,fromRow+(rangeRow+1)*direction,direction do
        for col=fromCol-math.floor(rangeCol/2),fromCol-math.floor(rangeCol/2)+rangeCol-1 do
            if  row>0 and row <24 and col>0 and col <10 then
                local pos = self:calPosition(row, col)
                table.insert(posTab,pos)
            end
        end
    end
    return posTab
end

function FightLayer:getAttackPosByRangeRow(fromRow,fromCol,rangeRow,direction)
    local posTab = {}
    for row=fromRow+direction,fromRow+(rangeRow+1)*direction,direction do
            if  row>0 and row <24 and fromCol>0 and fromCol <10 then
                local pos = self:calPosition(row, fromCol)
                table.insert(posTab,pos)
            end
    end
    return posTab
end

function FightLayer:calPosition(row, col)
    local height_offset = 130
    local cells_max_row, cells_max_col = 24, 8
    local cell_width  = (960/(cells_max_row/2))
    local cell_height = (440/(cells_max_col))
    local x = (row-6-0.5) * cell_width
    x = x + (col-1) * 0.2 * cell_width
    local y = col*cell_height + height_offset
    return {x=x, y=y}
end

return FightLayer

