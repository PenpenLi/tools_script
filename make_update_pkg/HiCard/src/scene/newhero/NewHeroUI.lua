--NewHeroUI.lua, created by php script. chris.li--
require "Cocos2d"

local PopDialog = require "common/PopDialog"

local soulConfig = require("config.define_soul")

local npcConfig = require("config.define_npc_map")

local shopConfig = require("config.define_shop")

local mapConfig = require("config.define_stage")

local heroMainlayer = nil

local heroUplayer = nil

local heroSkilllayer = nil

local NewHeroUI = class("NewHeroUI", function()
	return cc.Layer:create()
end)

function NewHeroUI:create()
	local layer = NewHeroUI.new()
	layer:initM()
	return layer
end

function NewHeroUI:ctor()
end

function NewHeroUI:onEnter()
end

function NewHeroUI:onExit()
end

function NewHeroUI:initM()
	local function onNodeEvent(event)
		if event == "enter" then self:onEnter() 
		elseif event == "exit" then self:onExit() end
	end
	self:registerScriptHandler(onNodeEvent)

	self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("NewHeroUI.csb")
	self:addChild(self.rootNode)

	local function touchEventHandler(sender, event)
		self:handleTouchEvent(sender, event)
	end
	
    heroMainlayer = nil

    heroUplayer = nil

    heroSkilllayer = nil

	--m_layerPanel: Layout
	self.m_layerPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_layerPanel")
	self.m_layerPanel:addTouchEventListener(touchEventHandler)
    self.rootNode:setBackGroundImage("ditu.jpg")
    
    self.m_layerPanel2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_layerPanel2")

	--m_closebtn: Button
	self.m_closebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closebtn")
	self.m_closebtn:addTouchEventListener(touchEventHandler)

	--m_ScrollView: ScrollView
	self.m_ScrollView = ccui.Helper:seekWidgetByName(self.rootNode, "m_ScrollView")
	self.m_ScrollView:addTouchEventListener(touchEventHandler)
	
    --m_huadong
    self.m_huadong = ccui.Helper:seekWidgetByName(self.rootNode, "huadong")
    self.m_huadong:setVisible(false)
	
	self.oldBagId = 0
	self.isgo = 0
	self.checkskillitem =nil
	

	self:initUI()
end

function NewHeroUI:handleTouchEvent(sender, event)
    
	if sender == nil then
		cclog("[ERR]unknown sender")

	elseif sender == self.m_layerPanel and event == cc.EventCode.ENDED then
		cclog("[NewHeroUI]--m_layerPanel touched--")

	elseif sender == self.m_closebtn and event == cc.EventCode.ENDED then
		cclog("[NewHeroUI]--m_closebtn touched--")
        local scene = require("scene.home.HomeScene") : create()
        cc.Director:getInstance():replaceScene(scene)

	elseif sender == self.m_ScrollView then
		cclog("[NewHeroUI]--m_ScrollView touched--")
        SetSlider(self.m_ScrollView,self.m_huadong)
	end
end



function NewHeroUI:UpdateEQlist(data,type)
 
    local m_listPanel = ccui.Helper:seekWidgetByName(heroMainlayer, "m_listPanel")

    m_listPanel:setVisible(true)
    m_listPanel:setEnabled(true)
    
    local list_ListView = ccui.Helper:seekWidgetByName(m_listPanel, "list_ListView")
    list_ListView:removeAllItems()
    
    local EquipmentS = ccs.GUIReader:getInstance():widgetFromBinaryFile("TiejiangpuUI_One.csb")
    local listviewsize = list_ListView:getContentSize()
    listviewsize.height = EquipmentS:getContentSize().height*table.getn(data["equipment"])
    list_ListView:setInnerContainerSize(listviewsize)
   
    
    for i=1, table.getn(data["equipment"]) do
        local eqitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("TiejiangpuUI_One.csb")
        list_ListView:pushBackCustomItem(eqitem)
        
        local btn = ccui.Helper:seekWidgetByName(eqitem, "btn")
        btn:setTitleText("装备")
        
        local function btnCallBack(sender, event)
            if event == cc.EventCode.ENDED then
        		local tag = sender:getTag()
                Http:req("Equip",{rolerSoulId = self.lookid,oldBagId = self.oldBagId,position = type,
                    newBagId = tag},function (result)
                        if result.data["result"] == 0 then
                        	self.oldBagId = tag
                            Http:req("GetSoul",{id = self.lookid,soulId = self.looksoulid},function(data)
                                self:UpdateMainLayer(1,data.data)
                            end)
                        end	
                        m_listPanel:setVisible(false)
                    end)
        	end
        end
        
        btn:addTouchEventListener(btnCallBack)
        btn:setTag(data["equipment"][i]["bagId"])
        
        local eqlabel = ccui.Helper:seekWidgetByName(eqitem, "eqlabel")
        eqlabel:setVisible(false)
        
        local pinzhi = ccui.Helper:seekWidgetByName(eqitem, "pinzhi")
        
        local zbpic = ccui.Helper:seekWidgetByName(eqitem, "zbpic")
        
        local name = ccui.Helper:seekWidgetByName(eqitem, "name")
        
        local level = ccui.Helper:seekWidgetByName(eqitem, "level")
        level:setString(data["equipment"][i]["strengLv"])
        
        local sxpic1 = ccui.Helper:seekWidgetByName(eqitem, "sxpic1")

        local sxpic2 = ccui.Helper:seekWidgetByName(eqitem, "sxpic2")

        local shuxing1 = ccui.Helper:seekWidgetByName(eqitem, "shuxing1")

        local shuxing2 = ccui.Helper:seekWidgetByName(eqitem, "shuxing2")
        
        local shopId = tostring(data["equipment"][i]["shopId"])
        
        if  shopConfig[shopId] ~= nil then
            local dataitem=shopConfig[shopId]
            zbpic:loadTexture(dataitem["icon"]..".png")
            pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            name:setString(dataitem["name"].."+"..data["equipment"][i]["composeCount"])
        end
        
        if data["equipment"][i]["prop"]["atk"] ~= 0 then
            sxpic1:loadTexture("gong.png")
            shuxing1:setString(data["equipment"][i]["prop"]["atk"])
        elseif data["equipment"][i]["prop"]["def"] ~= 0 then
            sxpic1:loadTexture("fang.png")
            shuxing1:setString(data["equipment"][i]["prop"]["def"])
        elseif data["equipment"][i]["prop"]["hp"] ~= 0 then
            sxpic1:loadTexture("xue.png")
            shuxing1:setString(data["equipment"][i]["prop"]["hp"])
        elseif data["equipment"][i]["prop"]["zhili"] ~= 0 then
            sxpic1:loadTexture("zhili.png")
            shuxing1:setString(data["equipment"][i]["prop"]["zhili"])
        end

        if data["equipment"][i]["prop"]["cri"] ~= 0 then
            sxpic2:loadTexture("baoji.png")
            shuxing2:setString(data["equipment"][i]["prop"]["cri"])
        elseif data["equipment"][i]["prop"]["block"] ~= 0 then
            sxpic2:loadTexture("gedang.png")
            shuxing2:setString(data["equipment"][i]["prop"]["block"])
        elseif data["equipment"][i]["prop"]["agl"] ~= 0 then
            sxpic2:loadTexture("shanbi.png")
            shuxing2:setString(data["equipment"][i]["prop"]["agl"])
        elseif data["equipment"][i]["prop"]["dex"] ~= 0 then
            sxpic2:loadTexture("mingzhong.png")
            shuxing2:setString(data["equipment"][i]["prop"]["dex"])
        end
    end
