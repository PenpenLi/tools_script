--UnionUi.lua, created by php script. chris.li--
require "Cocos2d"
local PopDialog = require "common/PopDialog"

local dinnerConfig = require("config.define_guild_dinner")
local UnionShopConfig_1 = require("config.define_guild_shop")
local shopConfig = require("config.define_shop")
local shopRefreshConfig = require("config.define_guildshop_refresh")
local roleData = require("data.Roler")
local SmallStageConfig = require("config.define_guild_stage") 
local BigStageConfig = require("config.define_guild_big_stage")
local UnionBagItemConfig = require("config.define_guild_bag_item")

local UnionUi = class("UnionUi", function()
	return cc.Layer:create()
end)

function UnionUi:create()
	local layer = UnionUi.new()
	layer:initM()
	return layer
end

function UnionUi:ctor()
end

function UnionUi:onEnter()
    self:updataTipPic()
end

function UnionUi:onExit()
end

function UnionUi:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("UnionUi.csb")
	self:addChild(self.rootNode)
	
	--全局变量
	self.IndexId = 5
	
	self.IsApply = 0
	
	self.IndexBossID = 0

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_Panel_Union: Layout
	self.m_Panel_Union = ccui.Helper:seekWidgetByName(self.rootNode, "m_Panel_Union")
	self.m_Panel_Union:addTouchEventListener(touchEventHandler)
    self.m_Panel_Union:setBackGroundImage("ditu.jpg")

	--m_UnionBg: ImageView
	self.m_UnionBg = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionBg")

	--m_ScrollUnionList: ScrollView
	self.m_ScrollUnionList = ccui.Helper:seekWidgetByName(self.rootNode, "m_ScrollUnionList")
	self.m_ScrollUnionList:addTouchEventListener(touchEventHandler)
	
    --m_MyUnionInfo: ImageView
    self.m_MyUnionInfo = ccui.Helper:seekWidgetByName(self.rootNode, "Image_91")
    self.m_MyUnionInfo:addTouchEventListener(touchEventHandler)
    
    --m_MyUnionActivity: ImageView
    self.m_MyUnionActivity = ccui.Helper:seekWidgetByName(self.rootNode, "Image_106")
    self.m_MyUnionActivity:addTouchEventListener(touchEventHandler)
    
    --m_MyItemProgress:ImageView
    self.m_MyItemProgress = ccui.Helper:seekWidgetByName(self.rootNode, "ProgressBar_100")
    self.m_MyItemProgress:addTouchEventListener(touchEventHandler)
    
	--m_FoundUnionFace: ImageView
	self.m_FoundUnionFace = ccui.Helper:seekWidgetByName(self.rootNode, "m_FoundUnionFace")

	--m_biaotou1: ImageView
	self.m_biaotou1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_biaotou1")

	--m_UnionCloseButton: Button
	self.m_UnionCloseButton = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionCloseButton")
	self.m_UnionCloseButton:addTouchEventListener(touchEventHandler)

	--m_UnionNoticeTitle: Text
	self.m_UnionNoticeTitle = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionNoticeTitle")

	--m_UnionNoticeDetails: Text
	self.m_UnionNoticeDetails = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionNoticeDetails")

	--m_JoinUnionBtn: Button
	self.m_JoinUnionBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_JoinUnionBtn")
	self.m_JoinUnionBtn:addTouchEventListener(touchEventHandler)

	--m_FoundUnionBtn: Button
	self.m_FoundUnionBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_FoundUnionBtn")
	self.m_FoundUnionBtn:addTouchEventListener(touchEventHandler)

	--m_CoverPanel: Layout
	self.m_CoverPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_CoverPanel")
	self.m_CoverPanel:addTouchEventListener(touchEventHandler)

	--m_titleUnion: Text
	self.m_titleUnion = ccui.Helper:seekWidgetByName(self.rootNode, "m_titleUnion")

	--m_TitleFoundCost: TextField
	self.m_TitleFoundCost = ccui.Helper:seekWidgetByName(self.rootNode, "m_TitleFoundCost")
	self.m_TitleFoundCost:addTouchEventListener(touchEventHandler)

	--m_FoundUnionButton: Button
	self.m_FoundUnionButton = ccui.Helper:seekWidgetByName(self.rootNode, "m_FoundUnionButton")
	self.m_FoundUnionButton:addTouchEventListener(touchEventHandler)

	--m_GongHui1: Text
	self.m_GongHui1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_GongHui1")

	--m_UnionId: Text
	self.m_UnionId = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionId")

	--m_UnionLevelTitle: Text
	self.m_UnionLevelTitle = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionLevelTitle")

	--m_UnionPeopleTitle: Text
	self.m_UnionPeopleTitle = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionPeopleTitle")

	--m_UnionMoneyTitle: Text
	self.m_UnionMoneyTitle = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionMoneyTitle")

	--m_UnionEXP: Text
	self.m_UnionEXP = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionEXP")

	--m_EXPprogresss: LoadingBar
	self.m_EXPprogresss = ccui.Helper:seekWidgetByName(self.rootNode, "m_EXPprogresss")

	--m_MyUnionCtrlButton: Button
	self.m_MyUnionCtrlButton = ccui.Helper:seekWidgetByName(self.rootNode, "m_MyUnionCtrlButton")
	self.m_MyUnionCtrlButton:addTouchEventListener(touchEventHandler)

	--m_MyUnionLV: Text
	self.m_MyUnionLV = ccui.Helper:seekWidgetByName(self.rootNode, "m_MyUnionLV")

	--m_MyUnionPeopleNum: Text
	self.m_MyUnionPeopleNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_MyUnionPeopleNum")

	--m_MyUnionMoney: Text
	self.m_MyUnionMoney = ccui.Helper:seekWidgetByName(self.rootNode, "m_MyUnionMoney")

	--m_DACAN: Button
	self.m_DACAN = ccui.Helper:seekWidgetByName(self.rootNode, "m_DACAN")
    self.m_DACAN:addTouchEventListener(touchEventHandler)
    
    --m_keleBtn: Button
    self.m_keleBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_keleBtn")
    self.m_keleBtn:addTouchEventListener(touchEventHandler)
    
    --m_hanbaoBtn: Button
    self.m_hanbaoBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_hanbaoBtn")
    self.m_hanbaoBtn:addTouchEventListener(touchEventHandler)
    
    --m_kaojiBtn: Button
    self.m_kaojiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_kaojiBtn")
    self.m_kaojiBtn:addTouchEventListener(touchEventHandler)

	--m_UnionShop: Button
	self.m_UnionShop = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionShop")
    self.m_UnionShop:addTouchEventListener(touchEventHandler)

	--m_UnionBoss: Button
	self.m_UnionBoss = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionBoss")
    self.m_UnionBoss:addTouchEventListener(touchEventHandler)
    
    --Show Boss
    self.m_ShowUnionBoss = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShowUnionBoss")
    
    --m_CloseBossBtn: Button
    self.m_CloseBossBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_CloseBossBtn")
    self.m_CloseBossBtn:addTouchEventListener(touchEventHandler)
    
    --m_ShowFood: ImageView
    self.m_ShowFood = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShowFood")
    
    --m_ShowUnionShop: ImageView
    self.m_ShowUnionShop = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShowUnionShop")

	--m_SetUpUnion: ImageView
	self.m_SetUpUnion = ccui.Helper:seekWidgetByName(self.rootNode, "m_SetUpUnion")

	--m_ApplyUnionList: ImageView
	self.m_ApplyUnionList = ccui.Helper:seekWidgetByName(self.rootNode, "m_ApplyUnionList")

	--m_LookUpUnionMember: ImageView
	self.m_LookUpUnionMember = ccui.Helper:seekWidgetByName(self.rootNode, "m_LookUpUnionMember")

	--m_CorrectUnionNotice: ImageView
	self.m_CorrectUnionNotice = ccui.Helper:seekWidgetByName(self.rootNode, "m_CorrectUnionNotice")

	--m_NormalMemberMenu: ImageView
	self.m_NormalMemberMenu = ccui.Helper:seekWidgetByName(self.rootNode, "m_NormalMemberMenu")

	--m_LeaderMenu: ImageView
	self.m_LeaderMenu = ccui.Helper:seekWidgetByName(self.rootNode, "m_LeaderMenu")

	--m_InputUnionName: TextField
	self.m_InputUnionName = ccui.Helper:seekWidgetByName(self.rootNode, "m_InputUnionName")
	self.m_InputUnionName:addTouchEventListener(touchEventHandler)

	--m_CostDiamond: Text
	self.m_CostDiamond = ccui.Helper:seekWidgetByName(self.rootNode, "m_CostDiamond")

	--m_Left1: Button
	self.m_Left1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Left1")
	self.m_Left1:addTouchEventListener(touchEventHandler)

	--m_Right1: Button
	self.m_Right1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Right1")
	self.m_Right1:addTouchEventListener(touchEventHandler)

	--m_Left2: Button
	self.m_Left2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Left2")
	self.m_Left2:addTouchEventListener(touchEventHandler)

	--m_Right2: Button
	self.m_Right2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Right2")
	self.m_Right2:addTouchEventListener(touchEventHandler)

	--m_CancleBtn: Button
	self.m_CancleBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_CancleBtn")
	self.m_CancleBtn:addTouchEventListener(touchEventHandler)

	--m_ConfirmButton: Button
	self.m_ConfirmButton = ccui.Helper:seekWidgetByName(self.rootNode, "m_ConfirmButton")
	self.m_ConfirmButton:addTouchEventListener(touchEventHandler)

	--m_ExitLookUpBtn: Button
	self.m_ExitLookUpBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_ExitLookUpBtn")
	self.m_ExitLookUpBtn:addTouchEventListener(touchEventHandler)

	--m_NoPlayerApply: Text
	self.m_NoPlayerApply = ccui.Helper:seekWidgetByName(self.rootNode, "m_NoPlayerApply")

	--m_PlayerApplyUnionInfo: ImageView
	self.m_PlayerApplyUnionInfo = ccui.Helper:seekWidgetByName(self.rootNode, "m_PlayerApplyUnionInfo")

	--m_ScrollUnionMember: ScrollView
	self.m_ScrollUnionMember = ccui.Helper:seekWidgetByName(self.rootNode, "m_ScrollUnionMember")
	self.m_ScrollUnionMember:addTouchEventListener(touchEventHandler)

	--m_CloseLookUpMemButton: Button
	self.m_CloseLookUpMemButton = ccui.Helper:seekWidgetByName(self.rootNode, "m_CloseLookUpMemButton")
	self.m_CloseLookUpMemButton:addTouchEventListener(touchEventHandler)

	--m_morenpaixu: Button
	self.m_morenpaixu = ccui.Helper:seekWidgetByName(self.rootNode, "m_morenpaixu")
	self.m_morenpaixu:addTouchEventListener(touchEventHandler)

	--m_ThreeDaysContribution: Button
	self.m_ThreeDaysContribution = ccui.Helper:seekWidgetByName(self.rootNode, "m_ThreeDaysContribution")

	--m_CloseCorrectUnionButton: Button
	self.m_CloseCorrectUnionButton = ccui.Helper:seekWidgetByName(self.rootNode, "m_CloseCorrectUnionButton")
	self.m_CloseCorrectUnionButton:addTouchEventListener(touchEventHandler)

	--m_CorrectNoticeTitle: Text
	self.m_CorrectNoticeTitle = ccui.Helper:seekWidgetByName(self.rootNode, "m_CorrectNoticeTitle")
	
    --m_CorrectNoticeTitle: Text
    self.m_UnionFoodNotice = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionFoodNotice")

	--m_CorrectNoticeBtn: Button
	self.m_CorrectNoticeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_CorrectNoticeBtn")
	self.m_CorrectNoticeBtn:addTouchEventListener(touchEventHandler)

	--m_NormalUnionMember: Button
	self.m_NormalUnionMember = ccui.Helper:seekWidgetByName(self.rootNode, "m_NormalUnionMember")
	self.m_NormalUnionMember:addTouchEventListener(touchEventHandler)

	--m_NormalExitUnion: Button
	self.m_NormalExitUnion = ccui.Helper:seekWidgetByName(self.rootNode, "m_NormalExitUnion")
	self.m_NormalExitUnion:addTouchEventListener(touchEventHandler)

	--m_CloseUnionMenu2: Button
	self.m_CloseUnionMenu2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_CloseUnionMenu2")
	self.m_CloseUnionMenu2:addTouchEventListener(touchEventHandler)

	--m_APPLY: Button
	self.m_APPLY = ccui.Helper:seekWidgetByName(self.rootNode, "m_APPLY")
	self.m_APPLY:addTouchEventListener(touchEventHandler)

	--m_MEMBER: Button
	self.m_MEMBER = ccui.Helper:seekWidgetByName(self.rootNode, "m_MEMBER")
	self.m_MEMBER:addTouchEventListener(touchEventHandler)

	--m_CORRECTNotice: Button
	self.m_CORRECTNotice = ccui.Helper:seekWidgetByName(self.rootNode, "m_CORRECTNotice")
	self.m_CORRECTNotice:addTouchEventListener(touchEventHandler)

	--m_UnionArrange: Button
	self.m_UnionArrange = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionArrange")
	self.m_UnionArrange:addTouchEventListener(touchEventHandler)

	--m_DismissUnion: Button
	self.m_DismissUnion = ccui.Helper:seekWidgetByName(self.rootNode, "m_DismissUnion")
	self.m_DismissUnion:addTouchEventListener(touchEventHandler)
	
    --m_ExitShowFoodBtn: Button
    self.m_ExitShowFoodBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_ExitShowFoodBtn")
    self.m_ExitShowFoodBtn:addTouchEventListener(touchEventHandler)
    
    --m_CloseUnionShopBtn: Button
    self.m_CloseUnionShopBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_CloseUnionShopBtn")
    self.m_CloseUnionShopBtn:addTouchEventListener(touchEventHandler)
    
	--m_ExitUnion: Button
	self.m_ExitUnion = ccui.Helper:seekWidgetByName(self.rootNode, "m_ExitUnion")
	self.m_ExitUnion:addTouchEventListener(touchEventHandler)

	--m_CloseLeaderMenu: Button
	self.m_CloseLeaderMenu = ccui.Helper:seekWidgetByName(self.rootNode, "m_CloseLeaderMenu")
	self.m_CloseLeaderMenu:addTouchEventListener(touchEventHandler)
	
    --m_UnionShopFreshBtn: Button
    self.m_UnionShopFreshBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionShopFreshBtn")
    self.m_UnionShopFreshBtn:addTouchEventListener(touchEventHandler)
    
    --m_MemberGongXianNum: Text
    self.m_MemberGongXianNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_MemberGongXianNum")

	--m_SetUnionTitle: Text
	self.m_SetUnionTitle = ccui.Helper:seekWidgetByName(self.rootNode, "m_SetUnionTitle")

	--m_AnyBodyCanJoin: Text
	self.m_SelectIndex = ccui.Helper:seekWidgetByName(self.rootNode, "m_AnyBodyCanJoin")

	--m_JoinWithAdmit: Text
	self.m_JoinWithAdmit = ccui.Helper:seekWidgetByName(self.rootNode, "m_JoinWithAdmit")

	--m_LV: Text
	self.m_LV = ccui.Helper:seekWidgetByName(self.rootNode, "m_LV31")

	--m_touxiangkuang: ImageView
	self.m_touxiangkuang = ccui.Helper:seekWidgetByName(self.rootNode, "m_touxiangkuang")

	--m_PlayerId: Text
	self.m_PlayerId = ccui.Helper:seekWidgetByName(self.rootNode, "m_PlayerId")

	--m_PlayerLastTimeLogIn: Text
	self.m_PlayerLastTimeLogIn = ccui.Helper:seekWidgetByName(self.rootNode, "m_PlayerLastTimeLogIn")

	--m_PlayerLv: Text
	self.m_PlayerLv = ccui.Helper:seekWidgetByName(self.rootNode, "m_PlayerLv")
	
    --m_MemberGongXianNum: Text
    self.m_MemberGongXianNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_MemberGongXianNum")

	--m_Allowed: ImageView
	self.m_Allowed = ccui.Helper:seekWidgetByName(self.rootNode, "m_Allowed")

	--m_Refused: ImageView
	self.m_Refused = ccui.Helper:seekWidgetByName(self.rootNode, "m_Refused")

	--m_inputNotice: TextField
	self.m_inputNotice = ccui.Helper:seekWidgetByName(self.rootNode, "m_inputNotice")
	self.m_inputNotice:addTouchEventListener(touchEventHandler)
	
    --m_ScrollApplyMemberList: ScrollView
    self.m_ScrollApplyMemberList = ccui.Helper:seekWidgetByName(self.rootNode, "ScrollView_102")
    self.m_ScrollApplyMemberList:addTouchEventListener(touchEventHandler)
    
    --m_GoodsScrollView: ScrollView
    self.m_GoodsScrollView = ccui.Helper:seekWidgetByName(self.rootNode, "m_GoodsScrollView")
    self.m_GoodsScrollView:addTouchEventListener(touchEventHandler)
    
    --m_ShowUnionCangKu: ImageView
    self.m_ShowUnionCangKu = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShowUnionCangKu")
    
    --m_CloseCangkuBtn: Button
    self.m_CloseCangkuBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_CloseCangkuBtn")
    self.m_CloseCangkuBtn:addTouchEventListener(touchEventHandler)
    
    --m_UnionCangku: Button
    self.m_UnionCangku = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnionCangku")
    self.m_UnionCangku:addTouchEventListener(touchEventHandler)
    
    --m_RuleBtn: Button
    self.m_RuleBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_RuleBtn")
    self.m_RuleBtn:addTouchEventListener(touchEventHandler)
    
    --m_CangkuListView: ScrollView
    self.m_CangkuListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_CangkuListView")
    self.m_CangkuListView:addTouchEventListener(touchEventHandler)
    
    --m_Sure2Union: ImageView 二次确认框 1创建公会
    self.m_Sure2Union = ccui.Helper:seekWidgetByName(self.rootNode, "m_Sure2Union")
    
    --m_cancel: Button
    self.m_cancel = ccui.Helper:seekWidgetByName(self.rootNode, "m_cancel")
    self.m_cancel:addTouchEventListener(touchEventHandler)
    
    --m_Confirm: Button
    self.m_Confirm = ccui.Helper:seekWidgetByName(self.rootNode, "m_Confirm")
    self.m_Confirm:addTouchEventListener(touchEventHandler)
    
    --m_Sure2Exit: ImageView 二次确认框 2 退出公会
    self.m_Sure2Exit = ccui.Helper:seekWidgetByName(self.rootNode, "m_Sure2Exit")

    --m_Cancel2: Button
    self.m_Cancel2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Cancel2")
    self.m_Cancel2:addTouchEventListener(touchEventHandler)

    --m_confirm2: Button
    self.m_confirm2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_confirm2")
    self.m_confirm2:addTouchEventListener(touchEventHandler)
 
    --m_Sure2Dismiss: ImageView 二次确认框 3 解散公会
    self.m_Sure2Dismiss = ccui.Helper:seekWidgetByName(self.rootNode, "m_Sure2Dismiss")

    --m_Cancel3: Button
    self.m_Cancel3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Cancel3")
    self.m_Cancel3:addTouchEventListener(touchEventHandler)

    --m_confirm3: Button
    self.m_Confirm3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Confirm3")
    self.m_Confirm3:addTouchEventListener(touchEventHandler)
    
    --m_Sure2Change: ImageView 二次确认框 4 兑换物品
    self.m_Sure2Change = ccui.Helper:seekWidgetByName(self.rootNode, "m_Sure2Change")

    --m_Cancel4: Button
    self.m_Cancel4 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Cancel4")

    --m_confirm4: Button
    self.m_confirm4 = ccui.Helper:seekWidgetByName(self.rootNode, "m_confirm4")
    --self.m_confirm4:addTouchEventListener(touchEventHandler)
    
    self.m_IName = ccui.Helper:seekWidgetByName(self.rootNode, "m_IName")
    self.m_PNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_PNum")
    
    --m_Sure2Fresh: ImageView 二次确认框 5 刷新物品
    self.m_Sure2Fresh = ccui.Helper:seekWidgetByName(self.rootNode, "m_Sure2Fresh")

    --m_Cancel5: Button
    self.m_Cancel5 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Cancel5")
    self.m_Cancel5:addTouchEventListener(touchEventHandler)

    --m_confirm5: Button
    self.m_confirm5 = ccui.Helper:seekWidgetByName(self.rootNode, "m_confirm5")
    self.m_confirm5:addTouchEventListener(touchEventHandler)
    
    self.m_PGongXian = ccui.Helper:seekWidgetByName(self.rootNode, "m_PGongXian")
    self.m_FreshNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_FreshNum")
    
    --m_Sure2Zhanglao: ImageView 二次确认框 6 提升长老
    self.m_Sure2Zhanglao = ccui.Helper:seekWidgetByName(self.rootNode, "m_Sure2Zhanglao")
    
    self.m_ID_6 = ccui.Helper:seekWidgetByName(self.rootNode, "m_ID_6")--被提升者名字
    
    --m_Cancel6: Button
    self.m_Cancel6 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Cancel6")
    self.m_Cancel6:addTouchEventListener(touchEventHandler)

    --m_confirm6: Button
    self.m_confirm6 = ccui.Helper:seekWidgetByName(self.rootNode, "m_confirm6")
    
    --m_Sure2Huizhang: ImageView 二次确认框 7 提升会长
    self.m_Sure2Huizhang = ccui.Helper:seekWidgetByName(self.rootNode, "m_Sure2Huizhang")

    self.m_ID_7 = ccui.Helper:seekWidgetByName(self.rootNode, "m_ID_7")--被提升者名字

    --m_Cancel7: Button
    self.m_Cancel7 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Cancel7")
    self.m_Cancel7:addTouchEventListener(touchEventHandler)

    --m_confirm7: Button
    self.m_confirm7 = ccui.Helper:seekWidgetByName(self.rootNode, "m_confirm7")
    
    --m_Sure2Jiezhi: ImageView 二次确认框 8 解职长老
    self.m_Sure2Jiezhi = ccui.Helper:seekWidgetByName(self.rootNode, "m_Sure2Jiezhi")

    self.m_ID_8 = ccui.Helper:seekWidgetByName(self.rootNode, "m_ID_8")--被解职者名字

    --m_Cancel8: Button
    self.m_Cancel8 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Cancel8")
    self.m_Cancel8:addTouchEventListener(touchEventHandler)

    --m_confirm8: Button
    self.m_confirm8 = ccui.Helper:seekWidgetByName(self.rootNode, "m_confirm8")

    self.m_PGongXian = ccui.Helper:seekWidgetByName(self.rootNode, "m_PGongXian")
    self.m_FreshNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_FreshNum")
    
    --m_Sure2OpenStage: ImageView 二次确认框 9 是否开启副本
    self.m_Sure2OpenStage = ccui.Helper:seekWidgetByName(self.rootNode, "m_Sure2OpenStage")
    self.m_Cancel9 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Cancel9")
    self.m_confirm9 = ccui.Helper:seekWidgetByName(self.rootNode, "m_confirm9")
    self.m_OpenMoney = ccui.Helper:seekWidgetByName(self.rootNode, "m_OpenMoney")
    
    
    --m_Leader2NormalBtn: ImageView 操作：会长对群众
    self.m_Leader2NormalBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_Leader2NormalBtn")
    
    --m_ExitLeader2Normal: Button
    self.m_ExitLeader2Normal = ccui.Helper:seekWidgetByName(self.rootNode, "m_ExitLeader2Normal")
    self.m_ExitLeader2Normal:addTouchEventListener(touchEventHandler)

    --m_Pofficial: Button
    self.m_Pofficial = ccui.Helper:seekWidgetByName(self.rootNode, "m_Pofficial")
    self.m_Pofficial:addTouchEventListener(touchEventHandler)
    
    --关闭BOSS二级界面
    self.m_CloseSmallBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_CloseSmallBtn")
    self.m_CloseSmallBtn:addTouchEventListener(touchEventHandler)
    
    --m_LetLeaveUnionBtn: Button 会长踢群众
    self.m_LetLeaveUnionBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_LetLeaveUnionBtn")
    
    --m_Leader2OfficialBtn: ImageView 操作：会长对官员
    self.m_Leader2OfficialBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_Leader2OfficialBtn")

    --m_ExitLeader2official: Button
    self.m_ExitLeader2official = ccui.Helper:seekWidgetByName(self.rootNode, "m_ExitLeader2official")
    self.m_ExitLeader2official:addTouchEventListener(touchEventHandler)

    --m_PromoteLeader: Button
    self.m_PromoteLeader = ccui.Helper:seekWidgetByName(self.rootNode, "m_PromoteLeader")
    self.m_PromoteLeader:addTouchEventListener(touchEventHandler)

    --m_DismissOfficial: Button
    self.m_DismissOfficial = ccui.Helper:seekWidgetByName(self.rootNode, "m_DismissOfficial")
    self.m_DismissOfficial:addTouchEventListener(touchEventHandler)
    
    --m_Official2Normal: ImageView 操作：官员对群众
    self.m_Official2Normal = ccui.Helper:seekWidgetByName(self.rootNode, "m_Official2Normal")

    --m_ExitOfficial2Normal: Button
    self.m_ExitOfficial2Normal = ccui.Helper:seekWidgetByName(self.rootNode, "m_ExitOfficial2Normal")
    self.m_ExitOfficial2Normal:addTouchEventListener(touchEventHandler)

    --m_LetLeaveUnion_2: Button 官员踢群众
    self.m_LetLeaveUnion_2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_LetLeaveUnion_2")
    
    self.m_AkeleBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_AkeleBtn")
    self.m_AhanbaoBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_AhanbaoBtn")
    self.m_AkaojiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_AkaojiBtn")
    
    self.m_kele = ccui.Helper:seekWidgetByName(self.rootNode, "m_kele")
    self.m_kele:addTouchEventListener(touchEventHandler)
    
    self.m_hanbao = ccui.Helper:seekWidgetByName(self.rootNode, "m_hanbao")
    self.m_hanbao:addTouchEventListener(touchEventHandler)
    
    self.m_kaoji = ccui.Helper:seekWidgetByName(self.rootNode, "m_kaoji")
    self.m_kaoji:addTouchEventListener(touchEventHandler)
    
    self.m_KeleWord = ccui.Helper:seekWidgetByName(self.rootNode, "m_KeleWord")
    self.m_hanbaoWord = ccui.Helper:seekWidgetByName(self.rootNode, "m_hanbaoWord")
    self.m_kaojiWord = ccui.Helper:seekWidgetByName(self.rootNode, "m_kaojiWord")
    
    --m_dituPicture: ImageView BOSS地图框架
    self.m_dituPicture = ccui.Helper:seekWidgetByName(self.rootNode, "m_dituPicture")
    self.m_BossScrollView = ccui.Helper:seekWidgetByName(self.rootNode, "m_BossScrollView")
    
    self.m_ShowSmallBoss = ccui.Helper:seekWidgetByName(self.rootNode, "m_ShowSmallBoss")
    self.m_GotoBoss = ccui.Helper:seekWidgetByName(self.rootNode, "m_GotoBoss")
    
    --m_prepare2Battle: ImageView 进入布阵前界面
    self.m_prepare2Battle = ccui.Helper:seekWidgetByName(self.rootNode, "m_prepare2Battle")
    
    self.m_Close2BattleBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_Close2BattleBtn")
    self.m_Close2BattleBtn:addTouchEventListener(touchEventHandler)
    
    self.m_StageName = ccui.Helper:seekWidgetByName(self.rootNode, "m_StageName")
    self.m_StageDesc = ccui.Helper:seekWidgetByName(self.rootNode, "m_StageDesc")
    self.m_TeammateInBattle = ccui.Helper:seekWidgetByName(self.rootNode, "m_TeammateInBattle")
    self.m_teammateName = ccui.Helper:seekWidgetByName(self.rootNode, "m_teammateName")
    self.m_GoBattleBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_GoBattleBtn")
    
    --m_cangkuApplyMember: ImageView  申请队列Ui
    self.m_cangkuApplyMember = ccui.Helper:seekWidgetByName(self.rootNode, "m_cangkuApplyMember")
    
    self.m_BagDetailListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_BagDetailListView")
    self.m_BagDetailListView:addTouchEventListener(touchEventHandler)
    
    self.m_Itemtouxiang = ccui.Helper:seekWidgetByName(self.rootNode, "m_Itemtouxiang")
    
    self.m_waitDistributionNum = ccui.Helper:seekWidgetByName(self.rootNode, "Label_35")
    
    self.m_dangqian1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_dangqian1")
    self.m_dangqian2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_dangqian2")
    self.m_dangqianNum = ccui.Helper:seekWidgetByName(self.rootNode, "m_dangqianNum")
    
    self.m_Weidangqian = ccui.Helper:seekWidgetByName(self.rootNode, "m_Weidangqian")
    
    self.m_yesApply = ccui.Helper:seekWidgetByName(self.rootNode, "m_yesApply")
    self.m_notApply = ccui.Helper:seekWidgetByName(self.rootNode, "m_notApply")
    
    self.m_closeCangkuApply = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeCangkuApply")
    self.m_closeCangkuApply:addTouchEventListener(touchEventHandler)
    
    --公会仓库申请改变，二次确认框
    self.m_sure2Cangku = ccui.Helper:seekWidgetByName(self.rootNode, "m_sure2Cangku")
    
    self.m_AlreadyApplyItem = ccui.Helper:seekWidgetByName(self.rootNode, "m_AlreadyApplyItem")
    self.m_AlreadyApplyItem_2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_alreadyApplyItem_2")
    self.m_donotApplyItem = ccui.Helper:seekWidgetByName(self.rootNode, "m_donotApplyItem")
    
    self.m_Cancel10 = ccui.Helper:seekWidgetByName(self.rootNode, "m_Cancel10")
    self.m_Cancel10:addTouchEventListener(touchEventHandler)
    
    self.m_confirm10 = ccui.Helper:seekWidgetByName(self.rootNode, "m_confirm10")
    --self.m_confirm10:addTouchEventListener(touchEventHandler)
    
    --小红点
    self.m_redDot = ccui.Helper:seekWidgetByName(self.rootNode, "m_RedDot")
    
	self:initUI()
