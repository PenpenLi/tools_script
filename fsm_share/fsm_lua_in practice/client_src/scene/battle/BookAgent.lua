require("Cocos2d")

local TeamBook = require "scene.battle.books.TeamBook"

local BookAgent = class("BookAgent", function()
    return {}
end)

function BookAgent:create()
    return BookAgent.new()
end

--[[teams = {team, team, xxx}, team = {npc, npc, xxx} ]]--
function BookAgent:set_teamsLBook(teamsLData)
    self.teamsLData = teamsLData
    self.teamsLBook = self:convertToTeamsBook(teamsLData)
end

function BookAgent:set_teamsRBook(teamsRData)
    self.teamsRData = teamsRData
    self.teamsRBook = self:convertToTeamsBook(teamsRData)
end

function BookAgent:convertToTeamsBook(teamsData)
    local teamsData = teamsData
    local teamsBook = {}
   
    for i=1, #teamsData do
        local teamData = teamsData[i]
        local teamBook = TeamBook:create(teamData)
        teamsBook[i] = teamBook
    end
    return teamsBook
end

function BookAgent:assignNpcKey(npcId)
    self.keyCount = self.keyCount or 1
    local ret = tostring(npcId).."_"..tostring(self.keyCount)
    self.keyCount = self.keyCount + 1
    return ret
end

function BookAgent:get_teamLBook()
    self.currTeamLIndex = self.currTeamLIndex or 1
    local teamBook = self.teamsLBook[self.currTeamLIndex]
    self.currTeamLIndex = self.currTeamLIndex + 1
    return teamBook
end

function BookAgent:get_teamRBook()
    self.currTeamRIndex = self.currTeamRIndex or 1
    local teamBook = self.teamsRBook[self.currTeamRIndex]
    self.currTeamRIndex = self.currTeamRIndex + 1
    return teamBook
end

function BookAgent:set_opMode(opMode) self.opMode = opMode end
function BookAgent:get_opMode() return self.opMode end
function BookAgent:set_bgID(bgID) self.bgID = bgID end
function BookAgent:get_bgID() return self.bgID end

return BookAgent
