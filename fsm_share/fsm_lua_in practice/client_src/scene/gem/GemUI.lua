--GemUI.lua, created by php script. chris.li--
require "Cocos2d"

local PopDialog = require "common/PopDialog"

local shopConfig = require("config.define_shop")

local soulConfig = require("config.define_soul")

local gemcomposeConfig = require("config.define_gem_compose")

local PopDialog = require "common/PopDialog"

local GemUI = class("GemUI", function()
    return cc.Layer:create()
end)

function GemUI:create()
    local layer = GemUI.new()
    layer:initM()
    return layer
end

function GemUI:ctor()
end

function GemUI:onEnter()
end

function GemUI:onExit()
end

function GemUI:initM()
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter()
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)

    self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("GemUI.csb")
    self.rootNode:setBackGroundImage("ditu.jpg")
    self:addChild(self.rootNode)

    local function touchEventHandler(sender, event)
        self:handleTouchEvent(sender, event)
    end

    --m_backgroundPanel: Layout
    self.m_backgroundPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_backgroundPanel")
    self.m_backgroundPanel:addTouchEventListener(touchEventHandler)

    --m_hcPanel: Layout
    self.m_hcPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_hcPanel")
    self.m_hcPanel:addTouchEventListener(touchEventHandler)
    self.m_hcPanel:setVisible(false)

    --m_gemlistPanel: Layout
    self.m_gemlistPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_gemlistPanel")
    self.m_gemlistPanel:addTouchEventListener(touchEventHandler)
    self.m_gemlistPanel:setVisible(false)

    --m_closebtn: Button
    self.m_closebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_closebtn")
    self.m_closebtn:addTouchEventListener(touchEventHandler)

    --m_hcgemPanel: ImageView
    self.m_hcgemPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_hcgemPanel")

    --m_paneltop: Layout
    self.m_paneltop = ccui.Helper:seekWidgetByName(self.rootNode, "m_paneltop")
    self.m_paneltop:addTouchEventListener(touchEventHandler)
    self.m_paneltop:setVisible(false)
    
    --m_UnInlayPanel: Layout
    self.m_UnInlayPanel = ccui.Helper:seekWidgetByName(self.rootNode, "m_UnInlayPanel")
    self.m_UnInlayPanel:addTouchEventListener(touchEventHandler)
    self.m_UnInlayPanel:setVisible(false)

    --m_scrollview: ScrollView
    self.m_scrollview = ccui.Helper:seekWidgetByName(self.rootNode, "m_scrollview")
    self.m_scrollview:addTouchEventListener(touchEventHandler)
    self.m_scrollview:setVisible(false)

    --m_hcgempic: ImageView
    self.m_hcgempic = ccui.Helper:seekWidgetByName(self.rootNode, "m_hcgempic")

    --m_hcbtnone: Button
    self.m_hcbtnone = ccui.Helper:seekWidgetByName(self.rootNode, "m_hcbtnone")
    self.m_hcbtnone:addTouchEventListener(touchEventHandler)

    --m_hcbtnten: Button
    self.m_hcbtnten = ccui.Helper:seekWidgetByName(self.rootNode, "m_hcbtnten")
    self.m_hcbtnten:addTouchEventListener(touchEventHandler)

    --m_backbtn: Button
    self.m_backbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_backbtn")
    self.m_backbtn:addTouchEventListener(touchEventHandler)
    
    --m_surebtn: Button
    self.m_surebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_surebtn")
    self.m_surebtn:addTouchEventListener(touchEventHandler)
    
    --m_canclebtn: Button
    self.m_canclebtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_canclebtn")
    self.m_canclebtn:addTouchEventListener(touchEventHandler)

    --m_hclistview: ListView
    self.m_hclistview = ccui.Helper:seekWidgetByName(self.rootNode, "m_hclistview")
    self.m_hclistview:addTouchEventListener(touchEventHandler)

    --m_gemlistview: ListView
    self.m_gemlistview = ccui.Helper:seekWidgetByName(self.rootNode, "m_gemlistview")
    self.m_gemlistview:addTouchEventListener(touchEventHandler)


    --m_tips: Text
    self.m_tips = ccui.Helper:seekWidgetByName(self.rootNode, "m_tips")
    self.m_tips:setString("点击左上角选择框选择需要镶嵌宝石的装备")
    
    --m_uninlaytips: Text
    self.m_uninlaytips = ccui.Helper:seekWidgetByName(self.rootNode, "m_uninlaytips")

    --m_eqername: Text
    self.m_eqername = ccui.Helper:seekWidgetByName(self.rootNode, "m_eqername")

    --m_hcbtn: Button
    self.m_hcbtn = ccui.Helper:seekWidgetByName(self.rootNode, "m_hcbtn")
    self.m_hcbtn:addTouchEventListener(touchEventHandler)

    --m_selectpic: ImageView
    self.m_selectpic = ccui.Helper:seekWidgetByName(self.rootNode, "m_selectpic")

    --m_hcgempinzhi: ImageView
    self.m_hcgempinzhi = ccui.Helper:seekWidgetByName(self.rootNode, "m_hcgempinzhi")

    --m_shuxing_1: Text
    self.m_shuxing_1 = ccui.Helper:seekWidgetByName(self.rootNode, "m_shuxing_1")

    --m_shuxing_2: Text
    self.m_shuxing_2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_shuxing_2")

    --m_shuxing_3: Text
    self.m_shuxing_3 = ccui.Helper:seekWidgetByName(self.rootNode, "m_shuxing_3")

    --m_shuxing_4: Text
    self.m_shuxing_4 = ccui.Helper:seekWidgetByName(self.rootNode, "m_shuxing_4")

    --m_shuxing_5: Text
    self.m_shuxing_5 = ccui.Helper:seekWidgetByName(self.rootNode, "m_shuxing_5")

    --m_isnull: Text
    self.m_isnull = ccui.Helper:seekWidgetByName(self.rootNode, "m_isnull")
    self.m_isnull:setVisible(false)
    self.m_isnull:setString("当前你没有蓝色及以上的装备")
    self.m_isnullstatus = false
    
    --m_isnull2: Text
    self.m_isnull2 = ccui.Helper:seekWidgetByName(self.rootNode, "m_isnull2")
    self.m_isnull2:setVisible(false)
    self.m_isnull2:setString("当前你没有剩余宝石可以镶嵌")

    --m_selectpinzhi: ImageView
    self.m_selectpinzhi = ccui.Helper:seekWidgetByName(self.rootNode, "m_selectpinzhi")
    self.m_selectpinzhi:addTouchEventListener(touchEventHandler)
    
    --m_mubiao: ImageView
    self.m_mubiao = ccui.Helper:seekWidgetByName(self.rootNode, "m_mubiao")
    self.m_mubiao:addTouchEventListener(touchEventHandler)
    
    --m_Slider
    self.m_Slider = ccui.Helper:seekWidgetByName(self.rootNode, "m_Slider")
    self.m_Slider:setVisible(false)
    
    self.mygemlist={}
    self.gemtypelist = {}
    self.gemtypeId={}
    local i = 0
    for k, v in pairs(shopConfig) do
        if  tonumber(shopConfig[k]["type"]) == 6 then
            i = i+1
            self.gemtypelist[tonumber(k)] = v
            self.gemtypeId[i] = tonumber(k)
            
        end
    end

    table.sort(self.gemtypeId,function(a,b) return a<b end)
    for j = 1, i do
        print(self.gemtypeId[j])
    end

    self.checkEquipID = 0
    self.checkEquipshopID = 0
    self.checkGemID = 0
    self.checkGemHoleID = 0
    self.materialID = 0
    self.ComposeGemID = 0
    self.eqername = ""
    self.checkTypeGemTag = 0

    self:initUI()
