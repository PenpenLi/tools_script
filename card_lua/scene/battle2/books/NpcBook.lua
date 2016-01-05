--[[
NpcBook describes NpcActor, NpcBook is NpcActor's data.
]]--
require("Cocos2d")

local NpcBook = class("NpcBook", function() return {} end)

function NpcBook:create(npcData)
    local npcBook = NpcBook.new()
    npcBook:init(npcData)
    return npcBook
end

--[[
Notice that npcId is not unique in team,npcId is just used to select config table, key is unique!!
]]--
function NpcBook:init(npcData)
    for k, v in pairs(npcData) do
        self[k] = v
    end
    --[[
    if self["attackInterval"] and self["smallSkillInterval"] and self["bigSkillInterval"] then
        self["attackInterval"] = math.floor(self["attackInterval"]*1.0/1000)
        self["smallSkillInterval"] = self["smallSkillInterval"]*self["attackInterval"]
        self["bigSkillInterval"] = self["bigSkillInterval"]*self["attackInterval"]
    end
    ]]
    self["attackInterval"] = nil
    self["smallSkillInterval"] = nil
    self["bigSkillInterval"] = nil
    
    local define_soul = require "config.define_soul"
    local soul = define_soul[tostring(self["soulId"])]
    self["attackInterval"] = soul["attackInterval"]/1000
    self["smallSkillInterval"] = soul["smallSkillInterval"]*self["attackInterval"]
    self["bigSkillInterval"] = soul["bigSkillInterval"]*self["attackInterval"]
    
    --select define_npcs_info table with npcId
    local define_npcs_info = require "config.define_npcs_info"
    local npcInfo = define_npcs_info[tostring(self.npcId)]
    for k, v in pairs(npcInfo) do
        if self[k] == nil then
            self[k] = v
        end
        if k=="attackInterval" and v > 9999 then
            self["attackInterval"] = v
        end
        if k=="smallSkillInterval" and v > 1000 then
            self["smallSkillInterval"] = v
        end
        if k=="bigSkillInterval" and v > 1000 then
            self["bigSkillInterval"] = v
        end
    end
    
    print("-----------------"..self.npcId.."-----------------")
    print(self["attackInterval"])
    print(self["smallSkillInterval"])
    print(self["bigSkillInterval"])
    --print(self["persistentBigSkillFramesNum"])
    --print(self["persistentBigSkillCount"])
    --[[
    local godHpFactor = 1
    if (guardCenter:canEnterGuardMode("pve") == true or
        guardCenter:canEnterGuardMode("pve2") == true or
        guardCenter:canEnterGuardMode("pve3") == true or
        guardCenter:canEnterGuardMode("pve4") == true) and self.direction==1 then
        godHpFactor = 10
    end
    self.normalAttackVal = self.normalAttackVal*godHpFactor
    ]]

    self["debug_name"] = self.name.."_"..self.key
    --select define_npc_(npcName) table with npcLevel
    --[[
    local define_npc_info = require("config.define_npc_"..self["name"])
    local npcLevelInfo = define_npc_info[tostring(npcData.npcLevel)]
    for k, v in pairs(npcLevelInfo) do
        self[k] = v
    end
    ]]--
end

return NpcBook

