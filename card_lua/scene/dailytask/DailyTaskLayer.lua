--DailyTaskLayer.lua, created by php script. chris.li--
require "Cocos2d"
local dailyConfig = require("config.define_daily_task")
local shopConfig = require("config.define_shop")
local PopDialog = require "common/PopDialog"
local OpenConditions = require("config.define_town")
local DailyTaskLayer = class("DailyTaskLayer", function()
	return cc.Layer:create()
end)

function DailyTaskLayer:create(param)
	local layer = DailyTaskLayer.new()
    layer:initM(param)
	return layer
end

function DailyTaskLayer:ctor()
end

function DailyTaskLayer:onEnter()
    if  self.param then
        self.tasklist = self.param.data["task"]
        self:updateListView()
        self.param = nil
    else
        Http: req("GetMainDt",
            nil,
            function(res) 
                print("onenter")
                if res.err then cclog("err!!!") return end
                self.tasklist = res.data["task"]
                self:updateListView()                  
            end)
    end
   
end

function DailyTaskLayer:onExit()
end

function DailyTaskLayer:initM(param)
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
    
    --m_percentSlider: Slider
    self.m_percentSlider = ccui.Helper:seekWidgetByName(self.rootNode, "m_percentSlider")
    
	--m_closeBtn: Button
	self.m_closeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeBtn")
	self.m_closeBtn:addTouchEventListener(touchEventHandler)
    
    --ImageTag1
    self.ImageTag1 = ccui.Helper:seekWidgetByName(self.rootNode, "ImageTag1")
    
    self:initUI(param)
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
		self.m_closeBtn:setTouchEnabled(false)
        cc.Director:getInstance():popScene()
	end
end

