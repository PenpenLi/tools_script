--ShopUI.lua, created by php script. chris.li--
require "Cocos2d"
local shopConfig = require("config.define_shop")
local openConfig = require("config.define_open_act")
local rewardConfig = require("config.define_open_act_reward")
local PopDialog = require "common/PopDialog"

local ServerUI = class("ServerUI", function()
    
    return cc.Layer:create()
end)

function ServerUI:create()
    
    local layer = ServerUI.new()
    layer:initM()
    return layer
end

function ServerUI:ctor()
    
end

function ServerUI:onEnter()
    
    self:updataTipPic()
    self:initUI()
end

function ServerUI:onExit()
    

end

function ServerUI:initM()
    
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)

    self.rootNode = ccs.GUIReader:getInstance():widgetFromBinaryFile("openServerUI.csb")
    self:addChild(self.rootNode)

    local function touchEventHandler(sender, event)
        self:handleTouchEvent(sender, event)
    end

    --m_buyRootPanel: Layout
    self.openServerPannel = ccui.Helper:seekWidgetByName(self.rootNode,"openServerPannel")
    self.openServerPannel:addTouchEventListener(touchEventHandler)
    -- self.m_buyRootPanel:setVisible(false)
    --self.m_buyRootPanel:setEnabled(false)
    self.openServerPannel:setBackGroundImage("ditu.jpg")
    --m_layerPanel: Layout
    self.ImageBack = ccui.Helper:seekWidgetByName(self.rootNode, "ImageBack")
    --self.m_layerPanel:addTouchEventListener(touchEventHandler)
    --self.m_layerPanel:setBackGroundImage("ditu.jpg")

    self.imagebackUp = ccui.Helper:seekWidgetByName(self.rootNode, "imagebackUp")
    self.ImageTop = ccui.Helper:seekWidgetByName(self.rootNode, "ImageTop")
    self.ImageKuang = ccui.Helper:seekWidgetByName(self.rootNode, "ImageKuang")

    --closeBtn: Button
    self.closeBtn = ccui.Helper:seekWidgetByName(self.rootNode, "closeBtn")
    self.closeBtn:addTouchEventListener(touchEventHandler)

    self.wardLabel = ccui.Helper:seekWidgetByName(self.rootNode, "wardLabel")
    self.chargedLabel = ccui.Helper:seekWidgetByName(self.rootNode, "chargedLabel")
    self.charge = ccui.Helper:seekWidgetByName(self.rootNode, "charge")

    self.ImageDown = ccui.Helper:seekWidgetByName(self.rootNode, "ImageDown")

    self.ScrollViewLeft = ccui.Helper:seekWidgetByName(self.rootNode, "ScrollViewLeft")
    self.ScrollViewLeft:addTouchEventListener(touchEventHandler)

    self.ScrollViewDown = ccui.Helper:seekWidgetByName(self.rootNode, "ScrollViewDown")
    self.ScrollViewDown:addTouchEventListener(touchEventHandler)

    local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
    LabelOne2:setVisible(false)
    local LabelTwo2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo2")
    LabelTwo2:setVisible(false)
    local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")
    labelThree2:setVisible(false)
    local chargPlabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargPlabel")
    chargPlabel:setVisible(false)
    --self:initUI()
end


function ServerUI:handleTouchEvent(sender, event)
    if sender == nil then
        cclog("[ERR]unknown sender")

    elseif sender == self.openServerPannel and event == cc.EventCode.ENDED then
        cclog("[]--openServerPannel touched--")

    elseif sender == self.closeBtn and event == cc.EventCode.ENDED then

        local scene = require("scene.home.HomeScene"):create()
        cc.Director:getInstance():replaceScene(scene)

    elseif sender == self.ScrollViewLeft and event == cc.EventCode.ENDED then
        cclog("--ScrollViewDown touched--")

    elseif sender == self.ScrollViewDown and event == cc.EventCode.ENDED then
        cclog("--ScrollViewDown touched--")

    end
end

--init your ui here
function ServerUI:initUI()
  
    self.ScrollViewLeft:setVisible(true)
    self.ScrollViewLeft:setBounceEnabled(true)
    local ScrovSize = self.ScrollViewLeft:getInnerContainerSize()
    local ScrovH = ScrovSize.height
    self.ScrollViewLeft:setScale(1.0)
    --button 的高度          
    ScrovH = 80*7                       
    ScrovSize.height = ScrovH
    self.ScrollViewLeft:setInnerContainerSize(ScrovSize)

    local function leftBtnCallBack(sender,event)
        -- if sender == chargeItem and event == cc.EventCode.ENDED then  
        if sender:getTag()==1 and event == cc.EventCode.ENDED then 
            --chargeImg:setVisible(true)
            --sender:loadTexture("huodong".."1"..".png")
            self.tableItem[1]:loadTexture("huodong".."1"..".png")
            self.tableItem[2]:loadTexture("huodong0".."2"..".png")           
            self.tableItem[3]:loadTexture("huodong0".."3"..".png")         
            self.tableItem[4]:loadTexture("huodong0".."4"..".png")           
            self.tableItem[5]:loadTexture("huodong0".."5"..".png")            
            self.tableItem[6]:loadTexture("huodong0".."6"..".png")             
            self.tableItem[7]:loadTexture("huodong0".."7"..".png")
            self:charg()
        elseif sender:getTag()==2 and event == cc.EventCode.ENDED then 
            self.tableItem[2]:loadTexture("huodong".."2"..".png")

            self.tableItem[1]:loadTexture("huodong0".."1"..".png")

            self.tableItem[3]:loadTexture("huodong0".."3"..".png")         
            self.tableItem[4]:loadTexture("huodong0".."4"..".png")           
            self.tableItem[5]:loadTexture("huodong0".."5"..".png")            
            self.tableItem[6]:loadTexture("huodong0".."6"..".png")             
            self.tableItem[7]:loadTexture("huodong0".."7"..".png")
            self:consume()
        elseif sender:getTag()==3 and event == cc.EventCode.ENDED then 
            self.tableItem[3]:loadTexture("huodong".."3"..".png")
            self.tableItem[1]:loadTexture("huodong0".."1"..".png")
            self.tableItem[2]:loadTexture("huodong0".."2"..".png")           

            self.tableItem[4]:loadTexture("huodong0".."4"..".png")           
            self.tableItem[5]:loadTexture("huodong0".."5"..".png")            
            self.tableItem[6]:loadTexture("huodong0".."6"..".png")             
            self.tableItem[7]:loadTexture("huodong0".."7"..".png")

            self:chargeBack()
        elseif sender:getTag()==4 and event == cc.EventCode.ENDED then 

            self.tableItem[4]:loadTexture("huodong".."4"..".png")
            self.tableItem[1]:loadTexture("huodong0".."1"..".png")
            self.tableItem[2]:loadTexture("huodong0".."2"..".png")           
            self.tableItem[3]:loadTexture("huodong0".."3"..".png")         

            self.tableItem[5]:loadTexture("huodong0".."5"..".png")            
            self.tableItem[6]:loadTexture("huodong0".."6"..".png")             
            self.tableItem[7]:loadTexture("huodong0".."7"..".png")
            self:atkList()                           
        elseif sender:getTag()==5 and event == cc.EventCode.ENDED then 
            self.tableItem[5]:loadTexture("huodong".."5"..".png")
            self.tableItem[1]:loadTexture("huodong0".."1"..".png")
            self.tableItem[2]:loadTexture("huodong0".."2"..".png")           
            self.tableItem[3]:loadTexture("huodong0".."3"..".png")         
            self.tableItem[4]:loadTexture("huodong0".."4"..".png")           

            self.tableItem[6]:loadTexture("huodong0".."6"..".png")             
            self.tableItem[7]:loadTexture("huodong0".."7"..".png")
            self:contest()
        elseif sender:getTag()==6 and event == cc.EventCode.ENDED then 
            self.tableItem[6]:loadTexture("huodong".."6"..".png")
            self.tableItem[1]:loadTexture("huodong0".."1"..".png")
            self.tableItem[2]:loadTexture("huodong0".."2"..".png")           
            self.tableItem[3]:loadTexture("huodong0".."3"..".png")         
            self.tableItem[4]:loadTexture("huodong0".."4"..".png")           
            self.tableItem[5]:loadTexture("huodong0".."5"..".png")            

            self.tableItem[7]:loadTexture("huodong0".."7"..".png")
            self:classm()
        elseif sender:getTag()==7 and event == cc.EventCode.ENDED then 
            self.tableItem[7]:loadTexture("huodong".."7"..".png")
            self.tableItem[1]:loadTexture("huodong0".."1"..".png")
            self.tableItem[2]:loadTexture("huodong0".."2"..".png")           
            self.tableItem[3]:loadTexture("huodong0".."3"..".png")         
            self.tableItem[4]:loadTexture("huodong0".."4"..".png")           
            self.tableItem[5]:loadTexture("huodong0".."5"..".png")            
            self.tableItem[6]:loadTexture("huodong0".."6"..".png") 

            self:public()
        end
        --end
    end

    self.tableItem={}
    self.tableDotItem={}
    for i=1,7 do
        local chargeItem = ccs.GUIReader:getInstance():widgetFromBinaryFile("buttonLlist.csb")
        
        chargeItem:setTag(i)
        local PanelCharge = ccui.Helper:seekWidgetByName(chargeItem, "PanelCharge")
        local charge = ccui.Helper:seekWidgetByName(chargeItem, "charge")
        
        local m_redDot = ccui.Helper:seekWidgetByName(chargeItem, "m_RedDot")
        --m_redDot:setName("m_redDot_"..i)
        --m_redDot:loadTexture("hongdian"..".png")
        charge:loadTexture("huodong0"..i..".png")
        self.tableDotItem[i] = m_redDot
        self.tableItem[i]=charge--放到table 中 为了以后切换材质~
        local chargeImg = ccui.Helper:seekWidgetByName(chargeItem, "chargeImg")
        chargeImg:setVisible(false)
        chargeItem:setPosition(cc.p(4,ScrovSize.height-(80+2)*i)) 
        self.ScrollViewLeft:addChild(chargeItem)

        chargeItem:addTouchEventListener(leftBtnCallBack)
        --chargeItem:setTag(i)       
    end
    self.tableItem[1]:loadTexture("huodong".."1"..".png")
    self.tableDotItem[1]:loadTexture("hongdian"..".png")
    self.tableItem[2]:loadTexture("huodong0".."2"..".png")
    self.tableDotItem[2]:loadTexture("hongdian"..".png")           
    self.tableItem[3]:loadTexture("huodong0".."3"..".png")
    self.tableDotItem[3]:loadTexture("hongdian"..".png")         
    self.tableItem[4]:loadTexture("huodong0".."4"..".png")
    self.tableDotItem[4]:loadTexture("hongdian"..".png")           
    self.tableItem[5]:loadTexture("huodong0".."5"..".png")
    self.tableDotItem[5]:loadTexture("hongdian"..".png")            
    self.tableItem[6]:loadTexture("huodong0".."6"..".png") 
    self.tableDotItem[6]:loadTexture("hongdian"..".png")            
    self.tableItem[7]:loadTexture("huodong0".."7"..".png")
    self.tableDotItem[7]:loadTexture("hongdian"..".png")


    self:refreshUI()
