--SportUILayer.lua, created by php script. chris.li--
require "Cocos2d"
local Roler = require("data.Roler")
local sportShopConfig = require("config.define_sport_shop")
local shopConfig = require("config.define_shop")
local soulConfig = require("config.define_soul")
local formation = require("data.Formation")
local PopDialog = require "common/PopDialog"
local FormationUILayer =  require "scene.formation.FormationUILayer"
local SportUILayer = class("SportUILayer", function()
	return cc.Layer:create()
end)

function SportUILayer:create()
	local layer = SportUILayer.new()
	layer:initM()
	return layer
end

function SportUILayer:ctor()
end

function SportUILayer:onEnter()
    --self:refreshUI()
    print("onEnter")
end

function SportUILayer:onExit()
    if self.ScriptFuncId ~= nil then
        cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId)
    end
    print("onExit")
end

function SportUILayer:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("SportUILayer.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_SportRootLayout: Layout
	self.m_SportRootLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_SportRootLayout")
	self.m_SportRootLayout:addTouchEventListener(touchEventHandler)

	--m_playerNameLabel: Text
	self.m_playerNameLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_playerNameLabel")

	--m_rankListBtn: Button
	self.m_rankListBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_rankListBtn")
	self.m_rankListBtn:addTouchEventListener(touchEventHandler)

	--m_battleReportBtn: Button
	self.m_battleReportBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_battleReportBtn")
	self.m_battleReportBtn:addTouchEventListener(touchEventHandler)

	--m_exchangeBtn: Button
	self.m_exchangeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_exchangeBtn")
	self.m_exchangeBtn:addTouchEventListener(touchEventHandler)

	--m_rulesBtn: Button
	self.m_rulesBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_rulesBtn")
	self.m_rulesBtn:addTouchEventListener(touchEventHandler)

	--m_closeBtn: Button
	self.m_closeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeBtn")
	self.m_closeBtn:addTouchEventListener(touchEventHandler)
    
    
    
	--m_adjustBtn: Button
	self.m_adjustBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_adjustBtn")
	self.m_adjustBtn:addTouchEventListener(touchEventHandler)

	--m_posHero1Image: ImageView
	self.m_posHero1Image = ccui.Helper:seekWidgetByName(self.rootNode, "m_posHero1Image")

	--m_posHero2Image: ImageView
	self.m_posHero2Image = ccui.Helper:seekWidgetByName(self.rootNode, "m_posHero2Image")

	--m_posHero3Image: ImageView
	self.m_posHero3Image = ccui.Helper:seekWidgetByName(self.rootNode, "m_posHero3Image")

	--m_posHero4Image: ImageView
	self.m_posHero4Image = ccui.Helper:seekWidgetByName(self.rootNode, "m_posHero4Image")

	--m_posHero5Image: ImageView
	self.m_posHero5Image = ccui.Helper:seekWidgetByName(self.rootNode, "m_posHero5Image")

	--m_rankLabel: Text
	self.m_rankLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_rankLabel")
    
	--m_rankRewardLabel: Text
	self.m_rankRewardLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_rankRewardLabel")
	
    --m_rankLabel
    self.m_rankLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_rankLabel")
    
    --m_challengeListview: ListView
    self.m_challengeListview = ccui.Helper:seekWidgetByName(self.rootNode, "m_challengeListview")
    
    --m_defFormationListViev: ListView
    self.m_defFormationListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_defFormationListView")
    
     --m_fightPowerLabel: Text
    self.m_fightPowerLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_fightPowerLabel")
    
    --m_challengeCntLayout: Layout
    self.m_challengeCntLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_challengeCntLayout")
    
    --m_buyCntBtn: Button
    self.m_buyCntBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_buyCntBtn")
    self.m_buyCntBtn:addTouchEventListener(touchEventHandler)
    
    --m_refreshBtn: Button
    self.m_refreshBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_refreshBtn")
    self.m_refreshBtn:addTouchEventListener(touchEventHandler)
    
    --m_challengetimesLabel: Text
    self.m_challengetimesLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_challengetimesLabel")
    
    --m_challengeCDLayout: Layout
    self.m_challengeCDLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_challengeCDLayout")

    --m_challengeCDLabel: Text
    self.m_challengeCDLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_challengeCDLabel")

    --m_clearCDBtn: Button
    self.m_clearCDBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_clearCDBtn")
    self.m_clearCDBtn:addTouchEventListener(touchEventHandler)
    
    --tips begin---
    
    --m_tipsLayout: Layout
    self.m_tipsLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipsLayout")
    
    --m_tipsClearCntLayout: Layout
    self.m_tipsClearCntLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipsClearCntLayout")
    
    --m_tipsBuyCntLayout: Layout
    self.m_tipsBuyCntLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipsBuyCntLayout")
    
    --m_clearCntCostLabel: Text
    self.m_clearCntCostLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_clearCntCostLabel")
    
    --m_buyCntCostLabel: Text
    self.m_buyCntCostLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_buyCntCostLabel")
    
    --m_tipsconfirmBtn: Button
    self.m_tipsconfirmBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipsconfirmBtn")
    self.m_tipsconfirmBtn:addTouchEventListener(touchEventHandler)

    --m_tipscancelBtn: Button
    self.m_tipscancelBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipscancelBtn")
    self.m_tipscancelBtn:addTouchEventListener(touchEventHandler)
    
    --tips end--
    
    --m_challengeLayout --
    self.m_challengeLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_challengeLayout")
    self.m_challengeLayout:addTouchEventListener(touchEventHandler)
      
    self.m_RoleBgImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_RoleBgImage")
	self:initUI()
end

function SportUILayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_SportRootLayout and event == cc.EventCode.ENDED then
		cclog("[SportUILayer]--m_SportRootLayout touched--")

	elseif sender == self.m_rankListBtn and event == cc.EventCode.ENDED then
		cclog("[SportUILayer]--m_rankListBtn touched--")
        
        Http: req("SprotRankList",
            nil,
            function(res) 
                if res.err then cclog("err!!!") return end
                self.rankList = res.data["rankUnit"]
                self:showInnerLayout("rank")
         end)
        
	elseif sender == self.m_battleReportBtn and event == cc.EventCode.ENDED then
		cclog("[SportUILayer]--m_battleReportBtn touched--")
        Http: req("SportReportList",
            nil,
            function(res) 
                if res.err then cclog("err!!!") return end
                self.reportList = res.data["rpt"]
                self:showInnerLayout("report")
            end)
        
	elseif sender == self.m_exchangeBtn and event == cc.EventCode.ENDED then
		cclog("[SportUILayer]--m_exchangeBtn touched--")
        Http: req("SportStore",
            {type = 0},
            function(res) 
                if res.err then cclog("err!!!") return end
                self.exchangeStore = res.data
                self:showInnerLayout("exchange")
            end)

	elseif sender == self.m_rulesBtn and event == cc.EventCode.ENDED then
		cclog("[SportUILayer]--m_rulesBtn touched--")
        self:showInnerLayout("rule")
        
	elseif sender == self.m_closeBtn and event == cc.EventCode.ENDED then
		cclog("[SportUILayer]--m_closeBtn touched--")
        cc.Director:getInstance():popScene()
        
	elseif sender == self.m_adjustBtn and event == cc.EventCode.ENDED then
		cclog("[SportUILayer]--m_adjustBtn touched--")
        --FormationUILayer:setBeforeSceneType("sport_formation")
        local scene = require("scene.formation.FormationScene"):create({BattleType ="sport_formation"})
        cc.Director:getInstance():pushScene(scene)
        
    elseif sender == self.m_refreshBtn and event == cc.EventCode.ENDED then
        cclog("[SportUILayer]--m_refreshBtn touched--")
        self:refreshUI()
    
    elseif sender == self.m_buyCntBtn and event == cc.EventCode.ENDED then
        cclog("[SportUILayer]--m_buyCntBtn touched--")
        self.m_tipsLayout:setVisible(true)
        self.m_tipsBuyCntLayout:setVisible(true)
        self.m_tipsClearCntLayout:setVisible(false)
        self.m_tipsType = "buyCnt"
        
    elseif sender == self.m_clearCDBtn and event == cc.EventCode.ENDED then
        cclog("[SportUILayer]--m_clearCDBtn touched--")
        self.m_tipsLayout:setVisible(true)
        self.m_tipsClearCntLayout:setVisible(true)
        self.m_tipsBuyCntLayout:setVisible(false)
        self.m_tipsType = "clearCnt"
       
    elseif sender == self.m_tipsconfirmBtn and event == cc.EventCode.ENDED then
        self.m_tipsLayout:setVisible(false)
        if self.m_tipsType == "buyCnt" then
            Http: req("BuyChallengeCnt",
                nil,
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                        self.m_challengetimesLabel:setString(self.challengeTotalFreeCout.."/"..self.challengeTotalFreeCout)
                        --self.m_buyCntBtn:setVisible(false)
                        --self.m_refreshBtn:setVisible(true)
                        self.cnt = self.challengeTotalFreeCout
                        self:dealChallengeLabel()
                    end
                end)
        elseif self.m_tipsType == "clearCnt" then
            Http: req("SportClerCD",
                nil,
                function(res)
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                        if self.ScriptFuncId ~= nil then
                            cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId)
                        end
                        self.cd = 0 
                        self:dealChallengeLabel()
                    end             
                end)
        end
    elseif sender == self.m_tipscancelBtn and event == cc.EventCode.ENDED then
        self.m_tipsLayout:setVisible(false)
    elseif sender == self.m_challengeLayout and event == cc.EventCode.ENDED then
        self.m_challengeLayout:setVisible(false)
	end
