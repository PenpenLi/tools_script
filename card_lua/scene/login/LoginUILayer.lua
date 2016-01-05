--LoginUILayer.lua, created by php script. chris.li--
require "Cocos2d"

local PopDialog = require "common/PopDialog"

local LoginUILayer = class("LoginUILayer", function()
	return cc.Layer:create()
end)

function LoginUILayer:create()
	local layer = LoginUILayer.new()
	layer:initM()
	return layer
end

function LoginUILayer:ctor()
end

function LoginUILayer:onEnter()
end

function LoginUILayer:onExit()
end

function LoginUILayer:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)
	
    if Roler.get("updateTip") ~= nil and Roler.get("updateTip") ~= "" then
        self:addNoticeLayer(Roler.get("updateTip"))
    end

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("LoginUILayer.csb")
	self:addChild(self.rootNode)
    
	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end
	
	--m_createRoleLayout: Layout
	self.m_createRoleLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_createRoleLayout")
	self.m_createRoleLayout:addTouchEventListener(touchEventHandler)

	--m_LoginLayout: Layout
	self.m_LoginLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_LoginLayout")
	self.m_LoginLayout:addTouchEventListener(touchEventHandler)

	--m_createBtn: Button
	self.m_createBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_createBtn")
	self.m_createBtn:addTouchEventListener(touchEventHandler)

	--m_selectRoleImage: ImageView
	self.m_selectRoleImage = ccui.Helper:seekWidgetByName(self.rootNode, "m_selectRoleImage")

	--m_rolemanBtn: Button
	self.m_rolemanBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_rolemanBtn")
	self.m_rolemanBtn:addTouchEventListener(touchEventHandler)

	--m_rolewomanBtn: Button
	self.m_rolewomanBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_rolewomanBtn")
	self.m_rolewomanBtn:addTouchEventListener(touchEventHandler)

	--m_randBtn: Button
	self.m_randBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_randBtn")
	self.m_randBtn:addTouchEventListener(touchEventHandler)

	--m_roleNameTextfield: TextField
    self.m_roleNameTextfield = ccui.Helper:seekWidgetByName(self.rootNode, "m_roleNameTextfield")
    self.m_roleNameTextfield:addTouchEventListener(touchEventHandler)

    --m_loginBtn: Button
    self.m_loginBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_loginBtn")
    self.m_loginBtn:addTouchEventListener(touchEventHandler)

    --m_registerBtn: Button
    self.m_registerBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_registerBtn")
    self.m_registerBtn:addTouchEventListener(touchEventHandler)

    --m_userNameTextField: TextField
    self.m_userNameTextField = ccui.Helper:seekWidgetByName(self.rootNode, "m_userNameTextField")
    self.m_userNameTextField:addTouchEventListener(touchEventHandler)

    --m_userPasswordTextField: TextField
    self.m_userPasswordTextField = ccui.Helper:seekWidgetByName(self.rootNode, "m_userPasswordTextField")
    self.m_userPasswordTextField:addTouchEventListener(touchEventHandler)
    
    --m_userPasswordLayout: Layout
    self.m_userPasswordLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_userPasswordLayout")
    self.m_userPasswordLayout:addTouchEventListener(touchEventHandler)
    
    --m_userNameLayout
    self.m_userNameLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_userNameLayout")
    self.m_userNameLayout:addTouchEventListener(touchEventHandler)
    
    --server
    --m_ChooseServerLayout
    self.m_ChooseServerLayout = ccui.Helper:seekWidgetByName(self.rootNode, "m_ChooseServerLayout")
    
    --m_DefaultServerPanels
    self.m_DefaultServerPanels = ccui.Helper:seekWidgetByName(self.rootNode, "m_DefaultServerPanels")
    
    --m_DefaultZoneNumber
    self.m_DefaultZoneNumber = ccui.Helper:seekWidgetByName(self.m_DefaultServerPanels, "m_DefaultZoneNumber")
    
    --m_DefaultZoneName
    self.m_DefaultZoneName = ccui.Helper:seekWidgetByName(self.m_DefaultServerPanels, "m_DefaultZoneName")
    
    --m_DefaultZoneName
    self.m_clickTagLabel = ccui.Helper:seekWidgetByName(self.m_DefaultServerPanels, "m_clickTagLabel")
    
    --m_enterServerBtn
    self.m_enterServerBtn = ccui.Helper:seekWidgetByName(self.m_DefaultServerPanels, "m_enterServerBtn")
    self.m_enterServerBtn:addTouchEventListener(touchEventHandler)
    
    --m_chooseServerImage
    self.m_chooseServerImage = ccui.Helper:seekWidgetByName(self.m_DefaultServerPanels, "m_chooseServerImage")
    self.m_chooseServerImage:addTouchEventListener(touchEventHandler)
    
    --m_ServerListPanel
    self.m_ServerListPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_ServerListPanel")
    
    --m_serverListView
    self.m_serverListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_serverListView")
    
    --m_serverConfirmBtn
    self.m_serverConfirmBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_serverConfirmBtn")
    self.m_serverConfirmBtn:addTouchEventListener(touchEventHandler)
    
    --m_serverListDefault
    self.m_serverListDefault = ccui.Helper:seekWidgetByName(self.rootNode, "m_serverListDefault")
    
	self:initUI()
	
	---test guard
	--local guardLayer = require("common.Guard")
    --self:addChild(guardLayer:create({type="guard_click", clickTarget=self.m_loginBtn}))
    --
    
    --self:addChild(dialog)
    --[[
    Helper.setTimeout(self, 5, function()
        self.dialog = require("common.ClickDialog"):create({name="重新发送", tip="网络连接超时", cb=function()
            cc.Director:getInstance():getRunningScene():removeChild(self.dialog) 
            end})
        self.dialog:setLocalZOrder(10)
        cc.Director:getInstance():getRunningScene():addChild(self.dialog)
    end)
    ]]
