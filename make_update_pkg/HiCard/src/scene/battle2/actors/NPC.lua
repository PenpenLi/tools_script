require("Cocos2d")

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
    elseif cmd.t == "die" then
        self.isDie = true
        self:hideAllEffect()
        self:playAnimation("die_1_1")
        self:runAction(cc.Sequence:create(cc.DelayTime:create(2), cc.CallFunc:create(function()
            self:removeFromParent()
        end)))
    end
end

function NPC:hideAllEffect()
    self.hpSlider:setVisible(false)
    self:removeBuffState()
end

function NPC:getDirection()
    return self.npcBook.direction
end

function NPC:getFirePosition()
    local point = {}
    table.merge(point,self.npcBook.firePosition)
    point.x = point.x*self.npcBook.direction
    return cc.pAdd(cc.p(self:getPositionX(),self:getPositionY()), point)
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
    self.hpSlider:setPercentage(100-self.hp/self.npcBook.maxHp)
    local effectHpSlider = self.hpSlider:getChildByTag(101)
    local  dt = 0.5
    if  hp/self.npcBook.maxHp < 0.05 then
        dt = 0.1
    elseif hp/self.npcBook.maxHp >=0.05 and hp/self.npcBook.maxHp <0.1 then
        dt = 0.2
    end
    local to = cc.ProgressFromTo:create(dt,100-preHp/self.npcBook.maxHp,100-self.hp/self.npcBook.maxHp)
    effectHpSlider:runAction(cc.Sequence:create(cc.Show:create(),to,cc.Hide:create()))
end

--[[ 
damageVal (negative number indicates increase blood)
]]--
function NPC:changeHpEffect(damageVal)
    if damageVal == 0 then return end
    local damageHpLabel = ccui.TextAtlas:create()

    damageHpLabel:setProperty("0123456789:", "battle/ui/shuzi_hong.png", 22, 32, "0")
    if  damageVal <= 0 then
        damageHpLabel:setProperty("0123456789:", "battle/ui/shuzi_lv.png", 22, 32, "0")
    end
    self:addChild(damageHpLabel)
    
    local factor = 1.0
    if  self.npcBook.name == "Tyrande" or self.npcBook.name == "Jaina" or
        self.npcBook.name == "DiXueLingZhu" or self.npcBook.name == "Kelthuzad" or 
        self.npcBook.name == "YiLiDan"  or self.npcBook.name == "ShuiYuanSu" or
        self.npcBook.name == "ShahelasiZhuMu"  or self.npcBook.name == "LaGeNaLuoSi" or
        self.npcBook.name == "AoNiKeXiYa"  or self.npcBook.name == "KeLuoMaGuSi" or
        self.npcBook.name == "MaNuoLuoSi" then
        factor = 0.7
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
    self:runAttackEft(effectType)
    self:changeHpEffect(subHp)
    self:changeHp(subHp)

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
        Helper.addSpriteFrames("battle/skill/XiaoHei/bingbao.plist","battle/skill/XiaoHei/bingbao.pvr.ccz")
        local frameArr = Helper.newFrames("bingbao%d.png",7,7)
        animation = Helper.newAnimation(frameArr,0.2/7)
        self.anmiSprite:setAnchorPoint(cc.p(0.5,0))
        self.anmiSprite:setPosition(cc.p(0.5,-self.armature:getContentSize().height*self.npcBook.scaleFactor/2))
    elseif attackType == "chishao" then
        Helper.addSpriteFrames("battle/hurt/chishao_hurt.plist","battle/hurt/chishao_hurt.pvr.ccz")
        local frameArr = Helper.newFrames("chishao_0_%d.png",0,7)
        animation = Helper.newAnimation(frameArr,1.0/7)
        animation:setLoops(2)
        --[[
        self.anmiSprite:setAnchorPoint(cc.p(0.5,0))
        self.anmiSprite:setPosition(cc.p(0.5,-self.armature:getContentSize().height*self.npcBook.scaleFactor/2))
        ]]
    end

    
    local action = cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create())
    self.anmiSprite:runAction(action)
    
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
    if  type == "yun" then
        Helper.addSpriteFrames("battle/buff/xuanyun.plist","battle/buff/xuanyun.png")
        local frameArr = Helper.newFrames("xuanyun_%d.png",0,19)
        animation = Helper.newAnimation(frameArr,1/20)
    end
    if  self.bufferSprite == nil then
        self.bufferSprite = cc.Sprite:create()
        self:addChild(self.bufferSprite)
    end
    self.bufferSprite:setPosition(cc.p(self.npcBook.hpPosition.x,self.npcBook.hpPosition.y+20))
    
    local action =  cc.RepeatForever:create(cc.Animate:create(animation))
    self.bufferSprite:runAction(action)
    self:playAnimation("standby_1_1")
