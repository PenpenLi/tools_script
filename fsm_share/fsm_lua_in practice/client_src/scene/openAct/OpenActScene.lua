require "Cocos2d"

local openActLayer = require "scene.openAct.OpenAct"


local OpenActScene = class("OpenActScene", function()
    return cc.Scene:create()
end)

function OpenActScene:create(dat)
    local scene = OpenActScene.new()
    scene:initM(dat)
    return scene
end

function OpenActScene:ctor()
end

function OpenActScene:initM(dat)
    self.openActLayer = openActLayer:create(dat)
    self:addChild(self.openActLayer)
end

return OpenActScene