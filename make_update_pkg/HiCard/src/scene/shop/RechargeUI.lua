--RechargeUI.lua, created by php script. chris.li--
require "Cocos2d"
local shopConfig = require("config.define_shop")

local Roler = require("data.Roler")

local RechargeUI = class("RechargeUI", function()
	return cc.Layer:create()
end)

function RechargeUI:create()
	local layer = RechargeUI.new()
	layer:initM()
	return layer
end

function RechargeUI:ctor()
end

function RechargeUI:onEnter()
end

function RechargeUI:onExit()
end

function RechargeUI:pageviewCallBack(sender,event)
    if event == ccui.PageViewEventType.turning then
        local  index = sender:getCurPageIndex()
        print(index)
        local viplevel = index+1;
        
        self.m_levelnow:setString(viplevel)
        self.m_level_left:setString(viplevel-1)
        self.m_level_right:setString(viplevel+1)
        
        if viplevel == 1 then
        	self.m_left:setVisible(false)
            self.m_level_left:setVisible(false)
            self.m_vipleft:setVisible(false)
            self.m_right:setVisible(true)
            self.m_level_right:setVisible(true)
            self.m_vipright:setVisible(true)
        elseif viplevel == 15 then
            self.m_left:setVisible(true)
            self.m_level_left:setVisible(true)
            self.m_vipleft:setVisible(true)
            self.m_right:setVisible(false)
            self.m_level_right:setVisible(false)
            self.m_vipright:setVisible(false)
        else
            self.m_left:setVisible(true)
            self.m_level_left:setVisible(true)
            self.m_vipleft:setVisible(true)
            self.m_right:setVisible(true)
            self.m_level_right:setVisible(true)
            self.m_vipright:setVisible(true)
        end
        
    end
	
end

