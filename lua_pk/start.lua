require "extern"
require "Cocos2d"
require "json"

bookAgent = {}
directorAgent = {}
actorAgent = {}
WIN_SIZE = {width=960,height=640}

function printJSON(t)
    if type(t) ~= "table" then
        print("print only support table")
        return
    end
    print(json.encode(t))
end

local BattleScene = class("BattleScene", function()
    return {}
end)

function BattleScene:create(param)
    assert(type(param) == "table","param type err")
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
    --1. (Model) init battle data configure
    local battleConfig = {
        teamsL = {
        --myself first team
        	{
        		{npcId=10000, npcLevel=1, normalAttackVal=200, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=20000, currHp=12000,memberId=1},
                {npcId=10002, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=1000, currHp=1000,memberId=2},
                {npcId=10003, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=1000, currHp=1000,memberId=3},
                {npcId=10004, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=1000, currHp=1000,memberId=4},
                {npcId=10005, npcLevel=1, normalAttackVal=1, smallSkillLevel=1, smallSkillAttackVal=1, bigSkillLevel=1, bigSkillAttackVal=1, armorVal=20, maxHp=1000, currHp=1000,memberId=5},             
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
        	}
        },
        fId = nil,--场次Id
        bgID = 5,
        opMode = 2
    }
    
    --[[
    --reset book data
    if  #param.teamL ~=0 then
        local teamLtable = {}
        table.insert(teamLtable,param.teamL)

        battleConfig.teamsL=teamLtable
        battleConfig.teamsR=param.teamR
        battleConfig.fId = param.fId
    end
    
    --agent data
    local agentData = {bookAgentParam ={},
                       actorAgentParam ={memberList = {}},
                       directorAgentParam ={}                     
                        }
    local memberList ={}
    local tempTeamsL = battleConfig.teamsL[1]
    for i=1,#tempTeamsL do
        local memberId = tempTeamsL[i]["memberId"]
        if  memberId == nil then return end
        table.insert(memberList,memberId)
    end
    agentData.actorAgentParam.memberList = memberList
    agentData.actorAgentParam.fId = battleConfig.fId
    ]]--
    
    bookAgent = (require "scene.battle2.BookAgent"):create()
    directorAgent = (require "scene.battle2.DirectorAgent"):create()
    actorAgent = (require "ActorAgent"):create()
    
    --1. (Model) init battle data configure
    bookAgent:initBook(battleConfig)
    
    --2. (View) init battle scene layers
    --self:addChild(actorAgent)
    
    --3. (Controller) init battle controller
    directorAgent:start()
end

function BattleScene:loop()
	while true do
		if actorAgent:updateM() == true then
			break
		end
		directorAgent:updateM(1/30)
	end
end

local battleScene = BattleScene:create({})
local s = os.clock()
battleScene:loop()
local e = os.clock()
print("used time: "..e-s.." seconds")



