--Smelt.lua, created by php script. chris.li--
require "Cocos2d"

local PopDialog = require "common/PopDialog"

local shopConfig = require("config.define_shop")

local soulConfig = require("config.define_soul")

local Smelt = class("Smelt", function()
	return cc.Layer:create()
end)

function Smelt:create()
	local layer = Smelt.new()
	layer:initM()
	return layer
end

function Smelt:ctor()
end

function Smelt:onEnter()
end

function Smelt:onExit()
end

function Smelt:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("Smelt.csb")
    self.rootNode:setBackGroundImage("ditu.jpg")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_smeltPanel: Layout
	self.m_smeltPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_smeltPanel")
	self.m_smeltPanel:addTouchEventListener(touchEventHandler)
   

	--m_mainPanel: ImageView
	self.m_mainPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_mainPanel")

	--m_listPanel: ImageView
	self.m_listPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_listPanel")
	self.m_listPanel:setVisible(false)

	--m_getPanel: Layout
	self.m_getPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_getPanel")
	self.m_getPanel:addTouchEventListener(touchEventHandler)
	self.m_getPanel:setVisible(false)

	--m_checkeqbtn: Button
	self.m_checkeqbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_checkeqbtn")
	self.m_checkeqbtn:addTouchEventListener(touchEventHandler)

	--m_checkherobtn: Button
	self.m_checkherobtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_checkherobtn")
	self.m_checkherobtn:addTouchEventListener(touchEventHandler)

	--m_closebtn: Button
	self.m_closebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closebtn")
	self.m_closebtn:addTouchEventListener(touchEventHandler)

	--m_smeltbtn: Button
	self.m_smeltbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_smeltbtn")
	self.m_smeltbtn:addTouchEventListener(touchEventHandler)

	--m_listtap1: Button
	self.m_listtap1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_listtap1")
	self.m_listtap1:addTouchEventListener(touchEventHandler)
	self.m_listtap1:setHighlighted(true)
	self.m_listtap1:setEnabled(false)
	self.m_listtap1:setTitleColor(cc.c3b(255,234,79))

	--m_listtap2: Button
	self.m_listtap2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_listtap2")
	self.m_listtap2:addTouchEventListener(touchEventHandler)

	--m_surebtn: Button
	self.m_surebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_surebtn")
	self.m_surebtn:addTouchEventListener(touchEventHandler)

	--m_closebtn2: Button
	self.m_closebtn2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_closebtn2")
	self.m_closebtn2:addTouchEventListener(touchEventHandler)

	--m_tip: Text
	self.m_tip = ccui.Helper:seekWidgetByName(self.rootNode, "m_tip")

	--m_scrollvieww: ScrollView
	self.m_scrollvieww = ccui.Helper:seekWidgetByName(self.rootNode, "m_scrollvieww")
	self.m_scrollvieww:addTouchEventListener(touchEventHandler)
    self.m_scrollvieww:setBounceEnabled(true)

	--m_ListView: ListView
	self.m_ListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_ListView")
	self.m_ListView:addTouchEventListener(touchEventHandler)
    self.m_ListView:setBounceEnabled(true)
    
    self.m_coins = ccui.Helper:seekWidgetByName(self.rootNode, "m_coins")

 --使用方法
 --[[   local richtext = ccui.RichText:create()
    richtext:setContentSize(500,50)
    local richitem1 = ccui.RichElementText:create(1,cc.c3b(125,125,125),255,"hello1","黑体",25)
    local richitem2 = ccui.RichElementText:create(2,cc.c3b(200,200,200),255,"hello2","黑体",25)

    richtext:pushBackElement(richitem1)
    richtext:pushBackElement(richitem2)
    
    local pic = ccui.Helper:seekWidgetByName(self.m_mainPanel, "pic_1")
    pic:addChild(richtext)  ]]
	
	self.checkKey={}
	self.checknum=0
	self.eqlist={}
    self.herolist={}

	self:initUI()
end