end

function LoginUILayer:handleTouchEvent(sender, event)
    local visibleSize=cc.Director:getInstance():getVisibleSize()
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_createRoleLayout and event == cc.EventCode.ENDED then
		cclog("[LoginUILayer]--m_createRoleLayout touched--")

	elseif sender == self.m_LoginLayout and event == cc.EventCode.ENDED then
		cclog("[LoginUILayer]--m_LoginLayout touched--")

	elseif sender == self.m_createBtn and event == cc.EventCode.ENDED then
		cclog("[LoginUILayer]--m_createBtn touched--")
        print("m_roleNameTextfield:"..self.m_roleNameTextfield:getStringValue())
        if (string.gsub(self.m_roleNameTextfield:getStringValue(),"^%s*(.-)%s*$","%1")~= "") then 
            Http:req("CreateRole", 
                {nickname = string.gsub(self.m_roleNameTextfield:getStringValue(),"^%s*(.-)%s*$","%1"),
                 roleIndex = self.m_roleIndex+1,
                 avatarId = self.m_roleIndex*2+1,
                 sex = self.m_roleIndex+1
                 },
                function(res) 
                    if res.err then cclog("err!!!") return end
                    self.m_result=res.data["result"]
                    if self:checkResult() then
                        Http: req("GetRoler",{roleId = res.data["roleLogin"]["roleId"]},
                                function(res) if res.err then return end
                                local scene = require("scene.load1.Load1Scene"):create()
                                cc.Director:getInstance():replaceScene(scene) 
                        end)                     
                    end 
            end)
        else 
            PopDialog:getInstance():popText("昵称不能为空",cc.c3b(0,255,0))
        end

	elseif sender == self.m_rolemanBtn and event == cc.EventCode.ENDED then
		cclog("[LoginUILayer]--m_rolemanBtn touched--")
        self.m_rolemanBtn:setTouchEnabled(false)
        self.m_rolewomanBtn:setTouchEnabled(true)
        self.m_roleIndex=0
        --self.m_selectRoleImage:loadTexture("select0.png")
        local btnRender1 = self.m_rolemanBtn:getVirtualRenderer()
        btnRender1:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("select_man1.png"))
        local btnRender2 = self.m_rolewomanBtn:getVirtualRenderer()
        btnRender2:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("select_women2.png"))
        self.m_selectRoleImage:setPosition(0, self.m_selectRoleImage:getPositionY())
        local sequence=cc.Sequence:create(cc.CallFunc:create(self.startMotionStreakCallBack),
                                          cc.MoveTo:create(0.5,cc.p(visibleSize.width/2,visibleSize.height/2)),
                                          cc.CallFunc:create(self.stopMotionStreakCallBack)
                                          )
        self.m_selectRoleImage:runAction(sequence)
        
	elseif sender == self.m_rolewomanBtn and event == cc.EventCode.ENDED then
		cclog("[LoginUILayer]--m_rolewomanBtn touched--")
        self.m_rolewomanBtn:setTouchEnabled(false)
        self.m_rolemanBtn:setTouchEnabled(true)
        self.m_roleIndex=1
        --self.m_selectRoleImage:loadTexture("select1.png")
        local btnRender1 = self.m_rolemanBtn:getVirtualRenderer()
        btnRender1:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("select_man2.png"))
        local btnRender2 = self.m_rolewomanBtn:getVirtualRenderer()
        btnRender2:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("select_women1.png"))
        self.m_selectRoleImage:setPosition(0, self.m_selectRoleImage:getPositionY())
        self.m_selectRoleImage:runAction(cc.MoveTo:create(0.3,cc.p(visibleSize.width/2,visibleSize.height/2)))

	elseif sender == self.m_randBtn and event == cc.EventCode.ENDED then
		cclog("[LoginUILayer]--m_randBtn touched--")
		--[[
		Http:req("RandomName", nil, function(res) 
		  if res.err then cclog("err!!!") return end
		  self.m_roleNameTextfield:setText(res.data["user"])
		end)
		]]
        local randName = self:randomName()
        if randName then
            self.m_roleNameTextfield:setText(randName)
        end
		 
    elseif sender == self.m_roleNameTextfield and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_roleNameTextfield touched--")
        self.textType = "roleName"
    elseif sender == self.m_chooseServerImage and event == cc.EventCode.ENDED then
        self.m_DefaultServerPanels:setVisible(false)
        self.m_ServerListPanel:setVisible(true)
    
    elseif sender == self.m_serverConfirmBtn and event == cc.EventCode.ENDED then
        self.m_DefaultServerPanels:setVisible(true)
        self.m_ServerListPanel:setVisible(false)
       if   self.lastServerListSelected > 0 then
            cc.UserDefault:getInstance():setIntegerForKey("DefaultServerNo",self.lastServerListSelected)
            self:UpdateDefaultServer()
       end
        
    elseif sender == self.m_enterServerBtn and event == cc.EventCode.ENDED then
        if  self.lastServerListSelected > 0 then
            local urlState = nil
            for i=1, table.getn(self.serverUrlList) do
                if self.serverUrlList[i]["codeid"] ==  self.lastServerListSelected then
                    urlState = self.serverUrlList[i]["state"]
                end
            end
            if  urlState == "维护中" then
                PopDialog:getInstance():popText("服务器维护中",cc.c3b(0,255,0))
                return
            end
        else
            PopDialog:getInstance():popText("请选择服务器",cc.c3b(0,255,0))
            return
        end
        
        if (string.gsub(self.m_userNameTextField: getStringValue(),"^%s*(.-)%s*$","%1")~= "") then 
            Http:req("Login", 
                {user = string.gsub(self.m_userNameTextField: getStringValue(),"^%s*(.-)%s*$","%1"),
                    key = self.m_userPasswordTextField:getStringValue(),
                    version = "1.0.1"
                },
                function(res) 
                    if res.err then cclog("err!!!") return end
                    self.m_result=res.data["result"] 
                    if self:checkResult() then
                        local str = string.gsub(self.m_userNameTextField: getStringValue(),"^%s*(.-)%s*$","%1")
                        cc.UserDefault:getInstance():setStringForKey("userName_Login",str)

                        local player = require("data.Player")
                        if player:get("role1") >0 or player:get("role2")>0 or player:get("role3")>0 or player:get("role4")>0 then
                            Http:req("RoleList",
                                nil, 
                                function(res) if res.err then cclog("err!!!") return end
                                    Http:req("GetRoler",
                                        {roleId = res.data["roleLogin"][1]["roleId"]}, 
                                        function(res) if res.err then cclog("err!!!") return end
                                            local scene = require("scene.load1.Load1Scene"):create()
                                            cc.Director: getInstance():replaceScene(scene)
                                        end)
                                end)                 
                        else 
                            self.m_createRoleLayout: setVisible(true) 
                            self.m_LoginLayout:setVisible(false)
                            self.m_ChooseServerLayout:setVisible(false)
                        end

                    end 
                end)
        else 
        --self.m_tipsLabel:setVisible(true)
        end
    
    elseif sender == self.m_loginBtn and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_loginBtn touched--")
        Http:req("PtLogin",{user = self.m_userNameTextField:getStringValue(),pwd = self.m_userPasswordTextField:getStringValue()
        },function(res)
            if res.data["result"] == 0 then
                self:UpdateServerList(res.data["server"])
            elseif res.data["result"] == 1 then
                PopDialog:getInstance():popText("登录用户名或密码错误",cc.c3b(0,255,0))
            elseif res.data["result"] == 2 then
                PopDialog:getInstance():popText("登录未知错误",cc.c3b(0,255,0))
            end
        end)
        
        
        
        
    elseif sender == self.m_registerBtn and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_registerBtn touched--")

    elseif sender == self.m_userNameTextField and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_userNameTextField touched--")
        self.textType = "userName"
        
    elseif sender == self.m_userPasswordTextField and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_userPasswordTextField touched--")
        self.textType = "userPassword"
   elseif sender == self.m_userPasswordLayout and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_userPasswordLayout touched--")
        self.textType = "userPassword"
        self.m_userPasswordTextField:setAttachWithIME(true)
        
    elseif sender == self.m_userNameLayout and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_userNameLayout touched--")
        self.textType = "userName"
        self.m_userNameTextField:setAttachWithIME(true)
    end
