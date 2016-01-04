--GuardUi.lua, created by php script. chris.li--
require "Cocos2d"
local PopDialog = require "common/PopDialog"
local Roler = require("data.Roler")
local soulConfig = require("config.define_soul")

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
    
   -- if self.entertag == 1 then
    
        self.m_PageView:removeAllPages()
        self:showConvoy()
        self.faguang_1:setVisible(true)
        self.convoyN = 5
        
        self.entertag = 2
    --else
       -- cclog("enter")  
    --end
    
end

function GuardUi:onExit()
end

function GuardUi:pageviewCallBack(sender,event)

    if event == ccui.PageViewEventType.turning then
        local  index = sender:getCurPageIndex()
        cclog("here")
        print(index)
        
        self:initSmallTruck()
        
        local indexN = index +1
            
            local curFaguang = ccui.Helper:seekWidgetByName(self.rootNode, "faguang"..indexN)
            curFaguang:setVisible(true)
            
            if self.truckNum_3 == 1 then
                self.m_leftBtn:setVisible(false)
                self.m_rightBtn:setVisible(false)
            else
                --判断左右箭头是否显示
                if indexN == 1 then
                    self.m_leftBtn:setVisible(false)
                    self.m_rightBtn:setVisible(true)
                elseif indexN == self.truckNum_3 then
                    self.m_leftBtn:setVisible(true)
                    self.m_rightBtn:setVisible(false)
                else
                    self.m_leftBtn:setVisible(true)
                    self.m_rightBtn:setVisible(true)
                end    
            end
            
    end    
end

