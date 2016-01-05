--ShopUI.lua, created by php script. chris.li--
require "Cocos2d"
local shopConfig = require("config.define_shop")
local taskConfig = require("config.define_task")
local soulConfig = require("config.define_soul")
local PopDialog = require "common/PopDialog"
local TaskUI = class("TaskUI", function()
    return cc.Layer:create()
end)

function TaskUI:create()
    local layer = TaskUI.new()
    layer:initM()
    
    return layer
end

function TaskUI:ctor()
end

function TaskUI:onEnter()
    self:updataTipPic()
    self.guildTag = 0
    -- guard_tag --
    self:processGuard()
end

function TaskUI:onExit()
end

function TaskUI:processGuard()
    local function addGuardDialogLayer(cmd)
        cmd.type = "guard_dialog"
        cmd.cb = function() self:processGuard() end
        Helper.setTimeout(self, 1, function()
            self.guardLayer = require("common.Guard"):create(cmd)
            cc.Director:getInstance():getRunningScene():addChild(self.guardLayer)
        end)
    end

    local function addGuardClickLayer(cmd)
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

    if guardCenter:canEnterGuardMode("task") == true then
        local cmd = guardCenter:getCmd("task")
        if cmd == nil then return end
        if cmd.name == "task_click_loadbtn" then
            cmd.clickTarget = self.logBtn
            addGuardClickLayer(cmd)
        elseif cmd.name == "task_click_award" then
            cmd.clickTarget = self.myImage
            addGuardClickLayer(cmd)
        elseif cmd.name == "task_click_closetask" then
            cmd.clickTarget = self.closeBtn
            addGuardClickLayer(cmd)
        end
    end
    
    if guardCenter:canEnterGuardMode("task2") == true then
        local cmd = guardCenter:getCmd("task2")
        if cmd == nil then return end
        if cmd.name == "task2_click_levelupBtn" then
            cmd.clickTarget = self.levelupBtn
            addGuardClickLayer(cmd)
        elseif cmd.name == "task2_click_award" then
            cmd.clickTarget = self.myImage2
            addGuardClickLayer(cmd)
        elseif cmd.name == "task2_click_closetask" then
            cmd.clickTarget = self.closeBtn
            addGuardClickLayer(cmd)
        end
    end
end

function TaskUI:initM()
    ccs.ArmatureDataManager:destroyInstance()
    cc.Director:getInstance():getTextureCache():removeAllTextures()
    cc.SpriteFrameCache:getInstance():removeSpriteFrames()
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)  

    local function touchEventHandler(sender, event)
        self:handleTouchEvent(sender, event)
    end

    self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("task.csb")
    self:addChild(self.rootNode)

    --taskRootPanel: Layout
    self.taskPanel = ccui.Helper:seekWidgetByName(self.rootNode, "taskPanel")
    self.taskPanel:addTouchEventListener(touchEventHandler)
    self.taskPanel:setBackGroundImage("ditu.jpg")

    self.taskImageBack = ccui.Helper:seekWidgetByName(self.rootNode, "taskImageBack")
    self.ImageRenwu = ccui.Helper:seekWidgetByName(self.rootNode,"ImageRenwu")

    --m_ScrollView: ScrollView
    self.taskScrollView = ccui.Helper:seekWidgetByName(self.rootNode,"taskScrollView")
    self.taskScrollView:addTouchEventListener(touchEventHandler)
    self.taskScrollView2 = ccui.Helper:seekWidgetByName(self.rootNode,"taskScrollView2")
    self.taskScrollView2:addTouchEventListener(touchEventHandler) 
    self.ScrollViewDown = ccui.Helper:seekWidgetByName(self.rootNode,"ScrollViewDown")
    self.ScrollViewDown:setBounceEnabled(true)
    self.ScrollViewDown:addTouchEventListener(touchEventHandler)
    --closeBtn: Button
    self.closeBtn = ccui.Helper:seekWidgetByName(self.rootNode,"closeBtn")
    self.closeBtn:addTouchEventListener(touchEventHandler)

    self.qiandaoBtn = ccui.Helper:seekWidgetByName(self.rootNode,"qiandaoBtn")
    self.qiandaoBtn:addTouchEventListener(touchEventHandler)

    self.logBtn = ccui.Helper:seekWidgetByName(self.rootNode,"logBtn")
    self.logBtn:addTouchEventListener(touchEventHandler)

    self.levelupBtn = ccui.Helper:seekWidgetByName(self.rootNode,"levelupBtn")
    self.levelupBtn:addTouchEventListener(touchEventHandler)

    self.putongBtn = ccui.Helper:seekWidgetByName(self.rootNode,"putongBtn")
    self.putongBtn:addTouchEventListener(touchEventHandler)

    self.jingyingBtn = ccui.Helper:seekWidgetByName(self.rootNode,"jingyingBtn")
    self.jingyingBtn:addTouchEventListener(touchEventHandler)

    self.shoujiBtn = ccui.Helper:seekWidgetByName(self.rootNode,"shoujiBtn")
    self.shoujiBtn:addTouchEventListener(touchEventHandler)
    
    self.m_huadong = ccui.Helper:seekWidgetByName(self.rootNode, "Sliderhd")
    self.m_huadong:setVisible(false)
    
    self.fengexian = ccui.Helper:seekWidgetByName(self.rootNode, "Image_8")
    
     for i = 1,6 do
        self.hongdian = ccui.Helper:seekWidgetByName(self.rootNode,"hongdian"..i)
        self.hongdian:setName("hongdian_"..i)
    end
    

    self.topLabel = ccui.Helper:seekWidgetByName(self.rootNode,"topLabel")
    self.ImageTips = ccui.Helper:seekWidgetByName(self.rootNode,"ImageTips")
    self.ImageTips:setVisible(false)
    self.labelTips = ccui.Helper:seekWidgetByName(self.rootNode,"labelTips")
    self.labelTips2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelTips2")
    local ImageTipX=self.ImageTips:getPositionX()
    local ImageTipY=self.ImageTips:getPositionY()
    local  labBg = cc.Scale9Sprite:create("taskui_duihuahua.png")
    labBg:setAnchorPoint(cc.p(0.5,0.5))
    labBg:setPreferredSize(cc.size(262,92))
    labBg:setPosition(cc.p(ImageTipX,ImageTipY+2))
    labBg:setTag(10)
    self.ImageRenwu:addChild(labBg)
    self.labelTips:setVisible(true)
    self.labelTips2:setVisible(true)
    self.labelTips:setString("每天签到就可以领取奖励了,")
    self.labelTips2:setString("明天不要忘记哦")
    local tapLab=self.labelTips:getTag()
    local labbg=labBg:getTag()

    self.qiandaoBtn:loadTextures("qiandao01.png","qiandao.png","qiandao.png")    
    self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
    self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
    self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")
    self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
    self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png")

    --self:leveLLayer()

    self:signLayer()
