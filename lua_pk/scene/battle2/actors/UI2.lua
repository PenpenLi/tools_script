require "Cocos2d"
local define_soul = require "config.define_soul"
local define_jueXing_skill = require "config.juexing_skill"

local pos_scale_factor = WIN_SIZE.width/960

------------------------- SkillBtn ---------------------------
local SkillBtn = class("SkillBtn",function()
    return ccui.Button:create()
end)

function SkillBtn:create(param)
    local skillBtn = SkillBtn.new()
    skillBtn:initM(param)
    return skillBtn
end

function SkillBtn:initM(param)
    local spriteFrameName = param["icon"]..".png"
    self:loadTextures(spriteFrameName, spriteFrameName, "")
    self:setTag(param.type)
    
    local ring = cc.Sprite:createWithSpriteFrameName("battle_jineng_bg.png")
    ring:setPosition(46,49)
    ring:setVisible(false)
    local scaleAction1 = cc.ScaleBy:create(0.5,1.05)
    local scaleAction2 = scaleAction1:reverse()
    ring:runAction(cc.RepeatForever:create(cc.Sequence:create(scaleAction1, scaleAction2)))
    self:addChild(ring)
    self.ring = ring
    
    local maskSprite = cc.Sprite:createWithSpriteFrameName("battle_skillbtn_mask.png")
    local mask = cc.ProgressTimer:create(maskSprite)
    mask:setPosition(46,49)
    mask:setType(0)
    mask:setPercentage(100)
    mask:setReverseProgress(true)
    self:addChild(mask)
    self.mask = mask
    
    self:setTouchEnabled(false)
    self.updateCount = 0
    self.maxFireCount = 30*param["cd"]
end

function SkillBtn:updateM()
end

function SkillBtn:canFire()
    if self.percentage >= 100 then
        return true
    end
    return false
end

function SkillBtn:reset()
    self.updateCount = 0
    self.mask:setPercentage(100)
    self:setTouchEnabled(false)
    self.ring:setVisible(false)
end

function SkillBtn:disable()
    self:setTouchEnabled(false)
    self.ring:setVisible(false)
end

function SkillBtn:enable()
    if self.percentage >= 100 then
        self:setTouchEnabled(true)
        self.ring:setVisible(true)
    else
        self:setTouchEnabled(false)
        self.ring:setVisible(false)
    end
end

function SkillBtn:setPowerPercentage(percentage)
    self.percentage = percentage
    self.mask:setPercentage(100-percentage)
    self:enable()
end

------------------------- HeroSkillBtn ---------------------------
local HeroSkillBtn = class("HeroSkillBtn",function()
    return cc.Node:create()
end)

function HeroSkillBtn:create(param)
    local skillBtn = HeroSkillBtn.new()
    skillBtn:initM(param)
    return skillBtn
end

function HeroSkillBtn:initM(param)
    self.id = param.soulId
    local btnBg =  cc.Sprite:create("image/battle/battleuilayer2/battleuilayer3_iconbg.png")
    btnBg:setPosition(0,-45)
    self:addChild(btnBg)
    
    self.btn = ccui.Button:create()
    local headIconName = "image/soul/"..param.icon..".png"
    self.btn:loadTextures(headIconName, headIconName, "")
    self:addChild(self.btn)
    
    local colorMask = cc.Sprite:create("image/common/normal_aochao_0"..param.quality..".png")
    self:addChild(colorMask)
    
    local ring = cc.Sprite:create("image/battle/battleuilayer2/battleuilayer3_ring.png")
    --ring:setPosition(46,49)
    ring:setVisible(false)
    local scaleAction1 = cc.ScaleBy:create(0.5,1.05)
    local scaleAction2 = scaleAction1:reverse()
    ring:runAction(cc.RepeatForever:create(cc.Sequence:create(scaleAction1, scaleAction2)))
    self:addChild(ring)
    self.ring = ring
    
    local hpoffset = -70
    local hpSpritebg = cc.Sprite:create("image/battle/battleuilayer2/battleuilayer3_hp1.png")
    hpSpritebg:setPosition(0,hpoffset)
    self:addChild(hpSpritebg)
    local hpSprite = cc.Sprite:create("image/battle/battleuilayer2/battleuilayer3_hp2.png")
    local hp = cc.ProgressTimer:create(hpSprite)
    hp:setType(1)
    hp:setBarChangeRate(cc.p(1,0))
    hp:setMidpoint(cc.p(0, 0.5))
    hp:setPercentage(100)
    --hp:setReverseProgress(true)
    hp:setPosition(0,hpoffset)
    self:addChild(hp)
    self.hp = hp
    
    local poweroffset = -85
    local powerSpritebg = cc.Sprite:create("image/battle/battleuilayer2/battleuilayer3_hp1.png")
    powerSpritebg:setPosition(0,poweroffset)
    self:addChild(powerSpritebg)
    local powerSprite = cc.Sprite:create("image/battle/battleuilayer2/battleuilayer3_hp3.png")
    local power = cc.ProgressTimer:create(powerSprite)
    power:setType(1)
    power:setBarChangeRate(cc.p(1,0))
    power:setMidpoint(cc.p(0, 0.5))
    power:setPercentage(0)
    --mask:setReverseProgress(true)
    power:setPosition(0,poweroffset)
    self:addChild(power)
    self.power = power
    
    self.btn:setTouchEnabled(false)
    self.updateCount = 0
    self.maxFireCount = 30*6