end

--init your ui here
function LoginUILayer:initUI()
    local armatureFile1 = "image/ui/login/LoadBg/LoadBg.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile1)
    local bg = ccs.Armature:create("LoadBg")
    bg:getAnimation():play("Animation1")
    bg:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/2)
    self:addChild(bg,-1)
    ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile1)
    
    
    bg:setScaleX(WIN_SIZE.width/960)
    bg:setScaleY(WIN_SIZE.height/640)
    
    
    local lizi = cc.Node:create()
    local ParticleSystemQuad = cc.ParticleSystemQuad:create("image/ui/login/1.plist")
    lizi:setPosition(WIN_SIZE.width/2, -WIN_SIZE.height/2)
    lizi:addChild(ParticleSystemQuad)
    self:addChild(lizi)
    
    self.m_roleIndex = 0
    self.m_result = -1
    self.m_createRoleLayout:setVisible(false)
    self.m_LoginLayout:setVisible(true)
    self.m_rolemanBtn:setTouchEnabled(false)
    
    Helper.addSpriteFrames("LoginUI0.plist","LoginUI0.png")
    cc.Texture2D:setDefaultAlphaPixelFormat(cc.TEXTURE2_D_PIXEL_FORMAT_AUTO)
    local btnRender1 = self.m_rolemanBtn:getVirtualRenderer()
    btnRender1:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("select_man1.png"))
    local btnRender2 = self.m_rolewomanBtn:getVirtualRenderer()
    btnRender2:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("select_women2.png"))
    self.m_selectRoleImage:setPosition(0, self.m_selectRoleImage:getPositionY())
    --self.m_selectRoleImage:loadTexture("select0.png")
    
    local savedUserName = cc.UserDefault:getInstance():getStringForKey("userName_Login")
    if  savedUserName then self.m_userNameTextField:setText(savedUserName)  end
    
    local function onkeyPressed(keycode, event)  
        if keycode == cc.KeyCode.KEY_BACKSPACE then
            if  self.textType == "userName" then 
                local str = self.m_userNameTextField:getStringValue()  
                str = string.sub( str, 0, string.len( str ) - 1 )  
                self.m_userNameTextField:setText( str )
            elseif  self.textType == "userPassword" then 
                local str = self.m_userPasswordTextField:getStringValue()
                str = string.sub( str, 0, string.len( str ) - 1 )  
                self.m_userPasswordTextField:setText( str )
            elseif  self.textType == "roleName" then 
                local str = self.m_roleNameTextfield:getStringValue()  
                str = string.sub( str, 0, string.len( str ) - 1 )  
                self.m_roleNameTextfield:setText( str )    
            end          
        end  
    end  
    local keyListener = cc.EventListenerKeyboard:create()  
    keyListener:registerScriptHandler(onkeyPressed,cc.Handler.EVENT_KEYBOARD_PRESSED)  
    local eventDispatcher = self:getEventDispatcher()  
    eventDispatcher:addEventListenerWithSceneGraphPriority(keyListener,self)
    
    
    --Just For Testing
    --[[
    local winSize = cc.Director:getInstance():getWinSize()
    local serverBg2 = cc.Sprite:create("server_bg2.png")
    serverBg2:setPosition(winSize.width/2, winSize.height-50)
    self:addChild(serverBg2, 200)
    
    local function addServerBtn(param)
        local server1Btn = ccui.Button:create()
        server1Btn:setTitleText(param.name)
        server1Btn:setTitleFontSize(20)
        server1Btn:loadTextureNormal("server_bg1.png", 0)
        self.serverBtnCount = self.serverBtnCount or 0
        self.serverBtnCount = self.serverBtnCount + 1
        server1Btn:setPosition(winSize.width/2, winSize.height-50*self.serverBtnCount)
        self:addChild(server1Btn)
        if param.name == "log" then
            server1Btn:addTouchEventListener(function(sender,eventType) 
                if eventType == ccui.TouchEventType.ended then
                    print(param.name)
                    local scene = (require "scene.printLog.printScene"):create()
                    cc.Director:getInstance():pushScene(scene)
                end
            end)
        else
            server1Btn:addTouchEventListener(function(sender,eventType) 
                if eventType == ccui.TouchEventType.ended then
                    print(param.name)
                    serverBg2:setPosition(server1Btn:getPosition())
                    Http:setServerUrl(param.url)
                end
            end)
        end

    end
    
    addServerBtn({name="艾欧里亚", url="http://115.231.81.10:12000/sl"})
    --addServerBtn({name="192.168.16.128", url="http://192.168.16.128:12000/sl"})
    --addServerBtn({name="192.168.16.127", url="http://192.168.16.127:13000/sl"})
    --addServerBtn({name="log", url="http://192.168.16.41:12000/sl"})
    ]]