function Smelt:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_smeltPanel and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_smeltPanel touched--")

	elseif sender == self.m_getPanel and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_getPanel touched--")
        self:updateMainLayer(0)
		self.m_getPanel:setVisible(false)
		

	elseif sender == self.m_checkeqbtn and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_checkeqbtn touched--")
        self:updateMainLayer(1)

	elseif sender == self.m_checkherobtn and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_checkherobtn touched--")
        self:updateMainLayer(2)
       
	elseif sender == self.m_closebtn and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_closebtn touched--")
        local scene = require("scene.home.HomeScene"):create()
        cc.Director:getInstance():replaceScene(scene)

	elseif sender == self.m_smeltbtn and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_smeltbtn touched--")
        Http:req("SmeltMaterial",nil,function(res)
            if res.data["result"] == 0 then
                self.checkKey = {}
                PopDialog:getInstance():popText("熔炼成功！")
                self.m_coins:setString(res.data["coins"])
                self:showGetPanel(res.data["item"])
            elseif res.data["result"] == 1 then
                PopDialog:getInstance():popText("请放入熔炼材料！")
            else
            
            end
        end)

	elseif sender == self.m_listtap1 and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_listtap1 touched--")
		self.m_listtap1:setTitleColor(cc.c3b(255,234,79))
        self.m_listtap1:setHighlighted(true)
        self.m_listtap1:setEnabled(false)
        self.m_listtap2:setTitleColor(cc.c3b(255,255,255))
        self.m_listtap2:setHighlighted(false)
        self.m_listtap2:setEnabled(true)
        
        self:showListPanel(0)

	elseif sender == self.m_listtap2 and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_listtap2 touched--")
		self.m_listtap2:setTitleColor(cc.c3b(255,234,79))
        self.m_listtap2:setHighlighted(true)
        self.m_listtap2:setEnabled(false)
        self.m_listtap1:setHighlighted(false)
        self.m_listtap1:setEnabled(true)
        self.m_listtap1:setTitleColor(cc.c3b(255,255,255))
        
        self:showListPanel(1)

	elseif sender == self.m_surebtn and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_surebtn touched--")
        if table.getn(self.checkKey) == 0 then
            PopDialog:getInstance():popText("请放入熔炼材料！")
            return
		end
        Http:req("CheckSmeltMaterial",{key = self.checkKey},function(res)
                self:updateMainLayer(0)
           
        end)

	elseif sender == self.m_closebtn2 and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_closebtn2 touched--")
        
        self:updateMainLayer(0)
          
	elseif sender == self.m_scrollvieww and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_scrollvieww touched--")

	elseif sender == self.m_ListView and event == cc.EventCode.ENDED then
		cclog("[Smelt]--m_ListView touched--")

	end
end

function Smelt:showGetPanel(data)
    self.m_scrollvieww:removeAllChildren()

    local itemList = data
    local itemListSize = table.getn(itemList)
    local ScrovSize = self.m_scrollvieww:getInnerContainerSize()

    local ScrovH = ScrovSize.height
    if itemListSize>4 then
        if itemListSize%2 == 0 then
            ScrovH = (90+10)*(itemListSize/2)
        else
            ScrovH = (90+10)*(math.floor(itemListSize/2)+1)
        end

    end

    ScrovSize.height = ScrovH
    self.m_scrollvieww:setInnerContainerSize(ScrovSize)
    
    for i=1, itemListSize do
        local equipItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("Smelt_item2.csb")
        self.m_scrollvieww:addChild(equipItem)
        equipItem:setPosition((i-1)%2*200+30,ScrovSize.height-100*(math.floor((i-1)/2)+1))
            
        local pinzhi = ccui.Helper:seekWidgetByName(equipItem, "pinzhi")
        local pic = ccui.Helper:seekWidgetByName(equipItem, "pic")
        local num = ccui.Helper:seekWidgetByName(equipItem, "num")
        
        num:setString("x"..itemList[i]["num"])
        
        local dataitem=shopConfig[tostring(itemList[i]["itemId"])]
        pic:loadTexture(dataitem["icon"]..".png")
        pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
    end
            
	self.m_getPanel:setVisible(true)
end