function GuardUi:initM()

    self.entertag = 1

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
	
    local function PagetouchEventHandler(sender, event)
        self:pageviewCallBack(sender, event)
    end

	--m_GuardPanel: Layout
	self.m_GuardPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_GuardPanel")
	self.m_GuardPanel:addTouchEventListener(touchEventHandler)
    self.m_GuardPanel:setBackGroundImage("ditu.jpg")

	--m_coverPanel: Layout
	self.m_coverPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_coverPanel")
	self.m_coverPanel:addTouchEventListener(touchEventHandler)
	
    self.faguang_1 = ccui.Helper:seekWidgetByName(self.rootNode, "faguang1")
    self.faguang_2 = ccui.Helper:seekWidgetByName(self.rootNode, "faguang2")
    self.faguang_3 = ccui.Helper:seekWidgetByName(self.rootNode, "faguang3")
    self.faguang_4 = ccui.Helper:seekWidgetByName(self.rootNode, "faguang4")

	--m_ShowEnemy: ImageView
	self.m_ShowEnemy = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShowEnemy")

	--m_ShowHusong: ImageView
	self.m_ShowHusong = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShowHusong")

	--m_GuardMainDitu: ImageView
    self.m_GuardMainDitu = ccui.Helper:seekWidgetByName(self.rootNode, "m_GuardMainDitu")

	--m_touxiangkuang: ImageView
	self.m_touxiangkuang = ccui.Helper:seekWidgetByName(self.rootNode, "m_touxiangkuang")

	--m_closeShowEnemyBtn: Button
	self.m_closeShowEnemyBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeShowEnemyBtn")
	self.m_closeShowEnemyBtn:addTouchEventListener(touchEventHandler)

	--m_sureBattleBtn: Button
	self.m_sureBattleBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_sureBattleBtn")
	--self.m_sureBattleBtn:addTouchEventListener(touchEventHandler)
	
    --fanhuiBtn: Button  选择货车
    self.fanhuiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "fanhuiBtn")
    self.fanhuiBtn:addTouchEventListener(touchEventHandler)
    
    --fanhui2Btn: Button  抢劫货车
    self.fanhui2Btn = ccui.Helper:seekWidgetByName(self.rootNode, "fanhui2Btn")
    self.fanhui2Btn:addTouchEventListener(touchEventHandler)

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
	
    --robrecordbtn:Button
    self.robrecordbtn = ccui.Helper:seekWidgetByName(self.rootNode, "robrecordbtn")
    self.robrecordbtn:addTouchEventListener(touchEventHandler)

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
    
    --dun(huoche5): ImageView
    self.m_huoche5 = ccui.Helper:seekWidgetByName(self.rootNode, "m_huoche5")
    self.faguang5 = ccui.Helper:seekWidgetByName(self.rootNode, "faguang5")

	--m_husongLable: Text
	self.m_husongLable = ccui.Helper:seekWidgetByName(self.rootNode, "m_husongLable")

	--m_husongBtn: button
	self.m_husongBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_husongBtn")
    --self.m_husongBtn:addTouchEventListener(touchEventHandler)

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
    self.m_PageView:addEventListener(PagetouchEventHandler)

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
    
    --m_convoyDitu: image
    self.m_convoyDitu = ccui.Helper:seekWidgetByName(self.rootNode, "m_convoyDitu")
    
    --convoy return: Button
    self.m_convoyReturnBtn = ccui.Helper:seekWidgetByName(self.rootNode, "Button_27")
    self.m_convoyReturnBtn:addTouchEventListener(touchEventHandler)
    
    self.m_PageView:removeAllPages()
    
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
		self.m_coverPanel:setVisible(false)
		self.m_ShowEnemy:setVisible(false)
		self.m_ShowHusong:setVisible(false)
		
		for i = 1,5 do
            local soulDetail_1 = ccui.Helper:seekWidgetByName(self.rootNode, "heroPic"..i)
            soulDetail_1:setVisible(false)
        end

	elseif sender == self.m_closeShowHusong and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_closeShowHusong touched--")
		self.m_ShowHusong:setVisible(false)
		self.m_coverPanel:setVisible(false)
		self.m_ShowEnemy:setVisible(false)

	elseif sender == self.m_RecordListView and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_RecordListView touched--")

	elseif sender == self.m_closeGuardBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_closeGuardBtn touched--")
        local scene = require("scene.movement.MovementScene") : create()
        cc.Director:getInstance():replaceScene(scene)

	elseif sender == self.m_RecordBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_RecordBtn touched--")
		self.m_coverPanel:setVisible(true)
		self.m_ShowHusong:setVisible(true)
		self.m_ShowEnemy:setVisible(false)
		self:showRecord()
		
    elseif sender == self.robrecordbtn and event == cc.EventCode.ENDED then
        cclog("[GuardUi]--robrecordbtn touched--")
        self.m_coverPanel:setVisible(true)
        self.m_ShowHusong:setVisible(true)
        self.m_ShowEnemy:setVisible(false)
        self:showRecord()	

	elseif sender == self.m_rightBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_rightBtn touched--")
        local curIndex = self.m_PageView:getCurPageIndex()
        if  curIndex >= 0 and curIndex < self.truckNum_3 then
            self.m_PageView:scrollToPage(curIndex+1)
        elseif curIndex == self.truckNum_3 then
            self.m_rightBtn:setVisible(false)
        end

	elseif sender == self.m_leftBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_leftBtn touched--")
        local curIndex = self.m_PageView:getCurPageIndex()
        if  curIndex > 0 then
            self.m_PageView:scrollToPage(curIndex-1)
        elseif curIndex == 0 then
            self.m_leftBtn:setVisible(false)
        end
		

	elseif sender == self.m_continueSearchBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_continueSearchBtn touched--")
		
        Http:req("RefreshRobTruckRequest",{type = 1},function(res)
            self.m_goldLable:setString(res.data.refreshCost)
            
            self:myrefresh()
            
            if res.data.result == 0 then
            
                if table.getn(res.data.truck) ~= 0 then
                    PopDialog:getInstance():popText("刷新成功！",cc.c3b(0,255,0))
                    local enemyNum = table.getn(res.data.truck)
                    
                    if enemyNum > 4 then
                        enemyNum = 4
                    else
                        enemyNum = enemyNum    
                    end
                    
                    --抢夺第一界面
                    for i = 1,enemyNum do
                        local roblist = ccui.Helper:seekWidgetByName(self.rootNode, "RobList"..i)
                        roblist:setVisible(true)
                        local UnionName = ccui.Helper:seekWidgetByName(roblist, "UnionName")
                        local PlayerName = ccui.Helper:seekWidgetByName(roblist, "PlayerName")
                        local LV = ccui.Helper:seekWidgetByName(roblist, "LV")
                       
                        if res.data.truck[i].owner.guildName ~= nil then 
                            UnionName:setString(res.data.truck[i].owner.guildName)
                        else
                            UnionName:setString("暂未加入公会")      
                        end
                        
                        PlayerName:setString(res.data.truck[i].owner.name)
                        LV:setString(res.data.truck[i].owner.level)
                        
                        local function rob2CallBack(sender,event)--按钮点击进入第二届面
                            if sender == roblist and event == cc.EventCode.ENDED then
                            cclog("here.....")
                                if self.robRestNum ~= 0 then
                                    local soulNum = table.getn(res.data.truck[i].owner.soul)
                                    self:robAlready(res.data.truck[i].owner.soul,res.data.truck[i].owner)
                                    self:convoyRob(res.data.truck[i].id,res.data.truck[i].owner.rolerId)
                                else
                                    PopDialog:getInstance():popText("抢劫失败！今日抢劫次数已用完！",cc.c3b(0,255,0))
                                end
                            end
                        end      

                        roblist:addTouchEventListener(rob2CallBack)
                        
                    end
                else
                    PopDialog:getInstance():popText("当前无护送队伍，稍后再来试试吧",cc.c3b(0,255,0))
                    for i = 1,4 do
                        local roblist = ccui.Helper:seekWidgetByName(self.rootNode, "RobList"..i)
                        roblist:setVisible(false)
                    end        
                end
            elseif res.data.result == 1 then
                PopDialog:getInstance():popText("搜索失败，您的金币不足！",cc.c3b(0,255,0))
                for i = 1,4 do
                    local roblist = ccui.Helper:seekWidgetByName(self.rootNode, "RobList"..i)
                    roblist:setVisible(false)
                end       
            end
            
        end)
		
		
    elseif sender == self.fanhuiBtn and event == cc.EventCode.ENDED then
        cclog("[GuardUi]--fanhuiBtn touched--")
        self.m_ChooseHuoche:setVisible(false)
        self.m_mainPic:setVisible(true)
        self.m_rightBtn:setVisible(false)
        self.m_leftBtn:setVisible(false)
        self.faguang5:setVisible(true)
        self:initSmallTruck()
        self:initUI()
        
    elseif sender == self.fanhui2Btn and event == cc.EventCode.ENDED then
        cclog("[GuardUi]--fanhui2Btn touched--")
        self.robTag = 0
        self.m_RobPic:setVisible(false)
        self.m_mainPic:setVisible(true)
        self.m_rightBtn:setVisible(false)
        self.m_leftBtn:setVisible(false)
        self.faguang5:setVisible(true)
        self:initSmallTruck()
        self:initUI()
    
    --刷新按钮
    elseif sender == self.m_refreshBtn and event == cc.EventCode.ENDED then
        cclog("[GuardUi]--m_refreshBtn touched--")
        --刷新前检测是否已达最高品质
        Http:req("RefreshConvoyTruckRequest",{type = 0},function(res)
            if res.data.truck.convoyId == 5 then
                PopDialog:getInstance():popText("货车已达最高品质，无需刷新！",cc.c3b(0,255,0))
            else
                self:refreshTruck()    
            end
        end)
        
    
    --确认进入护送
	elseif sender == self.m_confirmBtn and event == cc.EventCode.ENDED then
		cclog("[GuardUi]--m_confirmBtn touched--")
		
		if self.convoyN == 0 then
		    PopDialog:getInstance():popText("您的护送次数已满！",cc.c3b(0,255,0))
		else
    		Http:req("GetConvoyMainRequest",nil,function(res)
                if res.data.convoyNum ~= 0 then
                    Http:req("GetConvoySoulsRequest",nil,function(res)
                        local IsSoulGray = res.data.soul
                        self:Go2fomation(IsSoulGray)
                    end)
                else 
                    PopDialog:getInstance():popText("您的护送次数已满！",cc.c3b(0,255,0))      
                end
            end)
		end
        
    elseif sender == self.m_robBtn and event == cc.EventCode.ENDED then
        cclog("抢劫")
        
        self.robTag = 1
        
        self.m_RobPic:setVisible(true)
        self.m_mainPic:setVisible(false)
        
        self.m_leftBtn:setVisible(false)
        self.m_rightBtn:setVisible(false)
        
        self.m_RecordBtn:setVisible(false)
        self.faguang5:setVisible(false)
        
        self.m_HuocheList:setVisible(false)
        
        --抢劫默认状态
        Http:req("RefreshRobTruckRequest",{type = 0},function(res)
            self.m_goldLable:setString(res.data.refreshCost)
            
            self:myrefresh()
            
            if table.getn(res.data.truck) ~= 0 then
                local enemyNum = table.getn(res.data.truck)
                
                if enemyNum > 4 then
                    enemyNum = 4
                else
                    enemyNum = enemyNum    
                end
                
                for i = 1,enemyNum do
                    local roblist = ccui.Helper:seekWidgetByName(self.rootNode, "RobList"..i)
                    roblist:setVisible(true)
                    local UnionName = ccui.Helper:seekWidgetByName(roblist, "UnionName")
                    local PlayerName = ccui.Helper:seekWidgetByName(roblist, "PlayerName")
                    local LV = ccui.Helper:seekWidgetByName(roblist, "LV")
                   
                   
                   if res.data.truck[i].owner.guildName ~= nil then 
                       UnionName:setString(res.data.truck[i].owner.guildName)
                   else
                       UnionName:setString("暂未加入公会")      
                   end
                   PlayerName:setString(res.data.truck[i].owner.name)
                   LV:setString(res.data.truck[i].owner.level)
                   
                    local function rob2CallBack(sender,event)--抢劫
                        if sender == roblist and event == cc.EventCode.ENDED then
                            local soulNum = table.getn(res.data.truck[i].owner.soul)
                            self:convoyRob(res.data.truck[i].id,res.data.truck[i].owner.rolerId)
                            self:robAlready(res.data.truck[i].owner.soul,res.data.truck[i].owner)
                        end
                    end      

                    roblist:addTouchEventListener(rob2CallBack)
                   
                end
            else
                for i = 1,4 do
                   local roblist = ccui.Helper:seekWidgetByName(self.rootNode, "RobList"..i)
                   roblist:setVisible(false)
                end      
            end
            
        end)
        
    elseif sender == self.m_convoyReturnBtn and event == cc.EventCode.ENDED then
        self.m_convoyDitu:setVisible(false)
        self.m_GuardMainDitu:setVisible(true)
        
        self.m_ChooseHuoche:setVisible(false)
        self.m_mainPic:setVisible(true)
        self.faguang5:setVisible(true)
        self.m_rightBtn:setVisible(false)
        self.m_leftBtn:setVisible(false)
        self.m_RecordBtn:setVisible(true)
        
        self.m_PageView:removeAllPages()
        
        self:initSmallTruck()
        
        Http:req("GetConvoyMainRequest",nil,function(res)
            self.m_SendNum:setString(res.data.convoyNum.."/"..res.data.convoyMax)
            self.m_RobNum:setString(res.data.robNum.."/"..res.data.robMax)
        end)
	end
