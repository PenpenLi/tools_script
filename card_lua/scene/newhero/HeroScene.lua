require "Cocos2d"

local HeroLayer = require "scene.newhero.NewHeroUI"


local HeroScene = class("HeroScene", function()
    return cc.Scene:create()
end)

function HeroScene:create(res)
    local scene = HeroScene.new()
    scene:initM(res)
    return scene
end

function HeroScene:ctor()
end

function HeroScene:initM(res)
    self.heroLayer = HeroLayer:create(res)
    self:addChild(self.heroLayer)
end

return HeroScene