end

function HeroSkillBtn:addClickListener(callback)
    self.btn:addTouchEventListener(function(sender,eventType) 
        if eventType == ccui.TouchEventType.ended then
            callback({id=self.id})
        end
    end)
end

function HeroSkillBtn:updateM()
    if self.updateCount > self.maxFireCount then return end
    self.updateCount = self.updateCount + 1
    if self.updateCount % 5 == 0 then
        self.mask:setPercentage(100-math.floor(100*self.updateCount/self.maxFireCount))
    end
    if self.updateCount == self.maxFireCount then
        self.btn:setTouchEnabled(true)
        self.ring:setVisible(true)
    end
end

function HeroSkillBtn:setPowerPercentage(percentage)
    self.percentage = percentage
    self.power:setPercentage(percentage)
    if percentage >= 100 then
        self:enable()
    else
        self:disable()
    end
end

function HeroSkillBtn:setHpPercentage(percentage)
    self.hp:setPercentage(percentage)
end

function HeroSkillBtn:canFire()
    if self.updateCount >= self.maxFireCount then
        return true
    end
    return false
end

function HeroSkillBtn:reset()
    self.updateCount = 0
    self.power:setPercentage(0)
    self.btn:setTouchEnabled(false)
    self.ring:setVisible(false)
end

function HeroSkillBtn:disable()
    self.btn:setTouchEnabled(false)
    self.ring:setVisible(false)
end

function HeroSkillBtn:enable()
    self.isDie = self.isDie or false
    if self.percentage and self.percentage >= 100 and self.isDie==false and self.isSilence==nil then
        self.btn:setTouchEnabled(true)
        self.ring:setVisible(true)
    end
end

function HeroSkillBtn:die()
    self:reset()
    self.isDie = true
    Helper.graySprite(self.btn:getVirtualRenderer())
end

------------------------- UI2 ---------------------------
local UI2 = class("UI2", function()
    return cc.Layer:create()
end)

function UI2:create(param)
    local layer = UI2.new()
    layer:initM(param)
    return layer
end

function UI2:ctor()
end

function UI2:onEnter()
end

function UI2:onExit()
end