end

--refresh all ui here
function LoginUILayer:refreshUI()
end

--
function LoginUILayer:addNoticeLayer(updateTip)
    local noticeLayer = ccs.GUIReader:getInstance():widgetFromBinaryFile("NoticeLayer.csb")
    self:addChild(noticeLayer,2)
    
    local m_noticeScrollView = ccui.Helper:seekWidgetByName(noticeLayer,"m_noticeScrollView")
    local m_noticeCancelBtn = ccui.Helper:seekWidgetByName(noticeLayer,"m_noticeCancelBtn")
    
    local function cancelFunc()
        noticeLayer:removeFromParent()
    end
    m_noticeCancelBtn:addTouchEventListener(cancelFunc)
    
    local ScrowContentSize = m_noticeScrollView:getContentSize()
    local noticeDesc = ccui.Text:create()
    noticeDesc:setString(updateTip)
    local textHeight = (math.floor(noticeDesc:getStringLength()/35)+1)*30
                        
    noticeDesc:setFontSize(20)
    noticeDesc:setColor(cc.c3b(92,33,1))
    noticeDesc:setFontName("黑体")
    noticeDesc:setAnchorPoint(0,1)
    noticeDesc:setPosition(0,ScrowContentSize.height)
    noticeDesc:setContentSize(ScrowContentSize.width,textHeight)
    noticeDesc:ignoreContentAdaptWithSize(false)
    m_noticeScrollView:addChild(noticeDesc)
    
    m_noticeScrollView:setInnerContainerSize(cc.size(ScrowContentSize.width,textHeight))
    m_noticeScrollView:setBounceEnabled(false)
