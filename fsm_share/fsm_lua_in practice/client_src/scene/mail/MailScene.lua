require "Cocos2d"

local MailCityLayer = require "scene.mail.Mail"


local MailScene = class("MailScene", function()
    return cc.Scene:create()
end)

function MailScene:create(res)
    local scene = MailScene.new()
    scene:initM(res)
    return scene
end

function MailScene:ctor()
end

function MailScene:initM(res)
    self.mailCityLayer = MailCityLayer:create(res)
    self:addChild(self.mailCityLayer)
end

return MailScene