function UI2:initM(param)
    Helper.StopBackGroundMuisc()
    Helper.PlayBackGroundMuisc("sound/battle_bg_music.mp3", 1)
    
    self.gameoverTimeout = param.roundTime
    
    self.fireModeKey = Roler:get("roleId").."_fireModeKey"
    self.fireMode = cc.UserDefault:getInstance():getIntegerForKey(self.fireModeKey)
    if self.fireMode == nil or self.fireMode == "" then
        cc.UserDefault:getInstance():setIntegerForKey(self.fireModeKey,0)
        self.fireMode = cc.UserDefault:getInstance():getIntegerForKey(self.fireModeKey)
    end
    
    self.frameSq = 0
    cc.SpriteFrameCache:getInstance():addSpriteFrames("image/battle/battleuilayer2/battleuilayer2.plist")
    
    local pauseBtn = ccui.Button:create()
    pauseBtn:loadTextureNormal("battle_pause.png", 1)
    
    if self:isGuarding() then 
        pauseBtn:setVisible(false)
    end
    
    pauseBtn:setPosition(WIN_SIZE.width-60,WIN_SIZE.height-60)
    pauseBtn:addTouchEventListener(function(sender,eventType)
     
    local status = Roler:get("settingStatus")
    local before_status = status
        if eventType == ccui.TouchEventType.ended then
            print("[UI2] pauseBtn pressed")
            cc.Director:getInstance():pause()
            local dialog = cc.Node:create()
            dialog:setPosition(WIN_SIZE.width/2,WIN_SIZE.height/2)
            self:addChild(dialog)
            local colorLayer = cc.LayerColor:create(cc.c4b(0, 0, 0, 100), WIN_SIZE.width, WIN_SIZE.height)
            colorLayer:setPosition(-WIN_SIZE.width/2,-WIN_SIZE.height/2)
            colorLayer:setOpacity(100)
            dialog:addChild(colorLayer)
            local diaglogBg = ccui.ImageView:create()
            diaglogBg:loadTexture("battle_dialog_bg.png", 1)
            diaglogBg:setScale9Enabled(true)
            diaglogBg:setContentSize(435, 140)
            dialog:addChild(diaglogBg)
            local function onTouchBegan(touch, event) return true end
            local function onTouchMoved(touch, event) end
            local function onTouchEnded(touch, event) end
            local listener = cc.EventListenerTouchOneByOne:create()
            listener:setSwallowTouches(true)
            listener:registerScriptHandler(onTouchBegan,cc.Handler.EVENT_TOUCH_BEGAN )
            listener:registerScriptHandler(onTouchMoved,cc.Handler.EVENT_TOUCH_MOVED )
            listener:registerScriptHandler(onTouchEnded,cc.Handler.EVENT_TOUCH_ENDED )
            local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
            eventDispatcher:addEventListenerWithSceneGraphPriority(listener, diaglogBg)
            
            local exitBtn = ccui.Button:create()
            exitBtn:setPosition(-125,10)
            exitBtn:loadTextureNormal("battle_exit_01.png", 1)
            exitBtn:loadTextureNormal("battle_exit_02.png", 1)
            exitBtn:addTouchEventListener(function(sender, eventType)
                if eventType == ccui.TouchEventType.ended then
                    cc.Director:getInstance():resume()
                    cc.Director:getInstance():popScene()
                    
                    if before_status ~= status then
                            Http:req("SetTheSettingStatus",{settingStatus = status},function(res)
                                end)
                    end
                    
                end
            end)
            dialog:addChild(exitBtn)
            
            local exitTip = ccui.Text:create()
            exitTip:setString("退出战斗")
            exitTip:setPosition(-125,-50)
            exitTip:setFontSize(16)
            dialog:addChild(exitTip)
            
            local soundTip = ccui.Text:create()
            soundTip:setString("关闭音效")
            soundTip:setPosition(5,-50)
            soundTip:setFontSize(16)
            dialog:addChild(soundTip)
            
            
            local soundBtn = ccui.Button:create()
            soundBtn:setPosition(5,10)
            if cc.SimpleAudioEngine:getInstance():isMusicPlaying() then
                soundBtn:loadTextures("battle_yinxiao_03.png","battle_yinxiao_04.png","battle_yinxiao_04.png")
                soundTip:setString("关闭音效")
                --soundBtn:lo额adTextureNormal("battle_yinxiao_04.png", 1)
                print("It is playing music!!!")
            else
                soundBtn:loadTextures("battle_yinxiao_01.png","battle_yinxiao_02.png","battle_yinxiao_02.png")
                --soundBtn:loadTextureNormal("battle_yinxiao_02.png", 1)
                soundTip:setString("打开音效")
                print("It is not playing music!!!")
            end
            soundBtn:addTouchEventListener(function(sender, eventType)
                    
                if eventType == ccui.TouchEventType.ended then
                    if cc.SimpleAudioEngine:getInstance():isMusicPlaying() then
                        status[4] = 0
                        Roler:set("settingStatus",status)
                        Helper.StopBackGroundMuisc()
                        soundBtn:loadTextures("battle_yinxiao_01.png","battle_yinxiao_02.png","battle_yinxiao_02.png")
                        --soundBtn:loadTextureNormal("battle_yinxiao_02.png", 1)
                        soundTip:setString("打开音效")
                         
                    else
                        status[4] = 1
                        Roler:set("settingStatus",status)
                        Helper.StopBackGroundMuisc()
                        Helper.PlayBackGroundMuisc("sound/battle_bg_music.mp3", 1)
                        soundBtn:loadTextures("battle_yinxiao_03.png","battle_yinxiao_04.png","battle_yinxiao_04.png")
                        soundTip:setString("关闭音效")
                        
                    end
                end
            end)
            dialog:addChild(soundBtn)
            
            local continueBtn = ccui.Button:create()
            continueBtn:setPosition(135,10)
            continueBtn:loadTextureNormal("battle_continue_01.png", 1)
            continueBtn:loadTextureNormal("battle_continue_02.png", 1)
            continueBtn:addTouchEventListener(function(sender, eventType)
                if eventType == ccui.TouchEventType.ended then
                    cc.Director:getInstance():resume()
                    dialog:removeFromParent()
                    
                    if before_status ~= status then
                        Http:req("SetTheSettingStatus",{settingStatus = status},function(res)
                            end)
                    end
                end
            end)
            dialog:addChild(continueBtn)
            
            local continueTip = ccui.Text:create()
            continueTip:setString("继续战斗")
            continueTip:setPosition(135,-50)
            continueTip:setFontSize(16)
            dialog:addChild(continueTip)
        end
    end)
    self:addChild(pauseBtn)
    pauseBtn:setName("pauseBtn")
    
    local treasure = cc.Node:create()
    treasure:setPosition(10,WIN_SIZE.height-60)
    local treasureBg = ccui.ImageView:create()
    treasureBg:loadTexture("battle_xiaodi.png", 1)
    treasureBg:setAnchorPoint(0,0)
    treasureBg:setScale9Enabled(true)
    treasureBg:setScale(1.3,1)
    treasure:addChild(treasureBg)
    
    local treasureIcon = cc.Sprite:createWithSpriteFrameName("battle_baoxiang.png")
    treasureIcon:setPosition(0,-4)
    treasureIcon:setAnchorPoint(0,0)
    treasure:addChild(treasureIcon)
    local treasureLabel = ccui.Text:create()
    treasureLabel:setString("0")
    treasureLabel:setFontSize(32)
    treasureLabel:setPosition(80,0)
    treasureLabel:setAnchorPoint(0,0)
    treasure:addChild(treasureLabel)
    self:addChild(treasure)
    
    local shalou = cc.Node:create()
    shalou:setPosition(180,WIN_SIZE.height-60)
    local shalouBg = ccui.ImageView:create()
    shalouBg:loadTexture("battle_xiaodi.png", 1)
    shalouBg:setAnchorPoint(0,0)
    shalouBg:setScale9Enabled(true)
    shalouBg:setScale(1.3,1)
    shalou:addChild(shalouBg)
    local shalouIcon = cc.Sprite:createWithSpriteFrameName("battle_shalou.png")
    shalouIcon:setPosition(0,-4)
    shalouIcon:setAnchorPoint(0,0)
    --shalouIcon:runAction(cc.RepeatForever:create(cc.RotateBy:create(1,360)))
    shalou:addChild(shalouIcon)
    local shalouLabel = ccui.Text:create()
    shalouLabel:setString("00:00")
    shalouLabel:setFontSize(32)
    shalouLabel:setPosition(45,0)
    shalouLabel:setAnchorPoint(0,0)
    shalou:addChild(shalouLabel)
    self.shalouLabel = shalouLabel
    self:addChild(shalou)
    
    local fireModeBtn = ccui.Button:create()
    --2 is pvp battle
    self.battleType = param.battleType
    if param.battleType == 2 then 
        fireModeBtn:loadTextureNormal("battle_shoudong01.png", 1)
        fireModeBtn:loadTexturePressed("battle_shoudong02.png", 1)
        fireModeBtn:setTouchEnabled(false)
    elseif param.battleType == 1 then
        if self.fireMode == 0 then
            fireModeBtn:loadTextureNormal("battle_zidong01.png", 1)
            fireModeBtn:loadTexturePressed("battle_zidong02.png", 1)
        else
            fireModeBtn:loadTextureNormal("battle_shoudong01.png", 1)
            fireModeBtn:loadTexturePressed("battle_shoudong02.png", 1)
            directorAgent:setAutoFight(true)
        end
    end
    if self:isGuarding() then 
        fireModeBtn:setVisible(false) 
    end
    
    fireModeBtn:setPosition(WIN_SIZE.width-150,WIN_SIZE.height-60)
    fireModeBtn:addTouchEventListener(function(sender,eventType) 
        if eventType == ccui.TouchEventType.ended then
            print("[UI2] fireModeBtn pressed")
            self.fireMode = (self.fireMode + 1) % 2
            if self.fireMode == 0 then
                fireModeBtn:loadTextureNormal("battle_zidong01.png", 1)
                fireModeBtn:loadTexturePressed("battle_zidong02.png", 1)
                directorAgent:setAutoFight(false)
            else
                fireModeBtn:loadTextureNormal("battle_shoudong01.png", 1)
                fireModeBtn:loadTexturePressed("battle_shoudong02.png", 1)
                directorAgent:setAutoFight(true)
            end
            cc.UserDefault:getInstance():setIntegerForKey(self.fireModeKey,self.fireMode)
        end
    end)
    self:addChild(fireModeBtn)
    
    local maxSkillNum = 4
    self.skillPanel = cc.Node:create()
    --local skillPanelBg = cc.Sprite:createWithSpriteFrameName("battle_xiaodi.png")
    local skillPanelBg = cc.Sprite:create("image/battle/battleuilayer2/battleuilayer3_padbg.png")
    --skillPanelBg:setScale9Enabled(true)
    --skillPanelBg:setScaleX(maxSkillNum*120/skillPanelBg:getContentSize().width)
    --skillPanelBg:setScaleY(125/skillPanelBg:getContentSize().height)
    --skillPanelBg:setAnchorPoint(0,0)
    skillPanelBg:setPosition(250,250)
    --self:addChild(skillPanelBg)
    
    self.skillPanel:setPosition(750*pos_scale_factor,30)
    self.skillPanel:setAnchorPoint(0,0)
    self:addChild(self.skillPanel)
    
    self.isJX = false
    self.seqNum = 0
    self.btns = {}
    self.btnNum = 0
    
    local configJXParam = {}
    for k, v in pairs(param.memberList) do
        local item = {}
        item.soulId = v
        item.zhili = 0
        table.insert(configJXParam, item)
    end
    
    local configJXSkills = {}
    if param.isEmulate and param.isEmulate==true then
        configJXSkills = Helper.createJXSkill(2, 200)
    else
        configJXSkills = Helper.calJXSkills(configJXParam)
    end
    for k, v in pairs(configJXSkills) do
        self:addBtn(v)
    end
    
    maxSkillNum = self.btnNum
    --skillPanelBg:setScaleX(maxSkillNum*120/skillPanelBg:getContentSize().width)
    
    --Add hero skill fire button
    self.heroSkillBtns = {}
    self.heroSkillBtnNum = 0
    self.heroSkillPad = cc.Node:create()
    self.heroSkillPad:setPosition(0.1*WIN_SIZE.width, 50)
    self:addChild(self.heroSkillPad)
    local padBg = cc.Sprite:create("image/battle/battleuilayer2/battleuilayer3_padbg.png")
    --self.heroSkillPad:addChild(padBg)
    
    for _, v in pairs(param.memberList) do
        local soul = define_soul[tostring(v)]
        if soul then
            self:addHeroSkillBtn({soulId=v, icon=soul.icon, quality=soul.quality})
        end
    end