end
--set notice content here
function LoginUILayer:getNoticeText()
    local str = "这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告这是公告"
    return str
end

function LoginUILayer:UpdateServerList(severlist)
    local serverNum = table.getn(severlist)
    self.serverUrlList = {}
    local C_State = {"维护中","推荐","良好","爆满"}
    
    self.defaultSeverNo =  severlist[1]["serverNo"]
    --self.DefaultSetting = {zoneNumber = severlist[1]["serverNo"],zoneName=severlist[1]["serverName"],zoneUrl=string.format("http://"..severlist[1]["ip"]..":"..severlist[1]["port"]..severlist[1]["path"]),zoneState=C_State[severlist[1]["state"]+1]} 

    local ScrovSize = self.m_serverListView:getInnerContainerSize()

    local ScrovH = ScrovSize.height
    if serverNum>10 then
        if serverNum%2 == 0 then
            ScrovH = (50+5)*(serverNum/2)
        else
            ScrovH = (50+5)*(math.floor(serverNum/2)+1)
        end
    end

    ScrovSize.height = ScrovH
    self.m_serverListView:setInnerContainerSize(ScrovSize)
    
    local listitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("ServeriItemPanel.csb")
    self.lastServerListSelected = 0
    
    for i=1,serverNum do
        table.insert(self.serverUrlList,{codeid = severlist[i]["serverNo"], name = severlist[i]["serverName"],url= string.format("http://"..severlist[i]["ip"]..":"..severlist[i]["port"]..severlist[i]["path"])
            ,state = C_State[severlist[i]["state"]+1]})
    end
    
    self:UpdateDefaultServer()
    
    for i=1,serverNum do
        local listcopy = listitem:clone()
        listcopy:setPosition(2+(i-1)%2*300,ScrovSize.height-55*(math.floor((i-1)/2)+1))
        listcopy:setName("listcopy"..severlist[i]["serverNo"])
        
        local m_serverZoneBtn = ccui.Helper:seekWidgetByName(listcopy,"m_serverZoneBtn")
        local m_zoneNumber = ccui.Helper:seekWidgetByName(listcopy,"m_zoneNumber")
        local m_zoneName = ccui.Helper:seekWidgetByName(listcopy,"m_zoneName")
        local m_zoneState = ccui.Helper:seekWidgetByName(listcopy,"m_zoneState")
        
        m_zoneNumber:setString((string.format("%d区",severlist[i]["serverNo"])))
        m_zoneName:setString(severlist[i]["serverName"])
        m_zoneState:setString(C_State[severlist[i]["state"]+1])
        
        if severlist[i]["state"] == 0 then
            
        elseif severlist[i]["state"] == 1 then
            m_zoneState:setColor(getColorByQuality(2))
        elseif severlist[i]["state"] == 2 then
            m_zoneState:setColor(getColorByQuality(2))
        elseif severlist[i]["state"] == 3 then
            m_zoneState:setColor(getColorByQuality(5))
        end
        
        if self.lastServerListSelected == severlist[i]["serverNo"] then
            m_serverZoneBtn:setTouchEnabled(false)
            local btnRender = m_serverZoneBtn:getVirtualRenderer()
            btnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("dengluxiaokuang02.png"))
        end
        
        local function ItemTouchedFunc()
            --Http:setServerUrl(self.serverUrlList[i].url)
            m_serverZoneBtn:setTouchEnabled(false)
            local btnRender = m_serverZoneBtn:getVirtualRenderer()
            btnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("dengluxiaokuang02.png"))
            if  self.lastServerListSelected ~= 0 then
                local listcopy_last = self.m_serverListView:getChildByName("listcopy"..self.lastServerListSelected)
                local m_serverZoneBtn_last = ccui.Helper:seekWidgetByName(listcopy_last,"m_serverZoneBtn")
                m_serverZoneBtn_last:setTouchEnabled(true)
                local btnRender = m_serverZoneBtn_last:getVirtualRenderer()
                btnRender:setSpriteFrame(cc.SpriteFrameCache:getInstance():getSpriteFrame("dengluxiaokuang01.png"))
            end
            self.lastServerListSelected = severlist[i]["serverNo"]
        end
        m_serverZoneBtn:addTouchEventListener(ItemTouchedFunc)

        self.m_serverListView:addChild(listcopy)
    end
    

    self.m_LoginLayout:setVisible(false)
    self.m_ChooseServerLayout:setVisible(true)
