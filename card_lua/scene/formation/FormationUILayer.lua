--FormationUILayer.lua, created by php script. chris.li--
require "Cocos2d"
local PopDialog = require "common/PopDialog"
local onStageId_tab ={} -- 记录位置上的魂魄个体ID   默认位置 5,4,3,2,1 配置默认位置用此表
local onStageTypeId_tab ={} -- 记录位置上的魂魄种类ID
local site_table = {} --上阵魂魄的位置
local fieldId = nil --战斗场次id
local soulConfig = require("config.define_soul")
local npcMapConfig = require("config.define_npc_map")
local pveStageId = require("scene.pve.PVEUILayer")
local juexingConfig = require("config.juexing_skill")
local beforeSceneType = nil --pve book夺书 sport_formation竞技场阵型 sport_battle竞技场战斗 coinsEctype夺宝 guild公会BOSS convoy押镖
local battleType = {pve=0,sport=1,book=2,coins=3,elitePve=4,guild=5,convoy=6}


local FormationUILayer = class("FormationUILayer", function()
	return cc.Layer:create()
end)

function FormationUILayer:create(param)
	local layer = FormationUILayer.new()
    layer:initM(param)
	return layer
end

function FormationUILayer:ctor()
end 

function FormationUILayer:onEnter()
    self:processGuard()
end

function FormationUILayer:onExit()
end

function FormationUILayer:processGuard()
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
        if cmd == nil then return end
        
        if table.getn(self.guildTable) == 0 then
            if cmd.name == "pve_click_hero1btn" then
            cmd.clickTarget = self.hero1Item
            addGuardClickLayer(cmd)
            elseif cmd.name == "pve_click_hero2btn" then
                cmd.clickTarget = self.hero2Item
                addGuardClickLayer(cmd)
            elseif cmd.name == "pve_click_fightbtn" then
                cmd.clickTarget = self.m_challengeBtn
                addGuardClickLayer(cmd)
            end 
         end
    end
    
    if guardCenter:canEnterGuardMode("pve2") == true then
        local cmd = guardCenter:getCmd("pve2")
        if cmd == nil then return end
       -- print("tablenum_"..(table.getn(self.guildTable)))
        --if table.getn(self.guildTable) = 3 then
            if cmd.name == "pve2_click_hero3btn" then
                if table.getn(self.guildTable) == 2 then
                cmd.clickTarget = self.hero2Item
                addGuardClickLayer(cmd)
                end
            elseif cmd.name == "pve2_click_fightbtn" then
                cmd.clickTarget = self.m_challengeBtn
                addGuardClickLayer(cmd)
            end
       -- end
        
    end
    
    if guardCenter:canEnterGuardMode("pve3") == true then
        local cmd = guardCenter:getCmd("pve3")
        if cmd == nil then return end
        if cmd.name == "pve3_click_fightbtn" then
            cmd.clickTarget = self.m_challengeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("pve4") == true then
        local cmd = guardCenter:getCmd("pve4")
        if cmd == nil then return end
        if cmd.name == "pve4_click_fightbtn" then
            cmd.clickTarget = self.m_challengeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("pve5") == true then
        local cmd = guardCenter:getCmd("pve5")
        if cmd == nil then return end
        if cmd.name == "pve5_click_fightbtn" then
            cmd.clickTarget = self.m_challengeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("pve6") == true then
        local cmd = guardCenter:getCmd("pve6")
        if cmd == nil then return end
        if cmd.name == "pve6_click_fightbtn" then
            cmd.clickTarget = self.m_challengeBtn
            addGuardClickLayer(cmd)
        end
    end
    
   --[[ if guardCenter:canEnterGuardMode("sport") == true then
        local cmd = guardCenter:getCmd("sport")
        if cmd == nil then return end
        if cmd.name == "sport_click_fomationicon1" then
            cmd.clickTarget = self.hero1Item
            addGuardClickLayer(cmd)
        elseif cmd.name == "sport_click_fomationicon2" then
            cmd.clickTarget = self.hero2Item
            addGuardClickLayer(cmd)
        elseif cmd.name == "sport_click_fomationfight" then
            cmd.clickTarget = self.m_challengeBtn
            addGuardClickLayer(cmd)
        end
    end--]]
end

function FormationUILayer:initM(param)
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)
    
    self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("FormationUILayer.csb")
    self:addChild(self.rootNode)

    local function touchEventHandler(sender, event)
        self:handleTouchEvent(sender, event)
    end

    --m_FormationLayout: Layout
    self.m_FormationLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_FormationLayout")
    self.m_FormationLayout:addTouchEventListener(touchEventHandler)

    --m_xuanzeImage: ImageView
    self.m_xuanzeImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_xuanzeImage")

    --m_cancelBtn: Button
    self.m_cancelBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_cancelBtn")
    self.m_cancelBtn:addTouchEventListener(touchEventHandler)
    if guardCenter:canEnterGuardMode("pve2") == true or guardCenter:canEnterGuardMode("pve") == true then
        self.m_cancelBtn:setVisible(false)
    end

    --m_allHeroBtn: Button
    self.m_allHeroBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_allHeroBtn")
    self.m_allHeroBtn:addTouchEventListener(touchEventHandler)

    --m_frontHeroBtn: Button
    self.m_frontHeroBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_frontHeroBtn")
    self.m_frontHeroBtn:addTouchEventListener(touchEventHandler)

    --m_middleHeroBtn: Button
    self.m_middleHeroBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_middleHeroBtn")
    self.m_middleHeroBtn:addTouchEventListener(touchEventHandler)

    --m_backHeroBtn: Button
    self.m_backHeroBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_backHeroBtn")
    self.m_backHeroBtn:addTouchEventListener(touchEventHandler)

    --m_herolistScrollView: ScrollView
    self.m_herolistScrollView = ccui.Helper:seekWidgetByName(self.rootNode, "m_herolistScrollView")
    self.m_herolistScrollView:addTouchEventListener(touchEventHandler)

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

    --m_challengeBtn: Button
    self.m_challengeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_challengeBtn")
    self.m_challengeBtn:addTouchEventListener(touchEventHandler)

    --m_onStageImage: ImageView
    self.m_onStageImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_onStageImage")

    --m_fightPowerLabel: Text
    self.m_fightPowerLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_fightPowerLabel")

    self:initUI(param)
end

function FormationUILayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_FormationLayout and event == cc.EventCode.ENDED then
		cclog("[FormationUILayer]--m_FormationLayout touched--")

	elseif sender == self.m_cancelBtn and event == cc.EventCode.ENDED then
		cclog("[FormationUILayer]--m_cancelBtn touched--")
        cc.Director:getInstance():popScene()
        --[[
		if beforeSceneType == "sport_formation" then   
            local scene = require("scene.sport.SportScene.lua"):create()
            cc.Director:getInstance():replaceScene(scene)  
        end
        ]]
        
       -- self:processGuard()
        
	elseif sender == self.m_allHeroBtn and event == cc.EventCode.ENDED then
		cclog("[FormationUILayer]--m_allHeroBtn touched--")
		self.curBtnType = "all"
		self:refreshUI()
        
	elseif sender == self.m_frontHeroBtn and event == cc.EventCode.ENDED then
		cclog("[FormationUILayer]--m_frontHeroBtn touched--")
        self.curBtnType = "front"
        self:refreshUI()

	elseif sender == self.m_middleHeroBtn and event == cc.EventCode.ENDED then
		cclog("[FormationUILayer]--m_middleHeroBtn touched--")
        self.curBtnType = "middle"
        self:refreshUI()

	elseif sender == self.m_backHeroBtn and event == cc.EventCode.ENDED then
		cclog("[FormationUILayer]--m_backHeroBtn touched--")
        self.curBtnType = "back"
        self:refreshUI()
        
	elseif sender == self.m_herolistScrollView and event == cc.EventCode.ENDED then
		cclog("[FormationUILayer]--m_herolistScrollView touched--")

	elseif sender == self.m_challengeBtn and event == cc.EventCode.ENDED then
		cclog("[FormationUILayer]--m_challengeBtn touched--")
		local challenge_tab = {}
		if   table.getn(onStageId_tab) == 0 then
             PopDialog:getInstance():popText("请选择上阵英雄",cc.c3b(0,255,0))
             return 
		end
         for i=1,table.getn(onStageId_tab) do
             table.insert(challenge_tab,onStageId_tab[i])
         end
         --[[
         if table.getn(onStageId_tab)<5 then
         	for i=table.getn(onStageId_tab),4 do
                table.insert(challenge_tab,0)  
         	end
         end
         ]]
         
         
       if beforeSceneType == "pve" then
            Http: req("NewStartCheck",
                {stageId =  pveStageId:getChallengeStageId() ,matrix = challenge_tab},
                function(res) 
                    if res.err then cclog("err!!!") return end
                        if  self:checkResult(res.data["result"]) then

                        local Formation = require "data.Formation"
                        Formation:set("pveAgainTab",challenge_tab)

                        --[[                   
                        local tmpTable = {}
                        table.merge(tmpTable,challenge_tab)
                        local jsonStr = json.encode(tmpTable)
                        cc.UserDefault:getInstance():setStringForKey("pve",jsonStr)
                        ]]
                        self:transFormData(res,battleType.pve)
                        end
                end)
       elseif beforeSceneType == "sport_formation" then
            --发送个体ID 自用种类ID
            Http: req("ChangeSportMatrix",
                {matrix = challenge_tab},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                        local Formation = require "data.Formation"
                        Formation:set("sport_formation",onStageTypeId_tab)
                        Formation:set("fightpower_sport",self.m_fightPowerLabel:getString())
                        cc.Director:getInstance():popScene()
                        local scene = require("scene.sport.SportScene"):create()
                        cc.Director:getInstance():replaceScene(scene)  
                    end
            end)
       elseif beforeSceneType == "sport_battle" then
            local Formation = require "data.Formation"
            local battleData = Formation:get("battledata")
            battleData["matrix"] = challenge_tab
            Http: req("SprotChallenge",
                battleData,
                function(res) 
                    if res.err then cclog("err!!!") return end
                        if  self:checkResult(res.data["result"]) then
                            local BattleScene = require("scene.battle2.BattleScene")
                            cc.Director:getInstance():replaceScene(BattleScene:createPK(res.data))
                            local sportUILayer = require("scene.sport.SportUILayer")
                            sportUILayer:setSportRefresh(true)     
                        end
                end) 
       elseif beforeSceneType == "book" then
            local Formation = require "data.Formation"
            local battleData = Formation:get("battledata")
            battleData["matrix"] = challenge_tab
            Http: req("RobBook",
                battleData,
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                        self:transFormData(res,battleType.book)
                    end
                end)
        elseif beforeSceneType == "coinsEctype" then
            local  battleData= challenge_tab
            Http: req("EnterCoinsEctype",
                {matrix = battleData},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                        self:transFormData(res,battleType.coins)
                    end
                end)
        elseif beforeSceneType == "guild" then
            local battleData = Formation:get("battledata")
            battleData["matrix"] = challenge_tab
            Http: req("ChallengeGuildStageRequest",
                battleData,
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                        self:transFormData(res,battleType.guild)
                    end
                end)
        elseif beforeSceneType == "convoy" then
            Http: req("CheckConvoySoulsRequest",
                {matrix = challenge_tab},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                        local Formation = require "data.Formation"
                        Formation:set("convoy",onStageTypeId_tab)
                        --Formation:set("fightpower",self.m_fightPowerLabel:getString())
                        cc.Director:getInstance():popScene()
                    elseif res.data["result"] == 2 then
                        PopDialog:getInstance():popText("您可同时护送的货车个数已满！",cc.c3b(0,255,0))     
                    end
                end)
        elseif beforeSceneType == "convoyRob" then
            local battleData = Formation:get("battledata")
            battleData["matrix"] = challenge_tab
            Http: req("RobTruckRequest",
                battleData,
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                        --Formation:set("fightpower",self.m_fightPowerLabel:getString())
                        local BattleScene = require("scene.battle2.BattleScene")
                        res.data["bt"] = battleType.convoy
                        cc.Director:getInstance():pushScene(BattleScene:createPK(res.data))     
                    end
                end)  
       end
        local tmpTable = {}
        table.merge(tmpTable,challenge_tab)
        local jsonStr = json.encode(tmpTable)
        local key = string.format("%d%s",Roler:get("roleId"),beforeSceneType)
        cc.UserDefault:getInstance():setStringForKey(key,jsonStr)
           
	end
end

function FormationUILayer:transFormData(res,bt)
    local BattleScene = require("scene.battle2.BattleScene")
    res.data["bt"] = bt
    cc.Director:getInstance():replaceScene(BattleScene:create(res.data))         
end

--init your ui here
function FormationUILayer:initUI(param)
    self.m_FormationLayout:setBackGroundImage("ditu.jpg")
    Helper.addSpriteFrames("FormationUI0.plist","FormationUI0.png")
    cc.Texture2D:setDefaultAlphaPixelFormat(cc.TEXTURE2_D_PIXEL_FORMAT_AUTO)
    self.isMoveActionEable = true
    self.brightJueXingId = {}
    onStageId_tab = {}
    onStageTypeId_tab = {}
    self.curBtnType = "all"
    self.posTable = {}
    for i=1,5 do
        local m_posHeroImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_posHero"..i.."Image")
        local tempX = m_posHeroImage:getPositionX()
        local tempY = m_posHeroImage:getPositionY()
        local pos = cc.p(tempX,tempY)
        table.insert(self.posTable,pos)
    end  
    
    self.m_allHeroBtn:setTouchEnabled(false)
    local btnRender = self.m_allHeroBtn:getVirtualRenderer()
    btnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("quanbu01.png"))
    
    local m_frontHeroRender = self.m_frontHeroBtn:getVirtualRenderer()
    local m_middleHeroBtnRender = self.m_middleHeroBtn:getVirtualRenderer()
    local m_backHeroBtnRender = self.m_backHeroBtn:getVirtualRenderer()

    m_frontHeroRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("qianpai02.png"))
    m_middleHeroBtnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("zhongpai02.png"))
    m_backHeroBtnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("houpai02.png"))
    
    if  param then
        beforeSceneType = param.BattleType
        if beforeSceneType=="convoy" then
            self.allsoulList = param.soul
            self:dealSoulList()           
            self:updateSoulList()
            
            --self:loadPlayerFormation()
            return
        elseif beforeSceneType == "pve" then
            self.allsoulList = param.soul
            self:dealSoulList()           
            self:updateSoulList()
            ----- debug guard ----
            self:loadPlayerFormation()
            return
        end
    end

    if  beforeSceneType == nil then
        beforeSceneType = "pve"
    end
    
    Http: req("GetWholeSoul",
        nil,
        function(res) 
            if res.err then cclog("err!!!") return end
            self.allsoulList = res.data["soul"]
            self:dealSoulList()           
            --self:updateSoulList()
            self:loadPlayerFormation()
        end)
end

