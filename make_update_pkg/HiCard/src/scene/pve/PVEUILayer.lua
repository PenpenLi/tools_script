--PVEUILayer.lua, created by php script. chris.li--
require "Cocos2d"
local pveChallengeStageId = 0
local Roler = require("data.Roler")
local stageConfig = require("config.define_stage")
local mapConfig = require("config.define_stage_map")
local shopConfig = require("config.define_shop")
local soulConfig = require("config.define_soul")
local stageBuyConfig = require("config.define_stage_buy_attack")
local PopDialog = require "common/PopDialog"
local PVEUILayer = class("PVEUILayer", function()
	return cc.Layer:create()
end)

function PVEUILayer:create(param)
	local layer = PVEUILayer.new()
    layer:initM(param)
	return layer
end

function PVEUILayer:ctor()
end

function PVEUILayer:onEnter()
end

function PVEUILayer:onExit()
end

function PVEUILayer:initM(param)
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("PVEUILayer.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end
    
    
    
	--m_PVELayout: Layout
	self.m_PVELayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_PVELayout")
	self.m_PVELayout:addTouchEventListener(touchEventHandler)

	--m_PVEPageView: PageView
	self.m_PVEPageView = ccui.Helper:seekWidgetByName(self.rootNode, "m_PVEPageView")

    --m_closeBtn: Button
    self.m_closeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeBtn")
    self.m_closeBtn:addTouchEventListener(touchEventHandler)

    --m_NormalPveBtn: Button
    self.m_NormalPveBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_NormalPveBtn")
    self.m_NormalPveBtn:addTouchEventListener(touchEventHandler)

    --m_elitePveBtn: Button
    self.m_elitePveBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_elitePveBtn")
    self.m_elitePveBtn:addTouchEventListener(touchEventHandler)

    --m_preBtn: Button
    self.m_preBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_preBtn")
    self.m_preBtn:addTouchEventListener(touchEventHandler)

    --m_nextBtn: Button
    self.m_nextBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_nextBtn")
    self.m_nextBtn:addTouchEventListener(touchEventHandler)

    --m_curMapNameLabel: Text
    self.m_curMapNameLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_curMapNameLabel")
    
    --tag Image
    self.TagImage = ccui.Helper:seekWidgetByName(self.rootNode, "TagImage")

    self:initUI(param)
end

function PVEUILayer:handleTouchEvent(sender, event)
    if sender == nil then
        cclog("[ERR]unknown sender")

    elseif sender == self.m_PVELayout and event == cc.EventCode.ENDED then
        cclog("[PVEUILayer]--m_PVELayout touched--")

    elseif sender == self.m_closeBtn and event == cc.EventCode.ENDED then
        cclog("[PVEUILayer]--m_closeBtn touched--")
        if  self.fromStageId ~= 0 then
            cc.Director:getInstance():popScene()
            return
        end
        local scene = require("scene.home.HomeScene"):create()
        cc.Director:getInstance():replaceScene(scene)
        
    elseif sender == self.m_NormalPveBtn and event == cc.EventCode.ENDED then
        cclog("[PVEUILayer]--m_NormalPveBtn touched--")
        self:changePveType("normal")
        self.m_elitePveBtn:loadTextures("jingying02.png","jingying02.png","jingying02.png")
        self.m_NormalPveBtn:loadTextures("putong01.png","putong02.png","putong02.png")
        self.m_NormalPveBtn:setTouchEnabled(false)
        self.m_elitePveBtn:setTouchEnabled(true)
        
    elseif sender == self.m_elitePveBtn and event == cc.EventCode.ENDED then
        cclog("[PVEUILayer]--m_elitePveBtn touched--")
        self:changePveType("elite")
        self.m_elitePveBtn:loadTextures("jingying01.png","jingying02.png","jingying02.png")
        self.m_NormalPveBtn:loadTextures("putong02.png","putong02.png","putong02.png")
        self.m_NormalPveBtn:setTouchEnabled(true)
        self.m_elitePveBtn:setTouchEnabled(false)
        
    elseif sender == self.m_preBtn and event == cc.EventCode.ENDED then
        cclog("[PVEUILayer]--m_preBtn touched--")
        local curIndex = self.m_PVEPageView:getCurPageIndex()
        if  curIndex > 0 then
            self.m_PVEPageView:scrollToPage(curIndex-1)
        elseif curIndex == 0 then
        	self.m_preBtn:setVisible(false)
        end
        
    elseif sender == self.m_nextBtn and event == cc.EventCode.ENDED then
        cclog("[PVEUILayer]--m_nextBtn touched--")
        local curIndex = self.m_PVEPageView:getCurPageIndex()+1
        if curIndex == self.maxPageIndex then
        	PopDialog:getInstance():popText("人物等级"..mapConfig[tostring(curIndex)]["level"].."级开放")
        	return
        end
        if  curIndex < self.maxPageIndex then
            self.m_PVEPageView:scrollToPage(curIndex)
        elseif curIndex == self.maxPageIndex then
            self.m_nextBtn:setVisible(false)
        end
        
	end
end

--init your ui here
function PVEUILayer:initUI(param)
    self.hasRequestTable = {0,0,0,0,0,0,0}
    
    self.m_PVELayout:setBackGroundImage("ditu.jpg")

    
    self.fromStageId = 0
    local pveType = 0
    local mapId = 0
    if  param then
        if  param.pveType ~= nil then pveType = param.pveType end
        if  param.stageId ~= nil then mapId = param.stageId/13 + 1 self.fromStageId = param.stageId end
    end
    if  pveType == 1 then
        self.m_elitePveBtn:loadTextures("jingying01.png","jingying02.png","jingying02.png")
        self.m_NormalPveBtn:loadTextures("putong02.png","putong02.png","putong02.png")
        self.m_NormalPveBtn:setTouchEnabled(true)
        self.m_elitePveBtn:setTouchEnabled(false)
        --self.m_elitePveBtn:setHighlighted(true)
    else
        self.m_NormalPveBtn:setTouchEnabled(false)
        self.m_elitePveBtn:setTouchEnabled(true)

        self.m_elitePveBtn:loadTextures("jingying02.png","jingying02.png","jingying02.png")
        self.m_NormalPveBtn:loadTextures("putong01.png","putong02.png","putong02.png")
    end
    Http: req("GetAllStage",
        {pveType = pveType,mapId = mapId},
        function(res) 
            if res.err then cclog("err!!!") return end
            self.TagImage:setVisible(true)
            self.sweepCnt = res.data["sweepCnt"]
            self.maxStageId = res.data["maxStageId"]
            self.maxPageIndex = res.data["maxMapId"]
            self.curPageIndex = res.data["maxMapId"]
            self.stageList = res.data["stageList"]
            self:LoadPvePage()
            self:setData(self.curPageIndex)
        end)
end

--refresh all ui here
function PVEUILayer:refreshUI()
end

function PVEUILayer:changePveType(type)
    self.hasRequestTable = {0,0,0,0,0,0,0}
    if  type == "elite" then 
        Http: req("GetAllStage",
            {pveType = 1,mapId = 0},
            function(res) 
                if res.err then cclog("err!!!") return end
                self.sweepCnt = res.data["sweepCnt"]
                self.maxStageId = res.data["maxStageId"]
                self.maxPageIndex = res.data["maxMapId"]
                self.curPageIndex = res.data["maxMapId"]
                self.stageList = res.data["stageList"]
                self:LoadPvePage()
                self:setData(self.curPageIndex)
            end)
    else
        Http: req("GetAllStage",
            {pveType = 0,mapId = 0},
            function(res) 
                if res.err then cclog("err!!!") return end
                self.sweepCnt = res.data["sweepCnt"]
                self.maxStageId = res.data["maxStageId"]
                self.maxPageIndex = res.data["maxMapId"]
                self.curPageIndex = res.data["maxMapId"]
                self.stageList = res.data["stageList"]
                self:LoadPvePage()
                self:setData(self.curPageIndex)
            end)        
    end
    
end

function PVEUILayer:requestPVEList(type,map)
    if  self.hasRequestTable[map] == 1 then return end
    Http: req("GetAllStage",
        {pveType = type,mapId = map},
        function(res) 
            if res.err then cclog("err!!!") return end
            self.sweepCnt = res.data["sweepCnt"]
            self.maxStageId = res.data["maxStageId"]
            self.curPageIndex = res.data["maxMapId"]
            self.stageList = res.data["stageList"] 
            self:setData(map) 
        end)
end

--向后插入一页地图
function PVEUILayer:addPvePage(index)
    local page_Layout = ccui.Helper:seekWidgetByName(self.pageNode,"m_pvepage"..index.."Layout")
    --page_Layout:setBackGroundImage("map"..index..".png")
    page_Layout:removeFromParent(false)
    self.m_PVEPageView:addPage(page_Layout)
end

--向前插入一页地图
function PVEUILayer:insertPvePage(index)
    local page_Layout = ccui.Helper:seekWidgetByName(self.pageNode,"m_pvepage"..index.."Layout")
    --page_Layout:setBackGroundImage("map"..index..".png")
    page_Layout:removeFromParent(false)
    self.m_PVEPageView:insertPage(page_Layout,0)
end

--载入地图
function PVEUILayer:LoadPvePage()
    if self.m_PVEPageView ~= nil then self.m_PVEPageView:removeFromParent() end
    
    local function pageViewEvent(sender, eventType)
        if eventType == ccui.PageViewEventType.turning then
            local pageView = sender
            local pageInfo =  pageView:getCurPageIndex() + 1
            
            self.m_curMapNameLabel:setString(mapConfig[tostring(pageInfo)]["name"])
            self.m_preBtn:setVisible(true)
            self.m_nextBtn:setVisible(true)
            local curIndex = self.m_PVEPageView:getCurPageIndex()
            if  curIndex == 0 then
                self.m_preBtn:setVisible(false)
            elseif curIndex == self.maxPageIndex-1 then
                self.m_nextBtn:setVisible(false)

            end          
            self:requestPVEList(0,pageInfo)
        end
    end 
    
    local tagImage = ccui.Helper:seekWidgetByName(self.rootNode, "TagImage")
    self.m_PVEPageView = ccui.PageView:create()
    self.m_PVEPageView:setTouchEnabled(true)
    self.m_PVEPageView:setContentSize(cc.size(870, 548))
    self.m_PVEPageView:setPosition(cc.p(-435,-274))
    self.m_PVEPageView:addEventListener(pageViewEvent)
    tagImage:addChild(self.m_PVEPageView)
    local index = self.curPageIndex
    --self.m_curMapNameLabel:setString(mapConfig[tostring(index)]["name"])
    self.pageNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("PVEPageLayer.csb")
    --[[
    local function pageTouched(sender,event)
    print("touched")
    if sender == self.m_PVEPageView and event == cc.EventCode.ENDED then
    if  loadFirst then
    for i=index,1,-1 do
    self:insertPvePage(i)
    end
    loadFirst = false
    end
    end
    end
    ]]
    for i=index,1,-1 do
        self:insertPvePage(i)
    end
    self.m_PVEPageView:scrollToPage(index-1)
end

--设置地图数据
function PVEUILayer:setData(index)
   local page = self.m_PVEPageView:getPage(index-1)
   page:setBackGroundImage("map"..index..".png")
   self.hasRequestTable[index] = 1
   for i=1,13 do
       local stageBtn =  ccui.Helper:seekWidgetByName(page, "Level"..i.."Btn")
       --local stageBGImage =  ccui.Helper:seekWidgetByName(stageBtn,"BgGuangImage")
       --stageBGImage:setScale(1)
       --local seq = cc.Sequence:create(cc.FadeOut:create(1.5),cc.FadeIn:create(1.5))
       --stageBGImage:runAction(cc.Repeat:create(seq,cc.REPEAT_FOREVER))
       local score = self.stageList[i]["score"]
       if   score >0 then
            local starBgImage = ccui.ImageView:create("xindi0.png")
            starBgImage:setScale(1)
            starBgImage:setPosition(cc.p(stageBtn:getCustomSize().width/2,stageBtn:getCustomSize().height/2-50))
            stageBtn:addChild(starBgImage,-1)
       end
        local startBeginPosX = stageBtn:getCustomSize().width*(5+1-score)*0.1
       for j=1,score do
           local starImage = ccui.ImageView:create("xin.png")
           starImage:setScale(0.8)
           starImage:setPosition(cc.p(startBeginPosX+16*(j-1),stageBtn:getCustomSize().height/2-50))
           stageBtn:addChild(starImage)
       end
   end
    local draw = cc.DrawNode:create()
    
   page:addChild(draw,0)
   for i=1,13 do
        local stageBtn =  ccui.Helper:seekWidgetByName(page, "Level"..i.."Btn")
        stageBtn:setTag(self.stageList[i]["stageId"])
        if  self.stageList[i]["stageId"]>self.maxStageId then
            --[[
            -- set gray
            local texture = stageBtn:getVirtualRenderer()
            Helper.graySprite(texture)
            stageBtn:setTouchEnabled(false)
            ]]
            stageBtn:setVisible(false)
        elseif  i>1 then
            local j= i-1
            local stageBtnfrom =  ccui.Helper:seekWidgetByName(page, "Level"..j.."Btn")
            local posFrom = page:convertToNodeSpace(stageBtnfrom:getWorldPosition())
            local posTo = page:convertToNodeSpace(stageBtn:getWorldPosition())
            --[[
            draw:drawSegment(cc.p(posFrom.x,posFrom.y-stageBtnfrom:getCustomSize().height/2+5),
                             cc.p(posTo.x,posTo.y-stageBtn:getCustomSize().height/2+5), 
                             2, cc.c4f(0, 1, 0, 1))
                             ]]
        end
        local score = self.stageList[i]["score"]    
        local attackSum = self.stageList[i]["attackSum"]
        local buyAttackSum = self.stageList[i]["buyAttackSum"]
        local function showInfo(sender,event)
            if sender == stageBtn and event == cc.EventCode.ENDED then
                self:showLevelInfo(stageBtn:getTag(),score,attackSum,buyAttackSum)
            end
        end
        stageBtn:addTouchEventListener(showInfo)
   end
end

--显示关卡挑战界面
function PVEUILayer:showLevelInfo(stageId,scroe,attackSum,buyAttackSum)
    local levelInfoNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("PVELevelInfoLayer.csb")
    self:addChild(levelInfoNode,1)
    
    --m_LevelInfoRootLayout: Layout
    local m_LevelInfoRootLayout = ccui.Helper:seekWidgetByName(levelInfoNode, "m_LevelInfoRootLayout")
    m_LevelInfoRootLayout:setBackGroundImage("ditu.jpg")
    
    --m_BGLayout: Layout
    local m_BGLayout = ccui.Helper:seekWidgetByName(levelInfoNode, "m_BGLayout")
    
    --m_raidsLayout: Layout
    local m_raidsLayout = ccui.Helper:seekWidgetByName(levelInfoNode, "m_raidsLayout")
    
    --m_tipsLayout: Layout
    local m_tipsLayout = ccui.Helper:seekWidgetByName(levelInfoNode, "m_tipsLayout")
    --m_buyCntCostLabel: Label
    local m_buyCntCostLabel = ccui.Helper:seekWidgetByName(m_tipsLayout, "buyCntCostLabel")
    --m_buyCntCancelBtn: Button
    local m_buyCntCancelBtn = ccui.Helper:seekWidgetByName(m_tipsLayout, "m_buyCntCancelBtn")
    --m_buyCntConfirmBtn: Button
    local m_buyCntConfirmBtn = ccui.Helper:seekWidgetByName(m_tipsLayout, "m_buyCntConfirmBtn")
    
    --m_closeBtn: Button
    local m_closeBtn = ccui.Helper:seekWidgetByName(levelInfoNode, "m_closeBtn")
    local function closeDisplay(sender,event)
        if sender ==m_closeBtn and event == cc.EventCode.ENDED then
            levelInfoNode:removeFromParent()
            --[[
            Http: req("GetAllStage",
                {pveType = 0,mapId = self.curPageIndex},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    self.curPageIndex = res.data["maxMapId"]
                    self.maxStageId = res.data["maxStageId"]
                    self.stageList = res.data["stageList"] 
                    self:setData(self.curPageIndex) 
                end)
                ]]
        end
    end
    m_closeBtn:addTouchEventListener(closeDisplay)
    
    --score
    for i=1,scroe do
        local m_starImage = ccui.Helper:seekWidgetByName(levelInfoNode, "m_star"..i.."Image")
        m_starImage:loadTexture("xingxing.png")
    end
    
    local stageItem = stageConfig[tostring(stageId)]
    --m_LevelNameLabel: Text
    local m_LevelNameLabel = ccui.Helper:seekWidgetByName(levelInfoNode, "m_LevelNameLabel")
    m_LevelNameLabel:setString(stageItem["stage_name"])
    

    --level desc
    local descLabel = ccui.Helper:seekWidgetByName(levelInfoNode, "m_monsterDescLabel")
    --descLabel:setString(stageItem["desc"])
    
    --m_cntLabel: Text
    local m_cntLabel = ccui.Helper:seekWidgetByName(levelInfoNode, "m_cntLabel")
    m_cntLabel:setString(math.max(stageItem["day_attack"]-attackSum,0).."/"..stageItem["day_attack"])
    
    --m_energyCostLabel: Text
    local m_energyCostLabel = ccui.Helper:seekWidgetByName(levelInfoNode, "m_energyCostLabel")
    m_energyCostLabel:setString(stageItem["use_cp"])

    --rewardList
    local m_rewardListView = ccui.Helper:seekWidgetByName(levelInfoNode, "m_rewardListView")
    
    -----------buy cnt-----------
    --m_buyCntBtn : button
    local m_buyCntBtn = ccui.Helper:seekWidgetByName(levelInfoNode, "m_buyCntBtn")
    if Roler:get("energy") < stageItem["use_cp"] then
        m_buyCntBtn:setVisible(true)
    end
    local function buyCntConfirmFunc(sender,event)
        if sender == m_buyCntConfirmBtn and event == cc.EventCode.ENDED then
            Http: req("BuyPveCount",
                {stageId=stageId},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    PopDialog:getInstance():popText("购买挑战次数成功",cc.c3b(0,255,0))
                    m_tipsLayout:setVisible(false)
                end)
        end
    end
    m_buyCntConfirmBtn:addTouchEventListener(buyCntConfirmFunc)
    local function buyCntCanelFunc(sender,event)
        if (sender == m_buyCntCancelBtn or sender == m_tipsLayout) and event == cc.EventCode.ENDED then
            m_tipsLayout:setVisible(false)
        end
    end
    m_buyCntCancelBtn:addTouchEventListener(buyCntCanelFunc)
    local function buyCntFunc(sender,event)
        if sender == m_buyCntBtn and event == cc.EventCode.ENDED then
            m_tipsLayout:setVisible(true)
            m_buyCntCostLabel:setString(stageBuyConfig[tostring(buyAttackSum)]["cost"])
        end
    end
    m_tipsLayout:addTouchEventListener(buyCntCanelFunc)
    m_buyCntBtn:addTouchEventListener(buyCntFunc)
    ----------------------------------------------------------------------------
    
    --reward image
    local rewardlist = string.split(stageItem["item"],";")
    for i=1,table.getn(rewardlist) do
        local rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("battleRewardItem.csb")
        local list = string.split(rewardlist[i],":")
        local m_rewardImage = ccui.Helper:seekWidgetByName(rootNode, "m_picImage")
        local m_pinzhiImage = ccui.Helper:seekWidgetByName(rootNode, "m_pinzhiImage")
        local m_suipianImage = ccui.Helper:seekWidgetByName(rootNode, "m_suipianImage")
        local m_numLabel = ccui.Helper:seekWidgetByName(rootNode, "m_numLabel")
        m_numLabel:setVisible(false)
        local shopItem = shopConfig[list[1]]
        m_rewardImage:setVisible(true)
        m_rewardImage:loadTexture(shopItem["icon"]..".png")
        m_pinzhiImage:loadTexture("normal_aochao_0"..shopItem["quality"]..".png")
        if  shopItem["type"] == 16 then
            m_suipianImage:setVisible(true)
        end
        m_rewardListView:pushBackCustomItem(rootNode)
    end
    
    --m_monsterImage: ImageView
    local m_monsterImage = ccui.Helper:seekWidgetByName(levelInfoNode, "m_monsterImage")
    local monsterlist = string.split(stageItem["second_monster"],"|")
    local monsterItem = string.split(monsterlist[1],",")
    --print("monsterItem[1]:"..monsterItem[1])
    if monsterItem[1] ~= "nil" then
        local soulItem = soulConfig[monsterItem[1]]
        if  soulItem == nil then return end
        m_monsterImage:loadTexture(soulItem["icon"]..".png")
        local m_pinzhiImage = ccui.Helper:seekWidgetByName(m_monsterImage, "pinzhiImage")
        m_pinzhiImage:loadTexture("normal_aochao_0"..soulItem["quality"]..".png")
        m_monsterImage:setVisible(true)
    end
    
    --m_challengeBtn: Button
    local m_challengeBtn = ccui.Helper:seekWidgetByName(levelInfoNode, "m_challengeBtn")
    local function challengeStart(sender,event)
        if sender ==m_challengeBtn and event == cc.EventCode.ENDED then
            pveChallengeStageId = stageId
            local FormationUILayer =  require "scene.formation.FormationUILayer"
            --FormationUILayer:setBeforeSceneType("pve")
            local scene = require("scene.formation.FormationScene"):create({BattleType="pve"})
            --local scene = require("scene.battle.BattleScene"):create()
            cc.Director:getInstance():pushScene(scene)
            
            
            --[[
            Http: req("NewStartCheck",
                {stageId = stageId},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                    end
            end)
            ]]
        end
    end
    m_challengeBtn:addTouchEventListener(challengeStart)
    
    --m_saodangLabel: Text
    local m_saodangLabel = ccui.Helper:seekWidgetByName(levelInfoNode, "m_saodangLabel")
    m_saodangLabel:setString(self.sweepCnt)
    -------------------------------------扫荡-------------------------------------------------
    
    --m_oneTimesBtn: Button
    local m_oneTimesBtn = ccui.Helper:seekWidgetByName(levelInfoNode, "m_oneTimesBtn")
    
    --m_tenTimesBtn: Button
    local m_tenTimesBtn = ccui.Helper:seekWidgetByName(levelInfoNode, "m_tenTimesBtn")
    
    if  self.sweepCnt == 0 or scroe < 3 then
        m_oneTimesBtn:setVisible(false)
        m_tenTimesBtn:setVisible(false)
        --[[
        m_oneTimesBtn:setTouchEnabled(false)
        m_tenTimesBtn:setTouchEnabled(false)
        Helper.graySprite(m_oneTimesBtn:getVirtualRenderer())
        Helper.graySprite(m_tenTimesBtn:getVirtualRenderer())
        ]]
    end
    
    
    ---
    local m_RaidsListView = ccui.Helper:seekWidgetByName(levelInfoNode, "m_RaidsListView")
    local m_RaidsRewardListView = ccui.Helper:seekWidgetByName(levelInfoNode, "m_RaidsRewardListView")
    local m_RaidsKuangImage = ccui.Helper:seekWidgetByName(levelInfoNode, "m_RaidsKuangImage")
    local m_SweepFinishImage = ccui.Helper:seekWidgetByName(levelInfoNode, "m_SweepFinishImage")
    
    local function showRaidsLayout(num,param)
        --if  #param == 0 then return end
        --[[
        if  copyListView:getChildrenCount() ~= 0 then 
            --m_RaidsListView:removeAllItems()
            copyListView:scrollToTop(0,false)
            copyListView:removeAllChildren()
        end
        ]]
        
        local copyListView = ccui.ListView:create()
        copyListView:setPosition(m_RaidsListView:getPosition())
        copyListView:setContentSize(m_RaidsListView:getCustomSize())
        copyListView:setGravity(ccui.ListViewGravity.centerVertical)
        copyListView:setDirection(ccui.ListViewDirection.vertical)
        copyListView:setItemsMargin(15)
        m_RaidsKuangImage:addChild(copyListView)
        copyListView:setTag(5000)
        --copyListView:scrollToTop(0,false)
        print("create")
        m_SweepFinishImage:setVisible(false)
        m_SweepFinishImage:runAction(cc.Sequence:create(cc.DelayTime:create(1.0*num),cc.Show:create()))
        local raidsItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("SweepPanel.csb")
        m_raidsLayout:setVisible(true)
        for   i=1,num do
            local function scrollTo()
                if  i%2 == 0 then
                    copyListView:scrollToPercentVertical(10*i,5,false)
                end
            end
            --m_RaidsListView:scrollToPercentVertical(10,0,false)
            local copyItem = raidsItem:clone()
            copyItem:setVisible(false)
            local delay = cc.DelayTime:create(0.5*i)
            local delay2 = cc.DelayTime:create(0.5*i)
             if  i==1 then delay = cc.DelayTime:create(0) end
            copyItem:runAction(cc.Sequence:create(delay,cc.CallFunc:create(scrollTo),delay2,cc.Show:create()))
            local dataItem = param[i]
            copyListView:pushBackCustomItem(copyItem)
            local m_TaitouLabel = ccui.Helper:seekWidgetByName(copyItem, "m_TaitouLabel")
            local m_coinsLabel = ccui.Helper:seekWidgetByName(copyItem, "m_coinsLabel")
            local m_rewardItemLabel = ccui.Helper:seekWidgetByName(copyItem, "m_rewardItemLabel")
            local m_expLabel = ccui.Helper:seekWidgetByName(copyItem, "m_expLabel")
            m_TaitouLabel:setString(string.format("第%d轮战斗结果",i))
            m_coinsLabel:setString(dataItem["coins"])     
            m_expLabel:setString(dataItem["expReward"][1]["addExp"])
            --reward 取了一个
            local itemReward = dataItem["itemReward"][1]
            local name = shopConfig[tostring(itemReward["id"])]["name"]
            local str = string.format("%s*%s",name,itemReward["num"])

            m_rewardItemLabel:setString(str)
        end
        --扫荡券
        self.sweepCnt = self.sweepCnt - num
        m_saodangLabel:setString(self.sweepCnt)
        --m_RaidsListView:scrollToPercentVertical(50,1,false)
    end
    
    local function showRaids(sender,event)
        if sender == m_oneTimesBtn and event == cc.EventCode.ENDED then
            Http: req("Sweep",
                {stageId=stageId,num=1},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if self:checkResult(res.data["result"]) then
                        attackSum = attackSum +1
                        m_cntLabel:setString(math.max(stageItem["day_attack"]-attackSum,0).."/"..stageItem["day_attack"])
                        showRaidsLayout(1,res.data["reward"])
                    end
                end)
        end
    end
    local function showTenRaids(sender,event)
        if sender == m_tenTimesBtn and event == cc.EventCode.ENDED then
            Http: req("Sweep",
                {stageId=stageId,num=10},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if self:checkResult(res.data["result"]) then
                        attackSum = attackSum +10
                        m_cntLabel:setString(math.max(stageItem["day_attack"]-attackSum,0).."/"..stageItem["day_attack"])
                        showRaidsLayout(10,res.data["reward"])
                    end
                end)
        end
    end
    local function hideRaidsLayout(sender,event)
        if sender == m_raidsLayout and event == cc.EventCode.ENDED then
            m_raidsLayout:setVisible(false)
            local copyListView = m_RaidsKuangImage:getChildByTag(5000)
            if  copyListView then copyListView:removeFromParent() print("remove") end
        end
    end
    m_raidsLayout:addTouchEventListener(hideRaidsLayout)
    m_oneTimesBtn:addTouchEventListener(showRaids)
    m_tenTimesBtn:addTouchEventListener(showTenRaids)
    

end



function PVEUILayer:checkResult(_result)
    local bRet=false
    if _result== 0 then 
        bRet = true
    elseif  _result == 1 then
        cclog("PVE_CP_NOT_ENOUGH")
        PopDialog:getInstance():popText("体力不足",cc.c3b(0,255,0))
    elseif  _result == 2 then
        cclog("TEAM_FULL")
    elseif  _result == 3 then
        cclog("TEAM_IS_ERROR")
    elseif  _result == 4 then
        cclog("TEAM_IS_FIGHT")
    elseif  _result == 5 then
        cclog("TEAM_NOT_EXITS")
    elseif  _result == 6 then
        cclog("IS_IN_TEAM")
    elseif  _result == 7 then
        cclog("NOT_HAVA_RIGHT")
    elseif  _result == 8 then
        cclog("IS_OTHER_TEAM")
    elseif  _result == 9 then
        cclog("IS_NOT_ONLINE")
    elseif  _result == 10 then
        cclog("MEMBER_NOT_PREPARE")
    elseif  _result == 11 then
        cclog("DAY_ATTACK_ENOUGH")
        PopDialog:getInstance():popText("该关卡已达到每日可以攻打次数上限",cc.c3b(0,255,0))
    elseif  _result == 12 then
        cclog("BORROW_FAIL")
    elseif  _result == 13 then
        cclog("SPORT_BUY_COUNT_NOT_ENOUGH")
    elseif  _result == 14 then
        cclog("SPORT_GOLD_NOT_ENOUGH")
    elseif  _result == 15 then
        cclog("COINS_ECTYPE_NOT_ENOUGH")
    elseif  _result == 16 then
        cclog("PVE_NODE_PRE_NOT_PASS")
        PopDialog:getInstance():popText("前置关卡未通过",cc.c3b(0,255,0))
    elseif  _result == 17 then
        cclog("PVE_NODE_LEVEL_NOT_PASS")
        PopDialog:getInstance():popText("玩家等级不足不允许进入",cc.c3b(0,255,0))
    elseif  _result == 18 then
        cclog("SPORT_CHALLENGE_CD")
    elseif  _result == 19 then
        cclog("BOOK_ROB_COUNT_NOT_ENOUGH")
    elseif  _result == 20 then
        cclog("SPORT_CHALLENGE_REFRESH")
    elseif  _result == 21 then
        cclog("SPORT_CHALLENGE_COUNT_NOT_ENOUGH")
    elseif  _result == 22 then
        cclog("PVE_SWEEP_NOT_ENOUGH")
        PopDialog:getInstance():popText("扫荡券不足",cc.c3b(0,255,0))
    elseif  _result == 23 then
        cclog("PVE_BUY_COUNT_NOT_ENOUGH")
        PopDialog:getInstance():popText("购买挑战次数不足",cc.c3b(0,255,0))
    elseif  _result == 24 then
        cclog("PVE_NODE_NOT_THREE_STAR")
        PopDialog:getInstance():popText("副本未三星通关",cc.c3b(0,255,0))
    elseif  _result == 25 then
        cclog("PVE_GOLD_NOT_ENOUGH")
    end
    return bRet
end

function PVEUILayer:getChallengeStageId()
    return  pveChallengeStageId
end

return PVEUILayer
