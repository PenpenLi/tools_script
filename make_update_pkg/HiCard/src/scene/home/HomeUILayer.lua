--HomeUILayer.lua, created by php script. chris.li--
require "Cocos2d"

--m_BuyGoldUI: layer
local m_BuyGoldUI = nil

--m_BuyTiliUI: layer
local m_BuyTiliUI = nil

--m_BuyTipUI: layer
local m_BuyTipUI = nil

--m_SettingUI: layer
local m_SettingUI = nil

--m_FirstRechargeUI: layer
local m_FirstRechargeUI = nil

--m_ChatUI: layer
local m_ChatUI = nil

m_chatSJ={}
m_chatGH={}

nextSence = 1 -- 1为城镇，2为其他

local Roler = require("data.Roler")
local shopConfig = require("config.define_shop")
local PopDialog = require "common/PopDialog"

local HomeUILayer = class("HomeUILayer", function()
	return cc.Layer:create()
end)

function HomeUILayer:create()
	local layer = HomeUILayer.new()
	layer:initM()
	return layer
end

function HomeUILayer:ctor()
end

function HomeUILayer:onEnter()
    nextSence = 2
    
    Http:req("ToBuyCoins",{type = 1},function(res)
        self.m_myhasbuytilinum = tonumber(res.data["hasBuyCnt"])
        self.m_buytilinum = tonumber(res.data["maxBuyCnt"])-tonumber(res.data["hasBuyCnt"])
        self.m_buytilioneprice = tonumber(res.data["cost"])
        
        self:updateTipPic()
    end)
    
    self:refreshLabelUI()
    
end

function HomeUILayer:onExit()
    nextSence = 1
    cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId)
end

