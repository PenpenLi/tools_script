--PVEUILayer.lua, created by php script. chris.li--
require "Cocos2d"
local pveChallengeStageId = 0
local challengeCntValue = {attackSum = 0,totalSum = 0}
local Roler = require("data.Roler")
local stageConfig = require("config.define_stage")
local mapConfig = require("config.define_stage_map")
local shopConfig = require("config.define_shop")
local soulConfig = require("config.define_soul")
local stageBuyConfig = require("config.define_stage_buy_attack")
local vipConfig = require("config.define_vip")
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
    print("onEnter pve")
    if  self.m_cntLabel ~= nil then
        self.m_cntLabel:setString(math.max(challengeCntValue.totalSum-challengeCntValue.attackSum,0).."/"..challengeCntValue.totalSum)
    end
    
   --[[ if Roler:get("level") == 12 then
        if guardCenter:canEnterGuardMode("duobao") == true then
            self:processGuard()
        end
    elseif Roler:get("level") == 18 then
        if guardCenter:canEnterGuardMode("sport") == true then
            self:processGuard()
        end    
    end--]]
    
end

function PVEUILayer:onExit()
end

function PVEUILayer:processGuard()
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

    if guardCenter:canEnterGuardMode("pve") == true then
        local cmd = guardCenter:getCmd("pve")
        printJSON(cmd)
        if cmd == nil then return end
        if cmd.name == "pve_click_level1btn" then
            local page = self.m_PVEPageView:getPage(0)
            local stageBtn =  ccui.Helper:seekWidgetByName(page, "Level1Btn")
            cmd.clickTarget = stageBtn
            addGuardClickLayer(cmd)
        elseif cmd.name == "pve_click_goonbtn" then
            cmd.clickTarget = self.challengeBtn
            addGuardClickLayer(cmd)
        elseif cmd.name == "pve_dialog_log1" then
            addGuardDialogLayer(cmd)
        elseif cmd.name == "pve_dialog_log2" then
            addGuardDialogLayer(cmd)
        elseif cmd.name == "pve_click_closepvebtn" then
            cmd.clickTarget = self.m_closeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("pve2") == true then
        local cmd = guardCenter:getCmd("pve2")
        if cmd == nil then return end
        if cmd.name == "pve2_click_level2btn" then
            local page = self.m_PVEPageView:getPage(0)
            local stageBtn =  ccui.Helper:seekWidgetByName(page, "Level2Btn")
            cmd.clickTarget = stageBtn
            addGuardClickLayer(cmd)  
        elseif cmd.name == "pve2_click_goonbtn" then
            cmd.clickTarget = self.challengeBtn
            addGuardClickLayer(cmd)
        elseif cmd.name == "pve2_click_closepvebtn" then
            cmd.clickTarget = self.m_closeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("herowear") == true then
        local cmd = guardCenter:getCmd("herowear")
        if cmd == nil then return end
        if cmd.name == "herowear_click_closepvebtn" then
            cmd.clickTarget = self.m_closeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("jinengyaoshui") == true then
        local cmd = guardCenter:getCmd("jinengyaoshui")
        if cmd == nil then return end
        if cmd.name == "jinengyaoshui_click_closepvebtn" then
            cmd.clickTarget = self.m_closeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("duobao") == true then
        local cmd = guardCenter:getCmd("duobao")
        if cmd == nil then return end
        if cmd.name == "duobao_click_closepvebtn" then
            cmd.clickTarget = self.m_closeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("sport") == true then
        local cmd = guardCenter:getCmd("sport")
        if cmd == nil then return end
        if cmd.name == "sport_click_closepvebtn" then
            cmd.clickTarget = self.m_closeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("qianghua") == true then
        local cmd = guardCenter:getCmd("qianghua")
        if cmd == nil then return end
        if cmd.name == "qianghua_click_closepvebtn" then
            cmd.clickTarget = self.m_closeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("jinjie") == true then
        local cmd = guardCenter:getCmd("jinjie")
        if cmd == nil then return end
        if cmd.name == "jinjie_click_closepvebtn" then
            cmd.clickTarget = self.m_closeBtn
            addGuardClickLayer(cmd)
        end
    end
    
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
    
    -- guard_tag --
    self:processGuard()
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
        --local scene = require("scene.home.HomeScene"):create()
        --cc.Director:getInstance():replaceScene(scene)
        local scene = require("scene.load1.Load1Scene"):create()
        cc.Director:getInstance():replaceScene(scene) 
        
        if guardCenter:canEnterGuardMode("pve") == true or
            guardCenter:canEnterGuardMode("pve2") == true then
         guardCenter:jumpNextGuardStage()
        end
        
        
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
        if  curIndex < self.maxPageIndex then
            if curIndex == self.maxPageIndex -1 then self.m_nextBtn:setVisible(false) end
            self.m_PVEPageView:scrollToPage(curIndex)
        elseif curIndex == self.maxPageIndex then
            self.m_nextBtn:setVisible(false)
        end
        
        local curMapStageId = self.maxStageId %13
        if curMapStageId  < 13 and curIndex == self.maxPageIndex then
            PopDialog:getInstance():popText("本章副本未全部通关")
            return
        elseif curIndex == self.maxPageIndex then
            PopDialog:getInstance():popText("人物等级"..mapConfig[tostring(curIndex)]["level"].."级开放")
            return
        end
        

        
	end
