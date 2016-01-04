require "Cocos2d"

local JueXingCityLayer = require "scene.juexing.JueXingUI"


local JueXingScene = class("JueXingScene", function()
    return cc.Scene:create()
end)

function JueXingScene:create()
    local scene = JueXingScene.new()
    scene:initM()
    return scene
end

function JueXingScene:ctor()
end

function JueXingScene:initM()
    self.juexingCityLayer = JueXingCityLayer:create()
    self:addChild(self.juexingCityLayer)
end

return JueXingScene
