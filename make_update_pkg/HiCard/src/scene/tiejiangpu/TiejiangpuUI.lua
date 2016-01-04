--TiejiangpuUI.lua, created by php script. chris.li--
require "Cocos2d"

local shopConfig = require("config.define_shop")

local PopDialog = require "common/PopDialog"

local TiejiangpuUI = class("TiejiangpuUI", function()
	return cc.Layer:create()
end)

function TiejiangpuUI:create()
	local layer = TiejiangpuUI.new()
	layer:initM()
	return layer
end

function TiejiangpuUI:ctor()
end

function TiejiangpuUI:onEnter()
    
end

function TiejiangpuUI:onExit()
end

function TiejiangpuUI:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("TiejiangpuUI.csb")
    self.rootNode:setBackGroundImage("ditu.jpg")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_tipsPanel: Layout
	self.m_tipsPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipsPanel")
	self.m_tipsPanel:addTouchEventListener(touchEventHandler)
    
	self.m_tipsPanel:setVisible(false)

	--m_layerPanel: Layout
	self.m_layerPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_layerPanel")
	self.m_layerPanel:addTouchEventListener(touchEventHandler)

	--m_returnBtn: Button
	self.m_returnBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_returnBtn")
	self.m_returnBtn:addTouchEventListener(touchEventHandler)

	--m_qhPanel: Layout
	self.m_qhPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_qhPanel")
--	self.m_qhPanel:addTouchEventListener(touchEventHandler)

	--m_jjPanel: Layout
	self.m_jjPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjPanel")