end

function NewHeroUI:UpdateUpLayer(data)
    if heroUplayer == nil then
        heroUplayer = ccs.GUIReader:getInstance():widgetFromBinaryFile("NewHero_Uplevel.csb")
        self:addChild(heroUplayer)
        
        --m_backbtn: Button
        local m_backbtn = ccui.Helper:seekWidgetByName(heroUplayer, "m_backbtn")
        
        --m_upbtn: Button
        local m_upbtn = ccui.Helper:seekWidgetByName(heroUplayer, "m_upbtn")
        
        --m_heropic: ImageView
        local m_heropic = ccui.Helper:seekWidgetByName(heroUplayer, "m_heropic")
        
        local function UpLayerBtnCallBack(sender, event)
            if sender == m_backbtn and event == cc.EventCode.ENDED then
                heroUplayer:setVisible(false)
                heroUplayer:setEnabled(false)
            elseif sender == m_upbtn and event == cc.EventCode.BEGAN then
                self.isgo = 1
                Http:req("SoulUp",{id = self.lookid},function(res)
                    if res.data["result"] == 0 then
                        PopDialog:getInstance():popText("英雄升级成功！")
                        Http:req("GetSoulUp",{id = self.lookid},function(data)
                            self:UpdateUpLayer(data.data)
                            local texiao1 = cc.Sprite:create()
                            Helper.addSpriteFrames("yuanpan.plist","yuanpan.pvr.ccz")
                            local frameArr = Helper.newFrames("zhen%04d.png",1,10)
                            local animation = Helper.newAnimation(frameArr,1/10)
                            texiao1:setPosition(65,70)
                            texiao1:setScale(2)

                            local texiao2 = cc.Sprite:create()
                            Helper.addSpriteFrames("guangzhu.plist","guangzhu.pvr.ccz")
                            local frameArr1 = Helper.newFrames("light%04d.png",1,9)
                            local animation1 = Helper.newAnimation(frameArr1,1/9)
                            texiao2:setPosition(65,170)
                            texiao2:setScale(2)

                            m_heropic:addChild(texiao1,0)
                            m_heropic:addChild(texiao2,3)
                            local seq1 = (cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create()))
                            local seq2 = (cc.Sequence:create(cc.Animate:create(animation1),cc.RemoveSelf:create()))

                            texiao1:runAction(seq1)
                            texiao2:runAction(seq2)
                            
                            if self.isgo == 1 then
                                UpLayerBtnCallBack(m_upbtn, cc.EventCode.BEGAN)
                            end
                            
                        end)
                    elseif res.data["result"] == 17 then
                        PopDialog:getInstance():popText("不能超过战队等级！")
                    elseif res.data["result"] == 3 then
                        PopDialog:getInstance():popText("金币不足！")
                    elseif res.data["result"] == 16 then
                        PopDialog:getInstance():popText("经验药水不足！")
                    end
                end)
            elseif sender == m_upbtn and (event == cc.EventCode.ENDED or event == cc.EventCode.CANCELLED) then
                self.isgo = 0
            end
        end
        
        m_backbtn:addTouchEventListener(UpLayerBtnCallBack)
        m_upbtn:addTouchEventListener(UpLayerBtnCallBack)
    end
    heroUplayer:setVisible(true)
    heroUplayer:setEnabled(true)
   
    --m_heropic: ImageView
    local m_heropic = ccui.Helper:seekWidgetByName(heroUplayer, "m_heropic")


    --m_namelabel: Text
    local m_namelabel = ccui.Helper:seekWidgetByName(heroUplayer, "m_namelabel")

    --m_leveljindu: ImageView
    local m_leveljindu = ccui.Helper:seekWidgetByName(heroUplayer, "m_leveljindu")
    m_leveljindu:setScaleX(data["level"]/data["levelMax"])

    --m_atk: Text
    local m_atk = ccui.Helper:seekWidgetByName(heroUplayer, "m_atk")
    m_atk:setString(data["prop"]["atk"])

    --m_def: Text
    local m_def = ccui.Helper:seekWidgetByName(heroUplayer, "m_def")
    m_def:setString(data["prop"]["def"])

    --m_hp: Text
    local m_hp = ccui.Helper:seekWidgetByName(heroUplayer, "m_hp")
    m_hp:setString(data["prop"]["hp"])

    --m_zhili: Text
    local m_zhili = ccui.Helper:seekWidgetByName(heroUplayer, "m_zhili")
    m_zhili:setString(data["prop"]["zhili"])

    --m_agl: Text
    local m_agl = ccui.Helper:seekWidgetByName(heroUplayer, "m_agl")
    m_agl:setString(data["prop"]["agl"])

    --m_dex: Text
    local m_dex = ccui.Helper:seekWidgetByName(heroUplayer, "m_dex")
    m_dex:setString(data["prop"]["dex"])

    --m_cri: Text
    local m_cri = ccui.Helper:seekWidgetByName(heroUplayer, "m_cri")
    m_cri:setString(data["prop"]["cri"])

    --m_block: Text
    local m_block = ccui.Helper:seekWidgetByName(heroUplayer, "m_block")
    m_block:setString(data["prop"]["block"])

    --m_upbtn: Button
    local m_upbtn = ccui.Helper:seekWidgetByName(heroUplayer, "m_upbtn")
    m_upbtn:setTag(self.lookid)
    
    --m_atkadd: Text
    local m_atkadd = ccui.Helper:seekWidgetByName(heroUplayer, "m_atkadd")
    m_atkadd:setString("+0")

    --m_defadd: Text
    local m_defadd = ccui.Helper:seekWidgetByName(heroUplayer, "m_defadd")
    m_defadd:setString("+0")

    --m_hpadd: Text
    local m_hpadd = ccui.Helper:seekWidgetByName(heroUplayer, "m_hpadd")
    m_hpadd:setString("+0")

    --m_zhiliadd: Text
    local m_zhiliadd = ccui.Helper:seekWidgetByName(heroUplayer, "m_zhiliadd")
    m_zhiliadd:setString("+0")

    --m_agladd: Text
    local m_agladd = ccui.Helper:seekWidgetByName(heroUplayer, "m_agladd")
    m_agladd:setString("+0")

    --m_dexadd: Text
    local m_dexadd = ccui.Helper:seekWidgetByName(heroUplayer, "m_dexadd")
    m_dexadd:setString("+0")

    --m_criadd: Text
    local m_criadd = ccui.Helper:seekWidgetByName(heroUplayer, "m_criadd")
    m_criadd:setString("+0")

    --m_blockadd: Text
    local m_blockadd = ccui.Helper:seekWidgetByName(heroUplayer, "m_blockadd")
    m_blockadd:setString("+0")

    --m_yaoshui: Text
    local m_yaoshui = ccui.Helper:seekWidgetByName(heroUplayer, "m_yaoshui")
    m_yaoshui:setString(data["costDraugs"].."/"..data["hasDraugs"])

    --m_levelpic: ImageView
    local m_levelpic = ccui.Helper:seekWidgetByName(heroUplayer, "m_levelpic")
    m_levelpic:setPosition(cc.p(m_leveljindu:getPositionX()+m_leveljindu:getCustomSize().width*(data["level"]/data["levelMax"]),m_levelpic:getPositionY()))
    
    --m_levelnum: Text
    local m_levelnum = ccui.Helper:seekWidgetByName(heroUplayer, "m_levelnum")
    m_levelnum:setString(data["level"])
    
    if data["nextProp"] ~= nil then
        m_atkadd:setString("+"..data["nextProp"]["atk"]-data["prop"]["atk"])
        m_defadd:setString("+"..data["nextProp"]["def"]-data["prop"]["def"])
        m_hpadd:setString("+"..data["nextProp"]["hp"]-data["prop"]["hp"])
        m_zhiliadd:setString("+"..data["nextProp"]["zhili"]-data["prop"]["zhili"])
        m_agladd:setString("+"..data["nextProp"]["agl"]-data["prop"]["agl"])
        m_dexadd:setString("+"..data["nextProp"]["dex"]-data["prop"]["dex"])
        m_criadd:setString("+"..data["nextProp"]["cri"]-data["prop"]["cri"])
        m_blockadd:setString("+"..data["nextProp"]["block"]-data["prop"]["block"])
   end
   
    if  soulConfig[tostring(self.looksoulid)] ~= nil then
        local dataitem=soulConfig[tostring(self.looksoulid)]
        m_namelabel:setString(dataitem["name"])

        for i = 1, 5 do
            local xing = ccui.Helper:seekWidgetByName(heroUplayer, "xing_"..(i-1))
            xing:setVisible(false)
            if i <= tonumber(dataitem["quality"]) then
                xing:setVisible(true)
            end
        end

        m_heropic:removeAllChildren()
        local armature = Helper.createNpcArmature(tostring(dataitem["series"]))
        armature:setPosition(60,45)
        m_heropic:addChild(armature,1)
    end
    