--refresh all ui here
function FormationUILayer:refreshUI()
    self.m_allHeroBtn:setTouchEnabled(true)
    self.m_frontHeroBtn:setTouchEnabled(true)
    self.m_middleHeroBtn:setTouchEnabled(true)
    self.m_backHeroBtn:setTouchEnabled(true)

    
    local m_allHeroBtnRender = self.m_allHeroBtn:getVirtualRenderer()
    local m_frontHeroRender = self.m_frontHeroBtn:getVirtualRenderer()
    local m_middleHeroBtnRender = self.m_middleHeroBtn:getVirtualRenderer()
    local m_backHeroBtnRender = self.m_backHeroBtn:getVirtualRenderer()

    m_allHeroBtnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("quanbu02.png"))
    m_frontHeroRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("qianpai02.png"))
    m_middleHeroBtnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("zhongpai02.png"))
    m_backHeroBtnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("houpai02.png"))
    
    if  self.curBtnType == "all" then
        self.m_allHeroBtn:setTouchEnabled(false)
        m_allHeroBtnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("quanbu01.png"))
        self:updateSoulList()
    elseif self.curBtnType == "front" then
        self.m_frontHeroBtn:setTouchEnabled(false)
        m_frontHeroRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("qianpai01.png"))
        self:updateSoulList()
    elseif  self.curBtnType == "middle" then
        self.m_middleHeroBtn:setTouchEnabled(false)
        m_middleHeroBtnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("zhongpai01.png"))
        self:updateSoulList()
    elseif  self.curBtnType == "back" then
        self.m_backHeroBtn:setTouchEnabled(false)
        m_backHeroBtnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("houpai01.png"))
        self:updateSoulList()        
    end
end

function FormationUILayer:dealSoulList()
    -- 5 site
    -- 6 onstage
    self.frontsoulList = {}
    self.middlesoulList = {}
    self.backsoulList = {}
    self.jueXingIdList = {}
	for i=1,table.getn(self.allsoulList) do
	    local soulItem = soulConfig[tostring(self.allsoulList[i]["soulId"])]
        local juexingId = soulItem["blast_book_id"]
        self.allsoulList[i].site = soulItem["site"]
        self.allsoulList[i].onstage = 0
        self.allsoulList[i].juexingId = juexingId
        table.insert(self.jueXingIdList,juexingId)
        --table.insert(self.allsoulList[i],5,soulItem["site"])
        --table.insert(self.allsoulList[i],6,0)
	end
    for i=1,table.getn(self.allsoulList) do
        local site1 = self.allsoulList[i]["site"]/100
        local site =  math.floor(tonumber(site1))
        if  site == 1 then
            table.insert(self.frontsoulList,self.allsoulList[i])
        elseif site == 2 then
            table.insert(self.middlesoulList,self.allsoulList[i])
        elseif site == 3 then
            table.insert(self.backsoulList,self.allsoulList[i])
        end
    end
end
--[[---
    
    
    @param table site_table 已上阵英雄位置表
    @param number site 要上阵英雄的位置

    @return number pos 上阵英雄的插入位置
]]
function FormationUILayer:getRoleInsertPos(sitetable,site)
    local pos = 0
    local site = tonumber(site)
    local over = true
    table.sort(sitetable)
    --print("site:"..site)
    for i=1,table.getn(sitetable) do
        pos = i
        if site < tonumber(sitetable[i]) then
            over = false
        	break
        end
    end
    if  over then pos = pos + 1 end
    if  pos == 0 then pos =1 end
    --[[
    print("pos:"..pos)
    for i=1,table.getn(sitetable) do
        print("table[i]:"..sitetable[i])
    end
    ]]
    return pos