end

--init your ui here
function SportUILayer:initUI()
    self.m_SportRootLayout:setBackGroundImage("jingjichangbeijing.jpg")
    
    Http: req("SportMainRank",
        nil,
        function(res) 
            if res.err then cclog("err!!!") return end
            self:showMainSport(res.data)
        end)
        
    local sprite = cc.Sprite:create("select0.png")
    sprite:setAnchorPoint(cc.p(0.3,0))
    sprite:setScale(0.6,0.6)
    self.m_RoleBgImage:addChild(sprite)

end

--refresh all ui here
function SportUILayer:refreshUI()
    Http: req("SportMainRank",
        nil,
        function(res) 
            if res.err then cclog("err!!!") return end
            self:showMainSport(res.data)
        end)
end


function SportUILayer:showMainSport(data)
    if  data == nil then return end
    self.m_rankLabel:setString(data["myRank"])
    self.m_challengetimesLabel:setString(data["totalFreeCount"]-data["usedFreeCount"].."/"..data["totalFreeCount"])
    self.m_rankRewardLabel:setString(data["myScore"])
    self.m_playerNameLabel:setString(Roler:get("nickname"))
    self.cd = data["leftCd"]
    self.cnt = data["totalFreeCount"]-data["usedFreeCount"]
    
    self.m_buyCntCostLabel:setString(data["sportBuyPrice"])
    self.m_clearCntCostLabel:setString(data["sportCdPrice"])
    
    self.challengeTotalFreeCout = data["totalFreeCount"]
    self.roleChallengeList = data["sporter"]
    self.defMatrix = data["defMatrix"]
    self:dealChallengeLabel()
    self:setFormationData() --保存阵型数据
    self:updateChallengeList() --更新挑战列表
    self:updateDefenceFormation()   --调整防守阵型
