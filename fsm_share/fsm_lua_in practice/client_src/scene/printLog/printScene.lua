require "Cocos2d"

local printLayer = require "scene.printLog.TagPrint"


local printScene = class("printScene", function()
    return cc.Scene:create()
end)

function printScene:create()
    local scene = printScene.new()
    scene:initM()
    return scene
end

function printScene:ctor()
end

function printScene:initM()
    self.printLayer = printLayer:create()
    self:addChild(self.printLayer)
end

return printScene