end

function NewHeroUI:UpdateSkillLayer(data)
    
    if heroSkilllayer == nil then
        heroSkilllayer =ccs.GUIReader:getInstance():widgetFromBinaryFile("NewHero_Skill.csb")
        self:addChild(heroSkilllayer)
        
        
        
        local m_backbtn = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_backbtn")
        
        local m_upbtn = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_upbtn")
        
        local function SkillBtnCallBack(sender, event)
        	if sender == m_backbtn and event == cc.EventCode.ENDED then
                heroSkilllayer:setVisible(false)
                heroSkilllayer:setEnabled(false)
            elseif sender == m_upbtn and event == cc.EventCode.BEGAN then
                self.isgo = 1
                
                Http:req("NewBookUp",{id = data["book"][self.checkbookid]["id"]},function(res)
                    if res.data["result"] == 0 then
                        local armatureFile = "heffect/heffect.csb"
                        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
                        local skillarmature = ccs.Armature:create("heffect")
                        skillarmature:setScale(2)
                        skillarmature:setPosition(40,-20)
                        local pinzhi = ccui.Helper:seekWidgetByName(self.checkskillitem, "pinzhi")
                        pinzhi:removeAllChildren()
                        pinzhi:addChild(skillarmature)
                        skillarmature:getAnimation():playWithIndex(0)
                        
                        PopDialog:getInstance():popText("技能升级成功!")
                        
                        texiao:getAnimation():playWithIndex(0)
                        Http:req("GetNewBookUp",{id = self.lookid},function(data)
                            self:UpdateSkillLayer(data.data)
                            if self.isgo == 1 then
                                SkillBtnCallBack(m_upbtn,cc.EventCode.BEGAN)
                            end
                        end)
                    elseif res.data["result"] == 3 then
                        PopDialog:getInstance():popText("金币不足!")
                    elseif res.data["result"] == 19 then
                        PopDialog:getInstance():popText("技能等级不能超过英雄等级!")
                    elseif res.data["result"] == 16 then
                        PopDialog:getInstance():popText("技能经验药水不足!")
                    end
                end)
            elseif sender == m_upbtn and (event == cc.EventCode.ENDED or event == cc.EventCode.CANCELLED) then
                self.isgo = 0
        	end
        end
        
        m_backbtn:addTouchEventListener(SkillBtnCallBack)
        
        m_upbtn:addTouchEventListener(SkillBtnCallBack)
    end
    
    heroSkilllayer:setVisible(true)
    heroSkilllayer:setEnabled(true)
    
    --m_skillPanel: Layout
    local m_skillPanel = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_skillPanel")
    
    --m_ListView: ListView
    local m_ListView = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_ListView")
    
    --m_checkname: Text
    local m_checkname = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_checkname")

    --m_skilldesc: Text
    local m_skilldesc = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_skilldesc")

    --m_nowlevel: TextAtlas
    local m_nowlevel = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_nowlevel")
    m_nowlevel:setString(data["book"][self.checkbookid]["level"])
    
    --m_upbtn: Button
    local m_upbtn = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_upbtn")
    m_upbtn:setTag(self.checkbookid)

    --m_nextlevel: TextAtlas
    local m_nextlevel = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_nextlevel")
    m_nextlevel:setString(data["book"][self.checkbookid]["level"])
    if data["book"][self.checkbookid]["level"] < data["book"][self.checkbookid]["levelMax"] then
        m_nextlevel:setString(data["book"][self.checkbookid]["level"]+1)
        m_upbtn:loadTextureNormal("button_01.png")
        m_upbtn:setEnabled(true)
    else
        m_upbtn:loadTextureNormal("button_03.png")
        m_upbtn:setEnabled(false)
    end


    --m_yaoshui: Text
    local m_yaoshui = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_yaoshui")
    m_yaoshui:setString(data["book"][self.checkbookid]["costDraugs"].."/"..data["book"][self.checkbookid]["hasDraugs"])

    --m_jinbi: Text
    local m_jinbi = ccui.Helper:seekWidgetByName(heroSkilllayer, "m_jinbi")
    m_jinbi:setString(data["book"][self.checkbookid]["costCoins"])
    
    local shopId = tostring(data["book"][self.checkbookid]["bookId"])

    if  shopConfig[shopId] ~= nil then
        local dataitem=shopConfig[shopId]
        
        m_checkname:setString(dataitem["name"])
        m_skilldesc:setString(dataitem["desc"])
    end
    
    m_ListView:removeAllItems()
    m_ListView:setBounceEnabled(true)
    
    for i = 1, table.getn(data["book"]) do
        local skillitem = ccs.GUIReader:getInstance():widgetFromBinaryFile("NewHero_Skillone.csb")
        m_ListView:pushBackCustomItem(skillitem)
        
        if i == 1 then
            self.checkskillitem =skillitem
            local now = ccui.Helper:seekWidgetByName(self.checkskillitem, "ischeck")
            now:setVisible(true)
        end
        
        local function CheckBookCallBack(sender,event)
            if sender == skillitem and event == cc.EventCode.ENDED then
        		local tag = sender:getTag()
                local before = ccui.Helper:seekWidgetByName(self.checkskillitem, "ischeck")
                before:setVisible(false)
                local now = ccui.Helper:seekWidgetByName(sender, "ischeck")
                now:setVisible(true)
                self.checkskillitem = sender
                self.checkbookid = tag
        		
                m_nowlevel:setString(data["book"][tag]["level"])
                if data["book"][tag]["level"] < data["book"][tag]["levelMax"] then
                    m_nextlevel:setString(data["book"][tag]["level"]+1)
                end
                m_yaoshui:setString(data["book"][tag]["costDraugs"].."/"..data["book"][tag]["hasDraugs"])

                m_jinbi:setString(data["book"][tag]["costCoins"])
                
                local bookId = tostring(data["book"][tag]["bookId"])

                if  shopConfig[bookId] ~= nil then
                    local dataitem=shopConfig[bookId]

                    m_checkname:setString(dataitem["name"])
                    m_skilldesc:setString(dataitem["desc"])
                end
        	end
        end
        
        skillitem:addTouchEventListener(CheckBookCallBack)
        skillitem:setTag(i)
        
        local skillpic = ccui.Helper:seekWidgetByName(skillitem, "skillpic")
        
        local pinzhi = ccui.Helper:seekWidgetByName(skillitem, "pinzhi")
        
        local level = ccui.Helper:seekWidgetByName(skillitem, "level")
        level:setString("LV."..data["book"][i]["level"])
        
        local skillname = ccui.Helper:seekWidgetByName(skillitem, "skillname")
        
        if  shopConfig[tostring(data["book"][i]["bookId"])] ~= nil then
            local dataitem=shopConfig[tostring(data["book"][i]["bookId"])]

            skillname:setString(dataitem["name"])
            skillpic:loadTexture(dataitem["icon"]..".png")
            pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
        end
        
        
    end
    
