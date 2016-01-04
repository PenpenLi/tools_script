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
local beforeSceneType = nil --pve book夺书 sport_formation竞技场阵型 sport_battle竞技场战斗 coinsEctype夺宝 guild公会BOSS convoy押镖

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
end

function FormationUILayer:onExit()
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
                        --[[
                        local Formation = require "data.Formation"
                        Formation:set("pve",challenge_tab)
                        
                        ]]
                        --[[                   
                        local tmpTable = {}
                        table.merge(tmpTable,challenge_tab)
                        local jsonStr = json.encode(tmpTable)
                        cc.UserDefault:getInstance():setStringForKey("pve",jsonStr)
                        ]]
                        self:transFormData(res)
                        end
                end)
       elseif beforeSceneType == "sport_formation" then
            --发送个体ID 自用种类ID
            Http: req("ChangeSportMatrix",
                {matrix = challenge_tab},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    local Formation = require "data.Formation"
                    Formation:set("sport_formation",onStageTypeId_tab)
                    Formation:set("fightpower",self.m_fightPowerLabel:getString())
                    cc.Director:getInstance():popScene()
                    local scene = require("scene.sport.SportScene"):create()
                    cc.Director:getInstance():replaceScene(scene)  
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
                        self:transFormData(res)
                    end
                end)
        elseif beforeSceneType == "coinsEctype" then
            local  battleData= challenge_tab
            Http: req("EnterCoinsEctype",
                {matrix = battleData},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    if  self:checkResult(res.data["result"]) then
                        self:transFormData(res)
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
                        self:transFormData(res)
                    end
                end)
        elseif beforeSceneType == "convoy" then
            Http: req("CheckConvoySoulsRequest",
                {matrix = challenge_tab},
                function(res) 
                    if res.err then cclog("err!!!") return end
                    local Formation = require "data.Formation"
                    Formation:set("convoy",onStageTypeId_tab)
                    --Formation:set("fightpower",self.m_fightPowerLabel:getString())
                    cc.Director:getInstance():popScene()
                end) 
       end
        local tmpTable = {}
        table.merge(tmpTable,challenge_tab)
        local jsonStr = json.encode(tmpTable)
        cc.UserDefault:getInstance():setStringForKey(beforeSceneType,jsonStr)
           
	end
end

function FormationUILayer:transFormData(res)
    local BattleScene = require("scene.battle2.BattleScene")
    cc.Director:getInstance():replaceScene(BattleScene:create(res.data))         
end

--init your ui here
function FormationUILayer:initUI(param)
    Helper.addSpriteFrames("FormationUI0.plist","FormationUI0.png")
    
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
            --self:updateSoulList()
            
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
	for i=1,table.getn(self.allsoulList) do
	    local soulItem = soulConfig[tostring(self.allsoulList[i]["soulId"])]
        self.allsoulList[i].site = soulItem["site"]
        self.allsoulList[i].onstage = 0
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

function FormationUILayer:loadPlayerFormation()
    print("[FormationUILayer]:  loadFormation")
    local savedTable = {}
    local jsonstr = ""
    jsonstr = cc.UserDefault:getInstance():getStringForKey(beforeSceneType)
    if  jsonstr == "" then self:updateSoulList() return end
    local tableTmp = json.decode(jsonstr)
    print("get str:"..jsonstr)
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

            table.insert(onStageTypeId_tab,soulId)

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
            self.m_xuanzeImage:addChild(copyitem)  
            copyitem:setTag(10000+indexj)
            

            local function backToHeroList(senderin,envenin)
                if senderin == copyitem and envenin == cc.EventCode.ENDED then
                    local indexDelete = copyitem:getTag() - 10000
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

                    local action = cc.MoveBy:create(0.2,cc.p(0,100))
                    local fadeout = cc.FadeOut:create(0.2)
                    local spwan = cc.Spawn:create(action,fadeout)
                    local sequence = cc.Sequence:create(spwan,cc.CallFunc:create(destoryFuc),cc.CallFunc:create(Movefunc))
                    copyitem:runAction(sequence)          
                end
            end
            copyitem:addTouchEventListener(backToHeroList)
            if count == #onStageId_tab then
                self:updateSoulList() return
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
        cc.UserDefault:getInstance():setStringForKey(beforeSceneType,jsonStr)
        self:updateSoulList()
    end

end

