require("Cocos2d")
local speedup_factor = 1.2

local NPC = class("NPC", function()
    return cc.Node:create()
end)

-----------------------[[External API]]-----------------------------
function NPC:create(cmd)
    local NPC = NPC.new()
    NPC:initM(cmd)
    return NPC
end

function NPC:actCMD(cmd)
    if self.isDie == true and cmd.t ~= "move" then return end
    
    --self:stopAllActions()
    if cmd.t == "stand" then
        self:playAnimation("standby_1_1")
    elseif cmd.t == "walk" then
        self:playAnimation("move_1_1")
    elseif cmd.t == "move" then
        --self:resume()
        self:setPosition(self:getPositionX()+cmd.dx, self:getPositionY()+cmd.dy)
    elseif cmd.t == "atk_r" then
        self:playAnimation("attack_0_1")
    elseif cmd.t == "atk_s" then
        cc.SimpleAudioEngine:getInstance():playEffect(self.npcBook.normalMusic)
    elseif cmd.t == "atked" then
        --self:playAnimation("embattled_1_1")
    elseif cmd.t == "small_r" then
        self:playAnimation("attack_1_1")
    elseif cmd.t == "small_s" then
        cc.SimpleAudioEngine:getInstance():playEffect(self.npcBook.smallMusic)
    elseif cmd.t == "smalled" then
    elseif cmd.t == "big_r" then
        self:playAnimation("attack_2_1")
    elseif cmd.t == "big_s" then
        cc.SimpleAudioEngine:getInstance():playEffect(self.npcBook.bigMusic)
    elseif cmd.t == "biged" then
    elseif cmd.t == "global_s" then
        self:playAnimation("standby_1_1")
    elseif cmd.t == "global_in" then
        self:playAnimation("embattled_1_1")
    elseif cmd.t == "globalen" then
    elseif cmd.t == "tui_s" then
        self:playAnimation("embattled_1_1")
    elseif cmd.t == "yun_s" then
        self:playAnimation("embattled_1_1")
    elseif cmd.t == "interrupt_s" then
        self:playAnimation("embattled_1_1")
    elseif cmd.t == "fucked_s" then
        self:playAnimation("embattled_1_1")
    elseif cmd.t == "fucked_o" then
        print("stand")
        self:playAnimation("standby_1_1")
    elseif cmd.t == "die" then
        self.isDie = true
        self:hideAllEffect()
        self:playAnimation("die_1_1")
        self:runAction(cc.Sequence:create(cc.FadeOut:create(1), 
                                          cc.CallFunc:create(function() self:setVisible(false) end), 
                                          cc.DelayTime:create(1.0), 
                                          cc.CallFunc:create(function() self:removeFromParent() end)
                                         )
                      )
    end
end

function NPC:hideAllEffect()
    local function func()
        self.hpSlider:setVisible(false)
        if self.ScriptFuncId ~= nil then
            cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId)
        end
        self:removeBuffState("all")
    end
    
    local effectHpSlider = self.hpSlider:getChildByTag(101)
    local  dt = 0.5
    local to = cc.ProgressFromTo:create(dt,100-self.hp/self.npcBook.maxHp,100-0/self.npcBook.maxHp)
    effectHpSlider:runAction(cc.Sequence:create(cc.Show:create(),to,cc.Hide:create(),cc.CallFunc:create(func)))

end

function NPC:playEmbattled()
    self:playAnimation("embattled_1_1")
end

function NPC:getDirection()
    return self.npcBook.direction
end

function NPC:getFirePosition()
    local point = {}
    table.merge(point,self.npcBook.firePosition)
    point.x = point.x*self.npcBook.direction
    local scale = self.npcBook.scaleFactor
    return cc.pAdd(cc.p(self:getPositionX(),self:getPositionY()), cc.p(point.x*scale,point.y*scale))
end

function NPC:getHurtPosition()
    local point = {}
    table.merge(point,self.npcBook.hurtPosition)
    point.x = point.x*self.npcBook.direction
    return cc.pAdd(cc.p(self:getPositionX(),self:getPositionY()), point)
end

function NPC:getDirection()
    return self.npcBook.direction
end