end

function NPC:removeBuffState(type)
    if  self.bufferSprite == nil then return end
    if self.bufferSprite ~= nil then
        self.bufferSprite:removeFromParent()
        self.bufferSprite = nil
    end  
end

function NPC:runAttackEft(eft)
    if  eft == 1 then
        return
    elseif eft==2 then
        self.eftSprite:setTexture("battle/ui/shanbi.png")
    elseif eft==3 then
        self.eftSprite:setTexture("battle/ui/gedang.png")
    elseif  eft == 4 then
        self.eftSprite:setTexture("battle/ui/baoji.png")
    end
    --self.eftSprite:setScaleY(0.5)
    local scale =1
    if  self.npcBook.direction == -1 then 
        self.eftSprite:setScaleX(-1.0)
        scale = -1
    else  
        self.eftSprite:setScaleX(1.0)
    end
    self.eftSprite:setPosition(cc.p(self.npcBook.hpPosition.x,self.npcBook.hpPosition.y+60))
    --local spawn = cc.Spawn:create(cc.MoveBy:create(1.0,cc.p(0, 150.0)) ,cc.ScaleTo:create(1.0 ,1.2*scale ,1.2) ,cc.FadeOut:create(1.0))
    local seq = cc.Sequence:create(cc.Show:create(),cc.DelayTime:create(0.4),cc.FadeOut:create(0.4),cc.Hide:create())
    self.eftSprite:runAction(seq)

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
    self:setScaleX(self.npcBook.scaleFactor*self.npcBook.direction)
    self:setScaleY(self.npcBook.scaleFactor)
    --set z order
    self.zOrder = cmd.zOrder
    self:setLocalZOrder(self.zOrder)
    --armature
    local armatureFile = "image/battle/armature/"..self.npcBook.name.."/"..self.npcBook.name..".csb"
    print(armatureFile)
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    self.armature = ccs.Armature:create(self.npcBook.name)
    self.armature:getAnimation():playWithIndex(0)
    self:addChild(self.armature)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
   
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
    if  self.npcBook.name == "Tyrande" or self.npcBook.name == "Jaina" or
        self.npcBook.name == "DiXueLingZhu" or self.npcBook.name == "Kelthuzad" or 
        self.npcBook.name == "YiLiDan"  or self.npcBook.name == "ShuiYuanSu" or
        self.npcBook.name == "ShahelasiZhuMu"  or self.npcBook.name == "LaGeNaLuoSi" or
        self.npcBook.name == "AoNiKeXiYa"  or self.npcBook.name == "KeLuoMaGuSi" or
        self.npcBook.name == "MaNuoLuoSi" then
        self.hpSlider:setScale(0.7)
    end
    --self.hpSlider:setPercentage(0)
    self:addChild(self.hpSlider)
    self.hpSlider:addChild(hpSprite,-2)
    
    hpSprite:setAnchorPoint(cc.p(0.5,0.5))
    hpSprite:setPosition(cc.p(self.hpSlider:getContentSize().width/2,self.hpSlider:getContentSize().height/2)) 
    hpSprite:setTag(100)
    hpSprite:setVisible(false)
    
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
end

function NPC:playAnimation(animationName)
    self.armature:getAnimation():play(animationName, -1, -1)
end

return NPC