function FormationUILayer:updateSoulList()
    if  self.m_herolistScrollView:getChildrenCount() ~=0 then
        self.m_herolistScrollView:removeAllChildren()
    end
   
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
    if roleSize>10 then
        ScrovH = (130)*(math.floor((roleSize-1)/6)+1)
    end

    ScrovSize.height = ScrovH
    self.m_herolistScrollView:setInnerContainerSize(ScrovSize)
    local beCopyedItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("FormationRoleItem.csb")
    --print("copy csb")
    local fightPower = 0
    local localIdIndex = 1
    table.sort(curSoulList,function(a, b) return a.id < b.id end)
    local onStageId_tab_copy = {}
    table.merge(onStageId_tab_copy,onStageId_tab)
    table.sort(onStageId_tab_copy)
    for i=1,roleSize do
        local dataItem = curSoulList[i]
        local roleitem =beCopyedItem:clone()
        roleitem:setPosition(20+(i-1)%6*130,ScrovSize.height-130*(math.floor((i-1)/6)+1))
        self.m_herolistScrollView:addChild(roleitem)
        roleitem:setTag(dataItem["id"])
        
        local mLevelLabel = ccui.Helper:seekWidgetByName(roleitem, "m_LevelLabel")
        local mpinzhiImage = ccui.Helper:seekWidgetByName(roleitem, "m_pinzhiImage")
        local mtouxiangImage = ccui.Helper:seekWidgetByName(roleitem, "m_touxiangImage")
        local mgrayImage = ccui.Helper:seekWidgetByName(roleitem, "m_grayImage")
        
        local soulId = dataItem["soulId"]
        local soulItem = soulConfig[tostring(soulId)]
        mLevelLabel:setString(dataItem["level"])

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
        if #onStageId_tab ~= 0 and localIdIndex <= #onStageId_tab  then
            if  onStageId_tab_copy[localIdIndex] == dataItem["id"] then
                mgrayImage:setVisible(true)
                --print(dataItem["id"])
                localIdIndex = localIdIndex + 1
                fightPower = fightPower + dataItem["fc"]
            end
        end
        self.m_fightPowerLabel:setString(fightPower)
        
        --上阵方法
        local function moveToFormationList(sender,event)
            if sender == roleitem and event == cc.EventCode.ENDED then
                --print("soulId:"..soulId)
                local beforeinsert_tab = {}
                local beforeinserttype_tab = {}
                table.merge(beforeinsert_tab,onStageId_tab)
                table.merge(beforeinserttype_tab,onStageTypeId_tab)
                if table.getn(beforeinsert_tab) >=5 then print("over") return end
                
                table.insert(onStageTypeId_tab,soulId)
                

                local copyitem = roleitem:clone()

                local postransform = self.m_xuanzeImage:convertToNodeSpace(roleitem:getWorldPosition())
                --local posBegin = cc.p(postransform.x+roleitem:getCustomSize().width/2,postransform.y+roleitem:getCustomSize().height/2)
                local posBegin = cc.p(postransform.x,postransform.y)        
                copyitem:setPosition(posBegin)

                self.m_xuanzeImage:addChild(copyitem)          
                
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
                self.m_fightPowerLabel:setString(fightPower)
                
                local movetoPos = self.posTable[tonumber(indexInsert)]
                local action = cc.MoveTo:create(0.2,movetoPos)
                
                --local copysequence = cc.Sequence:create(cc.CallFunc:create(loadStart),action,cc.CallFunc:create(loadStop))
                
                copyitem:runAction(action)
                copyitem:setAnchorPoint(cc.p(0.5,0.5))
            
                mgrayImage:setVisible(true)
                roleitem:setTouchEnabled(false)
                curSoulList[i]["onstage"] = 1
                
                --下阵方法
                local function backToHeroList(senderin,envenin)
                    if senderin == copyitem and envenin == cc.EventCode.ENDED then

                        copyitem:setVisible(true)

                        local indexDelete = copyitem:getTag()-10000
                        
                        
                        
                        local function destoryFuc()
                            copyitem:removeFromParent(true)
                            table.remove(onStageId_tab,indexDelete)
                            local soulIdRemovePos = table.indexof(onStageTypeId_tab,soulId)
                            table.remove(onStageTypeId_tab,soulIdRemovePos)
                            --table.remove(onStageTypeId_tab,indexDelete)
                            
                            fightPower = fightPower - dataItem["fc"]
                            self.m_fightPowerLabel:setString(fightPower)
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
                        
                        local action = cc.MoveTo:create(0.2,posBegin)
                        local fadeout = cc.FadeOut:create(0.2)
                        local spwan = cc.Spawn:create(action,fadeout)
                        local sequence = cc.Sequence:create(spwan,cc.CallFunc:create(destoryFuc),cc.CallFunc:create(Movefunc))
                        copyitem:runAction(sequence)
                    end
                end
                copyitem:addTouchEventListener(backToHeroList)
            end   
        end

        roleitem:addTouchEventListener(moveToFormationList)
    end
end

function FormationUILayer:getBattleFieldId()
	return fieldId
end

function FormationUILayer:setBeforeSceneType(param)
    beforeSceneType = param
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
    end
    return bRet
end

return FormationUILayer
