require("Cocos2d")

local NpcBook = require("scene.battle2.books.NpcBook")

local BookAgent = class("BookAgent", function()
    return {}
end)

function BookAgent:create()
    return BookAgent.new()
end

function BookAgent:assignKey()
    self.npcCount = self.npcCount or 1000
    local ret = self.npcCount
    self.npcCount = self.npcCount + 1
    return ret
end

function BookAgent:initBook(config)
    local teamDataListL = config.teamsL
    local teamDataListR= config.teamsR
    local towerNpcData = config.towerNpcData

    local function createTeamBookList(teamDataList, direction)
        if teamDataList==nil then return end
        local teamBookList = {}
        for i=1, #teamDataList do
            local teamData = teamDataList[i]
            local teamBook = {}
            for j=1, #teamData do
                local npcData = teamData[j]
                npcData.key = self:assignKey()
                npcData.direction = direction
                local npcBook = NpcBook:create(npcData)
                teamBook[j] = npcBook
            end
            table.sort(teamBook, function(a,b) return a.attackRange < b.attackRange end)
            teamBookList[i] = teamBook
        end
        return teamBookList
    end
    
    self.teamBookListL = createTeamBookList(teamDataListL, 1)
    self.teamBookListR = createTeamBookList(teamDataListR, -1)
    self.towerNpcBooks = createTeamBookList(towerNpcData,-1)
end

function BookAgent:copyNpcBook(npcId)
    local retNpcBook = {}
    for i=1, #self.towerNpcBooks[1] do
        local npcBook = self.towerNpcBooks[1][i]
        if npcBook.npcId == npcId then
            for k, v in pairs(npcBook) do
                retNpcBook[k] = v
            end
            retNpcBook.key = self:assignKey()
            return retNpcBook
        end
    end
    return nil
end

function BookAgent:getLeftTeamBook()
    self.indexL = self.indexL or 1
    self.currTeamBookL = self.teamBookListL[self.indexL]
    self.indexL = self.indexL + 1
    return self.currTeamBookL
end

function BookAgent:getRightTeamBook()
    self.indexR = self.indexR or 1
    self.currTeamBookR = self.teamBookListR[self.indexR]
    self.indexR = self.indexR + 1
    return self.currTeamBookR
end

function BookAgent:getNpcBook(key)
    for i=1, #self.currTeamBookL do
        if self.currTeamBookL[i].key == key then
            return self.currTeamBookL[i]
        end
    end
    for i=1, #self.currTeamBookR do
        if self.currTeamBookR[i].key == key then
            return self.currTeamBookR[i]
        end
    end
    return nil
end

return BookAgent
