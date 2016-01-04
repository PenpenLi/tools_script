--ShopUI.lua, created by php script. chris.li--
require "Cocos2d"

local PopDialog = require "common/PopDialog"

local shopConfig = require("config.define_shop")
local marketConfig = require("config.define_market")
local ShopUI = class("ShopUI", function()
	return cc.Layer:create()
end)

function ShopUI:create(res)
	local layer = ShopUI.new()
	layer:initM(res)
	return layer
end

function ShopUI:ctor()
end

function ShopUI:onEnter()
end

function ShopUI:onExit()
end

function ShopUI:initM(res)
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("ShopUI.csb")
	self:addChild(self.rootNode)
	
	self.lasttag = 0
	self.buyshopid = 0
	self.oneprice = 0

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_buyRootPanel: Layout
	self.m_buyRootPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_buyRootPanel")
	self.m_buyRootPanel:addTouchEventListener(touchEventHandler)
	self.m_buyRootPanel:setVisible(false)

	--m_layerPanel: Layout
	self.m_layerPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_layerPanel")
	self.m_layerPanel:addTouchEventListener(touchEventHandler)
    self.rootNode:setBackGroundImage("ditu.jpg")

	--m_returnBtn: Button
	self.m_returnBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_returnBtn")
	self.m_returnBtn:addTouchEventListener(touchEventHandler)

	--m_shopname: Text
	self.m_shopname = ccui.Helper:seekWidgetByName(self.rootNode, "m_shopname")

	--m_buyBtn: Button
	self.m_buyBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_buyBtn")
	self.m_buyBtn:addTouchEventListener(touchEventHandler)

	--m_cancleBtn: Button
	self.m_cancleBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_cancleBtn")
	self.m_cancleBtn:addTouchEventListener(touchEventHandler)

	--m_subBtn: Button
	self.m_subBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_subBtn")
	self.m_subBtn:addTouchEventListener(touchEventHandler)

	--m_addBtn: Button
	self.m_addBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_addBtn")
	self.m_addBtn:addTouchEventListener(touchEventHandler)

	--m_add10Btn: Button
	self.m_add10Btn = ccui.Helper:seekWidgetByName(self.rootNode, "m_add10Btn")
	self.m_add10Btn:addTouchEventListener(touchEventHandler)

	--m_numlabel: Text
	self.m_numlabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_numlabel")

	--m_totalPriLabel: Text
	self.m_totalPriLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_totalPriLabel")

	--m_pic: ImageView
	self.m_pic = ccui.Helper:seekWidgetByName(self.rootNode, "m_pic")

	--m_ScrollView: ScrollView
    self.m_ScrollView = ccui.Helper:seekWidgetByName(self.rootNode, "m_ScrollView")
	self.m_ScrollView:addTouchEventListener(touchEventHandler)

	--m_payBtn: Button
	self.m_payBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_payBtn")
	self.m_payBtn:addTouchEventListener(touchEventHandler)

	--m_pinzhi: ImageView
	self.m_pinzhi = ccui.Helper:seekWidgetByName(self.rootNode, "m_pinzhi")

	--m_TextArea: Text
	self.m_TextArea = ccui.Helper:seekWidgetByName(self.rootNode, "m_TextArea")
	
    self.m_moneytype = ccui.Helper:seekWidgetByName(self.rootNode, "money_type")

	self:initUI(res)
end