--	self.m_jjPanel:addTouchEventListener(touchEventHandler)

	--m_closeBtn: Button
	self.m_closeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closeBtn")
	self.m_closeBtn:addTouchEventListener(touchEventHandler)

	--m_tips: Text
	self.m_tips = ccui.Helper:seekWidgetByName(self.rootNode, "m_tips")

	--m_tipBtn2: Button
	self.m_tipBtn2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipBtn2")
	self.m_tipBtn2:addTouchEventListener(touchEventHandler)

	--m_tipBtn0: Button
	self.m_tipBtn0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipBtn0")
	self.m_tipBtn0:addTouchEventListener(touchEventHandler)

	--m_tipBtn1: Button
	self.m_tipBtn1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipBtn1")
	self.m_tipBtn1:addTouchEventListener(touchEventHandler)

	--m_tipBtn3: Button
	self.m_tipBtn3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipBtn3")
	self.m_tipBtn3:addTouchEventListener(touchEventHandler)

	--m_pic1: ImageView
	self.m_pic1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_pic1")

	--m_name: Text
	self.m_name = ccui.Helper:seekWidgetByName(self.rootNode, "m_name")

	--m_canQH: Text
	self.m_canQH = ccui.Helper:seekWidgetByName(self.rootNode, "m_canQH")

	--m_noQH: Text
	self.m_noQH = ccui.Helper:seekWidgetByName(self.rootNode, "m_noQH")

	--m_levelpic: ImageView
	self.m_levelpic = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelpic")

	--m_shuxingpic: ImageView
	self.m_shuxingpic = ccui.Helper:seekWidgetByName(self.rootNode, "m_shuxingpic")

	--m_zhanlipic: ImageView
	self.m_zhanlipic = ccui.Helper:seekWidgetByName(self.rootNode, "m_zhanlipic")
	
    --m_levelpic_0: ImageView
    self.m_levelpic_0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelpic_0")

    --m_shuxingpic_0: ImageView
    self.m_shuxingpic_0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_shuxingpic_0")

    --m_zhanlipic: ImageView
    self.m_zhanlipic_0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_zhanlipic_0")

	--m_level: Text
	self.m_level = ccui.Helper:seekWidgetByName(self.rootNode, "m_level")

	--m_shuxing: Text
	self.m_shuxing = ccui.Helper:seekWidgetByName(self.rootNode, "m_shuxing")

	--m_zhanli: Text
	self.m_zhanli = ccui.Helper:seekWidgetByName(self.rootNode, "m_zhanli")

	--m_levelnext: Text
	self.m_levelnext = ccui.Helper:seekWidgetByName(self.rootNode, "m_levelnext")

	--m_shuxingnext: Text
	self.m_shuxingnext = ccui.Helper:seekWidgetByName(self.rootNode, "m_shuxingnext")

	--m_zhanlinext: Text
	self.m_zhanlinext = ccui.Helper:seekWidgetByName(self.rootNode, "m_zhanlinext")

	--m_qhjinbi: Text
	self.m_qhjinbi = ccui.Helper:seekWidgetByName(self.rootNode, "m_qhjinbi")

	--m_qhbtn1: Button
	self.m_qhbtn1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_qhbtn1")
	self.m_qhbtn1:addTouchEventListener(touchEventHandler)

	--m_qhbtn2: Button
	self.m_qhbtn2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_qhbtn2")
	self.m_qhbtn2:addTouchEventListener(touchEventHandler)

	--m_jjpic1: ImageView
	self.m_jjpic1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjpic1")
	

	--m_jjname: Text
	self.m_jjname = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjname")
	

	--m_jjlevelpic: ImageView
	self.m_jjlevelpic = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjlevelpic")

	--m_jjshuxingpic: ImageView
	self.m_jjshuxingpic = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjshuxingpic")

	--m_jjzhanlipic: ImageView
	self.m_jjzhanlipic = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjzhanlipic")
	
    --m_jjlevelpic_0: ImageView
    self.m_jjlevelpic_0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjlevelpic_0")

    --m_jjshuxingpic_0: ImageView
    self.m_jjshuxingpic_0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjshuxingpic_0")

    --m_jjzhanlipic_0: ImageView
    self.m_jjzhanlipic_0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjzhanlipic_0")

	--m_jjlevel: Text
	self.m_jjlevel = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjlevel")

	--m_jjshuxing: Text
	self.m_jjshuxing = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjshuxing")

	--m_jjzhanli: Text
	self.m_jjzhanli = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjzhanli")

	--m_jjlevelnext: Text
	self.m_jjlevelnext = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjlevelnext")

	--m_jjshuxingnext: Text
	self.m_jjshuxingnext = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjshuxingnext")

	--m_jjzhanlinext: Text
	self.m_jjzhanlinext = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjzhanlinext")

	--m_jjjinbi: Text
	self.m_jjjinbi = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjjinbi")

	--m_jjbtn: Button
	self.m_jjbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjbtn")
	self.m_jjbtn:addTouchEventListener(touchEventHandler)

	--m_topBtn3: Button
	self.m_topBtn3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_topBtn3")
	self.m_topBtn3:addTouchEventListener(touchEventHandler)

	--m_topBtn2: Button
	self.m_topBtn2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_topBtn2")
	self.m_topBtn2:addTouchEventListener(touchEventHandler)

	--m_topBtn1: Button
	self.m_topBtn1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_topBtn1")
	self.m_topBtn1:addTouchEventListener(touchEventHandler)

	--m_topBtn0: Button
	self.m_topBtn0 = ccui.Helper:seekWidgetByName(self.rootNode, "m_topBtn0")
	self.m_topBtn0:addTouchEventListener(touchEventHandler)

	--m_ListView: ListView
	self.m_ListView = ccui.Helper:seekWidgetByName(self.rootNode, "m_ListView")
	self.m_ListView:addTouchEventListener(touchEventHandler)
    self.m_ListView:setBounceEnabled(true)

	--m_pinzhi: ImageView
	self.m_pinzhi = ccui.Helper:seekWidgetByName(self.rootNode, "m_pinzhi")

	--m_jjpinzhi: ImageView
	self.m_jjpinzhi = ccui.Helper:seekWidgetByName(self.rootNode, "m_jjpinzhi")
	
  
    --m_huadong
    self.m_huadong = ccui.Helper:seekWidgetByName(self.rootNode, "m_huadong")
    self.m_huadong:setVisible(false)
	
	--0为饰品，1为武器，2为衣服，3为头盔
	self.listtype = 1
	
    local armatureFile = "qianghua/qianghua.csb"
    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
    self.armature = ccs.Armature:create("qianghua")
    self.armature:setVisible(false)
    self.armature:setScale(0.6)
    self.armature:setPosition(120,60)
    self.m_pinzhi:addChild(self.armature)
    
    self.ishere = true

	self:initUI()
end

