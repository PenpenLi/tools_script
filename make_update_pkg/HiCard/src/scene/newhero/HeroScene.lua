require "Cocos2d"

local HeroLayer = require "scene.newhero.NewHeroUI"


local HeroScene = class("HeroScene", function()
    return cc.Scene:create()
end)

function HeroScene:create()
    local scene = HeroScene.new()
    scene:initM()
    return scene
end

function HeroScene:ctor()
end

function HeroScene:initM()
    self.heroLayer = HeroLayer:create()
    self:addChild(self.heroLayer)
end

return HeroScene
