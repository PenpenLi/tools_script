require("Cocos2d")

local NpcActor = class("NpcActor", function()
    return cc.Node:create()
end)

function NpcActor:create(param)
    local npc = NpcActor.new()
    npc:initM(param)
    return npc
end

function NpcActor:ctor()
end

function NpcActor:onEnter()
end

function NpcActor:onExit()
end

--[[
npcBook: {
    position={x,y},
    scaleFactor = 1.0,
    armatureName = "lina",
}

direction: 1 looking right, -1 looking left
]]--
function NpcActor:initM(param)
    self.npcBook = param.npcBook    
    --position
    self:setPosition(self.npcBook.position)
    
    --scale size
    self:setScaleX(self.npcBook.scaleFactor*self.npcBook.direction)
    self:setScaleY(self.npcBook.scaleFactor)
    
    --set z order
    self:setLocalZOrder(self.npcBook.zOrder)
    
    --armature
    local armatureFile = "image/battle/armature/"..self.npcBook.name.."/"..self.npcBook.name..".csb"
    print(armatureFile)
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    self.armature = ccs.Armature:create(self.npcBook.name)
    self.armature:getAnimation():playWithIndex(0)
    self:addChild(self.armature)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
    
    --current FSM state
    self.fsmState = "stand"
    --[[
    self.sp = sp.SkeletonAnimation:create("image/battle/armature/spine/spineboy.json", "image/battle/armature/spine/spineboy.atlas", 0.8)
    self:addChild(self.sp)
    ]]--
    --[[
    local tmpNpcName = self.npcBook.name
    local function loaded(param)
        self.armature = ccs.Armature:create(tmpNpcName)
        self.armature:getAnimation():playWithIndex(0)
        self:addChild(self.armature)
        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
        self.hpSlider:setPositionY(self.armature:getContentSize().height) 
    end
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfoAsync(armatureFile, loaded)
    ]]--
    --hp
    self.hp = self.npcBook.currHp
    local hpSprite = cc.Sprite:create("image/battle/ui/xuetiao_01.png")
    local hpBgSprite = nil 
    if  self.npcBook.direction == 1 then 
        hpBgSprite = cc.Sprite:create("image/battle/ui/xuetiao_02.png")
    else
        hpBgSprite = cc.Sprite:create("image/battle/ui/xuetiao_03.png")
    end
    
    self.hpSlider = cc.ControlSlider:create(hpSprite,hpBgSprite,cc.Sprite:create())
    self.hpSlider:setMinimumValue(0.0)
    self.hpSlider:setMaximumValue(1.0)
    self.hpSlider:setEnabled(false)
    self.hpSlider:setPositionY(self.armature:getContentSize().height)  
    self.hpSlider:setValue(self.hp/self.npcBook.maxHp)
    self:addChild(self.hpSlider)
    
    self.isDie = false
end

function NpcActor:getFirePosition()
    local point = self.npcBook.firePosition
    point.x = self.npcBook.firePosition.x*self.npcBook.direction
    return cc.pAdd(cc.p(self:getPositionX(),self:getPositionY()), point)
end

function NpcActor:getHurtPosition()
    local point = self.npcBook.hurtPosition
    point.x = self.npcBook.firePosition.x*self.npcBook.direction
    return cc.pAdd(cc.p(self:getPositionX(),self:getPositionY()), point)
end

function NpcActor:getDirection()
    return self.npcBook.direction
end

function NpcActor:getNormalAttackReadyTime()
    return self.npcBook.normalAttackReadyTime
end

function NpcActor:getSmallSkillAttackReadyTime()
    return self.npcBook.smallSkillReadyTime
end

function NpcActor:getBigSkillAttackReadyTime()
    return self.npcBook.bigSkillReadyTime
end

--[[播放动画]]--
function NpcActor:playAnimation(animationName)
    self.preAnimationName = animationName
    if self.armature then
        self.armature:getAnimation():play(animationName, -1, -1)
    end
end

function NpcActor:actCMD(CMD)
    self.fsmState = CMD.cmdId
    
    if self.isDie == true then return end
    
    if CMD.cmdId == "stand" then
        self:playAnimation("standby_1_1")
    elseif CMD.cmdId == "walk" then
        self:stopAllActions()
        self:playAnimation("move_1_1")
        if CMD.duration and CMD.duration > 0 and CMD.targetPosition then
            local moveTo = cc.MoveTo:create(CMD.duration, CMD.targetPosition)
            self:runAction(moveTo)
        end
        
    elseif CMD.cmdId == "die" then
        self.hpSlider:setVisible(false)
        self:playAnimation("die_1_1", -1, false)
        local delay = cc.DelayTime:create(self.npcBook.dieDuration)
        local callback = cc.CallFunc:create(function () self:setVisible(false) end)
        self:runAction(cc.Sequence:create(delay, callback))
        self.isDie = true
    elseif CMD.cmdId == "dizzy" then
        self:stopAllActions()
        self:playAnimation("standby_1_1")
        self:normalAttackHurt(CMD.subHpVal, "normal")
    elseif CMD.cmdId == "fallback" then
        local moveTo = cc.MoveTo:create(CMD.duration, CMD.targetPosition)
        self:runAction(cc.Sequence:create(moveTo,cc.CallFunc:create(function()
            self:normalAttackHurt(CMD.subHpVal, "normal")
        end)))
    elseif CMD.cmdId == "normalAttack" then
        self:playAnimation("attack_0_1")
    elseif CMD.cmdId == "smallSkillAttack" then
        self:playAnimation("attack_1_1")
    elseif CMD.cmdId == "bigSkillAttack" then
        self:playAnimation("attack_2_1")
    elseif CMD.cmdId == "embattled" then
        self:playAnimation("embattled_1_1")
    elseif CMD.cmdId == "globalSkillAttack" then
        print("[NpcActor]globalSkillAttack")
        --self:playAnimation("standby_1_1")
    end