end

function SportUILayer:dealChallengeLabel()
    local cd = self.cd
    local cnt = self.cnt 
	if cd > 0 then
	   self.m_challengeCDLayout:setVisible(true)
       self.m_challengeCntLayout:setVisible(false)   
       --self.m_challengeCDLabel:setString(cd)
        local function UpdateCD(dt)
            cd = cd -1
            self.cd = cd
            if  cd>0 then 
                local str = string.format("%02d:%02d",math.floor(cd/60),math.floor(cd%60))
                self.m_challengeCDLabel:setString(str)
            else
                self.m_challengeCDLayout:setVisible(false)
                self.m_challengeCntLayout:setVisible(true)
                if self.ScriptFuncId ~= nil then
                    cc.Director:getInstance():getScheduler():unscheduleScriptEntry(self.ScriptFuncId)
                end   
            end
        end
       self.ScriptFuncId = cc.Director:getInstance():getScheduler():scheduleScriptFunc(UpdateCD,1,false)
    else
        self.m_challengeCDLayout:setVisible(false)
        self.m_challengeCntLayout:setVisible(true)   
        if  cnt == 0 then
            self.m_buyCntBtn:setVisible(true)
            self.m_refreshBtn:setVisible(false)
        else
            self.m_buyCntBtn:setVisible(false)
            self.m_refreshBtn:setVisible(true)
        end
	end
end

function SportUILayer:setFormationData()
    local Formation = require "data.Formation"
    local defmatrix = {}
    local fightPower = 0
    if  self.defMatrix == nil then print("nil") return end
    for i=1,table.getn(self.defMatrix) do
        table.insert(defmatrix,self.defMatrix[i]["soulId"])
        fightPower = self.defMatrix[i]["fc"] + fightPower
    end
    Formation:set("sport_formation",defmatrix)
    Formation:set("fightpower",tostring(fightPower))

end
--[[type: rank
          report
          exchange
          rule
                  
]]

