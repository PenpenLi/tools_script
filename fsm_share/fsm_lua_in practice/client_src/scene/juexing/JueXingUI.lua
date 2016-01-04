--JueXingUI.lua, created by php script. chris.li--
require "Cocos2d"

local soulConfig = require("config.define_soul")

local juexingConfig = require("config.juexing_skill")

local JueXingUI = class("JueXingUI", function()
	return cc.Layer:create()
end)

function JueXingUI:create()
	local layer = JueXingUI.new()
	layer:initM()
	return layer
end

function JueXingUI:ctor()
end

function JueXingUI:onEnter()
end

function JueXingUI:onExit()
end

function JueXingUI:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("JueXingUI.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_Panel: Layout
	self.m_Panel = ccui.Helper:seekWidgetByName(self.rootNode, "m_Panel")
	self.m_Panel:addTouchEventListener(touchEventHandler)
	self.m_Panel:setBackGroundImage("ditu.jpg")

	--m_ListView: ListView
	self.m_ListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_ListView")
	self.m_ListView:addTouchEventListener(touchEventHandler)
    self.m_ListView:removeAllItems()
    self.m_ListView:setBounceEnabled(true)

	--m_closebtn: Button
	self.m_closebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closebtn")
	self.m_closebtn:addTouchEventListener(touchEventHandler)
	
    --m_Slider
    self.m_Slider = ccui.Helper:seekWidgetByName(self.rootNode, "m_Slider")
    self.m_Slider:setVisible(false)
	
	self.juexinglistsize = 0
	
    for k, v in pairs(juexingConfig) do
        self.juexinglistsize = self.juexinglistsize +1
	end

	self:initUI()
end

function JueXingUI:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_Panel and event == cc.EventCode.ENDED then
		cclog("[JueXingUI]--m_Panel touched--")

	elseif sender == self.m_ListView then
		cclog("[JueXingUI]--m_ListView touched--")
        SetSlider(self.m_ListView,self.m_Slider)

	elseif sender == self.m_closebtn and event == cc.EventCode.ENDED then
		cclog("[JueXingUI]--m_closebtn touched--")
        local scene = require("scene.home.HomeScene"):create()
        cc.Director:getInstance():replaceScene(scene)
	end
end

--init your ui here
function JueXingUI:initUI()
    local tmpJXitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("JueXingUI_item.csb")
    for i =1, self.juexinglistsize do
    	local juexingid = tostring(i)
        local juexingdata = juexingConfig[juexingid]
        
        --local JXitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("JueXingUI_item.csb")
        local JXitem = tmpJXitem:clone()
        self.m_ListView:pushBackCustomItem(JXitem)
        
        local juexingpic = ccui.Helper:seekWidgetByName(JXitem, "juexingpic")
        
        local name = ccui.Helper:seekWidgetByName(JXitem, "name")
        
        local heronum = ccui.Helper:seekWidgetByName(JXitem, "heronum")
        
        local desc = ccui.Helper:seekWidgetByName(JXitem, "desc")
        desc:setString("效果："..juexingdata["dic"])
        desc:setFontName("fonts/DFGB_Y7_0.ttf")
   --[[     desc:setVisible(false)
        local JXdesc = ccui.Text:create()
        JXdesc:setFontSize(20)
        JXdesc:setColor(cc.c3b(255,234,79))
        JXdesc:setFontName("黑体")
        JXdesc:setAnchorPoint(0,0)
        JXdesc:setPosition(desc:getPositionX(),desc:getPositionY()-40)
        JXdesc:setString("效果："..juexingdata["dic"])
        JXdesc:ignoreContentAdaptWithSize(false)
        JXdesc:setContentSize(540,60)
        JXitem:addChild(JXdesc)
        ]]--
        
        juexingpic:loadTexture(juexingdata["icon"]..".png")
        name:setString(juexingdata["name"])
        heronum:setString("(需"..juexingdata["lv_num"].."英雄)")
       
        
        local soullist = string.split(juexingdata["hero_id"],";")
        for j = 1, 6 do
            local soulpic = ccui.Helper:seekWidgetByName(JXitem, "pic_"..j)
            local soulpinzhi = ccui.Helper:seekWidgetByName(soulpic, "pinzhi")
            soulpic:setVisible(false)
            if table.getn(soullist) >= j then
                soulpic:setVisible(true)
                
                local dataitem=soulConfig[tostring(soullist[j])]
                if dataitem == nil then
                    soulpic:setVisible(false)
                else
                    soulpic:loadTexture(dataitem["icon"]..".png")
                    soulpinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                end
            end
        end
    end
end

--refresh all ui here
function JueXingUI:refreshUI()
end

return JueXingUI