end

function UI2:updateM()
    self.frameSq = self.frameSq + 1
    if self.frameSq % 30 == 0 then
        local seconds = math.floor(self.frameSq/30)
        seconds = self.gameoverTimeout-seconds
        if seconds > 0 and seconds < 10 then
            self.shalouLabel:setColor(cc.c3b(255, 0, 0))
        end
        --game time out
        if seconds ==0 then
            return
        end
        local s = seconds%60
        local m = math.floor(seconds/60)
        if m >= 10 and s >= 10 then
            self.shalouLabel:setString(m..":"..s)
        elseif m >= 10 and s < 10 then
            self.shalouLabel:setString(m..":0"..s)
        elseif m < 10 and s >= 10 then
            self.shalouLabel:setString("0"..m..":"..s)
        elseif m < 10 and s < 10 then
            self.shalouLabel:setString("0"..m..":0"..s)
        end
    end
end

function UI2:addBtn(param)
    local btn = SkillBtn:create(param)
    btn:setPosition((self.btnNum*100+50)*pos_scale_factor,60)
    self.skillPanel:addChild(btn)
    self.btns[param.type] = btn
    local function touchEvent(sender,eventType)
        if eventType == ccui.TouchEventType.ended then
            if directorAgent:canHandleGlobalSkill() then
                --guard support
                local function removeGuardLayer()
                    if self.guardLayer then
                        cc.Director:getInstance():getRunningScene():removeChild(self.guardLayer)
                        self.guardLayer = nil
                        directorAgent:resume()
                    end
                end
                removeGuardLayer()
                self:fireJX(param.type)
                btn:reset()
            end
        end
    end
    if self.battleType == 1 then
        btn:addTouchEventListener(touchEvent)
    end
    self.btnNum = self.btnNum + 1