end

function TaskUI:handleTouchEvent(sender, event)

    if sender == nil then
        cclog("[ERR]unknown sender")

    elseif sender == self.taskPanel and event == cc.EventCode.ENDED then
        cclog("[TaskUI]--taskPanel touched--")

    elseif sender == self.closeBtn and event == cc.EventCode.ENDED then
        cclog("[TaskUI]--closeBtn touched--")
        local scene = require("scene.home.HomeScene"):create()
        cc.Director:getInstance():replaceScene(scene)
        if guardCenter:canEnterGuardMode("task") == true or guardCenter:canEnterGuardMode("task2") == true then
            guardCenter:jumpNextGuardStage()
        end
        
    elseif sender == self.qiandaoBtn and event == cc.EventCode.ENDED then
        cclog("[TaskUI]--qiandaoBtn touched--") 
        self.qiandaoBtn:loadTextures("qiandao01.png","qiandao.png","qiandao.png") 
        self.qiandaoBtn:setEnabled(false)   
        self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
        self.logBtn:setEnabled(true)
        self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
        self.levelupBtn:setEnabled(true)
        self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")
        self.putongBtn:setEnabled(true)
        self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
        self.jingyingBtn:setEnabled(true)
        self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png")
        self.shoujiBtn:setEnabled(true) 
        self:signLayer() 
        --self:updataTipPic() 

    elseif sender == self.logBtn and event == cc.EventCode.ENDED then
        cclog("[taskUI]--logBtn touched--")
            self.logBtn:loadTextures("denglu01.png","denglu.png","denglu.png")
            self.logBtn:setEnabled(false)
            self.qiandaoBtn:loadTextures("qiandao.png","qiandao.png","qiandao.png")
            self.qiandaoBtn:setEnabled(true)
            self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
            self.levelupBtn:setEnabled(true)
            self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")
            self.putongBtn:setEnabled(true)
            self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
            self.jingyingBtn:setEnabled(true)
            self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png")
            self.shoujiBtn:setEnabled(true)
            if guardCenter:canEnterGuardMode("task") == true and self.guildTag == 0 then
                self.guilgTag = 1
                self:logLayer()
                self.logBtn:setEnabled(false)
            elseif guardCenter:canEnterGuardMode("task") == true and self.guildTag == 1 then
            else
                self:logLayer()   
            end 

    elseif sender == self.levelupBtn and event == cc.EventCode.ENDED then
        cclog("[TaskUI]--levelupBtn touched--")
        
        
        --if guardCenter:canEnterGuardMode("task2") == true then
            --self.levelupBtn:setEnabled(false)
            self.levelupBtn:loadTextures("shengji01.png","shengji.png","shengji.png")
            self.levelupBtn:setEnabled(false)
            self.qiandaoBtn:loadTextures("qiandao.png","qiandao.png","qiandao.png")
            self.qiandaoBtn:setEnabled(true)
            self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
            self.logBtn:setEnabled(true)
            self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")
            self.putongBtn:setEnabled(true) 
            self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
            self.jingyingBtn:setEnabled(true)
            self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png")
            self.shoujiBtn:setEnabled(true)
            if guardCenter:canEnterGuardMode("task2") == true and self.guildTag == 0 then
                self.guildTag = 1
                 self:leveLLayer()
            elseif guardCenter:canEnterGuardMode("task2") == true and self.guildTag == 1 then     
            else
                 self:leveLLayer()     
            end     

        
        --self:updataTipPic()
        
    elseif sender == self.putongBtn and event == cc.EventCode.ENDED then
        cclog("[taskUI]--putongBtn touched--")
        self.putongBtn:loadTextures("putongben01.png","putongben.png","putongben.png")
        self.putongBtn:setEnabled(false)
        self.qiandaoBtn:loadTextures("qiandao.png","qiandao.png","qiandao.png")
        self.qiandaoBtn:setEnabled(true)
        self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
        self.logBtn:setEnabled(true)
        self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
        self.levelupBtn:setEnabled(true)
        self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
        self.jingyingBtn:setEnabled(true)
        self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png")
        self.shoujiBtn:setEnabled(true)
        self:commonLayer()
        --self:updataTipPic()
        
    elseif sender == self.jingyingBtn and event == cc.EventCode.ENDED then
        cclog("[tasUI]--m_add10Btn touched--")
        self.jingyingBtn:loadTextures("jingyingben01.png","jingyingben.png","jingyingben.png") 
        self.jingyingBtn:setEnabled(false)
        self.qiandaoBtn:loadTextures("qiandao.png","qiandao.png","qiandao.png")
        self.qiandaoBtn:setEnabled(true)
        self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
        self.logBtn:setEnabled(true)
        self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
        self.levelupBtn:setEnabled(true)
        self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")
        self.putongBtn:setEnabled(true)      
        self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png")
        self.shoujiBtn:setEnabled(true)
        self:expertLayer()
        --self:updataTipPic()
        
    elseif sender == self.shoujiBtn and event == cc.EventCode.ENDED then
        cclog("[tassUI]--shoujiBtn touched--")
        self.shoujiBtn:loadTextures("shouji01.png","shouji.png","shouji.png")
        self.shoujiBtn:setEnabled(false) 
        self.qiandaoBtn:loadTextures("qiandao.png","qiandao.png","qiandao.png")
        self.qiandaoBtn:setEnabled(true)
        self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
        self.logBtn:setEnabled(true)
        self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
        self.levelupBtn:setEnabled(true)
        self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")
        self.putongBtn:setEnabled(true)
        self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
        self.jingyingBtn:setEnabled(true)
        self:conlectLayer()
        --self:updataTipPic()
        
    elseif sender == self.taskScrollView and event == cc.EventCode.ENDED then
        cclog("[tasUI]--taskScrollView touched--")
        --SetSlider(self.taskScrollView,self.m_huadong)

    elseif sender == self.taskScrollView2 and event == cc.EventCode.ENDED then
        cclog("[tasUI]--taskScrollView2 touched--")
        SetSlider(self.taskScrollView2,self.m_huadong)

    elseif sender == self.ScrollViewDown and event == cc.EventCode.ENDED then
        cclog("[tasUI]--ScrollViewDown touched--")
    end