end
--read formation cache
function FormationUILayer:loadPlayerFormation()
    print("[FormationUILayer]:  loadFormation")
    
    local savedTable = {}
    
    self.guildTable = savedTable
    
    local jsonstr = ""
    local key = string.format("%d%s",Roler:get("roleId"),beforeSceneType)
    jsonstr = cc.UserDefault:getInstance():getStringForKey(key)
    if  jsonstr == "" then self:updateSoulList() return end
    local tableTmp = json.decode(jsonstr)
    --print("get str:"..jsonstr)
    table.merge(savedTable,tableTmp)
    
    onStageId_tab = savedTable
    onStageTypeId_tab ={}
    local count = 0
    local copyNum = #onStageId_tab
    local indexj_tab = {}
    if  #onStageId_tab <1 then self:updateSoulList() return end
    local curSoulList ={}
    table.merge(curSoulList,self.allsoulList)
    for i=1,#curSoulList do
        for j=1,#onStageId_tab do
            if  onStageId_tab[j] ==  curSoulList[i]["id"] then
                break
            end
        end
    end
    local beCopyedItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("FormationRoleItem.csb")
    for i=1,#curSoulList do
        local dataItem = curSoulList[i]
        local copyitem =beCopyedItem:clone()
        local indexj = 0
        for j=1,#onStageId_tab do
            if  onStageId_tab[j] == dataItem["id"] then
                indexj = j
                count = count + 1
                --print("loadId:"..dataItem["soulId"])
                table.insert(indexj_tab,indexj)
                curSoulList[i].onstage = 1 --已上阵
            end
        end
        if  indexj ~= 0 and count <=#onStageId_tab then
            local mLevelLabel = ccui.Helper:seekWidgetByName(copyitem, "m_LevelLabel")
            local mpinzhiImage = ccui.Helper:seekWidgetByName(copyitem, "m_pinzhiImage")
            local mtouxiangImage = ccui.Helper:seekWidgetByName(copyitem, "m_touxiangImage")
            local mgrayImage = ccui.Helper:seekWidgetByName(copyitem, "m_grayImage")

            local soulId = dataItem["soulId"]
            local soulItem = soulConfig[tostring(soulId)]
            mLevelLabel:setString(dataItem["level"])
            
            onStageTypeId_tab[indexj] = soulId
            --table.insert(onStageTypeId_tab,indexj,soulId)

            local finalSoulId = soulId %30
            if finalSoulId == 0 then
                finalSoulId = 30
            end

            mtouxiangImage:loadTexture("soul_pic_soulicon"..finalSoulId..".png")
            mpinzhiImage:loadTexture("normal_aochao_0"..soulItem["quality"]..".png")


            local soulItem = soulConfig[tostring(soulId)]
            local movetoPos = self.posTable[indexj]
            copyitem:setPosition(movetoPos)
            copyitem:setAnchorPoint(cc.p(0.5,0.5))
            self.m_xuanzeImage:addChild(copyitem,1)  
            copyitem:setTag(10000+indexj)
            
            --juexing-------begin----------------------------------------
            local juexingId = soulItem["blast_book_id"]
            local juexingData = juexingConfig[tostring(juexingId)]
            local juexingSprite = cc.Sprite:create(juexingData["icon"]..".png")
            local juexingLvNum = juexingData["lv_num"]
            local onJueXingNum = 0
            for i=1,#onStageId_tab do
                if  onStageTypeId_tab[i] ~= nil then
                    local soulItem = soulConfig[tostring(onStageTypeId_tab[i])]
                    local tmp_juexingId = soulItem["blast_book_id"]
                    if  juexingId == tmp_juexingId then onJueXingNum = onJueXingNum +1 end
                end
            end

            if  onJueXingNum < juexingLvNum then
                --if onJueXingNum > juexingLvNum or onJueXingNum < juexingLvNum gray sprite
                Helper.graySprite(juexingSprite)
            else
                local sameJueXingIndex_tab = {}
                --find before same skill index 
                for i=1,#onStageId_tab do
                    local soulItem = soulConfig[tostring(onStageTypeId_tab[i])]
                    if  soulItem ~= nil then
                        local tmp_juexingId = soulItem["blast_book_id"]
                        if  juexingId == tmp_juexingId then
                            if  i ~= indexj then
                                table.insert(sameJueXingIndex_tab,i)
                            end                           
                        end
                    end
                end
                --reset before item[index] data
                for i=1,#sameJueXingIndex_tab do
                    local index = sameJueXingIndex_tab[i]
                    if  index ~= nil then
                        local mItem = self.m_xuanzeImage:getChildByTag(10000+index)
                        if  mItem then
                            local lightedSprite = mItem:getChildByName("juexing")
                            if  lightedSprite then
                                local position = lightedSprite:getPosition()
                                lightedSprite:removeFromParent()
                                local newjuexingSprite = cc.Sprite:create(juexingData["icon"]..".png")
                                newjuexingSprite:setScale(0.5)
                                newjuexingSprite:setPosition(mItem:getContentSize().width/2,mItem:getContentSize().height/2)
                                newjuexingSprite:setName("juexing")
                                mItem:addChild(newjuexingSprite)
                                newjuexingSprite:setVisible(false)
                                newjuexingSprite:runAction(cc.FadeOut:create(0))
                                local action2 = cc.Sequence:create(cc.DelayTime:create(0.2),cc.Show:create(),cc.FadeIn:create(0.5),
                                    cc.MoveBy:create(0.5,cc.p(0,newjuexingSprite:getContentSize().height/2*0.5+mItem:getContentSize().height/2)))
                                newjuexingSprite:runAction(action2)     
                            end      
                        end
                    end
                end
                --set this  item[indexj] ring
                --[[
                local ring = cc.Sprite:create("image/ui/formation/formation_guang.png")
                ring:setColor(cc.c3b(255,255,0))
                ]]
                --[[
                local ring = cc.Sprite:create()
                Helper.addSpriteFrames("effect_formation.plist","effect_formation.png")
                local effectFrameArray = Helper.newFrames("effect_formation%d.png",1,9)
                local animation = Helper.newAnimation(effectFrameArray,1.0/9)
                animation:setLoops(-1)
                ring:setScale(1.5)
                ring:setColor(cc.c3b(255,255,0))
                ring:setPosition(juexingSprite:getContentSize().width/2,juexingSprite:getContentSize().height/2)
                ring:runAction(cc.RepeatForever:create(cc.Animate:create(animation)))
                ]]
                local ring = cc.Sprite:create()
                --ring:setColor(cc.c3b(255,255,0))
                ring:setScale(1.5)
                local armatureFile = "image/ui/formation/yuan/yuan.csb"
                ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
                local skillarmature = ccs.Armature:create("yuan")
                ring:setPosition(juexingSprite:getContentSize().width/2,juexingSprite:getContentSize().height/2)
                ring:addChild(skillarmature)
                --skillarmature:getAnimation():setSpeedScale(0.4)
                skillarmature:getAnimation():playWithIndex(0,0,1) skillarmature:setColor(cc.c3b(255,255,0))
                ring:setName("ring")
                juexingSprite:addChild(ring,1) print("add ring")
                ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)              
            end
            copyitem:addChild(juexingSprite)
            juexingSprite:setScale(0.5)
            juexingSprite:setPosition(copyitem:getContentSize().width/2,copyitem:getContentSize().height/2)
            juexingSprite:setName("juexing")
            juexingSprite:setVisible(false)
            juexingSprite:runAction(cc.FadeOut:create(0))
            local action2 = cc.Sequence:create(cc.DelayTime:create(0.2),cc.Show:create(),cc.FadeIn:create(0.5),
                cc.MoveBy:create(0.5,cc.p(0,juexingSprite:getContentSize().height/2*0.5+copyitem:getContentSize().height/2)))
            juexingSprite:runAction(action2)               
            --juexing--------end-----------------------------------------

            local function backToHeroList(senderin,envenin)
                if senderin == copyitem and envenin == cc.EventCode.ENDED then
                    if  self.isMoveActionEable == false then return end
                    self.isMoveActionEable = false
                    local juexingSprite = copyitem:getChildByName("juexing")
                    if  juexingSprite ~= nil then juexingSprite:removeFromParent() end
                    
                    local indexDelete = copyitem:getTag() - 10000
                    
                    local function grayFunc()
                        local remainJueXingNum = 0
                        local sameJueXingIndex_tab = {}
                        for i=1,#onStageTypeId_tab do
                            local soulItem = soulConfig[tostring(onStageTypeId_tab[i])]
                            local tmp_juexingId = soulItem["blast_book_id"]
                            if  juexingId == tmp_juexingId then 
                                remainJueXingNum = remainJueXingNum +1
                                table.insert(sameJueXingIndex_tab,i)                                     
                            end
                        end
                        if  remainJueXingNum < juexingLvNum then
                            for i=1,#sameJueXingIndex_tab do
                                local index = sameJueXingIndex_tab[i]
                                local mItem = self.m_xuanzeImage:getChildByTag(10000+index)
                                local sprite = mItem:getChildByName("juexing")
                                local ring = sprite:getChildByName("ring")
                                if  ring ~= nil then ring:removeFromParent() end
                                Helper.graySprite(sprite)
                            end
                        else
                            if  #sameJueXingIndex_tab > 0 then
                                for i=1,#sameJueXingIndex_tab do
                                    local index = sameJueXingIndex_tab[i]
                                    local mItem = self.m_xuanzeImage:getChildByTag(10000+index)
                                    local lightedSprite = mItem:getChildByName("juexing")
                                    local position = lightedSprite:getPosition()
                                    lightedSprite:removeFromParent()
                                    local newjuexingSprite = cc.Sprite:create(juexingData["icon"]..".png")
                                    newjuexingSprite:setScale(0.5)
                                    newjuexingSprite:setPosition(mItem:getContentSize().width/2,mItem:getContentSize().height+newjuexingSprite:getContentSize().height/2*0.5)
                                    newjuexingSprite:setName("juexing")
                                    mItem:addChild(newjuexingSprite)
                                end
                                local lightIndex = sameJueXingIndex_tab[#sameJueXingIndex_tab]
                                local mItem = self.m_xuanzeImage:getChildByTag(10000+lightIndex)
                                local lightedSprite = mItem:getChildByName("juexing")
                                local position = lightedSprite:getPosition()
                                lightedSprite:removeFromParent()
                                local newjuexingSprite = cc.Sprite:create(juexingData["icon"]..".png")
                                newjuexingSprite:setScale(0.5)
                                newjuexingSprite:setPosition(mItem:getContentSize().width/2,mItem:getContentSize().height+newjuexingSprite:getContentSize().height/2*0.5)
                                newjuexingSprite:setName("juexing")
                                mItem:addChild(newjuexingSprite)
                                --[[
                                local ring = cc.Sprite:create("image/ui/formation/formation_guang.png")
                                ring:setColor(cc.c3b(255,255,0))
                                ]]
                                --[[
                                local ring = cc.Sprite:create()
                                Helper.addSpriteFrames("effect_formation.plist","effect_formation.png")
                                local effectFrameArray = Helper.newFrames("effect_formation%d.png",1,9)
                                local animation = Helper.newAnimation(effectFrameArray,1.0/9)
                                animation:setLoops(-1)
                                ring:setScale(1.5)
                                ring:setPosition(newjuexingSprite:getContentSize().width/2,newjuexingSprite:getContentSize().height/2)
                                ]]
                                local ring = cc.Sprite:create()
                                --ring:setColor(cc.c3b(255,255,0))
                                ring:setScale(1.5)
                                local armatureFile = "image/ui/formation/yuan/yuan.csb"
                                ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
                                local skillarmature = ccs.Armature:create("yuan")
                                ring:setPosition(newjuexingSprite:getContentSize().width/2,newjuexingSprite:getContentSize().height/2)
                                ring:addChild(skillarmature)
                                --skillarmature:getAnimation():setSpeedScale(0.4)
                                skillarmature:getAnimation():playWithIndex(0,0,1) skillarmature:setColor(cc.c3b(255,255,0))
                                ring:setName("ring")
                                newjuexingSprite:addChild(ring)
                                ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
                            end
                        end
                    end
                    
                    local function destoryFuc()
                        copyitem:removeFromParent(true)
                        table.remove(onStageId_tab,indexDelete)
                        local soulIdRemovePos = table.indexof(onStageTypeId_tab,soulId)
                        table.remove(onStageTypeId_tab,soulIdRemovePos)
                        curSoulList[i]["onstage"] = 0
                        self:updateSoulList()                                     
                    end           
                    local function Movefunc()
                        for i=indexDelete,table.getn(onStageId_tab) do
                            local topos = self.posTable[tonumber(i)]
                            local mItem = self.m_xuanzeImage:getChildByTag(10000+i+1)                                
                            local moveAction = cc.MoveTo:create(0.2,topos)
                            mItem:runAction(moveAction)
                            mItem:setTag(10000+i)
                        end
                    end   
                    
                    local function loadStart()
                        --self.m_FormationLayout:setEnabled(false)
                        self.isMoveActionEable = false
                    end

                    local function loadStop()
                        --self.m_FormationLayout:setEnabled(true)
                        self.isMoveActionEable = true
                    end
                    
                    --match juexingSkill begin
                    local function finaMatch()
                        self:findMatchJueXingHero()
                    end

                    --match end
                    
                    local action = cc.MoveBy:create(0.2,cc.p(0,100))
                    local fadeout = cc.FadeOut:create(0.2)
                    local spwan = cc.Spawn:create(action,fadeout)
                    local sequence = cc.Sequence:create(spwan,cc.CallFunc:create(destoryFuc),cc.CallFunc:create(Movefunc),cc.CallFunc:create(grayFunc))
                    copyitem:runAction(cc.Sequence:create(cc.CallFunc:create(loadStart),sequence,
                                                          cc.CallFunc:create(finaMatch),cc.CallFunc:create(loadStop)))          
                end
            end
            copyitem:addTouchEventListener(backToHeroList)
            if count == #onStageId_tab then
                --self:updateSoulList()
                if  self:findMatchJueXingHero() == false then
                    self:updateSoulList()
                end
                return
            end
        end      
    end
    if  count ~= copyNum then
        for i=1,#indexj_tab do
            local mItem = self.m_xuanzeImage:getChildByTag(10000+indexj_tab[i])
            mItem:removeFromParent()
        end
        onStageId_tab = {}
        local tmpTable = {}
        local jsonStr = json.encode(onStageId_tab)
        local key = string.format("%d%s",Roler:get("roleId"),beforeSceneType)
        cc.UserDefault:getInstance():setStringForKey(key,jsonStr)
        self:updateSoulList()
    end
    
end

function FormationUILayer:updateSoulList()
    if  self.m_herolistScrollView:getChildrenCount() ~=0 then
        self.m_herolistScrollView:removeAllChildren()
    end
   print("update soul")
    --onStageId_tab = {}
    local curSoulList ={}
    
    if  self.curBtnType == "all" then
        table.merge(curSoulList,self.allsoulList)
    elseif self.curBtnType == "front" then
        table.merge(curSoulList,self.frontsoulList)     
    elseif  self.curBtnType == "middle" then
        table.merge(curSoulList,self.middlesoulList)
    elseif  self.curBtnType == "back" then
        table.merge(curSoulList,self.backsoulList )  
    end
    
    
    local ScrovSize = self.m_herolistScrollView:getInnerContainerSize()

    local ScrovH = ScrovSize.height
    local roleSize = table.getn(curSoulList)
    if roleSize == nil then
    	roleSize = 0
    end
    ScrovH = math.max((130)*(math.floor((roleSize-1)/6)+1),250)


    ScrovSize.height = ScrovH
    --print(ScrovSize.height)
    self.m_herolistScrollView:setInnerContainerSize(ScrovSize)
    local beCopyedItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("FormationRoleItem.csb")
    --print("copy csb")
    local fightPower = 0
    if  roleSize == 0 then self.m_fightPowerLabel:setString(0) end
    --table.sort(curSoulList,function(a, b) return a.id < b.id end)
    local onStageId_tab_copy = {}
    table.merge(onStageId_tab_copy,onStageId_tab)
    table.sort(onStageId_tab_copy)
    for i=1,roleSize do
        local dataItem = curSoulList[i]
        local roleitem =beCopyedItem:clone()
        roleitem:setAnchorPoint(0.5,0.5)
        -- guard_tag --
        if i==1 then self.hero1Item = roleitem print("hero1Item") end
        if i==2 then self.hero2Item = roleitem print("hero2Item") end
        if i==3 then self.hero3Item = roleitem print("hero2Item") end
        roleitem:setPosition(71+(i-1)%6*130,ScrovSize.height+65-130*(math.floor((i-1)/6)+1))
        self.m_herolistScrollView:addChild(roleitem)
        roleitem:setTag(dataItem["id"])
        
        local mLevelLabel = ccui.Helper:seekWidgetByName(roleitem, "m_LevelLabel")
        local mpinzhiImage = ccui.Helper:seekWidgetByName(roleitem, "m_pinzhiImage")
        local mtouxiangImage = ccui.Helper:seekWidgetByName(roleitem, "m_touxiangImage")
        local mgrayImage = ccui.Helper:seekWidgetByName(roleitem, "m_grayImage")
        
        local soulId = dataItem["soulId"]
        local soulItem = soulConfig[tostring(soulId)]
        mLevelLabel:setString(dataItem["level"])

        local juexingId = soulItem["blast_book_id"]
        
        if  #self.brightJueXingId > 0 then
            for i=1,#self.brightJueXingId do
                local brightJueXingId = self.brightJueXingId[i]
                if juexingId == brightJueXingId and dataItem["onstage"] == 0 and dataItem["gray"] == 0  then
                    if roleitem:getChildByName("brightJueXing") == nil then
                        local armatureFile = "image/ui/formation/heffect/heffect.csb"
                        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
                        local skillarmature = ccs.Armature:create("heffect")
                        skillarmature:setScale(2.2)
                        skillarmature:setPosition(51,-15)
                        skillarmature:setName("brightJueXing")
                        roleitem:addChild(skillarmature)
                        --速度
                        skillarmature:getAnimation():setSpeedScale(0.4)
                        skillarmature:getAnimation():playWithIndex(0,0,1)
                        ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
                    end                    
                end
            end

        end
        
        local finalSoulId = soulId %30
        if finalSoulId == 0 then
        	finalSoulId = 30
        end
        
        mtouxiangImage:loadTexture("soul_pic_soulicon"..finalSoulId..".png")
        mpinzhiImage:loadTexture("normal_aochao_0"..soulItem["quality"]..".png")
        
        local starNum = soulItem["quality"]
        local startBeginPosX = roleitem:getCustomSize().width*(5+1-starNum)*0.1
        for i=1,starNum do
        	local star = cc.Sprite:create("xing01.png")
        	star:setScale(0.5)
        	star:setPosition(cc.p(startBeginPosX+20*(i-1),5))
            roleitem:addChild(star)
        end
        
        if  dataItem["onstage"] == 1 then
            roleitem:setTouchEnabled(false)
            mgrayImage:setVisible(true)
        else
            roleitem:setTouchEnabled(true)
            mgrayImage:setVisible(false)    
        end
        
        if  dataItem["gray"] == 1 then
            roleitem:setTouchEnabled(false)
            mgrayImage:setVisible(true)
        end
        
        --本地存储
        if #onStageId_tab ~= 0 then
            for i=1,#onStageId_tab_copy do
                if  onStageId_tab_copy[i] == dataItem["id"] then
                    mgrayImage:setVisible(true)
                    --print(dataItem["id"])
                    fightPower = fightPower + dataItem["fc"]
                end
            end           
        end
        self.m_fightPowerLabel:setString(self:countFightPower(fightPower))
        
        --上阵方法
        local function moveToFormationList(sender,event)
            if sender == roleitem and event == cc.EventCode.ENDED then
                -- guard_tag --
                if sender == self.hero1Item then
                    Helper.setTimeout(self, 10, function()self:processGuard()end)
                end
                
                if sender == self.hero2Item then
                    Helper.setTimeout(self, 1, function()self:processGuard()end)
                end
                
                if sender == self.hero3Item then
                    Helper.setTimeout(self, 1, function()self:processGuard()end)
                end
                
                if self.isMoveActionEable == false then return end
                self.isMoveActionEable = false
                --print("soulId:"..soulId)
                local beforeinsert_tab = {}
                local beforeinserttype_tab = {}
                table.merge(beforeinsert_tab,onStageId_tab)
                table.merge(beforeinserttype_tab,onStageTypeId_tab)
                if table.getn(beforeinsert_tab) >=5 then print("over") self.isMoveActionEable =true return end
                
                --table.insert(onStageTypeId_tab,soulId)
                local brightEffect = roleitem:getChildByName("brightJueXing")
                if  brightEffect ~= nil then brightEffect:removeFromParent() end

                local copyitem = roleitem:clone()

                local postransform = self.m_xuanzeImage:convertToNodeSpace(roleitem:getWorldPosition())
                --local posBegin = cc.p(postransform.x+roleitem:getCustomSize().width/2,postransform.y+roleitem:getCustomSize().height/2)
                local posBegin = cc.p(postransform.x,postransform.y)        
                copyitem:setPosition(posBegin)

                self.m_xuanzeImage:addChild(copyitem,1)          
                
                --插入位置
                site_table={}
                for i=1,table.getn(beforeinserttype_tab) do
                    local soulItem = soulConfig[tostring(beforeinserttype_tab[i])]
                    --print("soulItem[site]:"..soulItem["site"])
                    --print("beforeinserttype_tab[i]:"..beforeinserttype_tab[i])
                    table.insert(site_table,soulItem["site"])
                end
                local soulItem = soulConfig[tostring(soulId)]
                local indexInsert = self:getRoleInsertPos(site_table,soulItem["site"])--要移动到的位置 
                table.insert(onStageId_tab,indexInsert,roleitem:getTag())
                
                table.insert(onStageTypeId_tab,indexInsert,soulId)
                --[[
                print("英雄Id"..roleitem:getTag())               
                for i=1,table.getn(beforeinsert_tab) do
                    print("上阵前英雄id："..beforeinsert_tab[i])
                end
                print("")
                for i=1,table.getn(onStageId_tab) do
                    print("上阵后英雄id："..onStageId_tab[i])
                end
                --]]
                
                --调整插入之前英雄位置
                for i=table.getn(beforeinsert_tab),indexInsert,-1 do
                    local topos = self.posTable[tonumber(i+1)]
                    local mItem = self.m_xuanzeImage:getChildByTag(10000+i)
                    mItem:runAction(cc.MoveTo:create(0.2,topos))
                    mItem:setTag(10000+i+1)
                end
                copyitem:setTag(indexInsert+10000)
                
                fightPower = fightPower + dataItem["fc"]
                self.m_fightPowerLabel:setString(self:countFightPower(fightPower))
                
                local movetoPos = self.posTable[tonumber(indexInsert)]
                local action = cc.MoveTo:create(0.2,movetoPos)
                
                local function loadStart()
                    --self.m_FormationLayout:setEnabled(false)
                    self.isMoveActionEable = false
                end
                
                local function loadStop()
                    --self.m_FormationLayout:setEnabled(true)
                    self.isMoveActionEable  =true
                end
                
                local function adjustEndPosition()
                    --移动中可能会出现有些移不到位置，此处调整位置
                    for i=1,#onStageId_tab do
                        local topos = self.posTable[tonumber(i)]
                        local mItem = self.m_xuanzeImage:getChildByTag(10000+i)
                        mItem:runAction(cc.MoveTo:create(0.1,topos))
                    end
                end
                

                --match juexingSkill begin
                local function finaMatch()
                    -- guard_tag --
                    if guardCenter:canEnterGuardMode("pve") == false then
                        self:findMatchJueXingHero()
                    end
                end

                --match end
                
                local copysequence = cc.Sequence:create(cc.CallFunc:create(loadStart),action,cc.CallFunc:create(loadStop),
                    cc.CallFunc:create(adjustEndPosition),cc.CallFunc:create(finaMatch))
                
                copyitem:runAction(copysequence)
                
                copyitem:setAnchorPoint(cc.p(0.5,0.5))
            
                mgrayImage:setVisible(true)
                roleitem:setTouchEnabled(false)
                curSoulList[i]["onstage"] = 1
                
                --juexing----begin---------------------------------------------------
                local juexingData = juexingConfig[tostring(juexingId)]
                local juexingSprite = cc.Sprite:create(juexingData["icon"]..".png")
                local juexingLvNum = juexingData["lv_num"]
                local onJueXingNum = 0
                for i=1,#onStageTypeId_tab do
                    local soulItem = soulConfig[tostring(onStageTypeId_tab[i])]
                    local tmp_juexingId = soulItem["blast_book_id"]
                    if  juexingId == tmp_juexingId then onJueXingNum = onJueXingNum +1 end
                end
                
                if  onJueXingNum < juexingLvNum then
                    --if onJueXingNum > juexingLvNum or onJueXingNum < juexingLvNum gray sprite
                    Helper.graySprite(juexingSprite)
                else
                    --reset the same skill before
                    local sameJueXingIndex_tab = {}
                    for j=1,#onStageTypeId_tab do
                        local soulItem = soulConfig[tostring(onStageTypeId_tab[j])]
                        local tmp_juexingId = soulItem["blast_book_id"]
                        if  juexingId == tmp_juexingId then 
                            --print("indexInsert:"..indexInsert)
                            if j~= indexInsert then
                                table.insert(sameJueXingIndex_tab,j)
                            end                                   
                        end
                    end

                    for i=1,#sameJueXingIndex_tab do
                        local index = sameJueXingIndex_tab[i]
                        local mItem = self.m_xuanzeImage:getChildByTag(10000+index)
                        local lightedSprite = mItem:getChildByName("juexing")
                        local position = lightedSprite:getPosition()
                        lightedSprite:removeFromParent()
                        local newjuexingSprite = cc.Sprite:create(juexingData["icon"]..".png")
                        newjuexingSprite:setScale(0.5)
                        newjuexingSprite:setPosition(mItem:getContentSize().width/2,mItem:getContentSize().height+newjuexingSprite:getContentSize().height/2*0.5)
                        newjuexingSprite:setName("juexing")
                        mItem:addChild(newjuexingSprite)
                    end  
                    --[[
                    local ring = cc.Sprite:create("image/ui/formation/formation_guang.png")
                    ring:setScale(1.5)
                    ring:setColor(cc.c3b(255,255,0))
                    ring:setPosition(juexingSprite:getContentSize().width/2,juexingSprite:getContentSize().height/2+2)
                    local seq1 = cc.Sequence:create(cc.FadeIn:create(0.5),cc.FadeOut:create(0.5))
                    local scaleBy = cc.ScaleBy:create(0.5,1.1)
                    local seq2 = cc.Sequence:create(scaleBy,scaleBy:reverse())
                    ring:runAction(cc.RepeatForever:create(cc.Spawn:create(seq1,seq2)))
                    ]]
                    --[[
                    local ring = cc.Sprite:create()
                    Helper.addSpriteFrames("effect_formation.plist","effect_formation.png")
                    local effectFrameArray = Helper.newFrames("effect_formation%d.png",1,9)
                    local animation = Helper.newAnimation(effectFrameArray,1.0/9)
                    animation:setLoops(-1)
                    ring:setScale(1.5)
                    ring:setColor(cc.c3b(255,255,0))
                    ring:setPosition(juexingSprite:getContentSize().width/2,juexingSprite:getContentSize().height/2)
                    ring:runAction(cc.RepeatForever:create(cc.Animate:create(animation)))
                    ]]
                    local ring = cc.Sprite:create()
                    --ring:setColor(cc.c3b(255,255,0))
                    ring:setScale(1.5)
                    local armatureFile = "image/ui/formation/yuan/yuan.csb"
                    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
                    local skillarmature = ccs.Armature:create("yuan")
                    ring:setPosition(juexingSprite:getContentSize().width/2,juexingSprite:getContentSize().height/2)
                    ring:addChild(skillarmature)
                    --skillarmature:getAnimation():setSpeedScale(0.4)
                    skillarmature:getAnimation():playWithIndex(0,0,1) skillarmature:setColor(cc.c3b(255,255,0))
                    ring:setName("ring")
                    juexingSprite:addChild(ring,1) print("add ring")
                    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
                end
                
                copyitem:addChild(juexingSprite)
                juexingSprite:setScale(0.5)
                juexingSprite:setPosition(copyitem:getContentSize().width/2,copyitem:getContentSize().height/2)
                juexingSprite:setName("juexing")
                juexingSprite:setVisible(false)
                juexingSprite:runAction(cc.FadeOut:create(0))
                local action2 = cc.Sequence:create(cc.DelayTime:create(0.2),cc.Show:create(),cc.FadeIn:create(0.5),
                                                   cc.MoveBy:create(0.5,cc.p(0,juexingSprite:getContentSize().height/2*0.5+copyitem:getContentSize().height/2)))
                juexingSprite:runAction(action2)                
                --juexing----end-----------------------------------------------------
                


                
                --下阵方法
                local function backToHeroList(senderin,envenin)
                    if senderin == copyitem and envenin == cc.EventCode.ENDED then
                        if self.isMoveActionEable == false then return end
                        self.isMoveActionEable = false 
                        copyitem:setVisible(true)
                        local juexingSprite = copyitem:getChildByName("juexing")
                        if  juexingSprite ~= nil then juexingSprite:removeFromParent() end
                        
                        local function grayFunc()
                            local remainJueXingNum = 0
                            local sameJueXingIndex_tab = {}
                            for i=1,#onStageTypeId_tab do
                                local soulItem = soulConfig[tostring(onStageTypeId_tab[i])]
                                local tmp_juexingId = soulItem["blast_book_id"]
                                if  juexingId == tmp_juexingId then 
                                    remainJueXingNum = remainJueXingNum +1
                                    table.insert(sameJueXingIndex_tab,i)                                     
                                end
                            end
                            if  remainJueXingNum < juexingLvNum then
                                for i=1,#sameJueXingIndex_tab do
                                    local index = sameJueXingIndex_tab[i]
                                    local mItem = self.m_xuanzeImage:getChildByTag(10000+index)
                                    local sprite = mItem:getChildByName("juexing")
                                    local ring = sprite:getChildByName("ring")
                                    if  ring ~= nil then ring:removeFromParent() end
                                    Helper.graySprite(sprite)
                                end
                            else
                                for i=1,#sameJueXingIndex_tab-1 do
                                    local index = sameJueXingIndex_tab[i]
                                    local mItem = self.m_xuanzeImage:getChildByTag(10000+index)
                                    local sprite = mItem:getChildByName("juexing")
                                    local ring = sprite:getChildByName("ring")
                                    if  ring ~= nil then ring:removeFromParent() end
                                end
                                local lightIndex = sameJueXingIndex_tab[#sameJueXingIndex_tab]
                                if  lightIndex ~= nil then
                                    local mItem = self.m_xuanzeImage:getChildByTag(10000+sameJueXingIndex_tab[#sameJueXingIndex_tab])
                                    local lightedSprite = mItem:getChildByName("juexing")
                                    local position = lightedSprite:getPosition()
                                    lightedSprite:removeFromParent()
                                    local newjuexingSprite = cc.Sprite:create(juexingData["icon"]..".png")
                                    newjuexingSprite:setScale(0.5)
                                    newjuexingSprite:setPosition(mItem:getContentSize().width/2,mItem:getContentSize().height+newjuexingSprite:getContentSize().height/2*0.5)
                                    newjuexingSprite:setName("juexing")
                                    mItem:addChild(newjuexingSprite)
                                    --[[
                                    local ring = cc.Sprite:create("formation_guang.png")
                                    ring:setColor(cc.c3b(255,255,0))]]
                                    --[[
                                    local ring = cc.Sprite:create()
                                    Helper.addSpriteFrames("effect_formation.plist","effect_formation.png")
                                    local effectFrameArray = Helper.newFrames("effect_formation%d.png",1,9)
                                    local animation = Helper.newAnimation(effectFrameArray,1.0/9)
                                    animation:setLoops(-1)
                                    ring:setScale(1.5)       
                                    ring:setColor(cc.c3b(255,255,0))
                                    ring:setPosition(newjuexingSprite:getContentSize().width/2,newjuexingSprite:getContentSize().height/2)                                    
                                    ring:runAction(cc.RepeatForever:create(cc.Animate:create(animation)))
                                    ]]
                                    local ring = cc.Sprite:create()
                                    --ring:setColor(cc.c3b(255,255,0))
                                    ring:setScale(1.5) 
                                    local armatureFile = "image/ui/formation/yuan/yuan.csb"
                                    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
                                    local skillarmature = ccs.Armature:create("yuan")
                                    ring:setPosition(newjuexingSprite:getContentSize().width/2,newjuexingSprite:getContentSize().height/2)
                                    ring:addChild(skillarmature)
                                    --skillarmature:getAnimation():setSpeedScale(0.4)
                                    skillarmature:getAnimation():playWithIndex(0,0,1) skillarmature:setColor(cc.c3b(255,255,0))
                                    ring:setName("ring")
                                    newjuexingSprite:addChild(ring,1) print("add ring")
                                    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)   
                                end                              
                            end
                        end
                        
                        
                        
                        local indexDelete = copyitem:getTag()-10000
                        
                        
                        
                        local function destoryFuc()
                            copyitem:removeFromParent(true)
                            table.remove(onStageId_tab,indexDelete)
                            local soulIdRemovePos = table.indexof(onStageTypeId_tab,soulId)
                            table.remove(onStageTypeId_tab,soulIdRemovePos)
                            --table.remove(onStageTypeId_tab,indexDelete)
                            
                            fightPower = fightPower - dataItem["fc"]
                            self.m_fightPowerLabel:setString(self:countFightPower(fightPower))
                            curSoulList[i]["onstage"] = 0
                            self:updateSoulList()                                     
                        end           
                        
                        --删除之后调整英雄位置
                        local function Movefunc()
                            for i=indexDelete,table.getn(onStageId_tab) do
                                local topos = self.posTable[tonumber(i)]
                                local mItem = self.m_xuanzeImage:getChildByTag(10000+i+1)                                
                                local moveAction = cc.MoveTo:create(0.2,topos)
                                mItem:runAction(moveAction)
                                mItem:setTag(10000+i)
                            end
                        end   
                        
                        local function loadStart()
                            --self.m_FormationLayout:setEnabled(false)
                            self.isMoveActionEable = false
                        end

                        local function loadStop()
                            --self.m_FormationLayout:setEnabled(true)
                            self.isMoveActionEable = true
                        end
                        
                        --match juexingSkill begin
                        local function finaMatch()
                            self:findMatchJueXingHero()
                        end

                        --match end
                        
                        local action = cc.MoveTo:create(0.2,posBegin)
                        local fadeout = cc.FadeOut:create(0.2)
                        local spwan = cc.Spawn:create(action,fadeout)
                        local sequence = cc.Sequence:create(spwan,cc.CallFunc:create(destoryFuc),cc.CallFunc:create(Movefunc),cc.CallFunc:create(grayFunc))
                        copyitem:runAction(cc.Sequence:create(cc.CallFunc:create(loadStart),sequence,cc.CallFunc:create(finaMatch),cc.CallFunc:create(loadStop)))
                    end
                end
                copyitem:addTouchEventListener(backToHeroList)
            end   
        end

        roleitem:addTouchEventListener(moveToFormationList)
    end
end

function FormationUILayer:findMatchJueXingHero()
    local onStagejueXingIdList = {}
    local onStagejueXingNumList = {}
    for i=1,#onStageTypeId_tab do
        local soulItem = soulConfig[tostring(onStageTypeId_tab[i])]
        local tmpId = soulItem["blast_book_id"]
        local juexingData = juexingConfig[tostring(tmpId)]
        local tmpNum = juexingData["lv_num"]
        table.insert(onStagejueXingIdList,tmpId)
        table.insert(onStagejueXingNumList,tmpNum)
    end
    local remainNum = 5 - #onStageTypeId_tab
    if  remainNum == 0 then self.brightJueXingId = {} self:updateSoulList() return false end
    if  remainNum ==5 then self.brightJueXingId = {} self:updateSoulList() end
    -- find whether match success
    local onstageMatchSuccess ={}
    for i=1,#onStageTypeId_tab do
        local id = onStagejueXingIdList[i]
        local num = onStagejueXingNumList[i]
        local count = 0
        for j=1,#onStageTypeId_tab do
            if  id == onStagejueXingIdList[j] then
                count = count +1
            end
        end
        if  count >= num then
            table.insert(onstageMatchSuccess,1)
        else
            table.insert(onstageMatchSuccess,0)
        end
    end
    self.brightJueXingId = {}
    for i=1,#onStageTypeId_tab do
        local id = onStagejueXingIdList[i]
        local num = onStagejueXingNumList[i]
	   local count = 0
	   for j=1,#self.jueXingIdList do
	       if self.jueXingIdList[j] == id then
	           count = count+1
	       end
	   end
	   if  count >= num and count-1 <= remainNum and onstageMatchSuccess[i] == 0 then
	       table.insert(self.brightJueXingId,id)
           print("id:"..id.."_count:"..count)
	   end
	end
    if  #self.brightJueXingId > 0 then
        self:updateSoulList() 
        return true
    end
	return false
end

function FormationUILayer:getBattleFieldId()
	return fieldId
end

function FormationUILayer:setBeforeSceneType(param)
    beforeSceneType = param
end

function FormationUILayer:countFightPower(totalpower)
    local n = #onStageId_tab
    if  n and n~=0 then
        local average = totalpower/n
        local power = math.floor(math.pow(n,2)*average)
        return power
    end
    return 0
end

function FormationUILayer:checkResult(_result)
    --pve还有一个
    local bRet=false
    if _result== 0 then 
        bRet = true
    elseif  _result == 1 then
        cclog("PVE_CP_NOT_ENOUGH")
        PopDialog:getInstance():popText("体力不足",cc.c3b(0,255,0))  
    elseif  _result == 11 then
        cclog("DAY_ATTACK_ENOUGH")
        PopDialog:getInstance():popText("该关卡已达到每日可以攻打次数上限",cc.c3b(0,255,0))
    elseif  _result == 15 then
        cclog("COINS_ECTYPE_NOT_ENOUGH")
        PopDialog:getInstance():popText("铜钱本挑战次数已用完",cc.c3b(0,255,0))
    elseif  _result == 16 then
        cclog("PVE_NODE_PRE_NOT_PASS")
        PopDialog:getInstance():popText("前置副本未通过",cc.c3b(0,255,0))
    elseif  _result == 17 then
        cclog("PVE_NODE_LEVEL_NOT_PASS")
        PopDialog:getInstance():popText("玩家等级不足不允许进入",cc.c3b(0,255,0))
    elseif  _result == 18 then
        cclog("SPORT_CHALLENGE_CD")
        PopDialog:getInstance():popText("竞技场挑战CD中",cc.c3b(0,255,0))
    elseif  _result == 19 then
        cclog("BOOK_ROB_COUNT_NOT_ENOUGH")
        PopDialog:getInstance():popText("夺书次数已用完",cc.c3b(0,255,0))
    elseif  _result == 20 then
        cclog("SPORT_CHALLENGE_REFRESH")
        PopDialog:getInstance():popText("请刷新挑战列表",cc.c3b(0,255,0))
    elseif  _result == 21 then
        cclog("SPORT_CHALLENGE_COUNT_NOT_ENOUGH")
        PopDialog:getInstance():popText("竞技场挑战次数不足",cc.c3b(0,255,0))
    elseif  _result == 22 then
        cclog("PVE_SWEEP_NOT_ENOUGH")
        PopDialog:getInstance():popText("扫荡券不足",cc.c3b(0,255,0)) 
    elseif  _result == 23 then
        cclog("PVE_BUY_COUNT_NOT_ENOUGH")
        PopDialog:getInstance():popText("PVE购买挑战次数已用完",cc.c3b(0,255,0)) 
    elseif  _result == 24 then
        cclog("PVE_NODE_NOT_THREE_STAR")
        PopDialog:getInstance():popText("副本未三星通关",cc.c3b(0,255,0)) 
    elseif  _result == 25 then
        cclog("PVE_GOLD_NOT_ENOUGH")
        PopDialog:getInstance():popText("金币不足",cc.c3b(0,255,0))
    elseif  _result == 26 then
        cclog("GUILD_ATTACK_NUM_NOT_ENOUGH")
        PopDialog:getInstance():popText("公会副本挑战次数不足",cc.c3b(0,255,0))
    elseif  _result == 27 then
        cclog("GUILD_ATTACK_NOW")
        PopDialog:getInstance():popText("已有玩家正在挑战副本",cc.c3b(0,255,0))
    elseif  _result == 28 then
        cclog("GUILD_ATTACK_REST")
        PopDialog:getInstance():popText("每天00:00-8:00不能挑战公会副本",cc.c3b(0,255,0))
    elseif  _result == 29 then
        cclog("CONVOY_ROB_PROTECT_CD")
        PopDialog:getInstance():popText("护送保护CD中",cc.c3b(0,255,0))
    elseif  _result == 30 then
        cclog("CONVOY_ROB_NOW")
        PopDialog:getInstance():popText("有人正在抢劫护送中",cc.c3b(0,255,0))                       
    end
    return bRet
end

return FormationUILayer