--[[hp slider]]--
function NPC:changeHp(hp)
    if hp == 0 then return end
    local preHp = self.hp
    if preHp < 0 then
    	return
    end
    self.hp = self.hp - hp
    if self.hp>self.npcBook.maxHp then self.hp = self.npcBook.maxHp end
    self.hpSlider:getChildByTag(100):setVisible(true)
    local effectHpSlider = self.hpSlider:getChildByTag(101)
    if  hp> 0 then
        self.hpSlider:setPercentage(100-self.hp/self.npcBook.maxHp)       
        local  dt = 0.5
        if  hp/self.npcBook.maxHp < 0.05 then
            dt = 0.1
        elseif hp/self.npcBook.maxHp >=0.05 and hp/self.npcBook.maxHp <0.1 then
            dt = 0.2
        end
        local to = cc.ProgressFromTo:create(dt,100-preHp/self.npcBook.maxHp,100-self.hp/self.npcBook.maxHp)
        effectHpSlider:runAction(cc.Sequence:create(cc.Show:create(),to,cc.Hide:create()))
    else
        local function setPer()
            self.hpSlider:setPercentage(100-self.hp/self.npcBook.maxHp)      
        end
        local to = cc.ProgressFromTo:create(0.1,100-preHp/self.npcBook.maxHp,100-self.hp/self.npcBook.maxHp)
        effectHpSlider:runAction(cc.Sequence:create(cc.Show:create(),to,cc.CallFunc:create(setPer),cc.Hide:create()))
    end
    if self.npcBook.direction==1 then
        actorAgent:setUIHp(self.npcBook.soulId, math.ceil(100*self.hp/self.npcBook.maxHp))
    end
end

--[[shield slider]]--
function NPC:changeShield(hp)
    if hp <= 0 then return end
    local preHp = self.shield
    if preHp < 0 then
        return
    end
    self.shield = self.shield - hp
    local maxshield = self.maxShield
    local toSubHp = 0
    if self.shield <= 0 then toSubHp = -self.shield self:changeHp(toSubHp) self.maxShield = nil self.shieldSlider:setVisible(false) end
    self.shieldSlider:getChildByTag(100):setVisible(true)
    self.shieldSlider:setPercentage(100-self.shield/maxshield)
end

function NPC:setShield(shieldValue)
    if shieldValue == 0 then return end
    self.maxShield = shieldValue
    self.shield = shieldValue
    self.shieldSlider:setVisible(true)
    self.shieldSlider:getChildByTag(100):setVisible(true)
    self.shieldSlider:setPercentage(100-self.shield/self.maxShield)
end

function NPC:hideShield()
    self.maxShield = nil
    self.shieldSlider:setVisible(false)
end

--[[ 
damageVal (negative number indicates increase blood)
]]--
function NPC:changeHpEffect(damageVal,effectType)
    if damageVal == 0 then return end
    local damageHpLabel = ccui.TextAtlas:create()

    damageHpLabel:setProperty("0123456789:", "battle/ui/shuzi_hong.png", 22, 32, "0")
    if  damageVal <= 0 then
        damageHpLabel:setProperty("0123456789:", "battle/ui/shuzi_lv.png", 22, 32, "0")
    end
    self:addChild(damageHpLabel)
    
    local factor = 0.7*1.2/self.npcBook.scaleFactor
    if  self.npcBook.boss == 1 then
        factor = 1.0
    end
    
    if  effectType == "jianfang" then
        local sprite = cc.Sprite:create("battle/ui/fangyu.png")
        sprite:setAnchorPoint(cc.p(1,0))
        sprite:setPosition(0,0)
        damageHpLabel:addChild(sprite)
        factor = 0.5
    end
    
    local scale = 1*factor
    if  self.npcBook.direction == -1 then 
        damageHpLabel:setScaleX(-0)
        scale = -1*factor
    else  
        damageHpLabel:setScaleX(0)
    end
    damageHpLabel:setScaleY(0)


    damageHpLabel:setPosition(cc.p(self.npcBook.hpPosition.x,self.npcBook.hpPosition.y-10))
    damageHpLabel:setOpacity(255)
    damageHpLabel:setString(":"..math.abs(math.floor(damageVal)))
    local spawn = cc.Spawn:create(cc.MoveBy:create(0.4,cc.p(0, 50.0)) ,cc.ScaleBy:create(0.4 ,1.2 ,1.2) ,cc.FadeOut:create(0.4))
    local spawn2 = cc.Spawn:create(cc.MoveBy:create(0.1,cc.p(0, 30.0)) ,cc.ScaleTo:create(0.1 ,1.0*scale ,1.0*factor) ,cc.FadeIn:create(0.1))
    local seq = cc.Sequence:create(cc.Show:create(),cc.EaseOut:create(spawn2,0.1),cc.DelayTime:create(0.1),spawn,cc.Hide:create())
    damageHpLabel:runAction(seq)
