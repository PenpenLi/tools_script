--ShowUnionMemberInfo.lua, created by php script. chris.li--
require "Cocos2d"

local ShowUnionMemberInfo = class("ShowUnionMemberInfo", function()
	return cc.Layer:create()
end)

function ShowUnionMemberInfo:create()
	local layer = ShowUnionMemberInfo.new()
	layer:initM()
	return layer
end

function ShowUnionMemberInfo:ctor()
end

function ShowUnionMemberInfo:onEnter()
end

function ShowUnionMemberInfo:onExit()
end

function ShowUnionMemberInfo:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("ShowUnionMemberInfo.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_UnionMemberInfo: ImageView
	self.m_UnionMemberInfo = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionMemberInfo")

	--m_touxiangkuang: ImageView
	self.m_touxiangkuang = ccui.Helper:seekWidgetByName(self.rootNode, "m_touxiangkuang")

	--m_MemberId: Text
	self.m_MemberId = ccui.Helper:seekWidgetByName(self.rootNode, "m_MemberId")

	--m_ZongGongXian: Text
	self.m_ZongGongXian = ccui.Helper:seekWidgetByName(self.rootNode, "m_ZongGongXian")

	--m_GongXianNumber: Text
	self.m_GongXianNumber = ccui.Helper:seekWidgetByName(self.rootNode, "m_GongXianNumber")

	--m_MemberLV: Text
	self.m_MemberLV = ccui.Helper:seekWidgetByName(self.rootNode, "m_MemberLV")

	--m_UnionMemberPosition: Text
	self.m_UnionMemberPosition = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionMemberPosition")

	--m_OperateMemberButton: Button
	self.m_OperateMemberButton = ccui.Helper:seekWidgetByName(self.rootNode, "m_OperateMemberButton")
	self.m_OperateMemberButton:addTouchEventListener(touchEventHandler)

	--m_LastTimeLogIn: Text
	self.m_LastTimeLogIn = ccui.Helper:seekWidgetByName(self.rootNode, "m_LastTimeLogIn")

	--m_ShouLoginTime: Text
	self.m_ShouLoginTime = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShouLoginTime")

	self:initUI()
end

function ShowUnionMemberInfo:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_OperateMemberButton and event == cc.EventCode.ENDED then
		cclog("[ShowUnionMemberInfo]--m_OperateMemberButton touched--")

	end
end

--init your ui here
function ShowUnionMemberInfo:initUI()
end

--refresh all ui here
function ShowUnionMemberInfo:refreshUI()
end

return ShowUnionMemberInfo
