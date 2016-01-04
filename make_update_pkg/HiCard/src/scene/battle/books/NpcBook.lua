--[[
NpcBook describes NpcActor, NpcBook is NpcActor's data.
]]--
require("Cocos2d")

local NpcBook = class("NpcBook", function()
    return {}
end)

--[[ npcData = {npcId=xxx, npcLevel=xxx} ]]--
function NpcBook:create(npcData)
    local npcBook = NpcBook.new()
    npcBook:init(npcData)
    return npcBook
end

--[[
Notice that npcId is not unique in team,npcId is just used to select config table, key is unique!!
NpcBook:{
    key = 1
    npcId=10000,
    position={x=100,y=100}, 
    scaleFactor=0.5, 
    armatureName="lina"
}
]]--
function NpcBook:init(npcData)
    for k, v in pairs(npcData) do
        self[k] = v
    end
    self.position = {x=100,y=100}
    
    --search define_npcs_info table with npcId
    local define_npcs_info = require "config.define_npcs_info"
    local npcInfo = define_npcs_info[tostring(self.npcId)]
    for k, v in pairs(npcInfo) do
        self[k] = v
    end
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