end
--refresh all ui here
function ServerUI:refreshUI()
    self:charg()
end

function ServerUI:charg()
    cclog("charg")

    local btnlist = ccui.Helper:seekWidgetByName(self.rootNode, "btnlist")
    local goalBtn = ccui.Helper:seekWidgetByName(self.rootNode, "goalBtn")
    goalBtn:setVisible(true)
    btnlist:setVisible(true)
    goalBtn:loadTextures("qieye02.png","qieye01.png","qieye01.png") 
    btnlist:loadTextures("qieye01.png","qieye01.png","qieye01.png")

    local function scrolList1()
        Http:req("openStart",{actId =1},
            function(res)
                cclog("scroll~1")
                local itemList = res.data["detail"]
                local actSize = table.getn(itemList)
                cclog("acSize"..actSize)
                local actId=tostring(res.data["act"]["actId"])
                cclog("adrID~~"..actId)
                local openActList=openConfig[actId]
                local actName=openActList["name"]
                local actDes=openActList["desc"]
                

                local actStartTm=res.data["act"]["startTm"]
                local acTmFrom=tostring(os.date("%c",actStartTm))
                local year="20"..string.sub(acTmFrom,7,8)
                
                local month=string.sub(acTmFrom,1,2)
                local day=string.sub(acTmFrom,4,5)
                local time=string.sub(acTmFrom,10,string.len(acTmFrom))

                local actEndTm=res.data["act"]["endTm"]
                local acTmEnd=tostring(os.date("%c",actEndTm))
                local year2="20"..string.sub(acTmEnd,7,8)               
                local month2=string.sub(acTmEnd,1,2)
                local day2=string.sub(acTmEnd,4,5)
                local time2=string.sub(acTmEnd,10,string.len(acTmEnd))

                local num=tostring(res.data["act"]["num"])
                
                --活动名称
                local labelOne = ccui.Helper:seekWidgetByName(self.rootNode,"labelOne")
                local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
                LabelOne2:setVisible(true)
                LabelOne2:setString(actName)
                --活动时间
                local LabelTwo = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo")                 
                local LabelTwo2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo2")
                LabelTwo2:setVisible(true)
                LabelTwo2:setString(year.."年"..month.."月"..day.."日"..time.."至"..year2.."年"..month2.."月"..day2.."日"..time2)              
                --活动规则
                local labelThree = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree")                 
                local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")  
                labelThree2:setVisible(true) 
                labelThree2:setString(actDes)             
                --当前已充值
                local wardLabel = ccui.Helper:seekWidgetByName(self.rootNode,"wardLabel")                
                local chargedLabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargedLabel") 
                chargedLabel:setString("您已充值:") 
                chargedLabel:setVisible(true)               
                local chargPlabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargPlabel") 
                chargPlabel:setVisible(true) 
               
                chargPlabel:setString(num)

                --下方scrollView
                self.ScrollViewDown:removeAllChildren()
                self.ScrollViewDown:setBounceEnabled(true)    
                local ScrovSize = self.ScrollViewDown:getInnerContainerSize()
                
                self.ScrollViewDown:setScale(1.0)
              
                ScrovSize.height =(100+6)*actSize
                self.ScrollViewDown:setInnerContainerSize(ScrovSize)

                for i=1,actSize do
                    local actRewardId=tostring(res.data["detail"][i]["actRewardId"])        
                    local status=res.data["detail"][i]["rec"] 
                                                 
                    local rewardItemlist= rewardConfig[actRewardId]  
                    local desc1=tostring(rewardItemlist["desc"]) --描述     
                    local PanelDown =ccs.GUIReader:getInstance():widgetFromBinaryFile("downItem.csb")                        
                    local ImageDownItem = ccui.Helper:seekWidgetByName(PanelDown, "ImageDownItem") 
                    local LabelTotal=ccui.Helper:seekWidgetByName(PanelDown,"LabelTotal")
                    LabelTotal:setString(desc1)
                    local LabelDes=ccui.Helper:seekWidgetByName(PanelDown,"LabelDes")
                    LabelDes:setVisible(false)
                    local LabelPre=ccui.Helper:seekWidgetByName(PanelDown,"LabelPre")
                    LabelPre:setVisible(false)
                    local PlayName=ccui.Helper:seekWidgetByName(PanelDown,"PlayName")
                    PlayName:setVisible(false)
                    local imageNum=ccui.Helper:seekWidgetByName(PanelDown,"imageNum") 
                    imageNum:setString(tostring(i))
                    local ButtonReceive=ccui.Helper:seekWidgetByName(PanelDown,"ButtonReceive")

                    PanelDown:setPosition(cc.p(2,ScrovSize.height-(100+6)*i)) 
                    self.ScrollViewDown:addChild(PanelDown)
                    --发现冒号的个数 然后判断当前有几个奖励物品
                    local itmId=tostring(rewardItemlist["item"])
                   
                    
                    
                   -- local pX=PanelDown:getPositionX()
                    --local pY=PanelDown:getPositionY()
              
                    self.nimei=0
                    self.comma=0
                    for q=1,string.len(itmId) do
                        local m=string.sub(itmId,q,q)
                        --print("m~~"..m)
                        if m == "," then
                            local item=string.sub(itmId,self.comma+1,q-1)
                            self.comma=q                          
                            self.nimei=self.nimei+1    
                                for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                        if n==":" then
                                        local goodsId=string.sub(item,1,v-1)                                       
                                        local goodsNum=string.sub(item,v+1,string.len(item))                                                                                 
                                        local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                        local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                        local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                        local dataitem=shopConfig[goodsId] 
                                        --print("goodID@@@@@@@@@@@@~~~"..goodsId)  
                                        ImageGoods:loadTexture(dataitem["icon"]..".png")
                                        ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")                                     
                                        local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                        labelNum:setString(goodsNum)
                                        
                                        ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                        ImageDownItem:addChild(ImageItem)
                                        
                                    local sp = cc.Sprite:create("lan1.png")
                                    sp:setScale(0.7)
                                    sp:setPosition(cc.p(45+(70+10)*(self.nimei),40));
                                    ImageDownItem:addChild(sp,200)
                                    local  animation = cc.Animation:create()
                                    for i=1,13 do
                                        animation:addSpriteFrameWithFile("lan"..i..".png");
                                    end
                                    animation:setDelayPerUnit(0.05)
                                    animation:setRestoreOriginalFrame(true)
                                    animation:setLoops(-1)                                   
                                    local animate = cc.Animate:create(animation)
                                    sp:runAction(animate)
                                    end
                                end
                        elseif q==string.len(itmId) then                    
                            local item=string.sub(itmId,self.comma+1,string.len(itmId))                          
                           
                            self.nimei=self.nimei+1   
                                                
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                    
                                    
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId]
                                    --print("goodID@@@@@@@@@@@@~~~"..goodsId)  
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                    
                                    
                                    local sp = cc.Sprite:create("lan1.png")
                                    sp:setScale(0.7)
                                    sp:setPosition(cc.p(45+(70+10)*(self.nimei),40));
                                    ImageDownItem:addChild(sp,200)
                                    local  animation = cc.Animation:create()
                                    for i=1,13 do
                                        animation:addSpriteFrameWithFile("lan"..i..".png");
                                    end
                                    animation:setDelayPerUnit(0.05)
                                    animation:setRestoreOriginalFrame(true)
                                    animation:setLoops(-1)                                   
                                    local animate = cc.Animate:create(animation)
                                    sp:runAction(animate)
                                end
                            end
                        end
                    end
                    
                    --领取button状态 设置button 上面的字体
                    if status==0 then--0未完成  
                        ButtonReceive:setTitleText("未达成")
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setEnabled(false)
                        --PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                    elseif status==1 then--1可领取
                        ButtonReceive:setTitleText("领取")
                        ButtonReceive:loadTextures("anniu.png","anniu00.png","anniugrey.png")
                        ButtonReceive:setEnabled(true)
                    elseif status==2 then--2已领奖
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setTitleText("已领取")

                    end

                    local function midBtnCallback(sender,event)
                        if sender == ButtonReceive and event == cc.EventCode.ENDED then
                            --领取button状态 设置button 上面的字体
                            if status==0 then--0未完成  
                                PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                            elseif status==1 then--1可领取
                                Http:req("openOver",{actRewardId=actRewardId},function(res)
                                    if res.data["result"] == 0 then
                                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                                        ButtonReceive:setTitleText("已领取")                                     
                                        sender:setEnabled(false)                            
                                    end
                                end)

                            elseif status==2 then--2已领奖
                                sender:setEnabled(false)

                            end
                            self:updataTipPic()

                        end
                    end

                    ButtonReceive:addTouchEventListener(midBtnCallback)
                end
            end)
    end
    scrolList1()
    local function scrolList2()        
        cclog("scroll~2")
        Http:req("openStart",{actId =2},
            function(res)
                local itemList = res.data["detail"]
                local actSize = table.getn(itemList)
               

                local actId=tostring(res.data["act"]["actId"])
               
                local openActList=openConfig[actId]
                local actName=openActList["name"]
                local actDes=openActList["desc"]
                --活动名称
                local labelOne = ccui.Helper:seekWidgetByName(self.rootNode,"labelOne")
                local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
                LabelOne2:setString(actName)
                local labelThree = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree")                 
                local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")   
                labelThree2:setString(actDes)   

                --下方scrollView
                self.ScrollViewDown:removeAllChildren()
                self.ScrollViewDown:setBounceEnabled(true)    
                local ScrovSize = self.ScrollViewDown:getInnerContainerSize()
                
                self.ScrollViewDown:setScale(1.0)
             
                ScrovSize.height =(100+10)*actSize
                self.ScrollViewDown:setInnerContainerSize(ScrovSize)
                for i=1,actSize do                                     
                    local actRewardId=tostring(res.data["detail"][i]["actRewardId"])        
                    local status=res.data["detail"][i]["rec"]
                    local PlayN=res.data["detail"][i]["name"]              
                    
                    local rewardItemlist= rewardConfig[actRewardId]  
                    local desc1=tostring(rewardItemlist["desc"]) --描述     
                    local PanelDown =ccs.GUIReader:getInstance():widgetFromBinaryFile("downItem.csb")                        
                    local ImageDownItem = ccui.Helper:seekWidgetByName(PanelDown, "ImageDownItem") 
                    local LabelTotal=ccui.Helper:seekWidgetByName(PanelDown,"LabelTotal")
                    LabelTotal:setString(desc1)
                    local LabelDes=ccui.Helper:seekWidgetByName(PanelDown,"LabelDes")
                    LabelDes:setVisible(false)
                    local imageNum=ccui.Helper:seekWidgetByName(PanelDown,"imageNum") 
                    imageNum:setString(tostring(i))
                    local ButtonReceive=ccui.Helper:seekWidgetByName(PanelDown,"ButtonReceive")
                    ButtonReceive:setVisible(false)

                    PanelDown:setPosition(cc.p(2,ScrovSize.height-(100+6)*i)) 
                    self.ScrollViewDown:addChild(PanelDown)

                    --发现冒号的个数 然后判断当前有几个奖励物品
                    local itmId=tostring(rewardItemlist["item"])
                    
                    self.nimei=0
                    self.comma=0
                    for q=1,string.len(itmId) do
                        local m=string.sub(itmId,q,q)
                        --print("m~~"..m)
                        if m == "," then
                            local item=string.sub(itmId,self.comma+1,q-1)
                            self.comma=q
                            
                            self.nimei=self.nimei+1    
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                                                            
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId] 
                                      
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")                                     
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        elseif q==string.len(itmId) then                    
                            local item=string.sub(itmId,self.comma+1,string.len(itmId))                          
                            
                            self.nimei=self.nimei+1   
                                                   
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                   
                                   
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId]
                                      
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        end
                    end
                    
                    local LabelPre=ccui.Helper:seekWidgetByName(PanelDown,"LabelPre") 
                    LabelPre:setString("当前") 
                    local PlayName=ccui.Helper:seekWidgetByName(PanelDown,"PlayName") 

                    if PlayN ~=nil then
                        
                        PlayName:setString(PlayN)
                    else
                       
                        PlayName:setString("无")
                    end            
                end
            end)
    end

    local chongzhiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "chongzhiBtn")
    chongzhiBtn:setVisible(true)
    local function downBtnCallback(sender,event)
        if sender==goalBtn and event ==cc.EventCode.ENDED then
            cclog("goal~Btn")
            --goalBtn:setTitleText("nimei")
            goalBtn:loadTextures("qieye02.png","qieye01.png","qieye01.png") 
            btnlist:loadTextures("qieye01.png","qieye01.png","qieye01.png")
            --self.goalBtn:setString("nihao")
            scrolList1()
        elseif sender == btnlist and event == cc.EventCode.ENDED then
            btnlist:loadTextures("qieye02.png","qieye01.png","qieye01.png")
            goalBtn:loadTextures("qieye01.png","qieye01.png","qieye01.png") 

            scrolList2()
        elseif sender==chongzhiBtn and event ==cc.EventCode.ENDED then
            local scene = require("scene.shop.RechargeScene"):create()
            cc.Director:getInstance():pushScene(scene)

        end
    end
    goalBtn:addTouchEventListener(downBtnCallback) 
    btnlist:addTouchEventListener(downBtnCallback)        
    chongzhiBtn:addTouchEventListener(downBtnCallback)