end

--init your ui here
function PVEUILayer:initUI(res)
    self.hasRequestTable = {0,0,0,0,0,0,0}
    
    self.m_PVELayout:setBackGroundImage("ditu.jpg")

    
    self.fromStageId = 0
    local pveType = 0
    local mapId = 0
    if  res then
        if  res.pveType ~= nil then pveType = res.pveType end
        if  res.stageId ~= nil then mapId = res.stageId/13 + 1 self.fromStageId = res.stageId end
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
    
    if  res == nil then
        Http: req("GetAllStage",
            {pveType = pveType,mapId = mapId},
            function(res) 
                if res.err then cclog("err!!!") return end
                self.TagImage:setVisible(true)
                self.sweepCnt = res.data["sweepCnt"]
                self.maxStageId = res.data["maxStageId"]
                --GuildPveStage = self.maxStageId
                self.maxPageIndex = res.data["maxMapId"]
                self.curPageIndex = res.data["maxMapId"]
                self.stageList = res.data["stageList"]
                self:LoadPvePage()
                self:setData(self.curPageIndex)
            end)
         return
    end
    
    if  res.stageId == nil then
        self.TagImage:setVisible(true)
        self.sweepCnt = res.data["sweepCnt"]
        self.maxStageId = res.data["maxStageId"]
        self.maxPageIndex = res.data["maxMapId"]
        self.curPageIndex = res.data["maxMapId"]
        self.stageList = res.data["stageList"]
        self:LoadPvePage()
        self:setData(self.curPageIndex)
    else
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
    

    --[[
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
        ]]
end

--refresh all ui here
function PVEUILayer:refreshUI()
end

function PVEUILayer:changePveType(type)
    self.hasRequestTable = {0,0,0,0,0,0,0}
    if  type == "elite" then
        self.curPveType = 1 
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
        self.curPveType = 0 
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
    page_Layout:setBackGroundImage("map"..index..".png")
    print("pagesize:"..page_Layout:getContentSize().width..","..page_Layout:getContentSize().height)
    self.m_PVEPageView:addPage(page_Layout)
end

