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

	--m_tipsLabel: Text
	self.m_tipsLabel = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipsLabel")

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
    
	self:initUI()
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
        if (self.m_userNameTextField:getStringValue()~= "") then 
            Http:req("CreateRole", 
                {nickname = self.m_roleNameTextfield:getStringValue(),
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
            cclog("nickname should not be nil")
        end

	elseif sender == self.m_rolemanBtn and event == cc.EventCode.ENDED then
		cclog("[LoginUILayer]--m_rolemanBtn touched--")
        self.m_rolemanBtn:setTouchEnabled(false)
        self.m_rolewomanBtn:setTouchEnabled(true)
        self.m_roleIndex=0
        self.m_selectRoleImage:loadTexture("select0.png")
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
        self.m_selectRoleImage:loadTexture("select1.png")
        self.m_selectRoleImage:setPosition(0, self.m_selectRoleImage:getPositionY())
        self.m_selectRoleImage:runAction(cc.MoveTo:create(0.3,cc.p(visibleSize.width/2,visibleSize.height/2)))

	elseif sender == self.m_randBtn and event == cc.EventCode.ENDED then
		cclog("[LoginUILayer]--m_randBtn touched--")
		Http:req("RandomName", nil, function(res) 
		  if res.err then cclog("err!!!") return end
		  self.m_roleNameTextfield:setText(res.data["user"])
		end)
		 
    elseif sender == self.m_roleNameTextfield and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_roleNameTextfield touched--")
        self.textType = "roleName"
        
    elseif sender == self.m_loginBtn and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_loginBtn touched--")
        if (self.m_userNameTextField: getStringValue()~= "") then 
            Http:req("Login", 
                {user = self.m_userNameTextField:getStringValue(),
                key = self.m_userPasswordTextField:getStringValue(),
                version = "1.0.1"
                },
                function(res) 
                    if res.err then cclog("err!!!") return end
                    self.m_result=res.data["result"] 
                    if self:checkResult() then
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
                        end   
                    end 
            end)
        else 
            self.m_tipsLabel:setVisible(true)
        end
    elseif sender == self.m_registerBtn and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_registerBtn touched--")

    elseif sender == self.m_userNameTextField and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_userNameTextField touched--")
        self.textType = "userName"
        
    elseif sender == self.m_userPasswordTextField and event == cc.EventCode.ENDED then
        cclog("[LoginUILayer]--m_userPasswordTextField touched--")
        self.textType = "userPassword"
        
    end
end

--init your ui here
function LoginUILayer:initUI()
    self.m_roleIndex = 0
    self.m_result = -1
    self.m_createRoleLayout:setVisible(false)
    self.m_LoginLayout:setVisible(true)
    self.m_rolemanBtn:setTouchEnabled(false)
    
    self.m_selectRoleImage:loadTexture("select0.png")
    
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
      
end

--refresh all ui here
function LoginUILayer:refreshUI()
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
