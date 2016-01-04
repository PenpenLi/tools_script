--HomeCityLayer.lua, created by php script. chris.li--
require "Cocos2d"

local PopDialog = require "common/PopDialog"

local HomeCityLayer = class("HomeCityLayer", function()
	return cc.Layer:create()
end)

function HomeCityLayer:create()
	local layer = HomeCityLayer.new()
	layer:initM()
	return layer
end

function HomeCityLayer:ctor()
end

function HomeCityLayer:onEnter()
    self.m_scrollView:scrollToPercentHorizontal(50,0,false)
    Helper.PlayBackGroundMuisc("sound/background.mp3")
    
    self:updateTipPic()
end

function HomeCityLayer:onExit()
end

function HomeCityLayer:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
 
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("HomeCityLayer.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_scrollView: ScrollView
	self.m_scrollView = ccui.Helper:seekWidgetByName(self.rootNode, "m_scrollView")
	self.m_scrollView:addTouchEventListener(touchEventHandler)
    

	--m_mailCity: Button
	self.m_mailCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_mailCity")
	self.m_mailCity:addTouchEventListener(touchEventHandler)

	--m_activityCity: Button
	self.m_activityCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_activityCity")
	self.m_activityCity:addTouchEventListener(touchEventHandler)

	--m_tieCity: Button
	self.m_tieCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_tieCity")
	self.m_tieCity:addTouchEventListener(touchEventHandler)

	--m_pvpCity: Button
    self.m_pvpCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_pvpCity")
	self.m_pvpCity:addTouchEventListener(touchEventHandler)

	--m_clubCity: Button
	self.m_clubCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_clubCity")
	self.m_clubCity:addTouchEventListener(touchEventHandler)

	--m_shopCity: Button
	self.m_shopCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_shopCity")
	self.m_shopCity:addTouchEventListener(touchEventHandler)

	--m_yewaiCity: Button
	self.m_yewaiCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_yewaiCity")
	self.m_yewaiCity:addTouchEventListener(touchEventHandler)

	--m_heroCity: Button
	self.m_heroCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_heroCity")
	self.m_heroCity:addTouchEventListener(touchEventHandler)

	--m_scienceCity: Button
	self.m_scienceCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_scienceCity")
	self.m_scienceCity:addTouchEventListener(touchEventHandler)

	--m_taskCity: Button
	self.m_taskCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_taskCity")
	self.m_taskCity:addTouchEventListener(touchEventHandler)

	--m_pickCardCity: Button
	self.m_pickCardCity = ccui.Helper:seekWidgetByName(self.rootNode, "m_pickCardCity")
	self.m_pickCardCity:addTouchEventListener(touchEventHandler)
	
    --layerPanel: Panel
    self.layerPanel = ccui.Helper:seekWidgetByName(self.rootNode, "layerPanel")
    
    self.task_tip = ccui.Helper:seekWidgetByName(self.rootNode, "task_tip")
    self.guild_tip = ccui.Helper:seekWidgetByName(self.rootNode, "guild_tip")
    self.mail_tip = ccui.Helper:seekWidgetByName(self.rootNode, "mail_tip")
    self.card_tip = ccui.Helper:seekWidgetByName(self.rootNode, "card_tip")
    self.pvp_tip = ccui.Helper:seekWidgetByName(self.rootNode, "pvp_tip")
    self.guild_tip:setVisible(false)
    
    self.guang_pve = ccui.Helper:seekWidgetByName(self.rootNode, "guang_pve")
    self.guang_pvp = ccui.Helper:seekWidgetByName(self.rootNode, "guang_pvp")
    self.guang_keji = ccui.Helper:seekWidgetByName(self.rootNode, "guang_keji")
    self.guang_huodong = ccui.Helper:seekWidgetByName(self.rootNode, "guang_huodong")
    self.guang_shop = ccui.Helper:seekWidgetByName(self.rootNode, "guang_shop")
    self.guang_card = ccui.Helper:seekWidgetByName(self.rootNode, "guang_card")
    self.guang_hero = ccui.Helper:seekWidgetByName(self.rootNode, "guang_hero")
    self.guang_tj = ccui.Helper:seekWidgetByName(self.rootNode, "guang_tj")
    self.guang_task = ccui.Helper:seekWidgetByName(self.rootNode, "guang_task")
    self.guang_gh = ccui.Helper:seekWidgetByName(self.rootNode, "guang_gh")
    self.guang_mail = ccui.Helper:seekWidgetByName(self.rootNode, "guang_mail")
    
    self.guang_pve:setVisible(false)
    self.guang_pvp:setVisible(false)
    self.guang_keji:setVisible(false)
    self.guang_huodong:setVisible(false)
    self.guang_shop:setVisible(false)
    self.guang_card:setVisible(false)
    self.guang_hero:setVisible(false)
    self.guang_tj:setVisible(false)
    self.guang_task:setVisible(false)
    self.guang_gh:setVisible(false)
    self.guang_mail:setVisible(false)
    
	self:initUI()
end

function HomeCityLayer:updateTipPic()
    Http:req("GetRedDot",{multType = "2,8,16,128,1024"},function(res)
        local reslist = res.data["dot"]
        
        for i = 1, table.getn(reslist) do
        	if reslist[i]["type"] == 2 then
        		if reslist[i]["visible"] == 0 then
        			self.card_tip:setVisible(false)
        		else
        		    self.card_tip:setVisible(true)
        		end
            elseif reslist[i]["type"] == 8 then
                if reslist[i]["visible"] == 0 then
                    self.mail_tip:setVisible(false)
                else
                    self.mail_tip:setVisible(true)
                end
            elseif reslist[i]["type"] == 16 then
                if reslist[i]["visible"] == 0 then
                    self.task_tip:setVisible(false)
                else
                    self.task_tip:setVisible(true)
                end
      --[[      elseif reslist[i]["type"] == 128 then
                if reslist[i]["visible"] == 0 then
                    self.guild_tip:setVisible(false)
                else
                    self.guild_tip:setVisible(true)
                end]]
            elseif reslist[i]["type"] == 1024 then
                if reslist[i]["visible"] == 0 then
                    self.pvp_tip:setVisible(false)
                else
                    self.pvp_tip:setVisible(true)
                end
        	end
        end
    end)
end

function HomeCityLayer:handleTouchEvent(sender, event)
    if event == cc.EventCode.ENDED or event == cc.EventCode.CANCELLED then
        self.guang_pve:setVisible(false)
        self.guang_pvp:setVisible(false)
        self.guang_keji:setVisible(false)
        self.guang_huodong:setVisible(false)
        self.guang_shop:setVisible(false)
        self.guang_card:setVisible(false)
        self.guang_hero:setVisible(false)
        self.guang_tj:setVisible(false)
        self.guang_task:setVisible(false)
        self.guang_gh:setVisible(false)
        self.guang_mail:setVisible(false)
    elseif event == cc.EventCode.BEGAN then
        if sender == self.m_mailCity then
            self.guang_mail:setVisible(true)
        elseif sender == self.m_activityCity then
            self.guang_huodong:setVisible(true)
        elseif sender == self.m_tieCity then
            self.guang_tj:setVisible(true)
        elseif sender == self.m_pvpCity then
            self.guang_pvp:setVisible(true)
        elseif sender == self.m_clubCity then
            self.guang_gh:setVisible(true)
        elseif sender == self.m_shopCity then
            self.guang_shop:setVisible(true)
        elseif sender == self.m_yewaiCity then
            self.guang_pve:setVisible(true)
        elseif sender == self.m_heroCity then
            self.guang_hero:setVisible(true)
        elseif sender == self.m_scienceCity then
            self.guang_keji:setVisible(true)
        elseif sender == self.m_taskCity then
            self.guang_task:setVisible(true)
        elseif sender == self.m_pickCardCity then
            self.guang_card:setVisible(true)
        end
    end
    
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_scrollView and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_scrollView touched--")

	elseif sender == self.m_mailCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_mailCity touched--")
        local scene = require("scene.mail.MailScene"):create()
        cc.Director:getInstance():pushScene(cc.TransitionFadeTR:create(1,scene))
	elseif sender == self.m_activityCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_activityCity touched--")
        if Roler:get("level") < 5 then
            PopDialog:getInstance():popText("活动5级开放")
            return
        end
        local scene = require("scene.movement.MovementScene"):create()
        cc.Director:getInstance():replaceScene(cc.TransitionFadeTR:create(1,scene))

	elseif sender == self.m_tieCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_tieCity touched--")
        local scene = require("scene.tiejiangpu.TiejiangpuScene"):create()
        cc.Director:getInstance():pushScene(cc.TransitionFadeTR:create(1,scene))

	elseif sender == self.m_pvpCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_pvpCity touched--")
        if Roler:get("level") < 18 then
            PopDialog:getInstance():popText("竞技场18级开放")
            return
        end
        local scene = require("scene.sport.SportScene"):create()
        cc.Director:getInstance():pushScene(cc.TransitionFadeTR:create(1,scene))

	elseif sender == self.m_clubCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_clubCity touched--")
        if Roler:get("level") < 30 then
            PopDialog:getInstance():popText("公会30级开放")
            return
        end
        local scene = require("scene.union.UnionScene"):create()
        cc.Director:getInstance():replaceScene(cc.TransitionFadeTR:create(1,scene))
        
	elseif sender == self.m_shopCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_shopCity touched--")
        local scene = require("scene.shop.ShopScene"):create()
        cc.Director:getInstance():pushScene(cc.TransitionFadeTR:create(1,scene))

	elseif sender == self.m_yewaiCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_yewaiCity touched--")
        local scene = require("scene.pve.PVEScene"):create()
        cc.Director:getInstance():replaceScene(cc.TransitionFadeTR:create(1,scene))
        
	elseif sender == self.m_heroCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_heroCity touched--")
        local scene = require("scene.newhero.HeroScene"):create()
        cc.Director:getInstance():replaceScene(cc.TransitionFadeTR:create(1,scene))
        

	elseif sender == self.m_scienceCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_scienceCity touched--")
        local scene = require("scene.smelt.SmeltScene"):create()
        cc.Director:getInstance():replaceScene(cc.TransitionFadeTR:create(1,scene))
 
	elseif sender == self.m_taskCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_taskCity touched--")
        local scene = require("scene.task.taskScene"):create()
        cc.Director:getInstance():replaceScene(cc.TransitionFadeTR:create(1,scene))

	elseif sender == self.m_pickCardCity and event == cc.EventCode.ENDED then
        cclog("[HomeCityLayer]--m_pickCardCity touched--")
        local scene = require("scene.pickCard.pickCardScene"):create()
        cc.Director:getInstance():replaceScene(cc.TransitionFadeTR:create(1,scene))
      
	end
end

--init your ui here
function HomeCityLayer:initUI()
    local parallaxNode = cc.ParallaxNode:create()
    parallaxNode:setAnchorPoint(0, 0)
    local bg1 = cc.Sprite:create("image/bg/home_bg1.png") --jinjing
    local bg2 = cc.Sprite:create("image/bg/home_bg2.png") --zhongjing
    local bg3 = cc.Sprite:create("image/bg/home_bg3.png") --far
    bg1:setAnchorPoint(0, 0)
    bg2:setAnchorPoint(0, 0)
    bg3:setAnchorPoint(0, 0)
    parallaxNode:addChild(bg3,0, cc.p(1.0,1.0), cc.p(0,0))
    parallaxNode:addChild(bg2,0, cc.p(0.7,1.0), cc.p(0,0))
    parallaxNode:addChild(bg1,0, cc.p(1.0,1.0), cc.p(0,0))
    self.layerPanel:addChild(parallaxNode, -1)

    --mail动画    
    Helper.addSpriteFrames("mail/mailAnimation.plist","mail/mailAnimation.png")
    local mailPic = cc.Sprite:createWithSpriteFrameName("mail_0.png")
    mailPic:setAnchorPoint(0,0)
    self.m_mailCity:addChild(mailPic)
 
    local mailPicframes = Helper.newFrames("mail_%d.png", 0, 8)
    local mailPicAnimation = Helper.newAnimation(mailPicframes, 0.1)
    local mailPicAnimate = cc.Animate:create(mailPicAnimation)
    mailPic:runAction(cc.RepeatForever:create(mailPicAnimate))
    
    --guild动画    
    Helper.addSpriteFrames("guild/guildAnimation.plist","guild/guildAnimation.png")
    local guildPic = cc.Sprite:createWithSpriteFrameName("guild_0.png")
    self.m_clubCity:addChild(guildPic)

    guildPic:setAnchorPoint(0.5,0.5)
    guildPic:setPosition(135,178.5)
    local guildPicframes = Helper.newFrames("guild_%d.png", 0, 8)
    local guildPicAnimation = Helper.newAnimation(guildPicframes, 0.1)
    local guildPicAnimate = cc.Animate:create(guildPicAnimation)
    guildPic:runAction(cc.RepeatForever:create(guildPicAnimate))
    
    --tiejiang动画    
    Helper.addSpriteFrames("tiejiang/tjAnimation.plist","tiejiang/tjAnimation.png")
    local tjPic = cc.Sprite:createWithSpriteFrameName("tj_0.png")
    self.m_tieCity:addChild(tjPic)

    tjPic:setAnchorPoint(0.5,0.5)
    tjPic:setPosition(130,174.5)
    local tjPicframes = Helper.newFrames("tj_%d.png", 0, 8)
    local tjPicAnimation = Helper.newAnimation(tjPicframes, 0.15)
    local tjPicAnimate = cc.Animate:create(tjPicAnimation)
    tjPic:runAction(cc.RepeatForever:create(tjPicAnimate))
    
    --hero动画    
    Helper.addSpriteFrames("hero/heroAnimation.plist","hero/heroAnimation.png")
    local heroPic = cc.Sprite:createWithSpriteFrameName("hero_0.png")
    self.m_heroCity:addChild(heroPic)

    heroPic:setAnchorPoint(0.5,0.5)
    heroPic:setPosition(232.5,92.5)
    local heroPicframes = Helper.newFrames("hero_%d.png", 0, 8)
    local heroPicAnimation = Helper.newAnimation(heroPicframes, 0.2)
    local heroPicAnimate = cc.Animate:create(heroPicAnimation)
    heroPic:runAction(cc.RepeatForever:create(heroPicAnimate))
    
    --task动画    
    Helper.addSpriteFrames("task/taskAnimation.plist","task/taskAnimation.png")
    local taskPic = cc.Sprite:createWithSpriteFrameName("task_0.png")
    taskPic:setAnchorPoint(0,0)
    self.m_taskCity:addChild(taskPic)

    local taskPicframes = Helper.newFrames("task_%d.png", 0, 9)
    local taskPicAnimation = Helper.newAnimation(taskPicframes, 0.15)
    local taskPicAnimate = cc.Animate:create(taskPicAnimation)
    taskPic:runAction(cc.RepeatForever:create(taskPicAnimate))
    
    --pve动画    
    Helper.addSpriteFrames("pve/pveAnimation.plist","pve/pveAnimation.png")
    local pvePic = cc.Sprite:createWithSpriteFrameName("pve_0.png")
    pvePic:setAnchorPoint(0.5,0.5)
    pvePic:setPosition(140,169.5)
    self.m_yewaiCity:addChild(pvePic)

    local pvePicframes = Helper.newFrames("pve_%d.png", 0, 8)
    local pvePicAnimation = Helper.newAnimation(pvePicframes, 0.1)
    local pvePicAnimate = cc.Animate:create(pvePicAnimation)
    pvePic:runAction(cc.RepeatForever:create(pvePicAnimate))
    
    --pickcard动画    
    Helper.addSpriteFrames("pickcard/pickcardAnimation.plist","pickcard/pickcardAnimation.png")
    local pickcardPic = cc.Sprite:createWithSpriteFrameName("pickcard_0.png")
    pickcardPic:setAnchorPoint(0.5,0.5)
    pickcardPic:setPosition(92,115.5)
    self.m_pickCardCity:addChild(pickcardPic)

    local pickcardPicframes = Helper.newFrames("pickcard_%d.png", 0, 8)
    local pickcardPicAnimation = Helper.newAnimation(pickcardPicframes, 0.1)
    local pickcardPicAnimate = cc.Animate:create(pickcardPicAnimation)
    pickcardPic:runAction(cc.RepeatForever:create(pickcardPicAnimate))
    
    --huodong动画    
    Helper.addSpriteFrames("huodong/huodongAnimation.plist","huodong/huodongAnimation.png")
    local huodongPic = cc.Sprite:createWithSpriteFrameName("huodong_0.png")
    huodongPic:setAnchorPoint(0,0)
    self.m_activityCity:addChild(huodongPic)

    local huodongPicframes = Helper.newFrames("huodong_%d.png", 0, 9)
    local huodongPicAnimation = Helper.newAnimation(huodongPicframes, 0.15)
    local huodongPicAnimate = cc.Animate:create(huodongPicAnimation)
    huodongPic:runAction(cc.RepeatForever:create(huodongPicAnimate))
    
    --shop动画    
    Helper.addSpriteFrames("shop/shopAnimation.plist","shop/shopAnimation.png")
    local shopPic = cc.Sprite:createWithSpriteFrameName("shop_0.png")
    shopPic:setAnchorPoint(0.5,0.5)
    shopPic:setPosition(14.5,101.5)
    self.m_shopCity:addChild(shopPic)

    local shopPicframes = Helper.newFrames("shop_%d.png", 0, 8)
    local shopPicAnimation = Helper.newAnimation(shopPicframes, 0.15)
    local shopPicAnimate = cc.Animate:create(shopPicAnimation)
    shopPic:runAction(cc.RepeatForever:create(shopPicAnimate))
   
    --pvp动画    
    Helper.addSpriteFrames("pvp/pvpAnimation.plist","pvp/pvpAnimation.png")
    local pvpPic1 = cc.Sprite:createWithSpriteFrameName("pvp1_0.png")
    pvpPic1:setAnchorPoint(0.5,0.5)
    pvpPic1:setPosition(154,63.5)
    self.m_pvpCity:addChild(pvpPic1)

    local pvpPic1frames = Helper.newFrames("pvp1_%d.png", 0, 8)
    local pvpPic1Animation = Helper.newAnimation(pvpPic1frames, 0.1)
    local pvpPic1Animate = cc.Animate:create(pvpPic1Animation)
    pvpPic1:runAction(cc.RepeatForever:create(pvpPic1Animate))
    
    local pvpPic2 = cc.Sprite:createWithSpriteFrameName("pvp2_0.png")
    pvpPic2:setAnchorPoint(0.5,0.5)
    pvpPic2:setPosition(141,171.5)
    self.m_pvpCity:addChild(pvpPic2)

    local pvpPic2frames = Helper.newFrames("pvp2_%d.png", 0, 8)
    local pvpPic2Animation = Helper.newAnimation(pvpPic2frames, 0.1)
    local pvpPic2Animate = cc.Animate:create(pvpPic2Animation)
    pvpPic2:runAction(cc.RepeatForever:create(pvpPic2Animate))
    
    --keji动画    
    Helper.addSpriteFrames("keji/kejiAnimation.plist","keji/kejiAnimation.png")
    local kejiPic1 = cc.Sprite:createWithSpriteFrameName("keji1_0.png")
    kejiPic1:setAnchorPoint(0.5,0.5)
    kejiPic1:setPosition(73,153.5)
    self.m_scienceCity:addChild(kejiPic1)

    local kejiPic1frames = Helper.newFrames("keji1_%d.png", 0, 8)
    local kejiPic1Animation = Helper.newAnimation(kejiPic1frames, 0.1)
    local kejiPic1Animate = cc.Animate:create(kejiPic1Animation)
    kejiPic1:runAction(cc.RepeatForever:create(kejiPic1Animate))

    local kejiPic2 = cc.Sprite:createWithSpriteFrameName("keji2_0.png")
    kejiPic2:setAnchorPoint(0.5,0.5)
    kejiPic2:setPosition(80,232.5)
--    self.m_scienceCity:addChild(kejiPic2)

    local kejiPic2frames = Helper.newFrames("keji2_%d.png", 0, 8)
    local kejiPic2Animation = Helper.newAnimation(kejiPic2frames, 0.08)
    local kejiPic2Animate = cc.Animate:create(kejiPic2Animation)
    
    local kejiPic2frames2 = Helper.newFrames("keji2_%d.png", 4,4)
    local kejiPic2Animation2 = Helper.newAnimation(kejiPic2frames2, 0.15)
    local kejiPic2Animate2 = cc.Animate:create(kejiPic2Animation2)

    local seq = cc.Sequence:create(kejiPic2Animate,kejiPic2Animate2)
 --   kejiPic2:runAction(cc.RepeatForever:create(seq))
    
    --飞艇
    local feiting = cc.Sprite:create("image/common/chuan.png")
    self.layerPanel:addChild(feiting,10);
    feiting:setPosition(2000,500)
    
    local mt = cc.MoveTo:create(19,cc.p(-50,500))
    local pl = cc.Place:create(cc.p(2000,500))
    local ftseq = cc.Sequence:create(mt,pl)
    feiting:runAction(cc.RepeatForever:create(ftseq))
end

--refresh all ui here
function HomeCityLayer:refreshUI()
end

return HomeCityLayer
