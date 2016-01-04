require "Cocos2d"

local FormationUILayer = require "scene.formation.FormationUILayer"

local FormationScene = class("FormationScene", function()
    return cc.Scene:create()
end)

function FormationScene:create(param)
    local scene = FormationScene.new()
    scene:initM(param)
    return scene
end

function FormationScene:ctor()
end

function FormationScene:initM(param)
    self.formationUILayer = FormationUILayer:create(param)
    self:addChild(self.formationUILayer)
end

return FormationScene