end

--[[--
    被击接口 
    @param integer subHp失血量 
    @param string type 受攻击的类型
]]
function NpcActor:normalAttackHurt(subHp,type)
    self:changeHpEffect(subHp)
    self:changeHp(subHp)
    
    local hurtPercent = subHp/self.npcBook.maxHp*100
    
    local animation = nil
    if  type == nil then type = "normal" end

    if  type == "normal" then
        Helper.addSpriteFrames("battle/hurt/normal_hurt.plist","battle/hurt/normal_hurt.png")
        local frameArr = Helper.newFrames("putong%d.png",0,4)
        animation = Helper.newAnimation(frameArr,0.2/5)
    elseif type == "ice" then
        Helper.addSpriteFrames("battle/hurt/ice_hurt.plist","battle/hurt/ice_hurt.png")
        local frameArr = Helper.newFrames("ice%d.png",0,6)
        animation = Helper.newAnimation(frameArr,0.2/7)
    elseif  type == "fire" then
        Helper.addSpriteFrames("battle/hurt/fire_hurt.plist","battle/hurt/fire_hurt.png")
        local frameArr = Helper.newFrames("fire%d.png",0,6)
        animation = Helper.newAnimation(frameArr,0.2/7)
    elseif type == "thunder" then
        Helper.addSpriteFrames("battle/hurt/thunder_hurt.plist","battle/hurt/thunder_hurt.png")
        local frameArr = Helper.newFrames("leidian%d.png",0,6)
        animation = Helper.newAnimation(frameArr,0.2/7)
    end

    local anmiSprite = cc.Sprite:create()
    anmiSprite:setPosition(self.npcBook.hurtPosition)
    anmiSprite:setGlobalZOrder(10000)
    --anmiSprite:setScale(self.npcBook.scaleFactor)
    self:addChild(anmiSprite)
    local action = cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create())
    anmiSprite:runAction(action)
    
    if hurtPercent >10 then
        local preAnimationName = self.preAnimationName
        self:playAnimation("embattled_1_1")
        if preAnimationName == "walk" then
            self:runAction(cc.Sequence:create(cc.DelayTime:create(0.5), cc.CallFunc:create(function() self:playAnimation(preAnimationName) end)))
        end
    end
end

function NpcActor:isBigSkillAttackOk()
    return self.fsmState == "bigSkillAttack"
end

function NpcActor:isSmallSkillAttackOk()
    return self.fsmState == "smallSkillAttack"
end

--[[更新血条]]--
function NpcActor:changeHp(hp)
    self.hp = self.hp - hp
    self.hpSlider:setValue(self.npcBook.currHp/self.npcBook.maxHp)
    --print("hp:"..self.hp.."/"..self.npcBook.maxHp)
end

--[[
扣血效果 
damageVal 为负数表示加血
]]--
function NpcActor:changeHpEffect(damageVal)
    local damageHpLabel = ccui.TextAtlas:create()
    
    damageHpLabel:setProperty("0123456789:", "battle/ui/shuzi_hong.png", 23, 32, "0")
    if  damageVal < 0 then
        damageHpLabel:setProperty("0123456789:", "battle/ui/shuzi_lv.png", 23, 32, "0")
    end
    self.hpSlider:addChild(damageHpLabel)
    
    damageHpLabel:setScaleY(1.0)
    if  self.npcBook.direction == -1 then 
        damageHpLabel:setScaleX(-1.0)
    else  
        damageHpLabel:setScaleX(1.0)
    end
    
    damageHpLabel:setPosition(cc.p(0,0))
    damageHpLabel:setOpacity(255)
    damageHpLabel:setString(":"..damageVal)
    local spawn = cc.Spawn:create(cc.MoveBy:create(1.0,cc.p(-20.0, 100.0)) ,cc.ScaleBy:create(1.0 ,1.4 ,1.4))
    local seq = cc.Sequence:create(cc.EaseOut:create(spawn,3.0), cc.FadeOut:create(0.5),cc.RemoveSelf:create())
    damageHpLabel:runAction(seq)
end

--[[获取血条的血量百分比]]
function NpcActor:getHpPercent()
    local percent = self.hpSlider:getValue()
    return percent
end

return NpcActor