function SportUILayer:updateChallengeList()
    if  self.m_challengeListview:getChildrenCount() ~= 0 then 
        self.m_challengeListview:removeAllItems()
    end
    local chanllengeList = self.roleChallengeList
    for i=1,table.getn(chanllengeList) do
        local challengeItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("SportChallengeItem.csb")
        local m_challengeItemLayout = ccui.Helper:seekWidgetByName(challengeItem, "m_challengeItemLayout")
        local m_BgImage = ccui.Helper:seekWidgetByName(challengeItem, "m_BgImage")
        local m_rankLabel = ccui.Helper:seekWidgetByName(challengeItem, "m_rankLabel")
        local m_picImage = ccui.Helper:seekWidgetByName(challengeItem, "m_picImage")
        local m_nameLabel = ccui.Helper:seekWidgetByName(challengeItem, "m_nameLabel")
        local m_challengeBtn = ccui.Helper:seekWidgetByName(challengeItem, "m_challengeBtn")
        local m_levelLabel = ccui.Helper:seekWidgetByName(challengeItem, "m_levelLabel")
        local m_fightPowerLabel = ccui.Helper:seekWidgetByName(challengeItem, "m_fightPowerLabel")
        local dataItem = chanllengeList[i]
        m_rankLabel:setString(dataItem["rank"])
        m_nameLabel:setString(dataItem["name"])
        m_levelLabel:setString(dataItem["level"])
        m_fightPowerLabel:setString(dataItem["fc"])
        
        local micon = dataItem["icon"]
        if  micon > 30 then micon = micon%30 end
        m_picImage:loadTexture("touxiang_pic_soulicon"..micon..".png")
        
        local function challengeFunc(sender,event)
            if sender ==m_challengeBtn and event == cc.EventCode.ENDED then
                
                if  self.cnt == 0 then PopDialog:getInstance():popText("挑战次数已用完",cc.c3b(0,255,0)) return end
                if  self.cd > 0 then PopDialog:getInstance():popText("竞技场挑战CD中",cc.c3b(0,255,0)) return end
                
                --FormationUILayer:setBeforeSceneType("sport_battle")
                local data = {id = dataItem["id"],robot = dataItem["robot"],matrix ={},rank = dataItem["rank"]}
                local Formation = require "data.Formation"
                Formation:set("battledata",data)
                local scene = require("scene.formation.FormationScene"):create({BattleType="sport_battle"})
                cc.Director:getInstance():pushScene(scene)

            end
        end
        m_challengeBtn:addTouchEventListener(challengeFunc)
        
        local function showChallengeLayout(sender,event)
            if sender ==challengeItem and event == cc.EventCode.ENDED then
                self.m_challengeLayout:setVisible(true)
                local m_challengeRankLabel = ccui.Helper:seekWidgetByName(self.m_challengeLayout, "m_challengeRankLabel")
                local m_challengeWinCntLabel = ccui.Helper:seekWidgetByName(self.m_challengeLayout, "m_challengeWinCntLabel")
                local m_challengeFightPowerLabel = ccui.Helper:seekWidgetByName(self.m_challengeLayout, "m_challengeFightPowerLabel")
                local m_challengeGuildLabel = ccui.Helper:seekWidgetByName(self.m_challengeLayout, "m_challengeGuildLabel")
                local m_picImage = ccui.Helper:seekWidgetByName(self.m_challengeLayout, "m_picImage")
                local m_levelLabel = ccui.Helper:seekWidgetByName(self.m_challengeLayout, "m_levelLabel")
                local m_nameLabel = ccui.Helper:seekWidgetByName(self.m_challengeLayout, "m_nameLabel")
                
                m_challengeRankLabel:setString(dataItem["rank"])
                --m_challengeWinCntLabel:setString(0)
                m_challengeFightPowerLabel:setString(dataItem["fc"])
                m_challengeGuildLabel:setString(dataItem["guildName"])
                m_levelLabel:setString(dataItem["level"])
                m_nameLabel:setString(dataItem["name"])
                
                local micon = dataItem["icon"]
                if  micon > 30 then micon = micon%30 end
                m_picImage:loadTexture("touxiang_pic_soulicon"..micon..".png")
                
                for j=1,#dataItem["matrix"] do
                    local mHeroImage = ccui.Helper:seekWidgetByName(self.m_challengeLayout, "mHeroImage"..j)
                    mHeroImage:setVisible(true)
                    
                    local pinzhiImage = ccui.Helper:seekWidgetByName(mHeroImage, "pinzhiImage")

                    local soulId = dataItem["matrix"][j]["id"]
                    local soulItem = soulConfig[tostring(soulId)]

                    local finalSoulId = soulId %30
                    if finalSoulId == 0 then
                        finalSoulId = 30
                    end
                    mHeroImage:loadTexture("soul_pic_soulicon"..finalSoulId..".png")
                    pinzhiImage:loadTexture("normal_aochao_0"..soulItem["quality"]..".png")
                end
                
            end
        end
        challengeItem:addTouchEventListener(showChallengeLayout)
        
        self.m_challengeListview:pushBackCustomItem(challengeItem)
    end   
end