--init your ui here
function DailyTaskLayer:initUI(param)
    self.ImageTag1:loadTexture("tongyongdi.png")
    self.m_dailytaskLayout:setBackGroundImage("ditu.jpg")
    --[[
    Http: req("GetMainDt",
              nil,
              function(res) 
                   if res.err then cclog("err!!!") return end
                   self.tasklist = res.data["task"]
                   self:updateListView()                  
    end)
    ]]
    self.param = param
    
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
    
    local copyitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("DailyTaskItemLayer.csb")
    local listviewsize = self.m_dailytaskListView:getContentSize()
    listviewsize.height = copyitem:getContentSize().height*(#tasklist)+8*(#tasklist-1)
    self.m_dailytaskListView:setInnerContainerSize(listviewsize) 
    
    for i=1, table.getn(tasklist) do
        local tasktable = tasklist[i]
        local taskitem = copyitem:clone()
        
        local m_rewardImage = ccui.Helper:seekWidgetByName(taskitem, "m_rewardImage")
        local m_descLabel = ccui.Helper:seekWidgetByName(taskitem, "m_descLabel")
        --local m_stateImage = ccui.Helper:seekWidgetByName(taskitem, "m_stateImage")
        local m_stateBtn = ccui.Helper:seekWidgetByName(taskitem, "m_stateBtn")
        local m_jumpToBtn = ccui.Helper:seekWidgetByName(taskitem, "m_jumpToBtn")
        local m_progressLabel = ccui.Helper:seekWidgetByName(taskitem, "m_progressLabel")
        local m_pinzhiImage = ccui.Helper:seekWidgetByName(taskitem, "m_pinzhiImage")
        local m_nameLabel = ccui.Helper:seekWidgetByName(taskitem, "m_nameLabel")
        local m_rewardTypeImage = ccui.Helper:seekWidgetByName(taskitem, "m_rewardTypeImage")
        local m_rewardNumLabel = ccui.Helper:seekWidgetByName(taskitem, "m_rewardNumLabel")
        local tag_Image1 = ccui.Helper:seekWidgetByName(taskitem, "ImageTag1")
        local tag_Layout = ccui.Helper:seekWidgetByName(taskitem,"dailytaskItemLayout")
        local m_rewardListView = ccui.Helper:seekWidgetByName(taskitem, "m_rewardListView")
        
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
            

            
            m_progressLabel:setString("进度:"..tasktable["pro"].."/"..dataitem["num"])  
            
            --local shopItem = shopConfig[dataitem["icon"]]
            m_rewardImage:loadTexture(dataitem["icon"]..".png")
            --m_pinzhiImage:loadTexture("normal_aochao_0"..shopItem["quality"]..".png")
            
            if  tasktable["state"] == 0 then
                m_stateBtn:setVisible(false)
                m_stateBtn:setTouchEnabled(false)
            --    m_stateImage:setVisible(true)
                tag_Image1:loadTexture("hengdi04.png")
                --tag_Layout:setBackGroundImage("hengdi02.png")
                taskitem:setBackGroundImage("hengdi02.png")
            else
                m_jumpToBtn:setVisible(false)
                m_stateBtn:setVisible(true)
                m_stateBtn:setTouchEnabled(true)
                m_progressLabel:setVisible(false)   
            end
            
            if  tostring(dataitem["type"]) == "9" then
                --m_stateImage:setPosition(m_stateBtn:getPositionX(),taskitem:getCustomSize().height/2)
                --m_stateImage:setVisible(true)
                m_jumpToBtn:setVisible(false)
                m_progressLabel:setVisible(false)  
            end
            
            local function endjumpToFunc(sender,event)
            	if sender == m_jumpToBtn and event == cc.EventCode.ENDED then
                    local type =  tonumber(dataitem["type"])
                    if  type == 0 then
                        Http: req("GetAllStage",
                            {pveType = 0,mapId = 0},
                            function(res) 
                                if res.err then cclog("err!!!") return end
                                local scene = require("scene.pve.PVEScene"):create(res)
                                cc.Director:getInstance():replaceScene(scene)
                            end)
                    elseif type == 3 then
                        local scene = require("scene.pickCard.pickCardScene"):create()
                        cc.Director:getInstance():replaceScene(scene)
                    elseif type == 4 then
                        Http: req("BagList",
                            nil,
                            function(res) 
                                if res.err then cclog("err!!!") return end 
                                local scene = require("scene.bag.BagScene"):create(res)
                                cc.Director:getInstance():pushScene(scene)
                            end)
                    elseif type == 5 then
                        if Roler:get("level") < tonumber(OpenConditions["5"]["need_level"]) then
                            PopDialog:getInstance():popText("夺宝"..OpenConditions["5"]["need_level"].."级开放")
                        else
                            local scene = require("scene.indiana.IndianaScene"):create()
                            cc.Director:getInstance():pushScene(scene)
                        end     
                    elseif type == 6 then
                        if Roler:get("level") < tonumber(OpenConditions["1"]["need_level"]) then
                            PopDialog:getInstance():popText("竞技场"..OpenConditions["1"]["need_level"].."级开放")
                        else
                            local scene = require("scene.sport.SportScene"):create()
                            cc.Director:getInstance():pushScene(scene)
                        end
                        
                    elseif type == 10 then
                        local scene = require("scene.shop.RechargeScene"):create()
                        cc.Director:getInstance():pushScene(scene)
                    end
                end
            end
            m_jumpToBtn:addTouchEventListener(endjumpToFunc)
            
            local goldReward = tostring(dataitem["gold"])
            local coinsReward = tostring(dataitem["coins"])           
            local energyReward = tostring(dataitem["energy"])
            local expReward = tostring(dataitem["exp"])
            local itemReward = dataitem["item"]
            
            if  goldReward ~= tostring(0) then
                local image = ccui.ImageView:create()
                local label = ccui.Text:create()
                label:setColor(cc.c3b(82,41,1))
                --image:ignoreContentAdaptWithSize(false)
                --image:setContentSize(30,30)
                label:setFontSize(20)
                label:setAnchorPoint(cc.p(0.5,0.5))
                image:loadTexture("baoshi.png")
                local str = string.format("x%s",goldReward)
                label:setString(str)
                m_rewardListView:pushBackCustomItem(image)
                m_rewardListView:pushBackCustomItem(label)
            end
            
            if  coinsReward ~= tostring(0) then
                local image = ccui.ImageView:create()
                local label = ccui.Text:create()
                label:setAnchorPoint(cc.p(0.5,0.5))
                label:setColor(cc.c3b(82,41,1))
                --image:ignoreContentAdaptWithSize(false)
                --image:setContentSize(30,30)
                label:setFontSize(20)
                image:loadTexture("jinbi.png")
                label:setString("x"..coinsReward)
                m_rewardListView:pushBackCustomItem(image)
                m_rewardListView:pushBackCustomItem(label)
            end
            
            if  expReward ~= tostring(0) then
                local image = ccui.ImageView:create()
                local label = ccui.Text:create()
                label:setAnchorPoint(cc.p(0.5,0.5))
                label:setColor(cc.c3b(82,41,1))
                --image:ignoreContentAdaptWithSize(false)
                --image:setContentSize(30,30)
                label:setFontSize(20)
                image:loadTexture("ex.png")
                label:setString("x"..expReward)
                m_rewardListView:pushBackCustomItem(image)
                m_rewardListView:pushBackCustomItem(label)
            end
            
            if  energyReward ~= tostring(0) then
                local label = ccui.Text:create()
                
                local image = ccui.ImageView:create()
                image:ignoreContentAdaptWithSize(false)
                image:setContentSize(35,35)
                image:loadTexture("common/tili.png")
                label:setAnchorPoint(cc.p(0.5,0.5))
                label:setColor(cc.c3b(82,41,1))
                label:setFontSize(20)
                label:setString("x"..energyReward)
                m_rewardListView:pushBackCustomItem(image)
                m_rewardListView:pushBackCustomItem(label)
            end
            
            if  itemReward ~= nil then
                local image = ccui.ImageView:create()
                image:ignoreContentAdaptWithSize(false)
                image:setContentSize(30,30)
                --rootNode:setScale(0.45)
                image:setAnchorPoint(cc.p(0.5,0.5))
                --m_numLabel:setVisible(false)
                if  itemReward ~= "" then
                    local m_pinzhiImage = ccui.ImageView:create()
                    local m_suipianImage = ccui.ImageView:create()
                    
                    m_pinzhiImage:ignoreContentAdaptWithSize(false)
                    m_pinzhiImage:setContentSize(35,35)
                    m_pinzhiImage:setPosition(15,15)
                    
                    image:addChild(m_pinzhiImage)
                    image:addChild(m_suipianImage)
                    m_suipianImage:loadTexture("suipian01.png")
                    m_suipianImage:ignoreContentAdaptWithSize(false)
                    m_suipianImage:setContentSize(10,10)
                    m_suipianImage:setPosition(25,25)
                    
                    local list = string.split(dataitem["item"],",")
                    local shopItem = shopConfig[list[1]]
                    image:setVisible(true)
                    image:loadTexture(shopItem["icon"]..".png")
                    m_pinzhiImage:loadTexture("normal_aochao_0"..shopItem["quality"]..".png")
                    if  shopItem["type"] == 16 then
                        print("type.."..shopItem["type"])
                        m_suipianImage:setVisible(true)
                    end

                    m_rewardListView:pushBackCustomItem(image)
                    local label = ccui.Text:create()
                    label:setAnchorPoint(cc.p(0.5,0.5))
                    label:setColor(cc.c3b(82,41,1))
                    label:setFontSize(20)
                    label:setString("x"..list[2])
                    m_rewardListView:pushBackCustomItem(label)

                end
                


            end           
        end
        self.m_dailytaskListView:pushBackCustomItem(taskitem)      
    end
    self.m_dailytaskListView:jumpToTop()
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