function RechargeUI:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("RechargeUI.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end
	
    local function PagetouchEventHandler(sender, event)
        self:pageviewCallBack(sender, event)
    end

	--m_Panel: Layout
	self.m_Panel = ccui.Helper:seekWidgetByName(self.rootNode, "m_Panel")
	self.m_Panel:addTouchEventListener(touchEventHandler)
    self.rootNode:setBackGroundImage("ditu.jpg")

	--m_returnBtn: Button
	self.m_returnBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_returnBtn")
	self.m_returnBtn:addTouchEventListener(touchEventHandler)

	--m_goVipbtn: Button
	self.m_goVipbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_goVipbtn")
	self.m_goVipbtn:addTouchEventListener(touchEventHandler)

	--m_viplevel: Text
	self.m_viplevel = ccui.Helper:seekWidgetByName(self.rootNode, "m_viplevel")

	--m_desc: Text
	self.m_desc = ccui.Helper:seekWidgetByName(self.rootNode, "m_desc")

	--m_ScrollView: ScrollView
	self.m_ScrollView = ccui.Helper:seekWidgetByName(self.rootNode, "m_ScrollView")
	self.m_ScrollView:addTouchEventListener(touchEventHandler)

	--m_vipPanel: Layout
	self.m_vipPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_vipPanel")
	self.m_vipPanel:addTouchEventListener(touchEventHandler)
	self.m_vipPanel:setVisible(false)

	--m_jindutiao: ImageView
	self.m_jindutiao = ccui.Helper:seekWidgetByName(self.rootNode, "m_jindutiao")

	--m_jingyanzhi: Text
	self.m_jingyanzhi = ccui.Helper:seekWidgetByName(self.rootNode, "m_jingyanzhi")

	--m_vipright: ImageView
	self.m_vipright = ccui.Helper:seekWidgetByName(self.rootNode, "m_vipright")

	--m_level_right: Text
	self.m_level_right = ccui.Helper:seekWidgetByName(self.rootNode, "m_level_right")
    self.m_level_right:setString(Roler:get("vipLevel")+1)

	--m_left: ImageView
	self.m_left = ccui.Helper:seekWidgetByName(self.rootNode, "m_left")

	--m_right: ImageView
	self.m_right = ccui.Helper:seekWidgetByName(self.rootNode, "m_right")

    --m_PageView: PageView
    self.m_PageView = ccui.Helper:seekWidgetByName(self.rootNode, "m_PageView")
    self.m_PageView:addEventListener(PagetouchEventHandler)

	--m_backrecharge: Button
	self.m_backrecharge = ccui.Helper:seekWidgetByName(self.rootNode, "m_backrecharge")
	self.m_backrecharge:addTouchEventListener(touchEventHandler)

	--m_vipleft: ImageView
	self.m_vipleft = ccui.Helper:seekWidgetByName(self.rootNode, "m_vipleft")

	--m_level_left: Text
	self.m_level_left = ccui.Helper:seekWidgetByName(self.rootNode, "m_level_left")
    self.m_level_left:setString(Roler:get("vipLevel")-1)

	--m_levelnow: Text
	self.m_levelnow = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelnow")
    self.m_levelnow:setString(Roler:get("vipLevel"))
	  
	self.m_PageView:removeAllChildren()
	for i=1,15  do
		local layout = ccui.Layout:create()
        layout:setContentSize(self.m_PageView:getContentSize())
	
		local img = ccui.ImageView:create()
		img:loadTexture("vip-"..i..".png")
		img:setAnchorPoint(0,0)
 --       img:setPosition(cc.p(img:getCustomSize().width/2,img:getCustomSize().height/2))
        
        layout:addChild(img)

        self.m_PageView:addPage(layout)
	end

	self:initUI()
end

function RechargeUI:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_Panel and event == cc.EventCode.ENDED then
		cclog("[RechargeUI]--m_Panel touched--")

	elseif sender == self.m_returnBtn and event == cc.EventCode.ENDED then
		cclog("[RechargeUI]--m_returnBtn touched--")
        cc.Director:getInstance():popScene()
        
	elseif sender == self.m_goVipbtn and event == cc.EventCode.ENDED then
		cclog("[RechargeUI]--m_goVipbtn touched--")
        self.m_vipPanel:setVisible(true)
        self.m_vipPanel:setEnabled(true)
        self.m_ScrollView:setVisible(false)
        self.m_ScrollView:setEnabled(false)
        
        if Roler:get("vipLevel") <= 1 then
            self.m_levelnow:setString("1")
            self.m_level_right:setString("2")
            self.m_PageView:scrollToPage(0)
            self.m_left:setVisible(false)
            self.m_level_left:setVisible(false)
            self.m_vipleft:setVisible(false)
        elseif Roler:get("vipLevel") == 15 then
            self.m_right:setVisible(false)
            self.m_level_right:setVisible(false)
            self.m_vipright:setVisible(false)
            self.m_PageView:scrollToPage(Roler:get("vipLevel")-1)
        else
            self.m_PageView:scrollToPage(Roler:get("vipLevel")-1)
        end

	elseif sender == self.m_ScrollView and event == cc.EventCode.ENDED then
		cclog("[RechargeUI]--m_ScrollView touched--")

	elseif sender == self.m_vipPanel and event == cc.EventCode.ENDED then
		cclog("[RechargeUI]--m_vipPanel touched--")

	elseif sender == self.m_backrecharge and event == cc.EventCode.ENDED then
		cclog("[RechargeUI]--m_backrecharge touched--")
        self.m_vipPanel:setVisible(false)
        self.m_vipPanel:setEnabled(false)
        self.m_ScrollView:setVisible(true)
        self.m_ScrollView:setEnabled(true)

	end
end

--init your ui here
function RechargeUI:initUI()
    Http:req("VipShopList",nil,function(res)
        local nowjingyan = res.data["totalGold"]-res.data["nextGold"]
        
        self.m_viplevel:setString(res.data["vipLevel"])
        self.m_jingyanzhi:setString(nowjingyan.."/"..res.data["totalGold"])
        
        local nextlevel = tonumber(res.data["vipLevel"])+1
        if res.data["totalGold"] ~= 1 then
            nextlevel = nextlevel+1
            self.m_desc:setString("再充值"..res.data["nextGold"].."钻，就可以到VIP"..nextlevel)
        else
            self.m_desc:setString("您获得了最高的VIP荣誉")
            self.m_jingyanzhi:setString("MAX")
        end

        self.m_jindutiao:setScaleX(nowjingyan/res.data["totalGold"])
    
        local RClist = res.data["shop"]
        local RCsize = table.getn(RClist)
        self.m_ScrollView:removeAllChildren()
        self.m_ScrollView:setBounceEnabled(true)

        local ScrovSize = self.m_ScrollView:getInnerContainerSize()

        local ScrovH = ScrovSize.height
        if RCsize>4 then
            if RCsize%2 == 0 then
                ScrovH = (132+8)*(RCsize/2)
            else
                ScrovH = (132+8)*(math.floor(RCsize/2)+1)
            end

        end

        ScrovSize.height = ScrovH
        self.m_ScrollView:setInnerContainerSize(ScrovSize)

        for i=1,RCsize do
            local shopId=tostring(RClist[i]["shopId"])
            
            local shopitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("RechargeUI_One.csb")
            shopitem:setPosition(2+(i-1)%2*412,ScrovSize.height-140*(math.floor((i-1)/2)+1))
            self.m_ScrollView:addChild(shopitem)
            
            local function ShopMenuCallBack(sender,event)
                if sender == shopitem and event == cc.EventCode.ENDED then
                    Http:req("VipRecharge",{shopId = sender:getTag()},function(res)
                        if res.data["result"] == 0 then
                            self:initUI()
                            if Roler:get("firstRecharge") == 0 then
                                Roler:set("firstRecharge",1)
                            end
                        end
                    end)
                end
            end
            
            shopitem:addTouchEventListener(ShopMenuCallBack)
            shopitem:setTag(shopId)
            
            local m_pic = ccui.Helper:seekWidgetByName(shopitem, "m_pic")
            local m_pinzhi = ccui.Helper:seekWidgetByName(shopitem, "m_pinzhi")
            local m_namelabel = ccui.Helper:seekWidgetByName(shopitem, "m_namelabel")
            local m_price = ccui.Helper:seekWidgetByName(shopitem, "m_price")
            local m_desc = ccui.Helper:seekWidgetByName(shopitem, "m_desc")
            
            if  shopConfig[shopId] ~= nil then
                local dataitem=shopConfig[shopId]
                m_pic:loadTexture(dataitem["icon"]..".png")
                m_pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                m_namelabel:setString(dataitem["name"])
                m_price:setString(dataitem["price"])
                m_desc:setString(dataitem["desc"])

            end
        end
    end)
end

--refresh all ui here
function RechargeUI:refreshUI()
end

return RechargeUI