end

function TaskUI:signLayer()
    self.topLabel:setVisible(true)
    Http:req("taskStart",
        {type=1},
        function(res) 
            if res.err then cclog("err!!!") return end
            local itemList = res.data["taskList"]
            local tasksize = table.getn(itemList)
            self.labelTips:setVisible(true)
            self.labelTips2:setVisible(true)
            self.labelTips:setString("每天签到就可以领取奖励了,")
            self.labelTips2:setString("明天不要忘记哦")
            self.taskScrollView2:setVisible(true)
            self.taskScrollView:setVisible(false)
            self.fengexian:setVisible(false)
            self.taskScrollView2:removeAllChildren()
            self.taskScrollView2:setBounceEnabled(true)

            local ScrovSize = self.taskScrollView2:getInnerContainerSize()
            local ScrovH = ScrovSize.height
            self.taskScrollView2:setScale(1.0)
            if tasksize>6 then
                if tasksize%5 == 0 then
                    --ScrovH = (130+10)*(tasksize/5)
                    ScrovH = (135)*(tasksize/5)
                else
                    --ScrovH = (130+10)*(math.floor(tasksize/5)+1)
                    ScrovH = (135)*(math.floor(tasksize/5)+1)                  
                end
            end
            ScrovSize.height = ScrovH
            self.taskScrollView2:setInnerContainerSize(ScrovSize)
            
            local textBgNum = 0
            if tasksize%5 == 0 then
                textBgNum = tasksize/5
            else
                textBgNum = tasksize/5 + 1    
            end
            
            
            for j = 1,textBgNum do
                local TextBg = ccui.ImageView:create()
                TextBg:setAnchorPoint(0,0)
                TextBg:loadTexture("image/ui/task/tiao03.png")
                TextBg:setScale9Enabled(true)
                TextBg:setContentSize(ScrovSize.width,125)
                TextBg:setPosition(0,ScrovSize.height-129*(j)-7*(j-1))
                --TextBg:setPosition(cc.p(ScrovSize.width/2,ScrovSize.height - 73 - 145*(j-1)))
                self.taskScrollView2:addChild(TextBg)
            end   
                     
            self.tag=0
            for i=1,tasksize do
                local rolerTaskId=tostring(res.data["taskList"][i]["rolerTaskId"])
                local taskId=tostring(res.data["taskList"][i]["taskId"])
                local status=res.data["taskList"][i]["status"]
                local taskItemlist= taskConfig[taskId]
                
                --获取物品Id
                local itmId=tostring(taskItemlist["item"])
                local res = string.split(itmId,":") 
                local goodsId = res[1]
                local nums = res[2]
                
                local goldTag = tostring(taskItemlist["gold"]) --11
                local coinTag = tostring(taskItemlist["coins"]) --12
                local expTag = tostring(taskItemlist["exp"]) --13
                    
                local dataitem=shopConfig[goodsId]             
                local task_desc=tostring(taskItemlist["task_desc"])  
                local taskItemImage =ccs.GUIReader:getInstance():widgetFromBinaryFile("taskItemImage.csb")
                taskItemImage:setScale(0.8) 
                taskItemImage:setAnchorPoint(0.5,0.5)                       
                local PanelitemImage = ccui.Helper:seekWidgetByName(taskItemImage, "PanelitemImage")
                local ImageItem=ccui.Helper:seekWidgetByName(taskItemImage,"ImageItem")
                local Imagekuang=ccui.Helper:seekWidgetByName(taskItemImage,"Imagekuang")
                
                if dataitem ~= nil then
                    ImageItem:loadTexture(dataitem["icon"]..".png")
                    Imagekuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                else
                    if tonumber(goldTag) ~= 0 then
                        local golditem = shopConfig["11"]
                        ImageItem:loadTexture(golditem["icon"]..".png")
                    elseif tonumber(coinTag) ~= 0 then
                        local coinitem = shopConfig["12"]
                        ImageItem:loadTexture(coinitem["icon"]..".png")
                    elseif tonumber(expTag) ~= 0 then
                        local expitem = shopConfig["13"]
                        ImageItem:loadTexture(expitem["icon"]..".png")     
                    end    
                end
                
                --type 16~ suipian 
                local ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                ImageSuipian:setVisible(false)
                if dataitem ~= nil then
                    local type=(dataitem["type"])
                    if tonumber(type)==16 then
                        -- self.ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                        ImageSuipian:setVisible(true) 
                    else
                        ImageSuipian:setVisible(false) 
                    end
                end
                

                local numLabel=ccui.Helper:seekWidgetByName(taskItemImage,"num")
                numLabel:setString(nums)
                local labelPlus=ccui.Helper:seekWidgetByName(taskItemImage,"labelPlus")
                taskItemImage:setPosition(70+(i-1)%5*(100+4),ScrovSize.height+67-136*(math.floor((i-1)/5)+1))
                labelPlus:setPosition(4+(i-1)%5*(100+4),ScrovSize.height-130*(math.floor((i-1)/5)+1)-22)
                labelPlus:setString(task_desc) 
                --labelPlus:setScale(1.1)
                local receivedImage=ccui.Helper:seekWidgetByName(taskItemImage,"receivedImage")
                receivedImage:setVisible(false)

                if status==0 then--0未完成  
                    self.taskScrollView2:addChild(taskItemImage)
                elseif status==1 then--1可领取
                
                    local armatureFile = "image/ui/task/heffect/heffect.csb"
                    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
                    local skillarmature = ccs.Armature:create("heffect")
                    skillarmature:setScale(1.1)
                    skillarmature:setPosition(44,45)
                    ImageItem:addChild(skillarmature)
                    --速度
                    skillarmature:getAnimation():setSpeedScale(1)
                    skillarmature:getAnimation():playWithIndex(0,0,1)
                
                    self.taskScrollView2:addChild(taskItemImage)
                elseif status==2 then--2已领奖
                    self.tag=self.tag+1
                    self.taskScrollView2:addChild(taskItemImage)
                    receivedImage:setVisible(true)
                end

                --self.taskScrollView:addChild(taskItemImage)

                local function TaskMenuCallBack(sender,event)
                    SetSlider(self.taskScrollView2,self.m_huadong)
                    if sender == taskItemImage and event == cc.EventCode.ENDED then
                    
                        if status==0 then--0未完成  
                            PopDialog:getInstance():popText("未达到要求,不能领取",cc.c3b(0,255,0))
                        elseif status==1 then--1可领取
                            self.Isplay = 1

                            Http:req("taskCompleted",{rolerTaskId = rolerTaskId},function(res)
                                if res.data["result"] == 0 then
                                    PopDialog:getInstance():popText("领取成功！",cc.c3b(0,255,0))
                                    self:signLayer()
                                    self:updataTipPic()
                                    sender:setEnabled(false)
                                    receivedImage:setVisible(true)
                                end
                            end)

                        elseif status==2 then--2已领奖
                            PopDialog:getInstance():popText("已领取，不能重复领取",cc.c3b(0,255,0))
                        end

                    end
                end

                taskItemImage:addTouchEventListener(TaskMenuCallBack)
                taskItemImage:setTag(taskId)

            end  

            local day=tostring(self.tag)
            local topLayer= ccui.Helper:seekWidgetByName(self.rootNode,"topLabel")
            -- topLayer:setPosition(cc.p(topLayer:getPositionX(),topLayer:getPositionY()-7))
            topLayer:setString("天天签到就可以领取超赞宝物 ,您已经签到"..day.."天")

        end)

