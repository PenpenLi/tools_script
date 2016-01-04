--[[
TeamBook is set for NpcBook
]]--
require("Cocos2d")

local NpcBook = require "scene.battle.books.NpcBook"

local TeamBook = class("TeamBook", function()
    return {}
end)

function TeamBook:create(teamData)
    local teamBook = TeamBook.new()
    teamBook:init(teamData)
    return teamBook
end

--[[ teamData = {npcData, npcdata, npcData, xxx]]--
function TeamBook:init(teamData)
    self.npcBooks = {}
    for j=1, #teamData do
        local npcData = teamData[j]
        --[[assign one key for each Npc, Notice that npcId is not unique in team
            npcId is just used to select config table
        ]]-- 
        npcData.key = BOOK_AGENT:assignNpcKey(npcData.npcId)
        --self.npcBooks[npcData.key] = NpcBook:create(npcData)
        local npcBook = NpcBook:create(npcData)
        table.insert(self.npcBooks, npcBook)
    end
    table.sort(self.npcBooks, function(a, b) return a.attackRange < b.attackRange end)
end

function TeamBook:getNpcBooks()
    return self.npcBooks
end

function TeamBook:getNpcBook(key)
    for i=1, #self.npcBooks do
        if self.npcBooks[i].key == key then
            return self.npcBooks[i]
        end
    end
    return nil
end

function TeamBook:removeNpcBook(key)
    for i=1, #self.npcBooks do
        if self.npcBooks[i] and self.npcBooks[i].key == key then
            self.npcBooks[i] = nil
            table.remove(self.npcBooks, i)
            return
        end
    end
end

return TeamBook


