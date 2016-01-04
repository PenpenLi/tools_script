require "Cocos2d"

local RechargeCityLayer = require "scene.shop.RechargeUI"


local RechargeScene = class("RechargeScene", function()
    return cc.Scene:create()
end)

function RechargeScene:create()
    local scene = RechargeScene.new()
    scene:initM()
    return scene
end

function RechargeScene:ctor()
end

function RechargeScene:initM()
    self.rechargeCityLayer = RechargeCityLayer:create()
    self:addChild(self.rechargeCityLayer)
end

return RechargeScene
