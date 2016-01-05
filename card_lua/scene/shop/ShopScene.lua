require "Cocos2d"

local ShopCityLayer = require "scene.shop.ShopUI"

local HomeUILayer = require "scene.home.HomeUILayer"

local ShopScene = class("ShopScene", function()
    return cc.Scene:create()
end)

function ShopScene:create(res)
    local scene = ShopScene.new()
    scene:initM(res)
    return scene
end

function ShopScene:ctor()
end

function ShopScene:initM(res)
    self.shopCityLayer = ShopCityLayer:create(res)
    self:addChild(self.shopCityLayer)
    
    self.homeUILayer = HomeUILayer:create()
    self:addChild(self.homeUILayer)
end

return ShopScene
