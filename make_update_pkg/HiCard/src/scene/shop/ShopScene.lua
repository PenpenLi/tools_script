require "Cocos2d"

local ShopCityLayer = require "scene.shop.ShopUI"

local HomeUILayer = require "scene.home.HomeUILayer"

local ShopScene = class("ShopScene", function()
    return cc.Scene:create()
end)

function ShopScene:create()
    local scene = ShopScene.new()
    scene:initM()
    return scene
end

function ShopScene:ctor()
end

function ShopScene:initM()
    self.shopCityLayer = ShopCityLayer:create()
    self:addChild(self.shopCityLayer)
    
    self.homeUILayer = HomeUILayer:create()
    self:addChild(self.homeUILayer)
end

return ShopScene