end

function UI2:addHeroSkillBtn(param)
    local btn = HeroSkillBtn:create(param)
    self.heroSkillBtns[tonumber(param.soulId)] = btn
    self.heroSkillPad:addChild(btn)
    btn:setPosition((self.heroSkillBtnNum*140+30)*pos_scale_factor, 60)
    self.heroSkillBtnNum = self.heroSkillBtnNum+1
    
    if self.battleType == 1 then
        btn:addClickListener(function(param) 
            print("BigSkill Fired: "..param.id) 
            --guard support
            local function removeGuardLayer()
                if self.guardLayer then
                    cc.Director:getInstance():getRunningScene():removeChild(self.guardLayer)
                    self.guardLayer = nil
                    directorAgent:resume()
                end
            end
            removeGuardLayer()

            if directorAgent:canFireSkill(btn.id) then
                btn:reset()
                directorAgent:fireBigSkill({id=param.id})
            end
        end)
    end
end

function UI2:fireJX(type)
    print("[UI2]fire JX "..type)
    --type = type % 4 + 13
    self.seqNum = self.seqNum + 1
    directorAgent:fireJXSkill({skillid=type, hp=100, dt=3, seq=self.seqNum, direction=1})
end

function UI2:startJX()
    self.isJX = true
end

function UI2:stopJX()
    self.isJX = false