end

function GemUI:handleTouchEvent(sender, event)
    if sender == nil then
        cclog("[ERR]unknown sender")

    elseif sender == self.m_backgroundPanel and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_backgroundPanel touched--")

    elseif sender == self.m_hcPanel and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_hcPanel touched--")

    elseif sender == self.m_gemlistPanel and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_gemlistPanel touched--")
        self.m_gemlistPanel:setVisible(false)

    elseif sender == self.m_closebtn and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_closebtn touched--")
        local scene = require("scene.home.HomeScene"):create()
        cc.Director:getInstance():replaceScene(scene)

    elseif sender == self.m_paneltop and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_paneltop touched--")

    elseif sender == self.m_selectpinzhi and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_selectpinzhi touched--")
        self.m_scrollview:setVisible(true)
        self.m_isnull:setVisible(self.m_isnullstatus)
        self.m_tips:setString("已有装备列表已出现，请选择")

    elseif sender == self.m_scrollview then
        cclog("[GemUI]--m_scrollview touched--")
        SetSlider(self.m_scrollview,self.m_Slider)

    elseif sender == self.m_hcbtnone and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_hcbtnone touched--")
        if self.ComposeGemID == 0 then
            PopDialog:getInstance():popText("请选择合成目标再进行操作!")
            return
        end
        if self.materialID == 0 then
            PopDialog:getInstance():popText("宝石数量不足，听说神秘商店在出售!")
            return
        end
        Http:req("ComposeGem",{num = 1,gemId = self.materialID},function(res)
            if res.data["result"] == 0 then
                PopDialog:getInstance():popText("合成成功!")
                self:updateGemListView()
            elseif res.data["result"] == 1 then
                PopDialog:getInstance():popText("宝石数量不足，听说神秘商店在出售!")
            elseif res.data["result"] == 3 then
                PopDialog:getInstance():popText("合成失败，金币不足!")
            end
        end)

    elseif sender == self.m_hcbtnten and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_hcbtnten touched--")
        if self.ComposeGemID == 0 then
            PopDialog:getInstance():popText("请选择合成目标再进行操作!")
            return
        end
        if self.materialID == 0 then
            PopDialog:getInstance():popText("宝石数量不足，听说神秘商店在出售!")
            return
        end
        Http:req("ComposeGem",{num = 10,gemId = self.materialID},function(res)
            if res.data["result"] == 0 then
                PopDialog:getInstance():popText("合成成功!")
                self:updateGemListView()
            elseif res.data["result"] == 1 then
                PopDialog:getInstance():popText("宝石数量不足，听说神秘商店在出售!")
            elseif res.data["result"] == 3 then
                PopDialog:getInstance():popText("合成失败，金币不足!")
            end
        end)
        
    elseif sender == self.m_surebtn and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_surebtn touched--")
        Http:req("UnInlay",{equipId = self.checkEquipID,hole = self.checkGemHoleID},function(res)
            if res.data["result"] == 0 then
                PopDialog:getInstance():popText("你刚刚摘除了一个宝石快去镶嵌吧!")
                self:showUpPanel(self.checkEquipID,self.checkEquipshopID)
                self:updateGemListView()
                
                self:showtexiao(res.data["propMf"],2)
            else
                PopDialog:getInstance():popText("钻石不足，快去充值吧!")
            end
        end)
        self.m_UnInlayPanel:setVisible(false)
        
    elseif sender == self.m_canclebtn and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_canclebtn touched--")
        self.m_UnInlayPanel:setVisible(false)

    elseif sender == self.m_backbtn and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_backbtn touched--")
        self.m_hcPanel:setVisible(false)

    elseif sender == self.m_hclistview and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_hclistview touched--")

    elseif sender == self.m_gemlistview and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_gemlistview touched--")

    elseif sender == self.m_hcbtn and event == cc.EventCode.ENDED then
        cclog("[GemUI]--m_hcbtn touched--")
        self.m_hcPanel:setVisible(true)

    end
