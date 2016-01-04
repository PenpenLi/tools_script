--GuardUi.lua, created by php script. chris.li--
require "Cocos2d"
local PopDialog = require "common/PopDialog"

local GuardUi = class("GuardUi", function()
	return cc.Layer:create()
end)

function GuardUi:create()
	local layer = GuardUi.new()
	layer:initM()
	return layer
end


function GuardUi:ctor()
end

function GuardUi:onEnter()
    self.moveTag = 0
end

function GuardUi:onExit()
end

function GuardUi:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("GuardUi.csb")
	self:addChild(self.rootNode)
	

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_GuardPanel: Layout
	self.m_GuardPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_GuardPanel")
	self.m_GuardPanel:addTouchEventListener(touchEventHandler)
    self.m_GuardPanel:setBackGroundImage("ditu.jpg")

	--m_coverPanel: Layout
	self.m_coverPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_coverPanel")
	self.m_coverPanel:addTouchEventListener(touchEventHandler)

	--m_ShowEnemy: ImageView
	self.m_ShowEnemy = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShowEnemy")

	--m_ShowHusong: ImageView
	self.m_ShowHusong = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShowHusong")

	--m_GuardUiPanel: ImageView
	self.m_GuardUiPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_GuardUiPanel")

	--m_touxiangkuang: ImageView
	self.m_touxiangkuang = ccui.Helper:seekWidgetByName(self.rootNode, "m_touxiangkuang")

	--m_closeShowEnemyBtn: Button
	self.m_closeShowEnemyBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeShowEnemyBtn")
	self.m_closeShowEnemyBtn:addTouchEventListener(touchEventHandler)

	--m_sureBattleBtn: Button
	self.m_sureBattleBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_sureBattleBtn")
	self.m_sureBattleBtn:addTouchEventListener(touchEventHandler)

	--m_zhandouli: Text
	self.m_zhandouli = ccui.Helper:seekWidgetByName(self.rootNode, "m_zhandouli")

	--m_CoinNum: Text
	self.m_CoinNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_CoinNum")

	--m_closeShowHusong: Button
	self.m_closeShowHusong = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeShowHusong")
	self.m_closeShowHusong:addTouchEventListener(touchEventHandler)

	--m_RecordListView: ListView
	self.m_RecordListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_RecordListView")
	self.m_RecordListView:addTouchEventListener(touchEventHandler)

	--m_closeGuardBtn: Button
	self.m_closeGuardBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeGuardBtn")
	self.m_closeGuardBtn:addTouchEventListener(touchEventHandler)

	--m_RecordBtn: Button
	self.m_RecordBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_RecordBtn")
	self.m_RecordBtn:addTouchEventListener(touchEventHandler)

	--m_HuocheList: ImageView
	self.m_HuocheList = ccui.Helper:seekWidgetByName(self.rootNode, "m_HuocheList")

	--m_mainPic: ImageView
	self.m_mainPic = ccui.Helper:seekWidgetByName(self.rootNode, "m_mainPic")

	--m_ChooseHuoche: ImageView
	self.m_ChooseHuoche = ccui.Helper:seekWidgetByName(self.rootNode, "m_ChooseHuoche")
	
	--m_RobPic: ImageView
	self.m_RobPic = ccui.Helper:seekWidgetByName(self.rootNode, "m_RobPic")

	--m_LV: Text
	self.m_LV = ccui.Helper:seekWidgetByName(self.rootNode, "m_LV")

	--m_VIP: ImageView
	self.m_VIP = ccui.Helper:seekWidgetByName(self.rootNode, "m_VIP")

	--m_FuwuqiLable: Text
	self.m_FuwuqiLable = ccui.Helper:seekWidgetByName(self.rootNode, "m_FuwuqiLable")

	--m_huoche1: ImageView
	self.m_huoche1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_huoche1")
	self.faguang1 = ccui.Helper:seekWidgetByName(self.rootNode, "faguang1")

	--m_huoche2: ImageView
	self.m_huoche2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_huoche2")
    self.faguang2 = ccui.Helper:seekWidgetByName(self.rootNode, "faguang2")

	--m_huoche3: ImageView
	self.m_huoche3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_huoche3")
    self.faguang3 = ccui.Helper:seekWidgetByName(self.rootNode, "faguang3")

	--m_huoche4: ImageView
	self.m_huoche4 = ccui.Helper:seekWidgetByName(self.rootNode, "m_huoche4")
    self.faguang4 = ccui.Helper:seekWidgetByName(self.rootNode, "faguang4")
    
    --dun(huoche5): ImageView
    self.m_huoche5 = ccui.Helper:seekWidgetByName(self.rootNode, "m_huoche5")
    self.faguang5 = ccui.Helper:seekWidgetByName(self.rootNode, "faguang5")

	--m_husongLable: Text
	self.m_husongLable = ccui.Helper:seekWidgetByName(self.rootNode, "m_husongLable")

	--m_husongBtn: button
	self.m_husongBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_husongBtn")
    self.m_husongBtn:addTouchEventListener(touchEventHandler)

	--m_robBtn: button
	self.m_robBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_robBtn")
    self.m_robBtn:addTouchEventListener(touchEventHandler)

	--m_SendNum: Text
	self.m_SendNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_SendNum")

	--m_RobNum: Text
	self.m_RobNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_RobNum")

	--m_HusongDesc: ImageView
	self.m_HusongDesc = ccui.Helper:seekWidgetByName(self.rootNode, "m_HusongDesc")

	--m_HuochePic: ImageView
	self.m_HuochePic = ccui.Helper:seekWidgetByName(self.rootNode, "m_HuochePic")
	
    --m_PageView: ImageView
    self.m_PageView = ccui.Helper:seekWidgetByName(self.rootNode, "m_PageView")
    self.m_PageView:addTouchEventListener(touchEventHandler)

	--m_ItemDesc: ImageView
	self.m_ItemDesc = ccui.Helper:seekWidgetByName(self.rootNode, "m_ItemDesc")

	--m_gainDiomand: ImageView
	self.m_gainDiomand = ccui.Helper:seekWidgetByName(self.rootNode, "m_gainDiomand")

	--m_gainGongxianbi: ImageView
	self.m_gainGongxianbi = ccui.Helper:seekWidgetByName(self.rootNode, "m_gainGongxianbi")

	--m_rightBtn: Button
	self.m_rightBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_rightBtn")
	self.m_rightBtn:addTouchEventListener(touchEventHandler)

	--m_leftBtn: Button
	self.m_leftBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_leftBtn")
	self.m_leftBtn:addTouchEventListener(touchEventHandler)

	--m_RestRobNum: Text
	self.m_RestRobNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_RestRobNum")

	--m_goldLable: Text
	self.m_goldLable = ccui.Helper:seekWidgetByName(self.rootNode, "m_goldLable")

	--m_continueSearchBtn: Button
	self.m_continueSearchBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_continueSearchBtn")
	self.m_continueSearchBtn:addTouchEventListener(touchEventHandler)

	--m_robLable: Text
	self.m_robLable = ccui.Helper:seekWidgetByName(self.rootNode, "m_robLable")

	--m_remainNum: Text
	self.m_remainNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_remainNum")

	--m_banggongCoin: Text
	self.m_banggongCoin = ccui.Helper:seekWidgetByName(self.rootNode, "m_banggongCoin")

	--m_confirmBtn: Button
	self.m_confirmBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_confirmBtn")
	self.m_confirmBtn:addTouchEventListener(touchEventHandler)
    
    --m_refreshBtn: Button
    self.m_refreshBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_refreshBtn")
    self.m_refreshBtn:addTouchEventListener(touchEventHandler)
	
    --self.m_diamond: Text
    self.m_diamond = ccui.Helper:seekWidgetByName(self.rootNode, "m_diamond")

	--m_gainDiomandNum: Text
	self.m_gainDiomandNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_gainDiomandNum")

	--m_gainGongxianbiNum: Text
	self.m_gainGongxianbiNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_gainGongxianbiNum")
	
    --m_RefreshPrice: Text
    self.m_RefreshPrice = ccui.Helper:seekWidgetByName(self.rootNode, "m_RefreshPrice")
    
	self:initUI()
