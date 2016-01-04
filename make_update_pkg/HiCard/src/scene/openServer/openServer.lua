require "Cocos2d"

local ServerLayer = require "scene.openServer.openServerUI"
local OpenServer = class("OpenServer", function()
    return cc.Scene:create()
end)

function OpenServer:create()
    local scene = OpenServer.new()
    scene:initM()
    return scene
end

function OpenServer:ctor()
end

function OpenServer:initM()
    self.serverLayer = ServerLayer:create()
    self:addChild(self.serverLayer)
end

return OpenServer
