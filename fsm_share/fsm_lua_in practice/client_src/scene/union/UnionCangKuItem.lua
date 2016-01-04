--UnionCangKuItem.lua, created by php script. chris.li--
require "Cocos2d"

local UnionCangKuItem = class("UnionCangKuItem", function()
	return cc.Layer:create()
end)

function UnionCangKuItem:create()
	local layer = UnionCangKuItem.new()
	layer:initM()
	return layer
end

function UnionCangKuItem:ctor()
end

function UnionCangKuItem:onEnter()
end

function UnionCangKuItem:onExit()
end

function UnionCangKuItem:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("UnionCangKuItem.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_CangKu: ImageView
	self.m_CangKu = ccui.Helper:seekWidgetByName(self.rootNode, "m_CangKu")

	--m_touxiangK: ImageView
	self.m_touxiangK = ccui.Helper:seekWidgetByName(self.rootNode, "m_touxiangK")

	--m_ItemName: Text
	self.m_ItemName = ccui.Helper:seekWidgetByName(self.rootNode, "m_ItemName")

	--m_PaimingNum: Text
	self.m_PaimingNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_PaimingNum")

	--m_DetailBtn: Button
	self.m_DetailBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_DetailBtn")
	self.m_DetailBtn:addTouchEventListener(touchEventHandler)

	--m_ItemNumber: Text
	self.m_ItemNumber = ccui.Helper:seekWidgetByName(self.rootNode, "m_ItemNumber")

	--m_NoBtn: Button
	self.m_NoBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_NoBtn")
	self.m_NoBtn:addTouchEventListener(touchEventHandler)

	--m_YesBtn: Button
	self.m_YesBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_YesBtn")
	self.m_YesBtn:addTouchEventListener(touchEventHandler)

	self:initUI()
end

function UnionCangKuItem:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_DetailBtn and event == cc.EventCode.ENDED then
		cclog("[UnionCangKuItem]--m_DetailBtn touched--")

	elseif sender == self.m_NoBtn and event == cc.EventCode.ENDED then
		cclog("[UnionCangKuItem]--m_NoBtn touched--")

	elseif sender == self.m_YesBtn and event == cc.EventCode.ENDED then
		cclog("[UnionCangKuItem]--m_YesBtn touched--")

	end
end

--init your ui here
function UnionCangKuItem:initUI()
end

--refresh all ui here
function UnionCangKuItem:refreshUI()
end

return UnionCangKuItem
