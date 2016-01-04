require "Cocos2d"
require ("json")

bookAgent = {}
directorAgent = {}
actorAgent = {}

local npcMapConfig = require("config.define_npc_map")
local soulConfig = require("config.define_soul")

local BattleScene = class("BattleScene", function()
    return cc.Scene:create()
end)

function BattleScene:create(param)
    --assert(type(param) == "table","param type err")
    local scene = BattleScene.new()
    scene:initM(param)
    return scene
end

function BattleScene:createPK(param)
    local scene = BattleScene.new()
    scene:initPK(param)
    return scene
end

function BattleScene:createEmulate()
    local emulateBattleJson = cc.FileUtils:getInstance():getStringFromFile("emulate_battle.json")
    local emulateBattleParam = json.decode(emulateBattleJson)
    emulateBattleParam.isEmulate = true
    local scene = self:create(emulateBattleParam)
    return scene
end

function BattleScene:onEnter()
    --[[
    local function scheduleUpdate(dt) self:updateM(dt) end
    local function schedulePK(dt) self:updatePK(dt) end
    if self.fightMode == 1 then
        self:scheduleUpdateWithPriorityLua(scheduleUpdate, 1)
    elseif self.fightMode == 2 then
        self:scheduleUpdateWithPriorityLua(schedulePK, 1)
    end
    ]]
end

function BattleScene:onExit()
    local Formation = require "data.Formation"
    local onExitTag = Formation:get("releaseBattleRes")
    if  onExitTag then
        bookAgent = nil
        directorAgent = nil
        actorAgent = nil
        self:unregisterScriptHandler()
        cc.Director:getInstance():getTextureCache():removeAllTextures()
        cc.SpriteFrameCache:getInstance():removeSpriteFrames()
        for i=1, #self.resList do
            ccs.ArmatureDataManager:getInstance():removeArmatureFileInfo(self.resList[i])
        end
        ccs.ArmatureDataManager:destroyInstance()
    end  
end