end

function GemUI:updateScrollView()
    self.m_scrollview:removeAllChildren()
    Http:req("GetAllGemEquip",nil,function(res)
        local equipList = res.data["equip"]
        local equipListSize = table.getn(equipList)
        
        if equipListSize == 0 then
            self.m_isnullstatus = true
        else
            self.m_isnullstatus = false
        end
        
        self.m_scrollview:setBounceEnabled(true)

        local ScrovSize = self.m_scrollview:getInnerContainerSize()

        local ScrovH = ScrovSize.height
        if equipListSize>12 then
            if equipListSize%4 == 0 then
                ScrovH = (90+10)*(equipListSize/4)
            else
                ScrovH = (90+10)*(math.floor(equipListSize/4)+1)
            end

        end

        ScrovSize.height = ScrovH
        self.m_scrollview:setInnerContainerSize(ScrovSize)
        
        for i=1, equipListSize do
            local equipItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("GemUI_Item.csb")
            self.m_scrollview:addChild(equipItem)
            equipItem:setTag(i)
            equipItem:setPosition((i-1)%4*130,ScrovSize.height-100*(math.floor((i-1)/4)+1))
            
            local pinzhi = ccui.Helper:seekWidgetByName(equipItem, "pinzhi")
            local pic = ccui.Helper:seekWidgetByName(equipItem, "pic")
            local eqer_pic = ccui.Helper:seekWidgetByName(equipItem, "eqer_pic")
            
            if shopConfig[tostring(equipList[i]["shopId"])] ~= nil then
                local dataitem=shopConfig[tostring(equipList[i]["shopId"])]
                pic:loadTexture(dataitem["icon"]..".png")
                pinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
            end
            
            if equipList[i]["soulId"] == 0 then
            	eqer_pic:setVisible(false)
            else
                if soulConfig[tostring(equipList[i]["soulId"])] ~= nil then
                    local dataitem=soulConfig[tostring(equipList[i]["soulId"])]
                    eqer_pic:loadTexture("touxiang_pic_soulicon"..dataitem["series"]..".png")
                end
            end
            
            
            local function checkequip(sender, event)
                if sender == equipItem and event == cc.EventCode.ENDED then
                    local tag = sender:getTag()
                    self.m_tips:setString("现在可以对装备进行镶嵌，拆除操作了")
                    self.checkEquipID = equipList[tag]["equipId"]
                    self.checkEquipshopID = equipList[tag]["shopId"]
                    
                    if equipList[tag]["soulId"] == 0 then
                        
                    else
                        if soulConfig[tostring(equipList[tag]["soulId"])] ~= nil then
                            local dataitem=soulConfig[tostring(equipList[tag]["soulId"])]
                            self.m_eqername:setString(dataitem["name"])
                        end
                    end
                    
                    self:showUpPanel(equipList[tag]["equipId"],equipList[tag]["shopId"])
                end
            end
            
            equipItem:addTouchEventListener(checkequip)
        end
    end)