end

function UnionUi:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_Panel_Union and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_Panel_Union touched--")
	
    elseif sender == self.m_closeCangkuApply and event == cc.EventCode.ENDED then
        self.m_cangkuApplyMember:setVisible(false)
        self.m_ShowUnionCangKu:setVisible(true)
	
    elseif sender == self.m_BagDetailListView and event == cc.EventCode.ENDED then
        cclog("物品申请详情")
		
    elseif sender == self.m_DACAN and event == cc.EventCode.ENDED then
        cclog("dacan")
        --公会大餐界面
        self.m_LeaderMenu:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_CoverPanel:setVisible(true)
        self.m_LookUpUnionMember:setVisible(false)
        self.m_ShowFood:setVisible(true)
        self.m_UnionFoodNotice:setVisible(true)
        self.m_Sure2Union:setVisible(false)
        self.m_Sure2Exit:setVisible(false)
        self.m_Sure2Dismiss:setVisible(false)
        self.m_Sure2Change:setVisible(false)
        self.m_Sure2Fresh:setVisible(false)
        
        Http:req("GetGuildDinnerRequest",nil,function(res)
            if res.data.bestEator ~= nil then
                self.m_UnionFoodNotice:setString(res.data.bestEator.."今天第一个分享了大餐，大家一起去品尝吧~")
            else
                self.m_UnionFoodNotice:setString("来个土豪分享大餐吧")
            end 
            
            if res.data.eatId ~= 0 then
                self.m_keleBtn:setVisible(false)
                self.m_hanbaoBtn:setVisible(false)
                self.m_kaojiBtn:setVisible(false)

                self.m_AkeleBtn:setVisible(true)
                self.m_AhanbaoBtn:setVisible(true)
                self.m_AkaojiBtn:setVisible(true)
            else
                self.m_keleBtn:setVisible(true)
                self.m_hanbaoBtn:setVisible(true)
                self.m_kaojiBtn:setVisible(true)

                self.m_AkeleBtn:setVisible(false)
                self.m_AhanbaoBtn:setVisible(false)
                self.m_AkaojiBtn:setVisible(false)    
            end
        end)

        self.m_SetUpUnion:setVisible(false)
        self.m_ApplyUnionList:setVisible(false)
        self.m_CorrectUnionNotice:setVisible(false)
        
    elseif sender == self.m_keleBtn and event == cc.EventCode.ENDED then
        cclog("taste kele")
        local kele = 1
        self:UnionFood(kele)
            
    elseif sender == self.m_hanbaoBtn and event == cc.EventCode.ENDED then
        cclog("taste hamburger")
        local hanbao = 2
        self:UnionFood(hanbao)
      
    elseif sender == self.m_kaojiBtn and event == cc.EventCode.ENDED then
        cclog("taste chicken")
        local kaoji = 3
        self:UnionFood(kaoji)
      
    elseif sender == self.m_ExitShowFoodBtn and event == cc.EventCode.ENDED then 
        cclog("exitFood")
        --退出公会大餐
        self.m_ShowFood:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self.m_UnionNoticeTitle:setVisible(true)
        self.m_UnionNoticeDetails:setVisible(true)
        self.m_MyUnionInfo:setVisible(true)
        self.m_MyUnionActivity:setVisible(true)
    
    elseif sender == self.m_kele and event == cc.EventCode.BEGAN then 
        self.m_KeleWord:setVisible(true)
        
    elseif sender == self.m_kele and event == cc.EventCode.ENDED then
        self.m_KeleWord:setVisible(false)
    
    elseif sender == self.m_hanbao and event == cc.EventCode.BEGAN then
        self.m_hanbaoWord:setVisible(true)
        
    elseif sender == self.m_hanbao and event == cc.EventCode.ENDED then
        self.m_hanbaoWord:setVisible(false)    
    
    elseif sender == self.m_kaoji and event == cc.EventCode.BEGAN then
        self.m_kaojiWord:setVisible(true)
    
    elseif sender == self.m_kaoji and event == cc.EventCode.ENDED then
        self.m_kaojiWord:setVisible(false)
        
        
    elseif sender == self.m_UnionShop and event == cc.EventCode.ENDED then
        cclog("shop")
        --公会商店界面
        self.m_LeaderMenu:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_LookUpUnionMember:setVisible(false)
        self.m_ShowUnionShop:setVisible(true)
        self.m_MemberGongXianNum:setVisible(true)
        self.m_Sure2Union:setVisible(false)
        self.m_Sure2Exit:setVisible(false)
        self.m_Sure2Dismiss:setVisible(false)
        self.m_Sure2Change:setVisible(false)
        self.m_Sure2Fresh:setVisible(false)
        self.m_CoverPanel:setVisible(true)
        
        self:UnionShopFace()

        self.m_SetUpUnion:setVisible(false)
        self.m_ApplyUnionList:setVisible(false)
        self.m_CorrectUnionNotice:setVisible(false)
    elseif sender == self.m_UnionShop and event == cc.EventCode.BEGAN then 
        cclog("12312")  
    elseif sender == self.m_CloseUnionShopBtn and event == cc.EventCode.ENDED then 
        cclog("exitshop")
        --退出公会商店
        self.m_ShowUnionShop:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self.m_UnionNoticeTitle:setVisible(true)
        self.m_UnionNoticeDetails:setVisible(true)
        self.m_MyUnionInfo:setVisible(true)
        self.m_MyUnionActivity:setVisible(true) 
        
    elseif sender == self.m_UnionShopFreshBtn and event == cc.EventCode.ENDED then
        cclog("公会商店刷新")
        --刷新商店
        --local FreshNum = 1
        self.m_Sure2Union:setVisible(false)
        self.m_Sure2Exit:setVisible(false)
        self.m_Sure2Dismiss:setVisible(false)
        self.m_Sure2Change:setVisible(false)
        
        self.m_Sure2Fresh:setVisible(true)
        self.m_CoverPanel:setVisible(true)
        Http:req("GetGuildShopRequest",nil,function(rse)
            print(rse)
            self.m_FreshNum:setString(rse.data.refreshCnt)
            local FreshNum = tonumber(rse.data.refreshCnt) + 1
            self.m_PGongXian:setString(shopRefreshConfig[tostring(FreshNum)]["cost"])
        end)
        
    elseif sender == self.m_Cancel5 and event == cc.EventCode.ENDED then 
        cclog("123")
        self.m_Sure2Fresh:setVisible(false)
        
    elseif sender == self.m_confirm5 and event == cc.EventCode.ENDED then
        Http:req("RefreshGuildShopRequest",nil,function(res)
            print("res")
            if res.data.result == 18 then
                PopDialog:getInstance():popText("抱歉！今日刷新次数已满",cc.c3b(0,255,0))
            elseif res.data.result == 0 then 
                PopDialog:getInstance():popText("刷新成功！",cc.c3b(0,255,0)) 
                --FreshNum = FreshNum + 1
                self:UnionShopFace()
            elseif res.data.result == 3 then 
                PopDialog:getInstance():popText("贡献币不足！",cc.c3b(0,255,0))
            end
        end)
        self.m_Sure2Fresh:setVisible(false)
          
    elseif sender == self.m_UnionCangku and event == cc.EventCode.ENDED then
        cclog("cangku")
        --公会仓库界面
        self.m_LeaderMenu:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_CoverPanel:setVisible(true)
        self.m_LookUpUnionMember:setVisible(false)
        self.m_ShowUnionCangKu:setVisible(true)
        self.m_Sure2Union:setVisible(false)
        self.m_Sure2Exit:setVisible(false)
        self.m_Sure2Dismiss:setVisible(false)
        self.m_Sure2Change:setVisible(false)
        self.m_Sure2Fresh:setVisible(false)
        
        self:UnionWareHouse()

        self.m_SetUpUnion:setVisible(false)
        self.m_ApplyUnionList:setVisible(false)
        self.m_CorrectUnionNotice:setVisible(false)

    elseif sender == self.m_CloseCangkuBtn and event == cc.EventCode.ENDED then 
        cclog("exitCangku")
        --退出公会仓库
        self.m_ShowUnionCangKu:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self.m_sure2Cangku:setVisible(false)
        self.m_UnionNoticeTitle:setVisible(true)
        self.m_UnionNoticeDetails:setVisible(true)
        self.m_MyUnionInfo:setVisible(true)
        self.m_MyUnionActivity:setVisible(true)
     
    elseif sender == self.m_RuleBtn and event == cc.EventCode.ENDED then 
        cclog("仓库规则说明")
        
    elseif sender == self.m_CangkuListView and event == cc.EventCode.ENDED then
        cclog("仓库物品滚动层")      
    
    elseif sender == self.m_Cancel10 and event == cc.EventCode.ENDED then
        cclog("否") 
        self.m_sure2Cangku:setVisible(false)
          
    elseif sender == self.m_UnionBoss and event == cc.EventCode.ENDED then
        cclog("Boss")
        --进入公会Boss
        
        self.m_LeaderMenu:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_CoverPanel:setVisible(true)
        self.m_LookUpUnionMember:setVisible(false)
        self.m_ShowUnionCangKu:setVisible(false)
        self.m_Sure2Union:setVisible(false)
        self.m_Sure2Exit:setVisible(false)
        self.m_Sure2Dismiss:setVisible(false)
        self.m_Sure2Change:setVisible(false)
        self.m_Sure2Fresh:setVisible(false)
        
        self:UnionBossInterface()

        self.m_SetUpUnion:setVisible(false)
        self.m_ApplyUnionList:setVisible(false)
        self.m_CorrectUnionNotice:setVisible(false)
        self.m_ShowUnionBoss:setVisible(true)
    
    elseif sender == self.m_Stage1 and event == cc.EventCode.ENDED then
        local StageNum = 1    
    
    elseif sender == self.m_Stage2 and event == cc.EventCode.ENDED then
        local StageNum = 2
        
    elseif sender == self.m_CloseBossBtn and event == cc.EventCode.ENDED then  
        --退出公会BOSS
        self.m_ShowUnionBoss:setVisible(false)
        self.m_Sure2OpenStage:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self.m_UnionNoticeTitle:setVisible(true)
        self.m_UnionNoticeDetails:setVisible(true)
        self.m_MyUnionInfo:setVisible(true)
        self.m_MyUnionActivity:setVisible(true) 
        
        
    elseif sender == self.m_CloseSmallBtn and event == cc.EventCode.ENDED then 
        --退出公会BOSS二级界面 
        self.m_ShowSmallBoss:setVisible(false)
        self.m_ShowUnionBoss:setVisible(true)
        
        self.m_CloseSmallBtn:setVisible(false)
        self.m_CloseBossBtn:setVisible(true)
        
    elseif sender == self.m_Close2BattleBtn and event == cc.EventCode.ENDED then
        --退出 进入布阵前界面 
        self.m_prepare2Battle:setVisible(false)
        
        self:SmallBossInterface(self.IndexBossID)

	elseif sender == self.m_ScrollUnionList and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_ScrollUnionList touched--")
	
    elseif sender == self.m_GoodsScrollView and event == cc.EventCode.ENDED then
        cclog("公会商店滚动层")
        
    elseif sender == self.m_ExitLeader2Normal and event == cc.EventCode.ENDED then 
        cclog("退出1") 
        self.m_Leader2NormalBtn:setVisible(false)
        
    elseif sender == self.m_Pofficial and event == cc.EventCode.ENDED then 
        cclog("提升长老")
        self.m_Sure2Zhanglao:setVisible(true)
        self.m_Leader2NormalBtn:setVisible(false)
        
    elseif sender == self.m_PromoteLeader and event == cc.EventCode.ENDED then 
        cclog("提升会长")
        self.m_Sure2Huizhang:setVisible(true)
        self.m_Leader2OfficialBtn:setVisible(false)
          
    elseif sender == self.m_DismissOfficial and event == cc.EventCode.ENDED then 
        cclog("解职长老")
        self.m_Sure2Jiezhi:setVisible(true)
        self.m_Leader2OfficialBtn:setVisible(false)
        
    elseif sender == self.m_Cancel6 and event == cc.EventCode.ENDED then 
        self.m_Sure2Zhanglao:setVisible(false)
        
    elseif sender == self.m_Cancel7 and event == cc.EventCode.ENDED then 
        self.m_Sure2Huizhang:setVisible(false)
        
    elseif sender == self.m_Cancel8 and event == cc.EventCode.ENDED then 
        self.m_Sure2Jiezhi:setVisible(false)
                      
    elseif sender == self.m_ExitLeader2official and event == cc.EventCode.ENDED then 
        cclog("退出2") 
        self.m_Leader2OfficialBtn:setVisible(false) 
        
    elseif sender == self.m_ExitOfficial2Normal and event == cc.EventCode.ENDED then 
        cclog("退出3") 
        self.m_Official2Normal:setVisible(false)            

	elseif sender == self.m_UnionCloseButton and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_UnionCloseButton touched--")
        local scene = require("scene.home.HomeScene") : create()
        cc.Director:getInstance():replaceScene(scene)

	elseif sender == self.m_JoinUnionBtn and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_JoinUnionBtn touched--")
        --进入公会界面，待加入公会列表
        self.m_FoundUnionFace:setVisible(false)
        self.m_UnionNoticeTitle:setVisible(false)
        self.m_UnionNoticeDetails:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self.m_MyUnionInfo:setVisible(false)
        self.m_MyUnionActivity:setVisible(false)
        self.m_JoinUnionBtn:setHighlighted(true)
        self.m_JoinUnionBtn:setEnabled(false)
        
        self.m_FoundUnionBtn:setHighlighted(false)
        self.m_FoundUnionBtn:setEnabled(true)

        self.m_ScrollUnionList:setVisible(true)
        
        self:UnionList()

	elseif sender == self.m_FoundUnionBtn and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_FoundUnionBtn touched--")
        --创建公会界面
        self.m_UnionNoticeTitle:setVisible(false)
        self.m_UnionNoticeDetails:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self.m_MyUnionInfo:setVisible(false)
        self.m_MyUnionActivity:setVisible(false)
        self.m_ScrollUnionList:setVisible(false)
        
        self.m_JoinUnionBtn:setHighlighted(false)
        self.m_JoinUnionBtn:setEnabled(true)
        
        self.m_FoundUnionBtn:setHighlighted(true)
        self.m_FoundUnionBtn:setEnabled(false)

        self.m_FoundUnionFace:setVisible(true)

	elseif sender == self.m_CoverPanel and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_CoverPanel touched--")

	elseif sender == self.m_TitleFoundCost and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_TitleFoundCost touched--")

	elseif sender == self.m_FoundUnionButton and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_FoundUnionButton touched--")
		--确认创建公会按钮,弹出二次确认框
        self.m_CoverPanel:setVisible(true)
        self.m_Sure2Union:setVisible(true)
        self.m_SetUpUnion:setVisible(false)
        self.m_ApplyUnionList:setVisible(false)
        self.m_LookUpUnionMember:setVisible(false)
        self.m_CorrectUnionNotice:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_LeaderMenu:setVisible(false)
        self.m_ShowFood:setVisible(false)
        self.m_ShowUnionShop:setVisible(false)
        self.m_ShowUnionCangKu:setVisible(false)
        self.m_Sure2Exit:setVisible(false)
        self.m_Sure2Dismiss:setVisible(false)
        self.m_Sure2Change:setVisible(false)
        self.m_Sure2Fresh:setVisible(false)
        
    elseif sender == self.m_cancel and event == cc.EventCode.ENDED then
        self.m_Sure2Union:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        
    elseif sender == self.m_Confirm and event == cc.EventCode.ENDED then 
    --确认创建 
        local UnionNamestring = string.gsub(self.m_InputUnionName:getStringValue(),"^%s*(.-)%s*$","%1")
        if string.len(UnionNamestring) == 0 or string.len(UnionNamestring) > 6 then
            PopDialog:getInstance():popText("名称输入不合法！",cc.c3b(0,255,0))
        end
        Http:req("CreatGuildRequest",{guildName=UnionNamestring},function(res)
            if res.data["result"] == 0 then
                self.m_Sure2Union:setVisible(false)
                self.m_CoverPanel:setVisible(false)
                
                PopDialog:getInstance():popText("恭喜！创建公会成功",cc.c3b(0,255,0))
                self.m_UnionNoticeTitle:setVisible(true)
                self.m_UnionNoticeDetails:setVisible(true)
                self.m_CoverPanel:setVisible(false)
                self.m_ScrollUnionList:setVisible(false)
                self.m_MyUnionInfo:setVisible(true)
                self.m_MyUnionActivity:setVisible(true)

                self.m_FoundUnionFace:setVisible(false)
                self.m_JoinUnionBtn:setVisible(false)
                self.m_FoundUnionBtn:setVisible(false)
                self.m_Sure2Union:setVisible(false)

                self:initMyUnion()

            elseif res.data["result"] == 1 then
                PopDialog:getInstance():popText("此名称太受欢迎，已有人抢了",cc.c3b(0,255,0))
                self.m_Sure2Union:setVisible(false)
                self.m_CoverPanel:setVisible(false)
            elseif res.data["result"] == 2 then
                PopDialog:getInstance():popText("你输入的名字中存在敏感字符",cc.c3b(0,255,0))
                self.m_Sure2Union:setVisible(false)
                self.m_CoverPanel:setVisible(false)
            elseif res.data.guildName == "" then
                PopDialog:getInstance():popText("无效公会名",cc.c3b(0,255,0))
                self.m_Sure2Union:setVisible(false)
                self.m_CoverPanel:setVisible(false)    
            elseif res.data["result"] == 3 then
                PopDialog:getInstance():popText("钻石不足，请充值",cc.c3b(0,255,0))
                self.m_Sure2Union:setVisible(false)
                self.m_CoverPanel:setVisible(false)
            end
        end)    

	elseif sender == self.m_MyUnionCtrlButton and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_MyUnionCtrlButton touched--")
        --公会管理按钮点击后界面
        self.m_Sure2Union:setVisible(false)
        self.m_Sure2Exit:setVisible(false)
        self.m_Sure2Dismiss:setVisible(false)
        self.m_Sure2Change:setVisible(false)
        self.m_Sure2Fresh:setVisible(false)
        self.m_Leader2OfficialBtn:setVisible(false)
        self.m_Leader2NormalBtn:setVisible(false)
        self.m_Official2Normal:setVisible(false)
        
        self:updataTipPic()
        
        self.m_CoverPanel:setVisible(true)
        Http:req("GuildInfoRequest",nil,function(res)
            if  res.data.guildInfo.guildPos == 1 then
                self.m_LeaderMenu:setVisible(true)
                self.m_NormalMemberMenu:setVisible(false)
                self.m_APPLY:setVisible(true)
                self.m_MEMBER:setVisible(true)
                self.m_CORRECTNotice:setVisible(true)
                self.m_UnionArrange:setVisible(true)
                self.m_DismissUnion:setVisible(true) 
                self.m_ExitUnion:setVisible(false)
            elseif  res.data.guildInfo.guildPos == 2 then
                self.m_LeaderMenu:setVisible(true)
                self.m_NormalMemberMenu:setVisible(false)
                self.m_APPLY:setVisible(true)
                self.m_MEMBER:setVisible(true)
                self.m_CORRECTNotice:setVisible(true)
                self.m_UnionArrange:setVisible(true)
                self.m_DismissUnion:setVisible(false) 
                self.m_ExitUnion:setVisible(true)
            elseif  res.data.guildInfo.guildPos == 3 then 
                self.m_NormalMemberMenu:setVisible(true)
                self.m_LeaderMenu:setVisible(false)
                self.m_NormalUnionMember:setVisible(true)
                self.m_NormalExitUnion:setVisible(true)
            end
        end)

	elseif sender == self.m_InputUnionName and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_InputUnionName touched--")

	elseif sender == self.m_Left1 and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_Left1 touched--")
        IndexNum = self.m_JoinWithAdmit:getString()-1
            if IndexNum <= 0 then
                self.m_SelectIndex:setString("所有人都可以加入")
                self.m_JoinWithAdmit:setString(0)
            else
                if IndexNum == 1 then
                    self.m_SelectIndex:setString("通过验证才可以加入")
                elseif  IndexNum == 2 then  
                    self.m_SelectIndex:setString("任何人不得加入")
                end 
            self.m_JoinWithAdmit:setString(IndexNum)
            end 
          

	elseif sender == self.m_Right1 and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_Right1 touched--")
        IndexNum = self.m_JoinWithAdmit:getString()+1
        if IndexNum >= 2 then
            self.m_SelectIndex:setString("任何人不得加入")
            self.m_JoinWithAdmit:setString(2)
        else
            if IndexNum == 0 then
                self.m_SelectIndex:setString("所有人都可以加入")
            elseif IndexNum == 1 then 
                self.m_SelectIndex:setString("通过验证才可以加入")
            end
            self.m_JoinWithAdmit:setString(IndexNum)    
        end  

	elseif sender == self.m_Left2 and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_Left2 touched--")
        LvNumber = self.m_LV:getString()-1
        if LvNumber <= 30 then
            self.m_LV:setString(30)
         else
            self.m_LV:setString(LvNumber)
         end  

	elseif sender == self.m_Right2 and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_Right2 touched--")
        LvNumber = self.m_LV:getString()+1
        if LvNumber >= 100 then
            self.m_LV:setString(100)
        else
            self.m_LV:setString(LvNumber)
        end 

	elseif sender == self.m_CancleBtn and event == cc.EventCode.ENDED then
        cclog("[UnionUi]--m_CancleBtn touched--")
        self.m_SetUpUnion:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self.m_UnionNoticeTitle:setVisible(true)
        self.m_UnionNoticeDetails:setVisible(true)
        self.m_MyUnionInfo:setVisible(true)
        self.m_MyUnionActivity:setVisible(true)

    --公会设置确认按钮
	elseif sender == self.m_ConfirmButton and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_ConfirmButton touched--")
		
        local MinLv = self.m_LV:getString()
        local ApplyIndex = self.m_JoinWithAdmit:getString()
		
        Http: req("GuildSettingRequest",{minMemberLv = MinLv,applyType = ApplyIndex},function(res)
          if res.data["result"] == 0 then
             PopDialog:getInstance():popText("设置已保存！",cc.c3b(0,255,0))
          end
        end)
        self.m_SetUpUnion:setVisible(false)
        self.m_CoverPanel:setVisible(false)      

	elseif sender == self.m_ExitLookUpBtn and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_ExitLookUpBtn touched--")
        self.m_ApplyUnionList:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self:initMyUnion()

	elseif sender == self.m_ScrollUnionMember and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_ScrollUnionMember touched--")

	elseif sender == self.m_CloseLookUpMemButton and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_CloseLookUpMemButton touched--")
        self.m_LookUpUnionMember:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self.m_Sure2Zhanglao:setVisible(false)
        self.m_Sure2Huizhang:setVisible(false)
        self.m_Sure2Jiezhi:setVisible(false)
        
        self:initMyUnion()

	elseif sender == self.m_morenpaixu and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_morenpaixu touched--")
		self.m_morenpaixu:setHighlighted(true)
        self.m_morenpaixu:setEnabled(false)
        
        self.m_ThreeDaysContribution:setHighlighted(false)
        self.m_ThreeDaysContribution:setEnabled(true)
		self:MyUnionMember()

	elseif sender == self.m_CloseCorrectUnionButton and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_CloseCorrectUnionButton touched--")
        self.m_CorrectUnionNotice:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self.m_UnionNoticeTitle:setVisible(true)
        self.m_UnionNoticeDetails:setVisible(true)
        self.m_MyUnionInfo:setVisible(true)
        self.m_MyUnionActivity:setVisible(true)

	elseif sender == self.m_CorrectNoticeBtn and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_CorrectNoticeBtn touched--")
		
        local UnionNoticestring = string.gsub(self.m_inputNotice:getStringValue(),"^%s*(.-)%s*$","%1")
        if string.len(UnionNoticestring) == 0 then
            PopDialog:getInstance():popText("公告不能为空！",cc.c3b(0,255,0))
        else
            Http:req("ModifyNoticeRequest",{notice = UnionNoticestring},function(res)

                    if res.data.result == 0 then
                        self.m_UnionNoticeDetails:setString(UnionNoticestring)
                    end
            end)
            self.m_CorrectUnionNotice:setVisible(false)
            self.m_CoverPanel:setVisible(false)
        end

	elseif sender == self.m_NormalUnionMember and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_NormalUnionMember touched--")
		--群众查看成员
        self.m_LeaderMenu:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_CoverPanel:setVisible(true)
        self.m_LookUpUnionMember:setVisible(true)
        self.m_ScrollUnionMember:setVisible(true)
        
        self.m_morenpaixu:setHighlighted(true)
        self.m_morenpaixu:setEnabled(false)
        
        self.m_ThreeDaysContribution:setHighlighted(false)
        self.m_ThreeDaysContribution:setEnabled(true)

        self:MyUnionMember()

        self.m_SetUpUnion:setVisible(false)
        self.m_ApplyUnionList:setVisible(false)
        self.m_CorrectUnionNotice:setVisible(false)
        self.m_Sure2Union:setVisible(false)

	elseif sender == self.m_NormalExitUnion and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_NormalExitUnion touched--")
		--群众退出公会
        self.m_Sure2Union:setVisible(false)
        self.m_Sure2Exit:setVisible(true)
        self.m_Sure2Dismiss:setVisible(false)
        self.m_Sure2Change:setVisible(false)
        self.m_Sure2Fresh:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_CoverPanel:setVisible(true)
         
    elseif sender == self.m_Cancel2 and event == cc.EventCode.ENDED then 
        self.m_Sure2Exit:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        
    elseif sender == self.m_confirm2 and event == cc.EventCode.ENDED then 
        Http: req("LeaveGuildRequest",{rolerId=0},function(res)
            if res.data.result == 0 then 
                PopDialog:getInstance():popText("退出公会成功！",cc.c3b(0,255,0))
            end
            self:initUI()
        end) 
        
        self.m_Sure2Exit:setVisible(false)
        self.m_CoverPanel:setVisible(false)

	elseif sender == self.m_CloseUnionMenu2 and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_CloseUnionMenu2 touched--")
		--群众管理菜单关闭按钮
        self.m_NormalMemberMenu:setVisible(false)
        self.m_CoverPanel:setVisible(false)

	elseif sender == self.m_APPLY and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_APPLY touched--")
        --查看申请者列表界面
        self.m_CoverPanel:setVisible(true)
        self.m_LeaderMenu:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_ApplyUnionList:setVisible(true)
        
        self:ApplyMemberList()
        self.m_CorrectUnionNotice:setVisible(false)
        self.m_SetUpUnion:setVisible(false)
        self.m_LookUpUnionMember:setVisible(false)
        self.m_Sure2Union:setVisible(false)
        
        
	elseif sender == self.m_MEMBER and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_MEMBER touched--")
        --会长、长老查看公会成员*******
        self.m_LeaderMenu:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_Sure2Union:setVisible(false)
        self.m_CoverPanel:setVisible(true)
        self.m_LookUpUnionMember:setVisible(true)
        self.m_ScrollUnionMember:setVisible(true)
        
        self.m_morenpaixu:setHighlighted(true)
        self.m_morenpaixu:setEnabled(false)
        
        self.m_ThreeDaysContribution:setHighlighted(false)
        self.m_ThreeDaysContribution:setEnabled(true)
        
        self:MyUnionMember()
        
        self.m_SetUpUnion:setVisible(false)
        self.m_ApplyUnionList:setVisible(false)
        self.m_CorrectUnionNotice:setVisible(false)
    
	elseif sender == self.m_CORRECTNotice and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_CORRECTNotice touched--")
        --修改公告界面
        self.m_LeaderMenu:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_CoverPanel:setVisible(true)
        self.m_CorrectUnionNotice:setVisible(true)
        self.m_SetUpUnion:setVisible(false)
        self.m_ApplyUnionList:setVisible(false)
        self.m_LookUpUnionMember:setVisible(false)

	elseif sender == self.m_UnionArrange and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_UnionArrange touched--")
        --公会设置界面
        self.m_LeaderMenu:setVisible(false)
        self.m_NormalMemberMenu:setVisible(false)
        self.m_Sure2Union:setVisible(false)
        self.m_CoverPanel:setVisible(true)
        self.m_SetUpUnion:setVisible(true)
        Http: req("GuildInfoRequest",nil,function(res)
            if res.data.result == 0 then
                print(res.data.guildInfo.applyType)
                self.m_LV:setString(res.data.guildInfo.minMemberLv)
                self.m_JoinWithAdmit:setString(res.data.guildInfo.applyType)
                    if res.data.guildInfo.applyType == 0 then
                        self.m_SelectIndex:setString("所有人都可以加入")
                    elseif res.data.guildInfo.applyType == 1 then 
                        self.m_SelectIndex:setString("通过验证才可以加入") 
                    elseif res.data.guildInfo.applyType == 2 then 
                        self.m_SelectIndex:setString("任何人不得加入")
                    end
            end
        end)
        self.m_ApplyUnionList:setVisible(false)
        self.m_LookUpUnionMember:setVisible(false)
        self.m_CorrectUnionNotice:setVisible(false)

	elseif sender == self.m_DismissUnion and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_DismissUnion touched--")
        --解散公会界面
        self.m_Sure2Exit:setVisible(false)
        self.m_Sure2Dismiss:setVisible(true)
        self.m_Sure2Change:setVisible(false)
        self.m_Sure2Fresh:setVisible(false)
        self.m_LeaderMenu:setVisible(false)
        self.m_CoverPanel:setVisible(true)
        
        
    elseif sender == self.m_Cancel3 and event == cc.EventCode.ENDED then 
        self.m_Sure2Dismiss:setVisible(false)
        self.m_CoverPanel:setVisible(false)

    elseif sender == self.m_Confirm3 and event == cc.EventCode.ENDED then 
        Http: req("GuildInfoRequest",nil,function(res)
            if res.data.guildInfo.memberNum ~= 1 then
                PopDialog:getInstance():popText("当前公会还有成员存在，不能解散！",cc.c3b(0,255,0))
            else  
                Http: req("LeaveGuildRequest",{rolerId=0},function(res)
                    if res.data.result == 0 then 
                        PopDialog:getInstance():popText("解散公会成功！",cc.c3b(0,255,0))
                    end
                    self:initUI()
                end)
            end
        end)
        self.m_Sure2Dismiss:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        
	elseif sender == self.m_ExitUnion and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_ExitUnion touched--")
		--长老退出公会界面
        self.m_Sure2Union:setVisible(false)
        self.m_Sure2Exit:setVisible(true)
        self.m_Sure2Dismiss:setVisible(false)
        self.m_Sure2Change:setVisible(false)
        self.m_Sure2Fresh:setVisible(false)
        self.m_LeaderMenu:setVisible(false)
        self.m_CoverPanel:setVisible(true)
        
    elseif sender == self.m_Cancel2 and event == cc.EventCode.ENDED then 
        self.m_Sure2Exit:setVisible(false)
        self.m_CoverPanel:setVisible(false)

    elseif sender == self.m_confirm2 and event == cc.EventCode.ENDED then 
        Http: req("LeaveGuildRequest",{rolerId=0},function(res)
            if res.data.result == 0 then 
                PopDialog:getInstance():popText("退出公会成功！",cc.c3b(0,255,0))
            end
            self:initUI()
        end) 
        self.m_Sure2Exit:setVisible(false)
        self.m_CoverPanel:setVisible(false)

	elseif sender == self.m_CloseLeaderMenu and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_CloseLeaderMenu touched--")
        self.m_LeaderMenu:setVisible(false)
        self.m_CoverPanel:setVisible(false)
        self.m_UnionNoticeTitle:setVisible(true)
        self.m_UnionNoticeDetails:setVisible(true)
        self.m_MyUnionInfo:setVisible(true)
        self.m_MyUnionActivity:setVisible(true)

	elseif sender == self.m_inputNotice and event == cc.EventCode.ENDED then
		cclog("[UnionUi]--m_inputNotice touched--")
	end