end

--init your ui here

function GuardUi:initUI()
    
    self.m_RecordBtn:setVisible(true)
    
    self.m_leftBtn:setVisible(false)
    self.m_rightBtn:setVisible(false)
    
    if self.truckNum_3 == 0 then
        self.m_HuocheList:setVisible(false)
    else
        self.m_HuocheList:setVisible(true)
        self.m_huoche5:setVisible(true)
        self.faguang5:setVisible(false)
    end    
    
end

--进入货车选择
function GuardUi:isEnterChooseTruck()
    Http:req("RefreshConvoyTruckRequest",{type = 0},function(res)
        if res.data.result == 0 then
            self.m_ChooseHuoche:setVisible(true) 
            self.m_RefreshPrice:setString(res.data.refreshCost) 

            --self:JudgeCar(res.data.truck.convoyId)
            self:playeffect(self.m_HuochePic,res.data.truck.convoyId)
            --self:playeffect(self.m_HuochePic,1)
            self.m_diamond:setString(res.data.truck.gold)
            self.m_banggongCoin:setString(res.data.truck.devote) 

        elseif res.data.result == 3 then
            PopDialog:getInstance():popText("您的护送次数已满！",cc.c3b(0,255,0))
        elseif res.data.result == 2 then
            PopDialog:getInstance():popText("您的护送货车已达上限",cc.c3b(0,255,0))   
        end 
    end)

    self.m_mainPic:setVisible(false)
    self.m_RecordBtn:setVisible(false)
    self.faguang5:setVisible(false)
