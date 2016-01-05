require "Cocos2d"

local instance=nil

local PopDialog = class("PopDialog", function()
    return cc.Layer:create()
end)

function PopDialog:create()
    local layer = PopDialog.new()
    layer:initM()
    return layer
end

function PopDialog:onEnter()
end

function PopDialog:onExit()
end

function PopDialog:initM()
    self:setContentSize(cc.Director:getInstance():getWinSize())
    local visibleSize=cc.Director:getInstance():getVisibleSize()
    
    --print(visibleSize.width.."height"..visibleSize.height)
    self.bgImage = cc.Scale9Sprite:create("tishikuang.png")
    self.bgImage:setAnchorPoint(0.5,0.5)
    self.bgImage:setPosition(cc.p(visibleSize.width/2,visibleSize.height/2))
    self:addChild(self.bgImage)

    local ps1 = cc.Spawn:create(cc.ScaleTo:create(0.2,1),cc.MoveBy:create(0.2,cc.p(0,20)))
    local ps2 = cc.Spawn:create(cc.FadeOut:create(1))
    local action= cc.Sequence:create(ps1,ps2,cc.CallFunc:create(self.funcEnd))
    self.bgImage:runAction(action)

    self.alert =  ccui.Text:create()
    self.alert:setFontSize(36)
    self.alert:setAnchorPoint(0,0)
    self.alert:setPosition(cc.p(20,15))
    self.bgImage:addChild(self.alert,100)
end

function PopDialog:funcEnd()
    self:removeFromParent()
end
---------------------------API-----------------------------
function PopDialog:getInstance()
    if instance == nil then
        instance = PopDialog:create()
    end
    return instance
end

--[[--

弹出对应文字的提示框

@param string str 文字的内容
@param cc.c3b coloc 文字的颜色（默认（0，255，0））

]]
function PopDialog:popText(str,color)
    instance=nil
    if self:isRunning() == true then return end
    if  color == nil then color = cc.c3b(0, 255, 0) end
    self.alert:setColor(color)
    self.alert:setString(str)
    
    local ninesize = self.alert:getContentSize()
    ninesize.width = ninesize.width+40
    ninesize.height = ninesize.height+30
    self.bgImage:setPreferredSize(ninesize)
    local currScene = cc.Director:getInstance():getRunningScene()
    currScene:addChild(self)
end

return PopDialog