end


function TaskUI:logLayer()
    -- self.rootNode:removeFromParent()
    
    cclog("登陆奖励")
    Http:req("taskStart",
        {type=2},
        function(res) 
            if res.err then cclog("err!!!") return end
            local itemList = res.data["taskList"]
            local tasksize = table.getn(itemList)
            self.taskScrollView2:setVisible(true)
            self.taskScrollView:setVisible(false)
            self.fengexian:setVisible(false)          
            self.taskScrollView2:removeAllChildren()
            self.taskScrollView2:setBounceEnabled(true)

            self.labelTips:setString("每天登陆就可以领取奖励,")
            self.labelTips2:setString("明天不要忘记哦")
            self.labelTips:setVisible(true)
            self.labelTips2:setVisible(true)

            local ScrovSize = self.taskScrollView2:getInnerContainerSize()
            local ScrovH = ScrovSize.height
            self.taskScrollView:setScale(1.0)
            --if tasksize>10 then
                if tasksize%5 == 0 then
                    --ScrovH = (130+10)*(tasksize/5)
                    ScrovH = (135)*(tasksize/5)
                else
                    --ScrovH = (130+10)*(math.floor(tasksize/5)+1)
                    ScrovH = (135)*(math.floor(tasksize/5)+1)                  
                end

            --end
            ScrovSize.height = ScrovH
            self.taskScrollView2:setInnerContainerSize(ScrovSize)
            
            local textBgNum = 0
            if tasksize%5 == 0 then
                textBgNum = tasksize/5
            else
                textBgNum = tasksize/5 + 1    
            end

           --[[ for j = 1,textBgNum do
                local TextBg = ccui.ImageView:create()
                TextBg:setAnchorPoint(0,0)
                TextBg:loadTexture("image/ui/task/tiao03.png")
                TextBg:setScale9Enabled(true)
                TextBg:setContentSize(ScrovSize.width-20,125)
                --TextBg:setScaleY(-1)
                TextBg:setPosition(22,ScrovSize.height-137*(j-1))
                --TextBg:setPosition(cc.p(ScrovSize.width/2,ScrovSize.height - 73 - 145*(j-1)))
                self.taskScrollView2:addChild(TextBg)
            end --]]  
            
            for j = 1,textBgNum do
                local TextBg = ccui.ImageView:create()
                TextBg:setAnchorPoint(0,0)
                TextBg:loadTexture("image/ui/task/tiao03.png")
                TextBg:setScale9Enabled(true)
                TextBg:setContentSize(ScrovSize.width-12,125)
                TextBg:setPosition(22,ScrovSize.height-129*(j)-7*(j-1))
                --TextBg:setPosition(cc.p(ScrovSize.width/2,ScrovSize.height - 73 - 145*(j-1)))
                self.taskScrollView2:addChild(TextBg)
            end   
            
            
            self.dg=0
            for i=1,tasksize do
                local rolerTaskId=tostring(res.data["taskList"][i]["rolerTaskId"])
                local taskId=tostring(res.data["taskList"][i]["taskId"])
                local status=res.data["taskList"][i]["status"]
                local taskItemlist= taskConfig[tostring(taskId)]
                
                local itmId=tostring(taskItemlist["item"])  
                local ishero = tostring(taskItemlist["soul_id"])
                local res = string.split(itmId, ":")
                
                local goodsId = res[1] 
                local nums = res[2]
                
                local goldTag = tostring(taskItemlist["gold"]) --11
                local coinTag = tostring(taskItemlist["coins"]) --12
                local expTag = tostring(taskItemlist["exp"]) --13
                
                local dataitem=shopConfig[tostring(goodsId)]
                local soulNum = soulConfig[tostring(ishero)]
                         
                local task_desc=tostring(taskItemlist["task_desc"])             
                local taskItemImage =ccs.GUIReader:getInstance():widgetFromBinaryFile("taskItemImage.csb")
                taskItemImage:setScale(0.8)
                taskItemImage:setAnchorPoint(0.5,0.5)
                if i == 1 then self.myImage = taskItemImage end
                                        
                local PanelitemImage = ccui.Helper:seekWidgetByName(taskItemImage, "PanelitemImage")
                local ImageItem=ccui.Helper:seekWidgetByName(taskItemImage,"ImageItem")
                local Imagekuang=ccui.Helper:seekWidgetByName(taskItemImage,"Imagekuang")
                local ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                ImageSuipian:setVisible(false)
                --ImageItem:loadTexture(taskItemlist["icon"]..".png")
                if dataitem ~= nil then
                    ImageItem:loadTexture(dataitem["icon"]..".png")
                    Imagekuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                elseif tonumber(ishero) ~= 0 then
                    ImageItem:loadTexture(soulNum["icon"]..".png")
                    Imagekuang:loadTexture("normal_aochao_0"..soulNum["quality"]..".png")
                else
                    if tonumber(goldTag) ~= 0 then
                        local golditem = shopConfig["11"]
                        ImageItem:loadTexture(golditem["icon"]..".png")
                    elseif tonumber(coinTag) ~= 0 then
                        local coinitem = shopConfig["12"]
                        ImageItem:loadTexture(coinitem["icon"]..".png")
                    elseif tonumber(expTag) ~= 0 then
                        local expitem = shopConfig["13"]
                        ImageItem:loadTexture(expitem["icon"]..".png")     
                    end     
                end
                
                --type 16~ suipian 

               --[[ if dataitem ~= nil then
                ImageItem:loadTexture(dataitem["icon"]..".png")
                end--]]

                local ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                ImageSuipian:setVisible(false)
                
                if dataitem ~= nil then
                    local type=(dataitem["type"])
                    if tonumber(type)==16 then
                        --self.ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                        ImageSuipian:setVisible(true) 
                    else
                        ImageSuipian:setVisible(false) 
                    end 
                end
                

                local numLabel=ccui.Helper:seekWidgetByName(taskItemImage,"num")
                numLabel:setString(nums)
                local labelPlus=ccui.Helper:seekWidgetByName(taskItemImage,"labelPlus")
                --taskItemImage:setPosition(2+(i-1)%5*(100+4),ScrovSize.height-136*(math.floor((i-1)/5)+1))
                taskItemImage:setPosition(70+(i-1)%5*(100+4),ScrovSize.height+67-136*(math.floor((i-1)/5)+1))
                labelPlus:setPosition(2+(i-1)%5*(100+4),ScrovSize.height-130*(math.floor((i-1)/5)+1)-20)
                labelPlus:setString(task_desc)               
                local receivedImage=ccui.Helper:seekWidgetByName(taskItemImage,"receivedImage")
                receivedImage:setVisible(false)

                if status==0 then--0未完成  
                    self.taskScrollView2:addChild(taskItemImage)
                elseif status==1 then--1可领取
                    self.dg=self.dg+1

                    local armatureFile = "image/ui/task/heffect/heffect.csb"
                    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
                    local skillarmature = ccs.Armature:create("heffect")
                    skillarmature:setScale(1.1)
                    skillarmature:setPosition(44,45)
                    ImageItem:addChild(skillarmature)
                    --速度
                    skillarmature:getAnimation():setSpeedScale(1)
                    skillarmature:getAnimation():playWithIndex(0,0,1)

                    self.taskScrollView2:addChild(taskItemImage)
                elseif status==2 then--2已领奖
                    self.dg=self.dg+1
                    self.taskScrollView2:addChild(taskItemImage)
                    receivedImage:setVisible(true)
                end
                
                local function TaskMenuCallBack(sender,event)
                    if sender == taskItemImage and event == cc.EventCode.ENDED then
                        if status==0 then--0未完成  
                            PopDialog:getInstance():popText("当前奖励不能领取",cc.c3b(0,255,0))
                        elseif status==1 then--1可领取                                          
                            Http:req("taskCompleted",{rolerTaskId = rolerTaskId},function(res)
                                if res.data["result"] == 0 then
                                    sender:setEnabled(false)
                                    self:logLayer() 
                                    self:updataTipPic()       
                                    PopDialog:getInstance():popText("领取成功",cc.c3b(0,255,0))                         
                                    receivedImage:setVisible(true)
                                    
                                    if guardCenter:canEnterGuardMode("task") == true then
                                        guardCenter:isJump2Stage()
                                    end
                                end
                            end)

                        elseif status==2 then--2已领奖
                            PopDialog:getInstance():popText("已领取，不能重复领取",cc.c3b(0,255,0))
                        end

                    end
                end
                taskItemImage:addTouchEventListener(TaskMenuCallBack)
                taskItemImage:setTag(tonumber(taskId))
            end  
            
            -- guard_tag --
            self:processGuard()
            
            local dy=tostring(self.dg)
            local topLayer= ccui.Helper:seekWidgetByName(self.rootNode,"topLabel")
            topLayer:setVisible(true)
            topLayer:setString("")
            -- topLayer:setPosition(cc.p(topLayer:getPositionX(),topLayer:getPositionY()-4))
            topLayer:setString("天天登陆就可以领取超赞宝物 ,您已经登陆"..dy.."天")
        end)