end

function GuardUi:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_GuardPanel and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_GuardPanel touched--")

	elseif sender == self.m_coverPanel and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_coverPanel touched--")

	elseif sender == self.m_closeShowEnemyBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_closeShowEnemyBtn touched--")

	elseif sender == self.m_sureBattleBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_sureBattleBtn touched--")

	elseif sender == self.m_closeShowHusong and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_closeShowHusong touched--")

	elseif sender == self.m_RecordListView and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_RecordListView touched--")

	elseif sender == self.m_closeGuardBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_closeGuardBtn touched--")
        local scene = require("scene.movement.MovementScene") : create()
        cc.Director:getInstance():replaceScene(scene)

	elseif sender == self.m_RecordBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_RecordBtn touched--")

	elseif sender == self.m_rightBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_rightBtn touched--")
		
        self.moveTag = self.moveTag - 1
        
        if self.moveTag < 0 then
            self.moveTag = 0
        end
        
        --self:isShowDirec(self.moveTag)

	elseif sender == self.m_leftBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_leftBtn touched--")
		
        self.moveTag = self.moveTag + 1

        if self.moveTag < 0 then
            self.moveTag = 0
        end

       -- self:isShowDirec(self.moveTag)

	elseif sender == self.m_continueSearchBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_continueSearchBtn touched--")
		
		self:RobTruck()
    
    --刷新按钮
    elseif sender == self.m_refreshBtn and event == cc.EventCode.ENDED then
        cclog("[GuardUi]--m_refreshBtn touched--")
        --刷新前检测是否已达最高品质
        Http:req("RefreshConvoyTruckRequest",{type = 0},function(res)
            if res.data.truck ~= nil then
                if res.data.truck.convoyId == 5 then
                    PopDialog:getInstance():popText("货车已达最高品质，无需刷新！",cc.c3b(0,255,0))
                else
                    self:refreshTruck()    
                end
            else
                self:refreshTruck()    
            end
        end)
        
    
    --确认进入护送
	elseif sender == self.m_confirmBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_confirmBtn touched--")
		local confirmTag = 0
		
		Http:req("GetConvoySoulsRequest",nil,function(res)
         local IsSoulGray = res.data.soul
            self:Go2fomation(IsSoulGray)
            
            self.moveTag = 3 + confirmTag
	    end)
	   confirmTag = confirmTag + 1
	   
	   self:JudgeCar()
	
	--进入选货车界面	
    elseif sender == self.m_husongBtn and event == cc.EventCode.ENDED then
        cclog("select truck")
        --self:initUI()
        
        Http:req("RefreshConvoyTruckRequest",{type = 0},function(res)
            if res.data.result == 0 then 
                self.moveTag = 1
                self.m_ChooseHuoche:setVisible(true) 
                self.m_RefreshPrice:setString(res.data.refreshCost) 
                if res.data.truck == nil then
                   self:JudgeCar(1) 
                   self.m_diamond:setString(92)
                   self.m_banggongCoin:setString(46)
                else
                   self:JudgeCar(res.data.truck.convoyId)
                   self.m_diamond:setString(res.data.truck.gold)
                   self.m_banggongCoin:setString(res.data.truck.devote)   
                end
             elseif res.data.result == 3 then
                PopDialog:getInstance():popText("您的护送次数不足！",cc.c3b(0,255,0))
             elseif res.data.result == 2 then
                PopDialog:getInstance():popText("您的护送货车已达上限",cc.c3b(0,255,0))   
             end   

        end)
        
        self.m_mainPic:setVisible(false)
        self.m_RecordBtn:setVisible(false)
        self.faguang5:setVisible(false)
        
        self.m_rightBtn:setVisible(true)
        --self.m_leftBtn:setVisible(false)
        
    elseif sender == self.m_robBtn and event == cc.EventCode.ENDED then
        cclog("抢劫")
        self.moveTag = -1
        self.m_RobPic:setVisible(true)
        self.m_mainPic:setVisible(false)
        
        self.m_RecordBtn:setVisible(false)
        self.faguang5:setVisible(false)

        self.m_leftBtn:setVisible(true)
        self.m_rightBtn:setVisible(false)
        
        self.m_HuocheList:setVisible(false)
        Http:req("RefreshRobTruckRequest",{type = 0},function(res)
            self.m_goldLable:setString(res.data.refreshCost)
        end)

	end