end

--刷新按钮点击后  界面
function GuardUi:refreshTruck()
    Http:req("RefreshConvoyTruckRequest",{type = 1},function(res)
        if res.data.result == 0 then
            
            self.m_diamond:setString(res.data.truck.gold)
            self.m_banggongCoin:setString(res.data.truck.devote)
            self.m_RefreshPrice:setString(res.data.refreshCost)
           -- self:JudgeCar()
            self:playeffect(self.m_HuochePic,res.data.truck.convoyId)
            PopDialog:getInstance():popText("刷新成功",cc.c3b(0,255,0))
                
        elseif res.data.result == 1 then
            PopDialog:getInstance():popText("刷新失败，您的金币不足！",cc.c3b(0,255,0))
        end    
    end) 
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

function GuardUi:Go2Convoyfomation(id_1,defId_1)

    local soulItem = {}
    
    local FormationUILayer =  require "scene.formation.FormationUILayer"

    local data = {id = id_1,matrix = {},defId = defId_1 }
    local Formation = require "data.Formation"
    Formation:set("battledata",data)
    
    local scene = require("scene.formation.FormationScene"):create({BattleType="convoyRob"})
    cc.Director:getInstance():pushScene(scene)
end

function GuardUi:showConvoy()
    
    Http:req("GetConvoyMainRequest",nil,function(res)
        cclog("enter showConvoy")
        
        --pageview中用
        self.truckNum_3 = table.getn(res.data.truck)
        
        self.m_SendNum:setString(res.data.convoyNum.."/"..res.data.convoyMax)
        self.m_RobNum:setString(res.data.robNum.."/"..res.data.robMax)
        
        self.robRestNum = res.data.robNum

        self.m_remainNum:setString(res.data.convoyNum.."/"..res.data.convoyMax)
        self.m_RestRobNum:setString(res.data.robNum.."/"..res.data.robMax)

        if table.getn(res.data.truck) == 0 then
            self.m_HuocheList:setVisible(false)
        else
            self.m_HuocheList:setVisible(true)
            self.m_huoche5:setVisible(true)
            self.faguang5:setVisible(false)
            
            --生成货车小图标
            for i = 1,4 do
                local huocheItem = ccui.Helper:seekWidgetByName(self.rootNode, "m_huoche"..i)
                huocheItem:loadTexture("huiguard"..".png")
            end
            
            for j = 1,self.truckNum_3 do
                local huocheItem = ccui.Helper:seekWidgetByName(self.rootNode, "m_huoche"..j)
                huocheItem:loadTexture(res.data.truck[j].convoyId.."guard"..".png")
            end
        end
        
        local truckNum = table.getn(res.data.truck)
        
        if truckNum ~= 0 then
            self.m_HuocheList:setVisible(true)
            self.m_GuardMainDitu:setVisible(false)
            self.m_convoyDitu:setVisible(true)
            self.m_PageView:removeAllPages()
            --生成护送中界面
            for i=1,truckNum do
                
                local layout = ccui.Layout:create()
                layout:setContentSize(self.m_PageView:getContentSize())

                local convoyItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("convoy.csb")
                convoyItem:setTag(i)
                local m_convoyPic = ccui.Helper:seekWidgetByName(convoyItem, "m_convoyPic")
                local m_shiqu = ccui.Helper:seekWidgetByName(convoyItem, "m_shiqu")

                local m_timeleft = ccui.Helper:seekWidgetByName(convoyItem, "m_timeleft")
                local dimondNum = ccui.Helper:seekWidgetByName(convoyItem, "dimondNum")
                local coinsNum = ccui.Helper:seekWidgetByName(convoyItem, "coinsNum")

                local baoshiNum = ccui.Helper:seekWidgetByName(convoyItem, "baoshiNum")
                local jinbishu = ccui.Helper:seekWidgetByName(convoyItem, "jinbishu")
                local other1 = ccui.Helper:seekWidgetByName(convoyItem, "Image_94")

                local truckId_2 = res.data.truck[i].convoyId
                
                self:playeffect(m_convoyPic,truckId_2)

                m_timeleft:setString(res.data.truck[i].leftTm.."分钟")
                dimondNum:setString(res.data.truck[i].gold)
                coinsNum:setString(res.data.truck[i].devote)

                baoshiNum:setString(res.data.truck[i].gold)
                jinbishu:setString(res.data.truck[i].devote)

                convoyItem:setAnchorPoint(0,0)
                layout:addChild(convoyItem)
                self.m_PageView:addPage(layout)
                
                if res.data.truck[i].leftTm == 0 then
                    m_convoyPic:setVisible(false)
                    other1:setVisible(false)
                    m_shiqu:setVisible(true)
                end

                local function shiqu2CallBack(sender,event)--拾取
                    if sender == m_shiqu and event == cc.EventCode.ENDED then
                        self:pickUp(res.data.truck[i].id)
                        
                        self.m_PageView:removePage(layout)
                        self.m_PageView:removeAllPages()
                        self:showConvoy()
                    end
                end        

                m_shiqu:addTouchEventListener(shiqu2CallBack)

            end 
            
        else
            self.m_HuocheList:setVisible(false)
            self.m_GuardMainDitu:setVisible(true)
            self.m_convoyDitu:setVisible(false)
        end
        
        if self.robTag == 1 then
            self.m_mainPic:setVisible(false)
            self.m_convoyDitu:setVisible(false)
            self.m_GuardMainDitu:setVisible(true)
            self.m_RobPic:setVisible(true)
            self.m_HuocheList:setVisible(false)
            self.m_RecordBtn:setVisible(false)
        end
        
        --进入选货车界面
        local function husongBtn2CallBack(sender,event)
            if sender == self.m_husongBtn and event == cc.EventCode.ENDED then
                if res.data.convoyNum == 0 then
                    self.convoyN = 0
                    if table.getn(res.data.truck) ~= 0 then
                        self.m_PageView:removeAllPages()
                        self:showConvoy()
                    else
                        self:isEnterChooseTruck()
                    end    
                else
                    self:isEnterChooseTruck()
                end
            end    
        end
        self.m_husongBtn:addTouchEventListener(husongBtn2CallBack)
       
    end)