end

function TaskUI:leveLLayer()
    --self.levelupBtn:setEnabled(false)
    self.topLabel:setVisible(false)
    cclog("level")
    Http:req("taskStart",
        {type=3},
        function(res) 
            if res.err then cclog("err!!!") return end
            local itemList = res.data["taskList"]
            local tasksize = table.getn(itemList)
            cclog("taskSize~~"..tasksize)
            self.taskScrollView:setVisible(false)
            self.fengexian:setVisible(false)
            self.taskScrollView2:setVisible(true)
            self.taskScrollView:removeAllChildren()
            self.taskScrollView2:removeAllChildren()          
            self.taskScrollView:setBounceEnabled(false)
            self.taskScrollView2:setBounceEnabled(true)
            self.labelTips:setString("到达指定等级就可以")
            self.labelTips2:setString("领取奖励了哦!快点升级吧!")
            self.labelTips:setVisible(true)
            self.labelTips2:setVisible(true)
            local ScrovSize = self.taskScrollView2:getInnerContainerSize()
            local ScrovH = ScrovSize.height
            
            if tasksize>6 then
                if tasksize%5 == 0 then
                    --ScrovH = (130+10)*(tasksize/5)
                    ScrovH = (135)*(tasksize/5)
                else
                    --ScrovH = (130+10)*(math.floor(tasksize/5)+1)
                    ScrovH = (135)*(math.floor(tasksize/5)+1)                  
                end

            end
            
            ScrovSize.height = ScrovH
            self.taskScrollView2:setInnerContainerSize(ScrovSize)
            --self.taskScrollView2:setScale(1.0)
            
            local textBgNum = 0
            if tasksize%5 == 0 then
                textBgNum = tasksize/5
            else
                textBgNum = tasksize/5 + 1    
            end

            --[[for j = 1,textBgNum do
                local TextBg = ccui.ImageView:create()
                TextBg:setAnchorPoint(0,0)
                TextBg:loadTexture("image/ui/task/tiao03.png")
                TextBg:setScale9Enabled(true)
                TextBg:setContentSize(ScrovSize.width-20,125)
               -- TextBg:setScaleY(-1)
                TextBg:setPosition(22,ScrovSize.height-137*(j-1))
                --TextBg:setPosition(cc.p(ScrovSize.width/2,ScrovSize.height - 73 - 145*(j-1)))
                self.taskScrollView2:addChild(TextBg)
            end  --]]
            
            for j = 1,textBgNum do
                local TextBg = ccui.ImageView:create()
                TextBg:setAnchorPoint(0,0)
                TextBg:loadTexture("image/ui/task/tiao03.png")
                TextBg:setScale9Enabled(true)
                TextBg:setContentSize(ScrovSize.width-12,125)
                TextBg:setPosition(22,ScrovSize.height-129*(j)-7*(j-1))
                --TextBg:setPosition(cc.p(ScrovSize.width/2,ScrovSize.height - 73 - 145*(j-1)))
                self.taskScrollView2:addChild(TextBg)
            end    
            
            for i=1,tasksize do
                local taskId=tostring(res.data["taskList"][i]["taskId"])        
                local taskItemlist= taskConfig[taskId]
                local task_desc=tostring(taskItemlist["task_desc"])
                --local gold=tostring(taskItemlist["gold"])
                
                local itmId=tostring(taskItemlist["item"]) 
                local ishero = tostring(taskItemlist["soul_id"]) 
                local res_1 = string.split(itmId, ":")
                
                local goodsId = res_1[1] 
                local nums = res_1[2]
                
                local goldTag = tostring(taskItemlist["gold"]) --11
                local coinTag = tostring(taskItemlist["coins"]) --12
                local expTag = tostring(taskItemlist["exp"]) --13
                
                local dataitem=shopConfig[tostring(goodsId)]
                local soulNum = soulConfig[tostring(ishero)]
                
                local status = res.data.taskList[i].status                
                local rolerTaskId=tostring(res.data["taskList"][i]["rolerTaskId"])
                local taskItemImage=ccs.GUIReader:getInstance():widgetFromBinaryFile("taskItemImage.csb")   
                taskItemImage:setScale(0.8)
                taskItemImage:setAnchorPoint(0.5,0.5)
                if i == 1 then self.myImage2 = taskItemImage end 

               local PanelitemImage = ccui.Helper:seekWidgetByName(taskItemImage, "PanelitemImage")
                local ImageItem=ccui.Helper:seekWidgetByName(taskItemImage,"ImageItem")
                local Imagekuang=ccui.Helper:seekWidgetByName(taskItemImage,"Imagekuang")
                
                if dataitem ~= nil then
                    ImageItem:loadTexture(dataitem["icon"]..".png")
                    Imagekuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                elseif tonumber(ishero) ~= 0 then
                    ImageItem:loadTexture(soulNum["icon"]..".png")
                    Imagekuang:loadTexture("normal_aochao_0"..soulNum["quality"]..".png")
                else
                    if tonumber(goldTag) ~= 0 then
                        local golditem = shopConfig["11"]
                        ImageItem:loadTexture(golditem["icon"]..".png")
                    elseif tonumber(coinTag) ~= 0 then
                        local coinitem = shopConfig["12"]
                        ImageItem:loadTexture(coinitem["icon"]..".png")
                    elseif tonumber(expTag) ~= 0 then
                        local expitem = shopConfig["13"]
                        ImageItem:loadTexture(expitem["icon"]..".png")     
                    end     
                end
                
                --ImageItem:loadTexture(dataitem["icon"]..".png")
               -- Imagekuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                --type 16~ suipian 
                local ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                ImageSuipian:setVisible(false)
                
                --if dataitem ~= nil then
               -- end
               if dataitem ~= nil then
                    local type=(dataitem["type"])
                    if tonumber(type)==16 then
                        -- self.ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                        ImageSuipian:setVisible(true) 
                    else
                        ImageSuipian:setVisible(false) 
                    end
               end
                

                local numLabel=ccui.Helper:seekWidgetByName(taskItemImage,"num")
                numLabel:setString(nums)
                local labelPlus=ccui.Helper:seekWidgetByName(taskItemImage,"labelPlus")
                --taskItemImage:setPosition(4+(i-1)%5*(100+4),ScrovSize.height-136*(math.floor((i-1)/5)+1))
                taskItemImage:setPosition(70+(i-1)%5*(100+4),ScrovSize.height+67-136*(math.floor((i-1)/5)+1))
                labelPlus:setPosition(4+(i-1)%5*(100+4),ScrovSize.height-130*(math.floor((i-1)/5)+1)-22)
                labelPlus:setString(task_desc) 
                local receivedImage=ccui.Helper:seekWidgetByName(taskItemImage,"receivedImage")
                receivedImage:setVisible(false)

                if status==0 then--0未完成  
                    self.taskScrollView2:addChild(taskItemImage)
                elseif status==1 then--1可领取
                
                    local armatureFile = "image/ui/task/heffect/heffect.csb"
                    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
                    local skillarmature = ccs.Armature:create("heffect")
                    skillarmature:setPosition(44,45)
                    skillarmature:setScale(1.1)
                    ImageItem:addChild(skillarmature)
                    --速度
                    skillarmature:getAnimation():setSpeedScale(1)
                    skillarmature:getAnimation():playWithIndex(0,0,1)
                
                    self.taskScrollView2:addChild(taskItemImage)
                elseif status==2 then--2已领奖
                    self.tag=self.tag+1
                    self.taskScrollView2:addChild(taskItemImage)
                    receivedImage:setVisible(true)
                end

                --self.taskScrollView:addChild(taskItemImage)

                local function TaskMenuCallBack(sender,event)
                    SetSlider(self.taskScrollView2,self.m_huadong)
                    if sender == taskItemImage and event == cc.EventCode.ENDED then
                        if status==0 then--0未完成  
                            PopDialog:getInstance():popText("未达到要求,不能领取",cc.c3b(0,255,0))
                        elseif status==1 then--1可领取
                        
                            self.Isplay = 1

                            Http:req("taskCompleted",{rolerTaskId = rolerTaskId},function(res)
                                if res.data["result"] == 0 then
                                    PopDialog:getInstance():popText("领取成功！",cc.c3b(0,255,0))
                                    self:leveLLayer()
                                    self:updataTipPic()
                                    sender:setEnabled(false)
                                    receivedImage:setVisible(true)
                                    
                                    if guardCenter:canEnterGuardMode("task2") == true then
                                        guardCenter:isJump2Stage()
                                    end
                                end
                            end)

                        elseif status==2 then--2已领奖

                            PopDialog:getInstance():popText("已领取，不能重复领取",cc.c3b(0,255,0))
                        end

                    end
                end

                taskItemImage:addTouchEventListener(TaskMenuCallBack)
                taskItemImage:setTag(taskId)

            end 
            
            -- guard_tag --
            self:processGuard()
             
        end)