end

function NewHeroUI:UpdateTips(data,type)
    local m_tipPanel = ccui.Helper:seekWidgetByName(heroMainlayer, "m_tipPanel")

    m_tipPanel:setVisible(true)
    m_tipPanel:setEnabled(true)
    
    local tip_tipbtn1 = ccui.Helper:seekWidgetByName(heroMainlayer, "tip_tipbtn1")
    tip_tipbtn1:setTag(type)
   
    local tip_eqpinzhi = ccui.Helper:seekWidgetByName(m_tipPanel, "tip_eqpinzhi")

    local tip_eqpic = ccui.Helper:seekWidgetByName(m_tipPanel, "tip_eqpic")

    local tip_eqname = ccui.Helper:seekWidgetByName(m_tipPanel, "tip_eqname")
    
    local tip_sxpic1 = ccui.Helper:seekWidgetByName(m_tipPanel, "tip_sxpic1")
    local tip_sxpic2 = ccui.Helper:seekWidgetByName(m_tipPanel, "tip_sxpic2")
    
    local tip_shuxing1 = ccui.Helper:seekWidgetByName(m_tipPanel, "tip_shuxing1")
    local tip_shuxing2 = ccui.Helper:seekWidgetByName(m_tipPanel, "tip_shuxing2")
    
    local shopId = tostring(data["shopId"])

    if  shopConfig[shopId] ~= nil then
        local dataitem=shopConfig[shopId]
        tip_eqpic:loadTexture(dataitem["icon"]..".png")
        tip_eqpinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
        tip_eqname:setString(dataitem["name"])
    end
    
    if data["prop"]["atk"] ~= 0 then
        tip_sxpic1:loadTexture("gong.png")
        tip_shuxing1:setString(data["prop"]["atk"])
    elseif data["prop"]["def"] ~= 0 then
        tip_sxpic1:loadTexture("fang.png")
        tip_shuxing1:setString(data["prop"]["def"])
    elseif data["prop"]["hp"] ~= 0 then
        tip_sxpic1:loadTexture("xue.png")
        tip_shuxing1:setString(data["prop"]["hp"])
    elseif data["prop"]["zhili"] ~= 0 then
        tip_sxpic1:loadTexture("zhili.png")
        tip_shuxing1:setString(data["prop"]["zhili"])
    end

    if data["prop"]["cri"] ~= 0 then
        tip_sxpic2:loadTexture("baoji.png")
        tip_shuxing2:setString(data["prop"]["cri"])
    elseif data["prop"]["block"] ~= 0 then
        tip_sxpic2:loadTexture("gedang.png")
        tip_shuxing2:setString(data["prop"]["block"])
    elseif data["prop"]["agl"] ~= 0 then
        tip_sxpic2:loadTexture("shanbi.png")
        tip_shuxing2:setString(data["prop"]["agl"])
    elseif data["prop"]["dex"] ~= 0 then
        tip_sxpic2:loadTexture("mingzhong.png")
        tip_shuxing2:setString(data["prop"]["dex"])
    end
    
   
end