end

function NPC:getHpPercent()
    local percent = self.hpSlider:getPercentage()
    return percent
end

--[[--
hit interface 
@param integer subH
@param integer effectType (normal miss gengdang baoji)
@param string attackType
]]
function NPC:normalAttackHurt(subHp,effectType,attackType)
    if subHp == nil then return end
    if  effectType == "jianfang" then
        self:changeHpEffect(subHp,effectType)
        return
    end
    self:runAttackEft(effectType)
    self:changeHpEffect(subHp)
    
    if  subHp > 0 then
          if  self.maxShield then
              self:changeShield(subHp)
          else
            self:changeHp(subHp)
        end
    else
        self:changeHp(subHp)
    end
    
    self.hpSliderFadeOutTimer = 5
    
    --if  isEmbattled == nil then isEmbattled = false end
    local animation = nil
    if  attackType == nil then attackType = "normal" end
    
    self.anmiSprite = cc.Sprite:create()
    self.anmiSprite:setPosition(self.npcBook.hurtPosition)
    --anmiSprite:setGlobalZOrder(10000)
    --anmiSprite:setScale(self.npcBook.scaleFactor)
    self:addChild(self.anmiSprite)
    
    if  attackType == "normal" then
        Helper.addSpriteFrames("battle/hurt/normal_hurt.plist","battle/hurt/normal_hurt.png")
        local frameArr = Helper.newFrames("putong%d.png",0,5)
        animation = Helper.newAnimation(frameArr,0.2/5)
    elseif attackType == "ice" then
        Helper.addSpriteFrames("battle/hurt/ice_hurt.plist","battle/hurt/ice_hurt.png")
        local frameArr = Helper.newFrames("ice%d.png",0,7)
        animation = Helper.newAnimation(frameArr,0.2/7)
    elseif  attackType == "fire" then
        Helper.addSpriteFrames("battle/hurt/fire_hurt.plist","battle/hurt/fire_hurt.png")
        local frameArr = Helper.newFrames("fire%d.png",0,7)
        animation = Helper.newAnimation(frameArr,0.2/7)
    elseif attackType == "thunder" then
        Helper.addSpriteFrames("battle/hurt/thunder_hurt.plist","battle/hurt/thunder_hurt.png")
        local frameArr = Helper.newFrames("leidian%d.png",0,7)
        animation = Helper.newAnimation(frameArr,0.2/7)
    elseif attackType == "bingbao" then
        --242 237 150 147
        Helper.addSpriteFrames("battle/skill/XiaoHei/bingbao.plist","battle/skill/XiaoHei/bingbao.pvr.ccz")
        local frameArr = Helper.newFrames("bingbao_%d_Jaina.png",0,5)
        animation = Helper.newAnimation(frameArr,0.2/5)
        --local scale = 0.5
        local spriteScaleX = 242/150
        local spriteScaleY = 237/147
        self.anmiSprite:setAnchorPoint(cc.p(0.5,0))
        self.anmiSprite:setScale(spriteScaleX,spriteScaleY)
        self.anmiSprite:setPosition(cc.p(0,-20))
    elseif attackType == "bingbao_jaina" then
        Helper.addSpriteFrames("battle/skill/XiaoHei/bingbao.plist","battle/skill/XiaoHei/bingbao.pvr.ccz")
        local frameArr = Helper.newFrames("bingbao_%d_Jaina.png",0,5)
        animation = Helper.newAnimation(frameArr,0.2/5)
        local spriteScaleX = 242/150
        local spriteScaleY = 237/147
        self.anmiSprite:setAnchorPoint(cc.p(0.5,0))
        self.anmiSprite:setScale(spriteScaleX,spriteScaleY)
        self.anmiSprite:setPosition(cc.p(0,-20))
    elseif attackType == "bingbao_kelthuzad" then
        --209 187 150 134
        local spriteScaleX = 209/150
        local spriteScaleY = 187/134
        self.anmiSprite:setScale(spriteScaleX,spriteScaleY)
        Helper.addSpriteFrames("battle/skill/Kelthuzad/bigSkillAttack_Kelthuzad_hurt.plist","battle/skill/Kelthuzad/bigSkillAttack_Kelthuzad_hurt.png")
        local frameArr = Helper.newFrames("bingbao_%d_Kelthuzad.png",0,8)
        animation = Helper.newAnimation(frameArr,1.0/8)
        
    elseif attackType == "chishao" then
        print("chishao")
        Helper.addSpriteFrames("battle/hurt/chishao_hurt.plist","battle/hurt/chishao_hurt.pvr.ccz")
        local frameArr = Helper.newFrames("chishao_0_%d.png",0,7)
        animation = Helper.newAnimation(frameArr,1.0/7)
        animation:setLoops(2)
    elseif attackType == "yan_wasiqi" then
        --308 291 180 170
        local spriteScaleX = 308/180
        local spriteScaleY = 291/170
        self.anmiSprite:setScale(spriteScaleX,spriteScaleY)
        Helper.addSpriteFrames("image/battle/skill/WaSiQi/smallSkillAttack_WaSiQi_yan.plist","image/battle/skill/WaSiQi/smallSkillAttack_WaSiQi_yan.pvr.ccz")
        local frameArr = Helper.newFrames("shoujiyan_%d_WaSiQi.png",3,5)
        animation = Helper.newAnimation(frameArr,0.2/5)
    elseif attackType == "du_wasiqi" then
        local spriteScaleX = 125/85
        local spriteScaleY = 153/104
        self.anmiSprite:setScale(spriteScaleX,spriteScaleY)
        Helper.addSpriteFrames("image/battle/skill/WaSiQi/bigSkillAttack_WaSiQi_du.plist","image/battle/skill/WaSiQi/bigSkillAttack_WaSiQi_du.pvr.ccz")
        local frameArr = Helper.newFrames("%d_WaSiQi.png",1,4)
        animation = Helper.newAnimation(frameArr,0.5/4)
        --animation:setLoops(4)
    end
    
    if animation == nil then print("[NPC]no animation for "..attackType) end
    local action = cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create())
    self.anmiSprite:runAction(action)
    
    --[[
    if  isEmbattled then
        self:playAnimation("embattled_1_1")
    end
    ]]
    --[[
    if hurtPercent >10 then
        local preAnimationName = self.preAnimationName
        self:playAnimation("embattled_1_1")
        if preAnimationName == "walk" then
            self:runAction(cc.Sequence:create(cc.DelayTime:create(0.5), cc.CallFunc:create(function() self:playAnimation(preAnimationName) end)))
        end
    end
    ]]
