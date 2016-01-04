require "Cocos2d"

local MailCityLayer = require "scene.mail.Mail"


local MailScene = class("MailScene", function()
    return cc.Scene:create()
end)

function MailScene:create()
    local scene = MailScene.new()
    scene:initM()
    return scene
end

function MailScene:ctor()
end

function MailScene:initM()
    self.mailCityLayer = MailCityLayer:create()
    self:addChild(self.mailCityLayer)
end

return MailScene