end


function ServerUI:consume()
    cclog("consume")
    local btnlist = ccui.Helper:seekWidgetByName(self.rootNode, "btnlist")  
    local goalBtn = ccui.Helper:seekWidgetByName(self.rootNode, "goalBtn")
    goalBtn:setVisible(false)
    btnlist:setVisible(false)
    Http:req("openStart",{actId =3},
        function(res)
            local itemList = res.data["detail"]
            local actSize = table.getn(itemList)
            
            local actId=tostring(res.data["act"]["actId"])
            
            local openActList=openConfig[actId]
            local actName=openActList["name"]
            local actdesc=openActList["desc"]
            



            local actStartTm=res.data["act"]["startTm"]
            local acTmFrom=tostring(os.date("%c",actStartTm))
            local year="20"..string.sub(acTmFrom,7,8)
           
            local month=string.sub(acTmFrom,1,2)
            local day=string.sub(acTmFrom,4,5)
            local time=string.sub(acTmFrom,10,string.len(acTmFrom))


            local actEndTm=res.data["act"]["endTm"]
            local acTmEnd=tostring(os.date("%c",actEndTm))
            local year2="20"..string.sub(acTmEnd,7,8)               
            local month2=string.sub(acTmEnd,1,2)
            local day2=string.sub(acTmEnd,4,5)
            local time2=string.sub(acTmEnd,10,string.len(acTmEnd))
            local num=tostring(res.data["act"]["num"])
           
            --活动名称
            local labelOne = ccui.Helper:seekWidgetByName(self.rootNode,"labelOne")
            local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
            LabelOne2:setString(actName)
            --活动时间
            local LabelTwo = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo")                 
            local LabelTwo2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo2")
            --时间先不用管他？？？？
            LabelTwo2:setString(year.."年"..month.."月"..day.."日"..time.."至"..year2.."年"..month2.."月"..day2.."日"..time2)              
            --活动规则
            local labelThree = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree")                 
            local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")   
            labelThree2:setString(actdesc) 
            LabelOne2:setVisible(true)
            LabelTwo2:setVisible(true)
            labelThree2:setVisible(true)            
            --当前已充值
            local wardLabel = ccui.Helper:seekWidgetByName(self.rootNode,"wardLabel")                
            local chargedLabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargedLabel")  
            chargedLabel:setString("您已消费 :")  
            chargedLabel:setVisible(true)         
            local chargPlabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargPlabel") 
            chargPlabel:setVisible(true)
            chargPlabel:setString(num)

            --下方scrollView
            self.ScrollViewDown:removeAllChildren()
            self.ScrollViewDown:setBounceEnabled(true)    
            local ScrovSize = self.ScrollViewDown:getInnerContainerSize()
            
            self.ScrollViewDown:setScale(1.0)
           
            ScrovSize.height =(100+10)*actSize
            self.ScrollViewDown:setInnerContainerSize(ScrovSize)

            local function scrolList1()
                cclog("scroll~1")
                for i=1,actSize do
                    local actRewardId=tostring(res.data["detail"][i]["actRewardId"])        
                    local status=res.data["detail"][i]["rec"]                

                    local rewardItemlist= rewardConfig[actRewardId]  

                    local desc1=tostring(rewardItemlist["desc"]) --描述     

                    local PanelDown =ccs.GUIReader:getInstance():widgetFromBinaryFile("downItem.csb")                        
                    local ImageDownItem = ccui.Helper:seekWidgetByName(PanelDown, "ImageDownItem") 
                    local LabelTotal=ccui.Helper:seekWidgetByName(PanelDown,"LabelTotal")
                    LabelTotal:setString(desc1)
                    local LabelDes=ccui.Helper:seekWidgetByName(PanelDown,"LabelDes")
                    LabelDes:setVisible(false)
                    local LabelPre=ccui.Helper:seekWidgetByName(PanelDown,"LabelPre")
                    LabelPre:setVisible(false)
                    local PlayName=ccui.Helper:seekWidgetByName(PanelDown,"PlayName")
                    PlayName:setVisible(false)
                    local imageNum=ccui.Helper:seekWidgetByName(PanelDown,"imageNum") 
                    imageNum:setString(tostring(i))
                    local ButtonReceive=ccui.Helper:seekWidgetByName(PanelDown,"ButtonReceive")


                    PanelDown:setPosition(cc.p(2,ScrovSize.height-(100+6)*i)) 
                    self.ScrollViewDown:addChild(PanelDown)

                    --发现冒号的个数 然后判断当前有几个奖励物品
                    local itmId=tostring(rewardItemlist["item"])
                    --cclog("itmId~~~~"..itmId)
                    self.nimei=0
                    self.comma=0
                    for q=1,string.len(itmId) do
                        local m=string.sub(itmId,q,q)
                        --print("m~~"..m)
                        if m == "," then
                            local item=string.sub(itmId,self.comma+1,q-1)
                            self.comma=q
                            cclog("wupin~~"..item)
                            self.nimei=self.nimei+1    
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    cclog("goodsId~~~~~"..goodsId)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                    cclog("goodsNum~~~~"..goodsNum)                                           
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId] 
                                     
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")                                     
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                   
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        elseif q==string.len(itmId) then                    
                            local item=string.sub(itmId,self.comma+1,string.len(itmId))                          
                          
                            self.nimei=self.nimei+1   
                                                  
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                    cclog("goodsId~~~~~"..goodsId)
                                    cclog("goodsNum~~~~"..goodsNum) 
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId]
                                    --print("goodID@@@@@@@@@@@@~~~"..goodsId)  
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        end
                    end
                    -- cclog("nimei的有这么多的~~"..self.nimei)



                    --领取button状态 设置button 上面的字体
                    if status==0 then--0未完成  
                        ButtonReceive:setTitleText("未达成")
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setEnabled(false)
                        --PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                    elseif status==1 then--1可领取
                        ButtonReceive:setTitleText("领取")
                        ButtonReceive:loadTextures("anniu.png","anniu00.png","anniugrey.png")
                        ButtonReceive:setEnabled(true)

                    elseif status==2 then--2已领奖
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setTitleText("已领取")                                     


                    end



                    local function midBtnCallback(sender,event)

                        if sender == ButtonReceive and event == cc.EventCode.ENDED then
                            --领取button状态 设置button 上面的字体
                            if status==0 then--0未完成  

                                PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                            elseif status==1 then--1可领取
                                Http:req("openOver",{actRewardId=actRewardId},function(res)
                                    if res.data["result"] == 0 then
                                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                                        ButtonReceive:setTitleText("已领取")                                     
                                        sender:setEnabled(false)                           
                                    end
                                end)

                            elseif status==2 then--2已领奖
                                sender:setEnabled(false)

                            end
                            self:updataTipPic()

                        end
                    end

                    ButtonReceive:addTouchEventListener(midBtnCallback)
                end
            end

            scrolList1()
            local chongzhiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "chongzhiBtn")
            chongzhiBtn:setVisible(true)
            local function downBtnCallback(sender,event)            
                if sender==chongzhiBtn and event ==cc.EventCode.ENDED then

                   local scene = require("scene.shop.RechargeScene"):create()
                   cc.Director:getInstance():pushScene(scene)
                                       
                end
            end                 
            chongzhiBtn:addTouchEventListener(downBtnCallback)
        end)