end

function NPC:setBuffState(type)
    local animation = nil
    local sprite = cc.Sprite:create()
    sprite:setName(type)
    if self.bufferSprite == nil then return end
    self.bufferSprite:addChild(sprite)
    if  type == "ice" or type == "silence" then
        if type == "ice" then
            sprite:setTexture("battle/buff/bing_buff.png")
            sprite:setScale(0.6)
            sprite:setAnchorPoint(cc.p(0.5,0))
            sprite:setPosition(cc.p(0,-20))
            self.bufferSprite:setLocalZOrder(self.armature:getLocalZOrder()+1)
        elseif type == "silence" then
            sprite:setTexture("battle/buff/silence_buff.png")
            sprite:setAnchorPoint(cc.p(0.5,0))
            sprite:setScaleX(0.6)
            sprite:setScaleY(0.6)
            sprite:setPosition(cc.p(self.npcBook.hpPosition.x,self.npcBook.hpPosition.y+10))
        end
    else
        if  type == "yun" then
            Helper.addSpriteFrames("battle/buff/xuanyun.plist","battle/buff/xuanyun.png")
            local frameArr = Helper.newFrames("xuanyun_%d.png",0,19)
            animation = Helper.newAnimation(frameArr,1/20)
        end
        sprite:setPosition(cc.p(self.npcBook.hpPosition.x,self.npcBook.hpPosition.y+20))

        local action =  cc.RepeatForever:create(cc.Animate:create(animation))
        sprite:runAction(action)
    end
    Helper.setTimeout(self, 15, function() self:playAnimation("standby_1_1") end)
