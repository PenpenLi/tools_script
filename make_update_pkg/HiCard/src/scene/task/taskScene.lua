require "Cocos2d"

local TaskLayer = require "scene.task.taskUI"

local HomeUILayer = require "scene.home.HomeUILayer"

local TaskScene = class("TaskScene", function()
    return cc.Scene:create()
end)

function TaskScene:create()
    local scene = TaskScene.new()
    scene:initM()
    return scene
end

function TaskScene:ctor()
end

function TaskScene:initM()
    self.taskLayer = TaskLayer:create()
    self:addChild(self.taskLayer)
    
    self.homeUILayer = HomeUILayer:create()
    self:addChild(self.homeUILayer)
end

return TaskScene