function SportUILayer:updateExchangeList()
    local showNode = self.exchangeNode
    
    local m_exchangeLayout = ccui.Helper:seekWidgetByName(showNode, "m_exchangeLayout")
    local m_propsListView = ccui.Helper:seekWidgetByName(showNode, "m_propsListView")
    local m_refreshBtn = ccui.Helper:seekWidgetByName(showNode, "m_refreshBtn")
    local m_cancelBtn = ccui.Helper:seekWidgetByName(showNode, "m_cancelBtn")
    local m_gongxianbiLabel = ccui.Helper:seekWidgetByName(showNode, "m_gongxianbiLabel")
    local BgImage = ccui.Helper:seekWidgetByName(showNode, "BgImage")
       
    --m_tipsLayout
    local m_tipsLayout = ccui.Helper:seekWidgetByName(showNode, "m_tipsLayout")
    local m_tipsBuyLayout = ccui.Helper:seekWidgetByName(m_tipsLayout, "m_tipsBuyLayout")
    local m_buyItemNameLabel = ccui.Helper:seekWidgetByName(m_tipsLayout, "m_buyItemNameLabel")
    local m_tipsRefreshLayout = ccui.Helper:seekWidgetByName(m_tipsLayout, "m_tipsRefreshLayout")
    local m_buyItemCostLabel = ccui.Helper:seekWidgetByName(m_tipsLayout, "m_buyItemCostLabel")
    local m_tipsconfirmBtn = ccui.Helper:seekWidgetByName(m_tipsLayout, "m_tipsconfirmBtn")
    local m_tipscancelBtn = ccui.Helper:seekWidgetByName(m_tipsLayout, "m_tipscancelBtn")
    local m_refreshCntLabel = ccui.Helper:seekWidgetByName(m_tipsLayout, "m_refreshCntLabel")
    --local colorLayer = cc.LayerColor:create(cc.c4b(0,0,0,100))

    
    local function closeself(sender, event)
        if sender == m_cancelBtn and event == cc.EventCode.ENDED then
            showNode:removeFromParent()
        end
    end
    m_cancelBtn:addTouchEventListener(closeself)   
    
    local tipsType = nil -- 0-兑换 1-刷新
    local function tipsConfirmFunc(sender, event)
        if sender == m_tipsconfirmBtn and event == cc.EventCode.ENDED then
           if  tipsType == 0 then 
                Http: req("BuySportItem",
                {storeId = m_tipsconfirmBtn:getTag()},
                function(res)
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                        m_tipsLayout:setVisible(false)
                        m_tipsBuyLayout:setVisible(false)
                        Http: req("SportStore",
                            {type = 0},
                            function(res)
                                if res.err then cclog("err!!!") return end
                                if  self:checkResult(res.data["result"]) then
                                    self.exchangeStore = res.data
                                    self:updateExchangeList()
                                    m_tipsLayout:setVisible(false)
                                    m_tipsRefreshLayout:setVisible(false)
                                end                               
                            end)
                    end
                end)
            elseif tipsType == 1 then
                Http: req("SportStore",
                    {type = 1},
                    function(res)
                        if res.err then cclog("err!!!") return end
                        if  self:checkResult(res.data["result"]) then
                            self.exchangeStore = res.data
                            self:updateExchangeList()
                            m_tipsLayout:setVisible(false)
                            m_tipsRefreshLayout:setVisible(false)
                         end
                    end)
            end
        end
    end
    m_tipsconfirmBtn:addTouchEventListener(tipsConfirmFunc)
    
    local function tipsCancelFunc(sender, event)
    	if sender == m_tipscancelBtn and event == cc.EventCode.ENDED then
    	   m_tipsLayout:setVisible(false)
           m_tipsRefreshLayout:setVisible(false)
           m_tipsBuyLayout:setVisible(false)
    	end
    end
    m_tipscancelBtn:addTouchEventListener(tipsCancelFunc)
    
    local function refreshFunc(sender, event)
        if sender == m_refreshBtn and event == cc.EventCode.ENDED then
            tipsType = 1
            m_tipsLayout:setVisible(true)
            m_tipsRefreshLayout:setVisible(true)
            m_buyItemCostLabel:setString(self.exchangeStore["refreshCost"])
            m_refreshCntLabel:setString(self.exchangeStore["refreshNum"].."/"..self.exchangeStore["refreshMax"])
        end
    end
    
    m_refreshBtn:addTouchEventListener(refreshFunc)
    
    m_exchangeLayout:setBackGroundImage("jingjichangbeijing.jpg") 
    m_gongxianbiLabel:setString(self.exchangeStore["score"])
    --[[
    optional int32 score=2;//拥有积分
    repeated SportStoreItem item=3;//物品
    optional int32 refreshCost=4;//刷新需花费N积分
    optional int32 refreshNum=5;//已刷新次数
    optional int32 refreshMax=6;//刷新次数上限
    ]]
    
    if  m_propsListView:getChildrenCount() ~= 0 then 
        m_propsListView:removeAllItems()
    end
    local exchangeList = self.exchangeStore["item"]
    for i=1,table.getn(exchangeList) do
        local exchangeItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("SporExchangeItem.csb")

        local m_exchangeItemLayout = ccui.Helper:seekWidgetByName(exchangeItem, "m_exchangeItemLayout")
        local m_HeroImage = ccui.Helper:seekWidgetByName(exchangeItem, "m_HeroImage")
        local m_propsNameLabel = ccui.Helper:seekWidgetByName(exchangeItem, "m_propsNameLabel")
        --local m_descLabel = ccui.Helper:seekWidgetByName(exchangeItem, "m_descLabel")
        local m_exchangeBtn = ccui.Helper:seekWidgetByName(exchangeItem, "m_exchangeBtn")
        local m_pinzhiImage = ccui.Helper:seekWidgetByName(exchangeItem, "m_pinzhiImage")
        local m_hasNumLabel = ccui.Helper:seekWidgetByName(exchangeItem, "m_hasNumLabel")
        local m_buyNumLabel = ccui.Helper:seekWidgetByName(exchangeItem, "m_buyNumLabel")
        local m_soldOutLayout = ccui.Helper:seekWidgetByName(exchangeItem, "m_soldOutLayout")
        local m_gongxianbiCostLabel = ccui.Helper:seekWidgetByName(exchangeItem, "m_gongxianbiCostLabel")
        local m_suipianImage = ccui.Helper:seekWidgetByName(exchangeItem, "m_suipianImage")
        
        local storeId = exchangeList[i]["storeId"]
        
       -- m_HeroImage:loadTexture("soul_pic_soulicon"..storeId..".png")
        m_hasNumLabel:setString(exchangeList[i]["ownNum"])
        --m_buyNumLabel:setString(exchangeList[i]["leftNum"])
        if  exchangeList[i]["leftNum"] == 0 then
            m_soldOutLayout:setVisible(true)
        end
        
        local stordName = nil
        if  sportShopConfig[tostring(storeId)] ~= nil then
            local sportData=sportShopConfig[tostring(storeId)]
            m_buyNumLabel:setString(sportData["per_num"])
            m_gongxianbiCostLabel:setString(sportData["per_cost"])
            local shopid = sportData["item_id"]
            if  shopConfig[tostring(shopid)] ~= nil then
                local shopData=shopConfig[tostring(shopid)]
                m_pinzhiImage:loadTexture("normal_aochao_0"..shopData["quality"]..".png")
                m_HeroImage:loadTexture(shopData["icon"]..".png")
                stordName = shopData["name"]
                m_propsNameLabel:setString(shopData["name"])
                --m_descLabel:setString(shopData["desc"])
                if  shopData["type"] == 16 then
                    m_suipianImage:setVisible(true)
                end
            end
        end
        
        local function exchangeFunc()
            tipsType = 0
            m_tipsconfirmBtn:setTag(storeId)
            m_tipsLayout:setVisible(true)
            m_tipsBuyLayout:setVisible(true)
            m_buyItemNameLabel:setString(stordName)
        end
        m_exchangeBtn:addTouchEventListener(exchangeFunc)
        
          
        m_propsListView:pushBackCustomItem(exchangeItem)
    end