end
function ServerUI:chargeBack()
    cclog("chargeBack")
    local btnlist = ccui.Helper:seekWidgetByName(self.rootNode, "btnlist")  
    local goalBtn = ccui.Helper:seekWidgetByName(self.rootNode, "goalBtn")
    goalBtn:setVisible(false)
    btnlist:setVisible(false)
    Http:req("openStart",{actId =4},
        function(res)
            local itemList = res.data["detail"]
            local actSize = table.getn(itemList)
            
            local actId=tostring(res.data["act"]["actId"])
         
            local openActList=openConfig[actId]
            local actName=openActList["name"]
            local actdesc=openActList["desc"]
           
            local actStartTm=res.data["act"]["startTm"]
            local acTmFrom=tostring(os.date("%c",actStartTm))
            local year="20"..string.sub(acTmFrom,7,8)
            
            local month=string.sub(acTmFrom,1,2)
            local day=string.sub(acTmFrom,4,5)
            local time=string.sub(acTmFrom,10,string.len(acTmFrom))


            local actEndTm=res.data["act"]["endTm"]
            local acTmEnd=tostring(os.date("%c",actEndTm))
            local year2="20"..string.sub(acTmEnd,7,8)               
            local month2=string.sub(acTmEnd,1,2)
            local day2=string.sub(acTmEnd,4,5)
            local time2=string.sub(acTmEnd,10,string.len(acTmEnd))
            local num=tostring(res.data["act"]["num"])
           
            --活动名称
            local labelOne = ccui.Helper:seekWidgetByName(self.rootNode,"labelOne")
            local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
            LabelOne2:setString(actName)
            --活动时间
            local LabelTwo = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo")                 
            local LabelTwo2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo2")
            --时间先不用管他？？？？
            LabelTwo2:setString(year.."年"..month.."月"..day.."日"..time.."至"..year2.."年"..month2.."月"..day2.."日"..time2)            
            --活动规则
            local labelThree = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree")                 
            local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")   
            labelThree2:setString(actdesc)   
            LabelOne2:setVisible(true)
            LabelTwo2:setVisible(true)
            labelThree2:setVisible(true)          
            --当前已充值
            local wardLabel = ccui.Helper:seekWidgetByName(self.rootNode,"wardLabel")                
            local chargedLabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargedLabel")  
            chargedLabel:setVisible(true)
            chargedLabel:setString("您已充值 :")           
            local chargPlabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargPlabel") 
            chargPlabel:setVisible(true)
            chargPlabel:setString(num)

            --下方scrollView
            self.ScrollViewDown:removeAllChildren()
            self.ScrollViewDown:setBounceEnabled(true)    
            local ScrovSize = self.ScrollViewDown:getInnerContainerSize()        
            self.ScrollViewDown:setScale(1.0)          
            ScrovSize.height = (100+10)* actSize
            self.ScrollViewDown:setInnerContainerSize(ScrovSize)

            local function scrolList1()
                cclog("scroll~1")
                for i=1,actSize do

                    local actRewardId=tostring(res.data["detail"][i]["actRewardId"])        
                    local status=res.data["detail"][i]["rec"]                

                    local rewardItemlist= rewardConfig[actRewardId]  

                    local desc1=tostring(rewardItemlist["desc"]) --描述     

                    local PanelDown =ccs.GUIReader:getInstance():widgetFromBinaryFile("downItem.csb")                        
                    local ImageDownItem = ccui.Helper:seekWidgetByName(PanelDown, "ImageDownItem") 
                    local LabelTotal=ccui.Helper:seekWidgetByName(PanelDown,"LabelTotal")

                    LabelTotal:setVisible(false)
                    local LabelPre=ccui.Helper:seekWidgetByName(PanelDown,"LabelPre")
                    LabelPre:setVisible(false)
                    local PlayName=ccui.Helper:seekWidgetByName(PanelDown,"PlayName")
                    PlayName:setVisible(false)
                    local imageNum=ccui.Helper:seekWidgetByName(PanelDown,"imageNum") 
                    imageNum:setString(tostring(i))                  
                    local LabelDes=ccui.Helper:seekWidgetByName(PanelDown,"LabelDes")
                    LabelDes:setString(desc1)
                    LabelDes:setVisible(true)
                    local ButtonReceive=ccui.Helper:seekWidgetByName(PanelDown,"ButtonReceive")

                    --领取button状态 设置button 上面的字体
                    if status==0 then--0未完成

                        ButtonReceive:setTitleText("未达成")
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setEnabled(false)
                        --PopDialog:getInstance():popText("您未完成任务,不能领       取",cc.c3b(0,255,0))
                    elseif status==1 then--1可领取

                        ButtonReceive:setTitleText("领取")
                        ButtonReceive:loadTextures("anniu.png","anniu00.png","anniugrey.png")
                        ButtonReceive:setEnabled(true)

                    elseif status==2 then--2已领奖
                        --ButtonReceive:loadTextures        ("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setTitleText("已领取")
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setEnabled(false)
                    end



                    local function midBtnCallback(sender,event)

                        if sender == ButtonReceive and event == cc.EventCode.ENDED then
                            --领取button状态 设置button 上面的字体
                            if status==0 then--0未完成  

                                PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                            elseif status==1 then--1可领取
                                Http:req("openOver",{actRewardId=actRewardId},function(res)
                                    if res.data["result"] == 0 then
                                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                                        ButtonReceive:setTitleText("已领取")                                     
                                        sender:setEnabled(false)                           
                                    end
                                end)

                            elseif status==2 then--2已领奖
                                sender:setEnabled(false)

                            end
                            
                            self:updataTipPic()

                        end
                    end
                    PanelDown:setPosition(cc.p(2,ScrovSize.height-(100+6)*i)) 
                    self.ScrollViewDown:addChild(PanelDown)
                    ButtonReceive:addTouchEventListener(midBtnCallback)                                                      
                end
            end

            scrolList1()
            local chongzhiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "chongzhiBtn")
            chongzhiBtn:setVisible(true)
            local function downBtnCallback(sender,event)            
                if sender==chongzhiBtn and event ==cc.EventCode.ENDED then


                    local scene = require("scene.shop.RechargeScene"):create()
                    cc.Director:getInstance():pushScene(scene)

                end
                
                self:updataTipPic()
            end                 
            chongzhiBtn:addTouchEventListener(downBtnCallback)
        end)