end

--init your ui here
function UnionUi:initUI()
    Http: req("GuildInfoRequest",nil,function(res) 
        if res.data["result"] == 4 then
            --载入没公会时界面
            self.m_UnionNoticeTitle:setVisible(false)
            self.m_UnionNoticeDetails:setVisible(false)
            self.m_CoverPanel:setVisible(false)
            self.m_MyUnionInfo:setVisible(false)
            self.m_MyUnionActivity:setVisible(false)

            self.m_FoundUnionFace:setVisible(false)
            self.m_JoinUnionBtn:setVisible(true)
            self.m_FoundUnionBtn:setVisible(true)
            self.m_ScrollUnionList:setVisible(true)
            
            self.m_JoinUnionBtn:setHighlighted(true)
            self.m_JoinUnionBtn:setEnabled(false)
            
            self.m_FoundUnionBtn:setHighlighted(false)
            self.m_FoundUnionBtn:setEnabled(true)
            
            self:UnionList()
        else
            --载入有/自己公会界面，并且设置好数据
            self.m_UnionNoticeTitle:setVisible(true)
            self.m_UnionNoticeDetails:setVisible(true)
            self.m_CoverPanel:setVisible(false)
            self.m_ScrollUnionList:setVisible(false)
            self.m_MyUnionInfo:setVisible(true)
            self.m_MyUnionActivity:setVisible(true)

            self.m_FoundUnionFace:setVisible(false)
            self.m_JoinUnionBtn:setVisible(false)
            self.m_FoundUnionBtn:setVisible(false)
            
            self:initMyUnion()
        end
    end)