end

function GemUI:showUpPanel(CheckequipId,CheckequipshopId)
    Http:req("GetGemMain",{equipId = CheckequipId},function (res)
        if shopConfig[tostring(CheckequipshopId)] ~= nil then
            local dataitem=shopConfig[tostring(CheckequipshopId)]
            self.m_selectpic:loadTexture(dataitem["icon"]..".png")
            self.m_selectpinzhi:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
        end
    	
        local holelist = res.data["hole"]
        
        for i = 1, 5 do
            local gem = ccui.Helper:seekWidgetByName(self.m_paneltop, "gem_"..i)
            local gempinzhi = ccui.Helper:seekWidgetByName(gem, "gempinzhi")
            gem:loadTexture("touxiangkuang_02.png")
            gempinzhi:loadTexture("normal_aochao_01.png")
            gem:setVisible(false)
            
            local function checkHole(sender,event)
                if sender == gempinzhi and event == cc.EventCode.ENDED then
                    local tag = sender:getTag()
                    if tag < 0 then
                        self.checkGemHoleID = tag+10
                    	--未开孔，等级限制
                        PopDialog:getInstance():popText("该孔有战队等级限制!")
                    elseif math.floor(tag/10) == 0 and tag>0 then
                        --可镶嵌，显示自己的宝石列表
                        self.checkGemHoleID = tag%10-1
                        self.m_gemlistPanel:setVisible(true)
                    else
                        --摘除，弹出提示
                        self.checkGemHoleID = tag%10-1
                        self.checkGemID = math.floor(tag/10)
                        self.m_UnInlayPanel:setVisible(true)
                        self.m_uninlaytips:setString("是否消耗"..res.data["uninlayCost"].."钻来摘除此宝石，拆除后宝石将放入你的包裹")
                    end
            	end
            end
            
            
            
            if table.getn(holelist) >= i then
            	gem:setVisible(true)
                gempinzhi:setTag(holelist[i]*10+i)
                
                if holelist[i]>0 then
                    if shopConfig[tostring(holelist[i])] ~= nil then
                        local gemitem=shopConfig[tostring(holelist[i])]
                        gem:loadTexture(gemitem["icon"]..".png")
                        gempinzhi:loadTexture("normal_aochao_0"..gemitem["quality"]..".png")
                    end
                    
                elseif holelist[i] == -1 then
                    gem:loadTexture("weikaifang.png")
                    gempinzhi:loadTexture("normal_aochao_01.png")
                end
            end
            
            gempinzhi:addTouchEventListener(checkHole)
        end
    	
        self.m_paneltop:setVisible(true)
    end)
