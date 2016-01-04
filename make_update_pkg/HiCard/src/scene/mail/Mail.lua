--Mail.lua, created by php script. chris.li--
require "Cocos2d"

local PopDialog = require "common/PopDialog"

local shopConfig = require("config.define_shop")

local Mail = class("Mail", function()
	return cc.Layer:create()
end)

function Mail:create()
	local layer = Mail.new()
	layer:initM()
	return layer
end

function Mail:ctor()
end

function Mail:onEnter()
end

function Mail:onExit()
end

function Mail:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("Mail.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end
	
	self.firstcheck = 0
	
    --m_layerPanel: Layout
    self.m_layerPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_layerPanel")
    self.rootNode:setBackGroundImage("ditu.jpg")

	--m_Panelmaildesc: ImageView
	self.m_Panelmaildesc = ccui.Helper:seekWidgetByName(self.rootNode, "m_Panelmaildesc")

	--m_listpanel: ImageView
	self.m_listpanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_listpanel")

	--m_mailtitle: Text
	self.m_mailtitle = ccui.Helper:seekWidgetByName(self.rootNode, "m_mailtitle")

	--m_getbtn: Button
	self.m_getbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_getbtn")
	self.m_getbtn:addTouchEventListener(touchEventHandler)

    --m_ListViewDesc: ListView
    self.m_ListViewDesc = ccui.Helper:seekWidgetByName(self.rootNode, "m_ListViewDesc")
    self.m_ListViewDesc:addTouchEventListener(touchEventHandler)

    --m_ListView: ListView
    self.m_ListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_ListView")
    self.m_ListView:addTouchEventListener(touchEventHandler)
  

	--m_nomail: Text
	self.m_nomail = ccui.Helper:seekWidgetByName(self.rootNode, "m_nomail")

	--m_returnBtn: Button
	self.m_returnBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_returnBtn")
	self.m_returnBtn:addTouchEventListener(touchEventHandler)
	
    --m_huadong
    self.m_huadong = ccui.Helper:seekWidgetByName(self.rootNode, "huadong")
    self.m_huadong:setVisible(false)

	self:initUI()
end

function Mail:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_getbtn and event == cc.EventCode.ENDED then
		cclog("[Mail]--m_getbtn touched--")
        Http:req("ReceiveFujian",{emailId = sender:getTag()}, function(res)
            if res.data["result"] == 0 then
                PopDialog:getInstance():popText("领取成功!")
                self.firstcheck = 0
                local mt1 = cc.MoveTo:create(0.2,cc.p(self.m_listpanel:getPositionX()+220,self.m_listpanel:getPositionY()))
                local mt2 = cc.MoveTo:create(0.2,cc.p(self.m_Panelmaildesc:getPositionX()-220,self.m_Panelmaildesc:getPositionY()))
                self.m_listpanel:runAction(mt1)
                self.m_Panelmaildesc:runAction(mt2)
		  	    
                self:UpdateMailList()
		  else
		  
		  end
		end)

    elseif sender == self.m_ListViewDesc and event == cc.EventCode.ENDED then
        cclog("[Mail]--m_ListViewDesc touched--")

    elseif sender == self.m_ListView and event == cc.EventCode.ENDED then
        cclog("[Mail]--m_ListView touched--")
        SetSlider(self.m_ListView,self.m_huadong)

	elseif sender == self.m_returnBtn and event == cc.EventCode.ENDED then
		cclog("[Mail]--m_returnBtn touched--")
        cc.Director:getInstance():popScene()
	end
end


function Mail:UpdateMaildesc() 
    self.m_mailtitle:setString(self.maildata["title"])
    local descLayout = ccui.Layout:create()
    
	local maildesc = ccui.Text:create()
    maildesc:setString(self.maildata["content"])
    maildesc:setFontSize(22)
    maildesc:setColor(cc.c3b(92,33,1))
    maildesc:setFontName("黑体")
    maildesc:setAnchorPoint(0,0)
    maildesc:setPosition(0,0)
    maildesc:setContentSize(290,(math.floor(maildesc:getStringLength()/13)+1)*25)
    maildesc:ignoreContentAdaptWithSize(false)
    descLayout:addChild(maildesc)
    descLayout:setContentSize(maildesc:getContentSize())
  
    local imageLayout = ccui.Layout:create()
    local xian = ccui.ImageView:create()
    xian:loadTexture("xian.png")
    xian:setAnchorPoint(0,0)
    xian:setPosition(0,10)
    imageLayout:addChild(xian)
    imageLayout:setContentSize(xian:getCustomSize().width,xian:getCustomSize().height+10)
    self.m_ListViewDesc:pushBackCustomItem(descLayout)
    self.m_ListViewDesc:pushBackCustomItem(imageLayout)
    
    if self.maildata["coins"] ~= 0 then
        local fujianitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("MailUI_fujian.csb")
        local pic = ccui.Helper:seekWidgetByName(fujianitem, "pic")
        pic:loadTexture("jinbi.png")
        local text = ccui.Helper:seekWidgetByName(fujianitem, "text")
        text:setString("x"..self.maildata["coins"])
        self.m_ListViewDesc:pushBackCustomItem(fujianitem)
    end
    
    if self.maildata["gold"] ~= 0 then
        local fujianitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("MailUI_fujian.csb")
        local pic = ccui.Helper:seekWidgetByName(fujianitem, "pic")
        pic:loadTexture("baoshi.png")
        local text = ccui.Helper:seekWidgetByName(fujianitem, "text")
        text:setString("x"..self.maildata["gold"])
        self.m_ListViewDesc:pushBackCustomItem(fujianitem)
    end
    
    if self.maildata["energy"] ~= 0 then
        local fujianitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("MailUI_fujian.csb")
        local pic = ccui.Helper:seekWidgetByName(fujianitem, "pic")
        pic:loadTexture("tili.png")
        local text = ccui.Helper:seekWidgetByName(fujianitem, "text")
        text:setString("x"..self.maildata["energy"])
        self.m_ListViewDesc:pushBackCustomItem(fujianitem)
    end
    
    if self.maildata["score"] ~= 0 then
        local fujianitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("MailUI_fujian.csb")
        local pic = ccui.Helper:seekWidgetByName(fujianitem, "pic")
        pic:loadTexture("gongxianbi.png")
        local text = ccui.Helper:seekWidgetByName(fujianitem, "text")
        text:setString("x"..self.maildata["score"])
        self.m_ListViewDesc:pushBackCustomItem(fujianitem)
    end
    
    local fujianlist = self.maildata["emailShops"]
    local fujianlistsize = table.getn(fujianlist)
    
    if  fujianlistsize~=0 then
    
        for i=1,fujianlistsize do
            local fujianitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("MailUI_fujian.csb")
            local pic = ccui.Helper:seekWidgetByName(fujianitem, "pic")
            local text = ccui.Helper:seekWidgetByName(fujianitem, "text")
            text:setString("x"..fujianlist[i]["count"])
            self.m_ListViewDesc:pushBackCustomItem(fujianitem)
            
            if  shopConfig[tostring(fujianlist[i]["shopId"])] ~= nil then
                local dataitem=shopConfig[tostring(fujianlist[i]["shopId"])]
                          
                pic:loadTexture(dataitem["icon"]..".png")
                
            end
        end
        
    end
    
    self.m_getbtn:setTag(self.maildata["emailId"])
    if table.getn(self.m_ListViewDesc:getItems()) <= 2 then
        self.m_getbtn:setVisible(false)
    	self.m_getbtn:setEnabled(false)
    else
        self.m_getbtn:setVisible(true)
        self.m_getbtn:setEnabled(true)
    end
    
end

function Mail:UpdateMailList() 
    Http:req("GetEmailList",nil, function(res)
        local maillist = res.data["emailInList"]
        local mailsize = table.getn(maillist)
        if mailsize<1 then
            self.m_nomail:setVisible(true)
        else
            self.m_nomail:setVisible(false)
        end
        local mailitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("MailUI_mailone.csb")
        self.m_ListView:removeAllItems()
        
        local listviewsize = self.m_ListView:getContentSize()
        listviewsize.height = mailitem:getContentSize().height*mailsize
        self.m_ListView:setInnerContainerSize(listviewsize)
        
        self.m_ListView:setBounceEnabled(true)


        for i=1, mailsize do
            local mailitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("MailUI_mailone.csb")
            self.m_ListView:pushBackCustomItem(mailitem)

            local mailbtn = ccui.Helper:seekWidgetByName(mailitem, "mailbtn")
            
            if maillist[i]["status"] == 0 then
                mailbtn:loadTextures("weidu01.png","weidu02.png","weidu02.png")
            else
                mailbtn:loadTextures("yidu.png","yidu02.png","yidu02.png")
            end
            local function mainBtnCallback(sender,event)
                SetSlider(self.m_ListView,self.m_huadong)
                if sender == mailbtn and event == cc.EventCode.ENDED then
                    if self.firstcheck == 0 then
                        self.firstcheck = 1
                        local mt1 = cc.MoveTo:create(0.2,
                            cc.p(self.m_listpanel:getPositionX()-220,self.m_listpanel:getPositionY()))
                        local mt2 = cc.MoveTo:create(0.2,
                            cc.p(self.m_Panelmaildesc:getPositionX()+220,self.m_Panelmaildesc:getPositionY()))
                        self.m_listpanel:runAction(mt1)
                        self.m_Panelmaildesc:runAction(mt2)
                    end

                    Http:req("ReadEmail",{emailId = maillist[i]["emailId"]},function (sendid)
                        sender:loadTextures("yidu.png","yidu02.png","yidu02.png")
                        self.maildata = sendid.data["email"]
                        self.m_ListViewDesc:removeAllItems()
                        self:UpdateMaildesc()
                    end)
                end

            end

            mailbtn:addTouchEventListener(mainBtnCallback)

            local mailname = ccui.Helper:seekWidgetByName(mailitem, "name")
            mailname:setString(maillist[i]["title"])
            
            local mailtime = ccui.Helper:seekWidgetByName(mailitem, "time")
            mailtime:setString(SetTime_2(maillist[i]["sendDt"]))

        end
        self.m_ListView:jumpToTop()
    end)
end

--init your ui here
function Mail:initUI()
    self:UpdateMailList()
end

--refresh all ui here
function Mail:refreshUI()
end

return Mail
