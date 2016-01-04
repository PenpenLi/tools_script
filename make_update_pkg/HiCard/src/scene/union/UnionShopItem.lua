--UnionShopItem.lua, created by php script. chris.li--
require "Cocos2d"

local UnionShopItem = class("UnionShopItem", function()
	return cc.Layer:create()
end)

function UnionShopItem:create()
	local layer = UnionShopItem.new()
	layer:initM()
	return layer
end

function UnionShopItem:ctor()
end

function UnionShopItem:onEnter()
end

function UnionShopItem:onExit()
end

function UnionShopItem:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("UnionShopItem.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_ShouItem: ImageView
	self.m_ShouItem = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShouItem")

	--m_ItemPrice: Text
	self.m_ItemPrice = ccui.Helper:seekWidgetByName(self.rootNode, "m_ItemPrice")

	--m_BuyBtn: Button
	self.m_BuyBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_BuyBtn")
	self.m_BuyBtn:addTouchEventListener(touchEventHandler)

	--m_ItemName: Text
	self.m_ItemName = ccui.Helper:seekWidgetByName(self.rootNode, "m_ItemName")

	--m_OwnNum: Text
	self.m_OwnNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_OwnNum")

	--m_touxiang: ImageView
	self.m_touxiang = ccui.Helper:seekWidgetByName(self.rootNode, "m_touxiang")

	self:initUI()
end

function UnionShopItem:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_BuyBtn and event == cc.EventCode.ENDED then
		cclog("[UnionShopItem]--m_BuyBtn touched--")

	end
end

--init your ui here
function UnionShopItem:initUI()
end

--refresh all ui here
function UnionShopItem:refreshUI()
end

return UnionShopItem