end

function GemUI:updateGemListView()
    self.m_gemlistview:removeAllItems()
    Http:req("GetGemList",nil,function(res)
        self.mygemlist = res.data["unit"]
        if self.checkTypeGemTag ~= 0 then
            self:updateHCPanel(self.checkTypeGemTag)
        end
        
        if table.getn(self.mygemlist) == 0 then
            self.m_isnull2:setVisible(true)
        else
            self.m_isnull2:setVisible(false)
        end
        for i = 1, table.getn(self.mygemlist) do
            local gemItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("GemUI_Item2.csb")
            self.m_gemlistview:pushBackCustomItem(gemItem)
            gemItem:setTag(self.mygemlist[i]["gemId"])
            
            local num = ccui.Helper:seekWidgetByName(gemItem, "num")
            num:setString(self.mygemlist[i]["num"])
            local pinzhi = ccui.Helper:seekWidgetByName(gemItem, "pinzhi")
            local pic = ccui.Helper:seekWidgetByName(gemItem, "pic")
            
            if shopConfig[tostring(self.mygemlist[i]["shopId"])] ~= nil then
                local gemitem=shopConfig[tostring(self.mygemlist[i]["shopId"])]
                pic:loadTexture(gemitem["icon"]..".png")
                pinzhi:loadTexture("normal_aochao_0"..gemitem["quality"]..".png")
            end
            
            local function checkInlayGem(sender,event)
                if sender == gemItem and event == cc.EventCode.ENDED then
                    self.checkGemID = sender:getTag()
                    Http:req("Inlay",{equipId = self.checkEquipID,gemId = self.checkGemID,hole = self.checkGemHoleID},function(result)
                        if result.data["result"] == 0 then
                            PopDialog:getInstance():popText("太棒了，你成功镶嵌了宝石!")
                            self:updateGemListView()
                            self.m_gemlistPanel:setVisible(false)
                            self:showUpPanel(self.checkEquipID,self.checkEquipshopID)
                            
                            self:showtexiao(result.data["propMf"],1)
                        end
                    end)
            	end
            end
            
            gemItem:addTouchEventListener(checkInlayGem)
        end
    end)
end