end

function GuardUi:pickUp(idOfTruck)

    local truckID = idOfTruck
    Http:req("GetConvoyRewardRequest",{id = truckID},function(res)
        if res.data.result == 0 then
            PopDialog:getInstance():popText("拾取奖励成功！",cc.c3b(0,255,0)) 
        end
    end)
end

function GuardUi:refreshMyTruck()
    self.m_PageView:removeAllPages()
    self:showConvoy()
end


function GuardUi:showRecord()
    cclog("enter record")
    Http:req("GetConvoyBattleReportListRequest",nil,function(res)
        print(res)
        
        if self.m_RecordListView:getChildrenCount() ~= 0 then 
            self.m_RecordListView:removeAllChildren()
            self.m_RecordListView:setBounceEnabled(true)
        end  
        
        local recordNum = table.getn(res.data.rpt)
        for i = 1,recordNum do
            local recordItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("RoberItem.csb")
            recordItem:setTag(i)
            local m_RoberName = ccui.Helper:seekWidgetByName(recordItem, "m_RoberName")
            local m_fuwuqi = ccui.Helper:seekWidgetByName(recordItem, "m_fuwuqi")
            local m_Time = ccui.Helper:seekWidgetByName(recordItem, "m_Time")
            local m_Win = ccui.Helper:seekWidgetByName(recordItem, "m_Win")
            local m_Failed = ccui.Helper:seekWidgetByName(recordItem, "m_Failed")
            
            local m_baoxiang = ccui.Helper:seekWidgetByName(recordItem, "m_baoxiang")
            local m_bofang = ccui.Helper:seekWidgetByName(recordItem, "m_bofang")
            
            m_RoberName:setString(res.data.rpt[i].otherName)
            m_Time:setString(math.floor(res.data.rpt[i].rptTm/3600))
            
            if res.data.rpt[i].win == 0 then
                m_Win:setVisible(true)
                m_Failed:setVisible(false)
            elseif res.data.rpt[i].win == 1 then
                m_Win:setVisible(false)
                m_Failed:setVisible(true)
            end
            
            local function showRecord2CallBack(sender,event)--显示记录
                if sender == m_baoxiang and event == cc.EventCode.ENDED then
                    cclog("baoxiang")
                elseif sender == m_bofang and event == cc.EventCode.ENDED then 
                    cclog("play") 
                    Http:req("GetBattleReport",{rptId = res.data.rpt[i].id,type = 1 },function(func)
                        if func.data.result == 0 then
                            local BattleScene = require("scene.battle2.BattleScene")
                            cc.Director:getInstance():pushScene(BattleScene:createPK(func.data))
                        end
                    end)  
                end
            end 
            m_baoxiang:addTouchEventListener(showRecord2CallBack)
            m_bofang:addTouchEventListener(showRecord2CallBack)
            
            self.m_RecordListView:pushBackCustomItem(recordItem)
            
        end
    
    end)