end

--refresh all ui here
function UnionUi:refreshUI()
end

--得到“我的公会信息”
function UnionUi:initMyUnion()
    Http: req("GuildInfoRequest",nil,function(res)
        --print(res)
        local p = ((res.data.guildInfo.guildExp) * 100)/(res.data.guildInfo.guildExpUp)
        if p>100 then
            p = 100
        elseif p<0 then
            p = 0
        end
        self.m_MyItemProgress:setVisible(true)
        self.m_MyItemProgress:setPercent(p)
        
        if res.data.result == 0 then
            self.m_UnionId:setString(res.data.guildInfo.guildName)
            self.m_MyUnionLV:setString(res.data.guildInfo.guildLevel)
            
            self.m_MyUnionPeopleNum:setString(res.data.guildInfo.memberNum.."/"..res.data.guildInfo.maxMember)
            self.m_MyUnionMoney:setString(res.data.guildInfo.guildFund)
            self.m_UnionEXP:setString(res.data.guildInfo.guildExp.."/"..res.data.guildInfo.guildExpUp)
            if res.data.guildInfo.guildNotice == "" then
                self.m_UnionNoticeDetails:setString("悄悄告诉你，参与击杀公会BOSS会有丰厚奖励哦~")
                else
                self.m_UnionNoticeDetails:setString(res.data.guildInfo.guildNotice)
            end
        end
    end)