end

function SportUILayer:showInnerLayout(type)
    if type == "rank" then --排行榜
        self:showBattleRank()      
    elseif type == "report" then --战报
        self:showBattleReport()
    elseif type == "exchange" then --兑换商店
        local showNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("SportExchangeLayer.csb")
        self:addChild(showNode,1)
        self.exchangeNode = showNode
        self:updateExchangeList()        
    elseif type == "rule" then
    end    
end

function SportUILayer:showBattleReport()
    local showNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("SportReportLayer.csb")
    self:addChild(showNode,1)

    local m_reportLayout = ccui.Helper:seekWidgetByName(showNode, "m_reportLayout")
    local m_cancelBtn = ccui.Helper:seekWidgetByName(showNode, "m_cancelBtn")
    local m_reportListView = ccui.Helper:seekWidgetByName(showNode, "m_reportListView")
    local function closeself(sender, event)
        if sender == m_cancelBtn and event == cc.EventCode.ENDED then
            showNode:removeFromParent()
        end
    end
    m_cancelBtn:addTouchEventListener(closeself)
    m_reportLayout:setBackGroundImage("jingjichangbeijing.jpg")

    local dataList = self.reportList

    for i=1,#dataList do
        local dataItem = dataList[i]

        local reportItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("SportReportItem.csb")

        local m_BgImage = ccui.Helper:seekWidgetByName(reportItem, "m_BgImage")
        local m_stateImage1 = ccui.Helper:seekWidgetByName(reportItem, "m_stateImage1")
        local m_stateImage2 = ccui.Helper:seekWidgetByName(reportItem, "m_stateImage2")
        local m_picImage = ccui.Helper:seekWidgetByName(reportItem, "m_picImage")
        local m_nameLabel = ccui.Helper:seekWidgetByName(reportItem, "m_nameLabel")
        local m_timeLabel = ccui.Helper:seekWidgetByName(reportItem, "m_timeLabel")
        local m_zhuanfaBtn = ccui.Helper:seekWidgetByName(reportItem, "m_zhuanfaBtn")
        local m_bofangBtn = ccui.Helper:seekWidgetByName(reportItem, "m_bofangBtn")
        local m_rankChangeLabel = ccui.Helper:seekWidgetByName(reportItem, "m_rankChangeLabel")
        local m_levelLabel = ccui.Helper:seekWidgetByName(reportItem, "m_levelLabel")
        local reportId = dataItem["id"]
        --[[
        optional int32 id=1;//战报ID
        optional int32 win=2;//我方是否获胜(0负 1是）
        optional int32 rankMf=3;//我方排名变动（负数表示下降，正数表示上升)
        optional string otherName=4;//对方姓名
        optional int32 otherLv=5;//对方等级
        optional int32 rptTm=6;//战报时间(秒)
        ]]
        --[[
        local micon = dataItem["icon"]
        if  micon > 30 then micon = micon%30 end
        m_picImage:loadTexture("touxiang_pic_soulicon"..micon..".png")
        ]]

        if  dataItem["win"] == 1 then
            m_stateImage1:loadTexture("sheng.png")
            m_stateImage2:loadTexture("jiantou02.png")
        end
        m_rankChangeLabel:setString(dataItem["rankMf"])
        if  dataItem["rankMf"]== 0 then
            m_rankChangeLabel:setVisible(false)
        end
        m_nameLabel:setString(dataItem["otherName"])
        m_levelLabel:setString(dataItem["otherLv"])
        local time = os.time() - dataItem["rptTm"]
        if  time < 60*60*24 then
            if  time < 60 then
                if  time < 0 then time = 0 end 
                local second = math.floor(time)
                local str =string.format("%d秒前",second)
                m_timeLabel:setString(str)
            elseif  time < 60*60 then
                local minute = math.floor(time/60)
                local str =string.format("%d分钟前",minute)
                m_timeLabel:setString(str)
            elseif time >= 60*60 then
                local hour = math.floor(time/(60*60))
                local str =string.format("%d小时前",hour)
                m_timeLabel:setString(str)
            end
        elseif time > 60*60*24 then
            local day = math.floor(time/(60*60*24))
            local str =string.format("%d天前",day)
            m_timeLabel:setString(str)
        end
        
        local function displayFunc(sender,envent)
            if sender == m_bofangBtn and envent == cc.EventCode.ENDED then
                Http: req("GetBattleReport",
                    {rptId = reportId,type = 1},
                    function(res) 
                        if res.err then cclog("err!!!") return end
                        if self:checkResult(res.data["result"]) then
                            local BattleScene = require("scene.battle2.BattleScene")
                            cc.Director:getInstance():pushScene(BattleScene:createPK(res.data))     
                        end
                    end)
            end
        end
        m_bofangBtn:addTouchEventListener(displayFunc)

        m_reportListView:pushBackCustomItem(reportItem)
    end