function TiejiangpuUI:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_tipsPanel and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_tipsPanel touched--")

	elseif sender == self.m_layerPanel and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_layerPanel touched--")

	elseif sender == self.m_returnBtn and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_returnBtn touched--")
        self.m_tipsPanel:setVisible(false)
        self.m_layerPanel:setVisible(true)
        
        self:initUI()

	elseif sender == self.m_qhPanel and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_qhPanel touched--")

	elseif sender == self.m_jjPanel and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_jjPanel touched--")

	elseif sender == self.m_closeBtn and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_closeBtn touched--")
        if self.ishere == true then
            self.ishere = false
            cc.Director:getInstance():popScene()
        end
        

	elseif sender == self.m_tipBtn2 and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_tipBtn2 touched--")

	elseif sender == self.m_tipBtn0 and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_tipBtn0 touched--")
        Http:req("PrepareStreng",{bagId = self.qhbagid},function(res)
                    self.m_tipsPanel:setVisible(true)
                    self.m_layerPanel:setVisible(false)
                    
                    self.m_jjPanel:setVisible(false)
                    
                    self.m_qhPanel:setVisible(true)
            
                    self.m_tipBtn0:loadTextureNormal("tap_01.png")
                    self.m_tipBtn0:setEnabled(false)
                    
                    self.m_tipBtn3:loadTextureNormal("tap_02.png")
                    self.m_tipBtn3:setEnabled(true)
                    
                    self:UpdateQHLayer(res.data)
                end)      
		

	elseif sender == self.m_tipBtn1 and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_tipBtn1 touched--")

	elseif sender == self.m_tipBtn3 and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_tipBtn3 touched--")
        Http:req("PrepareEquipmentAdvance",{bagId = self.qhbagid},function(res)
            self.m_tipsPanel:setVisible(true)
            self.m_layerPanel:setVisible(false)

            self.m_qhPanel:setVisible(false)
            
            self.m_jjPanel:setVisible(true)

            self.m_tipBtn3:loadTextureNormal("tap_01.png")
            self.m_tipBtn3:setEnabled(false)

            self.m_tipBtn0:loadTextureNormal("tap_02.png")
            self.m_tipBtn0:setEnabled(true)

            self:UpdateJJLayer(res.data)
        end)     

	elseif sender == self.m_qhbtn1 and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_qhbtn1 touched--")
        
        Http:req("Streng",{bagId = self.qhbagid , type = 1},function(res)
            if res.data["result"] == 0 then
                self.armature:setVisible(true)
                self.armature:getAnimation():playWithIndex(0)
                
                PopDialog:getInstance():popText("强化成功")
                self:handleTouchEvent(self.m_tipBtn0, cc.EventCode.ENDED)
            elseif res.data["result"] == 1 then
                PopDialog:getInstance():popText("金币不足")
            elseif res.data["result"] == 2 then
                PopDialog:getInstance():popText("装备等级不能高于战队等级")
            elseif res.data["result"] == 4 then
                PopDialog:getInstance():popText("装备已强化到最高等级")
            end
        end)

	elseif sender == self.m_qhbtn2 and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_qhbtn2 touched--")
        Http:req("Streng",{bagId = self.qhbagid , type = 0},function(res)
            if res.data["result"] == 0 then
                self.armature:setVisible(true)
                self.armature:getAnimation():playWithIndex(0)
            
                PopDialog:getInstance():popText("强化成功")
                self:handleTouchEvent(self.m_tipBtn0, cc.EventCode.ENDED)
            elseif res.data["result"] == 1 then
                PopDialog:getInstance():popText("金币不足")
            elseif res.data["result"] == 2 then
                PopDialog:getInstance():popText("装备等级不能高于战队等级")
            elseif res.data["result"] == 4 then
                PopDialog:getInstance():popText("装备已强化到最高等级")
            end
        end)

	elseif sender == self.m_jjbtn and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_jjbtn touched--")
        Http:req("Advance",{bagId = self.qhbagid},function(res)
            if res.data["result"] == 0 then
                local texiao = cc.Sprite:create()
                Helper.addSpriteFrames("jinjietexiao.plist","jinjietexiao.pvr.ccz")
                local frameArr = Helper.newFrames("%02d.png",1,13)
                local animation = Helper.newAnimation(frameArr,1/13)
                texiao:setPosition(45,45)
                texiao:setScale(1.5)
                
                self.m_jjpinzhi:addChild(texiao)
                texiao:runAction(cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create()))
                
                PopDialog:getInstance():popText("进阶成功")
                self:handleTouchEvent(self.m_tipBtn3, cc.EventCode.ENDED)
            elseif res.data["result"] == 1 then
                PopDialog:getInstance():popText("金币不足")
            elseif res.data["result"] == 2 then
                PopDialog:getInstance():popText("装备品质不足进阶更高阶")
            elseif res.data["result"] == 3 then
                PopDialog:getInstance():popText("材料不足")
            elseif res.data["result"] == 4 then
                PopDialog:getInstance():popText("装备已进阶到最高阶")
            end
        end)

	elseif sender == self.m_topBtn3 and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_topBtn3 touched--")
        self.listtype = 3
        self:Updatelist()
        self.m_ListView:jumpToTop()

	elseif sender == self.m_topBtn2 and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_topBtn2 touched--")
        self.listtype = 2
        self:Updatelist()
        self.m_ListView:jumpToTop()

	elseif sender == self.m_topBtn1 and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_topBtn1 touched--")
        self.listtype = 1
        self:Updatelist()
        self.m_ListView:jumpToTop()

	elseif sender == self.m_topBtn0 and event == cc.EventCode.ENDED then
		cclog("[TiejiangpuUI]--m_topBtn0 touched--")
        self.listtype = 0
        self:Updatelist()
        self.m_ListView:jumpToTop()	

	elseif sender == self.m_ListView then
		cclog("[TiejiangpuUI]--m_ListView touched--")
        SetSlider(self.m_ListView,self.m_huadong)
	end