end

--查看公会成员函数
function UnionUi:MyUnionMember()
    Http:req("GuildRolerListRequest",nil,function(rse)
    
        Http:req("GuildInfoRequest",nil,function(res)
        
           local CurrentMember = table.getn(rse.data["guildRolerList"])
            self.m_ScrollUnionMember:removeAllChildren()
            self.m_ScrollUnionMember:setBounceEnabled(true)
            
            local ScrovSize = self.m_ScrollUnionMember:getInnerContainerSize()
            local ScrovH = ScrovSize.height 
            
            if CurrentMember > 3 then
                ScrovH = 105 * CurrentMember
            end
            ScrovSize.height = ScrovH
            self.m_ScrollUnionMember:setInnerContainerSize(ScrovSize)
            
            --在滚动层显示公会成员
            for i=1,CurrentMember do
                local MemberItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("ShowUnionMemberInfo.csb")
                MemberItem:setName("MemberItem_"..i)
                local m_MemberId = ccui.Helper:seekWidgetByName(MemberItem, "m_MemberId")
                local m_MemberLV = ccui.Helper:seekWidgetByName(MemberItem, "m_MemberLV")
                local m_UnionMemberPosition = ccui.Helper:seekWidgetByName(MemberItem, "m_UnionMemberPosition")
                local m_ZongGongXian = ccui.Helper:seekWidgetByName(MemberItem, "m_ZongGongXian")
                m_ZongGongXian:setName("m_ZongGongXian_"..i)
                m_ZongGongXian:setVisible(false)
                local m_GongXianNumber = ccui.Helper:seekWidgetByName(MemberItem, "m_GongXianNumber")
                local m_LastTimeLogInTitle = ccui.Helper:seekWidgetByName(MemberItem, "m_LastTimeLogIn")
                m_LastTimeLogInTitle:setName("m_LastTimeLogInTitle_"..i)
                local m_ShouLoginTime = ccui.Helper:seekWidgetByName(MemberItem, "m_ShouLoginTime")
                local m_touxiangkuang = ccui.Helper:seekWidgetByName(MemberItem, "m_touxiangkuang")
                local m_PassBtn = ccui.Helper:seekWidgetByName(MemberItem, "Button_103")
                local m_RefuseBtn = ccui.Helper:seekWidgetByName(MemberItem, "Button_104")
                local m_OperateMemberButton = ccui.Helper:seekWidgetByName(MemberItem, "m_OperateMemberButton")
                
                m_PassBtn:setVisible(false)
                m_RefuseBtn:setVisible(false)
                m_UnionMemberPosition:setVisible(true)
                
                m_MemberId:setString(rse.data.guildRolerList[i].rolerName)
                m_MemberLV:setString(rse.data.guildRolerList[i].rolerLevel)
                --remember add "/soul/"
                m_touxiangkuang:loadTexture("soul/"..rse.data.guildRolerList[i].avatarId..".png")
                
                
                    if res.data.guildInfo.guildPos == 1 then
                        if rse.data.guildRolerList[i].guildPos == 2 then
                            m_UnionMemberPosition:setString("长老")
                            m_OperateMemberButton:setVisible(true)
                        elseif rse.data.guildRolerList[i].guildPos == 3 then
                            m_UnionMemberPosition:setString("群众")
                            m_OperateMemberButton:setVisible(true)
                        elseif rse.data.guildRolerList[i].guildPos == 1 then
                            m_UnionMemberPosition:setString("会长")
                            m_OperateMemberButton:setVisible(false)    
                        end 
                    elseif res.data.guildInfo.guildPos == 2 then
                        if rse.data.guildRolerList[i].guildPos == 1 then
                            m_UnionMemberPosition:setString("会长")
                            m_OperateMemberButton:setVisible(false)
                        elseif rse.data.guildRolerList[i].guildPos == 3 then
                            m_UnionMemberPosition:setString("群众")
                            m_OperateMemberButton:setVisible(true)
                        elseif rse.data.guildRolerList[i].guildPos == 2 then
                            m_UnionMemberPosition:setString("长老")
                            m_OperateMemberButton:setVisible(false)    
                        end 
                    elseif res.data.guildInfo.guildPos == 3 then 
                        if rse.data.guildRolerList[i].guildPos == 1 then
                            m_UnionMemberPosition:setString("会长")
                            m_OperateMemberButton:setVisible(false)
                        elseif rse.data.guildRolerList[i].guildPos == 2 then
                            m_UnionMemberPosition:setString("长老")
                            m_OperateMemberButton:setVisible(false)
                        elseif rse.data.guildRolerList[i].guildPos == 3 then
                            m_UnionMemberPosition:setString("群众")
                            m_OperateMemberButton:setVisible(false)  
                        end  
                    end
                
                local dateNum = math.floor(((rse.data.guildRolerList[i].lastLogin)/1000))
                local dateYear = os.date("%Y",dateNum)
                local dateMonth = os.date("%m",dateNum)
                local dateDay = os.date("%d",dateNum)
                local dateHour = os.date("%H",dateNum)--24小时制
                local dateMinute = os.date("%M",dateNum)
                m_ShouLoginTime:setString(dateYear.."-"..dateMonth.."-"..dateDay.." "..dateHour..":"..dateMinute)
                
                m_ShouLoginTime:setName("m_ShouLoginTime_"..i)
                m_GongXianNumber:setString(rse.data.guildRolerList[i].guildDevote)
                m_GongXianNumber:setName("m_GongXianNumber_"..i)
                m_GongXianNumber:setVisible(false)
                
                MemberItem:setPosition(13,ScrovSize.height-103*(i))
                local MemberId = rse.data.guildRolerList[i].rolerId
                MemberItem:setName("MemberItem_"..MemberId)
                self.m_ScrollUnionMember:addChild(MemberItem)
                
                local function ShowMember2CallBack(sender,event)--成员操作按钮
                    if sender == m_OperateMemberButton and event == cc.EventCode.ENDED then
                        --操作者是会长
                        if roleData:get("guildPosition") == 1 then
                            if rse.data.guildRolerList[i].guildPos == 2 then--会长对长老
                                self.m_Leader2OfficialBtn:setVisible(true)
                                self.m_Leader2NormalBtn:setVisible(false)
                                self.m_Official2Normal:setVisible(false)
                                
                                self.m_ID_7:setString(rse.data.guildRolerList[i].rolerName)
                                self.m_ID_7:setTag(i)
                                
                                self.m_ID_8:setString(rse.data.guildRolerList[i].rolerName)
                                self.m_ID_8:setTag(i)
                                
                                local function OperateMember2CallBack(sender,event)
                                    if sender == self.m_confirm7 and event == cc.EventCode.ENDED then
                                        Http:req("AssignGuildPosRequest",{rolerId = rse.data.guildRolerList[i].rolerId,guildPos = 1},function(res)
                                            if res.data.result == 0 then--提升为会长
                                                PopDialog:getInstance():popText("操作成功",cc.c3b(0,255,0))
                                                self.m_Sure2Huizhang:setVisible(false)
                                                self:MyUnionMember()
                                            end
                                        end)
                                     elseif sender == self.m_confirm8 and event == cc.EventCode.ENDED then
                                        Http:req("AssignGuildPosRequest",{rolerId = rse.data.guildRolerList[i].rolerId,guildPos = 3},function(res)
                                            if res.data.result == 0 then--降职为群众
                                                PopDialog:getInstance():popText("操作成功",cc.c3b(0,255,0))
                                                self.m_Sure2Jiezhi:setVisible(false)
                                                self:MyUnionMember()
                                            end
                                        end)
                                     end
                                    
                                end
                                self.m_confirm7:addTouchEventListener(OperateMember2CallBack)
                                self.m_confirm8:addTouchEventListener(OperateMember2CallBack)
                                
                            elseif rse.data.guildRolerList[i].guildPos == 3 then--会长对群众
                                self.m_Leader2OfficialBtn:setVisible(false)
                                self.m_Official2Normal:setVisible(false)
                                self.m_Leader2NormalBtn:setVisible(true)
                                self.m_ID_6:setString(rse.data.guildRolerList[i].rolerName)
                                self.m_ID_6:setTag(i)
                                
                                local function OperateMember2CallBack_2(sender,event)
                                    if sender == self.m_confirm6 and event == cc.EventCode.ENDED then
                                        Http:req("AssignGuildPosRequest",{rolerId = rse.data.guildRolerList[i].rolerId,guildPos = 2},function(res)
                                            if res.data.result == 0 then--提升为长老
                                                PopDialog:getInstance():popText("操作成功",cc.c3b(0,255,0))
                                                self.m_Sure2Zhanglao:setVisible(false)
                                                self:MyUnionMember()
                                            elseif res.data.result == 9 then 
                                                PopDialog:getInstance():popText("长老名额已满",cc.c3b(0,255,0))
                                                self.m_Sure2Zhanglao:setVisible(false)
                                                self:MyUnionMember()   
                                            end
                                        end)
                                    elseif sender == self.m_LetLeaveUnionBtn and event == cc.EventCode.ENDED then  
                                        Http:req("LeaveGuildRequest",{rolerId = rse.data.guildRolerList[i].rolerId},function(res)
                                            if res.data.result == 0 then--逐出公会
                                                PopDialog:getInstance():popText("操作成功",cc.c3b(0,255,0))
                                                self.m_Leader2NormalBtn:setVisible(false)
                                                self:MyUnionMember()
                                            end
                                        end) 
                                    end
                                end
                                self.m_confirm6:addTouchEventListener(OperateMember2CallBack_2)
                                self.m_LetLeaveUnionBtn:addTouchEventListener(OperateMember2CallBack_2)
                         end 
                    --操作者是长老    
                    elseif roleData:get("guildPosition") == 2 then --长老对群众
                            self.m_Official2Normal:setVisible(true)
                            self.m_Leader2OfficialBtn:setVisible(false)
                            self.m_Leader2NormalBtn:setVisible(false)
                            
                            local function OperateMember2CallBack_3(sender,event)
                                if sender == self.m_LetLeaveUnion_2 and event == cc.EventCode.ENDED then
                                    Http:req("LeaveGuildRequest",{rolerId = rse.data.guildRolerList[i].rolerId},function(res)
                                        if res.data.result == 0 then
                                            PopDialog:getInstance():popText("操作成功",cc.c3b(0,255,0))
                                            self.m_Official2Normal:setVisible(false)
                                            self:MyUnionMember()
                                        end
                                    end)         
                                end
                            end    
                            self.m_LetLeaveUnion_2:addTouchEventListener(OperateMember2CallBack_3)
                        end
                    end
                end 
                m_OperateMemberButton:addTouchEventListener(ShowMember2CallBack)
                
                local function ShowMember2CallBack_1(sender,event)--三日贡献按钮
                    if sender == self.m_ThreeDaysContribution and event == cc.EventCode.ENDED then
                        for i_1 = 1,CurrentMember do
                            local n_ZongGongXian = ccui.Helper:seekWidgetByName(self.m_ScrollUnionMember, "m_ZongGongXian_"..(i_1)) 
                            local n_GongXianNumber = ccui.Helper:seekWidgetByName(self.m_ScrollUnionMember, "m_GongXianNumber_"..(i_1))
                            local n_LastTimeLogInTitle = ccui.Helper:seekWidgetByName(self.m_ScrollUnionMember, "m_LastTimeLogInTitle_"..(i_1))
                            local n_ShouLoginTime = ccui.Helper:seekWidgetByName(self.m_ScrollUnionMember, "m_ShouLoginTime_"..(i_1))
                            
                            n_ZongGongXian:setVisible(true)
                            n_GongXianNumber:setVisible(true)
                            n_LastTimeLogInTitle:setVisible(false)
                            n_ShouLoginTime:setVisible(false)
                            
                            self.m_morenpaixu:setHighlighted(false)
                            self.m_morenpaixu:setEnabled(true)
                            self.m_ThreeDaysContribution:setHighlighted(true)
                            self.m_ThreeDaysContribution:setEnabled(false)
                        end
                    end
                end
                self.m_ThreeDaysContribution:addTouchEventListener(ShowMember2CallBack_1) 
            end
            
        end)    
    end)