end
function TaskUI:commonLayer()
    cclog("common")
    self.topLabel:setVisible(false)
    Http:req("taskStart",
        {type=4},
        function(res) 
            if res.err then cclog("err!!!") return end
            local itemList = res.data["taskList"]
            local tasksize = table.getn(itemList)
            cclog("taskSize"..tasksize)
            
            self.labelTips:setString("通关指定普通副本")
            self.labelTips2:setString("即可领取丰厚奖励!")
            self.taskScrollView:setVisible(true)
            self.fengexian:setVisible(true)
            self.taskScrollView2:setVisible(false)
            self.taskScrollView:removeAllChildren()
            self.taskScrollView:setBounceEnabled(true)
            local ScrovSize = self.taskScrollView:getInnerContainerSize()
            local ScrovH = ScrovSize.height

            if tasksize>6 then
                if tasksize%2 == 0 then
                    ScrovH = (90+5)*(tasksize/2)
                else
                    ScrovH = (90+5)*(math.floor(tasksize/2)+1)                  
                end

            end
            ScrovSize.height = ScrovH
            self.taskScrollView:setInnerContainerSize(ScrovSize)

            for i=1,tasksize do

                local taskId=tostring(res.data["taskList"][i]["taskId"])
                local taskItemlist=  taskConfig[taskId]
                local task_desc=tostring(taskItemlist["task_desc"])
                local gold=tostring(taskItemlist["gold"])
                local status=res.data["taskList"][i]["status"]
                local rolerTaskId=tostring(res.data["taskList"][i]["rolerTaskId"])
                local taskItem=ccs.GUIReader:getInstance():widgetFromBinaryFile("taskItem.csb")                        
                local PanelTaskItem = ccui.Helper:seekWidgetByName(taskItem, "PanelTaskItem")
                local taskImageItem=ccui.Helper:seekWidgetByName(taskItem,"taskImageItem")
                local LabelDes=ccui.Helper:seekWidgetByName(taskItem,"LabelDes")
                LabelDes:setString(task_desc) 
                local Label2=ccui.Helper:seekWidgetByName(taskItem,"Label2")
                Label2:setString("可领取")
                local Label3=ccui.Helper:seekWidgetByName(taskItem,"Label3")            


                Label3:setString(" x".." "..gold)
                local ImageBao=ccui.Helper:seekWidgetByName(taskItem,"ImageBao")
                local received = ccui.Helper:seekWidgetByName(taskItem,"received")
                local yiwancheng = ccui.Helper:seekWidgetByName(taskItem,"yiwancheng")
                received:setVisible(false)
                yiwancheng:setVisible(false)
                taskItem:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1))  
                --self.received:setPosition(2+(i-1)%2*266,ScrovSize.height-94(math.floor((i-1)/2)+1))  
                if status==0 then--0未完成  

                elseif status==1 then--1可领取 
                    yiwancheng:setVisible(true)       
                    --self.taskScrollView:addChild(taskItem)
                elseif status==2 then--2已领奖

                    --self.taskScrollView:addChild(taskItem)
                    received:setVisible(true)
                end

                self.taskScrollView:addChild(taskItem)



                local status=res.data["taskList"][i]["status"]
                local function TaskMenuCallBack(sender,event)
                    if sender == taskItem and event == cc.EventCode.ENDED then
                        
                        cclog("事件监听~~~~"..sender:getTag())
                        cclog("hello~~~~")
                        --self.taskImageItem:setVisible(false)
                        if status==0 then--0未完成  
                            PopDialog:getInstance():popText("副本暂未挑战成功",cc.c3b(0,255,0))

                        elseif status==1 then--1可领取

                            Http:req("taskCompleted",{rolerTaskId = rolerTaskId},function(res)
                                if res.data["result"] == 0 then
                                    cclog("领取成功")
                                    self:updataTipPic()
                                    PopDialog:getInstance():popText("领取成功",cc.c3b(0,255,0))
                                    -- self.tag=self.tag+1
                                    sender:setEnabled(false)
                                    received:setVisible(true)
                                end
                            end)



                        elseif status==2 then--2已领奖

                            PopDialog:getInstance():popText("已领取，不能重复领取",cc.c3b(0,255,0))
                        end

                    end
                end

                taskItem:addTouchEventListener(TaskMenuCallBack)
                taskItem:setTag(tonumber(taskId))
            end  

        end)