function ShopUI:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_buyRootPanel and event == cc.EventCode.ENDED then
		cclog("[ShopUI]--m_buyRootPanel touched--")

	elseif sender == self.m_layerPanel and event == cc.EventCode.ENDED then
		cclog("[ShopUI]--m_layerPanel touched--")

	elseif sender == self.m_returnBtn and event == cc.EventCode.ENDED then
		cclog("[ShopUI]--m_returnBtn touched--")
		self.m_returnBtn:setEnabled(false)
        cc.Director:getInstance():popScene()

	elseif sender == self.m_buyBtn and event == cc.EventCode.ENDED then
		cclog("[ShopUI]--m_buyBtn touched--")	
        Http:req("BuyItem",{shopId = self.buyshopid,num = self.m_numlabel:getString()},function(buyresult)
            if buyresult.data["result"] == 0 then
                PopDialog:getInstance():popText("购买成功")
                self.m_buyRootPanel:setVisible(false)
                self.m_buyRootPanel:setEnabled(false)
            elseif buyresult.data["result"] == 3 then
                PopDialog:getInstance():popText("钻石不足,购买失败")
            elseif buyresult.data["result"] == 4 then
                PopDialog:getInstance():popText("金币不足,购买失败")
            end
            
            
        end)

	elseif sender == self.m_cancleBtn and event == cc.EventCode.ENDED then
		cclog("[ShopUI]--m_cancleBtn touched--")
        self.m_buyRootPanel:setVisible(false)
        self.m_buyRootPanel:setEnabled(false)

	elseif sender == self.m_subBtn and event == cc.EventCode.ENDED then
		cclog("[ShopUI]--m_subBtn touched--")
        if tonumber(self.m_numlabel:getString()) == 1 then
			return
		end
		
        local num = tostring(tonumber(self.m_numlabel:getString())-1)
        local money = tostring(self.oneprice*num)
        
		self.m_numlabel:setString(num)
		self.m_totalPriLabel:setString(money)

	elseif sender == self.m_addBtn and event == cc.EventCode.ENDED then
		cclog("[ShopUI]--m_addBtn touched--")
        if tonumber(self.m_numlabel:getString()) >= 100 then
            return
        end
        local num = tostring(tonumber(self.m_numlabel:getString())+1)
        self.m_numlabel:setString(num)
        local money = tostring(self.oneprice*num)
        self.m_totalPriLabel:setString(money)

	elseif sender == self.m_add10Btn and event == cc.EventCode.ENDED then
		cclog("[ShopUI]--m_add10Btn touched--")
        if (tonumber(self.m_numlabel:getString())+10) >= 100 then
            local num = 100
            self.m_numlabel:setString(num)
            local money = tostring(self.oneprice*num)
            self.m_totalPriLabel:setString(money)
            return
        end
        local num = tostring(tonumber(self.m_numlabel:getString())+10)
        self.m_numlabel:setString(num)
        local money = tostring(self.oneprice*num)
        self.m_totalPriLabel:setString(money)

	elseif sender == self.m_ScrollView and event == cc.EventCode.ENDED then
		cclog("[ShopUI]--m_ScrollView touched--")

	elseif sender == self.m_payBtn and event == cc.EventCode.ENDED then
		cclog("[ShopUI]--m_payBtn touched--")
        local scene = require("scene.shop.RechargeScene"):create()
        cc.Director:getInstance():pushScene(scene)

	end
end

