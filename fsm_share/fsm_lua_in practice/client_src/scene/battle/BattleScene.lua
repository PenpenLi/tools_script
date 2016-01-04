require "Cocos2d"

BOOK_AGENT = (require "scene.battle.BookAgent"):create()
ACTOR_AGENT = (require "scene.battle.ActorAgent"):create()
DIRECTOR_AGENT = (require "scene.battle.DirectorAgent"):create()

local BattleScene = class("BattleScene", function()
    return cc.Scene:create()
end)

--[[
param = 
{
    teamsL = {
        {
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
        }
    },
    teamsR = {
        {
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
        },
        {
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
        },
        {
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
            {npcId = xxx, npcLevel = xxx},
        },
    },
    opMode = 0 --(0 manual only, 1 auto only, 2 manual or auto),
    bgID = 1 --(background number)
}
]]--
function BattleScene:create(param)
    assert(type(param) == "table","param type err")
    local scene = BattleScene.new()
    scene:initM(param)
    return scene
end

function BattleScene:ctor()
end

function BattleScene:onEnter()
    --local function scheduleUpdate() self:updateM() end
    --self:runAction(cc.RepeatForever:create(cc.Sequence:create(cc.CallFunc:create(scheduleUpdate),
    --    cc.DelayTime:create(1/30))))
    local function scheduleUpdate() self:updateM() end
    self:scheduleUpdateWithPriorityLua(scheduleUpdate, 1)
end

function BattleScene:onExit()
    self:removeChild(ACTOR_AGENT)
    BOOK_AGENT = nil
    ACTOR_AGENT = nil
    DIRECTOR_AGENT = nil
    self:unregisterScriptHandler()
end

--[[
All actors' data is passed by controller, and all books can only be fetched by director.
It means that actors have no relation with books. 
Data flows: Books -> Directors -> Actors
Control flows: Directors -> Books, Directors -> Actors
]]--
function BattleScene:initM(param)
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    
    local bgMusicPath = cc.FileUtils:getInstance():fullPathForFilename("sound/battle_bg_music.mp3") 
    cc.SimpleAudioEngine:getInstance():playMusic(bgMusicPath, true)
    
    self:registerScriptHandler(onNodeEvent)
    --[[
    --1. (Model) init battle data configure
    param = {
        teamsL = {
            --myself first team
            {
                {npcId=10000, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=2000, currHp=2000},
                {npcId=10001, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=4000, currHp=4000},
                {npcId=10002, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=4000, currHp=4000},
                {npcId=10003, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=4000, currHp=4000},
                {npcId=10004, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=2000, currHp=2000},
            }
        },
        
        teamsR = {
            --enemy first team
            {
                {npcId=10005, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=2000},
                {npcId=10006, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10007, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10008, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10009, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
            },
            --enemy second team
            {
                {npcId=10000, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10001, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10002, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10003, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10004, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
            },
            --enemy third team
            {
                {npcId=10000, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10002, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10007, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10009, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
                {npcId=10004, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000},
            }
        },
        
        bgID = 5,
        opMode = 2
    }
    ]]--
    
    BOOK_AGENT = BOOK_AGENT or (require "scene.battle.BookAgent"):create()
    ACTOR_AGENT = ACTOR_AGENT or (require "scene.battle.ActorAgent"):create()
    DIRECTOR_AGENT = DIRECTOR_AGENT or (require "scene.battle.DirectorAgent"):create()
    
    local teamLtable = {}
    table.insert(teamLtable,param[1])
    
    BOOK_AGENT:set_teamsLBook(teamLtable)
    BOOK_AGENT:set_teamsRBook(param[2])
    BOOK_AGENT:set_opMode(5)
    BOOK_AGENT:set_bgID(2)
    --[[
    BOOK_AGENT:set_teamsLBook(param.teamsL)
    BOOK_AGENT:set_teamsRBook(param.teamsR)
    BOOK_AGENT:set_opMode(param.opMode)
    BOOK_AGENT:set_bgID(param.bgID)
    ]]
    
    --2. (View) init battle scene layers
    self:addChild(ACTOR_AGENT)
    
    --3. (Controller) init battle controller
    DIRECTOR_AGENT:reset()
    
    self:loadResAsync()
end

function BattleScene:loadResAsync()
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/huaite/smallSkillAttack_huaite_daoguang.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/LiNa/xiaohuoqiu.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/captain/bigSkillAttack_captain.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/FanKeLiFu/bigSkillAttack_fankelifu.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/FengXing/bigSkillAttack_fengxing.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/huaite/smallSkillAttack_huaite_daoguang.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/huaite/smallSkillAttack_huaite_dimian.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/huaite/bigSkillAttack_huaite.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/KuLouWang/dazhao.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/XiaoHei/dazhao.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/zhurizhe/bigSkillAttack_zhurizhe.png", function(p) self:loadOk() end)
    cc.Director:getInstance():getTextureCache():addImageAsync("battle/skill/LiNa/fenghuang.png", function(p) self:loadOk() end)
    --ccs.ArmatureDataManager:getInstance():addArmatureFileInfoAsync("battle/ui/leiyun/leiyun.csb", function(p) self:loadOk() end)
end

function BattleScene:loadOk()
    print("[BattleScene]loadOk")
    self.loadNum = self.loadNum or 0
    self.loadNum = self.loadNum + 1
    if self.loadNum == 13 then
        self.loadOk = true
        cclog("--------------------RES ASYNC LOAD OK-----------------")
    end
end

function BattleScene:updateM()
    --print("updateM")
    if self.loadOk == nil then return end
    DIRECTOR_AGENT:updateM()
end

--[[helper functions]]--

return BattleScene