end

function NPC:playSkillTriggerBuff(dt)
    local triggerBuffDt = 0.6
    local speedScale = dt/triggerBuffDt
    local skillTrigger =self:getChildByName("skillTrigger")
    skillTrigger:getAnimation():setSpeedScale(speedScale)
    local function func1()
        skillTrigger:setVisible(true)
        skillTrigger:getAnimation():play("Animation1")
    end
    local function func2()
        skillTrigger:setVisible(false)
    end
    skillTrigger:runAction(cc.Sequence:create(cc.CallFunc:create(func1),cc.DelayTime:create(dt),cc.CallFunc:create(func2)))
end

function NPC:removeBuffState(type)
    if  self.bufferSprite == nil then return end
    local sprite = self.bufferSprite:getChildByName(type)
    if  type == "all" then
        if self.bufferSprite ~= nil then
            self.bufferSprite:removeFromParent()
            self.bufferSprite = nil
        end
    else
        if  sprite == nil then
            self.bufferSprite:removeAllChildren()
        else
            sprite:removeFromParent()
        end
    end
    
    
    --[[
    if  self.bufferSprite == nil then return end
   
    end]]
end

function NPC:runAttackEft(eft)
    local eftSprite = cc.Sprite:create()
    self:addChild(eftSprite)
    if  eft == 1 then
        return
    elseif eft==2 then
        eftSprite:setTexture("battle/ui/shanbi.png")
    elseif eft==3 then
        eftSprite:setTexture("battle/ui/gedang.png")
    elseif  eft == 4 then
        eftSprite:setTexture("battle/ui/baoji.png")
    end
    --self.eftSprite:setScaleY(0.5)
    local scale =1
    if  self.npcBook.direction == -1 then 
        eftSprite:setScaleX(-1.0)
        scale = -1
    else  
       eftSprite:setScaleX(1.0)
    end
    eftSprite:setPosition(cc.p(self.npcBook.hpPosition.x,self.npcBook.hpPosition.y+60))
    --local spawn = cc.Spawn:create(cc.MoveBy:create(1.0,cc.p(0, 150.0)) ,cc.ScaleTo:create(1.0 ,1.2*scale ,1.2) ,cc.FadeOut:create(1.0))
    local seq = cc.Sequence:create(cc.Show:create(),cc.DelayTime:create(0.4),cc.FadeOut:create(0.4),cc.RemoveSelf:create())
    eftSprite:runAction(seq)
end

function NPC:pause()
    self.armature:getAnimation():pause()
    
    --print("~~~~~start~~~~~~")
    local boneDic = self.armature:getBoneDic()
    for k, v in pairs(boneDic) do
        --print("render node")
        if v and v:getDisplayRenderNode() then
            
            local node = v:getDisplayRenderNode()
            --[[
            local shader = cc.GLProgram:create("shader/gray1.vsh", "shader/gray1.fsh")
            --shader:retain()
            shader:bindAttribLocation(cc.ATTRIBUTE_NAME_POSITION, cc.VERTEX_ATTRIB_POSITION)
            shader:bindAttribLocation(cc.ATTRIBUTE_NAME_COLOR, cc.VERTEX_ATTRIB_COLOR)
            shader:bindAttribLocation(cc.ATTRIBUTE_NAME_TEX_COORD, cc.VERTEX_ATTRIB_TEX_COORDS)
            shader:link()
            shader:updateUniforms()
            ]]
            local shader = cc.GLProgramCache:getInstance():getGLProgram("bigskill")
            if shader ~= node:getGLProgram() then
                node.preShader = node:getGLProgram()
                node:setGLProgram(shader)
            end
        end
    end
    --print("~~~~~end~~~~~~\n")    
end