end

function UI2:skillBegin()
    for k, v in pairs(self.btns) do
        v:disable()
    end
end

function UI2:skillEnd()
    for k, v in pairs(self.btns) do
        v:enable()
    end
end

function UI2:disableAllBigSkill()
    for k, v in pairs(self.heroSkillBtns) do
        v:disable()
    end
end

function UI2:enableAllBigSkill()
    for k, v in pairs(self.heroSkillBtns) do
        if v and v.isSilence==nil then
            v:enable()
        end
    end
end

function UI2:enableSkillBtn(soulId)
    local btn = self.heroSkillBtns[soulId]
    if btn then
        btn:enable()
    end
end

function UI2:disableSkillBtn(soulId)
    local btn = self.heroSkillBtns[soulId]
    if btn then
        btn:disable()
    end
end

function UI2:getTreasurePos()
end

function UI2:addTreasureNum()
end

function UI2:addBigSkillPower(param)
    local btn = self.heroSkillBtns[tonumber(param.soulId)]
    self.guildSkillBtn = btn
    if btn then
        btn:setPowerPercentage(param.powerRate)
        if param.powerRate >= 100 then
            if self.guardFightBtn == nil and guardCenter:canEnterGuardMode("pve") == true then
                self.guardFightBtn = btn.btn
                self:processGuard()
            end
            
            if self.guardFightBtn3 == nil and guardCenter:canEnterGuardMode("pve2") == true then
                self.guardFightBtn3 = btn.btn
                self:processGuard()
            end
            
            
            if guardCenter:canEnterGuardMode("moni") == true then
                --self.guardFightBtnMoni1 = btn.btn
               if self.guardFightBtnMoni1 == nil then
                    self.guardFightBtnMoni1 = btn.btn
                    self:processGuard()
                elseif self.guardFightBtnMoni2 == nil then
                    self.guardFightBtnMoni2 = btn.btn
                    self:processGuard()
                elseif self.guardFightBtnMoni3 == nil then
                    self.guardFightBtnMoni3 = btn.btn
                    self:processGuard()
                elseif self.guardFightBtnMoni4 == nil then
                    self.guardFightBtnMoni4 = btn.btn
                    self:processGuard()
                elseif self.guardFightBtnMoni5 == nil then
                    self.guardFightBtnMoni5 = btn.btn
                    self:processGuard()
                    --Helper.setTimeout(self:processGuard(), 10, function()btn:disable()end)
                elseif self.guardFightBtnMoni777 == nil then
                    btn:disable()
              end
            end
        end
    else
        print("[UI2] no skillbtn for soulId "..param.soulId)
    end