end
function ServerUI:atkList()
    cclog("atkList")
    Http:req("openStart",{actId =5},
        function(res)
            local chongzhiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "chongzhiBtn")
            chongzhiBtn:setVisible(false)
            local btnlist = ccui.Helper:seekWidgetByName(self.rootNode, "btnlist")  
            local goalBtn = ccui.Helper:seekWidgetByName(self.rootNode, "goalBtn")
            goalBtn:setVisible(false)
            btnlist:setVisible(false)
            goalBtn:loadTextures("qieye02.png","qieye01.png","qieye01.png") 
            btnlist:loadTextures("qieye01.png","qieye01.png","qieye01.png")

            local itemList = res.data["detail"]
            local actSize = table.getn(itemList)
            
            local actId=tostring(res.data["act"]["actId"])
          
            local openActList=openConfig[actId]
            local actName=openActList["name"]
            local actdesc=openActList["desc"]
           
            local actStartTm=res.data["act"]["startTm"]
            local acTmFrom=tostring(os.date("%c",actStartTm))
            local year="20"..string.sub(acTmFrom,7,8)
            
            local month=string.sub(acTmFrom,1,2)
            local day=string.sub(acTmFrom,4,5)
            local time=string.sub(acTmFrom,10,string.len(acTmFrom))

            local actEndTm=res.data["act"]["endTm"]
            local acTmEnd=tostring(os.date("%c",actEndTm))
            local year2="20"..string.sub(acTmEnd,7,8)               
            local month2=string.sub(acTmEnd,1,2)
            local day2=string.sub(acTmEnd,4,5)
            local time2=string.sub(acTmEnd,10,string.len(acTmEnd))
            local num=tostring(res.data["act"]["num"])
           
            --活动名称
            local labelOne = ccui.Helper:seekWidgetByName(self.rootNode,"labelOne")
            local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
            LabelOne2:setString(actName)
            --活动时间
            local LabelTwo = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo")                 
            local LabelTwo2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo2")
            --时间先不用管他
            LabelTwo2:setString(year.."年"..month.."月"..day.."日"..time.."至"..year2.."年"..month2.."月"..day2.."日"..time2)             
            --活动规则
            local labelThree = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree")                 
            local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")   
            labelThree2:setString(actdesc)             
            --当前已充值
            local wardLabel = ccui.Helper:seekWidgetByName(self.rootNode,"wardLabel")                
            local chargedLabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargedLabel") 
            chargedLabel:setVisible(false)                
            local chargPlabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargPlabel") 
            chargPlabel:setVisible(false) 
            --下方scrollView
            self.ScrollViewDown:removeAllChildren()
            self.ScrollViewDown:setBounceEnabled(true)    
            local ScrovSize = self.ScrollViewDown:getInnerContainerSize()
            
            self.ScrollViewDown:setScale(1.0)
          
            ScrovSize.height =(100+10)*actSize
            self.ScrollViewDown:setInnerContainerSize(ScrovSize)

            local function scrolList2()
                cclog("scroll~2")
                for i=1,actSize do

                    local actRewardId=tostring(res.data["detail"][i]["actRewardId"])        
                    local status=res.data["detail"][i]["rec"]
                    local PlayN=res.data["detail"][i]["name"]              

                    local rewardItemlist= rewardConfig[actRewardId]  

                    local desc1=tostring(rewardItemlist["desc"]) --描述     

                    local PanelDown =ccs.GUIReader:getInstance():widgetFromBinaryFile("downItem.csb")                        
                    local ImageDownItem = ccui.Helper:seekWidgetByName(PanelDown, "ImageDownItem") 
                    local LabelTotal=ccui.Helper:seekWidgetByName(PanelDown,"LabelTotal")
                    LabelTotal:setString(desc1)
                    local LabelDes=ccui.Helper:seekWidgetByName(PanelDown,"LabelDes")
                    LabelDes:setVisible(false)
                    local imageNum=ccui.Helper:seekWidgetByName(PanelDown,"imageNum") 
                    imageNum:setString(tostring(i))
                    local ButtonReceive=ccui.Helper:seekWidgetByName(PanelDown,"ButtonReceive")
                    ButtonReceive:setVisible(false)

                    PanelDown:setPosition(cc.p(2,ScrovSize.height-(100+6)*i)) 
                    self.ScrollViewDown:addChild(PanelDown)

                    --发现冒号的个数 然后判断当前有几个奖励物品
                    local itmId=tostring(rewardItemlist["item"])
                    --cclog("itmId~~~~"..itmId)
                    self.nimei=0
                    self.comma=0
                    for q=1,string.len(itmId) do
                        local m=string.sub(itmId,q,q)
                        
                        if m == "," then
                            local item=string.sub(itmId,self.comma+1,q-1)
                            self.comma=q
                            
                            self.nimei=self.nimei+1    
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    cclog("goodsId~~~~~"..goodsId)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                    cclog("goodsNum~~~~"..goodsNum)                                           
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId] 
                                     
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")                                     
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        elseif q==string.len(itmId) then                    
                            local item=string.sub(itmId,self.comma+1,string.len(itmId))                          
                            
                            self.nimei=self.nimei+1   
                                                   
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                     
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId]
                                    --print("goodID@@@@@@@@@@@@~~~"..goodsId)  
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        end
                    end
                    -- cclog("nimei的有这么多的~~"..self.nimei)

                    local LabelPre=ccui.Helper:seekWidgetByName(PanelDown,"LabelPre") 
                    LabelPre:setString("当前") 
                    local PlayName=ccui.Helper:seekWidgetByName(PanelDown,"PlayName") 
                    if PlayN~=nil then
                        PlayName:setString(PlayN)
                    else
                        PlayName:setString("无")
                    end

                end
            end
            scrolList2()


        end)

