--IndianaUi.lua, created by php script. chris.li--
require "Cocos2d"

local soulConfig = require("config.define_soul")

local PopDialog = require "common/PopDialog"

local IndianaUi = class("IndianaUi", function()
	return cc.Layer:create()
end)

function IndianaUi:create()
	local layer = IndianaUi.new()
	layer:initM()
	return layer
end

function IndianaUi:ctor()
end

function IndianaUi:onEnter()
    self:updateLayer()
    --飞艇暂时停不下来
    -- guard_tag --
   -- self:processGuard()
end

function IndianaUi:onExit()
end

--[[function IndianaUi:processGuard()
    local function addGuardDialogLayer(cmd)
        cmd.type = "guard_dialog"
        cmd.cb = function() self:processGuard() end
        Helper.setTimeout(self, 1, function()
            self.guardLayer = require("common.Guard"):create(cmd)
            cc.Director:getInstance():getRunningScene():addChild(self.guardLayer)
        end)
    end

    local function addGuardClickLayer(cmd)
        --assert(cmd.clickTarget~=nil,"")
        cmd.type = "guard_click"
        Helper.setTimeout(self, 1, function()
            self.guardLayer = require("common.Guard"):create(cmd)
            cc.Director:getInstance():getRunningScene():addChild(self.guardLayer)
        end)
    end

    local function removeGuardLayer()
        if self.guardLayer then
            cc.Director:getInstance():getRunningScene():removeChild(self.guardLayer)
            self.guardLayer = nil
        end
    end

    removeGuardLayer()

    if guardCenter:canEnterGuardMode("duobao") == true then
        local cmd = guardCenter:getCmd("duobao")
        if cmd == nil then return end
        if cmd.name == "duobao_click_feiting" then
            cmd.clickTarget = self.testXuanze
            addGuardClickLayer(cmd)
        elseif cmd.name == "duobao_click_zhanbtn" then
            cmd.clickTarget = self.m_ftbtn
            addGuardClickLayer(cmd)
        end
    end
end--]]