--init your ui here
function ShopUI:initUI(res)
    --Http:req("GetShopList",nil,function(res)
        local shoplist = res.data["shop"]
        local shopsize = table.getn(shoplist)
        self.m_ScrollView:removeAllChildren()
        self.m_ScrollView:setBounceEnabled(true)
        
        local ScrovSize = self.m_ScrollView:getInnerContainerSize()
        
        local ScrovH = ScrovSize.height
        if shopsize>4 then
            if shopsize%2 == 0 then
                ScrovH = (156+1)*(shopsize/2)
            else
                ScrovH = (156+1)*(math.floor(shopsize/2)+1)
            end
            
        end
        
        ScrovSize.height = ScrovH
        self.m_ScrollView:setInnerContainerSize(ScrovSize)
       
        
        for i=1,shopsize do
            local marketId = tostring(shoplist[i]["shopId"])
            local marketItem = marketConfig[marketId]
            local shopId= tostring(marketItem["shop_id"])
            
            local shopitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("ShopUI_One.csb")
            shopitem:setPosition(2+(i-1)%2*293,ScrovSize.height-157*(math.floor((i-1)/2)+1))
            shopitem:setName("shopitem_"..shopId)
           
            self.m_ScrollView:addChild(shopitem)
            local m_pinzhi = ccui.Helper:seekWidgetByName(shopitem, "m_pinzhi")
            m_pinzhi:setTag(shopId)
            local m_buybtn = ccui.Helper:seekWidgetByName(shopitem, "m_buybtn")
            
            local function ShopMenuCallBack(sender,event)
                if sender == m_buybtn and event == cc.EventCode.ENDED then
                    
                    self.buyshopid = marketId
                    self.m_buyRootPanel:setVisible(true)
                    self.m_buyRootPanel:setEnabled(true)
                    
                    
                     
                     if tonumber(marketItem["money_type"])== 0 then
                     	 self.m_moneytype:loadTexture("jinbi.png")
                     else
                         self.m_moneytype:loadTexture("baoshi.png")
                     end
                     
                     local panel_name = ccui.Helper:seekWidgetByName(self.m_buyRootPanel, "panel_name")
                     
          --[[           if ccui.Helper:seekWidgetByName(self.m_buyRootPanel, "shopname") == nil then
                            local shopname = createLabelTextAddOutline(dataitem["name"],"微软雅黑",22,getColorByQuality(tonumber(dataitem["quality"])),
                                cc.c3b(255,255,255),1)
                            shopname:setPosition(65,20)
                            shopname:setName("shopname")
                            panel_name:addChild(shopname,10)
                     else
                            panel_name:removeChild(ccui.Helper:seekWidgetByName(self.m_buyRootPanel, "shopname"))
                            local shopname = createLabelTextAddOutline(dataitem["name"],"微软雅黑",22,getColorByQuality(tonumber(dataitem["quality"])),
                                cc.c3b(255,255,255),1)
                            shopname:setPosition(65,20)
                            shopname:setName("shopname")
                            panel_name:addChild(shopname,10)
                     end]]--
                    local dataitem=shopConfig[shopId]
                    self.m_shopname:setString(dataitem["name"])
                    self.m_pic:loadTexture(dataitem["icon"]..".png")
                    self.m_pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                    self.m_numlabel:setString("1")
                    self.m_buyBtn:setTag(shopId)
                     
                    if tonumber(marketItem["discount"])== 0 then
                        self.m_totalPriLabel:setString(marketItem["price"])
                        self.oneprice = tonumber(marketItem["price"])
                    else
                        self.m_totalPriLabel:setString(marketItem["discount"])
                        self.oneprice = tonumber(marketItem["discount"])
                    end
                     
                   
                elseif sender == m_pinzhi and event == cc.EventCode.ENDED then
                    local tag = sender:getTag()
                    if self.lasttag ~= 0 then
                        local lastitem = self.m_ScrollView:getChildByName("shopitem_"..self.lasttag)
                        local lastimg = ccui.Helper:seekWidgetByName(lastitem, "m_select")
                        lastimg:setVisible(false)
                       
                    end
                    
                    local item = self.m_ScrollView:getChildByName("shopitem_"..tag)
                    
                    local item = ccui.Helper:seekWidgetByName(item, "m_select")	
                    
                    item:setVisible(true)
                    self.lasttag = tag
                  
                    if  shopConfig[shopId] ~= nil then
                     local dataitem=shopConfig[shopId]
                    self.m_TextArea:setString(dataitem["desc"])
                    end
                end
            end

            shopitem:setTag(shopId)
            
            local m_pic = ccui.Helper:seekWidgetByName(shopitem, "m_pic")
            
            m_pinzhi:addTouchEventListener(ShopMenuCallBack)
            local m_namelabel = ccui.Helper:seekWidgetByName(shopitem, "m_namelabel")
            local m_price = ccui.Helper:seekWidgetByName(shopitem, "m_price")
            local m_priceImg = ccui.Helper:seekWidgetByName(shopitem, "m_priceImg")
            
           
            
           
            m_buybtn:addTouchEventListener(ShopMenuCallBack)
            local m_select = ccui.Helper:seekWidgetByName(shopitem, "m_select")
            m_select:setVisible(false)      
            
            if  shopConfig[shopId] ~= nil then
                local dataitem=shopConfig[shopId]
                m_pic:loadTexture(dataitem["icon"]..".png")
                m_pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                m_namelabel:setString(dataitem["name"])
                
                if tonumber(marketItem["money_type"])== 0 then
                    m_priceImg:loadTexture("jinbi.png")
                else
                    m_priceImg:loadTexture("baoshi.png")
                end
                if tonumber(marketItem["discount"])== 0 then
                    m_price:setString(marketItem["price"])
                else
                    m_price:setString(marketItem["discount"])
                end
                
          --[[       local shopname = createLabelTextAddOutline(dataitem["name"],"微软雅黑",22,getColorByQuality(tonumber(dataitem["quality"])),
                     cc.c3b(255,255,255),1)
                shopname:setPosition(190,128)
                shopitem:addChild(shopname,10)]]--
            end
            
            
        end
    --end)
end

--refresh all ui here
function ShopUI:refreshUI()
end

return ShopUI