end

--init your ui here
function GuardUi:initUI()

    self.m_mainPic:setVisible(true)
    self.moveTag = 0
    Http:req("GetConvoyMainRequest",nil,function(res)
       self.m_SendNum:setString(res.data.convoyNum.."/"..res.data.convoyMax)
       self.m_RobNum:setString(res.data.robNum.."/"..res.data.robMax)
       
       self.m_remainNum:setString(res.data.convoyNum.."/"..res.data.convoyMax)
       self.m_RestRobNum:setString(res.data.robNum.."/"..res.data.robMax)
       
       if table.getn(res.data.truck) == 0 then
           self.m_HuocheList:setVisible(false) 
       end
       
    end)
end

--刷新按钮点击后  界面
function GuardUi:refreshTruck()
    Http:req("RefreshConvoyTruckRequest",{type = 1},function(res)
        if res.data.result == 0 then
            
            self.m_diamond:setString(res.data.truck.gold)
            self.m_banggongCoin:setString(res.data.truck.devote)
            self.m_RefreshPrice:setString(res.data.refreshCost)
            self:JudgeCar(res.data.truck.convoyId)
            
            PopDialog:getInstance():popText("刷新成功",cc.c3b(0,255,0))
                
        elseif res.data.result == 3 then
            PopDialog:getInstance():popText("您的金币已不足！",cc.c3b(0,255,0))
        end    
    end) 
end


