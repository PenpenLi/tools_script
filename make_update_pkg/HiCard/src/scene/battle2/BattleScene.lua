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

function BattleScene:onEnter()
    local function scheduleUpdate(dt) self:updateM(dt) end
    local function schedulePK(dt) self:updatePK(dt) end
    if self.fightMode == 1 then
        self:scheduleUpdateWithPriorityLua(scheduleUpdate, 1)
    elseif self.fightMode == 2 then
        self:scheduleUpdateWithPriorityLua(schedulePK, 1)
    end
end

function BattleScene:onExit()
    bookAgent = nil
    directorAgent = nil
    actorAgent = nil
    self:unregisterScriptHandler()
end

--[[
All actors' data is passed by controller, and all books can only be fetched by director.
It means that actors have no relation with books. 
Data flows: Books -> Directors -> Actors
Control flows: Directors -> Books, Directors -> Actors
]]--
function BattleScene:initM(param)
    --printJSON(param)
    cc.Director:getInstance():getTextureCache():removeAllTextures()
    math.randomseed(os.time())
    --1. (Model) init battle data configure
    local param = self:transFormData(param)
    local battleConfig = {
        teamsL = {
            --myself first team
            {
                {npcId=10016, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=100, armorVal=20, maxHp=10000, currHp=10000,memberId=1,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
               
                --{npcId=10016, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=10000, currHp=10000,memberId=2,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
            --[[{npcId=10013, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=1000, currHp=1000,memberId=3,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10014, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=1000, currHp=1000,memberId=4,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10010, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=200000, currHp=200000,memberId=5,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},             
                ]]
            }
        },

        teamsR = {
            --enemy first team
            {
                {npcId=10005, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=20000, currHp=20000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10005, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=10000, currHp=10000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10010, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10005, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10005, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=10005, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=20001, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=2000, currHp=2000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                --{npcId=20010, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
            },
            --enemy second team
            {
                {npcId=10000, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10001, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10002, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=10002, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=1000, currHp=1000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
                {npcId=20001, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=4000, currHp=4000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4,zhili=100},
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
                {npcId=20010, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=200, currHp=200,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4},
                {npcId=20011, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=2000, currHp=2000,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4},
                {npcId=20011, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=400, bigSkillLevel=1, bigSkillAttackVal=600, armorVal=20, maxHp=200, currHp=200,treasuresNum=0, cri=10, block=10,dex=10,agl=10,soulId=1,attackInterval=2000,smallSkillInterval=2,bigSkillInterval=4},

            }
        },

        fId = nil,--场次Id
        roundTime = 0, --战斗时间
        treasures = 0, --掉落宝箱数
        bgID = 5,
        opMode = 2
    }
    
    self.fightMode = 1
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
    for i=1,#tempTeamsL do
        local memberId = tempTeamsL[i]["memberId"]
        if  memberId == nil then return end
        table.insert(memberList,memberId)
        table.insert(zhiliList,tempTeamsL[i]["zhili"])
    end
    agentData.actorAgentParam.memberList = memberList
    agentData.actorAgentParam.zhiliList = zhiliList
    agentData.actorAgentParam.fId = battleConfig.fId
    agentData.actorAgentParam.roundTime = battleConfig.roundTime
    
    bookAgent = (require "scene.battle2.BookAgent"):create()
    directorAgent = (require "scene.battle2.DirectorAgent"):create()
    actorAgent = (require "scene.battle2.ActorAgent"):create(agentData.actorAgentParam)
    
    --1. (Model) init battle data configure
    bookAgent:initBook(battleConfig)
    
    --2. (View) init battle scene layers
    self:addChild(actorAgent)
    
    --3. (Controller) init battle controller
    directorAgent:start()
    
    --4. load res
    self:loadAsync()
end

function BattleScene:updateM(dt)
    --if self.loadOk == nil then return end
    directorAgent:updateM(dt)
    actorAgent:updateM(dt)
end

function BattleScene:initPK(param)
    cc.Director:getInstance():getTextureCache():removeAllTextures()
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
    self:loadAsync()
    
    self.cmds_str = param.report["replay"]
    --local file = io.open("/Users/ywxx/Documents/chris/study/cmds","w")
    --file:write(self.cmds_str)
    --file:close()
    
    self.pos = 2
    self.sq = 0
    
    --battle result
    self.battlePKfinish = param.report["finish"]
end

function BattleScene:fetchCMD()
    local startPos, endPos = string.find(self.cmds_str, "},{", self.pos)
    if startPos==nil or endPos==nil then return 1 end
    local cmd_str = string.sub(self.cmds_str, self.pos, startPos)
    local cmd = json.decode(cmd_str)
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

function BattleScene:loadAsync()
    local imageList = {
        "battle/skill/huaite/smallSkillAttack_huaite_daoguang.pvr.ccz",
        "battle/skill/huaite/smallSkillAttack_huaite_dimian.pvr.ccz",
        "battle/skill/LiNa/dahuoqiu.pvr.ccz",
        "battle/skill/XiaoHei/zidan.pvr.ccz",
        "battle/skill/LiNa/dahuoqiu.pvr.ccz",
        "battle/skill/captain/bigSkillAttack_captain.pvr.ccz",
        "battle/skill/FengXing/bigSkillAttack_fengxing.pvr.ccz",
        "battle/skill/huaite/bigSkillAttack_huaite.pvr.ccz",
        "battle/skill/KuLouWang/dazhao.pvr.ccz",
        "battle/skill/LiNa/fenghuang.pvr.ccz",
        "battle/skill/zhurizhe/bigSkillAttack_zhurizhe.pvr.ccz",
        "battle/skill/huaite/smallSkillAttack_huaite_daoguang.pvr.ccz",
        "battle/skill/huaite/smallSkillAttack_huaite_dimian.pvr.ccz",
        "battle/skill/XiaoHei/bingbao.pvr.ccz"
    }
    
    local function loadOk()
        print("[BattleScene]loadOk")
        self.loadNum = self.loadNum or 0
        self.loadNum = self.loadNum + 1
        if self.loadNum == #imageList then
            self.loadOk = true
            cclog("--------------------RES ASYNC LOAD OK-----------------")
        end
    end
    
    for i=1, #imageList do
        cc.Director:getInstance():getTextureCache():addImageAsync(imageList[i], loadOk)
    end
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
    if param ==  nil then return end
    local npcMapConfig = require("config.define_npc_map")
    local soulConfig = require("config.define_soul")
    local fieldId = param["fId"]
    local teamL = {}
    local teamR = {}
    local member = param["member"]
    if  member == nil then return end
    --member
    local function parseData(roundsData)
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
            local memberId = tostring(soulConfig[tostring(memberItem["id"])]["series"])

            local tempTable = {npcId=npcMapConfig[memberId], npcLevel=memberItem["level"], normalAttackVal=memberItem["atk"], 
                smallSkillLevel=skillItem[1]["skillLevel"], smallSkillAttackVal=skillItem[1]["skillAtk"], 
                bigSkillLevel=skillItem[2]["skillLevel"], bigSkillAttackVal = skillItem[2]["skillAtk"], 
                armorVal=memberItem["def"], maxHp=memberItem["mhp"], currHp=memberItem["curHp"],
                cri=memberItem["cri"], block=memberItem["block"], dex=memberItem["dex"], agl=memberItem["agl"],zhili = memberItem["zhili"],
                attackInterval=memberItem["attackInterval"],smallSkillInterval=memberItem["smallSkillInterval"],bigSkillInterval=memberItem["bigSkillInterval"],
                soulId = soulId,memberId = memberId,pos = pos,treasuresNum=0
            }
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
        treasures = param["treasures"]}
    return finalData
end

return BattleScene