--[[
All actors' data is passed by controller, and all books can only be fetched by director.
It means that actors have no relation with books. 
Data flows: Books -> Directors -> Actors
Control flows: Directors -> Books, Directors -> Actors
]]--
function BattleScene:initM(param)
    --printJSON(param)
    ccs.ArmatureDataManager:destroyInstance()
    cc.Director:getInstance():getTextureCache():removeAllTextures()
    cc.SpriteFrameCache:getInstance():removeSpriteFrames()
    self:loadShader()
    math.randomseed(os.time())
    --1. (Model) init battle data configure
    self.fightMode = 1
    self.isEmulate = param.isEmulate or false
    local param = self:transFormData(param)  
    local battleConfig = {
        teamsL = {
            --myself first team
            {
                --{npcId=20003, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=100, armorVal=20, maxHp=10000, currHp=10000,memberId=1,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10010, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=10000, currHp=10000,memberId=2,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10004, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=1000, currHp=1000,memberId=3,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10003, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=1000, currHp=1000,memberId=4,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10002, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=200000, currHp=200000,memberId=5,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},             
                
            }
        },

        teamsR = {
            --enemy first team
            {
                {npcId=20003, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=2, currHp=2,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10005, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=10000, currHp=10000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10004, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10003, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10002, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=2, currHp=12,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10005, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=20001, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=2000, currHp=2000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=20010, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
            },
            --enemy second team
            {
                {npcId=10010, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10011, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10012, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10012, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=20005, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=4000, currHp=4000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
            },
            --enemy third team
            {
                {npcId=10000, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10002, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10007, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10009, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=20002, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=3000, currHp=3000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
            }
        },

        --tower npc data
        towerNpcData = {
            {
                {npcId=20010, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=0, bigSkillLevel=1, bigSkillAttackVal=0, armorVal=20, maxHp=200, currHp=200,treasuresNum=0, cri=10, block=0,dex=10,agl=0,soulId=1,attackInterval=2000,smallSkillInterval=9999,bigSkillInterval=9999,zhili=100},
                {npcId=20011, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=2000, currHp=2000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=20005, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=200, currHp=200,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=20004, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=200, currHp=200,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=20000, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=200, currHp=200,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=20017, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=200, currHp=200,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=4000,smallSkillInterval=20000,bigSkillInterval=20000,zhili=100},
                {npcId=20014, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=200, currHp=200,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=4000,smallSkillInterval=20000,bigSkillInterval=20000,zhili=100},
                {npcId=20015, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=200, currHp=200,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=4000,smallSkillInterval=20000,bigSkillInterval=20000,zhili=100},
                {npcId=20016, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=200, currHp=200,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=4000,smallSkillInterval=20000,bigSkillInterval=20000,zhili=100},

            }
        },

        fId = nil,--场次Id
        roundTime = 0, --战斗时间
        treasures = 0, --掉落宝箱数
        bgID = 5,
        opMode = 2
    }
    
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter() 
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)
    
    --reset book data
    
    if  param and #param.teamL ~=0 then
        local teamLtable = {}
        table.insert(teamLtable,param.teamL)

        battleConfig.teamsL=teamLtable
        battleConfig.teamsR=param.teamR
        battleConfig.fId = param.fId
        battleConfig.roundTime = param.roundTime
        --roll treasures num
        battleConfig.treasures = param.treasures
    end

    --roll treasures to enemy
    self:rollTreasures(battleConfig.teamsR,battleConfig.treasures)
    
    --agent data
    local agentData = {bookAgentParam ={},
                       actorAgentParam ={memberList = {}},
                       directorAgentParam ={}                     
                        }                   
    local memberList ={}
    local zhiliList = {}
    local tempTeamsL = battleConfig.teamsL[1]
    for i=#tempTeamsL,1,-1 do
        local memberId = tempTeamsL[i]["soulId"]
        if  memberId == nil then return end
        table.insert(memberList,memberId)
        table.insert(zhiliList,tempTeamsL[i]["zhili"])
    end
    agentData.actorAgentParam.memberList = memberList
    --agentData.actorAgentParam.zhiliList = zhiliList

    local function calMemberList(team)
        team = team or {}
        local memberList = {}
        for i=1, #team do
            local memberId = team[i]["soulId"]
            table.insert(memberList,memberId)
        end
        return memberList
    end
    
    agentData.actorAgentParam.enemy1 = calMemberList(battleConfig.teamsR[1])
    agentData.actorAgentParam.enemy2 = calMemberList(battleConfig.teamsR[2])
    agentData.actorAgentParam.enemy3 = calMemberList(battleConfig.teamsR[3])
    
    agentData.actorAgentParam.fId = battleConfig.fId
    agentData.actorAgentParam.roundTime = battleConfig.roundTime
    agentData.actorAgentParam.battleType = self.fightMode
    agentData.actorAgentParam.isEmulate = self.isEmulate
    
    --JueXing Skill Config
    --local configJXSkills_l = Helper.calJXSkills(memberList)
    --local configJXSkills_r = Helper.calJXSkills()
    --agentData.actorAgentParam.configJXSkills = configJXSkills_l
    
    bookAgent = (require "scene.battle2.BookAgent"):create()
    directorAgent = (require "scene.battle2.DirectorAgent"):create()
    actorAgent = (require "scene.battle2.ActorAgent"):create({isEmulate=self.isEmulate})
    
    --1. (Model) init battle data configure
    bookAgent:initBook(battleConfig)
    
    --2. (View) init battle scene layers
    self:addChild(actorAgent)
    
    --3. (Controller) init battle controller
    directorAgent:setAutoFight(false)
    directorAgent:setFightMode(self.fightMode)
    directorAgent:setActorAgentData(agentData.actorAgentParam)
    --directorAgent:setJXSkills(configJXSkills_l, 1)
    directorAgent:start()
    
    --4. load res
    self:loadAsync(agentData.actorAgentParam.memberList, agentData.actorAgentParam.enemy1,
        agentData.actorAgentParam.enemy2, agentData.actorAgentParam.enemy3)
end

function BattleScene:updateM(dt)
    --if self.loadOk == nil then return end
    directorAgent:updateM(dt)
    actorAgent:updateM(dt)
end

function BattleScene:start()
    local function scheduleUpdate(dt) self:updateM(dt) end
    local function schedulePK(dt) self:updatePK(dt) end
    if self.fightMode == 1 then
        self:scheduleUpdateWithPriorityLua(scheduleUpdate, 1)
    elseif self.fightMode == 2 then
        self:scheduleUpdateWithPriorityLua(schedulePK, 1)
    end
end

function BattleScene:initPK(param)
    ccs.ArmatureDataManager:destroyInstance()
    cc.Director:getInstance():getTextureCache():removeAllTextures()
    cc.SpriteFrameCache:getInstance():removeSpriteFrames()
    math.randomseed(os.time())
    self:loadShader()
    local function onNodeEvent(event)
        if event == "enter" then self:onEnter()
        elseif event == "exit" then self:onExit() end
    end
    self:registerScriptHandler(onNodeEvent)
    self.fightMode = 2
    local tmpparam = {}
    tmpparam.isPvp = true
    actorAgent = (require "scene.battle2.ActorAgent"):create(tmpparam)
    self:addChild(actorAgent)
    --self:loadAsync()
    
    self.cmds_str = param.report["replay"]
    --print(self.cmds_str)
    --local file = io.open("/Users/ywxx/Documents/chris/study/cmds","w")
    --file:write(self.cmds_str)
    --file:close()
    
    self.pos = 2
    self.sq = 0
    
    --battle result
    self.battlePKfinish = param.report["finish"]
    
    local startPos, endPos = string.find(self.cmds_str, "},{", 2)
    local cmd_str = string.sub(self.cmds_str, 2, startPos)
    local firstCmd = cjson.decode(cmd_str)
    assert("init_actor_agent"==firstCmd.t, "")
    self:loadAsync(firstCmd.data.memberList, firstCmd.data.enemy1,
        firstCmd.data.enemy2, firstCmd.data.enemy3)
end

function BattleScene:fetchCMD()
    local startPos, endPos = string.find(self.cmds_str, "},{", self.pos)
    if startPos==nil or endPos==nil then return 1 end
    local cmd_str = string.sub(self.cmds_str, self.pos, startPos)
    
    local cmd = cjson.decode(cmd_str)
    if cmd.sq == self.sq then
        self.pos = endPos
        return cmd
    else
        return 2
    end
end

function BattleScene:updatePK(dt)
    while true do
        local cmd = self:fetchCMD()
        if cmd == 1 then
            local param = self.battlePKfinish
            actorAgent:onBattleOverPK(param)
            self:unregisterScriptHandler()
            return 
        elseif cmd == 2 then
            break
        else
            actorAgent:actCMD(cmd)
        end
    end
    self.sq = self.sq + 1
end

function BattleScene:loadAsync(my, enemy1, enemy2, enemy3)
    local define_npcs_info = require "config.define_npcs_info"
    local define_soul = require "config.define_soul"
    local resList = {}
    local function addResList(memberList)
        for i=1, #memberList do
            local soulId = memberList[i]
            local soulKey = define_soul[tostring(soulId)]["series"]
            print("[load]"..soulKey)
            local npcId = npcMapConfig[tostring(soulKey)]
            local npcInfo = define_npcs_info[tostring(npcId)]
            local path = "image/battle/armature/"..npcInfo.name.."/"..npcInfo.name..".csb"
            table.insert(resList, path)
        end
    end
    addResList(my)
    addResList(enemy1)
    addResList(enemy2)
    addResList(enemy3)
    self.resList = resList
    
    local function addLoadScreen()
        --self.loadLayer = cc.Node:create()
        self.loadLayer = cc.LayerColor:create(cc.c4b(0, 0, 0,255), WIN_SIZE.width, WIN_SIZE.height)
        local function onTouchBegan(touch, event) return true end
        local function onTouchMoved(touch, event) end
        local function onTouchEnded(touch, event) print("Mask Swallow Touches") end
        local listener = cc.EventListenerTouchOneByOne:create()
        listener:setSwallowTouches(true)
        listener:registerScriptHandler(onTouchBegan,cc.Handler.EVENT_TOUCH_BEGAN )
        listener:registerScriptHandler(onTouchMoved,cc.Handler.EVENT_TOUCH_MOVED )
        listener:registerScriptHandler(onTouchEnded,cc.Handler.EVENT_TOUCH_ENDED )
        local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
        eventDispatcher:addEventListenerWithSceneGraphPriority(listener, self.loadLayer)
        self:addChild(self.loadLayer)
        
        local bg = cc.Sprite:create("image/ui/load/load_gameicon.png")
        bg:setPosition(WIN_SIZE.width/2, WIN_SIZE.height*0.7)
        bg:setScaleX(WIN_SIZE.width/960)
        bg:setScaleY(WIN_SIZE.height/640)
        self.loadLayer:addChild(bg)
        
        local runboy = cc.Sprite:create()
        runboy:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/6+20)
        self.loadLayer:addChild(runboy)

        local loadingBarBg = cc.Sprite:create("image/ui/load/load_bar_bg.png")
        loadingBarBg:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/6)
        loadingBarBg:setScaleX(1.5)
        self.loadLayer:addChild(loadingBarBg)

        local loadingBar = ccui.LoadingBar:create()
        loadingBar:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/6)
        loadingBar:setTag(0)
        loadingBar:setName("LoadingBar")
        loadingBar:loadTexture("image/ui/load/load_bar.png")
        loadingBar:setScaleX(1.5)
        loadingBar:setPercent(0)
        self.loadLayer:addChild(loadingBar)
        self.loadLayer.loadingBar = loadingBar

        local leftIcon = cc.Sprite:create("image/ui/load/load_bar_icon.png")
        leftIcon:setPosition(WIN_SIZE.width/2 - loadingBar:getContentSize().width*1.5/2, WIN_SIZE.height/6)
        self.loadLayer:addChild(leftIcon)
        local rightIcon = cc.Sprite:create("image/ui/load/load_bar_icon.png")
        rightIcon:setPosition(WIN_SIZE.width/2 + loadingBar:getContentSize().width*1.5/2, WIN_SIZE.height/6)
        rightIcon:setScaleX(-1)
        self.loadLayer:addChild(rightIcon)

        local tip = ccui.Text:create()
        tip:setString("正在加载中...0%")
        --alert:setFontName(font_TextName)
        tip:setFontSize(24)
        tip:setColor(cc.c3b(159, 168, 176))    
        tip:setPosition(WIN_SIZE.width/2, WIN_SIZE.height/10)
        self.loadLayer:addChild(tip)
        self.loadLayer.tip = tip
    end
    
    local function loadOk()
        self.loadNum = self.loadNum or 0
        self.loadNum = self.loadNum + 1
        local percent = math.min(math.ceil(self.loadNum*100/(#resList)), 100)
        self.loadLayer.loadingBar:setPercent(percent)
        self.loadLayer.tip:setString("正在加载中..."..percent.."%")
        if self.loadNum >= #resList then
            --load success
            self:removeChild(self.loadLayer)
            self:start()
        end
    end
    
    addLoadScreen()
    
    Helper.setTimeout(self, 1, function()
        for i=1, #resList do
            ccs.ArmatureDataManager:getInstance():addArmatureFileInfoAsync(resList[i], loadOk)
        end
    end)
end

function BattleScene:loadShader()
    local shader = cc.GLProgram:create("shader/gray1.vsh", "shader/gray1.fsh")
    --shader:retain()
    shader:bindAttribLocation(cc.ATTRIBUTE_NAME_POSITION, cc.VERTEX_ATTRIB_POSITION)
    shader:bindAttribLocation(cc.ATTRIBUTE_NAME_COLOR, cc.VERTEX_ATTRIB_COLOR)
    shader:bindAttribLocation(cc.ATTRIBUTE_NAME_TEX_COORD, cc.VERTEX_ATTRIB_TEX_COORDS)
    shader:retain()
    shader:link()
    shader:updateUniforms()
    cc.GLProgramCache:getInstance():addGLProgram(shader, "bigskill")
end

function BattleScene:rollTreasures(enemys,treasures)
	local firstSize = 0
	local secondSize = 0 
	local thirdSize = 0
    for i=1,#enemys do
		if  i == 1 then
		    firstSize = #enemys[i]
		elseif i == 2 then
		    secondSize = #enemys[i]
	    elseif i == 3 then
	       thirdSize = #enemys[i]
		end
	end
	local totalSize = firstSize+secondSize+thirdSize-1
	for i=1,treasures do
		local targetEnemy = math.random(1,totalSize)
		if targetEnemy <= firstSize then
            enemys[1][targetEnemy].treasuresNum = enemys[1][targetEnemy].treasuresNum +1
        elseif targetEnemy >firstSize and targetEnemy <= secondSize +firstSize then
            enemys[2][targetEnemy-firstSize].treasuresNum = enemys[2][targetEnemy-firstSize].treasuresNum +1
        else    
            enemys[3][targetEnemy-firstSize-secondSize].treasuresNum = enemys[3][targetEnemy-firstSize-secondSize].treasuresNum +1
		end
	end
end

function BattleScene:transFormData(param)
    local qulityFactorConfig = {0.65, 0.71, 0.79, 0.89, 1, 1, 1,1,1,1,1,1,1,1,1,1}
    if param == nil then return end
    local npcMapConfig = require("config.define_npc_map")
    local soulConfig = require("config.define_soul")
    local npcSkillConfig = require("config.define_npc_skill")
    local fieldId = param["fId"]
    local teamL = {}
    local teamR = {}
    local member = param["member"]
    if  member == nil then return end
    --member
    local function parseData(roundsData)
        if roundsData == nil or roundsData=={} then return {} end
        local tmpTeam = {}
        for i=1,table.getn(roundsData) do
            local tempTable = {}
            local memberItem = roundsData[i]
            local skillItem = memberItem["skill"]
            local soulId = memberItem["id"]
            local pos = memberItem["pos"]
            if memberItem["id"] == 0 then
                return tmpTeam
            end
            print(memberItem["id"])

            local soulInfo = soulConfig[tostring(memberItem["id"])]
            local memberId = tostring(soulInfo["series"])
            local tempTable = {soulId=soulId, npcId=npcMapConfig[memberId], npcLevel=memberItem["level"], normalAttackVal=memberItem["atk"], 
                smallSkillLevel=skillItem[1]["skillLevel"], smallSkillAttackVal=skillItem[1]["skillAtk"], 
                bigSkillLevel=skillItem[2]["skillLevel"], bigSkillAttackVal = skillItem[2]["skillAtk"], 
                armorVal=memberItem["def"], maxHp=memberItem["mhp"], currHp=memberItem["mhp"],
                cri=memberItem["cri"], block=memberItem["block"], dex=memberItem["dex"], agl=memberItem["agl"],zhili = memberItem["zhili"],
                attackInterval=memberItem["attackInterval"],smallSkillInterval=memberItem["smallSkillInterval"],bigSkillInterval=memberItem["bigSkillInterval"],
                memberId = memberId,pos = pos,treasuresNum=0,boss = memberItem["boss"]
            }

            local bigSkill = npcSkillConfig[tostring(tonumber(tempTable.npcId)*10+1)]
            local smallSkill = npcSkillConfig[tostring(tonumber(tempTable.npcId)*10+2)]
            if smallSkill~=nil and bigSkill~=nil then
                assert(smallSkill["owner"]==bigSkill["owner"], "")
                local qulityFactor = qulityFactorConfig[soulInfo["quality"]]
                if smallSkill["once_hurt_hp_factor"] then
                    tempTable.smallSkillAttackVal = (1.5+tempTable.smallSkillLevel/100)*tempTable.normalAttackVal*smallSkill["once_hurt_hp_factor"]
                end
                if bigSkill["once_hurt_hp_factor"] then
                    tempTable.bigSkillAttackVal = (1.5+tempTable.bigSkillLevel/100)*tempTable.normalAttackVal*bigSkill["once_hurt_hp_factor"]
                end
                tempTable.smallOnGoingHp = smallSkill["persist_hurt_hp_factor"]*(1.5+tempTable.smallSkillLevel/100)*tempTable.normalAttackVal
                tempTable.bigOnGoingHp = bigSkill["persist_hurt_hp_factor"]*(1.5+tempTable.bigSkillLevel/100)*tempTable.normalAttackVal
                tempTable.persistentSmallSkillFramesNum = math.floor(smallSkill["persist_time"]*30/1000)
                tempTable.persistentBigSkillFramesNum = math.floor(bigSkill["persist_time"]*30/1000)
                tempTable.persistentSmallSkillCount = smallSkill["persist_count"]
                tempTable.persistentBigSkillCount = bigSkill["persist_count"]
                
                tempTable.armorVal = tempTable.armorVal*(1+bigSkill["add_def_factor"])
                tempTable.cri = tempTable.cri*(1+bigSkill["add_baoji_factor"])
                tempTable.block = tempTable.block*(1+bigSkill["add_gedang_factor"])
                tempTable.dex = tempTable.dex*(1+bigSkill["add_hit_factor"])
                tempTable.agl = tempTable.agl*(1+bigSkill["add_miss_factor"])
                
                -- skill config 
                tempTable.big_xuanyuntime = bigSkill["xuanyuntime"]
                tempTable.small_xuanyuntime = smallSkill["xuanyuntime"]
                tempTable.big_chenmotime = bigSkill["chenmotime"]
                tempTable.small_chenmotime = smallSkill["chenmotime"]
                tempTable.big_noname_data1 = bigSkill["noname_data1"]
                tempTable.small_noname_data1 = smallSkill["noname_data1"]
                tempTable.big_noname_data2 = bigSkill["noname_data2"]
                tempTable.small_noname_data2 = smallSkill["noname_data2"]
                tempTable.big_noname_data3 = bigSkill["noname_data3"]
                tempTable.small_noname_data3 = smallSkill["noname_data3"]
            else
                print("[BattleScene]ERR: no skill config in skillTable for "..tempTable.npcId)
            end
            table.insert(tmpTeam,tempTable)
        end
        return tmpTeam
    end
    
    teamL = parseData(member)
    --first
    local firstmonster = param["second"]
    local tempR1 = parseData(firstmonster)
    if  #tempR1 ~= 0 then
        table.insert(teamR,tempR1)
    end
    --seconde
    local secondmonster = param["third"]
    local tempR2 = parseData(secondmonster)
    if  #tempR2 ~= 0 then
        table.insert(teamR,tempR2)
    end
    --third
    local thirdmonster = param["fourth"]
    local tempR3 = parseData(thirdmonster)
    if  #tempR3 ~= 0 then
        table.insert(teamR,tempR3)
    end
    --final
    local finalData = {teamL =teamL,teamR = teamR,
        fId = param["fId"],roundTime=param["maxRounds"],
        treasures = param["treasures"],bt=param["bt"]}
    return finalData
end

return BattleScene