--判断货车品质
function GuardUi:JudgeCar(truckId)
    for i = 1,5 do
        local starItem = ccui.Helper:seekWidgetByName(self.rootNode, "m_starD"..i)
        
        starItem:setVisible(true)
    end
    
    if truckId ~= 5 then
        for j = 1,truckId do
            local starItemD = ccui.Helper:seekWidgetByName(self.rootNode, "m_starD"..j)
            starItemD:setVisible(false)
        end

        for j_1 = truckId+1,5 do
            local starItemD_1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_starD"..j_1)
            starItemD_1:setVisible(true)
        end
    elseif truckId == 5 then
        for j_2 = 1,5 do
            local starItemD_2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_starD"..j_2)
            starItemD_2:setVisible(false)
        end    
    end
    
end

function GuardUi:RobTruck()

    Http:req("RefreshRobTruckRequest",{type = 1},function(res)
        self.m_goldLable:setString(res.data.refreshCost)
    end)

end

--左右箭头
function GuardUi:isShowDirec(Scenetag)
    if Scenetag == 0 then
        self.m_rightBtn:setVisible(false)
        self.m_mainPic:setVisible(true)
        self:initUI()
        self.m_ChooseHuoche:setVisible(false)
        self.m_RecordBtn:setVisible(true)
        self.m_RobPic:setVisible(false)
    elseif Scenetag == 1 then
        self.m_mainPic:setVisible(false)
        self.m_ChooseHuoche:setVisible(true)
        self.m_RobPic:setVisible(false)
    elseif Scenetag == 2 then
        self.m_mainPic:setVisible(false)
        self.m_ChooseHuoche:setVisible(false) 
        self.m_RobPic:setVisible(true)
    elseif Scenetag == 3 then
        self.m_mainPic:setVisible(false)
        self.m_ChooseHuoche:setVisible(false) 
        self.m_RobPic:setVisible(false)         
    end
end

function GuardUi:Go2fomation(Indexsoul)
    
    local soulItem = {}
    soulItem = Indexsoul
    local FormationUILayer =  require "scene.formation.FormationUILayer"

    local data = {soulItem,matrix ={}}
    local Formation = require "data.Formation"
    Formation:set("battledata",data)
    
    local scene = require("scene.formation.FormationScene"):create({BattleType="convoy",soul=Indexsoul})
    cc.Director:getInstance():pushScene(scene)
end

function GuardUi:showConcoy()
    Http:req("GetConvoyMainRequest",nil,function(res)
        cclog("enter showConvoy")
        local truckNum = table.getn(res.data.truck)
        
        if truckNum == 0 then
            self.m_PageView:setVisible(false)
            self.m_rightBtn:setVisible(false)
            self.m_leftBtn:setVisible(false)
        else
            self.m_PageView:setVisible(true)
            self.m_rightBtn:setVisible(true)
            self.m_leftBtn:setVisible(false)    
        end
        
        if self.m_PageView:getChildrenCount() ~= 0 then 
            self.m_PageView:removeAllChildren()
            self.m_PageView:setBounceEnabled(true)
        end
        for i=1,truckNum do
            local convoyItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("convoy.csb")
            convoyItem:setTag(i)
            local m_timeleft = ccui.Helper:seekWidgetByName(convoyItem, "m_timeleft")
            local dimondNum = ccui.Helper:seekWidgetByName(convoyItem, "dimondNum")
            local coinsNum = ccui.Helper:seekWidgetByName(convoyItem, "coinsNum")
            
            local truckId_2 = res.data.truck[i].convoyId
            for i = 1,5 do
                local starItem = ccui.Helper:seekWidgetByName(convoyItem, "star"..i)

                starItem:setVisible(true)
            end

            if truckId_2 ~= 5 then
                for j = 1,truckId_2 do
                    local starItemD = ccui.Helper:seekWidgetByName(convoyItem, "star"..j)
                    starItemD:setVisible(false)
                end

                for j_1 = truckId_2+1,5 do
                    local starItemD_1 = ccui.Helper:seekWidgetByName(convoyItem, "star"..j_1)
                    starItemD_1:setVisible(true)
                end
            elseif truckId_2 == 5 then
                for j_2 = 1,5 do
                    local starItemD_2 = ccui.Helper:seekWidgetByName(convoyItem, "star"..j_2)
                    starItemD_2:setVisible(false)
                end    
            end
            
            m_timeleft:setString(res.data.truck[i].leftTm)
            dimondNum:setString(res.data.truck[i].gold)
            coinsNum:setString(res.data.truck[i].devote)
            
            self.m_PageView:addChild(convoyItem)
        end 
    end)
end

function GuardUi:refreshMyTruck()
    self.m_ChooseHuoche:setVisible(false)
    self:showConvoy()
end

--refresh all ui here
function GuardUi:refreshUI()
end

return GuardUi