end

function TiejiangpuUI:UpdateQHLayer(psr)
    local shopId=tostring(psr["shopId"])
    
    if psr["canBeStrenged"] == true then
        self.m_canQH:setVisible(true)
        self.m_noQH:setVisible(false)
    else
        self.m_canQH:setVisible(false)
        self.m_noQH:setVisible(true)
    end
    
    self.m_level:setString(psr["level"])
    if psr["nextProp"] == nil then
        self.m_levelnext:setString(psr["level"])
        
        if psr["prop"]["atk"] ~= 0 then
            self.m_shuxingpic_0:loadTexture("gong.png")
            self.m_shuxingnext:setString(psr["prop"]["atk"])
        elseif psr["prop"]["def"] ~= 0 then
            self.m_shuxingpic_0:loadTexture("fang.png")
            self.m_shuxingnext:setString(psr["prop"]["def"])
        elseif psr["prop"]["hp"] ~= 0 then
            self.m_shuxingpic_0:loadTexture("xue.png")
            self.m_shuxingnext:setString(psr["prop"]["hp"])
        elseif psr["prop"]["zhili"] ~= 0 then
            self.m_shuxingpic_0:loadTexture("zhili.png")
            self.m_shuxingnext:setString(psr["prop"]["zhili"])
        end

        if psr["prop"]["cri"] ~= 0 then
            self.m_zhanlipic_0:loadTexture("baoji.png")
            self.m_zhanlinext:setString(psr["prop"]["cri"])
        elseif psr["prop"]["block"] ~= 0 then
            self.m_zhanlipic_0:loadTexture("gedang.png")
            self.m_zhanlinext:setString(psr["prop"]["block"])
        elseif psr["prop"]["agl"] ~= 0 then
            self.m_zhanlipic_0:loadTexture("shanbi.png")
            self.m_zhanlinext:setString(psr["prop"]["agl"])
        elseif psr["prop"]["dex"] ~= 0 then
            self.m_zhanlipic_0:loadTexture("mingzhong.png")
            self.m_zhanlinext:setString(psr["prop"]["dex"])
        end
    else
        self.m_levelnext:setString(psr["level"]+1)

        if psr["nextProp"]["atk"] ~= 0 then
            self.m_shuxingpic_0:loadTexture("gong.png")
            self.m_shuxingnext:setString(psr["nextProp"]["atk"])
        elseif psr["nextProp"]["def"] ~= 0 then
            self.m_shuxingpic_0:loadTexture("fang.png")
            self.m_shuxingnext:setString(psr["nextProp"]["def"])
        elseif psr["nextProp"]["hp"] ~= 0 then
            self.m_shuxingpic_0:loadTexture("xue.png")
            self.m_shuxingnext:setString(psr["nextProp"]["hp"])
        elseif psr["nextProp"]["zhili"] ~= 0 then
            self.m_shuxingpic_0:loadTexture("zhili.png")
            self.m_shuxingnext:setString(psr["nextProp"]["zhili"])
        end

        if psr["nextProp"]["cri"] ~= 0 then
            self.m_zhanlipic_0:loadTexture("baoji.png")
            self.m_zhanlinext:setString(psr["nextProp"]["cri"])
        elseif psr["nextProp"]["block"] ~= 0 then
            self.m_zhanlipic_0:loadTexture("gedang.png")
            self.m_zhanlinext:setString(psr["nextProp"]["block"])
        elseif psr["nextProp"]["agl"] ~= 0 then
            self.m_zhanlipic_0:loadTexture("shanbi.png")
            self.m_zhanlinext:setString(psr["nextProp"]["agl"])
        elseif psr["nextProp"]["dex"] ~= 0 then
            self.m_zhanlipic_0:loadTexture("mingzhong.png")
            self.m_zhanlinext:setString(psr["nextProp"]["dex"])
        end
    end

    if psr["prop"]["atk"] ~= 0 then
        self.m_shuxingpic:loadTexture("gong.png")
        self.m_shuxing:setString(psr["prop"]["atk"])
    elseif psr["prop"]["def"] ~= 0 then
        self.m_shuxingpic:loadTexture("fang.png")
        self.m_shuxing:setString(psr["prop"]["def"])
    elseif psr["prop"]["hp"] ~= 0 then
        self.m_shuxingpic:loadTexture("xue.png")
        self.m_shuxing:setString(psr["prop"]["hp"])
    elseif psr["prop"]["zhili"] ~= 0 then
        self.m_shuxingpic:loadTexture("zhili.png")
        self.m_shuxing:setString(psr["prop"]["zhili"])
    end
    
    if psr["prop"]["cri"] ~= 0 then
        self.m_zhanlipic:loadTexture("baoji.png")
        self.m_zhanli:setString(psr["prop"]["cri"])
    elseif psr["prop"]["block"] ~= 0 then
        self.m_zhanlipic:loadTexture("gedang.png")
        self.m_zhanli:setString(psr["prop"]["block"])
    elseif psr["prop"]["agl"] ~= 0 then
        self.m_zhanlipic:loadTexture("shanbi.png")
        self.m_zhanli:setString(psr["prop"]["agl"])
    elseif psr["prop"]["dex"] ~= 0 then
        self.m_zhanlipic:loadTexture("mingzhong.png")
        self.m_zhanli:setString(psr["prop"]["dex"])
    end

    
    self.m_qhjinbi:setString(psr["useCoins"])
    
    if shopConfig[shopId] ~= nil then
        local dataitem=shopConfig[shopId]
        self.m_pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
        self.m_pic1:loadTexture(dataitem["icon"]..".png")
        self.m_name:setString(dataitem["name"])
        
    end
