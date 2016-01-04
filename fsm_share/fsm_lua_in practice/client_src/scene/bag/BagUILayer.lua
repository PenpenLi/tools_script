--BagUILayer.lua, created by php script. chris.li--
require "Cocos2d"
local shopConfig = require("config.define_shop")
local PopDialog = require "common/PopDialog"
local BagUILayer = class("BagUILayer", function()
	return cc.Layer:create()
end)

function BagUILayer:create(param)
	local layer = BagUILayer.new()
	layer:initM(param)
	return layer
end

function BagUILayer:ctor()
end

function BagUILayer:onEnter()
end

function BagUILayer:onExit()
end

function BagUILayer:initM(param)
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("BagUILayer.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_BagRootLayout: Layout
	self.m_BagRootLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_BagRootLayout")
	self.m_BagRootLayout:addTouchEventListener(touchEventHandler)
	
	--m_propsBtn: Button
	self.m_propsBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_propsBtn")
	self.m_propsBtn:addTouchEventListener(touchEventHandler)

	--m_papperBtn: Button
	self.m_papperBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_papperBtn")
	self.m_papperBtn:addTouchEventListener(touchEventHandler)

	--m_BagListView: ListView
	self.m_BagListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_BagListView")
	self.m_BagListView:addTouchEventListener(touchEventHandler)
	
	--m_itemEmptyLabel: Text
	self.m_itemEmptyLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_itemEmptyLabel")
	
    --m_propsLabel: Text
    self.m_propsLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_propsLabel")
    
    --m_itemEmptyLabel: Text
    self.m_papperLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_papperLabel")
    
    --m_percentSlider: Slider
    self.m_percentSlider = ccui.Helper:seekWidgetByName(self.rootNode, "m_percentSlider")
    
	--m_closeBtn: Button
	self.m_closeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeBtn")
	self.m_closeBtn:addTouchEventListener(touchEventHandler)

	self:initUI(param)
end

function BagUILayer:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_BagRootLayout and event == cc.EventCode.ENDED then
		cclog("[BagUILayer]--m_BagRootLayout touched--")

	elseif sender == self.m_propsBtn and event == cc.EventCode.ENDED then
		cclog("[BagUILayer]--m_propsBtn touched--")
        self.m_propsBtn:setTouchEnabled(false)
        self.m_propsBtn:setHighlighted(true)
        self.m_papperBtn:setTouchEnabled(true)
        self.m_papperBtn:setHighlighted(false)
        self.m_propsLabel:setColor(cc.c3b(92,33,1))
        self.m_papperLabel:setColor(cc.c3b(255,255,255))
        self.curType=0
        self:updateListView()
        
	elseif sender == self.m_papperBtn and event == cc.EventCode.ENDED then
		cclog("[BagUILayer]--m_papperBtn touched--")
        self.m_propsBtn:setTouchEnabled(true)
        self.m_papperBtn:setTouchEnabled(false)
        self.m_propsBtn:setHighlighted(false)
        self.m_papperBtn:setHighlighted(true)
        self.m_propsLabel:setColor(cc.c3b(255,255,255))
        self.m_papperLabel:setColor(cc.c3b(92,33,1))
        self.curType=1
        self:updateListView()
        
	elseif sender == self.m_BagListView and event == cc.EventCode.ENDED then
		cclog("[BagUILayer]--m_BagListView touched--")
        SetSlider(self.m_BagListView,self.m_percentSlider)
        
	elseif sender == self.m_closeBtn and event == cc.EventCode.ENDED then
		cclog("[BagUILayer]--m_closeBtn touched--")
        cc.Director:getInstance():popScene()
	end
end

--init your ui here
function BagUILayer:initUI(res)
    self.curType=0
    self.proplist = res.data["propList"]
    self.papperlist = res.data["papaerList"]                        
    self:updateListView()
    self.m_BagRootLayout:setBackGroundImage("ditu.jpg")
    self.m_propsBtn:setTouchEnabled(false)
    self.m_propsBtn:setHighlighted(true)
    self.m_propsLabel:setColor(cc.c3b(92,33,1))
    self.m_papperBtn:setTouchEnabled(true)      
end

--refresh all ui here
function BagUILayer:refreshUI()
    self:requestBagList()
end

function BagUILayer:requestBagList()
    Http: req("BagList",
        nil,
        function(res) 
            if res.err then cclog("err!!!") return end 
            self.proplist = res.data["propList"]
            self.papperlist = res.data["papaerList"]                        
            self:updateListView()
        end)
end

function BagUILayer:displayBox()
    local boxDisplayNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("BoxDisplayLayer.csb")
    self:addChild(boxDisplayNode,1)
       
    local m_boxRewardImage = ccui.Helper:seekWidgetByName(boxDisplayNode, "m_boxRewardImage")
    --m_BoxBgImage: ImageView
    local m_BoxBgImage = ccui.Helper:seekWidgetByName(boxDisplayNode, "m_BoxBgImage")

    --m_BoxcloseBtn: Button
    local m_BoxcloseBtn = ccui.Helper:seekWidgetByName(boxDisplayNode, "m_BoxcloseBtn")
    
    local m_boxRewardScrollView = ccui.Helper:seekWidgetByName(boxDisplayNode, "m_boxRewardScrollView")
    
    local function closeBoxDisplay(sender,event)
        if sender ==m_BoxcloseBtn and event == cc.EventCode.ENDED then
        boxDisplayNode:removeFromParent()
        end
    end
    m_BoxcloseBtn:addTouchEventListener(closeBoxDisplay)
    
    local scrollSize = m_boxRewardScrollView:getContentSize()
    if  #self.tableReward == 1 then
        local rewardId =  tostring(self.tableReward[1])
        local boxRewardImage = m_boxRewardImage:clone()
        m_boxRewardScrollView:addChild(boxRewardImage)
        boxRewardImage:setPosition(scrollSize.width/2,scrollSize.height/2)
        local m_pinzhiImage = boxRewardImage:getChildByName("Image_pinzhi")
        local m_numLabel = boxRewardImage:getChildByName("Label_num")
        local m_atsLabel = ccui.TextAtlas:create()
        m_atsLabel:setProperty("0123456789", "image/ui/formation/shuzi_lv.png", 17, 19, "0")
        m_atsLabel:setPosition(m_numLabel:getPosition())
        m_atsLabel:setAnchorPoint(cc.p(1.0,0.5))
        boxRewardImage:addChild(m_atsLabel)
        boxRewardImage:setVisible(true)
        if  shopConfig[rewardId] ~= nil then
            local dataitem=shopConfig[rewardId]
            m_pinzhiImage:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            boxRewardImage:loadTexture(dataitem["icon"]..".png")
            m_atsLabel:setString(1)
        end
    else
        for i=1,table.getn(self.tableReward) do
            local rewardId =  tostring(self.tableReward[i])
            local boxRewardImage = m_boxRewardImage:clone()
            m_boxRewardScrollView:addChild(boxRewardImage)
            boxRewardImage:setVisible(false)           
            boxRewardImage:runAction(cc.FadeOut:create(0))            
            boxRewardImage:setPosition(37+(i-1)%5*110,scrollSize.height+40-110*(math.floor((i-1)/5)+1))
            boxRewardImage:runAction(cc.Sequence:create(cc.DelayTime:create(0.2*i),cc.Show:create(),cc.FadeIn:create(0.2)))
            local m_pinzhiImage = boxRewardImage:getChildByName("Image_pinzhi")
            local m_numLabel = boxRewardImage:getChildByName("Label_num")
            local m_atsLabel = ccui.TextAtlas:create()
            m_atsLabel:setProperty("0123456789", "image/ui/formation/shuzi_lv.png", 17, 19, "0")
            m_atsLabel:setPosition(m_numLabel:getPosition())
            m_atsLabel:setAnchorPoint(cc.p(1.0,0.5))
            boxRewardImage:addChild(m_atsLabel)
            if  shopConfig[rewardId] ~= nil then
                local dataitem=shopConfig[rewardId]
                m_pinzhiImage:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                boxRewardImage:loadTexture(dataitem["icon"]..".png")
                m_atsLabel:setString(1)
            end
        end
    end
    
    
end

function BagUILayer:updateListView()
    if  self.m_BagListView:getChildrenCount() ~= 0 then 
        self.m_BagListView:removeAllItems()
    end
    local type = self.curType
    local baglist = self.proplist
    if  self.curType == 1 then baglist = self.papperlist end
    if  next(baglist) == nil then self.m_itemEmptyLabel:setVisible(true) return end
    self.m_itemEmptyLabel:setVisible(false)
    
    local copyitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("BagItemLayer.csb")
    local listviewsize = self.m_BagListView:getContentSize()
    listviewsize.height = copyitem:getContentSize().height*(#baglist)+6*(#baglist-1)
    self.m_BagListView:setInnerContainerSize(listviewsize) 
    
    for i=1, table.getn(baglist) do
        local bagtable = baglist[i]
        local bagitem = copyitem:clone()
        local m_qualityImage = ccui.Helper:seekWidgetByName(bagitem, "m_qualityImage")
        local m_picImage = ccui.Helper:seekWidgetByName(bagitem, "m_picImage")
        local m_BagItemnameLabel = ccui.Helper:seekWidgetByName(bagitem, "m_BagItemnameLabel")
        local m_BagItemnumLabel = ccui.Helper:seekWidgetByName(bagitem, "m_BagItemnumLabel")
        local m_BagItemDescLabel = ccui.Helper:seekWidgetByName(bagitem, "m_BagItemDescLabel")
        local m_BagItemUse1Btn = ccui.Helper:seekWidgetByName(bagitem, "m_BagItemUse1Btn")
        local m_BagItemUse2Btn = ccui.Helper:seekWidgetByName(bagitem, "m_BagItemUse2Btn")
        local m_nameBGImage = ccui.Helper:seekWidgetByName(bagitem, "m_nameBGImage")
        
        local function baguse(sender,event)
            if sender == m_BagItemUse2Btn and event == cc.EventCode.ENDED then
                Http: req("BagUse",
                    {bagId=bagtable["bagId"],num=1},
                    function(res) 
                        if res.err then cclog("err!!!") return end
                            if  self:checkResult(res.data["result"]) then
                            self.tableReward=res.data["shopId"]
                            self:displayBox()
                            self:refreshUI()
                            end
               end)
        	end
        end
        
        local function bagtenOpen(sender,event)
            if sender ==m_BagItemUse1Btn and event == cc.EventCode.ENDED then
                Http: req("BagUse",
                    {bagId=bagtable["bagId"],num=10},
                    function(res) 
                        if res.err then cclog("err!!!") return end
                            if  self:checkResult(res.data["result"]) then
                            self.tableReward=res.data["shopId"] 
                            self:displayBox()
                            self:refreshUI()
                            end
                    end)
            end
        end
        
        local function expuse(sender,event)
            if sender == m_BagItemUse2Btn and event == cc.EventCode.ENDED then self:displayBox() end
        end
        
        local function bagcompose(sender,event)
            if sender ==m_BagItemUse2Btn and event == cc.EventCode.ENDED then
                Http: req("BagCompose",
                    {bagId=bagtable["bagId"]},
                    function(res) 
                        if res.err then cclog("err!!!") return end
                            if  self:checkResult(res.data["result"]) then
                            self.tableReward={res.data["destId"]} 
                            self:displayBox()
                            self:refreshUI()
                            end
                    end)
            end
        end
        
        m_BagItemnumLabel:setString("数量："..bagtable["num"])
        m_BagItemnameLabel:setString(bagtable["shopId"])
        m_picImage:loadTexture("zhanque.png")
        if  type == 1 then m_BagItemUse2Btn:setTitleText("合成")
            if not bagtable["compose"] then
            	m_BagItemUse2Btn:setTouchEnabled(false)
            	m_BagItemUse2Btn:setTitleText("未集齐")
                Helper.graySprite(m_BagItemUse2Btn:getVirtualRenderer())
            end
            --m_BagItemUse2Btn:setVisible(true)
            m_BagItemUse2Btn:addTouchEventListener(bagcompose)
        else    
            m_BagItemUse2Btn:addTouchEventListener(baguse)
            m_BagItemUse1Btn:addTouchEventListener(bagtenOpen)
        end
        
        local shopId=tostring(bagtable["shopId"]) 
        if  shopConfig[shopId] ~= nil then
            local dataitem=shopConfig[shopId]
            m_qualityImage:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            m_picImage:loadTexture(dataitem["icon"]..".png")
            m_BagItemDescLabel:setString(dataitem["desc"])
            --[[
            m_BagItemDescLabel:setContentSize(250,(m_BagItemDescLabel:getStringLength()/15+1)*27)
            m_BagItemDescLabel:ignoreContentAdaptWithSize(false)
            ]]
            m_BagItemnameLabel:setString(dataitem["name"])
     --       m_BagItemnameLabel:setVisible(false)
            local nameLabel = createLabelTextAddOutline(dataitem["name"],"宋体",22,
                              getColorByQuality(tonumber(dataitem["quality"])),cc.c3b(255,255,255),1)
            --nameLabel:setAnchorPoint(cc.p(0,0.5))
            nameLabel:setPosition(m_BagItemnameLabel:getPosition())
            m_nameBGImage:addChild(nameLabel)
            if  type == 0 then
                if  tostring(dataitem["can_use"]) == "0" then m_BagItemUse2Btn:setVisible(false) m_BagItemUse2Btn:setTouchEnabled(false) end
                if  tostring(dataitem["use_type"]) == "1" then
                    m_BagItemUse1Btn:setVisible(true) 
                    m_BagItemUse1Btn:setTitleText("十连开")
                elseif  tostring(dataitem["use_type"]) == "2" then
                    --[[
                    m_BagItemUse2Btn:setVisible(true)
                    m_BagItemUse2Btn:setTouchEnabled(true)
                    m_BagItemUse2Btn:addTouchEventListener(expuse)
                    ]]
                    m_BagItemUse2Btn:setVisible(false)
                end
            end
        end
        
        local function bagItemTouchFunc()
            SetSlider(self.m_BagListView,self.m_percentSlider)        
        end
        bagitem:addTouchEventListener(bagItemTouchFunc)
        
        self.m_BagListView:pushBackCustomItem(bagitem)
    end
    self.m_BagListView:jumpToTop()
end
function BagUILayer:checkResult(_result)
    local bRet=false
    if _result== 0 then 
        bRet = true
    elseif  _result == 1 then
        cclog("BAG_CANNOT_EQUIP")
        PopDialog:getInstance():popText("无法装备",cc.c3b(0,255,0))
    elseif  _result == 2 then
        cclog("BAG_NOT_EXSIT")
        PopDialog:getInstance():popText("道具不存在",cc.c3b(0,255,0))
    elseif  _result == 3 then
        cclog("BAG_GOLD_NOT_ENOUGH")
        PopDialog:getInstance():popText("钻石不足",cc.c3b(0,255,0))
    elseif  _result == 4 then
        cclog("BAG_KEY_NOT_ENOUGH")
        PopDialog:getInstance():popText("钥匙数量不足",cc.c3b(0,255,0))
    elseif  _result == 5 then
        cclog("BOX_NOT_ENOUGH")
        PopDialog:getInstance():popText("宝箱数量不足",cc.c3b(0,255,0))
    elseif  _result == 6 then
        cclog("BAG_PAPER_NOT_ENOUGH")
        PopDialog:getInstance():popText("图纸数量不足",cc.c3b(0,255,0))
    elseif  _result == 7 then
        cclog("BAG_BAG_NOT_ENOUGH")
        PopDialog:getInstance():popText("背包空间不足",cc.c3b(0,255,0))
    elseif  _result == 8 then
        cclog("BAG_EXP_DRUG_NOT_ENOUGH")
        PopDialog:getInstance():popText("经验药剂不足",cc.c3b(0,255,0))
    elseif  _result == 9 then
        cclog("BAG_EXP_MAX_LEVEL")
        PopDialog:getInstance():popText("英雄等级不能超过主角等级",cc.c3b(0,255,0)) 
    end
    return bRet
end

return BagUILayer
