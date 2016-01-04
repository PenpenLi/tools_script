require "extern"
require "Cocos2d"
require "myjson"

bookAgent = {}
directorAgent = {}
actorAgent = {}
WIN_SIZE = {width=960,height=640}
    
Helper = {}

function Helper.createJXSkill(jxSkillId, zhili)
    local define_jueXing_skill = require "config.juexing_skill"
    local jxSkill = define_jueXing_skill[tostring(jxSkillId)]
    local configJXSkills = {}
    local data = {}
    for key, value in pairs(jxSkill) do
        data[key] = value
    end
    data.type = jxSkillId
    data.zhili = zhili
    table.insert(configJXSkills, data)
    return configJXSkills
end

function Helper.calJXSkills(memberList)
    local define_soul = require "config.define_soul"
    local define_jueXing_skill = require "config.juexing_skill"
    local jxSkills, configJXSkills = {}, {}
    for _, v in pairs(memberList) do
        local soul = define_soul[tostring(v)]
        if soul then
            local blast_book_id = soul["blast_book_id"]
            if jxSkills[blast_book_id] == nil then
                jxSkills[blast_book_id] = 1
            else
                jxSkills[blast_book_id] = jxSkills[blast_book_id] + 1
            end
        end
    end
    for k, v in pairs(jxSkills) do
        local jxSkill = define_jueXing_skill[tostring(k)]
        if jxSkill == nil then 
            print("[ERR] no jxSkill for type "..k)
        else
            local lv_num = jxSkill["lv_num"]
            if v >= lv_num then
                local data = {}
                for key, value in pairs(jxSkill) do
                    data[key] = value
                end
                data.type = k
                table.insert(configJXSkills, data)
            end
        end
    end
    return configJXSkills
end

guardCenter = {}
function guardCenter:canEnterGuardMode(str)
    return false
end

function printJSON(t)
    if type(t) ~= "table" then
        print("print only support table")
        return
    end
    print(json_encode(t))
end

local BattleScene = class("BattleScene", function()
    return {}
end)

function BattleScene:create(param)
    --assert(type(param) == "table","param type err")
    local scene = BattleScene.new()
    scene:initM(param)
    return scene
end

--[[
All actors' data is passed by controller, and all books can only be fetched by director.
It means that actors have no relation with books. 
Data flows: Books -> Directors -> Actors
Control flows: Directors -> Books, Directors -> Actors
]]--
function BattleScene:initM(param)
    math.randomseed(os.time())
    local param = self:transFormData(json_decode(param))
    self.isEmulate = param.isEmulate or false
    --1. (Model) init battle data configure
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
    agentData.actorAgentParam.battleType = 2
    agentData.actorAgentParam.isEmulate = self.isEmulate

    bookAgent = (require "scene.battle2.BookAgent"):create()
    directorAgent = (require "scene.battle2.DirectorAgent"):create()
    actorAgent = (require "ActorAgent"):create(agentData.actorAgentParam)
    
    --1. (Model) init battle data configure
    bookAgent:initBook(battleConfig)
    
    --2. (View) init battle scene layers
    --self:addChild(actorAgent)
    
    --3. (Controller) init battle controller
    directorAgent:setAutoFight(false)
    directorAgent:setFightMode(2)
    directorAgent:setActorAgentData(agentData.actorAgentParam)
    directorAgent:start()
end

function table.getn(t)
    return #t
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


function BattleScene:start()
	while true do
		if actorAgent:updateM() == true then
			break
		end
		directorAgent:updateM(1/30)
	end
    local ret = {}
    ret.cmds = actorAgent:getBattleCmds()
    ret.battleResult = actorAgent:getBattleResult()
    return json_encode(ret)
end


--[[
local battleScene = BattleScene:create({})
local s = os.clock()
battleScene:loop()
local e = os.clock()
print("used time: "..e-s.." seconds")
]]
return BattleScene