function GemUI:showtexiao(data,type)
    local num = 0
    if data["hp"] ~= 0 then
        local sprite = cc.Sprite:create()
        local text = ccui.Text:create()
        text:setFontSize(22)
        text:setPosition(40,80)
        if type == 1 then
            text:setText("血量:+"..data["hp"])
            text:setColor(getColorByQuality(2))
        else
            text:setText("血量:"..data["hp"])
            text:setColor(getColorByQuality(5))
        end
            
        sprite:addChild(text)
        self.m_selectpinzhi:addChild(sprite)
        sprite:runAction(cc.Sequence:create(cc.DelayTime:create(num*0.2),cc.MoveBy:create(0.5,cc.p(0,30)),cc.RemoveSelf:create()))
        num = num +1
    end
    if data["def"] ~= 0 then
        local sprite = cc.Sprite:create()
        local text = ccui.Text:create()
        text:setFontSize(22)
        text:setPosition(40,80)
        if type == 1 then
            text:setText("防御:+"..data["def"])
            text:setColor(getColorByQuality(2))
        else
            text:setText("防御:"..data["def"])
            text:setColor(getColorByQuality(5))
        end
        
        sprite:addChild(text)
        self.m_selectpinzhi:addChild(sprite)
        sprite:runAction(cc.Sequence:create(cc.DelayTime:create(num*0.2),cc.MoveBy:create(0.5,cc.p(0,30)),cc.RemoveSelf:create()))
        num = num +1
        end
    if data["atk"] ~= 0 then
        local sprite = cc.Sprite:create()
        local text = ccui.Text:create()
        text:setFontSize(22)
        text:setPosition(40,80)
        if type == 1 then
            text:setText("攻击:+"..data["atk"])
            text:setColor(getColorByQuality(2))
        else
            text:setText("攻击:"..data["atk"])
            text:setColor(getColorByQuality(5))
        end
        
        sprite:addChild(text)
        self.m_selectpinzhi:addChild(sprite)
        sprite:runAction(cc.Sequence:create(cc.DelayTime:create(num*0.2),cc.MoveBy:create(0.5,cc.p(0,30)),cc.RemoveSelf:create()))
        num = num +1
    end
    if data["zhili"] ~= 0 then
        local sprite = cc.Sprite:create()
        local text = ccui.Text:create()
        text:setFontSize(22)
        text:setPosition(40,80)
        if type == 1 then
            text:setText("智力:+"..data["zhili"])
            text:setColor(getColorByQuality(2))
        else
            text:setText("智力:"..data["zhili"])
            text:setColor(getColorByQuality(5))
        end
        
        sprite:addChild(text)
        self.m_selectpinzhi:addChild(sprite)
        sprite:runAction(cc.Sequence:create(cc.DelayTime:create(num*0.2),cc.MoveBy:create(0.5,cc.p(0,30)),cc.RemoveSelf:create()))
        num = num +1
    end
    if data["cri"] ~= 0 then
        local sprite = cc.Sprite:create()
        local text = ccui.Text:create()
        text:setFontSize(22)
        text:setPosition(40,80)
        if type == 1 then
            text:setText("暴击:+"..data["cri"])
            text:setColor(getColorByQuality(2))
        else
            text:setText("暴击:"..data["cri"])
            text:setColor(getColorByQuality(5))
        end
        
        sprite:addChild(text)
        self.m_selectpinzhi:addChild(sprite)
        sprite:runAction(cc.Sequence:create(cc.DelayTime:create(num*0.2),cc.MoveBy:create(0.5,cc.p(0,30)),cc.RemoveSelf:create()))
        num = num +1
    end
    if data["block"] ~= 0 then
        local sprite = cc.Sprite:create()
        local text = ccui.Text:create()
        text:setFontSize(22)
        text:setPosition(40,80)
        if type == 1 then
            text:setText("格挡:+"..data["block"])
            text:setColor(getColorByQuality(2))
        else
            text:setText("格挡:"..data["block"])
            text:setColor(getColorByQuality(5))
        end
        
        sprite:addChild(text)
        self.m_selectpinzhi:addChild(sprite)
        sprite:runAction(cc.Sequence:create(cc.DelayTime:create(num*0.2),cc.MoveBy:create(0.5,cc.p(0,30)),cc.RemoveSelf:create()))
        num = num +1
    end
    if data["agl"] ~= 0 then
        local sprite = cc.Sprite:create()
        local text = ccui.Text:create()
        text:setFontSize(22)
        text:setPosition(40,80)
        if type == 1 then
            text:setText("闪避:+"..data["agl"])
            text:setColor(getColorByQuality(2))
        else
            text:setText("闪避:"..data["agl"])
            text:setColor(getColorByQuality(5))
        end
        
        sprite:addChild(text)
        self.m_selectpinzhi:addChild(sprite)
        sprite:runAction(cc.Sequence:create(cc.DelayTime:create(num*0.2),cc.MoveBy:create(0.5,cc.p(0,30)),cc.RemoveSelf:create()))
        num = num +1
    end
    if data["dex"] ~= 0 then
        local sprite = cc.Sprite:create()
        local text = ccui.Text:create()
        text:setFontSize(22)
        text:setPosition(40,80)
        if type == 1 then
            text:setText("命中:+"..data["dex"])
            text:setColor(getColorByQuality(2))
        else
            text:setText("命中:"..data["dex"])
            text:setColor(getColorByQuality(5))
        end
        
        sprite:addChild(text)
        self.m_selectpinzhi:addChild(sprite)
        sprite:runAction(cc.Sequence:create(cc.DelayTime:create(num*0.2),cc.MoveBy:create(0.5,cc.p(0,30)),cc.RemoveSelf:create()))
        num = num +1
    end
    
    