function NewHeroUI:UpdateMainLayer(type,data)
    self.looksoulid = data["soul"]["soulId"]
    self.lookid = data["soul"]["id"]
    if heroMainlayer == nil then
        heroMainlayer = ccs.GUIReader:getInstance():widgetFromBinaryFile("NewHero_Main.csb")
        self:addChild(heroMainlayer)
        
        local m_mainPanel = ccui.Helper:seekWidgetByName(heroMainlayer, "m_mainPanel")
        
        --m_backbtn: Button
        local m_backbtn = ccui.Helper:seekWidgetByName(heroMainlayer, "m_backbtn")
        
        --m_btn3: Button
        local m_btn3 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_btn3")
        
        --m_btn1: Button
        local m_btn1 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_btn1")
        
        --m_btn0: Button
        local m_btn0 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_btn0")
        
        --m_btn0_0: Button
        local m_btn0_0 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_btn0_0")
        
        --m_btn2: Button
        local m_btn2 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_btn2")
        
        --m_eqpinzhi_0: ImageView
        local m_eqpinzhi_0 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpinzhi_0")

        --m_eqpinzhi_1: ImageView
        local m_eqpinzhi_1 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpinzhi_1")

        --m_eqpinzhi_2: ImageView
        local m_eqpinzhi_2 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpinzhi_2")

        --m_eqpinzhi_3: ImageView
        local m_eqpinzhi_3 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpinzhi_3")
        
        local list_closebtn = ccui.Helper:seekWidgetByName(heroMainlayer, "list_closebtn")
        
        local tip_closebtn = ccui.Helper:seekWidgetByName(heroMainlayer, "tip_closebtn")
        
        local tip_tipbtn1 = ccui.Helper:seekWidgetByName(heroMainlayer, "tip_tipbtn1")
        local tip_tipbtn2 = ccui.Helper:seekWidgetByName(heroMainlayer, "tip_tipbtn2")
        
        --m_heropic: ImageView
        local m_heropic = ccui.Helper:seekWidgetByName(heroMainlayer, "m_heropic")
        
        for i = 0,3 do
            local tujing = ccui.Helper:seekWidgetByName(heroMainlayer, "tujinbtn_"..i)
            tujing:setTag(i)
            
            local function tujingCallBack(sender, event)
                if event == cc.EventCode.ENDED then
                	local tag = sender:getTag()
                	--跳转到PVE对应管卡
                    local scene = require("scene.pve.PVEScene"):create({pveType = 1,stageId = tag})
                    cc.Director:getInstance():pushScene(cc.TransitionFadeTR:create(1,scene))
                end
            
            end
            
            tujing:addTouchEventListener(tujingCallBack)
        end
        
        local function mainBtnCallBack(sender, event)
            if sender == nil then
                cclog("[ERR]unknown sender")

            elseif sender == m_backbtn and event == cc.EventCode.ENDED then
                cclog("--m_backbtn touched--")
                heroMainlayer:setVisible(false)
                self.m_layerPanel2:setVisible(true)
                
                self:initUI()
                
            elseif sender == m_btn0_0 and event == cc.EventCode.ENDED then
                cclog("--m_btn0_0 touched--")
               
                Http:req("EnsureAdvance",{rolerSoulId = self.lookid},function(result)
                    if result.data["result"] == 0 then
                        Http:req("GetSoul",{id = self.lookid,soulId = self.looksoulid},function(data)
                            self:UpdateMainLayer(1,data.data)
                            
                            local texiao = cc.Sprite:create()
                            Helper.addSpriteFrames("herojjdi.plist","herojjdi.pvr.ccz")
                            local frameArr = Helper.newFrames("di%04d.png",1,17)
                            local animation = Helper.newAnimation(frameArr,1/17)
                            texiao:setPosition(40,0)

                            m_heropic:addChild(texiao,1)
                            texiao:runAction(cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create()))

                            local texiao2 = cc.Sprite:create()
                            Helper.addSpriteFrames("herojjguanghuan.plist","herojjguanghuan.pvr.ccz")
                            local frameArr2 = Helper.newFrames("guang%04d.png",1,17)
                            local animation2 = Helper.newAnimation(frameArr2,1/17)
                            texiao2:setPosition(35,80)

                            m_heropic:addChild(texiao2,3)
                            texiao2:runAction(cc.Sequence:create(cc.Animate:create(animation2),cc.RemoveSelf:create()))

                        end)
                    elseif result.data["result"] == 3 then
                        PopDialog:getInstance():popText("您的金币不足!")
                    elseif result.data["result"] == 13 then
                        PopDialog:getInstance():popText("该英雄碎片不足以进阶!")
                    end
                end)

            elseif sender == m_btn0 and event == cc.EventCode.ENDED then
                cclog("--m_btn0 touched--")
                Http:req("ComposeFragment",{soulId = self.looksoulid},function(result)
                    if result.data["result"] == 0 then
                        self.lookid = result.data["id"]
                    	Http:req("GetSoul",{id = self.lookid,soulId = self.looksoulid},function(data)
                            self:UpdateMainLayer(1,data.data)
                        end)
                    elseif result.data["result"] == 3 then
                        PopDialog:getInstance():popText("您的金币不足!")
                    elseif result.data["result"] == 13 then
                        PopDialog:getInstance():popText("该英雄碎片不足以合成!")
                    end
                end)

            elseif sender == m_btn1 and event == cc.EventCode.ENDED then
                cclog("--m_btn1 touched--")
                Http:req("GetSoulUp",{id = self.lookid},function(data)
                        self:UpdateUpLayer(data.data)
                end)
                
            elseif sender == m_btn2 and event == cc.EventCode.ENDED then
                cclog("--m_btn2 touched--")
                Http:req("GetNewBookUp",{id = self.lookid},function(data)
                    self.checkbookid = 1
                    self:UpdateSkillLayer(data.data)
                end)
                
            elseif sender == m_btn3 and event == cc.EventCode.ENDED then
                cclog("--m_btn3 touched--")
                Http:req("OnekeyEquip",{rolerSoulId = self.lookid},function(result)
                    if result.data["result"] == 0 then
                        Http:req("GetSoul",{id = self.lookid,soulId = self.looksoulid},function(data)
                            self:UpdateMainLayer(1,data.data)
                        end)
                    elseif result.data["result"] == 1 then
                        PopDialog:getInstance():popText("没有可装装备!")
                    end
                end)

            elseif sender == m_eqpinzhi_0 and event == cc.EventCode.ENDED then
                cclog("--m_eqpinzhi_0 touched--")
                local tag = sender:getTag()
                self.oldBagId = tag
                
                if tag == -1 then
                	
                elseif tag == 0 then
                    Http:req("EquipList",{rolerSoulId = self.lookid,type = 1},function(data)
                        self:UpdateEQlist(data.data,1)
                    end)
                else
                    Http:req("BagTip",{bagId = tag},function(data)
                        self:UpdateTips(data.data,1)
                    end)
                end

            elseif sender == m_eqpinzhi_1 and event == cc.EventCode.ENDED then
                cclog("--m_eqpinzhi_1 touched--")
                local tag = sender:getTag()
                self.oldBagId = tag
                
                if tag == -1 then

                elseif tag == 0 then
                    Http:req("EquipList",{rolerSoulId = self.lookid,type = 2},function(data)
                        self:UpdateEQlist(data.data,2)
                    end)
                else
                    Http:req("BagTip",{bagId = tag},function(data)
                        self:UpdateTips(data.data,2)
                    end)
                end

            elseif sender == m_eqpinzhi_2 and event == cc.EventCode.ENDED then
                cclog("--m_eqpinzhi_2 touched--")
                local tag = sender:getTag()
                self.oldBagId = tag
                
                if tag == -1 then

                elseif tag == 0 then
                    Http:req("EquipList",{rolerSoulId = self.lookid,type = 3},function(data)
                        self:UpdateEQlist(data.data,3)
                    end)
                else
                    Http:req("BagTip",{bagId = tag},function(data)
                        self:UpdateTips(data.data,3)
                    end)
                end

            elseif sender == m_eqpinzhi_3 and event == cc.EventCode.ENDED then
                cclog("--m_eqpinzhi_3 touched--")
                local tag = sender:getTag()
                self.oldBagId = tag
                
                if tag == -1 then

                elseif tag == 0 then
                    Http:req("EquipList",{rolerSoulId = self.lookid,type = 4},function(data)
                        self:UpdateEQlist(data.data,4)
                    end)
                else
                    Http:req("BagTip",{bagId = tag},function(data)
                        self:UpdateTips(data.data,4)
                    end)
                end
                
            elseif sender == list_closebtn and event == cc.EventCode.ENDED then
                local m_listPanel = ccui.Helper:seekWidgetByName(heroMainlayer, "m_listPanel")

                m_listPanel:setVisible(false)
                m_listPanel:setEnabled(false)
                
            elseif sender == tip_closebtn and event == cc.EventCode.ENDED then
                local m_tipPanel = ccui.Helper:seekWidgetByName(heroMainlayer, "m_tipPanel")

                m_tipPanel:setVisible(false)
                m_tipPanel:setEnabled(false)
                
            elseif sender == tip_tipbtn1 and event == cc.EventCode.ENDED then
                local type = sender:getTag()
                Http:req("EquipList",{rolerSoulId = self.lookid,type = type},function(data)
                    self:UpdateEQlist(data.data,type)
                end)
                
            elseif sender == tip_tipbtn2 and event == cc.EventCode.ENDED then
                --跳转到铁匠
                local scene = require("scene.tiejiangpu.TiejiangpuScene"):create()
                cc.Director:getInstance():pushScene(scene)
            end
        end

        m_backbtn:addTouchEventListener(mainBtnCallBack)
 
        m_btn3:addTouchEventListener(mainBtnCallBack)

        m_btn1:addTouchEventListener(mainBtnCallBack)

        m_btn0:addTouchEventListener(mainBtnCallBack)

        m_btn2:addTouchEventListener(mainBtnCallBack)
        
        m_btn0_0:addTouchEventListener(mainBtnCallBack)
        
        m_eqpinzhi_0:addTouchEventListener(mainBtnCallBack)
        
        m_eqpinzhi_1:addTouchEventListener(mainBtnCallBack)
        
        m_eqpinzhi_2:addTouchEventListener(mainBtnCallBack)
        
        m_eqpinzhi_3:addTouchEventListener(mainBtnCallBack)
        
        list_closebtn:addTouchEventListener(mainBtnCallBack)
        
        tip_closebtn:addTouchEventListener(mainBtnCallBack)
        
        tip_tipbtn1:addTouchEventListener(mainBtnCallBack)
        
        tip_tipbtn2:addTouchEventListener(mainBtnCallBack)

    end
    
    heroMainlayer:setVisible(true)
    self.m_layerPanel2:setVisible(false)
    
    local m_tipPanel = ccui.Helper:seekWidgetByName(heroMainlayer, "m_tipPanel")
    
    local m_listPanel = ccui.Helper:seekWidgetByName(heroMainlayer, "m_listPanel")
    
    m_tipPanel:setVisible(false)
    m_listPanel:setVisible(false)
    
    --m_btn3: Button
    local m_btn3 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_btn3")
   

    --m_btn1: Button
    local m_btn1 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_btn1")
   

    --m_btn0: Button
    local m_btn0 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_btn0")
    m_btn0:setVisible(false)

    --m_btn0_0: Button
    local m_btn0_0 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_btn0_0")
    

    --m_btn2: Button
    local m_btn2 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_btn2")
    
    
    for i = 0,3 do
        local tujing = ccui.Helper:seekWidgetByName(heroMainlayer, "tujinbtn_"..i)
        tujing:setVisible(false)
        tujing:setEnabled(false)
        if table.getn(data["origin"])>i then
            tujing:setVisible(true)
            tujing:setEnabled(true)
            
            local dataitem=mapConfig[tostring(data["origin"][i+1]["stageId"])]
            tujing:setTitleText(dataitem["stage_name"])
            
            if data["origin"][i+1]["opened"] == false then
                tujing:setEnabled(false)
                tujing:loadTextureNormal("newhero/anniu03.png")
            else
                tujing:loadTextureNormal("newhero/anniu01.png")
                tujing:setTag(tostring(data["origin"][i+1]["stageId"]))
            end
        end
    end
    
    --m_heropic: ImageView
    local m_heropic = ccui.Helper:seekWidgetByName(heroMainlayer, "m_heropic")
    

    --m_namelabel: Text
    local m_namelabel = ccui.Helper:seekWidgetByName(heroMainlayer, "m_namelabel")

    --m_atk: Text
    local m_atk = ccui.Helper:seekWidgetByName(heroMainlayer, "m_atk")
    m_atk:setString(data["prop"]["atk"])

    --m_def: Text
    local m_def = ccui.Helper:seekWidgetByName(heroMainlayer, "m_def")
    m_def:setString(data["prop"]["def"])

    --m_hp: Text
    local m_hp = ccui.Helper:seekWidgetByName(heroMainlayer, "m_hp")
    m_hp:setString(data["prop"]["hp"])

    --m_zhili: Text
    local m_zhili = ccui.Helper:seekWidgetByName(heroMainlayer, "m_zhili")
    m_zhili:setString(data["prop"]["zhili"])

    --m_agl: Text
    local m_agl = ccui.Helper:seekWidgetByName(heroMainlayer, "m_agl")
    m_agl:setString(data["prop"]["agl"])

    --m_dex: Text
    local m_dex = ccui.Helper:seekWidgetByName(heroMainlayer, "m_dex")
    m_dex:setString(data["prop"]["dex"])

    --m_cri: Text
    local m_cri = ccui.Helper:seekWidgetByName(heroMainlayer, "m_cri")
    m_cri:setString(data["prop"]["cri"])

    --m_block: Text
    local m_block = ccui.Helper:seekWidgetByName(heroMainlayer, "m_block")
    m_block:setString(data["prop"]["block"])
    
    --m_suipiannum: Text
    local m_suipiannum = ccui.Helper:seekWidgetByName(heroMainlayer, "m_suipiannum")
    m_suipiannum:setString(data["soul"]["frags"].."/"..data["soul"]["fragsMax"])
    
    --m_eqpic_0: ImageView
    local m_eqpic_0 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpic_0")
    m_eqpic_0:loadTexture("wuqi_jian.png")

    --m_eqpic_1: ImageView
    local m_eqpic_1 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpic_1")
    m_eqpic_1:loadTexture("wuqi_toukui.png")

    --m_eqpic_2: ImageView
    local m_eqpic_2 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpic_2")
    m_eqpic_2:loadTexture("wuqi_kuijia.png")

    --m_eqpic_3: ImageView
    local m_eqpic_3 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpic_3")
    m_eqpic_3:loadTexture("wuqi_shipin.png")

    --m_level: Text
    local m_level = ccui.Helper:seekWidgetByName(heroMainlayer, "m_level")
    m_level:setString(data["soul"]["level"])

    --m_zhanli: Text
    local m_zhanli = ccui.Helper:seekWidgetByName(heroMainlayer, "m_zhanli")
    m_zhanli:setString(data["fc"])

    --m_suipianjindu: ImageView
    local m_suipianjindu = ccui.Helper:seekWidgetByName(heroMainlayer, "m_suipianjindu")
    local baifenbi = 0
    if data["soul"]["frags"] > data["soul"]["fragsMax"] then
    	baifenbi = 1
    else
        baifenbi = data["soul"]["frags"]/data["soul"]["fragsMax"]
    end
    m_suipianjindu:setScaleX(baifenbi)

    --m_heroplace: ImageView
    local m_heroplace = ccui.Helper:seekWidgetByName(heroMainlayer, "m_heroplace")

    --m_eqpinzhi_0: ImageView
    local m_eqpinzhi_0 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpinzhi_0")
    m_eqpinzhi_0:loadTexture("normal_aochao_01.png")
    

    --m_eqpinzhi_1: ImageView
    local m_eqpinzhi_1 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpinzhi_1")
    m_eqpinzhi_1:loadTexture("normal_aochao_01.png")
    

    --m_eqpinzhi_2: ImageView
    local m_eqpinzhi_2 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpinzhi_2")
    m_eqpinzhi_2:loadTexture("normal_aochao_01.png")
    

    --m_eqpinzhi_3: ImageView
    local m_eqpinzhi_3 = ccui.Helper:seekWidgetByName(heroMainlayer, "m_eqpinzhi_3")
    m_eqpinzhi_3:loadTexture("normal_aochao_01.png")
    
    --type:1为已召唤，其他为未召唤
    if type == 1 then
        m_btn0:setVisible(false)
        m_btn0_0:setVisible(true)
        m_btn1:setEnabled(true)
        m_btn1:loadTextures("anniu.png","anniu00.png","anniu001.png")
        m_btn2:setEnabled(true)
        m_btn2:loadTextures("anniu.png","anniu00.png","anniu001.png")
        m_btn3:setEnabled(true)
        m_btn3:loadTextures("anniu.png","anniu00.png","anniu001.png")
        
        m_eqpic_0:setVisible(true)
       
        m_eqpic_1:setVisible(true)
       
        m_eqpic_2:setVisible(true)
        
        m_eqpic_3:setVisible(true)
        
        
        m_eqpinzhi_0:setTag(data["weaponId"]["id"])
        m_eqpinzhi_1:setTag(data["headId"]["id"])
        m_eqpinzhi_2:setTag(data["clothingId"]["id"])
        m_eqpinzhi_3:setTag(data["accessoryId"]["id"])
        
        if data["weaponId"]["id"] == 0 then
            m_eqpic_0:loadTexture("wuqi_jian01.png")
        elseif data["weaponId"]["id"] > 0 then
            local dataitem=shopConfig[tostring(data["weaponId"]["shopId"])]
            m_eqpic_0:loadTexture(dataitem["icon"]..".png")
            m_eqpinzhi_0:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            
            local texiao = cc.Sprite:create()
            Helper.addSpriteFrames("zbguanghuan.plist","zbguanghuan.pvr.ccz")
            local frameArr = Helper.newFrames("1%04d.png",1,12)
            local animation = Helper.newAnimation(frameArr,1/12)
            texiao:setPosition(43,43)
            
            m_eqpinzhi_0:addChild(texiao)
            
            texiao:runAction(cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create()))
                
        end

        if data["headId"]["id"] == 0 then
            m_eqpic_1:loadTexture("wuqi_toukui01.png")
        elseif data["headId"]["id"] > 0 then
            local dataitem=shopConfig[tostring(data["headId"]["shopId"])]
            m_eqpic_1:loadTexture(dataitem["icon"]..".png")
            m_eqpinzhi_1:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            
            local texiao = cc.Sprite:create()
            Helper.addSpriteFrames("zbguanghuan.plist","zbguanghuan.pvr.ccz")
            local frameArr = Helper.newFrames("1%04d.png",1,12)
            local animation = Helper.newAnimation(frameArr,1/12)
            texiao:setPosition(43,43)

            m_eqpinzhi_1:addChild(texiao)

            texiao:runAction(cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create()))

        end

        if data["clothingId"]["id"] == 0 then
            m_eqpic_2:loadTexture("wuqi_kuijia01.png")
        elseif data["clothingId"]["id"] > 0 then
            local dataitem=shopConfig[tostring(data["clothingId"]["shopId"])]
            m_eqpic_2:loadTexture(dataitem["icon"]..".png")
            m_eqpinzhi_2:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            
            local texiao = cc.Sprite:create()
            Helper.addSpriteFrames("zbguanghuan.plist","zbguanghuan.pvr.ccz")
            local frameArr = Helper.newFrames("1%04d.png",1,12)
            local animation = Helper.newAnimation(frameArr,1/12)
            texiao:setPosition(43,43)

            m_eqpinzhi_2:addChild(texiao)

            texiao:runAction(cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create()))

        end

        if data["accessoryId"]["id"] == 0 then
            m_eqpic_3:loadTexture("wuqi_shipin01.png")
        elseif data["accessoryId"]["id"] > 0 then
            local dataitem=shopConfig[tostring(data["accessoryId"]["shopId"])]
            m_eqpic_3:loadTexture(dataitem["icon"]..".png")
            m_eqpinzhi_3:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            
            local texiao = cc.Sprite:create()
            Helper.addSpriteFrames("zbguanghuan.plist","zbguanghuan.pvr.ccz")
            local frameArr = Helper.newFrames("1%04d.png",1,12)
            local animation = Helper.newAnimation(frameArr,1/12)
            texiao:setPosition(43,43)

            m_eqpinzhi_3:addChild(texiao)

            texiao:runAction(cc.Sequence:create(cc.Animate:create(animation),cc.RemoveSelf:create()))

        end
    else
        m_btn0:setVisible(true)
        m_btn0_0:setVisible(false)
        m_btn1:setEnabled(false)
        m_btn1:loadTextures("anniu001.png","anniu001.png","anniu001.png")
        m_btn2:setEnabled(false)
        m_btn2:loadTextures("anniu001.png","anniu001.png","anniu001.png")
        m_btn3:setEnabled(false)
        m_btn3:loadTextures("anniu001.png","anniu001.png","anniu001.png")
        
        m_eqpic_0:loadTexture("weikaifang.png")
        m_eqpic_0:setEnabled(false)
        m_eqpic_1:loadTexture("weikaifang.png")
        m_eqpic_1:setEnabled(false)
        m_eqpic_2:loadTexture("weikaifang.png")
        m_eqpic_2:setEnabled(false)
        m_eqpic_3:loadTexture("weikaifang.png")
        m_eqpic_3:setEnabled(false)
        
        
    end
    
    if  soulConfig[tostring(self.looksoulid)] ~= nil then
        local dataitem=soulConfig[tostring(self.looksoulid)]
        m_namelabel:setString(dataitem["name"])
        
        if tonumber(dataitem["quality"]) == 5 then
        	m_btn0_0:setVisible(false)
        end
        
        local site = math.floor(tonumber(dataitem["site"])/100)
        
        m_heroplace:loadTexture("place_"..site..".png")
        
        for i = 1, 5 do
            local xing = ccui.Helper:seekWidgetByName(heroMainlayer, "xing_"..(i-1))
            xing:setVisible(false)
            if i <= tonumber(dataitem["quality"]) then
                xing:setVisible(true)
            end
        end
        
        m_heropic:removeAllChildren()
        local armature = Helper.createNpcArmature(tostring(dataitem["series"]))
        armature:setPosition(40,0)
        m_heropic:addChild(armature,2)
    end

