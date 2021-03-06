require "Cocos2d"

local DailyTaskUILayer = require "scene.dailytask.DailyTaskLayer"

local HomeUILayer = require "scene.home.HomeUILayer"

local DailyTaskScene = class("DailyTaskScene", function()
    return cc.Scene:create()
end)

function DailyTaskScene:create(param)
    local scene = DailyTaskScene.new()
    scene:initM(param)
    return scene
end

function DailyTaskScene:ctor()
end

function DailyTaskScene:initM(param)
    self.dailytaskUILayer = DailyTaskUILayer:create(param)
    self:addChild(self.dailytaskUILayer)
    
    self.homeUILayer = HomeUILayer:create()
    self:addChild(self.homeUILayer)
end

return DailyTaskScene
