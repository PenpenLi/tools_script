--ShopUI.lua, created by php script. chris.li--
require "Cocos2d"
local shopConfig = require("config.define_shop")
local taskConfig = require("config.define_task")
local PopDialog = require "common/PopDialog"
local TaskUI = class("TaskUI", function()
    return cc.Layer:create()
end)

function TaskUI:create()
    local layer = TaskUI.new()
    layer:initM()
    
    --self:
    
    return layer
end

function TaskUI:ctor()
end

function TaskUI:onEnter()
    self:updataTipPic()
end

function TaskUI:onExit()
end

function TaskUI:initM()

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
    local  labBg = cc.Scale9Sprite:create("duihua.png")
    labBg:setAnchorPoint(cc.p(0.5,0.5))
    labBg:setPreferredSize(cc.size(248,97))
    labBg:setPosition(cc.p(ImageTipX+2,ImageTipY))
    labBg:setTag(10)
    self.ImageRenwu:addChild(labBg)
    self.labelTips:setVisible(true)
    self.labelTips2:setVisible(true)
    self.labelTips:setString("每天签到就可以领取奖励了,")
    self.labelTips2:setString("明天不要忘记哦")
    local tapLab=self.labelTips:getTag()
    local labbg=labBg:getTag()
    cclog("labelTips========="..tapLab)
    cclog("labbg========="..labbg)

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
    elseif sender == self.qiandaoBtn and event == cc.EventCode.ENDED then
        cclog("[TaskUI]--qiandaoBtn touched--") 
        self.qiandaoBtn:loadTextures("qiandao01.png","qiandao.png","qiandao.png")    
        self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
        self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
        self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")
        self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
        self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png") 
        self:signLayer() 
        --self:updataTipPic() 

    elseif sender == self.logBtn and event == cc.EventCode.ENDED then
        cclog("[taskUI]--logBtn touched--")
        self.logBtn:loadTextures("denglu01.png","denglu.png","denglu.png")
        self.qiandaoBtn:loadTextures("qiandao.png","qiandao.png","qiandao.png")
        self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
        self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")
        self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
        self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png")
        self:logLayer()
        --self:updataTipPic()

    elseif sender == self.levelupBtn and event == cc.EventCode.ENDED then
        cclog("[TaskUI]--levelupBtn touched--")

        self.levelupBtn:loadTextures("shengji01.png","shengji.png","shengji.png")
        self.qiandaoBtn:loadTextures("qiandao.png","qiandao.png","qiandao.png")
        self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
        self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")
        self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
        self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png") 
        self:leveLLayer()
        --self:updataTipPic()
        
    elseif sender == self.putongBtn and event == cc.EventCode.ENDED then
        cclog("[taskUI]--putongBtn touched--")
        self.putongBtn:loadTextures("putongben01.png","putongben.png","putongben.png")
        self.qiandaoBtn:loadTextures("qiandao.png","qiandao.png","qiandao.png")
        self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
        self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
        self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
        self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png")
        self:commonLayer()
        --self:updataTipPic()
        
    elseif sender == self.jingyingBtn and event == cc.EventCode.ENDED then
        cclog("[tasUI]--m_add10Btn touched--")
        self.jingyingBtn:loadTextures("jingyingben01.png","jingyingben.png","jingyingben.png") 
        self.qiandaoBtn:loadTextures("qiandao.png","qiandao.png","qiandao.png")
        self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
        self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
        self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")       
        self.shoujiBtn:loadTextures("shouji.png","shouji.png","shouji.png")
        self:expertLayer()
        --self:updataTipPic()
        
    elseif sender == self.shoujiBtn and event == cc.EventCode.ENDED then
        cclog("[tassUI]--shoujiBtn touched--")
        self.shoujiBtn:loadTextures("shouji01.png","shouji.png","shouji.png") 
        self.qiandaoBtn:loadTextures("qiandao.png","qiandao.png","qiandao.png")
        self.logBtn:loadTextures("denglu.png","denglu.png","denglu.png")
        self.levelupBtn:loadTextures("shengji.png","shengji.png","shengji.png")
        self.putongBtn:loadTextures("putongben.png","putongben.png","putongben.png")
        self.jingyingBtn:loadTextures("jingyingben.png","jingyingben.png","jingyingben.png")
        self:conlectLayer()
        --self:updataTipPic()
        
    elseif sender == self.taskScrollView and event == cc.EventCode.ENDED then
        cclog("[tasUI]--taskScrollView touched--")

    elseif sender == self.taskScrollView2 and event == cc.EventCode.ENDED then
        cclog("[tasUI]--taskScrollView2 touched--")

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
            self.taskScrollView2:removeAllChildren()
            self.taskScrollView2:setBounceEnabled(true)

            local ScrovSize = self.taskScrollView2:getInnerContainerSize()
            local ScrovH = ScrovSize.height
            self.taskScrollView2:setScale(1.0)
            if tasksize>6 then
                if tasksize%5 == 0 then
                    ScrovH = (130+10)*(tasksize/5)
                else
                    ScrovH = (130+10)*(math.floor(tasksize/5)+1)                  
                end

            end
            ScrovSize.height = ScrovH
            self.taskScrollView2:setInnerContainerSize(ScrovSize)
            self.tag=0
            for i=1,tasksize do
                local rolerTaskId=tostring(res.data["taskList"][i]["rolerTaskId"])
                local taskId=tostring(res.data["taskList"][i]["taskId"])
                local status=res.data["taskList"][i]["status"]
                local taskItemlist= taskConfig[taskId]
                --获取物品Id
                local itmId=tostring(taskItemlist["item"])
                ---cclog("你好~"..itmId)                           
                --取字符串 itmId             
                local _,q = string.find(itmId,":")                
                --cclog("冒号 标记"..q)  
                local goodsId=string.sub(itmId,1,q-1)
                --cclog("goodsId~~~"..goodsId) 
                --num 
                local nums =string.sub(itmId,3,string.len(itmId))
                --cclog("num~~~"..nums) 
                --itemList
                local dataitem=shopConfig[goodsId]              
                local task_desc=tostring(taskItemlist["task_desc"])    
                cclog("task描述~~~"..task_desc)      
                cclog("当前这"..i.."的状态是~"..status.."~~~taskID~~~"..taskId)   
                local taskItemImage =ccs.GUIReader:getInstance():widgetFromBinaryFile("taskItemImage.csb")                        
                local PanelitemImage = ccui.Helper:seekWidgetByName(taskItemImage, "PanelitemImage")
                local ImageItem=ccui.Helper:seekWidgetByName(taskItemImage,"ImageItem")
                local Imagekuang=ccui.Helper:seekWidgetByName(taskItemImage,"Imagekuang")
                ImageItem:loadTexture(dataitem["icon"]..".png")
                Imagekuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                --type 16~ suipian 
                local ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                local type=(dataitem["type"])
                if type==16 then
                    -- self.ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                    ImageSuipian:setVisible(true) 
                else
                    ImageSuipian:setVisible(false) 
                end

                local numLabel=ccui.Helper:seekWidgetByName(taskItemImage,"num")
                numLabel:setString(nums)
                local labelPlus=ccui.Helper:seekWidgetByName(taskItemImage,"labelPlus")
                taskItemImage:setPosition(4+(i-1)%5*(100+4),ScrovSize.height-130*(math.floor((i-1)/5)+1))
                labelPlus:setPosition(4+(i-1)%5*(100+4),ScrovSize.height-130*(math.floor((i-1)/5)+1)-22)
                labelPlus:setString(task_desc) 
                local receivedImage=ccui.Helper:seekWidgetByName(taskItemImage,"receivedImage")
                receivedImage:setVisible(false)

                if status==0 then--0未完成  
                    self.taskScrollView2:addChild(taskItemImage)
                elseif status==1 then--1可领取
                    self.taskScrollView2:addChild(taskItemImage)
                elseif status==2 then--2已领奖
                    self.tag=self.tag+1
                    self.taskScrollView2:addChild(taskItemImage)
                    receivedImage:setVisible(true)
                end

                --self.taskScrollView:addChild(taskItemImage)

                local function TaskMenuCallBack(sender,event)
                    if sender == taskItemImage and event == cc.EventCode.ENDED then
                        cclog("事件监听~~~~"..sender:getTag())

                        --self.taskImageItem:setVisible(false)
                        if status==0 then--0未完成  
                            PopDialog:getInstance():popText("未达到要求,不能领取",cc.c3b(0,255,0))
                        elseif status==1 then--1可领取

                            Http:req("taskCompleted",{rolerTaskId = rolerTaskId},function(res)
                                if res.data["result"] == 0 then
                                    PopDialog:getInstance():popText("领取成功！",cc.c3b(0,255,0))
                                    cclog("领取成功")
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
            cclog("签到"..day)
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
            cclog("taskSize"..tasksize)
            self.taskScrollView2:setVisible(true)
            self.taskScrollView:setVisible(false)          
            self.taskScrollView2:removeAllChildren()
            self.taskScrollView2:setBounceEnabled(true)

            self.labelTips:setString("每天登陆就可以领取奖励了,")
            self.labelTips2:setString("明天不要忘记哦")
            self.labelTips:setVisible(true)
            self.labelTips2:setVisible(true)

            local ScrovSize = self.taskScrollView2:getInnerContainerSize()
            local ScrovH = ScrovSize.height
            self.taskScrollView:setScale(1.0)
            if tasksize>10 then
                if tasksize%5 == 0 then
                    ScrovH = (130+10)*(tasksize/5)
                else
                    ScrovH = (130+10)*(math.floor(tasksize/5)+1)                  
                end

            end
            ScrovSize.height = ScrovH
            self.taskScrollView2:setInnerContainerSize(ScrovSize)
            self.dg=0
            for i=1,tasksize do
                local rolerTaskId=tostring(res.data["taskList"][i]["rolerTaskId"])
                local taskId=tostring(res.data["taskList"][i]["taskId"])
                local status=res.data["taskList"][i]["status"]
                local taskItemlist= taskConfig[taskId]
                --获取物品Id
                local itmId=tostring(taskItemlist["item"])
                cclog("你好~"..itmId)                           
                --取字符串 itmId             
                local _,q = string.find(itmId,':')                
                cclog("冒号 标记"..q)  
                if q ~=nil then
                    self.goodsId=string.sub(itmId,1,q-1)
                    --cclog("goodsId~~~"..goodsId) 
                end           
                local goodsId=self.goodsId            
                --num 
                local nums =string.sub(itmId,3,string.len(itmId))
                cclog("num~~~"..nums) 
                --itemList
                local dataitem=shopConfig[goodsId]              
                local task_desc=tostring(taskItemlist["task_desc"])             
                local taskItemImage =ccs.GUIReader:getInstance():widgetFromBinaryFile("taskItemImage.csb")                        
                local PanelitemImage = ccui.Helper:seekWidgetByName(taskItemImage, "PanelitemImage")
                local ImageItem=ccui.Helper:seekWidgetByName(taskItemImage,"ImageItem")
                local Imagekuang=ccui.Helper:seekWidgetByName(taskItemImage,"Imagekuang")
                ImageItem:loadTexture(dataitem["icon"]..".png")
                Imagekuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                --type 16~ suipian 
                local ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                local type=(dataitem["type"])
                if type==16 then
                    -- self.ImageSuipian=ccui.Helper:seekWidgetByName(taskItemImage,"ImageSuipian")
                    ImageSuipian:setVisible(true) 
                else
                    ImageSuipian:setVisible(false) 
                end

                local numLabel=ccui.Helper:seekWidgetByName(taskItemImage,"num")
                numLabel:setString(nums)
                local labelPlus=ccui.Helper:seekWidgetByName(taskItemImage,"labelPlus")
                taskItemImage:setPosition(2+(i-1)%5*(100+4),ScrovSize.height-130*(math.floor((i-1)/5)+1))
                labelPlus:setPosition(2+(i-1)%5*(100+4),ScrovSize.height-130*(math.floor((i-1)/5)+1)-20)
                labelPlus:setString(task_desc)               
                local receivedImage=ccui.Helper:seekWidgetByName(taskItemImage,"receivedImage")
                receivedImage:setVisible(false)

                if status==0 then--0未完成  
                    self.taskScrollView2:addChild(taskItemImage)
                elseif status==1 then--1可领取
                    self.dg=self.dg+1
                    self.taskScrollView2:addChild(taskItemImage)
                elseif status==2 then--2已领奖
                    self.dg=self.dg+1
                    self.taskScrollView2:addChild(taskItemImage)
                    receivedImage:setVisible(true)
                end


                local function TaskMenuCallBack(sender,event)
                    if sender == taskItemImage and event == cc.EventCode.ENDED then
                        cclog("事件监听~~~~"..sender:getTag())
                        cclog("hello~~~~")
                        
                        --self.taskImageItem:setVisible(false)
                        if status==0 then--0未完成  
                            PopDialog:getInstance():popText("当前奖励不能领取",cc.c3b(0,255,0))
                        elseif status==1 then--1可领取                                          
                            Http:req("taskCompleted",{rolerTaskId = rolerTaskId},function(res)
                                if res.data["result"] == 0 then
                                    sender:setEnabled(false)
                                    cclog("领取成功") 
                                    self:updataTipPic()       
                                    PopDialog:getInstance():popText("领取成功",cc.c3b(0,255,0))                         
                                    receivedImage:setVisible(true)
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
            local dy=tostring(self.dg)
            cclog("登陆~~~~~"..dy)
            local topLayer= ccui.Helper:seekWidgetByName(self.rootNode,"topLabel")
            topLayer:setVisible(true)
            -- topLayer:setPosition(cc.p(topLayer:getPositionX(),topLayer:getPositionY()-4))
            topLayer:setString("天天登陆就可以领取超赞宝物 ,您已经登陆"..dy.."天")
        end)

end

function TaskUI:leveLLayer()
    self.topLabel:setVisible(false)
    cclog("level")
    Http:req("taskStart",
        {type=3},
        function(res) 
            if res.err then cclog("err!!!") return end
            local itemList = res.data["taskList"]
            local tasksize = table.getn(itemList)
            cclog("taskSize~~"..tasksize)
            self.taskScrollView:setVisible(true)
            self.taskScrollView2:setVisible(false)
            self.taskScrollView:removeAllChildren()
            self.taskScrollView2:removeAllChildren()          
            self.taskScrollView:setBounceEnabled(true)
            self.labelTips:setString("到达指定等级就可以")
            self.labelTips2:setString("领取奖励了哦!快点升级吧!")
            self.labelTips:setVisible(true)
            self.labelTips2:setVisible(true)
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
            self.taskScrollView2:setScale(1.0)
            for i=1,tasksize do
                local taskId=tostring(res.data["taskList"][i]["taskId"])        
                local taskItemlist= taskConfig[taskId]
                local task_desc=tostring(taskItemlist["task_desc"])
                local gold=tostring(taskItemlist["gold"])
                local status=res.data["taskList"][i]["status"]                
                local rolerTaskId=tostring(res.data["taskList"][i]["rolerTaskId"])
                cclog("当前这"..i.."的状态是~"..status.."taskID~~~"..taskId.."rolerTaskId~~~~"..rolerTaskId)
                local taskItem=ccs.GUIReader:getInstance():widgetFromBinaryFile("taskItem.csb")    

                local PanelTaskItem = ccui.Helper:seekWidgetByName(taskItem, "PanelTaskItem")
                local taskImageItem=ccui.Helper:seekWidgetByName(taskItem,"taskImageItem")
                local LabelDes=ccui.Helper:seekWidgetByName(taskItem,"LabelDes")
                LabelDes:setString(task_desc) 
                local Label2=ccui.Helper:seekWidgetByName(taskItem,"Label2")
                Label2:setString("可领取")
                local Label3=ccui.Helper:seekWidgetByName(taskItem,"Label3")
                Label3:setString("*".." "..gold)
                local ImageBao=ccui.Helper:seekWidgetByName(taskItem,"ImageBao")
                local received = ccui.Helper:seekWidgetByName(taskItem,"received")
                received:setVisible(false)
                taskItem:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1))  
                -- (2+(i-1)%2*412,ScrovSize.height-140*(math.floor((i-1)/2)+1))
                --self.received:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1))


                self.taskScrollView:addChild(taskItem)

                if status==0 then--0未完成  

                elseif status==1 then--1可领取  
                    cclog("加 进来11111")                
                    --self.taskScrollView:addChild(taskItem)
                elseif status==2 then--2已领奖

                    --self.taskScrollView:addChild(taskItem)
                    received:setVisible(true)
                end

                local function TaskMenuCallBack(sender,event)
                    if sender == taskItem and event == cc.EventCode.ENDED then
                        cclog("事件监听~~~~"..sender:getTag())
                        
                         

                        if status==0 then--0未完成  
                            PopDialog:getInstance():popText("别太急，你的等级还未达到",cc.c3b(0,255,0))
                        elseif status==1 then--1可领取

                            Http:req("taskCompleted",{rolerTaskId = rolerTaskId},function(res)
                                if res.data["result"] == 0 then

                                    sender:setEnabled(false)

                                    cclog("领取成功")
                                    self:updataTipPic()   
                                    PopDialog:getInstance():popText("领取成功",cc.c3b(0,255,0))                               
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


                Label3:setString("*".." "..gold)
                local ImageBao=ccui.Helper:seekWidgetByName(taskItem,"ImageBao")
                local received = ccui.Helper:seekWidgetByName(taskItem,"received")
                received:setVisible(false)
                taskItem:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1))  
                --self.received:setPosition(2+(i-1)%2*266,ScrovSize.height-94(math.floor((i-1)/2)+1))  
                if status==0 then--0未完成  

                elseif status==1 then--1可领取  
                    cclog("加 进来11111")                
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

                Label3:setString("*".." "..gold)
                local ImageBao=ccui.Helper:seekWidgetByName(taskItem,"ImageBao")
                local received = ccui.Helper:seekWidgetByName(taskItem,"received")
                received:setVisible(false)
                taskItem:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1))  
                --self.received:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1)) 
                if status==0 then--0未完成  

                elseif status==1 then--1可领取  
                    cclog("加 进来11111")                
                    --self.taskScrollView:addChild(taskItem)
                elseif status==2 then--2已领奖

                    --self.taskScrollView:addChild(taskItem)
                    received:setVisible(true)
                end                            
                self.taskScrollView:addChild(taskItem)


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

                Label3:setString("*".." "..gold)
                local ImageBao=ccui.Helper:seekWidgetByName(taskItem,"ImageBao")
                local received = ccui.Helper:seekWidgetByName(taskItem,"received")
                received:setVisible(false)
                taskItem:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1))  
                --self.received:setPosition(2+(i-1)%2*266,ScrovSize.height-94*(math.floor((i-1)/2)+1))                             
                self.taskScrollView:addChild(taskItem)

                if status==0 then--0未完成  

                elseif status==1 then--1可领取  
                    cclog("加 进来11111")                
                    --self.taskScrollView:addChild(taskItem)
                elseif status==2 then--2已领奖

                    --self.taskScrollView:addChild(taskItem)
                    received:setVisible(true)
                end

                local rolerTaskId=tostring(res.data["taskList"][i]["rolerTaskId"])
                local status=res.data["taskList"][i]["status"]


                local function TaskMenuCallBack(sender,event)
                    if sender == taskItem and event == cc.EventCode.ENDED then
                        cclog("事件监听~~~~"..sender:getTag())
                        cclog("hello~~~~")
                        
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