end

function TiejiangpuUI:UpdateJJLayer(pears)
    local shopId=tostring(pears["shopId"])


    self.m_jjlevel:setString(pears["level"])
    if pears["nextProp"] == nil then
        self.m_jjlevelnext:setString(pears["level"])

        if pears["prop"]["atk"] ~= 0 then
            self.m_jjshuxingpic_0:loadTexture("gong.png")
            self.m_jjshuxingnext:setString(pears["prop"]["atk"])
        elseif pears["prop"]["def"] ~= 0 then
            self.m_jjshuxingpic_0:loadTexture("fang.png")
            self.m_jjshuxingnext:setString(pears["prop"]["def"])
        elseif pears["prop"]["hp"] ~= 0 then
            self.m_jjshuxingpic_0:loadTexture("xue.png")
            self.m_jjshuxingnext:setString(pears["prop"]["hp"])
        elseif pears["prop"]["zhili"] ~= 0 then
            self.m_jjshuxingpic_0:loadTexture("zhili.png")
            self.m_jjshuxingnext:setString(pears["prop"]["zhili"])
        end

        if pears["prop"]["cri"] ~= 0 then
            self.m_jjzhanlipic_0:loadTexture("baoji.png")
            self.m_jjzhanlinext:setString(pears["prop"]["cri"])
        elseif pears["prop"]["block"] ~= 0 then
            self.m_jjzhanlipic_0:loadTexture("gedang.png")
            self.m_jjzhanlinext:setString(pears["prop"]["block"])
        elseif pears["prop"]["agl"] ~= 0 then
            self.m_jjzhanlipic_0:loadTexture("shanbi.png")
            self.m_jjzhanlinext:setString(pears["prop"]["agl"])
        elseif pears["prop"]["dex"] ~= 0 then
            self.m_jjzhanlipic_0:loadTexture("mingzhong.png")
            self.m_jjzhanlinext:setString(pears["prop"]["dex"])
        end
    else
        self.m_jjlevelnext:setString(pears["level"]+1)

        if pears["nextProp"]["atk"] ~= 0 then
            self.m_jjshuxingpic_0:loadTexture("gong.png")
            self.m_jjshuxingnext:setString(pears["nextProp"]["atk"])
        elseif pears["nextProp"]["def"] ~= 0 then
            self.m_jjshuxingpic_0:loadTexture("fang.png")
            self.m_jjshuxingnext:setString(pears["nextProp"]["def"])
        elseif pears["nextProp"]["hp"] ~= 0 then
            self.m_jjshuxingpic_0:loadTexture("xue.png")
            self.m_jjshuxingnext:setString(pears["nextProp"]["hp"])
        elseif pears["nextProp"]["zhili"] ~= 0 then
            self.m_jjshuxingpic_0:loadTexture("zhili.png")
            self.m_jjshuxingnext:setString(pears["nextProp"]["zhili"])
        end

        if pears["nextProp"]["cri"] ~= 0 then
            self.m_jjzhanlipic_0:loadTexture("baoji.png")
            self.m_jjzhanlinext:setString(pears["nextProp"]["cri"])
        elseif pears["nextProp"]["block"] ~= 0 then
            self.m_jjzhanlipic_0:loadTexture("gedang.png")
            self.m_jjzhanlinext:setString(pears["nextProp"]["block"])
        elseif pears["nextProp"]["agl"] ~= 0 then
            self.m_jjzhanlipic_0:loadTexture("shanbi.png")
            self.m_jjzhanlinext:setString(pears["nextProp"]["agl"])
        elseif pears["nextProp"]["dex"] ~= 0 then
            self.m_jjzhanlipic_0:loadTexture("mingzhong.png")
            self.m_jjzhanlinext:setString(pears["nextProp"]["dex"])
        end
    end

    if pears["prop"]["atk"] ~= 0 then
        self.m_jjshuxingpic:loadTexture("gong.png")
        self.m_jjshuxing:setString(pears["prop"]["atk"])
    elseif pears["prop"]["def"] ~= 0 then
        self.m_jjshuxingpic:loadTexture("fang.png")
        self.m_jjshuxing:setString(pears["prop"]["def"])
    elseif pears["prop"]["hp"] ~= 0 then
        self.m_jjshuxingpic:loadTexture("xue.png")
        self.m_jjshuxing:setString(pears["prop"]["hp"])
    elseif pears["prop"]["zhili"] ~= 0 then
        self.m_jjshuxingpic:loadTexture("zhili.png")
        self.m_jjshuxing:setString(pears["prop"]["zhili"])
    end

    if pears["prop"]["cri"] ~= 0 then
        self.m_jjzhanlipic:loadTexture("baoji.png")
        self.m_jjzhanli:setString(pears["prop"]["cri"])
    elseif pears["prop"]["block"] ~= 0 then
        self.m_jjzhanlipic:loadTexture("gedang.png")
        self.m_jjzhanli:setString(pears["prop"]["block"])
    elseif pears["prop"]["agl"] ~= 0 then
        self.m_jjzhanlipic:loadTexture("shanbi.png")
        self.m_jjzhanli:setString(pears["prop"]["agl"])
    elseif pears["prop"]["dex"] ~= 0 then
        self.m_jjzhanlipic:loadTexture("mingzhong.png")
        self.m_jjzhanli:setString(pears["prop"]["dex"])
    end


    self.m_jjjinbi:setString(pears["useCoins"])

    if shopConfig[shopId] ~= nil then
        local dataitem=shopConfig[shopId]
        self.m_jjpinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
        self.m_jjpic1:loadTexture(dataitem["icon"]..".png")
        self.m_jjname:setString(dataitem["name"].."+"..pears["level"])
        
        
    end
    
    local materaillist = pears["materail"]
    
    local ListView_cailiao = ccui.Helper:seekWidgetByName(self.m_jjPanel, "ListView_cailiao")
    ListView_cailiao:removeAllItems()
    for i = 1, 3 do
        local cailiaoitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("tiejiangpuUI_Cailiao.csb")
        local cailiao = ccui.Helper:seekWidgetByName(cailiaoitem, "cailiao")
        local cailiaopinzhi = ccui.Helper:seekWidgetByName(cailiao, "cailiaopinzhi")
        local num = ccui.Helper:seekWidgetByName(cailiao, "num")
        local name = ccui.Helper:seekWidgetByName(cailiao, "name")
        
        ListView_cailiao:pushBackCustomItem(cailiaoitem)
        
        cailiao:setVisible(false)
        
        if table.getn(materaillist) >= i then
            cailiao:setVisible(true)
            num:setString("("..materaillist[i]["curCnt"].."/"..materaillist[i]["needCnt"]..")")
            if materaillist[i]["curCnt"] >= materaillist[i]["needCnt"] then
            	num:setColor(getColorByQuality(2))
            else
                num:setColor(getColorByQuality(5))
            end
            
            local materailitem = shopConfig[tostring(materaillist[i]["shopId"])]
            cailiao:loadTexture(materailitem["icon"]..".png")
            cailiaopinzhi:loadTexture("normal_aochao_0"..materailitem["quality"]..".png")
            name:setString(materailitem["name"])
            
            if materaillist[i]["curCnt"]<materaillist[i]["needCnt"] then
                local texture = cailiao:getVirtualRenderer()
                Helper.graySprite(texture)
                
                cailiaopinzhi:loadTexture("normal_aochao_01.png")

            end
        end
    end