end

--init your ui here
function NewHeroUI:initUI()
    Http:req("GetAllSoul",nil,function(res)
        self.calledlist = res.data["called"]
        self.nocalledlist = res.data["notCalled"]
        
        self.m_ScrollView:removeAllChildren()
        self.m_ScrollView:setBounceEnabled(true)
        
        local ScrovSize = self.m_ScrollView:getInnerContainerSize()
        
        local xian = ccui.ImageView:create()
        xian:loadTexture("di02.png")
        
        local havenum = table.getn(self.calledlist)
        local nohavenum = table.getn(self.nocalledlist)
        local xianheght = xian:getCustomSize().height
        local haveheght = (math.floor(havenum/4)+1)*150
        local nohaveheght = (math.floor(nohavenum/4)+1)*175
        if havenum%4 == 0 then
            haveheght = haveheght-150
        end
        if nohavenum%4 == 0 then
        	nohaveheght = nohaveheght -175
        end
        
        ScrovSize.height = xianheght + haveheght + nohaveheght+15

        self.m_ScrollView:setInnerContainerSize(ScrovSize)
        
        local NewHero_Summoned2 = ccs.GUIReader:getInstance():widgetFromBinaryFile("NewHero_Summoned.csb")

        for i = 1, havenum do
            local NewHero_Summoned = NewHero_Summoned2:clone()
            self.m_ScrollView:addChild(NewHero_Summoned)
            
            local soulid = tostring(self.calledlist[i]["soulId"])
            
            local namelabel = ccui.Helper:seekWidgetByName(NewHero_Summoned, "namelabel")
            local heropic = ccui.Helper:seekWidgetByName(NewHero_Summoned, "heropic")
            local hero_pinzhi = ccui.Helper:seekWidgetByName(NewHero_Summoned, "hero_pinzhi")
            local tip = ccui.Helper:seekWidgetByName(NewHero_Summoned, "tip")
            local level = ccui.Helper:seekWidgetByName(NewHero_Summoned, "level")
            
            tip:setVisible(false)
            level:setString(self.calledlist[i]["level"])
            
            local function goMain1CallBack(sender, event)
                SetSlider(self.m_ScrollView,self.m_huadong)
                if event == cc.EventCode.ENDED then
                    local tag = sender:getTag()

                    Http:req("GetSoul",{id = self.calledlist[tag]["id"],soulId = self.calledlist[tag]["soulId"]},function(data)
                        self:UpdateMainLayer(1,data.data)
                    end)
                end
             
            end
            
            if  soulConfig[soulid] ~= nil then
                local dataitem=soulConfig[soulid]
                hero_pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                heropic:loadTexture(dataitem["icon"]..".png")
                namelabel:setString(dataitem["name"])

            end
            
            NewHero_Summoned:setPosition(cc.p(25+(i-1)%4*185,ScrovSize.height-150*(math.floor((i-1)/4)+1)))
            NewHero_Summoned:addTouchEventListener(goMain1CallBack)
            NewHero_Summoned:setTag(i)
            
        end
        
        self.m_ScrollView:addChild(xian)
        xian:setPosition(cc.p(ScrovSize.width/2,ScrovSize.height-haveheght-20))
        
        local NewHero_Nohave2 = ccs.GUIReader:getInstance():widgetFromBinaryFile("NewHero_Nohave.csb")

        for i = 1, nohavenum do
            local NewHero_Nohave = NewHero_Nohave2:clone()
            self.m_ScrollView:addChild(NewHero_Nohave)
            
            local soulid = tostring(self.nocalledlist[i]["soulId"])

            local namelabel = ccui.Helper:seekWidgetByName(NewHero_Nohave, "namelabel")
            local heropic = ccui.Helper:seekWidgetByName(NewHero_Nohave, "heropic")
            local hero_pinzhi = ccui.Helper:seekWidgetByName(NewHero_Nohave, "hero_pinzhi")
            local jindunum = ccui.Helper:seekWidgetByName(NewHero_Nohave, "jindunum")
            local suipiantiao = ccui.Helper:seekWidgetByName(NewHero_Nohave, "suipiantiao")
            jindunum:setString(self.nocalledlist[i]["frags"].."/"..self.nocalledlist[i]["fragsMax"])
            suipiantiao:setScaleX(self.nocalledlist[i]["frags"]/self.nocalledlist[i]["fragsMax"])
            if self.nocalledlist[i]["frags"] >= self.nocalledlist[i]["fragsMax"] then
                suipiantiao:setScaleX(1)
            end
            

            local function goMain2CallBack(sender, event)
                SetSlider(self.m_ScrollView,self.m_huadong)
                if event == cc.EventCode.ENDED then
                    local tag = sender:getTag()

                    Http:req("GetSoul",{id = self.nocalledlist[tag]["id"],soulId = self.nocalledlist[tag]["soulId"]},function(data)
                        self:UpdateMainLayer(2,data.data)
                    end)
                end
                
            end
            
            if  soulConfig[soulid] ~= nil then
                local dataitem=soulConfig[soulid]
                hero_pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                heropic:loadTexture(dataitem["icon"]..".png")
                local texture = heropic:getVirtualRenderer()
                Helper.graySprite(texture)
                namelabel:setString(dataitem["name"])

            end

            NewHero_Nohave:setPosition(cc.p(25+(i-1)%4*185,ScrovSize.height-haveheght-10-xianheght-175*(math.floor((i-1)/4)+1)))
            NewHero_Nohave:addTouchEventListener(goMain2CallBack)
            NewHero_Nohave:setTag(i)

        end
    end)
end

--refresh all ui here
function NewHeroUI:refreshUI()
end

return NewHeroUI