end

function GuardUi:convoyRob(id_1,defId_1)
    
    self.m_coverPanel:setVisible(true)
    self.m_ShowHusong:setVisible(false)
    self.m_ShowEnemy:setVisible(true)
    
    local function convoyRob2CallBack(sender,event)--抢夺进入战斗
        if sender == self.m_sureBattleBtn and event == cc.EventCode.ENDED then
        
        --if self.robRestNum ~= 0 then
            self:Go2Convoyfomation(id_1,defId_1)

            self.m_coverPanel:setVisible(false)
            self.m_ShowHusong:setVisible(false)
            self.m_ShowEnemy:setVisible(false)
       -- else
            --PopDialog:getInstance():popText("抢劫失败！今日抢劫次数已用完！",cc.c3b(0,255,0))
        --end
        
            
        end    
    end        

    self.m_sureBattleBtn:addTouchEventListener(convoyRob2CallBack) 
    
end

function GuardUi:robAlready(enemyNum,ownerlist)
    
    local number = table.getn(enemyNum)
    
    
    self.m_LV:setString(ownerlist.level)
    self.m_zhandouli:setString(ownerlist.fc)
    self.m_CoinNum:setString(ownerlist.robGold)
    self.m_touxiangkuang:loadTexture("soul/"..ownerlist.icon..".png")
    self.m_touxiangkuang:setScale(0.45,0.45)
    
    for i = 1,number do
    
        local soulDetail = ccui.Helper:seekWidgetByName(self.rootNode, "heroPic"..i)

        local heroProgress = ccui.Helper:seekWidgetByName(soulDetail, "heroProgress")

        local touxiangkk = ccui.Helper:seekWidgetByName(soulDetail, "touxiangkk")

        soulDetail:setVisible(true)

        soulDetail:loadTexture(soulConfig[tostring(enemyNum[i].id)]["icon"]..".png")
        soulDetail:setScale(0.85,0.85)
        touxiangkk:loadTexture("normal_aochao_0"..soulConfig[tostring(enemyNum[i].id)]["quality"]..".png")
        touxiangkk:setScale(1.0,1.0)
        heroProgress:setPercent(enemyNum[i].curHp/enemyNum[i].hpMax*100)
    end
    