function HomeUILayer:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("HomeUILayer.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end
	
    --m_BuyGoldUI: layer
    m_BuyGoldUI = nil

    --m_BuyTiliUI: layer
    m_BuyTiliUI = nil

    --m_BuyTipUI: layer
    m_BuyTipUI = nil
    
    m_SettingUI = nil
    
    m_FirstRechargeUI = nil
    
    m_ChatUI = nil

	--m_chatBtn: Button
	self.m_chatBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_chatBtn")
	self.m_chatBtn:addTouchEventListener(touchEventHandler)

	--m_headImg: ImageView
	self.m_headImg = ccui.Helper:seekWidgetByName(self.rootNode, "m_headImg")
	
	

	--m_menuLayout: Layout
	self.m_menuLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_menuLayout")
	self.m_menuLayout:addTouchEventListener(touchEventHandler)

	--m_addStrengthBtn: Button
	self.m_addStrengthBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_addStrengthBtn")
	self.m_addStrengthBtn:addTouchEventListener(touchEventHandler)

	--m_addDiamondBtn: Button
	self.m_addDiamondBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_addDiamondBtn")
	self.m_addDiamondBtn:addTouchEventListener(touchEventHandler)
    self.m_addDiamondBtn:setTouchEnabled(false)

	--m_addCoinBtn: Button
	self.m_addCoinBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_addCoinBtn")
	self.m_addCoinBtn:addTouchEventListener(touchEventHandler)
	self.m_addCoinBtn:setTouchEnabled(false)
	
    --jinbipic: Image
    self.m_jinbipic = ccui.Helper:seekWidgetByName(self.rootNode, "jinbipic")
    self.m_jinbipic:addTouchEventListener(touchEventHandler)

    --zuanshipic: Image
    self.m_zuanshipic = ccui.Helper:seekWidgetByName(self.rootNode, "zuanshipic")
    self.m_zuanshipic:addTouchEventListener(touchEventHandler)

    --tilipic: Image
    self.m_tilipic = ccui.Helper:seekWidgetByName(self.rootNode, "tilipic")
    self.m_tilipic:addTouchEventListener(touchEventHandler)

	--m_strengthLabel: Text
	self.m_strengthLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_strengthLabel")
    self.m_strengthLabel:setString(Roler:get("energy").."/"..Roler:get("energyMax"))

	--m_diamondLabel: Text
	self.m_diamondLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_diamondLabel")
    self.m_diamondLabel:setString(Roler:get("gold"))

	--m_coinLabel: Text
	self.m_coinLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_coinLabel")
    self.m_coinLabel:setString(Roler:get("coins"))

	--m_headBtn: Button
	self.m_headBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_headBtn")
	self.m_headBtn:addTouchEventListener(touchEventHandler)
	

	--m_startServerActBtn: Button
	self.m_startServerActBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_startServerActBtn")
	self.m_startServerActBtn:addTouchEventListener(touchEventHandler)
    self.m_startServerActBtn:setVisible(false)

	--m_payBtn: Button
	self.m_payBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_payBtn")
	self.m_payBtn:addTouchEventListener(touchEventHandler)
	
    --m_shouchong: Button
    self.m_shouchong = ccui.Helper:seekWidgetByName(self.rootNode, "m_shouchong")
    self.m_shouchong:addTouchEventListener(touchEventHandler)
    self.m_shouchong:setVisible(false)

	--m_menuBtn: Button
	self.m_menuBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_menuBtn")
	self.m_menuBtn:addTouchEventListener(touchEventHandler)
	self.m_menuBtn:setTag(0)

	--m_menutiao: ImageView
	self.m_menutiao = ccui.Helper:seekWidgetByName(self.rootNode, "m_menutiao")

	--m_levelLabel: Text
	self.m_levelLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelLabel")
    self.m_levelLabel:setString(Roler:get("level"))

	--m_nameLabel: Text
	self.m_nameLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_nameLabel")
    self.m_nameLabel:setString(Roler:get("nickname"))
    
	--m_viplevelLabel: Text
	self.m_viplevelLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_viplevelLabel")
    self.m_viplevelLabel:setString(Roler:get("vipLevel"))

	--m_xinghunBtn: Button
	self.m_xinghunBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_xinghunBtn")
	self.m_xinghunBtn:addTouchEventListener(touchEventHandler)
	self.m_xinghunBtn:setVisible(false)

	--m_bagBtn: Button
	self.m_bagBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_bagBtn")
	self.m_bagBtn:addTouchEventListener(touchEventHandler)

	--m_dailyBtn: Button
	self.m_dailyBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_dailyBtn")
	self.m_dailyBtn:addTouchEventListener(touchEventHandler)

    --m_GemBtn: Button
    self.m_GemBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_GemBtn")
    self.m_GemBtn:addTouchEventListener(touchEventHandler)
	
    --Panel_max: Layout
    self.Panel_max = ccui.Helper:seekWidgetByName(self.rootNode, "Panel_max")
    self.Panel_max:setVisible(false)
    
    --Panel_normal: Layout
    self.Panel_normal = ccui.Helper:seekWidgetByName(self.rootNode, "Panel_normal")
    self.Panel_normal:setVisible(false)
    
    local timeinterval = ccui.Helper:seekWidgetByName(self.Panel_normal, "timeinterval")
    timeinterval:setString("恢复时间间隔："..Roler:get("recoverEnergyInterval").."分钟")
    
    --buttons: Layout
    local buttons = ccui.Helper:seekWidgetByName(self.rootNode, "buttons")
    
    self.kf_tip = ccui.Helper:seekWidgetByName(self.rootNode, "kf_tip")
    self.sc_tip = ccui.Helper:seekWidgetByName(self.rootNode, "sc_tip")
    self.rc_tip = ccui.Helper:seekWidgetByName(self.rootNode, "richang_tip")
    
    
    if nextSence == 1 then
        self.m_headImg:setVisible(true)
        self.m_menuLayout:setVisible(true)
        self.m_chatBtn:setVisible(true)
        buttons:setVisible(true)
       
    else
        self.m_headImg:setVisible(false)
        self.m_menuLayout:setVisible(false)
        self.m_chatBtn:setVisible(false)
        buttons:setVisible(false)
        
    end
    
    self.m_buytilinum = 0
    self.m_buyjinbinum = 0
    
    self.m_buytilioneprice = 0
    self.m_buyjinbioneprice = 0
    
    self.chattype = 0
    self.chatcount = 0
    
	self:initUI()
end

function HomeUILayer:updateTipPic()
    Http:req("GetRedDot",{multType = "1,64,256"},function(res)
        local reslist = res.data["dot"]

        for i = 1, table.getn(reslist) do
            if reslist[i]["type"] == 64 then
                if reslist[i]["visible"] == 0 then
                    self.sc_tip:setVisible(false)
                else
                    self.sc_tip:setVisible(true)
                end
            elseif reslist[i]["type"] == 256 then
                if reslist[i]["visible"] == 0 then
                    self.kf_tip:setVisible(false)
                else
                    self.kf_tip:setVisible(true)
                end
            elseif reslist[i]["type"] == 1 then
                if reslist[i]["visible"] == 0 then
                    self.rc_tip:setVisible(false)
                else
                    self.rc_tip:setVisible(true)
                end
            end
        end
    end)
end

function HomeUILayer:updateLabel()
    self.m_levelLabel:setString(Roler:get("level"))

    self.m_nameLabel:setString(Roler:get("nickname"))

    self.m_viplevelLabel:setString(Roler:get("vipLevel"))
    
    local energystring = string.format(tostring(Roler:get("energy")).."/"..tostring(Roler:get("energyMax")))
    if energystring ~= self.m_strengthLabel:getString() then
        self.m_strengthLabel:setString(Roler:get("energy").."/"..Roler:get("energyMax"))
        
        local st1 = cc.ScaleTo:create(0.4,1.3,1.3)
        local st2 = cc.ScaleTo:create(0.4,1.0,1.0)
        local seq = cc.Sequence:create(st1,st2)
        self.m_strengthLabel:runAction(seq)
    end

    
    if self.m_diamondLabel:getString() ~= tostring(Roler:get("gold")) then
        self.m_diamondLabel:setString(Roler:get("gold"))
        
        local st1 = cc.ScaleTo:create(0.4,1.3,1.3)
        local st2 = cc.ScaleTo:create(0.4,1.0,1.0)
        local seq = cc.Sequence:create(st1,st2)
        self.m_diamondLabel:runAction(seq)
    end
   
    if self.m_coinLabel:getString() ~= tostring(Roler:get("coins")) then
        self.m_coinLabel:setString(Roler:get("coins"))

        local st1 = cc.ScaleTo:create(0.4,1.3,1.3)
        local st2 = cc.ScaleTo:create(0.4,1.0,1.0)
        local seq = cc.Sequence:create(st1,st2)
        self.m_coinLabel:runAction(seq)
    end
    
    
    if Roler:get("firstRecharge") == 2 then
    	self.m_shouchong:setVisible(false)
    else
        self.m_shouchong:setVisible(true)
    end
    
    if Roler:get("openAct") == 0 then
        self.m_startServerActBtn:setVisible(false)
    else
        self.m_startServerActBtn:setVisible(true)
    end
end

function HomeUILayer:showFirstRechargeUI()
	if m_FirstRechargeUI == nil then
        m_FirstRechargeUI = ccs.GUIReader:getInstance():widgetFromBinaryFile("FirstRecharge.csb")
        self:addChild(m_FirstRechargeUI)
        
        local closebtn = ccui.Helper:seekWidgetByName(m_FirstRechargeUI, "closebtn")
        local firstbtn = ccui.Helper:seekWidgetByName(m_FirstRechargeUI, "firstbtn")
        
        local function btnCallBack(sender,event)
        	if sender == closebtn and event == cc.EventCode.ENDED then
                m_FirstRechargeUI:setVisible(false)
            elseif sender == firstbtn and event == cc.EventCode.ENDED then
                if Roler:get("firstRecharge") == 0 then
                    local scene = require("scene.shop.RechargeScene"):create()
                    cc.Director:getInstance():pushScene(scene)
                elseif Roler:get("firstRecharge") == 1 then
                    Http:req("GetFirstRechargeReward",nil,function(res)
                        if res.data["result"] == 0 then
                            Roler:set("firstRecharge",2)
                        end
                    end)
                end
                m_FirstRechargeUI:setVisible(false)
        	end
        end
        
        closebtn:addTouchEventListener(btnCallBack)
        firstbtn:addTouchEventListener(btnCallBack)
	end
	
    m_FirstRechargeUI:setVisible(true)
    
    local tablelist = {}
    local table1 = string.split(Roler:get("firstRechargeReward"),",")
    
    for i=1,table.getn(table1) do
        local table2 = string.split(table1[i],":")
        table.insert(tablelist, {id = table2[1],num = table2[2]})
    end
   
    local ListView = ccui.Helper:seekWidgetByName(m_FirstRechargeUI, "ListView")
    ListView:removeAllItems()
    
    for i = 1,table.getn(tablelist) do
        local item = ccs.GUIReader:getInstance():widgetFromBinaryFile("FirstRecharge_Package.csb")
        ListView:pushBackCustomItem(item)
        
        local num = ccui.Helper:seekWidgetByName(item, "num")
        num:setString(tablelist[i]["num"])
        local pic = ccui.Helper:seekWidgetByName(item, "pic")
        local pinzhi = ccui.Helper:seekWidgetByName(item, "pinzhi")
        
        if  shopConfig[tablelist[i]["id"]] ~= nil then
            local dataitem=shopConfig[tablelist[i]["id"]]
            pic:loadTexture(dataitem["icon"]..".png")
            pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
        end
    end
    
    local firstbtn = ccui.Helper:seekWidgetByName(m_FirstRechargeUI, "firstbtn")
    if Roler:get("firstRecharge") == 0 then
        firstbtn:loadTextures("firstrecharge/anniu01.png","firstrecharge/anniu02.png","firstrecharge/anniu02.png")
    elseif Roler:get("firstRecharge") == 1 then
        firstbtn:loadTextures("firstrecharge/anniu03.png","firstrecharge/anniu04.png","firstrecharge/anniu04.png")
    end
    
end

function HomeUILayer:refreshLabelUI()
    local nexttime = ccui.Helper:seekWidgetByName(self.Panel_normal, "nexttime")
    local alltime = ccui.Helper:seekWidgetByName(self.Panel_normal, "alltime")
    
    local tili_need = tonumber(Roler:get("energyMax"))-tonumber(Roler:get("energy"))
    
    tili_need = math.max(tili_need,0)
    
    local m_onetime = tonumber(Roler:get("recoverEnergyInterval"))*60

    
    local m_alltime = m_onetime*tili_need*60
    
    local function UpdateSome(dt)
        self:updateLabel()
        if m_onetime>0 then
            m_onetime = m_onetime-1
        else
            m_onetime = tonumber(Roler:get("recoverEnergyInterval"))*60
        end
        
        if m_alltime>0 then
            m_alltime = m_alltime-1
        else
            local tili_need = tonumber(Roler:get("energyMax"))-tonumber(Roler:get("energy"))

            tili_need = math.max(tili_need,0)
            m_alltime = m_onetime*tili_need*60
        end
        
        --下次体力时间
        
        local hours = math.floor(m_onetime/3600)
        local minute = math.floor((m_onetime-hours*3600)/60)
        local sec = m_onetime%60
        local updatetime = string.format("%02d:%02d:%02d",hours,minute,sec)
       

        nexttime:setString(updatetime)
       
        
        local hours2 = math.floor(m_alltime/3600)
        local minute2 = math.floor((m_alltime-hours*3600)/60)
        local sec2 = m_alltime%60
        local updatetime2 = string.format("%02d:%02d:%02d",hours2,minute2,sec2)

        
        alltime:setString(updatetime2)
        
    end
    self.ScriptFuncId = CCDirector:sharedDirector():getScheduler():scheduleScriptFunc(UpdateSome,1,false)
end

function HomeUILayer:openBuyTip(type)
    if m_BuyTipUI == nil then
        m_BuyTipUI = ccs.GUIReader:getInstance():widgetFromBinaryFile("BuyTipUI.csb")
        self:addChild(m_BuyTipUI,100)
        
        local Panel_tip1 = ccui.Helper:seekWidgetByName(m_BuyTipUI, "Panel_tip1")
        local Panel_tip2 = ccui.Helper:seekWidgetByName(m_BuyTipUI, "Panel_tip2")
        
        local closebtn1 = ccui.Helper:seekWidgetByName(Panel_tip1, "closebtn")
        local closebtn2 = ccui.Helper:seekWidgetByName(Panel_tip2, "closebtn")
        
        local govipbtn = ccui.Helper:seekWidgetByName(Panel_tip1, "govipbtn")
        local goRechargebtn = ccui.Helper:seekWidgetByName(Panel_tip2, "goRechargebtn")
        
        local function BuyTipUIbtnCallBack(sender,event)
            if (sender == closebtn1 or sender == closebtn2) and event == cc.EventCode.ENDED then
                m_BuyTipUI:setEnabled(false)
                m_BuyTipUI:setVisible(false)
            elseif sender == govipbtn and event == cc.EventCode.ENDED then
                local scene = require("scene.shop.RechargeScene") : create()
                cc.Director:getInstance():pushScene(scene)
            elseif sender == goRechargebtn and event == cc.EventCode.ENDED then
                local scene = require("scene.shop.RechargeScene") : create()
                cc.Director:getInstance():pushScene(scene)
            end
        end
        closebtn1:addTouchEventListener(BuyTipUIbtnCallBack)
        closebtn2:addTouchEventListener(BuyTipUIbtnCallBack)
        govipbtn:addTouchEventListener(BuyTipUIbtnCallBack)
        goRechargebtn:addTouchEventListener(BuyTipUIbtnCallBack)
    end
    m_BuyTipUI:setEnabled(true)
    m_BuyTipUI:setVisible(true)
    
    local Panel_tip1 = ccui.Helper:seekWidgetByName(m_BuyTipUI, "Panel_tip1")
    local Panel_tip2 = ccui.Helper:seekWidgetByName(m_BuyTipUI, "Panel_tip2")
    
    if type == "1" then
        Panel_tip2:setVisible(false)
        Panel_tip2:setEnabled(false)
    elseif type == "2" then
        Panel_tip1:setVisible(false)
        Panel_tip1:setEnabled(false)
    end
end

function HomeUILayer:RefreshBuyLayer(type)
    if type == 0 then
        if m_BuyGoldUI == nil then

            m_BuyGoldUI = ccs.GUIReader:getInstance():widgetFromBinaryFile("BuyGoldUI.csb")
            self:addChild(m_BuyGoldUI)

            local closebtn = ccui.Helper:seekWidgetByName(m_BuyGoldUI, "closebtn")
            local buy1btn = ccui.Helper:seekWidgetByName(m_BuyGoldUI, "buy1btn")
            local buy3btn = ccui.Helper:seekWidgetByName(m_BuyGoldUI, "buy3btn")
            local function BuyGoldUIBtnCallback(sender,event)
                if sender == closebtn and event == cc.EventCode.ENDED then
                    m_BuyGoldUI:setEnabled(false)
                    m_BuyGoldUI:setVisible(false)
                elseif sender == buy1btn and event == cc.EventCode.ENDED then
                    local buynum = 1
                    if buynum <= self.m_buytilinum then
                        self:showGetGold(buynum)               
                    else
                        self:openBuyTip("1")
                    end
                elseif sender == buy3btn and event == cc.EventCode.ENDED then
                    local buynum = 3
                    if buynum > self.m_buyjinbinum then
                        self:openBuyTip("1")             
                    elseif buynum*self.m_buyjinbioneprice > tonumber(Roler:get("gold")) then
                        self:openBuyTip("2")
                    else
                        self:showGetGold(buynum)
                    end
                end
            end

            closebtn:addTouchEventListener(BuyGoldUIBtnCallback)

            buy1btn:addTouchEventListener(BuyGoldUIBtnCallback)

            buy3btn:addTouchEventListener(BuyGoldUIBtnCallback)

        end

        m_BuyGoldUI:setEnabled(true)
        m_BuyGoldUI:setVisible(true)

        Http:req("ToBuyCoins",{type = 0},function(res)
            self.m_buyjinbinum = tonumber(res.data["maxBuyCnt"])-tonumber(res.data["hasBuyCnt"])
            self.m_buyjinbioneprice = tonumber(res.data["cost"])

            local numlabel = ccui.Helper:seekWidgetByName(m_BuyGoldUI, "numlabel")
            numlabel:setString(self.m_buyjinbinum.."/"..res.data["maxBuyCnt"])

            local zuanshi = ccui.Helper:seekWidgetByName(m_BuyGoldUI, "zuanshi")
            zuanshi:setString(res.data["cost"])

            local jinbi = ccui.Helper:seekWidgetByName(m_BuyGoldUI, "jinbi")
            jinbi:setString(res.data["gains"])

            local buynumlabel = ccui.Helper:seekWidgetByName(m_BuyGoldUI, "buynumlabel")
            buynumlabel:setString("今日还可以购买"..tonumber(res.data["maxBuyCnt"])-tonumber(res.data["hasBuyCnt"]).."次")


        end)        
    else
        if m_BuyTiliUI == nil then

            m_BuyTiliUI = ccs.GUIReader:getInstance():widgetFromBinaryFile("BuyTiliUI.csb")
            self:addChild(m_BuyTiliUI)

            local closebtn = ccui.Helper:seekWidgetByName(m_BuyTiliUI, "closebtn")
            local buy1btn = ccui.Helper:seekWidgetByName(m_BuyTiliUI, "buy1btn")
            local buy3btn = ccui.Helper:seekWidgetByName(m_BuyTiliUI, "buy3btn")
            local function BuyTiliUIBtnCallback(sender,event)
                if sender == closebtn and event == cc.EventCode.ENDED then
                    m_BuyTiliUI:setEnabled(false)
                    m_BuyTiliUI:setVisible(false)
                elseif sender == buy1btn and event == cc.EventCode.ENDED then
                    local buynum = 1
                    if buynum <= self.m_buytilinum then
                        Http:req("BuyCoins",{type = 1},function (res)
                            if res.data["result"] == 0 then
                                self:RefreshBuyLayer(1)
                            else
                                self:openBuyTip("2")
                            end
                        end)                    
                    else
                        self:openBuyTip("1")
                    end

                elseif sender == buy3btn and event == cc.EventCode.ENDED then
                    local buynum = 3
                    if buynum > self.m_buytilinum then
                        self:openBuyTip("1")             
                    elseif buynum*self.m_buytilioneprice > tonumber(Roler:get("gold")) then
                        self:openBuyTip("2")
                    else
                        for i=1,3 do
                            Http:req("BuyCoins",{type = 1},function (res)
                                if res.data["result"] == 0 then
                                    self:RefreshBuyLayer(1)
                                end
                            end)
                        end          
                    end
                end
            end

            closebtn:addTouchEventListener(BuyTiliUIBtnCallback)

            buy1btn:addTouchEventListener(BuyTiliUIBtnCallback)

            buy3btn:addTouchEventListener(BuyTiliUIBtnCallback)

        end

        m_BuyTiliUI:setEnabled(true)
        m_BuyTiliUI:setVisible(true)

        Http:req("ToBuyCoins",{type = 1},function(res)
            self.m_myhasbuytilinum = tonumber(res.data["hasBuyCnt"])
            self.m_buytilinum = tonumber(res.data["maxBuyCnt"])-tonumber(res.data["hasBuyCnt"])
            self.m_buytilioneprice = tonumber(res.data["cost"])

            local numlabel = ccui.Helper:seekWidgetByName(m_BuyTiliUI, "numlabel")
            numlabel:setString(self.m_buytilinum.."/"..res.data["maxBuyCnt"])

            local zuanshi = ccui.Helper:seekWidgetByName(m_BuyTiliUI, "zuanshi")
            zuanshi:setString(res.data["cost"])

            local tili = ccui.Helper:seekWidgetByName(m_BuyTiliUI, "tili")
            tili:setString(res.data["gains"])

            local buynumlabel = ccui.Helper:seekWidgetByName(m_BuyTiliUI, "buynumlabel")
            buynumlabel:setString("今日还可以购买"..tonumber(res.data["maxBuyCnt"])-tonumber(res.data["hasBuyCnt"]).."次")


        end)    
    end

end


function HomeUILayer:showGetGold(buynum)
    if buynum>0 then
        Http:req("BuyCoins",{type = 0},function (res)
            if res.data["result"] == 0 then
                local ShowBuyGoldUI = ccs.GUIReader:getInstance():widgetFromBinaryFile("ShowBuyGoldUI.csb")
                self:addChild(ShowBuyGoldUI)
                
                local isbaoji = ccui.Helper:seekWidgetByName(ShowBuyGoldUI, "isbaoji")
                
                local baojinum = ccui.Helper:seekWidgetByName(ShowBuyGoldUI, "baojinum")
                baojinum:setString(res.data["crit"])
                
                local x = 0
                if res.data["crit"] == 1 then
                	x = 100
                	isbaoji:setVisible(false)
                	baojinum:setVisible(false)
                else
                    isbaoji:setVisible(true)
                    baojinum:setVisible(true)
                end
                ShowBuyGoldUI:setPosition(cc.p(x,20))
                ShowBuyGoldUI:setVisible(false)
                
                local huafei = ccui.Helper:seekWidgetByName(ShowBuyGoldUI, "huafei")
                huafei:setString(self.m_buyjinbioneprice)
                
                local getgold = ccui.Helper:seekWidgetByName(ShowBuyGoldUI, "getgold")
                getgold:setString(res.data["gains"])
                
                local function removelayer(parameters)
                    self:removeChild(parameters)
                end
                local mt = cc.MoveTo:create(1,cc.p(x,150))
                local seq = cc.Sequence:create(cc.DelayTime:create((3-buynum)*0.5),cc.Show:create(),mt,cc.DelayTime:create(0.5),
                    cc.Hide:create(),cc.CallFunc:create(removelayer))
                ShowBuyGoldUI:runAction(seq)
                
                
                self:showGetGold(buynum-1)
                if buynum>0 then
                    self:RefreshBuyLayer(0)
                end
            
               
            else
                self:openBuyTip("2")
            end
        end)   
    end
end

function HomeUILayer:showChatUI(data)
    
    if m_ChatUI == nil then
        m_ChatUI = ccs.GUIReader:getInstance():widgetFromBinaryFile("ChatUI.csb")
        self:addChild(m_ChatUI,9999)
        
        local tap1 = ccui.Helper:seekWidgetByName(m_ChatUI, "tap1")
        tap1:loadTextures("chat/biaoqian_01.png","chat/biaoqian_01.png","chat/biaoqian_01.png")
        tap1:setEnabled(false)
        local tap2 = ccui.Helper:seekWidgetByName(m_ChatUI, "tap2")
        tap2:loadTextures("chat/biaoqian_02.png","chat/biaoqian_02.png","chat/biaoqian_02.png")
        
        
        local backbtn = ccui.Helper:seekWidgetByName(m_ChatUI, "backbtn")
        local sendbtn = ccui.Helper:seekWidgetByName(m_ChatUI, "sendbtn")
        
        local TextField = ccui.Helper:seekWidgetByName(m_ChatUI, "TextField")
        
        local function ChatBtnCallBack(sender, event)
        	if sender == tap1 and event == cc.EventCode.ENDED then
        		self.chattype = 0
                tap1:loadTextures("chat/biaoqian_01.png","chat/biaoqian_01.png","chat/biaoqian_01.png")
                tap1:setEnabled(false)
                tap2:loadTextures("chat/biaoqian_02.png","chat/biaoqian_02.png","chat/biaoqian_02.png")
                tap2:setEnabled(true)
                Http:req("OpenChat",{type = self.chattype},function(res)
                    self:updateChatUI(res.data)
                end)
            elseif sender == tap2 and event == cc.EventCode.ENDED then
                if Roler:get("guildId") == 0 then
                    PopDialog:getInstance():popText("未加入公会不能使用")
                	return
                end
                self.chattype = 1
                tap2:loadTextures("chat/biaoqian_01.png","chat/biaoqian_01.png","chat/biaoqian_01.png")
                tap2:setEnabled(false)
                tap1:loadTextures("chat/biaoqian_02.png","chat/biaoqian_02.png","chat/biaoqian_02.png")
                tap1:setEnabled(true)
                Http:req("OpenChat",{type = self.chattype},function(res)
                    self:updateChatUI(res.data)
                end)
            elseif sender == backbtn and event == cc.EventCode.ENDED then
                m_ChatUI:setVisible(false)
            
            elseif sender == sendbtn and event == cc.EventCode.ENDED then
    
                local chatstring = string.gsub(TextField:getStringValue(),"^%s*(.-)%s*$","%1")
                
                if string.len(chatstring) == 0 or self.chatcount <= 0 then
                	return
                end
                
                Http:req("Chat",{type = self.chattype,message = chatstring},function(res)
                    if res.data["result"] == 0 then
                        TextField:setText("")
                        Http:req("OpenChat",{type = self.chattype},function(res)
                            self:updateChatUI(res.data)
                        end)
                    end
                end)
        	end
        end
        
        tap1:addTouchEventListener(ChatBtnCallBack)
        tap2:addTouchEventListener(ChatBtnCallBack)
        backbtn:addTouchEventListener(ChatBtnCallBack)
        sendbtn:addTouchEventListener(ChatBtnCallBack)
        
        local function onkeyPressed(keycode, event)  
            if keycode == cc.KeyCode.KEY_BACKSPACE then
                
                local str = TextField:getStringValue()  
                str = string.sub( str, 0, string.len( str ) - 1 )  
                TextField:setText( str )    
                        
            end  
        end  
        local keyListener = cc.EventListenerKeyboard:create()  
        keyListener:registerScriptHandler(onkeyPressed,cc.Handler.EVENT_KEYBOARD_PRESSED)  
        local eventDispatcher = self:getEventDispatcher()  
        eventDispatcher:addEventListenerWithSceneGraphPriority(keyListener,self)  
    end
    
    m_ChatUI:setVisible(true)
    
    
    self:updateChatUI(data)
    
    
    
end

function HomeUILayer:updateChatUI(data)

    if self.chattype == 0 then
        for i = 1, table.getn(data["unit"]) do
            table.insert(m_chatSJ,data["unit"][i])
        end
        while table.getn(m_chatSJ)>10 do
            table.remove(m_chatSJ,1)
        end
    else
        for i = 1, table.getn(data["unit"]) do
            table.insert(m_chatGH,data["unit"][i])
        end
        while table.getn(m_chatGH)>10 do
            table.remove(m_chatGH,1)
        end
    end
    
    local mianfeinum = ccui.Helper:seekWidgetByName(m_ChatUI, "mianfeinum")
    mianfeinum:setString("免费"..data["freeCount"].."次")
    self.chatcount = data["freeCount"]

    local ListView = ccui.Helper:seekWidgetByName(m_ChatUI, "ListView")

    ListView:removeAllItems()

    if self.chattype == 0 then
        if table.getn(m_chatSJ) == 0 then
        	return
        end
        for i=table.getn(m_chatSJ), 1,-1 do
            local chatItem = ccui.Layout:create()
         
            local img = ccui.ImageView:create()
            img:loadTexture("chat/quan.png")
            chatItem:addChild(img)

            local levellabel = cc.Label:create()
            levellabel:setString(m_chatSJ[i]["level"])
            levellabel:setAnchorPoint(0.5,0.5)
            levellabel:setSystemFontSize(22)
            levellabel:setPosition(23,23)
            img:addChild(levellabel)

            local xian = ccui.ImageView:create()
            xian:setAnchorPoint(0,0)
            xian:loadTexture("chat/xian.png")
            chatItem:addChild(xian)

            local namelabel = cc.Label:create()
            namelabel:setSystemFontSize(22)
            namelabel:setAnchorPoint(0,1)
            namelabel:setString(m_chatSJ[i]["name"])
            chatItem:addChild(namelabel)

            local chatlabel = ccui.Text:create()
            chatlabel:setFontSize(22)
            chatlabel:setAnchorPoint(0,1)
            chatlabel:setString(m_chatSJ[i]["message"])
            chatItem:addChild(chatlabel)

            local labelheight = (math.floor(chatlabel:getStringLength()/24)+1)*25
            chatlabel:setContentSize(535,labelheight)
            chatlabel:ignoreContentAdaptWithSize(false)

            local Panelheight = labelheight+xian:getContentSize().height+namelabel:getContentSize().height+16

            xian:setPosition(4,0)
            img:setPosition(20,Panelheight/2)
            namelabel:setPosition(48,Panelheight-8)
            chatlabel:setPosition(48,Panelheight-40)

            chatItem:setContentSize(590,Panelheight)

            ListView:pushBackCustomItem(chatItem)
        end
    else
        if table.getn(m_chatGH) == 0 then
            return
        end
        for i=table.getn(m_chatGH), 1,-1 do
            local chatItem = ccui.Layout:create()
            chatItem:setAnchorPoint(0,0)
            local img = ccui.ImageView:create()
            img:loadTexture("chat/quan.png")
            chatItem:addChild(img)

            local levellabel = cc.Label:create()
            levellabel:setAnchorPoint(0.5,0.5)
            levellabel:setSystemFontSize(22)
            levellabel:setPosition(23,23)
            levellabel:setString(m_chatGH[i]["level"])
            img:addChild(levellabel)

            local xian = ccui.ImageView:create()
            xian:setAnchorPoint(0,0)
            xian:loadTexture("chat/xian.png")
            chatItem:addChild(xian)

            local namelabel = cc.Label:create()
            namelabel:setSystemFontSize(22)
            namelabel:setAnchorPoint(0,1)
            namelabel:setString(m_chatGH[i]["name"])
            chatItem:addChild(namelabel)

            local chatlabel = ccui.Text:create()
            chatlabel:setFontSize(22)
            chatlabel:setAnchorPoint(0,1)
            chatlabel:setString(m_chatGH[i]["message"])
            chatItem:addChild(chatlabel)

            local labelheight = (math.floor(chatlabel:getStringLength()/24)+1)*25
            chatlabel:setContentSize(535,labelheight)
            chatlabel:ignoreContentAdaptWithSize(false)

            local Panelheight = labelheight+xian:getContentSize().height+namelabel:getContentSize().height+16

            xian:setPosition(4,0)
            img:setPosition(20,Panelheight/2)
            namelabel:setPosition(48,Panelheight-8)
            namelabel:setPosition(48,Panelheight-40)

            chatItem:setContentSize(590,Panelheight)

            ListView:pushBackCustomItem(chatItem)
        end
    end

   
end

function HomeUILayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_chatBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_chatBtn touched--")
        Http:req("OpenChat",{type = self.chattype},function(res)
            self:showChatUI(res.data)
        end)
		
   
	elseif sender == self.m_menuLayout and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_menuLayout touched--")

    elseif (sender == self.m_addStrengthBtn or sender == self.tilipic) and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_addStrengthBtn touched--")
        self:RefreshBuyLayer(1)

	elseif sender == self.m_addDiamondBtn or sender == self.m_zuanshipic and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_addDiamondBtn touched--")
        local scene = require("scene.shop.RechargeScene"):create()
        cc.Director:getInstance():pushScene(cc.TransitionFadeTR:create(1,scene))
		

    elseif (sender == self.m_addCoinBtn or sender == self.m_jinbipic) and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_addCoinBtn touched--")
		
		self:RefreshBuyLayer(0)

	elseif sender == self.m_headBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_headBtn touched--")
		self:showSettingLayer()

	elseif sender == self.m_startServerActBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_startServerActBtn touched--")
        local scene = require("scene.openServer.openServer"):create()
        cc.Director:getInstance():replaceScene(cc.TransitionFadeTR:create(1,scene))
		
    elseif sender == self.m_shouchong and event == cc.EventCode.ENDED then
        cclog("[HomeUILayer]--m_shouchong touched--")
        self:showFirstRechargeUI()

	elseif sender == self.m_payBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_payBtn touched--")
        local scene = require("scene.shop.RechargeScene"):create()
        cc.Director:getInstance():pushScene(cc.TransitionFadeTR:create(1,scene))

	elseif sender == self.m_menuBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_menuBtn touched--")
        i = self.m_menuBtn:getTag()
        if i==0 then
            self.m_menuBtn:loadTextures("xia.png","xia.png","xia.png")
            self.m_menuBtn:setTag(1)
           
            self.m_menutiao:runAction(cc.ScaleTo:create(0.1,1,0))
        else
            self.m_menuBtn:loadTextures("shang.png","shang.png","shang.png")
            self.m_menuBtn:setTag(0)
            
            self.m_menutiao:runAction(cc.ScaleTo:create(0.1,1,1))
        end
		
		

	elseif sender == self.m_xinghunBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_xinghunBtn touched--")
        PopDialog:getInstance():popText("此功能暂未开放")

	elseif sender == self.m_bagBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_bagBtn touched--")
        local scene = require("scene.bag.BagScene"):create()
        cc.Director:getInstance():pushScene(cc.TransitionFadeTR:create(1,scene))
        
	elseif sender == self.m_dailyBtn and event == cc.EventCode.ENDED then
		cclog("[HomeUILayer]--m_dailyBtn touched--")
        local scene = require("scene.dailytask.DailyTaskScene"):create()
        cc.Director:getInstance():pushScene(cc.TransitionFadeTR:create(1,scene))

    elseif sender == self.m_GemBtn and event == cc.EventCode.ENDED then
        cclog("[HomeUILayer]--m_GemBtn touched--")
        local scene = require("scene.gem.GemScene"):create()
        cc.Director:getInstance():replaceScene(cc.TransitionFadeTR:create(1,scene))
		
    elseif sender == self.m_tilipic then
        cclog("[HomeUILayer]--m_tilipic touched--")
        self:showTiliPanel(event)
        
	end
end

function HomeUILayer:showGetList(data)
    local Panel_get = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_get")
    local ListView = ccui.Helper:seekWidgetByName(m_SettingUI, "ListView")
    ListView:removeAllItems()
    
    if data["coins"] >0  then
        local item = ccs.GUIReader:getInstance():widgetFromBinaryFile("Getlabel.csb")
        ListView:pushBackCustomItem(item)
        
        local label = ccui.Helper:seekWidgetByName(item, "label")
        local labelstring = string.format("金币*"..data["coins"])
        
        label:setString(labelstring)
    end
    
    if data["gold"] >0  then
        local item = ccs.GUIReader:getInstance():widgetFromBinaryFile("Getlabel.csb")
        ListView:pushBackCustomItem(item)

        local label = ccui.Helper:seekWidgetByName(item, "label")
        local labelstring = string.format("钻石*"..data["gold"])

        label:setString(labelstring)
    end
    
    for i = 1, table.getn(data["item"]) do
        local item = ccs.GUIReader:getInstance():widgetFromBinaryFile("Getlabel.csb")
        ListView:pushBackCustomItem(item)
        
        local label = ccui.Helper:seekWidgetByName(item, "label")
        
        local shopId = tostring(data["item"][i]["id"])
        if  shopConfig[shopId] ~= nil then
            local dataitem=shopConfig[shopId]
            
            local label = ccui.Helper:seekWidgetByName(item, "label")
            local labelstring = string.format(dataitem["name"].."*"..data["item"][i]["num"])

            label:setString(labelstring)
            
        end
    end
end

function HomeUILayer:showSettingLayer()
    if m_SettingUI == nil then
        m_SettingUI = ccs.GUIReader:getInstance():widgetFromBinaryFile("SettingUI_1.csb")
        self:addChild(m_SettingUI)
        
        local status = Roler:get("settingStatus")
        
        local Panel_get = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_get")
        local Panel_nametip = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_nametip")
        local Panel_jihuoma = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_jihuoma")
        local Panel_name = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_name")
        local Panel_setting = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_setting")
        local Panel_main = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_main")
        
        --主界面按钮
        local settingbtn = ccui.Helper:seekWidgetByName(Panel_main, "settingbtn")
        
        local jihuomabtn = ccui.Helper:seekWidgetByName(Panel_main, "jihuomabtn")
        
        local changebtn = ccui.Helper:seekWidgetByName(Panel_main, "changebtn")
        
        local closebtn = ccui.Helper:seekWidgetByName(Panel_main, "closebtn")
        
        --设置界面按钮
        local backbtn = ccui.Helper:seekWidgetByName(Panel_setting, "backbtn")
        
        
        --改名界面按钮
        local surebtn = ccui.Helper:seekWidgetByName(Panel_name, "surebtn")
        
        local canclebtn = ccui.Helper:seekWidgetByName(Panel_name, "canclebtn")
        
        local suijibtn = ccui.Helper:seekWidgetByName(Panel_name, "suijibtn")
        
        --激活码界面按钮
        local surebtn2 = ccui.Helper:seekWidgetByName(Panel_jihuoma, "surebtn")

        local canclebtn2 = ccui.Helper:seekWidgetByName(Panel_jihuoma, "canclebtn")
        --改名确认界面按钮
        local surebtn3 = ccui.Helper:seekWidgetByName(Panel_nametip, "surebtn")

        local canclebtn3 = ccui.Helper:seekWidgetByName(Panel_nametip, "canclebtn")

        
        local function SettingBtnCallBack(sender, event)
            if sender == Panel_get and event == cc.EventCode.ENDED then
                Panel_get:setVisible(false)
                
            elseif sender == settingbtn and event == cc.EventCode.ENDED then
                Panel_setting:setVisible(true)
                Panel_main:setVisible(false)
                
            elseif sender == jihuomabtn and event == cc.EventCode.ENDED then
                Panel_jihuoma:setVisible(true)
                Panel_main:setVisible(false)
                
            elseif sender == changebtn and event == cc.EventCode.ENDED then
                Panel_name:setVisible(true)
                Panel_main:setVisible(false)
                
            elseif sender == closebtn and event == cc.EventCode.ENDED then
                m_SettingUI:setVisible(false)
                
            elseif sender == backbtn and event == cc.EventCode.ENDED then      
                Roler:set("settingStatus",status)
                Http:req("SetTheSettingStatus",{settingStatus = status},function(res)
                    if res.data["result"] == 0 then
                        Panel_setting:setVisible(false)
                        Panel_main:setVisible(true)
                        Helper.PlayBackGroundMuisc("sound/background.mp3")
                    end
                end)
          
            elseif sender == surebtn and event == cc.EventCode.ENDED then
                Panel_nametip:setVisible(true)
                
                
            elseif sender == canclebtn and event == cc.EventCode.ENDED then
                Panel_name:setVisible(false)
                Panel_main:setVisible(true)
                local namelabel = ccui.Helper:seekWidgetByName(Panel_main, "namelabel")
                namelabel:setString(Roler:get("nickname"))
                
            elseif sender == surebtn2 and event == cc.EventCode.ENDED then
               
                Http:req("GetExchangeCodeReward",{excCode = TextField:getStringValue()},function(res)
                    if res.data["result"] == 0 then
                        self:showGetList(res.data["reward"])
                        
                        Panel_get:setVisible(true)
                    end
                end)

            elseif sender == canclebtn2 and event == cc.EventCode.ENDED then
                Panel_jihuoma:setVisible(false)
                Panel_main:setVisible(true)
                

            elseif sender == surebtn3 and event == cc.EventCode.ENDED then      
                local username = ccui.Helper:seekWidgetByName(Panel_name, "TextField")
                local jhstring = string.gsub(username:getStringValue(),"^%s*(.-)%s*$","%1")
                if string.len(jhstring) == 0 then
                    PopDialog:getInstance():popText("名字不能为空!")
                    return
                end
                Http:req("ChangeName",{name = username:getStringValue()},function(res)
                    if res.data["result"] == 0 then
                        PopDialog:getInstance():popText("成功修改名字!")
                        Panel_nametip:setVisible(false)
                    end
                end)

            elseif sender == canclebtn3 and event == cc.EventCode.ENDED then
                Panel_nametip:setVisible(false)

            elseif sender == suijibtn and event == cc.EventCode.ENDED then
                Http:req("RandomName",nil,function(res)
                    local username = ccui.Helper:seekWidgetByName(Panel_name, "TextField")
                    username:setText(res.data["user"])
                end)
            
            elseif sender == Panel_get and event == cc.EventCode.ENDED then
                Panel_get:setVisible(false)
            end
        end
        
        settingbtn:addTouchEventListener(SettingBtnCallBack)

        jihuomabtn:addTouchEventListener(SettingBtnCallBack)

        changebtn:addTouchEventListener(SettingBtnCallBack)

        closebtn:addTouchEventListener(SettingBtnCallBack)

        backbtn:addTouchEventListener(SettingBtnCallBack)

        
        for i=1,6 do
            local kai = ccui.Helper:seekWidgetByName(Panel_setting, "kai_"..i)
            kai:setTag(i)

            local guan = ccui.Helper:seekWidgetByName(Panel_setting, "guan_"..i)
            guan:setTag(i)
            
            if Roler:get("settingStatus")[i] == 1 then
            	kai:setVisible(true)
                
            else
                kai:setVisible(false)
                
            end
            
            local function kaiguan(sender, event)
            	if sender == kai and event == cc.EventCode.ENDED then
                    local tag = sender:getTag()
                    status[tag] = 0
                    if tag == 4 then
                        Helper.StopBackGroundMuisc()
                    end
                    sender:setVisible(false)
            	elseif sender == guan and event == cc.EventCode.ENDED then
                    local tag = sender:getTag()
                    status[tag] = 1
                    
                    local kai = ccui.Helper:seekWidgetByName(Panel_setting, "kai_"..tag)
                    kai:setVisible(true)
            	end
            end
            
            kai:addTouchEventListener(kaiguan)
            guan:addTouchEventListener(kaiguan)
        end

        surebtn:addTouchEventListener(SettingBtnCallBack)

        canclebtn:addTouchEventListener(SettingBtnCallBack)

        suijibtn:addTouchEventListener(SettingBtnCallBack)

        surebtn2:addTouchEventListener(SettingBtnCallBack)

        canclebtn2:addTouchEventListener(SettingBtnCallBack)

        surebtn3:addTouchEventListener(SettingBtnCallBack)

        canclebtn3:addTouchEventListener(SettingBtnCallBack)
        
        Panel_get:addTouchEventListener(SettingBtnCallBack)
    end
    m_SettingUI:setVisible(true)
    
    local Panel_get = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_get")
    local Panel_nametip = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_nametip")
    local Panel_jihuoma = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_jihuoma")
    local Panel_name = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_name")
    local Panel_setting = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_setting")
    local Panel_main = ccui.Helper:seekWidgetByName(m_SettingUI, "Panel_main")
    
    Panel_get:setVisible(false)
    Panel_nametip:setVisible(false)
    Panel_jihuoma:setVisible(false)
    Panel_name:setVisible(false)
    Panel_setting:setVisible(false)
    Panel_main:setVisible(true)
    
    local touxiang = ccui.Helper:seekWidgetByName(Panel_main, "touxiang")
    touxiang:loadTexture("soul/1.png")
    
    local namelabel = ccui.Helper:seekWidgetByName(Panel_main, "namelabel")
    namelabel:setString(Roler:get("nickname"))
    
    local levellabel = ccui.Helper:seekWidgetByName(Panel_main, "levellabel")
    local levelstring = ""
    levelstring = string.format("战队等级：Lv"..Roler:get("level"))
    levellabel:setString(levelstring)
    
    local jingyanlabel = ccui.Helper:seekWidgetByName(Panel_main, "jingyanlabel")
    local jingyanstring = ""
    jingyanstring = string.format("当前经验："..Roler:get("exp").."/"..Roler:get("needLvExp"))
    jingyanlabel:setString(jingyanstring)
end

function HomeUILayer:showTiliPanel(event)
    
    local tili_now = tonumber(Roler:get("energy"))
    local tili_max = tonumber(Roler:get("energyMax"))
    if event == cc.EventCode.BEGAN then
        if tili_now >= tili_max then
    		self.Panel_max:setVisible(true)
            self.Panel_normal:setVisible(false)
            
            local buynum = ccui.Helper:seekWidgetByName(self.Panel_max, "buynum")
            buynum:setString("已买体力次数："..self.m_myhasbuytilinum)
    	else
            self.Panel_max:setVisible(false)
            self.Panel_normal:setVisible(true)
            
            local buynum = ccui.Helper:seekWidgetByName(self.Panel_normal, "buynum")
            buynum:setString("已买体力次数："..self.m_myhasbuytilinum)
            
    	end
    elseif event == cc.EventCode.MOVED  then
    
    else
        self.Panel_max:setVisible(false)
        self.Panel_normal:setVisible(false)
    end
end

--init your ui here
function HomeUILayer:initUI()
end

--refresh all ui here
function HomeUILayer:refreshUI()
end

return HomeUILayer