function NPC:resume()
    self.armature:getAnimation():resume()
    local boneDic = self.armature:getBoneDic()
    
    --print("[start preColor]")
    for k, v in pairs(boneDic) do
        if v and v:getDisplayRenderNode() then
            local node = v:getDisplayRenderNode()
            if node.preShader and node.preShader~= node:getGLProgram() then
                node:setGLProgram(node.preShader)
                --print("~~~~~~preColor~~~~~~")
            end
        end
    end
    --print("[end preColor]\n")
end

------------------------[[Internal function]]------------------------
function NPC:ctor()
end

function NPC:onEnter()
end

function NPC:onExit()
end

function NPC:initM(cmd)
    assert(cmd.t=="createNpc","")
    self.key = cmd.key
    self.npcBook = cmd.npcBook
    --tag
    self:setTag(self.key)
    --position
    self:setPosition(cmd.pos)
    --scale size
    local function isTower()
        return self.npcBook.npcId >= 20000 and self.npcBook.npcId <= 20009
    end
    local bossScale = 2.04/self.npcBook.scaleFactor
    if  self.npcBook.boss == 1 then
        self:setScaleX(self.npcBook.scaleFactor*self.npcBook.direction*bossScale)
        self:setScaleY(self.npcBook.scaleFactor*bossScale)
    else
        self:setScaleX(self.npcBook.scaleFactor*self.npcBook.direction)
        self:setScaleY(self.npcBook.scaleFactor)
    end

    --set z order
    self.zOrder = cmd.zOrder
    self:setLocalZOrder(self.zOrder)
    --armature
    local armatureFile = "image/battle/armature/"..self.npcBook.name.."/"..self.npcBook.name..".csb"
    print(armatureFile)
    if self.npcBook.name == "PuTongTanKe" then
        printJSON(self.npcBook)
    end
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    self.armature = ccs.Armature:create(self.npcBook.name)
    self.armature:getAnimation():playWithIndex(0)
    self:addChild(self.armature)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
    self.armatureFile = armatureFile
   
    --hp
    self.hp = self.npcBook.currHp
    local hpSprite = cc.Sprite:create("image/battle/ui/xuetiao_01.png")
    local hpEffectSprite = cc.Sprite:create("image/battle/ui/xuetiao_04.png")
    local hpBgSprite = nil 
    if  self.npcBook.direction == 1 then 
        hpBgSprite = cc.Sprite:create("image/battle/ui/xuetiao_02.png")
    else
        hpBgSprite = cc.Sprite:create("image/battle/ui/xuetiao_03.png")
    end
    
    --[[
    loadProgress = ProgressTimer::create(Sprite::create("loading_progress.png"));//创建一个进程条
    loadProgress->setBarChangeRate(Point(1,0));//设置进程条的变化速率
    loadProgress->setType(ProgressTimer::Type::BAR);//设置进程条的类型
    loadProgress->setMidpoint(Point(0,1));//设置进度的运动方向
    loadProgress->setPosition(Point(visibleSize.width /2,visibleSize.height * .25));
    loadProgress->setPercentage(0.0f);//设置初始值为0
    this->addChild(loadProgress,1);
    ]]
    self.hpSlider = cc.ProgressTimer:create(hpBgSprite)--  create(hpSprite,hpBgSprite,cc.Sprite:create())
    self.hpSlider:setBarChangeRate(cc.p(100,0))
    self.hpSlider:setType(cc.PROGRESS_TIMER_TYPE_BAR)
    self.hpSlider:setReverseDirection(true)
    self.hpSlider:setMidpoint(cc.p(1,0))
    self.hpSlider:setPosition(self.npcBook.hpPosition)
    
    --shield
    self.maxShield = nil
    local shieldSprite = cc.Sprite:create("image/battle/ui/xuetiao_01.png")
    local shieldBgSprite = cc.Sprite:create("image/battle/ui/xuetiao_04.png")
    self.shieldSlider = cc.ProgressTimer:create(shieldBgSprite)--  create(hpSprite,hpBgSprite,cc.Sprite:create())
    self.shieldSlider:setBarChangeRate(cc.p(100,0))
    self.shieldSlider:setType(cc.PROGRESS_TIMER_TYPE_BAR)
    self.shieldSlider:setReverseDirection(true)
    self.shieldSlider:setMidpoint(cc.p(1,0))
    self.shieldSlider:setPosition(cc.p(self.npcBook.hpPosition.x,self.npcBook.hpPosition.y-8))  
    
    local sliderScale = 0.7*1.2/self.npcBook.scaleFactor
    self.hpSlider:setScale(sliderScale)
    self.shieldSlider:setScale(sliderScale)

    if  self.npcBook.boss == 1  then
        self.hpSlider:setScale(1.0)
        self.shieldSlider:setScale(1.0)
    end
    --self.hpSlider:setPercentage(0)
    self:addChild(self.hpSlider)
    self.hpSlider:addChild(hpSprite,-2)
    
    self:addChild(self.shieldSlider)
    self.shieldSlider:addChild(shieldSprite,-2)
    self.shieldSlider:setVisible(false)
    
    hpSprite:setAnchorPoint(cc.p(0.5,0.5))
    hpSprite:setPosition(cc.p(self.hpSlider:getContentSize().width/2,self.hpSlider:getContentSize().height/2)) 
    hpSprite:setTag(100)
    hpSprite:setVisible(false)
    
    shieldSprite:setAnchorPoint(cc.p(0.5,0.5))
    shieldSprite:setPosition(cc.p(self.hpSlider:getContentSize().width/2,self.hpSlider:getContentSize().height/2)) 
    shieldSprite:setTag(100)
    shieldSprite:setVisible(false)
    
    local hpEffectSlider = cc.ProgressTimer:create(hpEffectSprite)--  create(hpSprite,hpBgSprite,cc.Sprite:create())
    hpEffectSlider:setBarChangeRate(cc.p(100,0))
    hpEffectSlider:setType(cc.PROGRESS_TIMER_TYPE_BAR)
    hpEffectSlider:setReverseDirection(true)
    hpEffectSlider:setMidpoint(cc.p(1,0))
    hpEffectSlider:setAnchorPoint(cc.p(0.5,0.5))
    hpEffectSlider:setPosition(cc.p(self.hpSlider:getContentSize().width/2,self.hpSlider:getContentSize().height/2)) 
    hpEffectSlider:setTag(101)
    hpEffectSlider:setVisible(false)
    self.hpSlider:addChild(hpEffectSlider,-1)
    self.isDie = false
    
    --buffer
    self.bufferSprite = cc.Sprite:create()
    self:addChild(self.bufferSprite)

    --effect (normal miss gengdang baoji)
    self.eftSprite = cc.Sprite:create()
    self:addChild(self.eftSprite)
    
    --set animation speed scale
    self.armature:getAnimation():setSpeedScale(speedup_factor)
    
    --skillTrigger_buff
    local armatureFile_skillTrigger = "battle/buff/skillTrigger_buff/skillTrigger_buff.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile_skillTrigger)
    local skillTrigger = ccs.Armature:create("skillTrigger_buff")
    skillTrigger:setVisible(false)
    skillTrigger:setName("skillTrigger")
    self:addChild(skillTrigger,-100)
   
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile_skillTrigger)
    
    --add hpSliderScheduler
    self.hpSliderFadeOutTimer = 0
    local function UpdateHp(dt)
        if self.hpSliderFadeOutTimer == nil then self.hpSliderFadeOutTimer = 0 end
        self.hpSliderFadeOutTimer = self.hpSliderFadeOutTimer -1
        local timer = self.hpSliderFadeOutTimer
        if  self.hpSlider then
            if  timer <= 0 and self.maxShield == nil then
                self.hpSlider:setVisible(false)
                self.hpSliderFadeOutTimer = 0
            else
                self.hpSlider:setVisible(true)
            end
        end
    end
    self.ScriptFuncId = cc.Director:getInstance():getScheduler():scheduleScriptFunc(UpdateHp,1,false)
end

function NPC:playAnimation(animationName)
    if self.armature:getAnimation():getAnimationData():getMovement(animationName) then
        self.armature:getAnimation():play(animationName, -1, -1)
    else
        print("[NPC]no "..animationName.." for "..self.npcBook.debug_name)
    end
end

return NPC

