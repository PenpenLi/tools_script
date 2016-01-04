
--PickCard.lua, created by php script. chris.li--
require "Cocos2d"

local shopConfig = require("config.define_shop")

local soulConfig = require("config.define_soul")

local PopDialog = require "common/PopDialog"


local HomeUILayer = require "scene.home.HomeUILayer"

local cardlist = nil

local PickCard = class("PickCard", function()
	return cc.Layer:create()
end)

function PickCard:create()
	local layer = PickCard.new()
	layer:initM()
	return layer
end

function PickCard:ctor()
end

function PickCard:onEnter()
    self:refreshLabelUI()
    -- guard_tag --
    self:processGuard()
end

function PickCard:onExit()
    cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId)
end

function PickCard:processGuard()
    local function addGuardDialogLayer(cmd)
        cmd.type = "guard_dialog"
        cmd.cb = function() self:processGuard() end
        Helper.setTimeout(self, 1, function()
            self.guardLayer = require("common.Guard"):create(cmd)
            cc.Director:getInstance():getRunningScene():addChild(self.guardLayer)
        end)
    end

    local function addGuardClickLayer(cmd)
        assert(cmd.clickTarget~=nil,"")
        cmd.type = "guard_click"
        Helper.setTimeout(self, 1, function()
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

    if guardCenter:canEnterGuardMode("pickcard") == true then
        local cmd = guardCenter:getCmd("pickcard")
        if cmd == nil then return end
        if cmd.name == "pickcard_click_pickcardbtn" then
            cmd.clickTarget = self.m_pick1btn1
            addGuardClickLayer(cmd)
       -- elseif cmd.name == "pickcard_dialog_cardfunction" then
            --addGuardDialogLayer(cmd)
        elseif cmd.name == "pickcard_click_armaturebtn" then
            cmd.clickTarget = self.m_surebtn2
            addGuardClickLayer(cmd)
        elseif cmd.name == "pickcard_click_surebtn" then
            cmd.clickTarget = self.m_surebtn
            addGuardClickLayer(cmd)
        elseif cmd.name == "pickcard_click_closebtn" then
            cmd.clickTarget = self.m_closebtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("pickcard2") == true then
        local cmd = guardCenter:getCmd("pickcard2")
        if cmd == nil then return end
        if cmd.name == "pickcard2_click_pickcardbtn" then
            cmd.clickTarget = self.m_pick2btn1
            addGuardClickLayer(cmd)
        elseif cmd.name == "pickcard2_click_surebtn" then
            cmd.clickTarget = self.m_surebtn
            addGuardClickLayer(cmd)
        elseif cmd.name == "pickcard2_click_closebtn" then
            cmd.clickTarget = self.m_closebtn
            addGuardClickLayer(cmd)
        end
    end
end

function PickCard:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("PickCard.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end
	
    self.homeUILayer = HomeUILayer:create()
    self.rootNode:addChild(self.homeUILayer,1)

	--m_pickPanel: Layout
	self.m_pickPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_pickPanel")
	self.m_pickPanel:addTouchEventListener(touchEventHandler)
    self.rootNode:setBackGroundImage("ditu.jpg")

	--m_showPanel1: Layout
	self.m_showPanel1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_showPanel1")
	self.m_showPanel1:addTouchEventListener(touchEventHandler)
	self.m_showPanel1:setVisible(false)

	--m_showPanel2: Layout
	self.m_showPanel2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_showPanel2")
	self.m_showPanel2:addTouchEventListener(touchEventHandler)
	self.m_showPanel2:setVisible(false)
	
	local lizi = cc.Node:create()
    local ParticleSystemQuad = cc.ParticleSystemQuad:create("huise-effect.plist")
    ParticleSystemQuad:setPosition(cc.p(WIN_SIZE.width/4, WIN_SIZE.height/2))
    local ParticleSystemQuad2 = cc.ParticleSystemQuad:create("huise-effect.plist")
    ParticleSystemQuad2:setPosition(cc.p(WIN_SIZE.width/4*3, WIN_SIZE.height/2))
    lizi:addChild(ParticleSystemQuad)
    lizi:addChild(ParticleSystemQuad2)
    self.m_showPanel2:addChild(lizi,1)

	--m_pick1: ImageView
	self.m_pick1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick1")

	--m_pick2: ImageView
	self.m_pick2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick2")

	--m_closebtn: Button
	self.m_closebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closebtn")
	self.m_closebtn:addTouchEventListener(touchEventHandler)

	--m_pickagainbtn: Button
	self.m_pickagainbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_pickagainbtn")
	self.m_pickagainbtn:addTouchEventListener(touchEventHandler)

	--m_againcostpic: ImageView
	self.m_againcostpic = ccui.Helper:seekWidgetByName(self.rootNode, "m_againcostpic")

	--m_againcost: Text
	self.m_againcost = ccui.Helper:seekWidgetByName(self.rootNode, "m_againcost")

	--m_surebtn: Button
	self.m_surebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_surebtn")
	self.m_surebtn:addTouchEventListener(touchEventHandler)

	--m_pick1btn2: Button
	self.m_pick1btn2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick1btn2")
	self.m_pick1btn2:addTouchEventListener(touchEventHandler)

	--m_pick1gold2: Text
	self.m_pick1gold2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick1gold2")

	--m_pick1num: Text
	self.m_pick1num = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick1num")

	--m_pick1btn1: Button
	self.m_pick1btn1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick1btn1")
	self.m_pick1btn1:addTouchEventListener(touchEventHandler)

	--m_pick1gold1: Text
	self.m_pick1gold1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick1gold1")

	--m_pick1time: Text
	self.m_pick1time = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick1time")

	--m_pick2btn2: Button
	self.m_pick2btn2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick2btn2")
	self.m_pick2btn2:addTouchEventListener(touchEventHandler)

	--m_pick2diamond2: Text
	self.m_pick2diamond2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick2diamond2")

	--m_pick2num: Text
	self.m_pick2num = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick2num")

	--m_pick2btn1: Button
	self.m_pick2btn1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick2btn1")
	self.m_pick2btn1:addTouchEventListener(touchEventHandler)

	--m_pick2diamond1: Text
	self.m_pick2diamond1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick2diamond1")

	--m_pick2time: Text
	self.m_pick2time = ccui.Helper:seekWidgetByName(self.rootNode, "m_pick2time")

	--m_scrollview: ScrollView
	self.m_scrollview = ccui.Helper:seekWidgetByName(self.rootNode, "m_scrollview")
	self.m_scrollview:addTouchEventListener(touchEventHandler)

	--m_surebtn2: Button
	self.m_surebtn2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_surebtn2")
	self.m_surebtn2:addTouchEventListener(touchEventHandler)

	--m_tips: Text
	self.m_tips = ccui.Helper:seekWidgetByName(self.rootNode, "m_tips")

	--m_guangxiao: ImageView
	self.m_guangxiao = ccui.Helper:seekWidgetByName(self.rootNode, "m_guangxiao")

	--m_heronamepic: ImageView
	self.m_heronamepic = ccui.Helper:seekWidgetByName(self.rootNode, "m_heronamepic")
	
	self.picktime={0,0}

    self.m_pickagaintype = 1
    self.m_pickagainnum = 1
    
    self.nowtag = 1

	self:initUI()
end

function PickCard:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_pickPanel and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_pickPanel touched--")

	elseif sender == self.m_showPanel1 and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_showPanel1 touched--")

	elseif sender == self.m_showPanel2 and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_showPanel2 touched--")

	elseif sender == self.m_closebtn and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_closebtn touched--")
        if guardCenter:isGuilding() then self.m_closebtn:setEnabled(false) end
        local scene = require("scene.home.HomeScene"):create()
        cc.Director:getInstance():replaceScene(scene)
        
        -- guard_tag --
        guardCenter:jumpNextGuardStage()

	elseif sender == self.m_pickagainbtn and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_pickagainbtn touched--")
        local function hidefun1()
            self.m_showPanel1:setVisible(false)
        end
        
        local function hidefun2()
            self:PickCardResult(self.m_pickagaintype,self.m_pickagainnum)
        end
        self.m_showPanel1:runAction(cc.Sequence:create(cc.FadeOut:create(0.4),cc.CallFunc:create(hidefun1),cc.FadeIn:create(0),cc.CallFunc:create(hidefun2)))

        

	elseif sender == self.m_surebtn and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_surebtn touched--")
        if guardCenter:isGuilding() then self.m_surebtn:setEnabled(false) end
        local function hidefun()
			self.m_showPanel1:setVisible(false)
		end
        self.m_showPanel1:runAction(cc.Sequence:create(cc.FadeOut:create(0.4),cc.CallFunc:create(hidefun),cc.FadeIn:create(0)))
        
        -- guard_tag --
        self:processGuard()

	elseif sender == self.m_pick1btn2 and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_pick1btn2 touched--")
        self:PickCardResult(1,10)

	elseif sender == self.m_pick1btn1 and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_pick1btn1 touched--")
        if guardCenter:isGuilding() then 
            self.m_pick1btn1:setEnabled(false) 
            local function removeGuardLayer()
                if self.guardLayer then
                    cc.Director:getInstance():getRunningScene():removeChild(self.guardLayer)
                    self.guardLayer = nil
                end
            end
            removeGuardLayer()
        end
		self:PickCardResult(1,1)

	elseif sender == self.m_pick2btn2 and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_pick2btn2 touched--")
        self:PickCardResult(2,10)

	elseif sender == self.m_pick2btn1 and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_pick2btn1 touched--")
        self:PickCardResult(2,1)

	elseif sender == self.m_scrollview and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_scrollview touched--")

	elseif sender == self.m_surebtn2 and event == cc.EventCode.ENDED then
		cclog("[PickCard]--m_surebtn2 touched--")
		if guardCenter:isGuilding() then self.m_surebtn2:setEnabled(false) end
        local heroId = 0
        if cardlist[self.nowtag]["type"] == 0 then
            heroId = cardlist[self.nowtag]["dropId"]
        else
            heroId = cardlist[self.nowtag]["soulId"]
        end

        local dataitem=soulConfig[tostring(heroId)]
        for i =1,tonumber(dataitem["quality"]) do
            local xin = ccui.Helper:seekWidgetByName(self.m_showPanel2, "xingxing_"..i)
            self.m_showPanel2:removeChild(xin)
		end
		self.m_showPanel2:setVisible(false)
        self:showNextCard(self.nowtag)
        
        -- guard tag --
        self:processGuard()
	end
end

function PickCard:showNextCard(numberid)
    local nextnumber = numberid + 1
    self.nowtag = nextnumber
    
    local namelabel = nil
    local function isShowHero()
        namelabel:setVisible(true)
        self:isShowHero()
    end

    local function openTouch()
        self:openTouch()
    end
    
    if nextnumber < self.m_pickagainnum then
        local picItem = ccui.Helper:seekWidgetByName(self.m_scrollview, "card_"..nextnumber)
        local cardpic = ccui.Helper:seekWidgetByName(self.m_scrollview, "card_"..nextnumber)
        namelabel = ccui.Helper:seekWidgetByName(cardpic, "name")
        local picpoint = cc.p(cardpic:getPositionX(),cardpic:getPositionY())
        local action = MyDIYAction.fallStrikeAction2(cc.p(360,400),0,0.4,picpoint)
        local seq = cc.Sequence:create(cc.Show:create(),action,cc.CallFunc:create(isShowHero))

        picItem:runAction(seq)
    elseif nextnumber == self.m_pickagainnum then
        local picItem = ccui.Helper:seekWidgetByName(self.m_scrollview, "card_"..nextnumber)
        local cardpic = ccui.Helper:seekWidgetByName(self.m_scrollview, "card_"..nextnumber)
        namelabel = ccui.Helper:seekWidgetByName(cardpic, "name")
        local picpoint = cc.p(cardpic:getPositionX(),cardpic:getPositionY())
        local action = MyDIYAction.fallStrikeAction2(cc.p(360,400),0,0.4,picpoint)
        local seq = cc.Sequence:create(cc.Show:create(),action,cc.CallFunc:create(isShowHero),
            cc.CallFunc:create(openTouch))
        picItem:runAction(seq)
    end
end

function PickCard:PickCardResult(picktype,picknum)
    self.m_pickagaintype = picktype
    self.m_pickagainnum = picknum
    Http:req("PickCardComplete",{type = picktype,num = picknum},function(res)
        if res.data["result"] == 0 then
            self:refreshResultUI(picktype,picknum,res)
            
            if guardCenter:canEnterGuardMode("pickcard") == true then
                guardCenter:isJump2Stage()
            end
            -- guard_tag --
            Helper.setTimeout(self, 20, function()self:processGuard()end)

        elseif res.data["result"] == 1 then
            PopDialog:getInstance():popText("您的货币不足!")
        else
            PopDialog:getInstance():popText("程序错误!")
        end
    end)	
end

function PickCard:refreshResultUI(picktype,picknum,res)
    self.m_showPanel1:setVisible(true)
    self.m_pickagainbtn:setVisible(false)
    self.m_surebtn:setVisible(false)
--	if picktype == 1 then
--		self.m_againcostpic:loadTexture("jinbi.png")
--	else
        self.m_againcostpic:loadTexture("baoshi.png")
--	end
	
    if picknum == 1 then
        self.m_againcost:setString(res.data["pickGold"])
        self.m_pickagainbtn:setTitleText("再抽一次")
	else
        self.m_againcost:setString(res.data["tenPickGold"])
        self.m_pickagainbtn:setTitleText("再抽十次")
	end
    cardlist = res.data["drop"]
    self:refreshMainUI(res)
    self:showCard(picknum)
end

function PickCard:showCard(picknum)
    self.m_scrollview:removeAllChildren()
    local ScrovSize = self.m_scrollview:getInnerContainerSize()
    
    for i = 1, picknum do
        local cardItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("PickCard_Item.csb")
        self.m_scrollview:addChild(cardItem)
        cardItem:setAnchorPoint(0.5,0.5)
        cardItem:setName("card_"..i)
        cardItem:setVisible(false)
        
        cardItem:setPosition(cc.p(100+(i-1)%5*130,65+ScrovSize.height-150*(math.floor((i-1)/5)+1)))
        
        if picknum == 1 then
            cardItem:setPosition(cc.p(360,150))
        end
        
        local name = ccui.Helper:seekWidgetByName(cardItem, "name")
        local pic = ccui.Helper:seekWidgetByName(cardItem, "pic")
        local pinzhi = ccui.Helper:seekWidgetByName(cardItem, "pinzhi")
        local num = ccui.Helper:seekWidgetByName(cardItem, "num")
        local issuipian = ccui.Helper:seekWidgetByName(cardItem, "issuipian")
        name:setVisible(false)
        
        num:setString(cardlist[i]["dropNum"])
        if tonumber(cardlist[i]["soulId"]) >0 then
        	issuipian:setVisible(true)
        else
            issuipian:setVisible(false)
        end
        if cardlist[i]["type"] == 0 then
            if  soulConfig[tostring(cardlist[i]["dropId"])] ~= nil then
                local dataitem=soulConfig[tostring(cardlist[i]["dropId"])]
                pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                pic:loadTexture(dataitem["icon"]..".png")
                name:setString(dataitem["name"])
                
               
            end
        else
            if  shopConfig[tostring(cardlist[i]["dropId"])] ~= nil then
                local dataitem=shopConfig[tostring(cardlist[i]["dropId"])]
                pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                pic:loadTexture(dataitem["icon"]..".png")
                name:setString(dataitem["name"])
                
                if tonumber(dataitem["type"]) == 16 then
                    issuipian:setVisible(true)
                end
                
               
            end
        end
        
    end
    
    self.nowtag = 1   
    local firstpic = ccui.Helper:seekWidgetByName(self.m_scrollview, "card_1")
    local picpoint = cc.p(firstpic:getPositionX(),firstpic:getPositionY())
    local name = ccui.Helper:seekWidgetByName(firstpic, "name")
    local action = MyDIYAction.fallStrikeAction2(cc.p(360,400),0,0.4,picpoint)
    local seq =nil
    
    local function isShowHero()
        name:setVisible(true)
        self:isShowHero()
    end
    
    local function openTouch()
        self:openTouch()
    end
    if picknum == 1 then
        seq = cc.Sequence:create(cc.Show:create(),action,cc.CallFunc:create(isShowHero),
            cc.CallFunc:create(openTouch))
    else
        seq = cc.Sequence:create(cc.Show:create(),action,cc.CallFunc:create(isShowHero))
    end
    
    firstpic:runAction(seq)
    
end

function PickCard:isShowHero()
    local numberid = self.nowtag
    if cardlist[numberid]["type"] == 0 or (cardlist[numberid]["type"] == 1 and cardlist[numberid]["soulId"] > 0) then
        local heroId = 0
        if cardlist[numberid]["type"] == 0 then
            heroId = cardlist[numberid]["dropId"]
            self.m_tips:setVisible(false)
    	else
            heroId = cardlist[numberid]["soulId"]
            self.m_tips:setVisible(true)
            self.m_tips:setString("已拥有此英雄，即转化为"..cardlist[numberid]["dropNum"].."个碎片")
    	end
    	
        local dataitem=soulConfig[tostring(heroId)]
        
        local xin = ccui.Helper:seekWidgetByName(self.m_showPanel2, "xin")
        xin:setVisible(false)
       
        local ditu = ccui.Helper:seekWidgetByName(self.m_showPanel2, "Image_60")
        local point = cc.p(ditu:getContentSize().width/2,xin:getPositionY())
        point.x = point.x -30*(tonumber(dataitem["quality"])-1)-60
        print(point.x.."-"..point.y)
        for i = 1, tonumber(dataitem["quality"]) do
        	local xing = ccui.ImageView:create()
        	self.m_showPanel2:addChild(xing)
        	
        	xing:loadTexture("xing01.png")
        	xing:setName("xingxing_"..i)
        	
        	local action = MyDIYAction.fallStrikeAction(cc.p(point.x+50+60*i,point.y+100),0.2,0.2)
        	local seq = cc.Sequence:create(cc.DelayTime:create(0.1*i),cc.Show:create(),action)
            xing:runAction(seq)
        end
       
        self.m_showPanel2:setBackGroundColor(getColorByQuality(tonumber(dataitem["quality"])))
        
        local seriesid = tonumber(dataitem["series"])
        self.m_heronamepic:loadTexture("mingzi_"..seriesid..".png")
        
        local heropic = ccui.Helper:seekWidgetByName(self.m_showPanel2, "heropic")
        heropic:removeAllChildren()
        
        local armature = Helper.createNpcArmature(seriesid)
        armature:setPosition(65,75)
        armature:setScale(1.5)
        heropic:addChild(armature)
        
        self.m_guangxiao:runAction(cc.RepeatForever:create(cc.RotateBy:create(2,360)))
        
        self.m_showPanel2:setVisible(true)
        
    else
        self:showNextCard(numberid)
    end
end

function PickCard:openTouch()
    self.m_pickagainbtn:setVisible(true)
    self.m_surebtn:setVisible(true)
end

function PickCard:refreshMainUI(res)
    local data = res.data["card"]
    self.m_pick1gold1:setString(data[1]["pickGold"])
    self.m_pick1num:setString("免费"..data[1]["freeCount"].."次")
    self.picktime[1] = data[1]["coolDownTime"]
    self.m_pick1gold2:setString(data[1]["tenPickGold"])
    
    self.m_pick2diamond1:setString(data[2]["pickGold"])
    self.m_pick2diamond2:setString(data[2]["tenPickGold"])
    self.m_pick2num:setString("免费"..data[2]["freeCount"].."次")
    self.picktime[2] = data[2]["coolDownTime"]
    
    self.m_pick1time:setVisible(false)
    self.m_pick1num:setVisible(false)
    self.m_pick2time:setVisible(false)
    self.m_pick2num:setVisible(false)
    
end

function PickCard:refreshLabelUI()
    

    local function UpdateSome(dt)
        for i = 1, 2 do
            if self.picktime[i]>0 then
                self.picktime[i] = self.picktime[i]-1
            end
            
            local hours = math.floor(self.picktime[i]/3600)
            local minute = math.floor((self.picktime[i]-hours*3600)/60)
            local sec = self.picktime[i]%60
            local updatetime = string.format("%02d:%02d:%02d后免费",hours,minute,sec)
            
            if i == 1 then
                self.m_pick1time:setString(updatetime)
                if self.picktime[i] == 0 then
                    self.m_pick1time:setVisible(false)
                    self.m_pick1num:setVisible(true)
                else
                    self.m_pick1time:setVisible(true)
                    self.m_pick1num:setVisible(false)
                end
                
            else
                self.m_pick2time:setString(updatetime)
                if self.picktime[i] == 0 then
                    self.m_pick2time:setVisible(false)
                    self.m_pick2num:setVisible(true)
                else
                    self.m_pick2time:setVisible(true)
                    self.m_pick2num:setVisible(false)
                end
            end
            
            
             
        end

    end
    self.ScriptFuncId = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(UpdateSome,1,false)
end

--init your ui here
function PickCard:initUI()
    Http:req("MainPickCardStart",nil,function(res)      
        self:refreshMainUI(res)
        
    end)
    
    
end

--refresh all ui here
function PickCard:refreshUI()
end

return PickCard