end

function TiejiangpuUI:Updatelist()
    self.m_topBtn0:loadTextureNormal("tap_02.png")
    self.m_topBtn1:loadTextureNormal("tap_02.png")
    self.m_topBtn2:loadTextureNormal("tap_02.png")
    self.m_topBtn3:loadTextureNormal("tap_02.png")
    self.m_topBtn0:setEnabled(true)
    self.m_topBtn1:setEnabled(true)
    self.m_topBtn2:setEnabled(true)
    self.m_topBtn3:setEnabled(true)
    
    local EquipmentList = nil
    
    if self.listtype == 0 then
        self.m_topBtn0:loadTextureNormal("tap_01.png")
        self.m_topBtn0:setEnabled(false)
        EquipmentList = self.accessoryList
    elseif self.listtype == 1 then
        self.m_topBtn1:loadTextureNormal("tap_01.png")
        self.m_topBtn1:setEnabled(false)
        EquipmentList = self.weaponInList
    elseif self.listtype == 2 then
        self.m_topBtn2:loadTextureNormal("tap_01.png")
        self.m_topBtn2:setEnabled(false)
        EquipmentList = self.clothInList
    elseif self.listtype == 3 then
        self.m_topBtn3:loadTextureNormal("tap_01.png")
        self.m_topBtn3:setEnabled(false)
        EquipmentList = self.headInList
    end
    
    self.m_ListView:removeAllItems()
    local EquipmentS = ccs.GUIReader:getInstance():widgetFromBinaryFile("TiejiangpuUI_One.csb")
    local listviewsize = self.m_ListView:getContentSize()
    listviewsize.height = EquipmentS:getContentSize().height*table.getn(EquipmentList)
    self.m_ListView:setInnerContainerSize(listviewsize)
    
    if table.getn(EquipmentList) == 0 then
    	self.m_tips:setVisible(true)
    else
        self.m_tips:setVisible(false)
    end

    for i = 1,table.getn(EquipmentList) do
        local Equipment = ccs.GUIReader:getInstance():widgetFromBinaryFile("TiejiangpuUI_One.csb")
        self.m_ListView:pushBackCustomItem(Equipment)

        local shopId=tostring(EquipmentList[i]["shopId"])

        local btn = ccui.Helper:seekWidgetByName(Equipment, "btn")
        local pinzhi = ccui.Helper:seekWidgetByName(Equipment, "pinzhi")
        local zbpic = ccui.Helper:seekWidgetByName(Equipment, "zbpic")
        local name = ccui.Helper:seekWidgetByName(Equipment, "name")
        local level = ccui.Helper:seekWidgetByName(Equipment, "level")
        local level_jj = ccui.Helper:seekWidgetByName(Equipment, "level_jj")
        local sxpic1 = ccui.Helper:seekWidgetByName(Equipment, "sxpic1")
        local sxpic2 = ccui.Helper:seekWidgetByName(Equipment, "sxpic2")
        local shuxing1 = ccui.Helper:seekWidgetByName(Equipment, "shuxing1")
        local shuxing2 = ccui.Helper:seekWidgetByName(Equipment, "shuxing2")
        local eqlabel = ccui.Helper:seekWidgetByName(Equipment, "eqlabel")
        eqlabel:setVisible(false)
        if EquipmentList[i]["isEquiped"] == true then
            eqlabel:setVisible(true)
            eqlabel:setString(EquipmentList[i]["equipName"])
        end

        local function EquipmentMenuCallBack(sender, event)
            if sender == btn and event == cc.EventCode.ENDED then
                self.qhbagid = sender:getTag()
               self:handleTouchEvent(self.m_tipBtn0, cc.EventCode.ENDED)      
            end
        end

        btn:addTouchEventListener(EquipmentMenuCallBack)
        btn:setTag(EquipmentList[i]["bagId"])
        
        
        level:setString(EquipmentList[i]["equipLevel"])
        level_jj:setString(EquipmentList[i]["composeTime"])

        if EquipmentList[i]["prop"]["atk"] ~= 0 then
            sxpic1:loadTexture("gong.png")
            shuxing1:setString(EquipmentList[i]["prop"]["atk"])
        elseif EquipmentList[i]["prop"]["def"] ~= 0 then
            sxpic1:loadTexture("fang.png")
            shuxing1:setString(EquipmentList[i]["prop"]["def"])
        elseif EquipmentList[i]["prop"]["hp"] ~= 0 then
            sxpic1:loadTexture("xue.png")
            shuxing1:setString(EquipmentList[i]["prop"]["hp"])
        elseif EquipmentList[i]["prop"]["zhili"] ~= 0 then
            sxpic1:loadTexture("zhili.png")
            shuxing1:setString(EquipmentList[i]["prop"]["zhili"])
        end
        
        if EquipmentList[i]["prop"]["cri"] ~= 0 then
            sxpic2:loadTexture("baoji.png")
            shuxing2:setString(EquipmentList[i]["prop"]["cri"])
        elseif EquipmentList[i]["prop"]["block"] ~= 0 then
            sxpic2:loadTexture("gedang.png")
            shuxing2:setString(EquipmentList[i]["prop"]["block"])
        elseif EquipmentList[i]["prop"]["agl"] ~= 0 then
            sxpic2:loadTexture("shanbi.png")
            shuxing2:setString(EquipmentList[i]["prop"]["agl"])
        elseif EquipmentList[i]["prop"]["dex"] ~= 0 then
            sxpic2:loadTexture("mingzhong.png")
            shuxing2:setString(EquipmentList[i]["prop"]["dex"])
        end

        if  shopConfig[shopId] ~= nil then
            local dataitem=shopConfig[shopId]
            pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            zbpic:loadTexture(dataitem["icon"]..".png")
            name:setString(dataitem["name"])
            
        end

    end
	
end

--init your ui here
function TiejiangpuUI:initUI()
    Http:req("AllEquipmentList",nil,function(list)
        self.weaponInList = list.data["weaponInListInfo"]
        self.clothInList = list.data["clothInListInfo"]
        self.headInList = list.data["headInListInfo"]
        self.accessoryList = list.data["accessoryListInfo"]

        self:handleTouchEvent(self.m_topBtn1, cc.EventCode.ENDED)
    end)
end

--refresh all ui here
function TiejiangpuUI:refreshUI()
end

return TiejiangpuUI