end

function UI2:setHp(soulId, hpRate)
    local btn = self.heroSkillBtns[soulId]
    if btn then
        btn:setHpPercentage(hpRate)
    end
end

function UI2:disableBigSkill(soulId)
    local btn = self.heroSkillBtns[soulId]
    if btn then
        btn:disable()
    end
end

function UI2:enableBigSkill(soulId)
    local btn = self.heroSkillBtns[soulId]
    if btn and btn.isSilence==nil then
        btn:enable()
    end
end

function UI2:startSilence(soulId)
    local btn = self.heroSkillBtns[soulId]
    if btn then
        btn.isSilence = true
        btn:disable()
        print("Start Silence...")
    else
        print("[UI2]ERR: no such skill btn for "..soulId)
    end
end

function UI2:stopSilence(soulId)
    local btn = self.heroSkillBtns[soulId]
    if btn then
        btn.isSilence = nil
        btn:enable()
        print("Stop Silence...")
    end
end

function UI2:die(soulId)
    local btn = self.heroSkillBtns[soulId]
    if btn then
        btn:die()
    end
end

function UI2:addJXPower(cmd)
    local jxBtn = self.btns[cmd.type]
    if jxBtn then
        jxBtn:setPowerPercentage(cmd.p)
        if cmd.p >= 100 then
            if self.guardFightBtn1 == nil and guardCenter:canEnterGuardMode("pve") == true then
                self.guardFightBtn1 = jxBtn
                self:processGuard()
            end
            
            if self.guardFightBtn4 == nil and guardCenter:canEnterGuardMode("pve2") == true then
                self.guardFightBtn4 = jxBtn
                self:processGuard()
            end
            
             
            if guardCenter:canEnterGuardMode("moni") == true then
                --self.guardFightBtnMoni1 = btn.btn
                self.guildSkillBtn:disable()
               if self.guardFightBtnMoni6 == nil then
                    self.guardFightBtnMoni6 = jxBtn
                    self:processGuard()
               end
            end   
        end
    end
    
    if guardCenter:canEnterGuardMode("moni") == true and self.guardFightBtnMoni6 ~= nil then
        if jxBtn then
            jxBtn:setPowerPercentage(cmd.p)
            if cmd.p == 10 then 
               -- btn:disable()
               --self.guildSkillBtn:disable()
                Helper.setTimeout(self, 10, function()self:processGuard()end) 
            end
        end
    end
    
 end  

function UI2:resetBigSkillPower(soulId)
    local btn = self.heroSkillBtns[soulId]
    if btn then
        btn:setPowerPercentage(0)
    end
end