end


function TaskUI:expertLayer()
    cclog("expert")
    self.topLabel:setVisible(false)

    Http:req("taskStart",
        {type=5},
        function(res) 
            if res.err then cclog("err!!!") return end
            local itemList = res.data["taskList"]
            local tasksize = table.getn(itemList)
            cclog("taskSize"..tasksize)
            
            self.labelTips:setString("通关指定精英副本")
            self.labelTips2:setString("即可领取丰厚奖励!")
            self.taskScrollView:setVisible(true)
            self.fengexian:setVisible(true)
            self.taskScrollView2:setVisible(false)
            self.taskScrollView:removeAllChildren()
            self.taskScrollView:setBounceEnabled(true)
            local ScrovSize = self.taskScrollView:getInnerContainerSize()
            local ScrovH = ScrovSize.height

            if tasksize>6 then
                if tasksize%2 == 0 then
                    ScrovH = (90+5)*(tasksize/2)
                else
                    ScrovH = (90+5)*(math.floor(tasksize/2)+1)                  
                end

            end
            ScrovSize.height = ScrovH
            self.taskScrollView:setInnerContainerSize(ScrovSize)

            for i=1,tasksize do

                local taskId=tostring(res.data["taskList"][i]["taskId"])
                local taskItemlist=  taskConfig[taskId]
                local task_desc=tostring(taskItemlist["task_desc"])
                local gold=tostring(taskItemlist["gold"])
                local status=res.data["taskList"][i]["status"]                
                local rolerTaskId=tostring(res.data["taskList"][i]["rolerTaskId"])
                local taskItem=ccs.GUIReader:getInstance():widgetFromBinaryFile("taskItem.csb")                      
                local PanelTaskItem = ccui.Helper:seekWidgetByName(taskItem, "PanelTaskItem")
                local taskImageItem=ccui.Helper:seekWidgetByName(taskItem,"taskImageItem")
                local LabelDes=ccui.Helper:seekWidgetByName(taskItem,"LabelDes")
                LabelDes:setString(task_desc) 
                local Label2=ccui.Helper:seekWidgetByName(taskItem,"Label2")
                Label2:setString("可领取")
                local Label3=ccui.Helper:seekWidgetByName(taskItem,"Label3")

                Label3:setString(" x".." "..gold)
                local ImageBao=ccui.Helper:seekWidgetByName(taskItem,"ImageBao")
                local received = ccui.Helper:seekWidgetByName(taskItem,"received")
                local yiwancheng = ccui.Helper:seekWidgetByName(taskItem,"yiwancheng")
                received:setVisible(false)
                yiwancheng:setVisible(false)
                taskItem:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1))  
                --self.received:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1)) 
                if status==0 then--0未完成  

                elseif status==1 then--1可领取 
                    yiwancheng:setVisible(true)                
                    --self.taskScrollView:addChild(taskItem)
                elseif status==2 then--2已领奖

                    --self.taskScrollView:addChild(taskItem)
                    received:setVisible(true)
                end                            
                self.taskScrollView:addChild(taskItem)


                local function TaskMenuCallBack(sender,event)
                    if sender == taskItem and event == cc.EventCode.ENDED then
                        --self.taskImageItem:setVisible(false)
                        if status==0 then--0未完成  
                            PopDialog:getInstance():popText("副本暂未挑战成功",cc.c3b(0,255,0))
                        elseif status==1 then--1可领取

                            Http:req("taskCompleted",{rolerTaskId = rolerTaskId},function(res)
                                if res.data["result"] == 0 then
                                    cclog("领取成功")
                                    self:updataTipPic()
                                    PopDialog:getInstance():popText("领取成功",cc.c3b(0,255,0))
                                    --  self.tag=self.tag+1
                                    sender:setEnabled(false)
                                    received:setVisible(true)
                                end
                            end)



                        elseif status==2 then--2已领奖

                            PopDialog:getInstance():popText("已领取，不能重复领取",cc.c3b(0,255,0))
                        end

                    end
                end

                taskItem:addTouchEventListener(TaskMenuCallBack)
                taskItem:setTag(tonumber(taskId))

            end  

        end)

