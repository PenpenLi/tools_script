--OpenAct.lua, created by php script. chris.li--
require "Cocos2d"

local openActConfig = require("config.define_open_act")
local rewardConfig = require("config.define_open_act_reward")
local shopConfig = require("config.define_shop")
local PopDialog = require "common/PopDialog"

local OpenAct = class("OpenAct", function()
	return cc.Layer:create()
end)

function OpenAct:create(dat)
	local layer = OpenAct.new()
	layer:initM(dat)
	return layer
end

function OpenAct:ctor()
end

function OpenAct:onEnter()
    --刷新充值金额
    if self.chongzhiIndex ~= 0 then
        self:myInterface(self.chongzhiIndex)
        self:updateTipPicNew()
    end
end

function OpenAct:onExit()
end

function OpenAct:initM(dat)
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)
	
	self.ID = 1
	
	self.chongzhiIndex = 0
	
	self.actIdIndex = 0
	
	self.isHigh = 0
	self.isHighArr = {}
	--self.isHighArr_2 = {}

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("OpenAct.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_openActPanel: Layout
	self.m_openActPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_openActPanel")
	self.m_openActPanel:addTouchEventListener(touchEventHandler)
    self.rootNode:setBackGroundImage("ditu.jpg")

	--m_openMainInterface: ImageView
	self.m_openMainInterface = ccui.Helper:seekWidgetByName(self.rootNode, "m_openMainInterface")

	--m_closeOpenAct: Button
	self.m_closeOpenAct = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeOpenAct")
	self.m_closeOpenAct:addTouchEventListener(touchEventHandler)

	--m_alreayChongzhi: Text
	self.m_alreayChongzhi = ccui.Helper:seekWidgetByName(self.rootNode, "m_alreayChongzhi")

	--m_mubiaoBtn: Button
	self.m_mubiaoBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_mubiaoBtn")
	self.m_mubiaoBtn:addTouchEventListener(touchEventHandler)

	--m_paimingBtn: Button
	self.m_paimingBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_paimingBtn")
	self.m_paimingBtn:addTouchEventListener(touchEventHandler)

	--m_listView: ListView
	self.m_listView = ccui.Helper:seekWidgetByName(self.rootNode, "m_listView")
	self.m_listView:addTouchEventListener(touchEventHandler)

	--m_chongzhiBtn: Button
	self.m_chongzhiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_chongzhiBtn")
	self.m_chongzhiBtn:addTouchEventListener(touchEventHandler)

	--m_actName: Text
	self.m_actName = ccui.Helper:seekWidgetByName(self.rootNode, "m_actName")

	--m_actTime: Text
	self.m_actTime = ccui.Helper:seekWidgetByName(self.rootNode, "m_actTime")

	--m_interfaceListView: ListView
	self.m_interfaceListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_interfaceListView")
	self.m_interfaceListView:addTouchEventListener(touchEventHandler)

	--m_chongzhiNum: Text
	self.m_chongzhiNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_chongzhiNum")
	
    --m_desc: Text
    self.m_desc = ccui.Helper:seekWidgetByName(self.rootNode, "Label_14")

	self:initUI(dat)
    --self.Btn:setHighlighted(true)
end

function OpenAct:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_openActPanel and event == cc.EventCode.ENDED then
		cclog("[OpenAct]--m_openActPanel touched--")

	elseif sender == self.m_closeOpenAct and event == cc.EventCode.ENDED then
		cclog("[OpenAct]--m_closeOpenAct touched--")
        local scene = require("scene.home.HomeScene"):create()
        cc.Director:getInstance():replaceScene(scene)

	elseif sender == self.m_mubiaoBtn and event == cc.EventCode.ENDED then
		cclog("[OpenAct]--m_mubiaoBtn touched--")
		if self.ID == 1 then
		    self:myInterface(1)
		elseif self.ID == 6 then
		    self:myInterface(6)
		elseif self.ID == 8 then
            self:myInterface(8)
		end

	elseif sender == self.m_paimingBtn and event == cc.EventCode.ENDED then
		cclog("[OpenAct]--m_paimingBtn touched--")
        self.m_mubiaoBtn:setHighlighted(false)
        self.m_mubiaoBtn:setEnabled(true)
        self.m_paimingBtn:setHighlighted(true)
        self.m_paimingBtn:setEnabled(false)
        if self.ID == 1 then
            Http:req("GetOpenActRequest",{actId = 2},function(res)
                self:placeItem(res.data)
            end)
        elseif self.ID == 6 then
            Http:req("GetOpenActRequest",{actId = 7},function(res)
                self:placeItem(res.data)
            end)
        elseif self.ID == 8 then
            Http:req("GetOpenActRequest",{actId = 9},function(res)
                self:placeItem(res.data)
            end)
        end

	elseif sender == self.m_listView and event == cc.EventCode.ENDED then
		cclog("[OpenAct]--m_listView touched--")

	elseif sender == self.m_chongzhiBtn and event == cc.EventCode.ENDED then
		cclog("[OpenAct]--m_chongzhiBtn touched--")
        local scene = require("scene.shop.RechargeScene"):create()
        cc.Director:getInstance():pushScene(scene)

	elseif sender == self.m_interfaceListView and event == cc.EventCode.ENDED then
		cclog("[OpenAct]--m_interfaceListView touched--")

	end
end

--init your ui here
function OpenAct:initUI(dat)
    --侧边大按钮
    cclog("enter")
    local dataIndex = dat
    
    if dataIndex ~= nil then
        local endTime = dataIndex.data.act.endTm
        local startTime = dataIndex.data.act.startTm
        self.m_actTime:setString(os.date("%Y",startTime).."年"..os.date("%m",startTime).."月"
            ..os.date("%d",startTime).."日至"..os.date("%Y",endTime).."年"..os.date("%m",endTime)
            .."月"..os.date("%d",endTime).."日"..os.date("%H",endTime)..":"..os.date("%M",endTime))

        self.m_chongzhiNum:setString(":"..dataIndex.data.act.num)

        self.m_actName:setString(openActConfig["1"]["name"])
        self.m_desc:setString(openActConfig["1"]["desc"])
    end
    
    
    if  self.m_listView:getChildrenCount() ~= 0 then 
        self.m_listView:removeAllItems()
    end
    local copyitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("buttons.csb")
    local listviewsize = self.m_listView:getContentSize()
    listviewsize.height = copyitem:getContentSize().height*(7)+100
    self.m_listView:setInnerContainerSize(listviewsize)
    
    Http:req("GetRedDot",{multType = "512"},function(rs)
    	for i=1,7 do
            local ButtonItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("buttons.csb")
            --local ButtonItem = copyitem:clone()
            local myButton = ccui.Helper:seekWidgetByName(ButtonItem, "m_button")
            myButton:setName("myButton_"..i)
            local buttonFace = ccui.Helper:seekWidgetByName(ButtonItem, "buttonFace")
            local redDot = ccui.Helper:seekWidgetByName(ButtonItem, "m_redDot")
            redDot:setName("redDot_"..i)
            redDot:setVisible(false)
            
            buttonFace:loadTexture("huodong_"..i..".png")
            buttonFace:setScale(1.0,1.0)
            self.m_listView:pushBackCustomItem(ButtonItem)
            
            --游戏默认状态，首次进入
            if self.isHigh == 0 and self.actIdIndex == 0 then
                self.isHigh = self.isHigh + 1
                cclog("enterif")
                local Btn = ccui.Helper:seekWidgetByName(ButtonItem, "myButton_".."1")
                table.insert(self.isHighArr,Btn)
                self:myInterface(1)
            end
            if i == 1 then
            self.dot1 = ccui.Helper:seekWidgetByName(ButtonItem, "redDot_".."1")
            elseif i == 2 then
            self.dot2 = ccui.Helper:seekWidgetByName(ButtonItem, "redDot_".."2")
            elseif i == 3 then
            self.dot3 = ccui.Helper:seekWidgetByName(ButtonItem, "redDot_".."3")
            elseif i == 5 then
            self.dot4 = ccui.Helper:seekWidgetByName(ButtonItem, "redDot_".."5")
            elseif i == 6 then
            self.dot5 = ccui.Helper:seekWidgetByName(ButtonItem, "redDot_".."6")
            elseif i == 7 then
            self.dot6 = ccui.Helper:seekWidgetByName(ButtonItem, "redDot_".."7")
            end
            
            if i ~= 4 then
                if i < 4 then    
                self:updateTipPic(rs.data,i,redDot)
                elseif i > 4 then
                self:updateTipPic(rs.data,i-1,redDot)
                end
            end
            
            local function Button2CallBack(sender,event)
                if sender == myButton and event == cc.EventCode.ENDED then
                    table.insert(self.isHighArr,myButton)
                    self.isHigh =  self.isHigh + 1
                    
                    local j = 0
                    if i == 1 then
                        j = 1
                    elseif i == 2 then 
                        j = 3
                    elseif i == 3 then
                        j = 4
                    elseif i == 4 then
                        j = 5
                    elseif i == 5 then
                        j = 6
                    elseif i == 6 then
                        j = 8 
                    elseif i == 7 then
                        j = 10                       
                    end
                    self:myInterface(j)
                end
            end
            myButton:addTouchEventListener(Button2CallBack)
        end 
        --self.m_listView:jumpToTop()
        --显示置顶
        self.m_listView:scrollToPercentVertical(1,0,false)
    end)
end


function OpenAct:updateTipPic(tipPic,numberIndex,mydot)
    if tipPic.dot[numberIndex].visible == 1 then
        mydot:setVisible(true)
    else
        mydot:setVisible(false)    
    end
end

function OpenAct:myInterface(OpenActID)
    
    --self:initUI()
    cclog("enter open")
    if table.getn(self.isHighArr) == 1 then
        self.isHighArr[1]:setHighlighted(true)
        self.isHighArr[1]:setEnabled(false)
    elseif table.getn(self.isHighArr) == 2 then
        self.isHighArr[1]:setHighlighted(false)
        self.isHighArr[1]:setEnabled(true)
        self.isHighArr[2]:setHighlighted(true)
        self.isHighArr[2]:setEnabled(false)
        table.remove(self.isHighArr,1)
    end

    Http:req("GetOpenActRequest",{actId = OpenActID},function(res)
        
        if res.data.act.num ~= nil then
            self.m_chongzhiNum:setString(":"..res.data.act.num)
        else
            self.m_chongzhiNum:setString("")    
        end
        
        self.m_alreayChongzhi:setVisible(true)
        self.m_chongzhiBtn:setVisible(true)
        self.m_mubiaoBtn:setVisible(false)
        self.m_paimingBtn:setVisible(false)
        
        local actID = tostring(res.data.act.actId)
        
        self.m_actName:setString(openActConfig[actID]["name"])
        self.m_desc:setString(openActConfig[actID]["desc"])
        
        self:placeItem(res.data)
        
        if OpenActID == 1 then
            self.ID = 1 --累计充值(1,2)
            self.chongzhiIndex = 1
            self.m_alreayChongzhi:setString("您已充值")
            
            self.m_mubiaoBtn:setVisible(true)
            self.m_paimingBtn:setVisible(true)
            
            self.m_mubiaoBtn:setHighlighted(true)
            self.m_mubiaoBtn:setEnabled(false)
            
            self.m_paimingBtn:setHighlighted(false)
            self.m_paimingBtn:setEnabled(true)
        elseif OpenActID == 3 then
            self.chongzhiIndex = 3
            self.m_alreayChongzhi:setString("您已消费")
        elseif OpenActID == 4 then
            self.chongzhiIndex = 4
            self.m_alreayChongzhi:setString("您已充值") 
        elseif OpenActID == 5 then
            self.m_alreayChongzhi:setVisible(false)
            self.m_chongzhiBtn:setVisible(false)
        elseif OpenActID == 6 then
            self.ID = 6 --竞技场(6,7)
        
            self.m_alreayChongzhi:setVisible(false)
            self.m_chongzhiBtn:setVisible(false)
            
            self.m_mubiaoBtn:setVisible(true)
            self.m_paimingBtn:setVisible(true)
            
            self.m_mubiaoBtn:setHighlighted(true)
            self.m_mubiaoBtn:setEnabled(false)
            
            self.m_paimingBtn:setHighlighted(false)
            self.m_paimingBtn:setEnabled(true)
        elseif OpenActID == 8 then
            self.ID = 8 --等级(8,9)
            self.m_alreayChongzhi:setVisible(false)
            self.m_chongzhiBtn:setVisible(false)
            
            self.m_mubiaoBtn:setVisible(true)
            self.m_paimingBtn:setVisible(true)
            
            self.m_mubiaoBtn:setHighlighted(true)
            self.m_mubiaoBtn:setEnabled(false)
            
            self.m_paimingBtn:setHighlighted(false)
            self.m_paimingBtn:setEnabled(true)
        elseif OpenActID == 10 then
        
            self.m_alreayChongzhi:setVisible(false)
            self.m_chongzhiBtn:setVisible(false)                       
        end    
    end)
end

--奖励内容详情
function OpenAct:placeItem(itemList)
    local itemNumber = table.getn(itemList.detail)
    if  self.m_interfaceListView:getChildrenCount() ~= 0 then 
        self.m_interfaceListView:removeAllItems()
    end
    --self.m_interfaceListView:removeAllChildren()
    for i = 1,itemNumber do
        local Item = ccs.GUIReader:getInstance():widgetFromBinaryFile("items.csb")
        local m_paimingNum = ccui.Helper:seekWidgetByName(Item, "m_paimingNum")
        local m_receiveBtn = ccui.Helper:seekWidgetByName(Item, "m_receiveBtn")
        local m_alreadyReceiveBtn = ccui.Helper:seekWidgetByName(Item, "m_alreadyReceiveBtn")
        local m_alreadyLable = ccui.Helper:seekWidgetByName(Item, "m_alreadyLable")
        local m_playerName = ccui.Helper:seekWidgetByName(Item, "m_playerName")
        local m_alreadyReceiveBtnhui = ccui.Helper:seekWidgetByName(Item, "m_alreadyReceiveBtnhui")
        local m_rechargeLable = ccui.Helper:seekWidgetByName(Item, "m_rechargeLable")
        local describLable = ccui.Helper:seekWidgetByName(Item, "zuanshiNum")
        m_paimingNum:setString(i)
        
        if itemList.detail[i].name ~= nil then
            m_playerName:setString(itemList.detail[i].name)
        end
        
        m_rechargeLable:setString(rewardConfig[tostring(itemList.detail[i].actRewardId)]["desc"])
        
        local itemDetail = rewardConfig[tostring(itemList.detail[i].actRewardId)]["item"]
        if #itemDetail ~= 0 then
            local res_2 = string.split(itemDetail, ",")

            for j = 1,table.getn(res_2) do
                local arr = res_2[j]
                local res_1 = string.split(arr, ":")
                local imagePic = ccui.Helper:seekWidgetByName(Item, "imagePic"..j)
                local imageNum = ccui.Helper:seekWidgetByName(imagePic, "itemNum")
                local touxiangkuang = ccui.Helper:seekWidgetByName(imagePic, "touxiangkuang")
                local suipian = ccui.Helper:seekWidgetByName(imagePic, "suipian")
                
                imagePic:setVisible(true)
                if shopConfig[tostring(res_1[1])]["type"] == 16 then
                    suipian:setVisible(true)
                    imagePic:loadTexture("soul/"..shopConfig[tostring(res_1[1])]["icon"]..".png")
                else
                    suipian:setVisible(false)
                    imagePic:loadTexture(shopConfig[tostring(res_1[1])]["icon"]..".png")    
                end
                imagePic:setScale(0.7,0.7)
                touxiangkuang:loadTexture("normal_aochao_0"..shopConfig[tostring(res_1[1])]["quality"]..".png")
                imageNum:setString(res_1[2])
            end
            
        else
            m_rechargeLable:setVisible(false)
            describLable:setVisible(true)
            describLable:setString(rewardConfig[tostring(itemList.detail[i].actRewardId)]["desc"])
        end
        
        if itemList.detail[i].rec ~= nil and itemList.detail[i].name == nil then
            if itemList.detail[i].rec == 0 then
                m_alreadyReceiveBtnhui:setVisible(true)
                m_receiveBtn:setVisible(false)
                m_alreadyReceiveBtn:setVisible(false)
            elseif itemList.detail[i].rec == 1 then
                m_receiveBtn:setVisible(true)
                m_alreadyReceiveBtnhui:setVisible(false)
                m_alreadyReceiveBtn:setVisible(false)
             elseif itemList.detail[i].rec == 2 then
                m_alreadyReceiveBtn:setVisible(true)
                m_alreadyReceiveBtnhui:setVisible(false)
                m_receiveBtn:setVisible(false)
            end
        else
            m_alreadyLable:setVisible(true)
            m_playerName:setVisible(true)
        end
        
        local function Button2CallBack_1(sender,event)
            if sender == m_receiveBtn and event == cc.EventCode.ENDED then
                Http:req("GetOpenActRewardRequest",{actRewardId = itemList.detail[i].actRewardId},function(res)
                    if res.data.result == 0 then
                        PopDialog:getInstance():popText("领取成功！",cc.c3b(0,255,0))
                        m_alreadyReceiveBtn:setVisible(true)
                        m_alreadyReceiveBtnhui:setVisible(false)
                        m_receiveBtn:setVisible(false)
                        self.actIdIndex = itemList.act.actId
                        self:updateTipPicNew()
                    end
                end)   
            end
        end    
        m_receiveBtn:addTouchEventListener(Button2CallBack_1)
        self.m_interfaceListView:pushBackCustomItem(Item)  
    end
    self.m_interfaceListView:jumpToTop()
end
--refresh all ui here
function OpenAct:refreshUI()
    self.dot1:setVisible(false)
    self.dot2:setVisible(false)
    self.dot3:setVisible(false)
    self.dot4:setVisible(false)
    self.dot5:setVisible(false)
    self.dot6:setVisible(false)
end

function OpenAct:updateTipPicNew()
    self:refreshUI()
    Http:req("GetRedDot",{multType = "512"},function(res)
        if res.data.dot[1].visible == 1 then
            self.dot1:setVisible(true)
        end    
        if res.data.dot[2].visible == 1 then
            self.dot2:setVisible(true)
        end    
        if res.data.dot[3].visible == 1 then
            self.dot3:setVisible(true)
        end    
        if res.data.dot[4].visible == 1 then
            self.dot4:setVisible(true)
        end    
        if res.data.dot[5].visible == 1 then
            self.dot5:setVisible(true)
        end    
        if res.data.dot[6].visible == 1 then
            self.dot6:setVisible(true) 
        end      
    end)
end

return OpenAct