end

function SportUILayer:showBattleRank()
     local showNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("SportRankLayer.csb")
    self:addChild(showNode,1)

    local m_rankListLayout = ccui.Helper:seekWidgetByName(showNode, "m_rankListLayout")
    local m_cancelBtn = ccui.Helper:seekWidgetByName(showNode, "m_cancelBtn")
    local m_rankListView = ccui.Helper:seekWidgetByName(showNode, "m_rankListView")
    local function closeself(sender, event)
        if sender == m_cancelBtn and event == cc.EventCode.ENDED then
            showNode:removeFromParent()
        end
    end
    m_cancelBtn:addTouchEventListener(closeself)
    m_rankListLayout:setBackGroundImage("jingjichangbeijing.jpg")

    local dataList = self.rankList

    for i=1,table.getn(dataList) do
        local dataItem = dataList[i]

        local rankItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("SprotRankItem.csb")

        local m_rankItemLayout = ccui.Helper:seekWidgetByName(rankItem, "m_rankItemLayout")
        local m_rankImage = ccui.Helper:seekWidgetByName(rankItem, "m_rankImage")
        local m_picImage = ccui.Helper:seekWidgetByName(rankItem, "m_picImage")
        local m_nameLabel = ccui.Helper:seekWidgetByName(rankItem, "m_nameLabel")
        local m_levelLabel = ccui.Helper:seekWidgetByName(rankItem, "m_levelLabel")
        local m_fightPowerLabel = ccui.Helper:seekWidgetByName(rankItem, "m_fightPowerLabel")

        local mrank = dataItem["rank"]

        if mrank == 1 then
            m_rankImage:loadTexture("rank1.png")
            m_rankImage:setVisible(true)
        elseif  mrank == 2 then
            m_rankImage:loadTexture("rank2.png")
            m_rankImage:setVisible(true)
        elseif  mrank == 3 then
            m_rankImage:loadTexture("rank3.png")
            m_rankImage:setVisible(true)
        else
            local rannkLabel = ccui.TextAtlas:create()
            rannkLabel:setProperty("0123456789", "rankshuzi.png", 26, 37, "0")
            rankItem:addChild(rannkLabel)
            rannkLabel:setString(mrank)
            rannkLabel:setPosition(m_rankImage:getPosition())
        end

        local micon = dataItem["icon"]
        if  micon > 30 then micon = micon%30 end
        m_picImage:loadTexture("touxiang_pic_soulicon"..micon..".png")

        m_levelLabel:setString(dataItem["level"])
        m_nameLabel:setString(dataItem["name"])
        m_fightPowerLabel:setString(dataItem["fc"])

        m_rankListView:pushBackCustomItem(rankItem)
    end
