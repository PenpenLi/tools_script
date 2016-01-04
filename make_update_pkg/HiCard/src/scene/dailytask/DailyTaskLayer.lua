--DailyTaskLayer.lua, created by php script. chris.li--
require "Cocos2d"
local dailyConfig = require("config.define_daily_task")
local shopConfig = require("config.define_shop")
local PopDialog = require "common/PopDialog"

local DailyTaskLayer = class("DailyTaskLayer", function()
	return cc.Layer:create()
end)

function DailyTaskLayer:create()
	local layer = DailyTaskLayer.new()
	layer:initM()
	return layer
end

function DailyTaskLayer:ctor()
end

function DailyTaskLayer:onEnter()
end

function DailyTaskLayer:onExit()
end

function DailyTaskLayer:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("DailyTaskLayer.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_dailytaskLayout: Layout
	self.m_dailytaskLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_dailytaskLayout")
	self.m_dailytaskLayout:addTouchEventListener(touchEventHandler)

	--m_dailytaskListView: ListView
	self.m_dailytaskListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_dailytaskListView")
	self.m_dailytaskListView:addTouchEventListener(touchEventHandler)
    
    --m_percentSlider: ListView
    self.m_percentSlider = ccui.Helper:seekWidgetByName(self.rootNode, "m_percentSlider")
    
	--m_closeBtn: Button
	self.m_closeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeBtn")
	self.m_closeBtn:addTouchEventListener(touchEventHandler)
    
    --ImageTag1
    self.ImageTag1 = ccui.Helper:seekWidgetByName(self.rootNode, "ImageTag1")
    
	self:initUI()
end

function DailyTaskLayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_dailytaskLayout and event == cc.EventCode.ENDED then
		cclog("[DailyTaskLayer]--m_dailytaskLayout touched--")

	elseif sender == self.m_dailytaskListView and event == cc.EventCode.ENDED then
		cclog("[DailyTaskLayer]--m_dailytaskListView touched--")
        SetSlider(self.m_dailytaskListView,self.m_percentSlider)
        
	elseif sender == self.m_closeBtn and event == cc.EventCode.ENDED then
		cclog("[DailyTaskLayer]--m_closeBtn touched--")
        cc.Director:getInstance():popScene()
        
	end
end

--init your ui here
function DailyTaskLayer:initUI()
    self.ImageTag1:loadTexture("kuang.png")
    self.m_dailytaskLayout:setBackGroundImage("ditu.jpg")
    Http: req("GetMainDt",
              nil,
              function(res) 
                   if res.err then cclog("err!!!") return end
                   self.tasklist = res.data["task"]
                   self:updateListView()                  
    end)
    
end

--refresh all ui here
function DailyTaskLayer:refreshUI()
    Http: req("GetMainDt",
        nil,
        function(res) 
            if res.err then cclog("err!!!") return end
            self.tasklist = res.data["task"]
            self:updateListView()                  
        end)
end

function DailyTaskLayer:updateListView()
    
    if  self.m_dailytaskListView:getChildrenCount() ~= 0 then 
        self.m_dailytaskListView:removeAllItems()
    end
    local tasklist = self.tasklist
    for i=1, table.getn(tasklist) do
        local tasktable = tasklist[i]
        local taskitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("DailyTaskItemLayer.csb")
        
        local m_rewardImage = ccui.Helper:seekWidgetByName(taskitem, "m_rewardImage")
        local m_descLabel = ccui.Helper:seekWidgetByName(taskitem, "m_descLabel")
        local m_stateImage = ccui.Helper:seekWidgetByName(taskitem, "m_stateImage")
        local m_stateBtn = ccui.Helper:seekWidgetByName(taskitem, "m_stateBtn")
        local m_progressLabel = ccui.Helper:seekWidgetByName(taskitem, "m_progressLabel")
        local m_pinzhiImage = ccui.Helper:seekWidgetByName(taskitem, "m_pinzhiImage")
        local m_nameLabel = ccui.Helper:seekWidgetByName(taskitem, "m_nameLabel")
        local m_rewardTypeImage = ccui.Helper:seekWidgetByName(taskitem, "m_rewardTypeImage")
        local m_rewardNumLabel = ccui.Helper:seekWidgetByName(taskitem, "m_rewardNumLabel")
        local tag_Image1 = ccui.Helper:seekWidgetByName(taskitem, "ImageTag1")
        local tag_Layout = ccui.Helper:seekWidgetByName(taskitem,"dailytaskItemLayout")
        
        local function taskItemTouchFunc()
            SetSlider(self.m_dailytaskListView,self.m_percentSlider)    	
        end
        taskitem:addTouchEventListener(taskItemTouchFunc)
        
        local function getReward(sender,event)
            if sender ==m_stateBtn and event == cc.EventCode.ENDED then
                Http: req("GetDtReward",
                    {taskId=tasktable["taskId"]},
                    function(res) 
                        if res.err then cclog("err!!!") return end
                        if  self:checkResult(res.data["result"]) then
                            print("accept reward success")
                            self:refreshUI()
                        end
                    end)
            end
        end
        
        m_stateBtn:addTouchEventListener(getReward)
        
        local dailyTaskId = tostring(tasktable["taskId"])
        if  dailyConfig[dailyTaskId] ~= nil then
            local dataitem = dailyConfig[dailyTaskId]
            m_descLabel:setString(dataitem["desc"])
            m_nameLabel:setString(dataitem["name"])
            --m_pinzhiLayout:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            
            if  dataitem["type"] == "9" then
                m_stateBtn:setPosition(m_stateBtn:getPositionX(),taskitem:getCustomSize().height/2)
                m_stateImage:setPosition(m_stateBtn:getPositionX(),taskitem:getCustomSize().height/2)
                m_progressLabel:setVisible(false)  
            end
            
            m_progressLabel:setString("进度:"..tasktable["pro"].."/"..dataitem["num"])  
            
            local shopItem = shopConfig[dataitem["icon"]]
            m_rewardImage:loadTexture(shopItem["icon"]..".png")
            m_pinzhiImage:loadTexture("normal_aochao_0"..shopItem["quality"]..".png")
            
            if  tasktable["state"] == 0 then
                m_stateBtn:setVisible(false)
                m_stateBtn:setTouchEnabled(false)
            --    m_stateImage:setVisible(true)
                tag_Image1:loadTexture("hengdi04.png")
                --tag_Layout:setBackGroundImage("hengdi02.png")
                taskitem:setBackGroundImage("hengdi02.png")
            else
                m_stateBtn:setTouchEnabled(true)
                m_stateBtn:setPosition(m_progressLabel:getPosition())
                m_progressLabel:setVisible(false)   
            end
            
            if  dataitem["coins"] ~= tostring(0) then
                m_rewardNumLabel:setString("X"..dataitem["coins"])
                m_rewardTypeImage:loadTexture("jinbi.png")
            elseif dataitem["gold"] ~= tostring(0) then
                m_rewardNumLabel:setString("X"..dataitem["gold"])
                m_rewardTypeImage:loadTexture("baoshi.png")
            elseif dataitem["energy"] ~= tostring(0) then
                m_rewardNumLabel:setString("体力X"..dataitem["energy"])
                m_rewardTypeImage:setVisible(false)
                m_rewardNumLabel:setPositionX(m_rewardNumLabel:getPositionX()-50)
            elseif dataitem["item"] ~= nil then
                local list = string.split(dataitem["item"],",")

                m_rewardNumLabel:setString(shopItem["name"].."X"..list[2])
                m_rewardTypeImage:setVisible(false)
                m_rewardNumLabel:setPositionX(m_rewardNumLabel:getPositionX()-50)
            end
            
        end
        self.m_dailytaskListView:pushBackCustomItem(taskitem)      
    end
end

function DailyTaskLayer:checkResult(_result)
    local bRet=false
    if _result== 0 then
        PopDialog:getInstance():popText("领取成功",cc.c3b(0,255,0)) 
        bRet = true
    elseif  _result == 1 then
        cclog("DT_GOLD_NOT_ENOUGH")
        PopDialog:getInstance():popText("金币不足",cc.c3b(0,255,0)) 
    elseif  _result == 2 then
        cclog("DT_COUNT_NOT_ENOUGH")
    elseif  _result == 3 then
        cclog("DT_BUY_COUNT_NOT_ENOUGH")
    elseif  _result == 4 then
        cclog("DT_BAG_NOT_ENOUGH")
        PopDialog:getInstance():popText("背包空间不足",cc.c3b(0,255,0)) 
    end
    return bRet
end

return DailyTaskLayer
