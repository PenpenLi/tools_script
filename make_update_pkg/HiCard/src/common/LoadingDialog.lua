require "Cocos2d"

local LoadingDialog = class("LoadingDialog", function()
    return cc.Layer:create()
end)

function LoadingDialog:create()
    local layer = LoadingDialog.new()
    layer:initM()
    return layer
end

function LoadingDialog:onEnter()
end

function LoadingDialog:onExit()
end

function LoadingDialog:initM()
    self:setContentSize(cc.Director:getInstance():getWinSize())
    local visibleSize=cc.Director:getInstance():getVisibleSize()
    self.sprite = cc.Sprite:create("alertDlg_loading.png")
    self.sprite:setPosition(cc.p(visibleSize.width/2,visibleSize.height/2))
    self.sprite:runAction(cc.RepeatForever:create(cc.RotateBy:create(1.5,360)))
    self:addChild(self.sprite)

    local function onTouchBegan(touch, event) return true end
    local function onTouchMoved(touch, event) end
    local function onTouchEnded(touch, event) end
    local listener = cc.EventListenerTouchOneByOne:create()
    listener:setSwallowTouches(true)
    listener:registerScriptHandler(onTouchBegan,cc.Handler.EVENT_TOUCH_BEGAN )
    listener:registerScriptHandler(onTouchMoved,cc.Handler.EVENT_TOUCH_MOVED )
    listener:registerScriptHandler(onTouchEnded,cc.Handler.EVENT_TOUCH_ENDED )
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithSceneGraphPriority(listener, self)
end

---------------------------API-----------------------------
function LoadingDialog:start()
    if self:isRunning() == true then return end
    local currScene = cc.Director:getInstance():getRunningScene()
    local isloading = currScene:getChildByName("loading")
    if isloading ~= nil then
        currScene:removeChildByName("loading")
    end
    currScene:addChild(self)
    self:setName("loading")
end

function LoadingDialog:stop()
    self:removeFromParent()
end

return LoadingDialog