end

function LoginUILayer:UpdateDefaultServer()
    local defaultServerNo = cc.UserDefault:getInstance():getIntegerForKey("DefaultServerNo")
    if  defaultServerNo == 0 or defaultServerNo == nil then
        defaultServerNo = self.defaultSeverNo 
    end
    
    local defaultServerData = {}
    for i=1,#self.serverUrlList do
        if defaultServerNo == self.serverUrlList[i]["codeid"] then
            defaultServerData.zoneUrl = self.serverUrlList[i]["url"]
            defaultServerData.zoneNumber = self.serverUrlList[i]["codeid"]
            defaultServerData.zoneName = self.serverUrlList[i]["name"]
        end
    end
    
    if  defaultServerData.zoneNumber then
        self.lastServerListSelected = defaultServerData.zoneNumber
        Http:setServerUrl(defaultServerData.zoneUrl)
        self.m_DefaultZoneNumber:setString(defaultServerData.zoneNumber.."区")
        self.m_DefaultZoneName:setString(defaultServerData.zoneName)
        self.m_clickTagLabel:setString("点击换区")
        local listZoneNumber = self.m_serverListDefault:getChildByName("m_zoneNumber")
        local m_zoneName = self.m_serverListDefault:getChildByName("m_zoneName")
        local m_zoneState = self.m_serverListDefault:getChildByName("m_zoneState")
        listZoneNumber:setString(defaultServerData.zoneNumber.."区")
        m_zoneName:setString(defaultServerData.zoneName)
        m_zoneState:setVisible(false)
    else
        self.m_DefaultZoneNumber:setString("")
        self.m_DefaultZoneName:setString("请选择服务器")
        self.m_clickTagLabel:setString("点击选区")
        local listZoneNumber = self.m_serverListDefault:getChildByName("m_zoneNumber")
        local m_zoneName = self.m_serverListDefault:getChildByName("m_zoneName")
        local m_zoneState = self.m_serverListDefault:getChildByName("m_zoneState")
        listZoneNumber:setString("")
        m_zoneState:setString("")
        m_zoneName:setString("请选择服务器")
    end
    