end

function GuardUi:initSmallTruck()
    self.faguang_1:setVisible(false)
    self.faguang_2:setVisible(false)
    self.faguang_3:setVisible(false)
    self.faguang_4:setVisible(false)
end

function GuardUi:myrefresh()
    for i = 1,4 do
        local roblist_1 = ccui.Helper:seekWidgetByName(self.rootNode, "RobList"..i)
        roblist_1:setVisible(false)
    end 
end

function GuardUi:playeffect(truck,idtag)
    
    truck:removeAllChildren()
    
    local armatureFile = "image/ui/guard/JianTaZhanChe/JianTaZhanChe.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    local skillarmature = ccs.Armature:create("JianTaZhanChe")
    skillarmature:setScale(1.3)
    skillarmature:setPosition(170,20)
    skillarmature:setColor(cc.c3b(255,255,255))
    truck:addChild(skillarmature,-270)
    --速度
    skillarmature:getAnimation():setSpeedScale(0.4)
    skillarmature:getAnimation():playWithIndex(0,0,1)
    
    if idtag == 1 then --white
        skillarmature:setColor(cc.c3b(255,255,255))
        truck:loadTexture("tupianbai"..".png")
    elseif idtag == 2 then -- green
        skillarmature:setColor(cc.c3b(76,233,21))
        truck:loadTexture("tupianlv"..".png")
    elseif idtag == 3 then --blue
        skillarmature:setColor(cc.c3b(27,161,255))
        truck:loadTexture("tupianlan"..".png")
    elseif idtag == 4 then --purple
        skillarmature:setColor(cc.c3b(197,40,255))
        truck:loadTexture("tupianzi"..".png")
    elseif idtag == 5 then --red
        skillarmature:setColor(cc.c3b(255,33,33))
        truck:loadTexture("tupianhong"..".png")            
    end

end

--refresh all ui here
function GuardUi:refreshUI()
end

return GuardUi