end
function ServerUI:contest()
    cclog("contest")
    local btnlist = ccui.Helper:seekWidgetByName(self.rootNode, "btnlist")
    local goalBtn = ccui.Helper:seekWidgetByName(self.rootNode, "goalBtn")
    goalBtn:setVisible(true)
    btnlist:setVisible(true)
    goalBtn:loadTextures("qieye02.png","qieye01.png","qieye01.png") 
    btnlist:loadTextures("qieye01.png","qieye01.png","qieye01.png")
    local chongzhiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "chongzhiBtn")
    chongzhiBtn:setVisible(false)
    local function scrolList1()
        Http:req("openStart",{actId =6},
            function(res)
                cclog("scroll~1")
                local itemList = res.data["detail"]
                local actSize = table.getn(itemList)
                
                local actId=tostring(res.data["act"]["actId"])
                
                local openActList=openConfig[actId]
                local actName=openActList["name"]
                local actDes=openActList["desc"]
                
                local actStartTm=res.data["act"]["startTm"]
                local acTmFrom=tostring(os.date("%c",actStartTm))
                local year="20"..string.sub(acTmFrom,7,8)
                
                local month=string.sub(acTmFrom,1,2)
                local day=string.sub(acTmFrom,4,5)
                local time=string.sub(acTmFrom,10,string.len(acTmFrom))


                local actEndTm=res.data["act"]["endTm"]
                local acTmEnd=tostring(os.date("%c",actEndTm))
                local year2="20"..string.sub(acTmEnd,7,8)               
                local month2=string.sub(acTmEnd,1,2)
                local day2=string.sub(acTmEnd,4,5)
                local time2=string.sub(acTmEnd,10,string.len(acTmEnd))
                local num=tostring(res.data["act"]["num"])
               
                --活动名称
                local labelOne = ccui.Helper:seekWidgetByName(self.rootNode,"labelOne")
                local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
                LabelOne2:setString(actName)
                --活动时间
                local LabelTwo = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo")                 
                local LabelTwo2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo2")
                --时间先不用管他？？？？
                LabelTwo2:setString(year.."年"..month.."月"..day.."日"..time.."至"..year2.."年"..month2.."月"..day2.."日"..time2)              
                --活动规则
                local labelThree = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree")                 
                local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")   
                labelThree2:setString(actDes)             
                --当前已充值
                local wardLabel = ccui.Helper:seekWidgetByName(self.rootNode,"wardLabel")                
                local chargedLabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargedLabel")   
                chargedLabel:setVisible(false)              
                local chargPlabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargPlabel") 
                chargPlabel:setVisible(false)
                chargPlabel:setString(num)

                --下方scrollView
                self.ScrollViewDown:removeAllChildren()
                self.ScrollViewDown:setBounceEnabled(true)    
                local ScrovSize = self.ScrollViewDown:getInnerContainerSize()
                
                self.ScrollViewDown:setScale(1.0)
                ScrovSize.height =(100+10)*actSize
                self.ScrollViewDown:setInnerContainerSize(ScrovSize)

                for i=1,actSize do
                    local actRewardId=tostring(res.data["detail"][i]["actRewardId"])        
                    local status=res.data["detail"][i]["rec"]                                 
                    local rewardItemlist= rewardConfig[actRewardId]  
                    local desc1=tostring(rewardItemlist["desc"]) --描述     
                    local PanelDown =ccs.GUIReader:getInstance():widgetFromBinaryFile("downItem.csb")                        
                    local ImageDownItem = ccui.Helper:seekWidgetByName(PanelDown, "ImageDownItem") 
                    local LabelTotal=ccui.Helper:seekWidgetByName(PanelDown,"LabelTotal")
                    LabelTotal:setString(desc1)
                    local LabelDes=ccui.Helper:seekWidgetByName(PanelDown,"LabelDes")
                    LabelDes:setVisible(false)
                    local LabelPre=ccui.Helper:seekWidgetByName(PanelDown,"LabelPre")
                    LabelPre:setVisible(false)
                    local PlayName=ccui.Helper:seekWidgetByName(PanelDown,"PlayName")
                    PlayName:setVisible(false)
                    local imageNum=ccui.Helper:seekWidgetByName(PanelDown,"imageNum") 
                    imageNum:setString(tostring(i))
                    local ButtonReceive=ccui.Helper:seekWidgetByName(PanelDown,"ButtonReceive")

                    PanelDown:setPosition(cc.p(2,ScrovSize.height-(100+6)*i)) 
                    self.ScrollViewDown:addChild(PanelDown)

                    --发现冒号的个数 然后判断当前有几个奖励物品
                    local itmId=tostring(rewardItemlist["item"])
                    --cclog("itmId~~~~"..itmId)
                    self.nimei=0
                    self.comma=0
                    for q=1,string.len(itmId) do
                        local m=string.sub(itmId,q,q)
                        --print("m~~"..m)
                        if m == "," then
                            local item=string.sub(itmId,self.comma+1,q-1)
                            self.comma=q
                          
                            self.nimei=self.nimei+1    
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                   
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                                                              
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId] 
                                    --print("goodID@@@@@@@@@@@@~~~"..goodsId)  
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")                                     
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                   
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        elseif q==string.len(itmId) then                    
                            local item=string.sub(itmId,self.comma+1,string.len(itmId))                          
                            
                            self.nimei=self.nimei+1   
                                                   
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                     
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId]
                                     
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        end
                    end
                    -- cclog("nimei的有这么多的~~"..self.nimei)

                    --领取button状态 设置button 上面的字体
                    if status==0 then--0未完成  
                        ButtonReceive:setTitleText("未达成")
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setEnabled(false)
                        --PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                    elseif status==1 then--1可领取
                        ButtonReceive:setTitleText("领取")
                        ButtonReceive:loadTextures("anniu.png","anniu00.png","anniugrey.png")
                        ButtonReceive:setEnabled(true)
                    elseif status==2 then--2已领奖
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setTitleText("已领取")
                    end

                    local function midBtnCallback(sender,event)
                        if sender == ButtonReceive and event == cc.EventCode.ENDED then
                            --领取button状态 设置button 上面的字体
                            if status==0 then--0未完成  
                                PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                            elseif status==1 then--1可领取
                                Http:req("openOver",{actRewardId=actRewardId},function(res)
                                    if res.data["result"] == 0 then
                                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                                        ButtonReceive:setTitleText("已领取")                                     
                                        sender:setEnabled(false)                            
                                    end
                                end)

                            elseif status==2 then--2已领奖
                                sender:setEnabled(false)

                            end
                            
                            self:updataTipPic()

                        end
                    end

                    ButtonReceive:addTouchEventListener(midBtnCallback)
                end
            end)
    end
    scrolList1()
    local function scrolList2()        
        cclog("scroll~2")
        Http:req("openStart",{actId =7},
            function(res)
                local itemList = res.data["detail"]
                local actSize = table.getn(itemList)
               

                local actId=tostring(res.data["act"]["actId"])
                
                local openActList=openConfig[actId]
                local actName=openActList["name"]
                local actDes=openActList["desc"]
                --活动名称
                local labelOne = ccui.Helper:seekWidgetByName(self.rootNode,"labelOne")
                local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
                LabelOne2:setString(actName)
                local labelThree = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree")                 
                local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")   
                labelThree2:setString(actDes)
                --下方scrollView
                self.ScrollViewDown:removeAllChildren()
                self.ScrollViewDown:setBounceEnabled(true)    
                local ScrovSize = self.ScrollViewDown:getInnerContainerSize()
                
                self.ScrollViewDown:setScale(1.0)
                ScrovSize.height =(100+10)*actSize
                self.ScrollViewDown:setInnerContainerSize(ScrovSize)
                for i=1,actSize do                                     
                    local actRewardId=tostring(res.data["detail"][i]["actRewardId"])        
                    local status=res.data["detail"][i]["rec"]
                    local PlayN=res.data["detail"][i]["name"]              
                   
                    local rewardItemlist= rewardConfig[actRewardId]  
                    local desc1=tostring(rewardItemlist["desc"]) --描述     
                    local PanelDown =ccs.GUIReader:getInstance():widgetFromBinaryFile("downItem.csb")                        
                    local ImageDownItem = ccui.Helper:seekWidgetByName(PanelDown, "ImageDownItem") 
                    local LabelTotal=ccui.Helper:seekWidgetByName(PanelDown,"LabelTotal")
                    LabelTotal:setString(desc1)
                    local LabelDes=ccui.Helper:seekWidgetByName(PanelDown,"LabelDes")
                    LabelDes:setVisible(false)
                    local imageNum=ccui.Helper:seekWidgetByName(PanelDown,"imageNum") 
                    imageNum:setString(tostring(i))
                    local ButtonReceive=ccui.Helper:seekWidgetByName(PanelDown,"ButtonReceive")
                    ButtonReceive:setVisible(false)

                    PanelDown:setPosition(cc.p(2,ScrovSize.height-(100+6)*i)) 
                    self.ScrollViewDown:addChild(PanelDown)

                    --发现冒号的个数 然后判断当前有几个奖励物品
                    local itmId=tostring(rewardItemlist["item"])
                    --cclog("itmId~~~~"..itmId)
                    self.nimei=0
                    self.comma=0
                    for q=1,string.len(itmId) do
                        local m=string.sub(itmId,q,q)
                        --print("m~~"..m)
                        if m == "," then
                            local item=string.sub(itmId,self.comma+1,q-1)
                            self.comma=q
                            
                            self.nimei=self.nimei+1    
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                                                               
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId] 
                                      
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")                                     
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                   
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        elseif q==string.len(itmId) then                    
                            local item=string.sub(itmId,self.comma+1,string.len(itmId))                          
                             
                            self.nimei=self.nimei+1   
                                                   
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                    
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId]
                                    --print("goodID@@@@@@@@@@@@~~~"..goodsId)  
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        end
                    end
                    -- cclog("nimei的有这么多的~~"..self.nimei)

                    local LabelPre=ccui.Helper:seekWidgetByName(PanelDown,"LabelPre") 
                    LabelPre:setString("当前") 
                    local PlayName=ccui.Helper:seekWidgetByName(PanelDown,"PlayName") 

                    if PlayN ~=nil then
                        
                        PlayName:setString(PlayN)
                    else
                        
                        PlayName:setString("无")
                    end            
                end
            end)
    end

    local chongzhiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "chongzhiBtn")
    local function downBtnCallback(sender,event)
        if sender==goalBtn and event ==cc.EventCode.ENDED then
           
            --goalBtn:setTitleText("nimei")
            goalBtn:loadTextures("qieye02.png","qieye01.png","qieye01.png") 
            btnlist:loadTextures("qieye01.png","qieye01.png","qieye01.png")
            --self.goalBtn:setString("nihao")
            scrolList1()
        elseif sender == btnlist and event == cc.EventCode.ENDED then
            btnlist:loadTextures("qieye02.png","qieye01.png","qieye01.png")
            goalBtn:loadTextures("qieye01.png","qieye01.png","qieye01.png") 

            scrolList2()
        elseif sender==chongzhiBtn and event ==cc.EventCode.ENDED then
            --chongzhiBtn= self:createWithText("pause")



            local scene = require("scene.shop.RechargeScene"):create()
            cc.Director:getInstance():pushScene(scene)
        end
    end
    goalBtn:addTouchEventListener(downBtnCallback) 
    btnlist:addTouchEventListener(downBtnCallback)        
    chongzhiBtn:addTouchEventListener(downBtnCallback)


