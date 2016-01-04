require "Cocos2d"

local ClickDialog = class("ClickDialog", function()
    return cc.Layer:create()
end)

function ClickDialog:create(param)
    local layer = ClickDialog.new()
    layer:initM(param)
    return layer
end

function ClickDialog:onEnter()
end

function ClickDialog:onExit()
end

function ClickDialog:initM(param)
    local winSize = cc.Director:getInstance():getWinSize()
    local colorLayer = cc.LayerColor:create(cc.c4b(0, 0, 0, 10), WIN_SIZE.width, WIN_SIZE.height)
    --colorLayer:setPosition(-WIN_SIZE.width/2,-WIN_SIZE.height/2)
    --colorLayer:setOpacity(100)
    --colorLayer:setGlobalZOrder(10000)
    local function onTouchBegan(touch, event) return true end
    local function onTouchMoved(touch, event) end
    local function onTouchEnded(touch, event) print("Mask Swallow Touches") end
    local listener = cc.EventListenerTouchOneByOne:create()
    listener:setSwallowTouches(true)
    listener:registerScriptHandler(onTouchBegan,cc.Handler.EVENT_TOUCH_BEGAN )
    listener:registerScriptHandler(onTouchMoved,cc.Handler.EVENT_TOUCH_MOVED )
    listener:registerScriptHandler(onTouchEnded,cc.Handler.EVENT_TOUCH_ENDED )
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithSceneGraphPriority(listener, colorLayer)
    self:addChild(colorLayer)
    --[[
    local bg = ccui.ImageView:create()
    bg:loadTexture("image/common/tankuang.png", 0)
    --bg:setAnchorPoint(0,0)
    bg:setContentSize(400, 200)
    bg:setPosition(winSize.width/2, winSize.height/2)
    bg:setScale9Enabled(true)
    self:addChild(bg)
    ]]
    --[[
    local tip = ccui.Text:create()
    tip:setString(param.tip)
    tip:setPosition(winSize.width/2, winSize.height/2)
    tip:setFontSize(16)
    self:addChild(tip)
    ]]
    local btn = ccui.Button:create()
    btn:setPosition(winSize.width/2, winSize.height/2)
    btn:setTitleText(param.name)
    btn:setTitleFontSize(20)
    btn:loadTextureNormal("image/common/button_01.png", 0)
    btn:addTouchEventListener(function(sender, eventType)
        if eventType == ccui.TouchEventType.ended then
            param.cb()
        end
    end)
    self:addChild(btn)
end

return ClickDialog