function IndianaUi:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("IndianaUi.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end

	--m_indianaPanel: Layout
	self.m_indianaPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_indianaPanel")
	self.m_indianaPanel:addTouchEventListener(touchEventHandler)
	self.m_indianaPanel:setBackGroundImage("indiana/ditu.jpg")

	--m_tipPanel: Layout
	self.m_tipPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_tipPanel")
	self.m_tipPanel:addTouchEventListener(touchEventHandler)
    self.m_tipPanel:setVisible(false)
	
    --m_layerPanel: Layout
    self.m_layerPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_layerPanel")
    self.m_layerPanel:addTouchEventListener(touchEventHandler)

	--m_backbtn: Button
	self.m_backbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_backbtn")
	self.m_backbtn:addTouchEventListener(touchEventHandler)

	--m_shuaxin: Button
	self.m_shuaxin = ccui.Helper:seekWidgetByName(self.rootNode, "m_shuaxin")
	self.m_shuaxin:addTouchEventListener(touchEventHandler)

	--m_touxiang: ImageView
	self.m_touxiang = ccui.Helper:seekWidgetByName(self.rootNode, "m_touxiang")

	--m_ftbtn: Button
	self.m_ftbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_ftbtn")
	self.m_ftbtn:addTouchEventListener(touchEventHandler)

	--m_closebtn: Button
	self.m_closebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closebtn")
	self.m_closebtn:addTouchEventListener(touchEventHandler)

	--m_touxiangpinzhi: ImageView
	self.m_touxiangpinzhi = ccui.Helper:seekWidgetByName(self.rootNode, "m_touxiangpinzhi")

	--m_level: Text
	self.m_level = ccui.Helper:seekWidgetByName(self.rootNode, "m_level")

	--m_name: Text
	self.m_name = ccui.Helper:seekWidgetByName(self.rootNode, "m_name")
	
    --m_zhandouli: Text
    self.m_zhandouli = ccui.Helper:seekWidgetByName(self.rootNode, "m_zhandouli")
    
    --m_texiao: ImageView
    self.m_texiao = ccui.Helper:seekWidgetByName(self.rootNode, "m_texiao")
    self.m_texiao:runAction(cc.Hide:create())
    
    --m_tips: Button
    self.m_tips = ccui.Helper:seekWidgetByName(self.rootNode, "m_tips")
    self.m_tips:addTouchEventListener(touchEventHandler)
    self.m_tips:setFontSize(22)

    self.m_ftnum = 0
	self:initUI()
end

function IndianaUi:handleTouchEvent(sender, event)
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_indianaPanel and event == cc.EventCode.ENDED then
		cclog("[IndianaUi]--m_indianaPanel touched--")

    elseif sender == self.m_layerPanel and event == cc.EventCode.ENDED then
        cclog("[IndianaUi]--m_layerPanel touched--")
		
    elseif sender == self.m_tipPanel and event == cc.EventCode.ENDED then
        cclog("[IndianaUi]--m_tipPanel touched--")

	elseif sender == self.m_backbtn and event == cc.EventCode.ENDED then
		cclog("[IndianaUi]--m_backbtn touched--")
		cc.Director:getInstance():popScene()
        
     --   local scene = require("scene.home.HomeScene") : create()
     --   cc.Director:getInstance():replaceScene(scene)

	elseif sender == self.m_shuaxin and event == cc.EventCode.ENDED then
		cclog("[IndianaUi]--m_shuaxin touched--")
		self.m_texiao:runAction(cc.Sequence:create(cc.Show:create(),cc.FadeOut:create(1),cc.Hide:create(),cc.FadeIn:create(0)))
        self:updateLayer()

	elseif sender == self.m_ftbtn and event == cc.EventCode.ENDED then
		cclog("[IndianaUi]--m_ftbtn touched--")
		
		--飞艇暂时停不下来
        -- guard_tag --
        --self:processGuard()
        if self.m_ftnum == 0 then
            PopDialog:getInstance():popText("夺宝次数已用尽，请提升VIP等级！")
            return
        end
		
        local torolerid = sender:getTag()
        local data = {toRolerId = torolerid,matrix ={}}
        local Formation = require "data.Formation"
        Formation:set("battledata",data)

        local FormationUILayer =  require "scene.formation.FormationUILayer"

        --FormationUILayer:setBeforeSceneType("book")
        self.m_tipPanel:setVisible(false)

        local scence = require("scene.formation.FormationScene"):create({BattleType="book"})
        cc.Director:getInstance():pushScene(scence)
        
       
	elseif sender == self.m_closebtn and event == cc.EventCode.ENDED then
		cclog("[IndianaUi]--m_closebtn touched--")
  
        self.m_tipPanel:setVisible(false)

	end
end

function IndianaUi:showTip(data)
    self.m_level:setString(data["level"])
    self.m_name:setString(data["name"])
    self.m_zhandouli:setString(data["fc"])
    self.m_ftbtn:setTag(data["toRolerId"])
    
    for i=1, 5 do
        local heropic = ccui.Helper:seekWidgetByName(self.rootNode, "heropic_"..i-1)
        local heropinzhi = ccui.Helper:seekWidgetByName(heropic,"heropinzhi")
        local level = ccui.Helper:seekWidgetByName(heropic,"level")
        heropic:setVisible(false)
       
        if table.getn(data["matrix"])>= i then
            heropic:setVisible(true)
            level:setString(data["matrix"][i]["level"])
            if  soulConfig[tostring(data["matrix"][i]["id"])] ~= nil then
                local dataitem=soulConfig[tostring(data["matrix"][i]["id"])]
                heropinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                heropic:loadTexture(dataitem["icon"]..".png")
                
            end
        end
        
    end

    self.m_tipPanel:setVisible(true)
end

function IndianaUi:updateLayer()
    self.m_layerPanel:removeAllChildren()
    Http:req("getChallengeRolerList",nil,function(res)
        
        local rolerlist = res.data["list"]
        local rolerlistmax = table.getn(rolerlist)
        
        self.m_ftnum = res.data["leftCnt"]
        self.m_tips:setString("攻击飞艇有几率获得经验药水，当前挑战次数："..self.m_ftnum)
        
        local armatureFile = "indiana/feichuan/feichuan.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        
        for i=1, rolerlistmax do
        	local sprite = cc.Sprite:create()
        	
        	local xuanzequyu = ccui.ImageView:create()
        	xuanzequyu:loadTexture("common/xuanzequyu.png")
        	xuanzequyu:setContentSize(cc.size(189,191))
            sprite:addChild(xuanzequyu,1001)
            xuanzequyu:setPosition(cc.p(90,90))
            xuanzequyu:setTouchEnabled(true)
                
            
            local armature = ccs.Armature:create("feichuan")
            sprite:addChild(armature,1000)
            armature:getAnimation():playWithIndex(0)
            armature:setPosition(cc.p(0,0))
            
            
            local namedi = ccui.ImageView:create()
            namedi:loadTexture("indiana/di.png")
            sprite:addChild(namedi)
            namedi:setPosition(cc.p(5,100))
            
            local namelabel = cc.Label:create()
            namelabel:setString(rolerlist[i]["name"])
            namedi:addChild(namelabel)
            namelabel:setPosition(cc.p(106,12))
            namelabel:setAnchorPoint(0.5,0.5)
            namelabel:setSystemFontSize(18)
            namelabel:setColor(cc.c3b(255,234,79))
            
            local levellabel = cc.Label:create()
            levellabel:setString("Lv:"..rolerlist[i]["level"])
            namedi:addChild(levellabel)
            levellabel:setPosition(cc.p(6,12))
            levellabel:setSystemFontSize(18)
            levellabel:setAnchorPoint(0,0.5)
        	
        	self.m_layerPanel:addChild(sprite)
            
        	local h = 1
        	if i%2 == 0 then
        		h = -1
        	end
            
            sprite:setPosition(cc.p(-100-150*(i-1),WIN_SIZE.height/2+h*(50+i*10)))


            local function SpriteCallBack(sender, event)
                if event == cc.EventCode.ENDED then
                    local tag = sender:getTag()
                    self:showTip(rolerlist[tag])
                end
        	end
            xuanzequyu:addTouchEventListener(SpriteCallBack)
            xuanzequyu:setTag(i)
            xuanzequyu:setName("xuanzequyu_"..i)
            self.testXuanze = ccui.Helper:seekWidgetByName(self.rootNode, "xuanzequyu_".."1")
        	
            local mt = cc.MoveBy:create(15+(i-1)*1.5,cc.p(1500+(i-1)*150,0))
            local seq = cc.Sequence:create(mt,cc.Place:create(cc.p(-100-150*(i-1),WIN_SIZE.height/2+h*(50+i*10))))
            

            sprite:runAction(cc.RepeatForever:create(seq))
            
        end
        
            ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(armatureFile)
    end)
end

--init your ui here
function IndianaUi:initUI()
    
end

--refresh all ui here
function IndianaUi:refreshUI()
end

return IndianaUi