end
function ServerUI:classm()
    local btnlist = ccui.Helper:seekWidgetByName(self.rootNode, "btnlist")
    local goalBtn = ccui.Helper:seekWidgetByName(self.rootNode, "goalBtn")
    goalBtn:setVisible(true)
    btnlist:setVisible(true)
    goalBtn:loadTextures("qieye02.png","qieye01.png","qieye01.png") 
    btnlist:loadTextures("qieye01.png","qieye01.png","qieye01.png")
    local chongzhiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "chongzhiBtn")
    chongzhiBtn:setVisible(false)
    local function scrolList1()
        Http:req("openStart",{actId =8},
            function(res)
                cclog("scroll~1")
                local itemList = res.data["detail"]
                local actSize = table.getn(itemList)
               
                local actId=tostring(res.data["act"]["actId"])
                
                local openActList=openConfig[actId]
                local actName=openActList["name"]
                local actDes=openActList["desc"]
                --cclog("描述~~~~~"..desc1)
                local actStartTm=res.data["act"]["startTm"]
                local acTmFrom=tostring(os.date("%c",actStartTm))
                local year="20"..string.sub(acTmFrom,7,8)
               
                local month=string.sub(acTmFrom,1,2)
                local day=string.sub(acTmFrom,4,5)
                local time=string.sub(acTmFrom,10,string.len(acTmFrom))


                local actEndTm=res.data["act"]["endTm"]
                local acTmEnd=tostring(os.date("%c",actEndTm))
                local year2="20"..string.sub(acTmEnd,7,8)               
                local month2=string.sub(acTmEnd,1,2)
                local day2=string.sub(acTmEnd,4,5)
                local time2=string.sub(acTmEnd,10,string.len(acTmEnd))
                local num=tostring(res.data["act"]["num"])
               
                --活动名称
                local labelOne = ccui.Helper:seekWidgetByName(self.rootNode,"labelOne")
                local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
                LabelOne2:setString(actName)
                --活动时间
                local LabelTwo = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo")                 
                local LabelTwo2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo2")
                --时间先不用管他？？？？
                LabelTwo2:setString(year.."年"..month.."月"..day.."日"..time.."至"..year2.."年"..month2.."月"..day2.."日"..time2)              
                --活动规则
                local labelThree = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree")                 
                local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")   
                labelThree2:setString(actDes)             
                --当前已充值
                local wardLabel = ccui.Helper:seekWidgetByName(self.rootNode,"wardLabel")                
                local chargedLabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargedLabel") 
                chargedLabel:setVisible(false)                
                local chargPlabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargPlabel") 
                chargPlabel:setVisible(false)
                chargPlabel:setString(num)

                --下方scrollView
                self.ScrollViewDown:removeAllChildren()
                self.ScrollViewDown:setBounceEnabled(true)    
                local ScrovSize = self.ScrollViewDown:getInnerContainerSize()
                
                self.ScrollViewDown:setScale(1.0)
                ScrovSize.height =(100+10)*actSize
                self.ScrollViewDown:setInnerContainerSize(ScrovSize)

                for i=1,actSize do
                    local actRewardId=tostring(res.data["detail"][i]["actRewardId"])        
                    local status=res.data["detail"][i]["rec"]                                 
                    local rewardItemlist= rewardConfig[actRewardId]  
                    local desc1=tostring(rewardItemlist["desc"]) --描述     
                    local PanelDown =ccs.GUIReader:getInstance():widgetFromBinaryFile("downItem.csb")                        
                    local ImageDownItem = ccui.Helper:seekWidgetByName(PanelDown, "ImageDownItem") 
                    local LabelTotal=ccui.Helper:seekWidgetByName(PanelDown,"LabelTotal")
                    LabelTotal:setString(desc1)
                    local LabelDes=ccui.Helper:seekWidgetByName(PanelDown,"LabelDes")
                    LabelDes:setVisible(false)
                    local LabelPre=ccui.Helper:seekWidgetByName(PanelDown,"LabelPre")
                    LabelPre:setVisible(false)
                    local PlayName=ccui.Helper:seekWidgetByName(PanelDown,"PlayName")
                    PlayName:setVisible(false)
                    local imageNum=ccui.Helper:seekWidgetByName(PanelDown,"imageNum") 
                    imageNum:setString(tostring(i))
                    local ButtonReceive=ccui.Helper:seekWidgetByName(PanelDown,"ButtonReceive")

                    PanelDown:setPosition(cc.p(2,ScrovSize.height-(100+6)*i)) 
                    self.ScrollViewDown:addChild(PanelDown)

                    --发现冒号的个数 然后判断当前有几个奖励物品
                    local itmId=tostring(rewardItemlist["item"])
                    --cclog("itmId~~~~"..itmId)
                    self.nimei=0
                    self.comma=0
                    for q=1,string.len(itmId) do
                        local m=string.sub(itmId,q,q)
                        --print("m~~"..m)
                        if m == "," then
                            local item=string.sub(itmId,self.comma+1,q-1)
                            self.comma=q
                            
                            self.nimei=self.nimei+1    
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    cclog("goodsId~~~~~"..goodsId)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                    cclog("goodsNum~~~~"..goodsNum)                                           
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId] 
                                     
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")                                     
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        elseif q==string.len(itmId) then                    
                            local item=string.sub(itmId,self.comma+1,string.len(itmId))                          
                             
                            self.nimei=self.nimei+1                                                    
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                    
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId]
                                   
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        end
                    end
                    -- cclog("nimei的有这么多的~~"..self.nimei)

                    --领取button状态 设置button 上面的字体
                    if status==0 then--0未完成  
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setTitleText("未达成")
                        --PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                    elseif status==1 then--1可领取
                        ButtonReceive:setTitleText("领取")
                        ButtonReceive:loadTextures("anniu.png","anniu00.png","anniugrey.png")
                        ButtonReceive:setEnabled(true)
                    elseif status==2 then--2已领奖
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setTitleText("已领取")
                    end

                    local function midBtnCallback(sender,event)
                        if sender == ButtonReceive and event == cc.EventCode.ENDED then
                            --领取button状态 设置button 上面的字体
                            if status==0 then--0未完成  
                                PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                            elseif status==1 then--1可领取
                                Http:req("openOver",{actRewardId=actRewardId},function(res)
                                    if res.data["result"] == 0 then
                                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                                        ButtonReceive:setTitleText("已领取")                                     
                                        sender:setEnabled(false)                           
                                    end
                                end)

                            elseif status==2 then--2已领奖
                                sender:setEnabled(false)

                            end
                            self:updataTipPic()

                        end
                    end

                    ButtonReceive:addTouchEventListener(midBtnCallback)
                end
            end)
    end
    scrolList1()
    local function scrolList2()        
        cclog("scroll~2")
        Http:req("openStart",{actId =9},
            function(res)
                local itemList = res.data["detail"]
                local actSize = table.getn(itemList)
               
                local actId=tostring(res.data["act"]["actId"])
               
                local openActList=openConfig[actId]
                local actName=openActList["name"]
                local actDes=openActList["desc"]
                --活动名称
                local labelOne = ccui.Helper:seekWidgetByName(self.rootNode,"labelOne")
                local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
                LabelOne2:setString(actName)
                local labelThree = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree")                 
                local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")   
                labelThree2:setString(actDes)
                --下方scrollView
                self.ScrollViewDown:removeAllChildren()
                self.ScrollViewDown:setBounceEnabled(true)    
                local ScrovSize = self.ScrollViewDown:getInnerContainerSize()
                
                self.ScrollViewDown:setScale(1.0)
                ScrovSize.height =(100+10)*actSize
                self.ScrollViewDown:setInnerContainerSize(ScrovSize)
                for i=1,actSize do                                     
                    local actRewardId=tostring(res.data["detail"][i]["actRewardId"])        
                    local status=res.data["detail"][i]["rec"]
                    local PlayN=res.data["detail"][i]["name"]              
                    
                    local rewardItemlist= rewardConfig[actRewardId]  
                    local desc1=tostring(rewardItemlist["desc"]) --描述     
                    local PanelDown =ccs.GUIReader:getInstance():widgetFromBinaryFile("downItem.csb")                        
                    local ImageDownItem = ccui.Helper:seekWidgetByName(PanelDown, "ImageDownItem") 
                    local LabelTotal=ccui.Helper:seekWidgetByName(PanelDown,"LabelTotal")
                    LabelTotal:setString(desc1)
                    local LabelDes=ccui.Helper:seekWidgetByName(PanelDown,"LabelDes")
                    LabelDes:setVisible(false)
                    local imageNum=ccui.Helper:seekWidgetByName(PanelDown,"imageNum") 
                    imageNum:setString(tostring(i))
                    local ButtonReceive=ccui.Helper:seekWidgetByName(PanelDown,"ButtonReceive")
                    ButtonReceive:setVisible(false)

                    PanelDown:setPosition(cc.p(2,ScrovSize.height-(100+6)*i)) 
                    self.ScrollViewDown:addChild(PanelDown)

                    --发现冒号的个数 然后判断当前有几个奖励物品
                    local itmId=tostring(rewardItemlist["item"])
                    --cclog("itmId~~~~"..itmId)
                    self.nimei=0
                    self.comma=0
                    for q=1,string.len(itmId) do
                        local m=string.sub(itmId,q,q)
                        --print("m~~"..m)
                        if m == "," then
                            local item=string.sub(itmId,self.comma+1,q-1)
                            self.comma=q
                           
                            self.nimei=self.nimei+1    
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                                                            
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId] 
                                      
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")                                     
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                   
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        elseif q==string.len(itmId) then                    
                            local item=string.sub(itmId,self.comma+1,string.len(itmId))                          
                            
                            self.nimei=self.nimei+1   
                                                   
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                  
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId]
                                    --print("goodID@@@@@@@@@@@@~~~"..goodsId)  
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        end
                    end
                    -- cclog("nimei的有这么多的~~"..self.nimei)

                    local LabelPre=ccui.Helper:seekWidgetByName(PanelDown,"LabelPre") 
                    LabelPre:setString("当前") 
                    local PlayName=ccui.Helper:seekWidgetByName(PanelDown,"PlayName") 

                    if PlayN ~=nil then
                        
                        PlayName:setString(PlayN)
                    else
                      
                        PlayName:setString("无")
                    end            
                end
            end)
    end


    local chongzhiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "chongzhiBtn")
    chongzhiBtn:setVisible(false)
    local function downBtnCallback(sender,event)
        if sender==goalBtn and event ==cc.EventCode.ENDED then
            
            --goalBtn:setTitleText("nimei")
            goalBtn:loadTextures("qieye02.png","qieye01.png","qieye01.png") 
            btnlist:loadTextures("qieye01.png","qieye01.png","qieye01.png")
            --self.goalBtn:setString("nihao")
            scrolList1()
        elseif sender == btnlist and event == cc.EventCode.ENDED then
            btnlist:loadTextures("qieye02.png","qieye01.png","qieye01.png")
            goalBtn:loadTextures("qieye01.png","qieye01.png","qieye01.png") 
            scrolList2()           
        end
    end
    goalBtn:addTouchEventListener(downBtnCallback) 
    btnlist:addTouchEventListener(downBtnCallback)        