function Smelt:updateMainLayer(refreshtype)
    Http:req("GetSmeltMain",{type = refreshtype},function (res)
        self.m_listPanel:setVisible(false)
        self.m_mainPanel:setVisible(true)
        
        local oldchecknum = self.checknum
        
        
        local holelist = res.data["hole"]
         self.checknum = table.getn(holelist)
         
        if oldchecknum == self.checknum then
            if refreshtype == 1 then
                PopDialog:getInstance():popText("没有更多的可熔炼装备！")
            elseif refreshtype == 2 then
                PopDialog:getInstance():popText("没有更多的可熔炼英雄！")
            end
        end
         
        for i = 1,5 do
            local pic = ccui.Helper:seekWidgetByName(self.m_mainPanel, "pic_"..i)
            local pinzhi = ccui.Helper:seekWidgetByName(pic, "pinzhi")
            local num = ccui.Helper:seekWidgetByName(pic, "num")
            local issuipian = ccui.Helper:seekWidgetByName(pic, "issuipian")
            
            pic:loadTexture("touxiangkuang_02.png")
            pinzhi:loadTexture("normal_aochao_01.png")
            issuipian:setVisible(false)
            num:setString("0")
            num:setVisible(false)
        end 
         
         
        for i = 1, table.getn(holelist) do
            self.checkKey[i] = holelist[i]["key"]
            
            
            local pic = ccui.Helper:seekWidgetByName(self.m_mainPanel, "pic_"..i)
            local pinzhi = ccui.Helper:seekWidgetByName(pic, "pinzhi")
            local num = ccui.Helper:seekWidgetByName(pic, "num")
            local issuipian = ccui.Helper:seekWidgetByName(pic, "issuipian")
            
            num:setString(holelist[i]["num"])
            num:setVisible(true)
            
            local dataitem = nil
            if holelist[i]["key"]["type"] == 0 then
                dataitem=shopConfig[tostring(holelist[i]["itemId"])]
            else
                dataitem=soulConfig[tostring(holelist[i]["itemId"])]
            end
            
            pic:loadTexture(dataitem["icon"]..".png")
            pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            
            if tonumber(dataitem["type"]) == 16 then
            	issuipian:setVisible(true)
            else
                issuipian:setVisible(false)
            end
        end
    end)
end