end

--显示待加入公会 列表
function UnionUi:UnionList()
    Http:req("GuildListRequest",nil,function(res)
        local UnionNumber = table.getn(res.data["guildList"])
        
        self.m_ScrollUnionList:removeAllChildren()
        self.m_ScrollUnionList:setBounceEnabled(true)
        
        local ScrovSize = self.m_ScrollUnionList:getInnerContainerSize()
        local ScrovH = ScrovSize.height
              
        if UnionNumber > 4 then
            if UnionNumber%2 == 0 then
                ScrovH = (195+5)*(UnionNumber/2)
            else
                ScrovH = (195+5)*(math.floor(UnionNumber/2)+1)
            end
        end
        
        ScrovSize.height = ScrovH
        
        self.m_ScrollUnionList:setInnerContainerSize(ScrovSize)
        
        for i=1,UnionNumber do
            local UnionItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("ShowUnionInfo.csb")

            local m_UnionId = ccui.Helper:seekWidgetByName(UnionItem, "m_UnionId")
            local m_UnionPeopleNum = ccui.Helper:seekWidgetByName(UnionItem, "m_UnionPeopleNum")
            local m_UnionLeaderName = ccui.Helper:seekWidgetByName(UnionItem, "m_UnionLeaderName")
            local m_UnionLimitNum = ccui.Helper:seekWidgetByName(UnionItem, "m_UnionLimitNum")
            local m_UnionLevelNum = ccui.Helper:seekWidgetByName(UnionItem, "m_UnionLevelNum")
            local m_ApplyUnionBtn = ccui.Helper:seekWidgetByName(UnionItem, "m_ApplyUnionBtn")
            
            local m_UnionPeopleFull = ccui.Helper:seekWidgetByName(UnionItem, "m_UnionPeopleFull")
            local m_AlreadyApply = ccui.Helper:seekWidgetByName(UnionItem, "Button_89")  
            m_UnionId:setString(res.data.guildList[i].guildName)--得到公会名称
            m_UnionPeopleNum:setString(res.data.guildList[i].memberNum.."/"..res.data.guildList[i].maxMember)--得到公会人数
            
            if res.data.guildList[i].memberNum ~= res.data.guildList[i].maxMember then--判断是否可以加入
                m_ApplyUnionBtn:setVisible(true)
                m_UnionPeopleFull:setVisible(false)
                m_AlreadyApply:setVisible(false)
            else
                m_ApplyUnionBtn:setVisible(false)
                m_UnionPeopleFull:setVisible(true)
                m_AlreadyApply:setVisible(false)   
            end
            
            if res.data.guildList[i].state == 1 then
                m_ApplyUnionBtn:setVisible(false)
                m_UnionPeopleFull:setVisible(false)
                m_AlreadyApply:setVisible(true)
            end
            
            local GuildTypeNum = res.data.guildList[i].applyType--标记公会是否可以申请
            local GuildStateNum = res.data.guildList[i].state
            local function ApplyBtnCallBack(sender,event)
                if sender == m_ApplyUnionBtn and event == cc.EventCode.ENDED then
                    Http:req("ApplyGuildRequest",{guildId = res.data.guildList[i].guildId},function(res)
                        if res.data["result"] == 14 then
                            PopDialog:getInstance():popText("您未达到公会加入条件",cc.c3b(0,255,0))
                            m_ApplyUnionBtn:setVisible(true)
                            m_UnionPeopleFull:setVisible(false)
                            m_AlreadyApply:setVisible(false)
                        elseif res.data.result == 7 then 
                            PopDialog:getInstance():popText("当前状态限制加入公会，请耐心等待",cc.c3b(0,255,0))
                            m_ApplyUnionBtn:setVisible(true)
                            m_UnionPeopleFull:setVisible(false)
                            m_AlreadyApply:setVisible(false)
                        elseif res.data.result == 13 then 
                            PopDialog:getInstance():popText("已申请过该公会",cc.c3b(0,255,0))
                            m_ApplyUnionBtn:setVisible(true)
                            m_UnionPeopleFull:setVisible(false)
                            m_AlreadyApply:setVisible(false)
                        elseif res.data.result == 8 then 
                            PopDialog:getInstance():popText("申请次数已满",cc.c3b(0,255,0))
                            m_ApplyUnionBtn:setVisible(true)
                            m_UnionPeopleFull:setVisible(false)
                            m_AlreadyApply:setVisible(false)
                        elseif GuildTypeNum == 2 then 
                            PopDialog:getInstance():popText("该公会不允许任何玩家加入",cc.c3b(0,255,0))
                            m_ApplyUnionBtn:setVisible(true)
                            m_UnionPeopleFull:setVisible(false)
                            m_AlreadyApply:setVisible(false) 
                        elseif res.data["result"] == 0 then
                        --申请成功
                            m_ApplyUnionBtn:setVisible(false)
                            m_UnionPeopleFull:setVisible(false)
                            m_AlreadyApply:setVisible(true)
                            GuildStateNum = 1  
                            self:initUI()    
                        end
                    end)
               end
                res.data.guildList[i].state = 1
            end
            
            m_ApplyUnionBtn:addTouchEventListener(ApplyBtnCallBack)
            
            m_UnionLeaderName:setString(res.data.guildList[i].colonelName)--会长名字
            m_UnionLimitNum:setString(res.data.guildList[i].minMemberLv)--公会等级
            m_UnionLevelNum:setString(res.data.guildList[i].guildLevel)--最低等级限制
            
            UnionItem:setTag(i)
            UnionItem:setPosition(cc.p(21+(i-1)%2*390,ScrovSize.height-190*(math.floor((i-1)/2+1))))
            self.m_ScrollUnionList:addChild(UnionItem)
            
        end
  end)  
end