end

function GemUI:updateHCPanel(tag)
    self.materialID = 0
    self.checkTypeGemTag = tag
    self.ComposeGemID = self.gemtypeId[tag] --种类ID
    
    local gemitem=shopConfig[tostring(self.ComposeGemID)]
    self.m_hcgempic:loadTexture(gemitem["icon"]..".png")
    self.m_hcgempinzhi:loadTexture("normal_aochao_0"..gemitem["quality"]..".png")


    local materialitem=gemcomposeConfig[tostring(self.ComposeGemID)]
    self.m_shuxing_1:setString(gemitem["quality"].."级宝石属性")
    self.m_shuxing_2:setString("属性:提高"..gemitem["hp"].."%效果")
    self.m_shuxing_3:setString("每次合成消耗金币："..materialitem["coins"])
    self.m_shuxing_4:setString("消耗："..materialitem["material_id"].."级宝石x"..materialitem["num"])
    self.m_shuxing_5:setString("材料数量：0")
    for i=1, table.getn(self.mygemlist) do
        if self.mygemlist[i]["shopId"] == materialitem["material_id"] then
            self.m_shuxing_5:setString("材料数量："..self.mygemlist[i]["num"])
            self.materialID = self.mygemlist[i]["gemId"]
    	end
    end
    
    
end

function GemUI:initGemTypeListView()
    self.m_hclistview:removeAllItems()
    for i=2, table.getn(self.gemtypeId) do
        local gemTypeItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("GemUI_Item2.csb")
        self.m_hclistview:pushBackCustomItem(gemTypeItem)
        gemTypeItem:setTag(i)
        
        local num = ccui.Helper:seekWidgetByName(gemTypeItem, "num")
        num:setVisible(false)
        
        local pinzhi = ccui.Helper:seekWidgetByName(gemTypeItem, "pinzhi")
        local pic = ccui.Helper:seekWidgetByName(gemTypeItem, "pic")
        pinzhi:loadTexture("normal_aochao_0"..self.gemtypelist[self.gemtypeId[i]]["quality"]..".png")
        pic:loadTexture(self.gemtypelist[self.gemtypeId[i]]["icon"]..".png")
        
        local function checkHCgem(sender, event)
            if sender == gemTypeItem and event == cc.EventCode.ENDED then
                self.m_mubiao:setVisible(false)
                self:updateHCPanel(sender:getTag())
        	end
        end
        gemTypeItem:addTouchEventListener(checkHCgem)
    end
end

--init your ui here
function GemUI:initUI()
    self:updateScrollView()
    self:updateGemListView()
    self:initGemTypeListView()
end

--refresh all ui here
function GemUI:refreshUI()
end

return GemUI