end

function SportUILayer:updateDefenceFormation()

    local   defmatrix=formation:get("sport_formation")
    self.m_fightPowerLabel:setString(formation:get("fightpower"))

    if  defmatrix == nil then print("nil") return end
    if self.m_defFormationListView:getChildrenCount() ~= 0 then
        self.m_defFormationListView:removeAllChildren()
    end
    local copyitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("SportRoleItem.csb")
    for i=table.getn(defmatrix),1,-1 do
        local roleitem = copyitem:clone()
        local mHeroImage = ccui.Helper:seekWidgetByName(roleitem, "mHeroImage")
        local pinzhiImage = ccui.Helper:seekWidgetByName(roleitem, "pinzhiImage")

        local soulId = defmatrix[i]
        local soulItem = soulConfig[tostring(soulId)]

        local finalSoulId = soulId %30
        if finalSoulId == 0 then
            finalSoulId = 30
        end
        mHeroImage:loadTexture("soul_pic_soulicon"..finalSoulId..".png")
        pinzhiImage:loadTexture("normal_aochao_0"..soulItem["quality"]..".png")
        
        self.m_defFormationListView:pushBackCustomItem(roleitem)
    end
    for i=1,5-#defmatrix do
        local roleitem = copyitem:clone()
        local mHeroImage = ccui.Helper:seekWidgetByName(roleitem, "mHeroImage")
        local pinzhiImage = ccui.Helper:seekWidgetByName(roleitem, "pinzhiImage")
        local m_addHeroImage = ccui.Helper:seekWidgetByName(roleitem, "m_addHeroImage")
        mHeroImage:setVisible(false)
        m_addHeroImage:setVisible(true)

        self.m_defFormationListView:pushBackCustomItem(roleitem)
    end
end

function SportUILayer:checkResult(_result)
    local bRet=false
    if _result== 0 then 
        bRet = true
    elseif  _result == 1 then
        cclog("EXCHANGE_BAG_NOT_ENOUGH")
        PopDialog:getInstance():popText("背包不足",cc.c3b(0,255,0))
    elseif  _result == 2 then
        cclog("GET_BAG_NOT_ENOUGH")
        PopDialog:getInstance():popText("领取奖励，背包不足",cc.c3b(0,255,0))
    elseif  _result == 3 then
        cclog("SPORT_MATRIX_NOT_ENOUGH")
        PopDialog:getInstance():popText("没有上阵人员",cc.c3b(0,255,0))
    elseif  _result == 4 then
        cclog("SPORT_GOLD_NOT_ENOUGH")
        PopDialog:getInstance():popText("金币不足",cc.c3b(0,255,0))
    elseif  _result == 5 then
        cclog("SPORT_REFRESH_SHOP_CNT_NOT_ENOUGH")
        PopDialog:getInstance():popText("刷新次数已用完",cc.c3b(0,255,0))
    elseif  _result == 6 then
        cclog("SPORT_SCORE_NOT_ENOUGH")
        PopDialog:getInstance():popText("积分不足",cc.c3b(0,255,0))
    elseif  _result == 7 then
        cclog("SPORT_BUY_CHALLENGE_CNT_NOT_ENOUGH")
        PopDialog:getInstance():popText("购买挑战次数不足",cc.c3b(0,255,0))
    end
    return bRet
end

return SportUILayer