--向前插入一页地图
function PVEUILayer:insertPvePage(index)
    local page_Layout = ccui.Helper:seekWidgetByName(self.pageNode,"m_pvepage"..index.."Layout")
    --page_Layout:setBackGroundImage("map"..index..".png")
    page_Layout:removeFromParent(false)
    page_Layout:setBackGroundImage("map"..index..".png")
    print("pagesize:"..page_Layout:getContentSize().width..","..page_Layout:getContentSize().height)
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
            self:requestPVEList(self.curPveType,pageInfo)
        end
    end 
    
    local tagImage = ccui.Helper:seekWidgetByName(self.rootNode, "TagImage")
    self.m_PVEPageView = ccui.PageView:create()
    self.m_PVEPageView:setTouchEnabled(true)
    self.m_PVEPageView:setContentSize(cc.size(860, 548))
    self.m_PVEPageView:setPosition(cc.p(-430,-274))
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
   --page:setBackGroundImage("map"..index..".png")
   self.hasRequestTable[index] = 1
   for i=1,13 do
       local stageBtn =  ccui.Helper:seekWidgetByName(page, "Level"..i.."Btn")
       if i==13 then
            stageBtn:loadTextures("lvdian-03.png","lvdian-04.png","lvdian-04.png")
       else
            stageBtn:loadTextures("lvdian-01.png","lvdian-02.png","lvdian-02.png")
       end
        
       --local stageBGImage =  ccui.Helper:seekWidgetByName(stageBtn,"BgGuangImage")
       --stageBGImage:setScale(1)
       --local seq = cc.Sequence:create(cc.FadeOut:create(1.5),cc.FadeIn:create(1.5))
       --stageBGImage:runAction(cc.Repeat:create(seq,cc.REPEAT_FOREVER))
       local score = self.stageList[i]["score"]
       if   score >0 then
            local starBgImage = ccui.ImageView:create("xindi0.png")
            starBgImage:setScale(1)
            starBgImage:setPosition(cc.p(stageBtn:getCustomSize().width/2,stageBtn:getCustomSize().height/2-30))
            stageBtn:addChild(starBgImage)
       end
        local startBeginPosX = stageBtn:getCustomSize().width*(5+1-score)*0.1
       for j=1,score do
           local starImage = ccui.ImageView:create("xin.png")
           starImage:setScale(0.8)
           starImage:setPosition(cc.p(startBeginPosX+22*(j-1)-5,stageBtn:getCustomSize().height/2-30))
           stageBtn:addChild(starImage)
       end
   end
   -- local draw = cc.DrawNode:create()
    
  -- page:addChild(draw,0)
  local function DrawLineDiy(beginPos,endPos)
    print("draw")
    local midX = (endPos.x+beginPos.x)/2
    local midY = (endPos.y+beginPos.y)/2
    local sprite = cc.Sprite:create("image/ui/pve/xian.png")
    local spriteWidth = 59
    local spriteHeight = 32
    page:addChild(sprite)
    sprite:setPosition(cc.p(midX,midY))
    sprite:setScaleX(math.sqrt(math.pow(math.abs(endPos.x-beginPos.x),2)+math.pow(math.abs(endPos.y-beginPos.y),2))/spriteWidth)
    local angle = PositionToAngle(beginPos,endPos)
    sprite:setRotation(angle)
  end
  
   for i=1,13 do
        local stageBtn =  ccui.Helper:seekWidgetByName(page, "Level"..i.."Btn")
        local posTo = page:convertToNodeSpace(stageBtn:getWorldPosition())
        stageBtn:setTag(self.stageList[i]["stageId"])

        
        if  self.stageList[i]["stageId"]>self.maxStageId then
            --[[
            -- set gray
            local texture = stageBtn:getVirtualRenderer()
            Helper.graySprite(texture)
            stageBtn:setTouchEnabled(false)
            ]]
            stageBtn:setVisible(false)
        elseif self.stageList[i]["stageId"]==self.maxStageId then
            local jian = cc.Sprite:create("image/ui/pve/jian.png")
            local jianHeight = 68
            jian:setPosition(posTo.x,posTo.y+jianHeight/2)
            page:addChild(jian,2)
            local moveBy = cc.MoveBy:create(0.5,cc.p(0,20))
            jian:runAction(cc.RepeatForever:create(cc.Sequence:create(moveBy,moveBy:reverse())))
        end
        
        if  i>1 and self.stageList[i]["stageId"]<=self.maxStageId then
            local j= i-1
            local stageBtnfrom =  ccui.Helper:seekWidgetByName(page, "Level"..j.."Btn")
            local posFrom = page:convertToNodeSpace(stageBtnfrom:getWorldPosition())
            local posTo = page:convertToNodeSpace(stageBtn:getWorldPosition())
            DrawLineDiy(posFrom,posTo)
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
                -- guard_tag --
                self:processGuard()
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
    m_LevelInfoRootLayout:setName("LevelInfo")
    
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
        m_starImage:loadTexture("daxinxin.png")
    end
    
    local stageItem = stageConfig[tostring(stageId)]
    --m_LevelNameLabel: Text
    local m_LevelNameLabel = ccui.Helper:seekWidgetByName(levelInfoNode, "m_LevelNameLabel")
    m_LevelNameLabel:setString(stageItem["stage_name"])
    

    --level desc
    local descLabel = ccui.Helper:seekWidgetByName(levelInfoNode, "m_levelDescLabel")
    descLabel:setString(stageItem["desc"])
    
    --m_cntLabel: Text
    self.m_cntLabel = ccui.Helper:seekWidgetByName(levelInfoNode, "m_cntLabel")
    self:setChallengeCntValue(math.max(attackSum,0),stageItem["day_attack"])
    self.m_cntLabel:setString(math.max(challengeCntValue.totalSum-challengeCntValue.attackSum,0).."/"..challengeCntValue.totalSum)
    
    --m_energyCostLabel: Text
    local m_energyCostLabel = ccui.Helper:seekWidgetByName(levelInfoNode, "m_energyCostLabel")
    m_energyCostLabel:setString(stageItem["use_cp"])

    --rewardList
    local m_rewardListView = ccui.Helper:seekWidgetByName(levelInfoNode, "m_rewardListView")
    
    -----------buy cnt-----------
    --m_buyCntBtn : button
    local vip_level = tostring(Roler:get("vipLevel"))
    local canBuyCntNum = vipConfig[vip_level]["pve_buy_count"]
    
    local m_buyCntBtn = ccui.Helper:seekWidgetByName(levelInfoNode, "m_buyCntBtn")
    --if Roler:get("energy") < tonumber(stageItem["use_cp"]) then
    if  challengeCntValue.totalSum == challengeCntValue.attackSum and canBuyCntNum>buyAttackSum  then
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
            m_buyCntCostLabel:setString(stageBuyConfig[tostring(buyAttackSum+1)]["cost"])
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
            print("type.."..shopItem["type"])
            m_suipianImage:setVisible(true)
        end
        m_rewardListView:pushBackCustomItem(rootNode)
    end
    
    --monster to display
    local m_monsterScrollView = ccui.Helper:seekWidgetByName(levelInfoNode, "m_monsterScrollView")
    --m_monsterImage: ImageView
    local m_monsterImage = ccui.Helper:seekWidgetByName(levelInfoNode, "m_monsterImage")
    local monsterList = {}
    local monsterlist1 = string.split(stageItem["second_monster"],"|")
    local monsterlist2 = string.split(stageItem["third_monster"],"|")
    local monsterlist3 = string.split(stageItem["fourth_monster"],"|")

    table.removebyvalue(monsterlist1,"",true)
    table.removebyvalue(monsterlist2,"",true)
    table.removebyvalue(monsterlist3,"",true)
    table.removebyvalue(monsterlist1,nil,true)
    table.removebyvalue(monsterlist2,nil,true)
    table.removebyvalue(monsterlist3,nil,true)
    monsterList[1] = monsterlist1
    monsterList[2] = monsterlist2
    monsterList[3] = monsterlist3
    
    --print("monsterItem[1]:"..monsterItem[1])
    for i=1,3 do
        local rootNode = m_monsterImage:clone()
        local m_pinzhiImage = ccui.Helper:seekWidgetByName(rootNode, "pinzhiImage")
        rootNode:setVisible(true)
        rootNode:setLocalZOrder(10)
        local tmpList = string.split(monsterList[i][1],",")
        local soulItem = soulConfig[tmpList[1]]
        if  soulItem then
        rootNode:setPosition(50+130*(i-1),50)
        rootNode:setVisible(true)
        rootNode:loadTexture(soulItem["icon"]..".png")
            m_pinzhiImage:loadTexture("normal_aochao_0"..soulItem["quality"]..".png")

        m_monsterScrollView:addChild(rootNode)
        end
    end

    
    --m_challengeBtn: Button
    local m_challengeBtn = ccui.Helper:seekWidgetByName(levelInfoNode, "m_challengeBtn")
    self.challengeBtn = m_challengeBtn
    local function challengeStart(sender,event)
        if sender ==m_challengeBtn and event == cc.EventCode.ENDED then
            pveChallengeStageId = stageId           
            --local FormationUILayer =  require "scene.formation.FormationUILayer"
            --FormationUILayer:setBeforeSceneType("pve")
            Http: req("GetWholeSoul",
                nil,
                function(res) 
                    if res.err then cclog("err!!!") return end
                    local scene = require("scene.formation.FormationScene"):create({BattleType="pve",soul=res.data["soul"]})
                    --local scene = require("scene.battle.BattleScene"):create()
                    cc.Director:getInstance():pushScene(scene)
                end)
          
            
            
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
    
    if  scroe < 3 then
        --m_oneTimesBtn:setVisible(false)
        --m_tenTimesBtn:setVisible(false)
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
    local m_copyImage = ccui.Helper:seekWidgetByName(levelInfoNode, "m_copyImage")
    

    
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
        copyListView:setItemsMargin(10)
        m_RaidsKuangImage:addChild(copyListView)
        copyListView:setTag(5000)
        --copyListView:scrollToTop(0,false)
        m_SweepFinishImage:setVisible(false)
        --m_SweepFinishImage:runAction(cc.Sequence:create(cc.DelayTime:create(1.0*num),cc.Show:create(),cc.Blink:create(0.5,3)))
        local raidsItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("SweepPanel.csb")
        m_raidsLayout:setVisible(true)
        for   i=1,num do
            local copyItem = raidsItem:clone()  
            if  i <1 then copyListView:setContentSize(cc.size(475,150))
            else
                copyListView:setContentSize(m_RaidsListView:getCustomSize())
            end

            copyItem:setVisible(false)
            copyListView:pushBackCustomItem(copyItem)
            local function func()
                local height = 135*(i+1)+10*(i-1)
                if  i == 1 then height = 135
                elseif i== 10 then height = 135*i +10*(i-1)
                end
                copyListView:setInnerContainerSize(cc.size(475,height))
                copyListView:scrollToBottom(1.0,true)
                copyItem:setVisible(true)
            end
            copyItem:runAction(cc.Sequence:create(cc.DelayTime:create(1.0*i),cc.CallFunc:create(func)))

            --m_RaidsListView:scrollToPercentVertical(10,0,false)
            
            
            --copyItem:runAction(cc.Sequence:create(delay,cc.CallFunc:create(scrollTo),delay2,cc.Show:create()))
            local dataItem = param[i]
            
            local m_TaitouLabel = ccui.Helper:seekWidgetByName(copyItem, "m_TaitouLabel")
            local m_coinsLabel = ccui.Helper:seekWidgetByName(copyItem, "m_coinsLabel")
            local m_rewardItemLabel = ccui.Helper:seekWidgetByName(copyItem, "m_rewardItemLabel")
            local m_expLabel = ccui.Helper:seekWidgetByName(copyItem, "m_expLabel")
            local m_rewardItemListView = ccui.Helper:seekWidgetByName(copyItem, "m_rewardItemListView")
            m_TaitouLabel:setString(string.format("第%d轮战斗结果",i))
           
            m_coinsLabel:setString(dataItem["coins"])     
            m_expLabel:setString(dataItem["expReward"][1]["addExp"])
            --reward 取了一个
            local itemRewardList = dataItem["itemReward"]
            for i=1,#itemRewardList do
                local shopItem = shopConfig[tostring(itemRewardList[i]["id"])]
                local num = itemRewardList[i]["num"]
                local type = itemRewardList[i]["type"]
                
                local m_rewardImage = m_copyImage:clone()
                m_rewardImage:setVisible(true)
                local m_pinzhiImage = ccui.Helper:seekWidgetByName(m_rewardImage, "m_pinzhiImage")
                local m_suipianImage = ccui.Helper:seekWidgetByName(m_rewardImage, "m_suipianImage")
                local m_numLabel = ccui.Helper:seekWidgetByName(m_rewardImage, "m_numLabel")
                m_rewardImage:loadTexture("goods/"..shopItem["icon"]..".png")
                m_pinzhiImage:loadTexture("normal_aochao_0"..shopItem["quality"]..".png")
                m_numLabel:setString(num)
                m_suipianImage:setVisible(false)
                if  type == 16 then
                    m_suipianImage:setVisible(true)
                end
                m_rewardItemListView:pushBackCustomItem(m_rewardImage)
            end
            if  #itemRewardList < 1 then
                local label = ccui.Text:create()
                label:setFontSize(20)
                --label:setString("无")
                m_rewardItemListView:pushBackCustomItem(label)
            end
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
                        self:setChallengeCntValue(math.max(attackSum,0),stageItem["day_attack"])
                        self.m_cntLabel:setString(math.max(challengeCntValue.totalSum-challengeCntValue.attackSum,0).."/"..challengeCntValue.totalSum)                       
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
                        self:setChallengeCntValue(math.max(attackSum,0),stageItem["day_attack"])
                        self.m_cntLabel:setString(math.max(challengeCntValue.totalSum-challengeCntValue.attackSum,0).."/"..challengeCntValue.totalSum)                       
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
    
    -- guard_tag --
    --self:processGuard()
end

function PVEUILayer:setChallengeCntValue(value1,value2)
    challengeCntValue.attackSum = value1
    challengeCntValue.totalSum = value2
end

function PVEUILayer:getChallengeCntValue()
    return challengeCntValue
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
        PopDialog:getInstance():popText("金币不足",cc.c3b(0,255,0))
    elseif  _result == 32 then
        cclog("PVE_SWEEP_TEN_VIP_LEVEL_NOT_ENOUGH")
        PopDialog:getInstance():popText("VIP等级不够，不能使用PVE扫荡十次功能",cc.c3b(0,255,0))
    end
    return bRet
end

function PVEUILayer:getChallengeStageId()
    return  pveChallengeStageId
end

return PVEUILayer