end
function ServerUI:public()
    cclog("public")
    local btnlist = ccui.Helper:seekWidgetByName(self.rootNode, "btnlist")  
    local goalBtn = ccui.Helper:seekWidgetByName(self.rootNode, "goalBtn")
    goalBtn:setVisible(false)
    btnlist:setVisible(false)
    Http:req("openStart",{actId =10},
        function(res)
            local itemList = res.data["detail"]
            local actSize = table.getn(itemList)
            
            local actId=tostring(res.data["act"]["actId"])
            
            local openActList=openConfig[actId]
            local actName=openActList["name"]
            local actdesc=openActList["desc"]
            --cclog("描述~~~~~"..desc1)
            local actStartTm=res.data["act"]["startTm"]
            local acTmFrom=tostring(os.date("%c",actStartTm))
            local year="20"..string.sub(acTmFrom,7,8)
            cclog(year)
            local month=string.sub(acTmFrom,1,2)
            local day=string.sub(acTmFrom,4,5)
            local time=string.sub(acTmFrom,10,string.len(acTmFrom))

            local actEndTm=res.data["act"]["endTm"]
            local acTmEnd=tostring(os.date("%c",actEndTm))
            local year2="20"..string.sub(acTmEnd,7,8)               
            local month2=string.sub(acTmEnd,1,2)
            local day2=string.sub(acTmEnd,4,5)
            local time2=string.sub(acTmEnd,10,string.len(acTmEnd))
            local num=tostring(res.data["act"]["num"])
          
            --活动名称
            local labelOne = ccui.Helper:seekWidgetByName(self.rootNode,"labelOne")
            local LabelOne2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelOne2")
            LabelOne2:setString(actName)
            --活动时间
            local LabelTwo = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo")                 
            local LabelTwo2 = ccui.Helper:seekWidgetByName(self.rootNode,"LabelTwo2")
            --时间先不用管他
            LabelTwo2:setString(year.."年"..month.."月"..day.."日"..time.."至"..year2.."年"..month2.."月"..day2.."日"..time2)              
            --活动规则
            local labelThree = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree")                 
            local labelThree2 = ccui.Helper:seekWidgetByName(self.rootNode,"labelThree2")   
            labelThree2:setString(actdesc)             
            --当前已充值
            local wardLabel = ccui.Helper:seekWidgetByName(self.rootNode,"wardLabel")                
            local chargedLabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargedLabel")  
            chargedLabel:setVisible(false)          
            local chargPlabel = ccui.Helper:seekWidgetByName(self.rootNode,"chargPlabel") 
            chargPlabel:setVisible(false)   
            --下方scrollView
            self.ScrollViewDown:removeAllChildren()
            self.ScrollViewDown:setBounceEnabled(true)    
            local ScrovSize = self.ScrollViewDown:getInnerContainerSize()
            
            self.ScrollViewDown:setScale(1.0)
           
            ScrovSize.height =(100+10)*actSize
            self.ScrollViewDown:setInnerContainerSize(ScrovSize)
            local function scrolList1()
                cclog("scroll~1")
                for i=1,actSize do
                    local actRewardId=tostring(res.data["detail"][i]["actRewardId"])        
                    local status=res.data["detail"][i]["rec"]                                  
                    local rewardItemlist= rewardConfig[actRewardId]  
                    local desc1=tostring(rewardItemlist["desc"]) --描述     
                    local PanelDown =ccs.GUIReader:getInstance():widgetFromBinaryFile("downItem.csb")                        
                    local ImageDownItem = ccui.Helper:seekWidgetByName(PanelDown, "ImageDownItem") 
                    local LabelTotal=ccui.Helper:seekWidgetByName(PanelDown,"LabelTotal")
                    LabelTotal:setString(desc1)
                    local LabelDes=ccui.Helper:seekWidgetByName(PanelDown,"LabelDes")
                    LabelDes:setVisible(false)
                    local LabelPre=ccui.Helper:seekWidgetByName(PanelDown,"LabelPre")
                    LabelPre:setVisible(false)
                    local PlayName=ccui.Helper:seekWidgetByName(PanelDown,"PlayName")
                    PlayName:setVisible(false)
                    local imageNum=ccui.Helper:seekWidgetByName(PanelDown,"imageNum") 
                    imageNum:setString(tostring(i))
                    local ButtonReceive=ccui.Helper:seekWidgetByName(PanelDown,"ButtonReceive")
                    PanelDown:setPosition(cc.p(2,ScrovSize.height-(100+6)*i)) 
                    self.ScrollViewDown:addChild(PanelDown)
                    --发现冒号的个数 然后判断当前有几个奖励物品
                    local itmId=tostring(rewardItemlist["item"])
                    --cclog("itmId~~~~"..itmId)
                    self.nimei=0
                    self.comma=0
                    for q=1,string.len(itmId) do
                        local m=string.sub(itmId,q,q)
                        --print("m~~"..m)
                        if m == "," then
                            local item=string.sub(itmId,self.comma+1,q-1)
                            self.comma=q
                           
                            self.nimei=self.nimei+1    
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                                                               
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId] 
                                    --print("goodID@@@@@@@@@@@@~~~"..goodsId)  
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")                                     
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        elseif q==string.len(itmId) then                    
                            local item=string.sub(itmId,self.comma+1,string.len(itmId))                          
                            
                            self.nimei=self.nimei+1   
                                                   
                            for v=1,string.len(item) do
                                local n=string.sub(item,v,v) 
                                if n==":" then
                                    local goodsId=string.sub(item,1,v-1)
                                    local goodsNum=string.sub(item,v+1,string.len(item))
                                   
                                    local ImageItem =ccs.GUIReader:getInstance():widgetFromBinaryFile("ImageItem.csb")    
                                    local ImageGoods=ccui.Helper:seekWidgetByName(ImageItem,"ImageGoods")
                                    local ImageKuang=ccui.Helper:seekWidgetByName(ImageItem,"ImageKuang")                                                                                             
                                    local dataitem=shopConfig[goodsId]
                                    --print("goodID@@@@@@@@@@@@~~~"..goodsId)  
                                    ImageGoods:loadTexture(dataitem["icon"]..".png")
                                    ImageKuang:loadTexture("normal_aochao_0"..dataitem["quality"]..".png")
                                    local labelNum=ccui.Helper:seekWidgetByName(ImageItem,"itemNum")
                                    labelNum:setString(goodsNum)
                                    ImageGoods:setPosition(cc.p(45+(70+10)*(self.nimei),40))
                                    ImageDownItem:addChild(ImageItem)
                                end
                            end
                        end
                    end
                    -- cclog("nimei的有这么多的~~"..self.nimei)

                    --领取button状态 设置button 上面的字体
                    if status==0 then--0未完成

                        ButtonReceive:setTitleText("未达成")
                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setEnabled(false)
                        --PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                    elseif status==1 then--1可领取
                        ButtonReceive:setTitleText("领取")
                        ButtonReceive:loadTextures("anniu.png","anniu00.png","anniugrey.png")
                        ButtonReceive:setEnabled(true)
                    elseif status==2 then--2已领奖
                        --ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")

                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                        ButtonReceive:setTitleText("已领取")                                     
                        ButtonReceive:setEnabled(false)
                    end

                    local function midBtnCallback(sender,event)

                        if sender == ButtonReceive and event == cc.EventCode.ENDED then
                            --领取button状态 设置button 上面的字体
                            if status==0 then--0未完成  

                                PopDialog:getInstance():popText("您未完成任务,不能领取",cc.c3b(0,255,0))
                            elseif status==1 then--1可领取
                                Http:req("openOver",{actRewardId=actRewardId},function(res)
                                    if res.data["result"] == 0 then
                                        ButtonReceive:loadTextures("anniugrey.png","anniugrey.png","anniugrey.png")
                                        ButtonReceive:setTitleText("已领取")                                     
                                        sender:setEnabled(false)                           
                                    end
                                end)

                            elseif status==2 then--2已领奖
                                sender:setEnabled(false)

                            end

                        end
                    end

                    ButtonReceive:addTouchEventListener(midBtnCallback)
                end
            end

            scrolList1()
            local chongzhiBtn = ccui.Helper:seekWidgetByName(self.rootNode, "chongzhiBtn")
            chongzhiBtn:setVisible(false)

        end)
end

function ServerUI:updataTipPic()
    Http:req("GetRedDot",{multType = "512"},function(res)
       local DotNum = table.getn(res.data.dot) 
       
        self.tableDotItem[4]:setVisible(false)
        
        
        if res.data.dot[1].visible == 0 then
            self.tableDotItem[1]:setVisible(false)
        else
            self.tableDotItem[1]:setVisible(true)
        end 
           
        if res.data.dot[2].visible == 0 then
            self.tableDotItem[2]:setVisible(false)
        else
            self.tableDotItem[2]:setVisible(true)
        end
        
        if res.data.dot[3].visible == 0 then
            self.tableDotItem[3]:setVisible(false)
        else
            self.tableDotItem[3]:setVisible(true)
        end
        
        if res.data.dot[4].visible == 0 then
            self.tableDotItem[5]:setVisible(false)
        else
            self.tableDotItem[5]:setVisible(true)
        end
        
        if res.data.dot[5].visible == 0 then
            self.tableDotItem[6]:setVisible(false)
        else
            self.tableDotItem[6]:setVisible(true)
        end
        
        if res.data.dot[6].visible == 0 then
            self.tableDotItem[7]:setVisible(false)
        else
            self.tableDotItem[7]:setVisible(true)
        end
    end)
end

return ServerUI
