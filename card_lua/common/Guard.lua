local GuardLayer = class("GuardLayer", function()
    return cc.Layer:create()
end)

function GuardLayer:create(param)
    local layer = GuardLayer.new()
    layer:initM(param)
    return layer
end

function GuardLayer:initM(param)
    print("[GuardLayer]create guard layer")
    param.direction = param.direction or 1
    self.cb = param.cb
    --self:setScaleX(param.direction)
    self:setAnchorPoint(0,0)
    self:setPosition(0,0)
    self:setContentSize(WIN_SIZE)
    
    local layer = guardCenter:isSwallow()
    
    --[[if param.type == "guard_textdialog" then
       self.textbg = ccui.ImageView:create()
       self.textbg:loadTexture("image/ui/guardCenter/di01.png")
       self.textbg:setAnchorPoint(0,0)
       self.textbg:setPosition(WIN_SIZE.width/2,WIN_SIZE.height/2)
       self:addChild(self.textbg)
       
       
        --对话位置
        local dialogText = ccui.Text:create()
        dialogText:setString(param.textdialog)
        dialogText:setFontSize(18)
        dialogText:setAnchorPoint(0,0)
        dialogText:ignoreContentAdaptWithSize(false)
        dialogText:setColor(cc.c3b(92,33,1))
        dialogText:setTextHorizontalAlignment(0)
        dialogText:setTextVerticalAlignment(1)
        dialogText:setTextAreaSize(cc.size(textbg:getContentSize().width-10,textbg:getContentSize().height-8))
        
        dialogText:setPosition(0, 0)
        dialogText:setSize(cc.size(560, 60))
        dialogText:setTextAreaSize(cc.size(560, 60))
        self.textbg:addChild(dialogText)
    end--]]
    
    if param.type == "guard_dialog" then
        self:addChild(layer)
        
        --对话框
        local textbg = ccui.Button:create()
        textbg:loadTextureNormal("image/ui/guardCenter/guard_text_bg.png")
        textbg:loadTexturePressed("image/ui/guardCenter/guard_text_bg.png")
        textbg:setScale9Enabled(true)
        textbg:setPressedActionEnabled(false)
        textbg:setContentSize(WIN_SIZE.width + 50, 110)
        textbg:setAnchorPoint(0,0)
        textbg:setPosition(-20,-10)
        textbg:addTouchEventListener(function(sender,eventType)
            if eventType == ccui.TouchEventType.ended then
                print("textBg clicked")
                self.cb()
            end
        end)
        self:addChild(textbg)
        local person = ccui.ImageView:create()
        person:setAnchorPoint(0,0)
        
        if param.person == 1 then --魅魔
            person:loadTexture("image/ui/guardCenter/guard_person_2.png")
            person:setPosition(-50,0)
        elseif param.person == 2 then --玩家
            person:loadTexture("image/ui/guardCenter/guard_person_4.png")
            person:setPosition(-50,0)
        elseif param.person == 3 then --魔王
            person:loadTexture("image/ui/guardCenter/guard_person_3.png")
            person:setPosition(720,0)
        elseif param.person == 4 then --神秘人
            person:loadTexture("image/ui/guardCenter/guard_person_2.png")
            person:setScaleX(-1)
            person:setPosition(1100,0)
        end
        self:addChild(person)
        
        --名字所在的框
        local nameBg = ccui.ImageView:create()
        nameBg:loadTexture("image/ui/guardCenter/guard_name_bg.png")
        nameBg:setAnchorPoint(0,0)
        if param.person == 1 then --魅魔
            nameBg:setPosition(365, 80)
        elseif param.person == 2 then --玩家
            nameBg:setPosition(365, 80)
        elseif param.person == 3 then --魔王
            nameBg:setPosition(565, 80)
        elseif param.person == 4 then --神秘人
            nameBg:setPosition(565, 80)
        end
        self:addChild(nameBg)
        
        --名字
        local name = ccui.Text:create()
        --name:setAnchorPoint(0,0)
        name:setPosition(nameBg:getContentSize().width/2,nameBg:getContentSize().height/2)
        if param.person == 1 then --魅魔
            name:setString("魅魔")
        elseif param.person == 2 then --玩家
            name:setString("我")
        elseif param.person == 3 then --魔王
            name:setString("大魔王")
        elseif param.person == 4 then --神秘人
            name:setString("神秘人")
        end
        name:setFontSize(20)
        name:setColor(cc.c3b(255,234,79))
        --name:setScaleX(param.direction)
        nameBg:addChild(name)
        
        --对话位置
        local dialogText = ccui.Text:create()
        dialogText:setString(param.dialog)
        dialogText:setFontSize(18)
        dialogText:setAnchorPoint(0,0)
        dialogText:ignoreContentAdaptWithSize(false)
        dialogText:setColor(cc.c3b(92,33,1))
        dialogText:setTextHorizontalAlignment(0)
        dialogText:setTextVerticalAlignment(1)
        dialogText:setTextAreaSize(cc.size(textbg:getContentSize().width-10,textbg:getContentSize().height-8))
        if param.person == 1 then --魅魔
            dialogText:setPosition(340, 20)
        elseif param.person == 2 then --玩家
            dialogText:setPosition(445, 20)
        elseif param.person == 3 then --魔王
            dialogText:setPosition(290, 20)
        elseif param.person == 4 then --神秘人
            dialogText:setPosition(290, 20)
        end
        dialogText:setSize(cc.size(560, 60))
        dialogText:setTextAreaSize(cc.size(560, 60))
        self:addChild(dialogText)
        
        --箭头
        local arrow = ccui.ImageView:create()
        arrow:loadTexture("image/ui/guardCenter/guard_arrow.png")
        
        local arrow = cc.Sprite:create("image/ui/guardCenter/guard_arrow.png")
        arrow:setGlobalZOrder(20001)
        local scaleAction1 = cc.ScaleBy:create(0.4,0.84)
        local scaleAction2 = scaleAction1:reverse()
        arrow:runAction(cc.RepeatForever:create(cc.Sequence:create(scaleAction1, scaleAction2)))
       -- self:addChild(figure)
        
        if param.person == 1 then --魅魔
            arrow:setPosition(870,40)
        elseif param.person == 2 then --玩家
            arrow:setPosition(870,40)
        elseif param.person == 3 then --魔王
            arrow:setPosition(170,40)
        elseif param.person == 4 then --神秘人
            arrow:setPosition(170,40)
        end
        --arrow:setPosition(870,40)
        self:addChild(arrow)
        
    elseif param.type == "guard_click" then
        local clipNode = cc.ClippingNode:create()
        --clipNode:setPosition(WIN_SIZE.width/2,WIN_SIZE.height/2)
        clipNode:setInverted(true)
        self:addChild(clipNode)
        
        clipNode:addChild(layer)
        --clipNode:addChild(colorLayer)
        
        local target = param.clickTarget
        local clickText = param.textdialog
        
        
        assert(target~=nil,"")
        assert(target:getParent()~=nil,"")
        local worldPos = target:getParent():convertToWorldSpace({x=target:getPositionX(), y=target:getPositionY()})
        
        if clickText ~= nil or clickText == "" then
            
            local textbg = ccui.ImageView:create()
            textbg:loadTexture("image/ui/guardCenter/di01.png")
            textbg:setAnchorPoint(0,0)
            
            --对话位置
            local dialogText = ccui.Text:create()
            dialogText:setString(param.textdialog)
           --dialogText:ignoreContentAdaptWithSize(false)
            --dialogText:setSize(5,0) 
            --dialogText:setDimensions(15,0)
            dialogText:setFontSize(18)
            dialogText:setAnchorPoint(0,0)
            dialogText:ignoreContentAdaptWithSize(false)
            dialogText:setColor(cc.c3b(255,255,255))
            dialogText:setTextHorizontalAlignment(0)
            dialogText:setTextVerticalAlignment(1)
            dialogText:setTextAreaSize(cc.size(textbg:getContentSize().width-10,textbg:getContentSize().height-8))

           -- dialogText:setPosition(260, 25)
            dialogText:setSize(cc.size(300, 60))
           -- dialogText:setTextAreaSize(cc.size(560, 60)) 
            --dialogText:setDimensions(cc.size(15, 0))
            textbg:addChild(dialogText)
            
            
            if worldPos.x < WIN_SIZE.width/2 and worldPos.y < WIN_SIZE.height/2 then
                textbg:setAnchorPoint(1,0)
                dialogText:setScaleX(-1)
                dialogText:setPosition(587, 35)
                textbg:setScaleX(-1)   
                textbg:setPosition(worldPos.x - 85,worldPos.y + 85)
            elseif worldPos.x > WIN_SIZE.width/2 and worldPos.y < WIN_SIZE.height/2 then
                --dialogText:setPosition(worldPos.x - 25,worldPos.y + 25)
                textbg:setAnchorPoint(1,0)
                dialogText:setPosition(265, 35)
                textbg:setPosition(worldPos.x - 25,worldPos.y + 85) 
            elseif worldPos.x < WIN_SIZE.width/2 and worldPos.y > WIN_SIZE.height/2 then
                textbg:setAnchorPoint(1,0)
                dialogText:setScaleX(-1)
                dialogText:setPosition(587, 35)
                textbg:setScaleX(-1)
                textbg:setPosition(worldPos.x +85,worldPos.y-70)
            elseif worldPos.x > WIN_SIZE.width/2 and worldPos.y > WIN_SIZE.height/2 then  
                textbg:setAnchorPoint(0,0)
                dialogText:setPosition(265,35)
                textbg:setPosition(worldPos.x -700,worldPos.y - 25)
            end
            
            --self.textbg:setPosition(WIN_SIZE.width/2,WIN_SIZE.height/2)
            self:addChild(textbg)


            --对话位置
            --[[local dialogText = ccui.Text:create()
            dialogText:setString(param.textdialog)
            dialogText:setFontSize(18)
            dialogText:setAnchorPoint(0,0)
            dialogText:ignoreContentAdaptWithSize(false)
            dialogText:setColor(cc.c3b(92,33,1))
            dialogText:setTextHorizontalAlignment(0)
            dialogText:setTextVerticalAlignment(1)
            dialogText:setTextAreaSize(cc.size(textbg:getContentSize().width-10,textbg:getContentSize().height-8))

            dialogText:setPosition(0, 0)
            dialogText:setSize(cc.size(560, 60))
            dialogText:setTextAreaSize(cc.size(560, 60))
            textbg:addChild(dialogText)--]]
            
        end
        
        --print("Target_World_Position:"..worldPos.x..", "..worldPos.y)
        
        local function mySetGlobalZOrder(node, order)
            if node==nil then return end
            --print("mySetGlobalZOrder:"..order.."\n")
            node:setGlobalZOrder(order)
            local children = node:getChildren()
            if children ~= nil then
                for _, child in pairs(children) do
                    mySetGlobalZOrder(child, order+1)
                end
            end
        end
        
        mySetGlobalZOrder(target, 10001)
        
        local fingerTag = param.finger
        --动画光环
        local ring = cc.Sprite:create("image/ui/guardCenter/guard_ring_2.png")
        --local ring = cc.Sprite:create("image/ui/guardCenter/guard_ring_1.png")
        local nodePos = self:convertToNodeSpace(worldPos)
      --  ring:setPosition(worldPos)
        --ring:setGlobalZOrder(20000)
        local scaleAction1 = cc.ScaleBy:create(0.4,0.84)
       -- local scaleAction1_0 = cc.ScaleBy:create(0.35,1.05)
        local scaleAction2 = scaleAction1:reverse()
       -- local scaleAction2_0 = scaleAction1_0:reverse()
       -- ring:runAction(cc.RepeatForever:create(cc.Sequence:create(scaleAction1_0, scaleAction2_0)))
        --self:addChild(ring)
        
        --local ActionBy = cc.MoveBy:create(2,cc.p(-10,10))
       -- local ActionBy1 = ActionBy:reverse()
        
        --手指
        local figure = cc.Sprite:create("image/ui/guardCenter/guard_figure.png")
        if fingerTag == nil then
            figure:setPosition(worldPos)
        else
            figure:setPosition(worldPos.x,worldPos.y+32)    
        end
        
        figure:setAnchorPoint(0,1)
        figure:setGlobalZOrder(20001)
        figure:runAction(cc.RepeatForever:create(cc.Sequence:create(scaleAction1, scaleAction2)))
        self:addChild(figure)
        
        --特效光环
        local armatureFile = "image/ui/guardCenter/circle/circle.csb"
        ccs.ArmatureDataManager:getInstance():addArmatureFileInfo(armatureFile)
        local skillarmature = ccs.Armature:create("circle")
        skillarmature:setScale(1)
        skillarmature:setGlobalZOrder(20000)
        if fingerTag == nil then
            skillarmature:setPosition(worldPos)
        else
            skillarmature:setPosition(worldPos.x,worldPos.y+32)   
        end
        
        self:addChild(skillarmature)
        --[[速度]]
        --skillarmature:getAnimation():setSpeedScale(0.4)
        skillarmature:getAnimation():playWithIndex(0,0,1)
        
        
        local red = cc.c4b(1,0,1,1)
        local radius = math.max(target:getContentSize().width/128, target:getContentSize().height/128)
        local nCount = 200
        local angel = 2.0*3.1415926/nCount
        local circle = {}
        for i=1, nCount do
            local radian = (i-1)*angel
            circle[i] = {x=radius*math.cos(radian), y=radius*math.sin(radian)}
        end
        local stencil = cc.DrawNode:create()
        stencil:drawPolygon(circle,nCount,red,0, red)
        stencil:setPosition(worldPos)
        clipNode:setStencil(stencil)
    end
end

return GuardLayer