end


function TaskUI:conlectLayer()
    cclog("conlect")

    self.topLabel:setVisible(false)

    Http:req("taskStart",
        {type=6},
        function(res) 
            if res.err then cclog("err!!!") return end
            local itemList = res.data["taskList"]
            local tasksize = table.getn(itemList)
            cclog("taskSize"..tasksize)
            self.labelTips:setString("收集多多，奖励多多,")
            self.labelTips:setVisible(true)
            self.labelTips2:setVisible(true)
            self.labelTips2:setString("麻麻再也不怕我养不起英雄了!")
            self.taskScrollView:setVisible(true)
            self.fengexian:setVisible(true)
            self.taskScrollView2:setVisible(false)
            self.taskScrollView:removeAllChildren()
            self.taskScrollView:setBounceEnabled(true)
            local ScrovSize = self.taskScrollView:getInnerContainerSize()
            local ScrovH = ScrovSize.height

            if tasksize>6 then
                if tasksize%2 == 0 then
                    ScrovH = (90+5)*(tasksize/2)
                else
                    ScrovH = (90+5)*(math.floor(tasksize/2)+1)                  
                end

            end
            ScrovSize.height = ScrovH
            self.taskScrollView:setInnerContainerSize(ScrovSize)

            for i=1,tasksize do

                local taskId=tostring(res.data["taskList"][i]["taskId"])
                local taskItemlist=  taskConfig[taskId]
                local task_desc=tostring(taskItemlist["task_desc"])
                local status=res.data["taskList"][i]["status"] 
                local gold=tostring(taskItemlist["gold"])
                local taskItem=ccs.GUIReader:getInstance():widgetFromBinaryFile("taskItem.csb")                        
                local PanelTaskItem = ccui.Helper:seekWidgetByName(taskItem, "PanelTaskItem")
                local taskImageItem=ccui.Helper:seekWidgetByName(taskItem,"taskImageItem")
                local LabelDes=ccui.Helper:seekWidgetByName(taskItem,"LabelDes")
                LabelDes:setString(task_desc) 
                local Label2=ccui.Helper:seekWidgetByName(taskItem,"Label2")
                Label2:setString("可领取")
                local Label3=ccui.Helper:seekWidgetByName(taskItem,"Label3")

                Label3:setString(" x".." "..gold)
                local ImageBao=ccui.Helper:seekWidgetByName(taskItem,"ImageBao")
                local received = ccui.Helper:seekWidgetByName(taskItem,"received")
                local yiwancheng = ccui.Helper:seekWidgetByName(taskItem,"yiwancheng")
                received:setVisible(false)
                yiwancheng:setVisible(false)
                taskItem:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1))  
                --self.received:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1))                             
                self.taskScrollView:addChild(taskItem)

                if status==0 then--0未完成  

                elseif status==1 then--1可领取  
                    yiwancheng:setVisible(true)               
                    --self.taskScrollView:addChild(taskItem)
                elseif status==2 then--2已领奖

                    --self.taskScrollView:addChild(taskItem)
                    received:setVisible(true)
                end

                local rolerTaskId=tostring(res.data["taskList"][i]["rolerTaskId"])
                local status=res.data["taskList"][i]["status"]


                local function TaskMenuCallBack(sender,event)
                    if sender == taskItem and event == cc.EventCode.ENDED then
                        
                        --self.taskImageItem:setVisible(false)
                        if status==0 then--0未完成  
                            PopDialog:getInstance():popText("您的英雄数量太少了",cc.c3b(0,255,0))
                        elseif status==1 then--1可领取

                            Http:req("taskCompleted",{rolerTaskId = rolerTaskId},function(res)
                                if res.data["result"] == 0 then
                                    cclog("领取成功")
                                    self:updataTipPic()
                                    PopDialog:getInstance():popText("领取成功",cc.c3b(0,255,0))
                                    -- self.tag=self.tag+1
                                    sender:setEnabled(false)
                                    received:setVisible(true)
                                end
                            end)

                        elseif status==2 then--2已领奖

                            PopDialog:getInstance():popText("已领取，不能重复领取",cc.c3b(0,255,0))
                        end

                    end
                end

                taskItem:addTouchEventListener(TaskMenuCallBack)
                taskItem:setTag(tonumber(taskId))

            end  

        end)

end
function TaskUI:refreshUI()

end


function TaskUI:updataTipPic()
    Http:req("GetRedDot",{multType = "32"},function(res)
        for i = 1, 6 do
            local n_hongdian = ccui.Helper:seekWidgetByName(self.rootNode,"hongdian_"..i)
            if res.data.dot[i]["visible"] == 0 then
                n_hongdian:setVisible(false)
            else
                n_hongdian:setVisible(true)
            end 
        end

    end)
end

return TaskUI