--查看申请者
function UnionUi:ApplyMemberList()
    Http:req("ApplyGuildRolerListRequest",nil,function(res)
        if table.getn(res.data["applyGuildRolerList"]) == 0 then
                self.m_NoPlayerApply:setVisible(true)
                self.m_ScrollApplyMemberList:setVisible(false)
            else
                self.m_NoPlayerApply:setVisible(false)
                self.m_ScrollApplyMemberList:setVisible(true)
            end
               
        local ApplyMemNumber = table.getn(res.data["applyGuildRolerList"])
            
        self.m_ScrollApplyMemberList:removeAllChildren()
        self.m_ScrollApplyMemberList:setBounceEnabled(true)

        local ScrovSize = self.m_ScrollApplyMemberList:getInnerContainerSize()
        local ScrovH = ScrovSize.height

        if ApplyMemNumber > 4 then
            ScrovH = 105 * ApplyMemNumber
        end

        ScrovSize.height = ScrovH 
        self.m_ScrollApplyMemberList:setInnerContainerSize(ScrovSize)
        
        --在滚动层显示申请公会成员
        for i=1,ApplyMemNumber do

            local MemberItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("ShowUnionMemberInfo.csb")
            local m_MemberId = ccui.Helper:seekWidgetByName(MemberItem, "m_MemberId")
            local m_MemberLV = ccui.Helper:seekWidgetByName(MemberItem, "m_MemberLV")
            local m_LastTimeLogInTitle = ccui.Helper:seekWidgetByName(MemberItem, "m_LastTimeLogIn")
            local m_ShouLoginTime = ccui.Helper:seekWidgetByName(MemberItem, "m_ShouLoginTime")
            local m_touxiangkuang = ccui.Helper:seekWidgetByName(MemberItem, "m_touxiangkuang")
            local m_UnionMemberPosition = ccui.Helper:seekWidgetByName(MemberItem, "m_UnionMemberPosition")
            
            m_UnionMemberPosition:setVisible(false)

            local m_PassBtn = ccui.Helper:seekWidgetByName(MemberItem, "Button_103")
            local m_RefuseBtn = ccui.Helper:seekWidgetByName(MemberItem, "Button_104")
            local m_OperateMemberButton = ccui.Helper:seekWidgetByName(MemberItem, "m_OperateMemberButton")
            
            m_OperateMemberButton:setVisible(false)
            m_UnionMemberPosition:setVisible(false)
            m_PassBtn:setVisible(true)
            m_RefuseBtn:setVisible(true)
            
            m_MemberId:setString(res.data.applyGuildRolerList[i].rolerName)
            m_MemberLV:setString(res.data.applyGuildRolerList[i].rolerLevel)
            m_touxiangkuang:loadTexture("soul/"..res.data.applyGuildRolerList[i].avatarId..".png")
            
            local dateNum = math.floor(((res.data.applyGuildRolerList[i].lastLogin)/1000))
            local dateYear = os.date("%Y",dateNum)
            local dateMonth = os.date("%m",dateNum)
            local dateDay = os.date("%d",dateNum)
            local dateHour = os.date("%H",dateNum)--24小时制
            local dateMinute = os.date("%M",dateNum)
            m_ShouLoginTime:setString(dateYear.."-"..dateMonth.."-"..dateDay.." "..dateHour..":"..dateMinute)

            m_ShouLoginTime:setName("m_ShouLoginTime_"..i)
            
            MemberItem:setPosition(50,ScrovSize.height-103*(i))
            local MemberId2 = res.data.applyGuildRolerList[i].rolerId
            MemberItem:setName("MemberItem_"..MemberId2)
            self.m_ScrollApplyMemberList:addChild(MemberItem)
            
            local function ShowApply2CallBack(sender,event)
                if sender == m_PassBtn and event == cc.EventCode.ENDED then
                   --是否同意申请者加入
                    
                    Http:req("DealGuildApplyRequest",{rolerId = res.data.applyGuildRolerList[i].rolerId,type = 1},function(res)
                          if res.data.result == 6 then
                              PopDialog:getInstance():popText("通过失败，公会人数已满",cc.c3b(0,255,0))
                          elseif res.data.result == 15 then
                              PopDialog:getInstance():popText("申请已过期",cc.c3b(0,255,0))
                          elseif res.data.result == 0 then
                              PopDialog:getInstance():popText("加入新成员成功！",cc.c3b(0,255,0)) 
                          else
                              PopDialog:getInstance():popText("通过失败",cc.c3b(0,255,0))     
                          end
                          self.m_ScrollApplyMemberList:removeChild(MemberItem)
                          
                    end)
                elseif sender == m_RefuseBtn and event == cc.EventCode.ENDED then
                    
                    Http:req("DealGuildApplyRequest",{rolerId = res.data.applyGuildRolerList[i].rolerId,type = 0},function(res)
                        if res.data.result == 0 then
                            PopDialog:getInstance():popText("已拒绝",cc.c3b(0,255,0))
                        end
                    end)
                    self.m_ScrollApplyMemberList:removeChild(MemberItem)
                end
                --self:updataTipPic()
            end

            m_PassBtn:addTouchEventListener(ShowApply2CallBack)
            m_RefuseBtn:addTouchEventListener(ShowApply2CallBack)
            
        end 
     end)
end

function UnionUi:UnionFood(eatNumber)
    local EatNum = eatNumber
    Http:req("EatGuildDinnerRequest",{eatId = EatNum},function(res)
        print(res)
        if res.data.result == 19 then
            PopDialog:getInstance():popText("今天你已经吃过了哦~",cc.c3b(0,255,0))
        else  
            Http:req("GetGuildDinnerRequest",nil,function(rse)
                if rse.data.eatId == 1 then
                    if res.data.result == 0 then
                        PopDialog:getInstance():popText("你品尝了 可乐，获得了 30 点体力，30 点个人贡献",cc.c3b(0,255,0))
                    elseif res.data.result == 3 then
                        PopDialog:getInstance():popText("金币不足！",cc.c3b(0,255,0))   
                    end
                elseif rse.data.eatId == 2 then
                    if res.data.result == 0 then
                        PopDialog:getInstance():popText("你品尝了 汉堡，获得了 60 点体力，60 点个人贡献",cc.c3b(0,255,0))
                    elseif res.data.result == 3 then
                        PopDialog:getInstance():popText("钻石不足，请充值！",cc.c3b(0,255,0))   
                    end
                elseif rse.data.eatId == 3 then
                    if res.data.result == 0 then
                        PopDialog:getInstance():popText("你品尝了 烤鸡，获得了 150 点体力，100 点个人贡献",cc.c3b(0,255,0))
                        self.m_UnionFoodNotice:setString(rse.data.bestEator.."今天第一个分享了大餐，大家一起去品尝吧~")
                    elseif res.data.result == 3 then
                        PopDialog:getInstance():popText("钻石不足，请充值！",cc.c3b(0,255,0))   
                    end
                end
            end)   
        end
    end)
end

function UnionUi:UnionShopFace()
    Http:req("GetGuildShopRequest",nil,function(res)
        self.m_MemberGongXianNum:setString(res.data.guildDevote)
    
        local ItemNumber = table.getn(res.data.item)
        if self.m_GoodsScrollView:getChildrenCount() ~= 0 then 
            self.m_GoodsScrollView:removeAllChildren()
            self.m_GoodsScrollView:setBounceEnabled(true)
        end    
        
        local ScrovSize = self.m_GoodsScrollView:getInnerContainerSize()
        local ScrovW = ScrovSize.width
            if ItemNumber > 3 then
                ScrovW = 227*ItemNumber
            end 
            ScrovSize.width = ScrovW 
            self.m_GoodsScrollView:setInnerContainerSize(ScrovSize)
        --end
        
        --滚动层显示商品列表
        for i_2=1,ItemNumber do
            local UnionShopItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("UnionShopItem.csb")
            local m_touxiang = ccui.Helper:seekWidgetByName(UnionShopItem, "m_touxiang")
            local m_ItemName = ccui.Helper:seekWidgetByName(UnionShopItem, "m_ItemName")
            local m_OwnNum = ccui.Helper:seekWidgetByName(UnionShopItem, "m_OwnNum")
            local m_PurchaseNum = ccui.Helper:seekWidgetByName(UnionShopItem, "m_PurchaseNum")
            local m_ItemPrice = ccui.Helper:seekWidgetByName(UnionShopItem, "m_ItemPrice")
            local m_BuyBtn = ccui.Helper:seekWidgetByName(UnionShopItem, "m_BuyBtn")
            local m_ItemCover = ccui.Helper:seekWidgetByName(UnionShopItem, "Panel_15")
            
            --判断是否已购买
            if res.data.item[i_2].leftNum == 0 then
                m_ItemCover:setVisible(true)
            else
                m_ItemCover:setVisible(false)    
            end
            
            local ItemID = UnionShopConfig_1[tostring(i_2)]["item_id"]
            
            m_touxiang:loadTexture(shopConfig[tostring(ItemID)]["icon"]..".png")
            m_ItemName:setString(shopConfig[tostring(ItemID)]["name"])
            m_PurchaseNum:setString(UnionShopConfig_1[tostring(i_2)]["per_num"])
            m_ItemPrice:setString(UnionShopConfig_1[tostring(i_2)]["per_cost"])
            m_OwnNum:setString(res.data.item[i_2].ownNum)--已拥有数量
            UnionShopItem:setPosition(ScrovSize.width-227*(i_2),4)
            UnionShopItem:setTag(i_2)
            self.m_GoodsScrollView:addChild(UnionShopItem)
            local function ShopBuy2CallBack(sender,event)
                if sender == m_BuyBtn and event == cc.EventCode.ENDED then
                    self.m_Sure2Change:setVisible(true)
                    self.m_IName:setString(shopConfig[tostring(ItemID)]["name"])
                    self.m_PNum:setString(UnionShopConfig_1[tostring(i_2)]["per_cost"])
                    
                    local function ShopBuy2CallBack_1(sender,event)
                         if sender == self.m_Cancel4 and event == cc.EventCode.ENDED then  
                            self.m_Sure2Change:setVisible(false)
                            print(res.data.item[i_2].id)
                         elseif sender == self.m_confirm4 and event == cc.EventCode.ENDED then 
                            Http:req("BuyGuildShopRequest",{id = res.data.item[i_2].id },function(re)
                                if re.data.result == 0 then
                                    m_OwnNum:setString(res.data.item[i_2].ownNum + UnionShopConfig_1[tostring(i_2)]["per_num"])--买后加数量
                                    Http:req("GetGuildShopRequest",nil,function(rre)
                                        self.m_MemberGongXianNum:setString(rre.data.guildDevote)--买后减钱币
                                    end)
                                    m_ItemCover:setVisible(true)
                                elseif re.data.result == 3 then
                                    PopDialog:getInstance():popText("贡献币不足",cc.c3b(0,255,0))    
                                end
                            end) 
                            self.m_Sure2Change:setVisible(false)
                         end
                    end
                    self.m_Cancel4:addTouchEventListener(ShopBuy2CallBack_1)
                    self.m_confirm4:addTouchEventListener(ShopBuy2CallBack_1)
                end 
            end    
            m_BuyBtn:addTouchEventListener(ShopBuy2CallBack)
        end        
        
    end)
end