--0为装备列表，1为英雄列表
function Smelt:showListPanel(type)
    self.m_ListView:removeAllItems()
    
    if type == 0 then
    	for i = 1,table.getn(self.eqlist) do
            local MaterialItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("Smelt_item1.csb")
            self.m_ListView:pushBackCustomItem(MaterialItem)
            
            local pinzhi = ccui.Helper:seekWidgetByName(MaterialItem, "pinzhi")
            local pic = ccui.Helper:seekWidgetByName(MaterialItem, "pic")
            
            local dataitem=shopConfig[tostring(self.eqlist[i]["itemId"])]
            pic:loadTexture(dataitem["icon"]..".png")
            pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            
            local name = ccui.Helper:seekWidgetByName(MaterialItem, "name")
            local level = ccui.Helper:seekWidgetByName(MaterialItem, "level")
            local num = ccui.Helper:seekWidgetByName(MaterialItem, "num")
            local CheckBox = ccui.Helper:seekWidgetByName(MaterialItem, "CheckBox")
            local issuipian = ccui.Helper:seekWidgetByName(MaterialItem, "issuipian")
            
            if tonumber(dataitem["type"]) == 16 then
            	issuipian:setVisible(true)
            else
                issuipian:setVisible(false)
            end
            
            name:setString(dataitem["name"])
            level:setString("等级："..self.eqlist[i]["level"])
            num:setString("数量："..self.eqlist[i]["num"])
            
            if self.eqlist[i]["checked"] == 0 then
            	CheckBox:setSelectedState(false)
            else
                CheckBox:setSelectedState(true)
            end
            
            local function CheckBoxCallBack(sender, event)
                local tag = sender:getTag()
                if event == CHECKBOX_STATE_EVENT_SELECTED then
            		if self.checknum<5 then
                        self.checknum = self.checknum + 1
                        self.checkKey[self.checknum] = self.eqlist[tag]["key"]
                        self.eqlist[tag]["checked"] = 1
            		else
                        CheckBox:setSelectedState(false)  
            		end
            	else
                    for i = 1, table.getn(self.checkKey) do
                        if self.checkKey[i]["id"] == self.eqlist[tag]["key"]["id"] and
                            self.checkKey[i]["type"] == self.eqlist[tag]["key"]["type"] then
                            table.remove(self.checkKey,i)
                            self.checknum = self.checknum - 1
                            self.eqlist[tag]["checked"] = 0
                            self.m_tip:setString("已选择："..self.checknum.."/5")
                            return
            	    	end
            	    end
                    
            	end
            	
                
                self.m_tip:setString("已选择："..self.checknum.."/5")
            end
            
            CheckBox:addEventListener(CheckBoxCallBack)
            CheckBox:setTag(i)
    	end
    else
        for i = 1,table.getn(self.herolist) do
            local MaterialItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("Smelt_item1.csb")
            self.m_ListView:pushBackCustomItem(MaterialItem)

            local pinzhi = ccui.Helper:seekWidgetByName(MaterialItem, "pinzhi")
            local pic = ccui.Helper:seekWidgetByName(MaterialItem, "pic")

            local dataitem = nil 
            if self.herolist[i]["key"]["type"] == 0 then
                dataitem=shopConfig[tostring(self.herolist[i]["itemId"])]
            else
                dataitem=soulConfig[tostring(self.herolist[i]["itemId"])]
            end
            
            pic:loadTexture(dataitem["icon"]..".png")
            pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")

            local name = ccui.Helper:seekWidgetByName(MaterialItem, "name")
            local level = ccui.Helper:seekWidgetByName(MaterialItem, "level")
            local num = ccui.Helper:seekWidgetByName(MaterialItem, "num")
            local CheckBox = ccui.Helper:seekWidgetByName(MaterialItem, "CheckBox")
            local issuipian = ccui.Helper:seekWidgetByName(MaterialItem, "issuipian")
            
            if tonumber(dataitem["type"]) == 16 then
                issuipian:setVisible(true)
            else
                issuipian:setVisible(false)
            end

            name:setString(dataitem["name"])
            level:setString("等级："..self.herolist[i]["level"])
            num:setString("数量："..self.herolist[i]["num"])

            if self.herolist[i]["checked"] == 0 then
                CheckBox:setSelectedState(false)
            else
                CheckBox:setSelectedState(true)
            end

            local function CheckBoxCallBack(sender, event)
                local tag = sender:getTag()
                if event == CHECKBOX_STATE_EVENT_SELECTED then
                    if self.checknum<5 then
                        self.checknum = self.checknum + 1
                        self.checkKey[self.checknum] = self.herolist[tag]["key"]
                        self.herolist[tag]["checked"] = 1
                    else
                        CheckBox:setSelectedState(false)  
                    end
                else
                    for i = 1, table.getn(self.checkKey) do
                        if self.checkKey[i]["id"] == self.herolist[tag]["key"]["id"] and
                            self.checkKey[i]["type"] == self.herolist[tag]["key"]["type"] then
                            table.remove(self.checkKey,i)
                            self.checknum = self.checknum - 1
                            self.herolist[tag]["checked"] = 0
                            self.m_tip:setString("已选择："..self.checknum.."/5")
                            return
                        end
                    end
                    
                    
                end
                
                
                self.m_tip:setString("已选择："..self.checknum.."/5")
            end

            CheckBox:addEventListener(CheckBoxCallBack)
            CheckBox:setTag(i)
        end
    end


    self.m_listPanel:setVisible(true)
    self.m_mainPanel:setVisible(false)
end

function Smelt:updateListData()
    Http:req("GetSmeltMaterial",nil,function(res)
       self.m_tip:setString("已选择："..res.data["checks"].."/5")
        
        self.eqlist = res.data["equip"]
        self.herolist = res.data["soul"]
        
        self:handleTouchEvent(self.m_listtap1,cc.EventCode.ENDED)
    end)
end

--init your ui here
function Smelt:initUI()
    for i = 1, 5 do
        local pic = ccui.Helper:seekWidgetByName(self.m_mainPanel, "pic_"..i)
        local pinzhi = ccui.Helper:seekWidgetByName(pic, "pinzhi")
        
        local function openListPanel(sender, event)
            if sender == pinzhi and event == cc.EventCode.ENDED then
        		self:updateListData()
        	end
        end
        pinzhi:addTouchEventListener(openListPanel)
    end
    self:updateMainLayer(3)
end

--refresh all ui here
function Smelt:refreshUI()
end

return Smelt