end

function LoginUILayer:checkResult()
    local bRet=false
    local result=self.m_result
    if result== 0 then 
       cclog("success")
       bRet = true
    elseif  result == 1 then
       cclog("SERVER_BUSY")
       PopDialog:getInstance():popText("服务器忙",cc.c3b(0,255,0)) 
    elseif  result == 2 then
       cclog("VERSION_LOWER")
       PopDialog:getInstance():popText("版本太低",cc.c3b(0,255,0))
    elseif  result == 3 then
       cclog("USER_NOT_EXSIT")
       PopDialog:getInstance():popText("用户不存在",cc.c3b(0,255,0))
    elseif  result == 4 then
       cclog("KEY_INVALID")
       PopDialog:getInstance():popText("密码错误",cc.c3b(0,255,0))
    elseif  result == 5 then
       cclog("USER_HAD_EXSIT")
       PopDialog:getInstance():popText("用户已存在",cc.c3b(0,255,0))
    elseif  result == 6 then
       cclog("NICKNAME_EXSIT")
       PopDialog:getInstance():popText("该名字已被他人使用",cc.c3b(0,255,0))
    end
    return bRet
end

function LoginUILayer:randomName()
    local define_random_name = require "config.define_random_name"
    
    local firstNameList = {}
    local lastNameList = {}
    
    for  k, v in pairs(define_random_name) do
        if  v["type"] == 0 then        
            table.insert(firstNameList,v["name"])
        else
            table.insert(lastNameList,v["name"])
        end
    end
    
    math.randomseed(tostring(os.time()):reverse():sub(1, 6))  
    local random_first = math.random()
    random_first = math.floor(random_first*(#firstNameList))+1
    local random_last = math.random()
    random_last = math.floor(random_last*(#lastNameList))+1
    print(random_first.."and"..random_last)
    local name = string.format("%s%s",firstNameList[random_first],lastNameList[random_last])
    return name
end

function LoginUILayer:startMotionStreakCallBack()
    print("startMotionStreakCallBack")
    --MotionStreakTest=cc.Director:getInstance():getScheduler():scheduleScriptFunc(MotionStreak_update,0.3,false)
end
 
function LoginUILayer:stopMotionStreakCallBack()
    --cc.Director:getInstance():getScheduler():unscheduleScriptEntry(MotionStreakTest)
    print("stopMotionStreakCallBack")
end
 
 
function LoginUILayer:MotionStreak_update(dt)
    local visibleSize=cc.Director:getInstance():getVisibleSize()
    print("MotionStreak_update Begin")
    local streak = cc.Sprite:create("select0.png")
    self.m_selectRoleImage:addRenderer(streak)
    streak:setPosition(self.m_selectRoleImage:getPosition()-cc.p(50,0))
    streak:runAction(cc.FadeOut:create(0.3))
end


return LoginUILayer