--仓库实现
function UnionUi:UnionWareHouse()
    cclog("公会仓库")
    Http:req("GetGuildBagRequest",nil,function(res)

            local HouseItemNumber = table.getn(res.data.item)
            
            if self.m_CangkuListView:getChildrenCount() ~= 0 then 
                self.m_CangkuListView:removeAllChildren()
                self.m_CangkuListView:setBounceEnabled(true)
            end  
            
            local ScrovSize = self.m_CangkuListView:getInnerContainerSize()
            
            --列表层显示仓库物品列表
            for i=1,HouseItemNumber do
                local HouseItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("UnionCangKuItem.csb")

                HouseItem:setTag(i) 
                local m_touxiangK = ccui.Helper:seekWidgetByName(HouseItem, "m_touxiangK")
                local m_ItemName = ccui.Helper:seekWidgetByName(HouseItem, "m_ItemName")
                local m_PaimingNum = ccui.Helper:seekWidgetByName(HouseItem, "m_PaimingNum")
                local m_ItemNumber = ccui.Helper:seekWidgetByName(HouseItem, "m_ItemNumber")
                local m_DetailBtn = ccui.Helper:seekWidgetByName(HouseItem, "m_DetailBtn")
                local m_YesBtn = ccui.Helper:seekWidgetByName(HouseItem, "m_YesBtn")
                local m_NoBtn = ccui.Helper:seekWidgetByName(HouseItem, "m_NoBtn")
                local m_touxiangKK = ccui.Helper:seekWidgetByName(HouseItem, "m_touxiangKK")
                local m_ShowApplyLable = ccui.Helper:seekWidgetByName(HouseItem, "m_ShowApplyLable")
                local m_ExchangePrice = ccui.Helper:seekWidgetByName(HouseItem, "m_ExchangePrice")
               
                m_touxiangK:loadTexture(shopConfig[tostring(res.data.item[i].itemId)]["icon"]..".png")
                m_touxiangK:setScale(0.7,0.7)
                m_touxiangKK:setScale(0.8,0.8)
                
                m_ItemName:setString(shopConfig[tostring(res.data.item[i].itemId)]["name"])
                m_PaimingNum:setString(res.data.item[i].applyNo)
                
                --[[tostring(res.data.item[i].itemId)]]  --服务端更新后替换
                m_ExchangePrice:setString(UnionBagItemConfig["5101"]["score"])

                m_ItemNumber:setString(res.data.item[i].itemNum)

                self.m_CangkuListView:pushBackCustomItem(HouseItem)
                
                if res.data.item[i].applyed == 0 then--未申请
                    m_YesBtn:setVisible(false)
                    m_NoBtn:setVisible(true)
                    m_ShowApplyLable:setVisible(false)
                    m_PaimingNum:setVisible(false)
                    
                elseif res.data.item[i].applyed == 1 then --已申请
                    m_YesBtn:setVisible(true)
                    m_NoBtn:setVisible(false)
                    m_ShowApplyLable:setVisible(true)
                    m_PaimingNum:setVisible(true)
                    
                    IsApply = IsApply + 1
                    
                    --二次确认框内容
                    self.m_AlreadyApplyItem:setString(shopConfig[tostring(res.data.item[i].itemId)]["name"]) 
                    self.m_AlreadyApplyItem_2:setString(shopConfig[tostring(res.data.item[i].itemId)]["name"])     
                end

                local function WareHouse2CallBack(sender,event)--排队操作按钮
                    if sender == m_DetailBtn and event == cc.EventCode.ENDED then
                        cclog("物品详情")
                        self.m_CoverPanel:setVisible(true)
                        self.m_cangkuApplyMember:setVisible(true)
                        self.m_ShowUnionCangKu:setVisible(false)
                        
                        if res.data.item[i].applyed == 1 then
                            self.m_yesApply:setVisible(true)
                            self.m_notApply:setVisible(false)
                            self.m_Weidangqian:setVisible(false)
                            
                            self.m_dangqian1:setVisible(true)
                            self.m_dangqian2:setVisible(true)
                            self.m_dangqianNum:setVisible(true)
                            
                            self.m_dangqianNum:setString(res.data.item[i].applyNo)
                            
                            self.m_Itemtouxiang:loadTexture(shopConfig[tostring(res.data.item[i].itemId)]["icon"]..".png")
                            self.m_waitDistributionNum:setString(res.data.item[i].itemNum)
                        elseif  res.data.item[i].applyed == 0 then 
                            self.m_yesApply:setVisible(false)
                            self.m_notApply:setVisible(true)
                            self.m_Weidangqian:setVisible(true)

                            self.m_dangqian1:setVisible(false)
                            self.m_dangqian2:setVisible(false)
                            self.m_dangqianNum:setVisible(false) 
                            self.m_Itemtouxiang:loadTexture(shopConfig[tostring(res.data.item[i].itemId)]["icon"]..".png")
                            self.m_waitDistributionNum:setString(res.data.item[i].itemNum)
                        end
                        
                        self:DetailGuildBag(res.data.item[i].itemId)
                        
                    elseif sender == m_YesBtn and event == cc.EventCode.ENDED then
                        cclog("already")
                        
                    --申请按钮    
                    elseif sender == m_NoBtn and event == cc.EventCode.ENDED then
                        if self.IsApply == 0 then--从未申请过
                        self:ApplyGuildBag(res.data.item[i].itemId,m_YesBtn,m_NoBtn)
                           
                        else --之前申请过
                            self.m_donotApplyItem:setString(shopConfig[tostring(res.data.item[i].itemId)]["name"])
                            self.m_sure2Cangku:setVisible(true) 
                            
                            local function WareHouse2CallBack_2(sender,event)
                                if sender == self.m_confirm10 and event == cc.EventCode.ENDED then
                                    self:ApplyGuildBag(res.data.item[i].itemId,m_YesBtn,m_NoBtn)
                                        
                                    self.m_sure2Cangku:setVisible(false)
                                end
                            end
                            self.m_confirm10:addTouchEventListener(WareHouse2CallBack_2)
                        end
                        
                    end
                end 
                   
                m_DetailBtn:addTouchEventListener(WareHouse2CallBack)
                m_YesBtn:addTouchEventListener(WareHouse2CallBack)
                m_NoBtn:addTouchEventListener(WareHouse2CallBack)
                
            end 
    end)
    
end

--申请公会物品
function UnionUi:ApplyGuildBag(itemIdNum,Btn1,Btn2)
    Http:req("ApplyGuildBagItemRequest",{itemId = itemIdNum},function(res_1)
        if res_1.data.result == 0 then
            PopDialog:getInstance():popText("申请成功！请等待发放！",cc.c3b(0,255,0))
            self.IndexId = 0
            
            if self.IndexId == 0 then
                Btn1:setVisible(true)
                Btn2:setVisible(false)
            elseif self.IndexId == 1 then
                Btn1:setVisible(false)
                Btn2:setVisible(true)    
            end
            
            --刷新公会仓库
            self:UnionWareHouse()
        elseif res_1.data.result == 25 then 
            PopDialog:getInstance():popText("您的积分不足，暂不能申请",cc.c3b(0,255,0))
            self.IndexId = 1 
            
            if self.IndexId == 0 then
                Btn1:setVisible(true)
                Btn2:setVisible(false)
            elseif self.IndexId == 1 then
                Btn1:setVisible(false)
                Btn2:setVisible(true)    
            end
              
        end
    end)
    
    
     
end

--公会仓库物品详情
function UnionUi:DetailGuildBag(itemIdNum)
    Http:req("DetailGuildBagItemRequest",{itemId = itemIdNum},function(res)
        local CangkuApplyNumber = table.getn(res.data.detail)

        if self.m_BagDetailListView:getChildrenCount() ~= 0 then 
            self.m_BagDetailListView:removeAllChildren()
            self.m_BagDetailListView:setBounceEnabled(true)
        end  

        local ScrovSize = self.m_BagDetailListView:getInnerContainerSize()
        
        for i=1,CangkuApplyNumber do
            local cangkuApplyMember = ccs.GUIReader:getInstance():widgetFromBinaryFile("ApplyCangkuMember.csb")
            
            local m_memberTouxiang = ccui.Helper:seekWidgetByName(cangkuApplyMember, "m_memberTouxiang")
            local m_memberLvNum = ccui.Helper:seekWidgetByName(cangkuApplyMember, "m_memberLvNum")
            local m_memberApplyName = ccui.Helper:seekWidgetByName(cangkuApplyMember, "m_memberApplyName")
            local m_paimingNum = ccui.Helper:seekWidgetByName(cangkuApplyMember, "m_shuzi")
            
            
            m_memberTouxiang:loadTexture("soul/"..res.data.detail[i].icon..".png")
            m_memberLvNum:setString(res.data.detail[i].level)
            m_memberApplyName:setString(res.data.detail[i].name)
            
            m_paimingNum:setString(i)
            
            
            self.m_BagDetailListView:pushBackCustomItem(cangkuApplyMember)
        end
        
    end)
end

--公会BOSS一级界面实现
function UnionUi:UnionBossInterface()
    Http:req("GetGuildBigStageListRequest",nil,function(res)
        local StageNum = table.getn(res.data.info)
        for i=1,StageNum do
            local StageItem = ccui.Helper:seekWidgetByName(self.m_BossScrollView, "m_Stage"..i)
            StageItem:setTag(i)
            local m_big_progress = ccui.Helper:seekWidgetByName(StageItem, "m_Stage_progress")
            local m_Stage_Name = ccui.Helper:seekWidgetByName(StageItem, "m_Stage_Name")
            local m_wu = ccui.Helper:seekWidgetByName(StageItem, "m_wu")
            
            m_big_progress:setPercent(res.data.info[i].pro)
            m_Stage_Name:setString(BigStageConfig[tostring(res.data.info[i].id)]["name"])
            
            --关卡未开放
            if res.data.info[i].state == 0 then
                m_wu:setVisible(true)
            --关卡已开放  
            elseif res.data.info[i].state == 1 or res.data.info[i].state == 2 then
                if res.data.info[i].id >=2 then
                    m_wu:setVisible(false)
                end
            end 
            
            local function AttackStage2CallBack(sender,event)
                if sender == StageItem and event == cc.EventCode.ENDED then
                    --关卡未开启或者进度打满
                    if res.data.info[i].state == 1 or res.data.info[i].pro == 100 then
                        Http:req("GuildInfoRequest",nil,function(fuc)
                            if fuc.data.guildInfo.guildPos == 1 or fuc.data.guildInfo.guildPos == 2 then--会长或者长老拥有该权限
                                --二次确认是否开启
                                self.m_Sure2OpenStage:setVisible(true)
                                --开启资金
                                self.m_OpenMoney:setString(BigStageConfig[tostring(res.data.info[i].id)]["open_fund"])
                                
                                local function AttackStage2CallBack_2(sender,event)
                                    if sender == self.m_confirm9 and event == cc.EventCode.ENDED then 
                                        Http:req("OpenGuildBigStageRequest",{id = res.data.info[i].id},function(re)
                                            if re.data.result == 0 then

                                                --刷新大关卡
                                                self:UnionBossInterface()

                                                self.m_Sure2OpenStage:setVisible(false)

                                            elseif re.data.result == 24 then
                                                PopDialog:getInstance():popText("工会资金不足",cc.c3b(0,255,0))
                                                self.m_Sure2OpenStage:setVisible(false)       
                                            end
                                        end)

                                    elseif sender == self.m_Cancel9 and event == cc.EventCode.ENDED then 
                                        self.m_Sure2OpenStage:setVisible(false)   
                                    end
                                end

                                self.m_Cancel9:addTouchEventListener(AttackStage2CallBack_2)
                                self.m_confirm9:addTouchEventListener(AttackStage2CallBack_2)

                            elseif fuc.data.guildInfo.guildPos == 3 then
                                 PopDialog:getInstance():popText("关卡未开启！",cc.c3b(0,255,0))    
                            end
                        end)    
                  elseif res.data.info[i].state == 2 then 

                     --进入小boss界面
                     self:SmallBossInterface(res.data.info[i].id)

                     self.m_ShowSmallBoss:setVisible(true)
                     self.m_CloseSmallBtn:setVisible(true)

                     self.m_CloseBossBtn:setVisible(false)
                     self.m_ShowUnionBoss:setVisible(false)        
                  end
                    
              end 
                
           end
           StageItem:addTouchEventListener(AttackStage2CallBack)  
        end
    end)
end

--公会Boss二级界面实现(小boss)
function UnionUi:SmallBossInterface(BigBossId)
    cclog("enter MY BOSS")
    local SmallStageId = BigBossId
    self.IndexBossID = BigBossId
    
    Http:req("GetGuildStageRequest",{id = SmallStageId},function(res)
    for i = 1,7 do
        local SmallStageItem = ccui.Helper:seekWidgetByName(self.m_GotoBoss, "m_boss"..i)
        SmallStageItem:setTag(i)
        local m_boss_progress = ccui.Helper:seekWidgetByName(SmallStageItem, "m_boss_progress")
        local m_boss_name = ccui.Helper:seekWidgetByName(SmallStageItem, "m_boss_name")
        local m_cover = ccui.Helper:seekWidgetByName(SmallStageItem, "m_cover")
        local m_boss_pro = ccui.Helper:seekWidgetByName(SmallStageItem, "m_boss_pro")
        
        local SmallStageIdNum = (SmallStageId-1)*7+i
        
        m_boss_name:setString(SmallStageConfig[tostring(SmallStageIdNum)]["stage_name"])
        
        
        --Http:req("GetGuildStageRequest",{id = SmallStageId},function(res)
            if SmallStageIdNum == res.data.stageId then
                m_boss_progress:setPercent(res.data.pro)
                m_cover:setVisible(false)
                m_boss_pro:setVisible(true)
            elseif SmallStageIdNum < res.data.stageId then
                m_boss_progress:setPercent(100)
                m_cover:setVisible(true)
                m_boss_pro:setVisible(false)
            elseif SmallStageIdNum > res.data.stageId then 
                m_boss_progress:setPercent(0)
                m_cover:setVisible(true)
                m_boss_pro:setVisible(false)   
            end
            
            local function StartFight2CallBack(sender,event)
                if sender == SmallStageItem and event == cc.EventCode.ENDED then
                    self.m_prepare2Battle:setVisible(true)
                    self.m_StageDesc:setString(SmallStageConfig[tostring(res.data.stageId)]["desc"])
                    self.m_StageName:setString(SmallStageConfig[tostring(SmallStageIdNum)]["stage_name"])
                    --显示掉落物品icon
                    self:ItemPicture(SmallStageConfig[tostring(res.data.stageId)]["item"])
                    
                    if res.data.atkName == nil then
                        self.m_TeammateInBattle:setVisible(false)
                        self.m_GoBattleBtn:setEnabled(true)
                    elseif res.data.atkName ~= nil then  
                        self.m_TeammateInBattle:setVisible(true)
                        self.m_teammateName:setString(res.data.atkName)
                        self.m_GoBattleBtn:setEnabled(false)  
                    end
                    
                    local function StartFight2CallBack_2(sender,event)
                        if sender == self.m_GoBattleBtn and event == cc.EventCode.ENDED then
                            self:Go2Battle(res.data.stageId)
                        end
                    end
                    self.m_GoBattleBtn:addTouchEventListener(StartFight2CallBack_2)
                elseif sender == m_cover and event == cc.EventCode.ENDED then 
                    cclog("cover")   
                end
            end

            SmallStageItem:addTouchEventListener(StartFight2CallBack)
            
        --end)
        
    end
    end)
end

--跳转到布阵界面
function UnionUi:Go2Battle(StageItemId)
    
    local StageIDNum = StageItemId
    
    local FormationUILayer =  require "scene.formation.FormationUILayer"
    
    local data = {stageId = StageIDNum,matrix ={}}
    local Formation = require "data.Formation"
    Formation:set("battledata",data)
    
    --FormationUILayer:setBeforeSceneType("guild")
    
    local scene = require("scene.formation.FormationScene"):create({BattleType="guild"})
    cc.Director:getInstance():pushScene(scene)
end

--显示掉落物品icon函数
function UnionUi:ItemPicture(lable)
    local input = tostring(lable)
    local res = string.split(input, ";")
    
    local LableLen = table.getn(res)
    for i = 1,LableLen do
        local arr = res[i]
        local res_2 = string.split(arr, ":")
        local m_goods = ccui.Helper:seekWidgetByName(self.m_prepare2Battle, "m_goods"..i)
        m_goods:loadTexture(shopConfig[tostring(res_2[1])]["icon"]..".png")
    end
end

function UnionUi:updataTipPic()
     Http:req("GetRedDot",{multType = "128"},function(rs)
            if rs.data.dot[1].visible == 0 then
                self.m_redDot:setVisible(false)
            else
                self.m_redDot:setVisible(true)
            end 
     end)
end

return UnionUi
