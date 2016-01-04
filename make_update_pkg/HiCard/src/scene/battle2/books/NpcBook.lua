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
    if self["attackInterval"] and self["smallSkillInterval"] and self["bigSkillInterval"] then
        self["attackInterval"] = self["attackInterval"]*1.0/1000
        self["smallSkillInterval"] = self["smallSkillInterval"]*self["attackInterval"]
        self["bigSkillInterval"] = self["bigSkillInterval"]*self["attackInterval"]
    end
    
    --select define_npcs_info table with npcId
    local define_npcs_info = require "config.define_npcs_info"
    local npcInfo = define_npcs_info[tostring(self.npcId)]
    for k, v in pairs(npcInfo) do
        if self[k] == nil then
            self[k] = v
        end
    end
    
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