function UI2:processGuard()
    local function addGuardDialogLayer(cmd)
        cclog("here pause")
        directorAgent:pause()
        cmd.type = "guard_dialog"
        cmd.cb = function() 
        if cmd.tag then
            directorAgent:resume()
        end
            
            self:processGuard() 
        end
        Helper.setTimeout(self, 5, function()
            self.guardLayer = require("common.Guard"):create(cmd)
            cc.Director:getInstance():getRunningScene():addChild(self.guardLayer)
        end)
    end
    local function addGuardClickLayer(cmd)
        assert(cmd.clickTarget~=nil,"")
        directorAgent:pause()
        cmd.type = "guard_click"
        Helper.setTimeout(self, 5, function()
            self.guardLayer = require("common.Guard"):create(cmd)
            cc.Director:getInstance():getRunningScene():addChild(self.guardLayer)
        end)
    end
    local function removeGuardLayer()
        if self.guardLayer then
            cc.Director:getInstance():getRunningScene():removeChild(self.guardLayer)
            self.guardLayer = nil
        end
    end
    removeGuardLayer()

    if guardCenter:canEnterGuardMode("pve") == true then
        self.fightGuardStep = self.fightGuardStep or 0
        self.fightGuardStep = self.fightGuardStep + 1
        print("here760                            "..self.fightGuardStep)
        if self.fightGuardStep <= 6 then
            local cmd = guardCenter:getCmd("pve")
            if cmd == nil then return print("no cmd for pve guard now") end
            if cmd.name == "pve_dialog_skill" then
           -- self.guardFightBtn:
                addGuardDialogLayer(cmd)
            elseif cmd.name == "pve_click_fireskillbtn" then
                cmd.clickTarget = self.guardFightBtn
                addGuardClickLayer(cmd)
            elseif cmd.name == "pve_dialog_jxskill" then 
                addGuardDialogLayer(cmd)
            elseif cmd.name == "pve_dialog_jxskill2" then 
                addGuardDialogLayer(cmd)
            elseif cmd.name == "pve_dialog_jxskill3" then 
                addGuardDialogLayer(cmd)
            elseif cmd.name == "pve_click_fireJXbtn" then
                cmd.clickTarget = self.guardFightBtn1
                addGuardClickLayer(cmd)
            end
        end
    end

    if guardCenter:canEnterGuardMode("pve2") == true then
        self.fightGuardStep1 = self.fightGuardStep1 or 0
        self.fightGuardStep1 = self.fightGuardStep1 + 1
        if self.fightGuardStep1 <= 2 then
            local cmd = guardCenter:getCmd("pve2")
            if cmd == nil then return print("no cmd for pve2 guard now") end
            if cmd.name == "pve2_click_fireskillbtn" then
                cmd.clickTarget = self.guardFightBtn3
                addGuardClickLayer(cmd)
            elseif cmd.name == "pve2_click_fireJXbtn" then
                cmd.clickTarget = self.guardFightBtn4
                addGuardClickLayer(cmd)
            end
        end
    end
    
    if guardCenter:canEnterGuardMode("moni") == true then
        self.fightGuardStep3 = self.fightGuardStep3 or 0
        self.fightGuardStep3 = self.fightGuardStep3 + 1

        if self.fightGuardStep3 <= 9 then
            local cmd = guardCenter:getCmd("moni")
            if cmd == nil then return print("no cmd for moni guard now") end
            printJSON(cmd)
            print(self.fightGuardStep3.."   aaaaaaaaaaa")
            if cmd.name == "moni_dialog_bt2" then
                addGuardDialogLayer(cmd)
                printJSON(cmd)
            elseif cmd.name == "moni_click_skillBtn" then
                cmd.clickTarget = self.guardFightBtnMoni1
                addGuardClickLayer(cmd)
            elseif cmd.name == "moni_click_skillBtn2" then
                cmd.clickTarget = self.guardFightBtnMoni2
                addGuardClickLayer(cmd)
            elseif cmd.name == "moni_click_skillBtn3" then
                cmd.clickTarget = self.guardFightBtnMoni3
                addGuardClickLayer(cmd)
            elseif cmd.name == "moni_click_skillBtn4" then
                cmd.clickTarget = self.guardFightBtnMoni4
                addGuardClickLayer(cmd)
            elseif cmd.name == "moni_click_skillBtn5" then
                cmd.clickTarget = self.guardFightBtnMoni5
                addGuardClickLayer(cmd)
            elseif cmd.name == "moni_click_skillBtn6" then
                cmd.clickTarget = self.guardFightBtnMoni6
                addGuardClickLayer(cmd)
            elseif cmd.name == "moni_dialog_bt3" then
                addGuardDialogLayer(cmd)
            elseif cmd.name == "moni_dialog_bt1" then
                cmd.tag = true
                addGuardDialogLayer(cmd)
                --Helper.setTimeout(addGuardDialogLayer(cmd), 10, function()directorAgent:resume()end)
            end
        end
    end
end

function UI2:isGuarding()
    return guardCenter:canEnterGuardMode("pve2") or guardCenter:canEnterGuardMode("pve")
        or guardCenter:canEnterGuardMode("moni")
end

return UI2


