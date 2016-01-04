require("Cocos2d")

local NpcDirector = require "scene.battle.directors.NpcDirector"

CELL_WIDTH  = (960 / 12) * (WIN_SIZE.width/960)
CELL_HEIGHT = (240 / 5) * (WIN_SIZE.height/640)
local CAL_COL_CONFIG = {
    --[1] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    --[2] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4}
    [1] = {[1]=3, [2]=4, [3]=2, [4]=5, [5]=1},
    [2] = {[1]=4, [2]=3, [3]=5, [4]=2, [5]=1}
}

local ROW_COL_CONFIG = {
    [1] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    [2] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    
    [3] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    [4] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    
    [5] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    [6] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    
    [7] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    [8] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    
    [9] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    [10] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    
    [11] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    [12] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    
    [13] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    [14] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    
    [15] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    [16] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    
    [17] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    [18] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    
    [19] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    [20] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    
    [21] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    [22] = {[1]=3.5, [2]=2.5, [3]=2, [4]=3, [5]=4},
    
    [23] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
    [24] = {[1]=2.5, [2]=3.5, [3]=3, [4]=2, [5]=4},
}

local DirectorAgent = class("DirectorAgent", function()
    return {}
end)

function DirectorAgent:create()
    local directorAgent = DirectorAgent.new()
    directorAgent:init()
    return directorAgent
end

function DirectorAgent:init()
    local listener = cc.EventListenerCustom:create("EVENT_GLOBAL_SKILL",function(event)
        self:handleGlobalSkill(event.data)
    end)
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithFixedPriority(listener, 1)
    
    self.leftAtkOk = false
    self.rightAtkOk = false
end

function DirectorAgent:updateM()
    for _, npcDirector in pairs(self.npcDirectors) do
        if npcDirector then
            npcDirector:updateM()
        end
    end
end

function DirectorAgent:reset()
    math.randomseed(os.time())
    self.leftAtkOk = false
    self.rightAtkOk = false
    
    self.round = self.round or 0
    self.round = self.round + 1

    self.currTeamLBook = self.currTeamLBook or BOOK_AGENT:get_teamLBook()
    self.currTeamRBook = BOOK_AGENT:get_teamRBook()
    
    if self.round > 3 or (#self.currTeamLBook:getNpcBooks())==0 or (#self.currTeamRBook:getNpcBooks())==0 then 
        self.winner = 1
        self:handleBattleOver()
        return
    end
    
    self:resetPosition()
    self:resetDirectors()

    --printJSON(self.matrix)
    ACTOR_AGENT:reset({
        teamLBook = self.currTeamLBook,
        teamRBook = self.currTeamRBook,
        bgID=5
    })
    
    ACTOR_AGENT:updateUI({round=self.round})
end

function DirectorAgent:resetDirectors()
    self.npcDirectors = {}
    for key, npcBook in pairs(self.currTeamLBook:getNpcBooks()) do
        if npcBook then
            self.npcDirectors[npcBook.key] = NpcDirector:create({npcBook=npcBook})
        end
    end
    
    for key, npcBook in pairs(self.currTeamRBook:getNpcBooks()) do
        if npcBook then
            self.npcDirectors[npcBook.key] = NpcDirector:create({npcBook=npcBook})
        end
    end
end

--[[
the whole row number is 18, the total column number is 5
1  - 6   is left invisible area,
7  - 12  is left visible area,
13 - 18  is right visible area,
19 - 24  is right invisible area,
so, 7-18 is screen visible area
-----------
When reset stand row, suppose left attack target row is 9, right attack target row is 24
]]--
function DirectorAgent:resetPosition()
    self.cells = {}
    local counter = 0

    for key, npcBook in pairs(self.currTeamLBook:getNpcBooks()) do
        if npcBook then
            local attactRange = npcBook.attackRange
            print("[attackRange]"..attactRange)
            
            counter = counter+1
            npcBook.row = 7 - attactRange
            npcBook.targetRow = 13 - attactRange
            npcBook.direction = 1
            self.cells[tostring(npcBook.row)] = self.cells[tostring(npcBook.row)] or {}
            npcBook.col = self:calColumn(npcBook.row)
            npcBook.targetCol = npcBook.col
            npcBook.zOrder = 10000-npcBook.col*100
            self.cells[tostring(npcBook.row)][tostring(npcBook.col)] = npcBook.key
            
            npcBook.row = 7 - counter
            --npcBook.col = counter%2 + 2
            npcBook.position = self:cell2Position({row=npcBook.row, col=npcBook.col})
        end
    end
    
    counter = 0
    for key, npcBook in pairs(self.currTeamRBook:getNpcBooks()) do
        if npcBook then
            counter = counter+1
            local attactRange = npcBook.attackRange
            npcBook.row = 18 + attactRange
            npcBook.targetRow = 12 + attactRange
            npcBook.direction = -1
            self.cells[tostring(npcBook.row)] = self.cells[tostring(npcBook.row)] or {}
            npcBook.col = self:calColumn(npcBook.row)
            npcBook.targetCol = npcBook.col
            npcBook.zOrder = 10000-npcBook.col*100
            self.cells[tostring(npcBook.row)][tostring(npcBook.col)] = npcBook.key
            npcBook.row = 18 + counter
            --npcBook.col = counter%2 + 2
            npcBook.position = self:cell2Position({row=npcBook.row, col=npcBook.col})
        end
    end
    
    printJSON(self.cells)
end

--[[]]--
function DirectorAgent:cell2Position(cell)
    local x = (cell.row-6-0.6) * CELL_WIDTH
    x = x + (cell.col-1) * 0.3 * CELL_WIDTH
    local y = (cell.col-0.5) * CELL_HEIGHT + 180
    
    return {x=x, y=y}
end

function DirectorAgent:calColumn(row)
    --[[
    local col = 0
    local n = table.nums(self.cells[tostring(row)])
    
    if (row/2) % 2 == 1 then
        if row < 13 then
            col = CAL_COL_CONFIG[1][n+1]
        else
            col = CAL_COL_CONFIG[2][n+1]
        end
    else
        if row < 13 then
            col = CAL_COL_CONFIG[2][n+1]
        else
            col = CAL_COL_CONFIG[1][n+1]
        end
    end
    
    return col
    ]]--
    local n = table.nums(self.cells[tostring(row)])
    return ROW_COL_CONFIG[row][n+1]
    
end

function DirectorAgent:getNearestAttackNpc(npcDirector)
    local myNpcBook = npcDirector.npcBook
    local nearestNpcBook = {}
    local distance = 9999999
    local function calDistance(npcBook1, npcBook2)
        local rowDistance = npcBook1.row - npcBook2.targetRow
        local colDistance = npcBook1.col - npcBook2.targetCol
        return rowDistance*rowDistance + colDistance*colDistance*0.5
    end
    local enemyNpcBooks = {}
    if myNpcBook.direction == 1 then
        enemyNpcBooks = self.currTeamRBook:getNpcBooks()
    else
        enemyNpcBooks = self.currTeamLBook:getNpcBooks()
    end
    
    for key, npcBook in pairs(enemyNpcBooks) do 
        if npcBook then
            local tmpDistance = calDistance(myNpcBook, npcBook)
            if tmpDistance < distance then
                distance = tmpDistance
                nearestNpcBook = npcBook
            end
        end
    end
    
    if nearestNpcBook == {} then
        self:handleBattleOver()
    end
    
    return self.npcDirectors[nearestNpcBook.key]
end

function DirectorAgent:removeNpcDirector(key)
    local director = self.npcDirectors[key]
    if director.npcBook.direction == 1 then
        self.currTeamLBook:removeNpcBook(key)
    else
        self.currTeamRBook:removeNpcBook(key)
    end
    
    self.npcDirectors[key] = nil
    
    --check if battle over ?
    self:checkBattleOver()
end

function DirectorAgent:checkBattleOver()
    local leftNpcNum = #self.currTeamLBook:getNpcBooks()
    local rightNpcNum = #self.currTeamRBook:getNpcBooks()

    for key, director in pairs(self.npcDirectors) do
        if director and director.npcBook.currHp <= 0 then
            ACTOR_AGENT:actCMD({cmdId = "die", npcKey = director.key})
        end
    end
    
    if leftNpcNum == 0 then
        self.winner = -1
        self:handleBattleOver()
        return
    end
    
    if rightNpcNum == 0 then
        --self:reset()
        for key, director in pairs(self.npcDirectors) do
            if director and director.npcBook.direction == -1 then
                ACTOR_AGENT:actCMD({cmdId = "die", npcKey = director.key})
            end
        end
        self.npcDirectors = {}
        ACTOR_AGENT:delayAndReset()
    end
end

function DirectorAgent:handleBattleOver()
    cclog("[DirectorAgent] battle is over!")
    
    self.npcDirectors = {}
    ACTOR_AGENT:onBattleOver()
end

function DirectorAgent:subHp(key, subHpVal)
    local director = self.npcDirectors[key]
    if director then
        director:subHp(subHpVal)
        if director:getHp() <= 0 then
            ACTOR_AGENT:actCMD({cmdId = "die", npcKey = director.key})
            cclog("[DirectorAgent] die "..director.key)
            self:removeNpcDirector(director.key)
        end
    end
end

function DirectorAgent:getNameHelper(npcKey)
    local director = self.npcDirectors[npcKey]
    if director then
        return director.npcBook.name
    end
    return ""
end

function DirectorAgent:getDirectionHelper(npcKey)
    local director = self.npcDirectors[npcKey]
    if director then
        return director.npcBook.direction
    end
    return 0
end

function DirectorAgent:handleGlobalSkill(data)
    print("[DirectorAgent]event.data:"..data.skillid)
    for _, npcDirector in pairs(self.npcDirectors) do
        if npcDirector then
            npcDirector:setGlobalSkillId(data.skillid)
        end
    end
    
    ACTOR_AGENT:actCMD({
        cmdId="globalSkillAttack", 
        level=data.skillid,
        subHpVal = 500,
        duration = 5
    })
end

function DirectorAgent:onGlobalSkill(param)
    for _, npcDirector in pairs(self.npcDirectors) do
        if npcDirector and npcDirector.npcBook.direction == -1 then
            self:subHp(npcDirector.key, param.subHpVal)
        end
    end
end

function DirectorAgent:onNormalAttackSuccess(param)
    local director = self.npcDirectors[param.npcKey]
    self:subHp(param.npcKey, param.subHpVal)
    
    if director.npcBook.direction == 1 then
        self.leftAtkOk = true
    else
        self.rightAtkOk = true
    end
    if self.leftAtkOk == true and self.rightAtkOk == true then
        ACTOR_AGENT:startJX()
    end
end

function DirectorAgent:onSmallSkillAttackSuccess(param)
    self:subHp(param.nearestAttackNpc, param.subHpVal)
    local director = self.npcDirectors[param.npcKey]


end

function DirectorAgent:onBigSkillAttackSuccess(param)
    local name = self:getNameHelper(param.npcKey)
    local direction = self:getDirectionHelper(param.npcKey)
    if direction == 0 or name == "" then return end
    
    if name == "Captain" then
        self:bigSkillEffect_Captain({direction=direction, subHpVal = param.subHpVal})
    elseif name == "FengXing" then
        self:bigSkillEffect_FengXing({direction=direction, subHpVal = param.subHpVal})
    else
        self:subHp(param.nearestAttackNpc, param.subHpVal)
    end
end

function DirectorAgent:bigSkillEffect_Captain(param)
    for _, npcDirector in pairs(self.npcDirectors) do
        if npcDirector and -param.direction == self:getDirectionHelper(npcDirector.key) then
            npcDirector:setDizzyTimer(2)
            self:subHp(npcDirector.key, param.subHpVal)
        end
    end
end

function DirectorAgent:bigSkillEffect_FengXing(param)
    for _, npcDirector in pairs(self.npcDirectors) do
        if npcDirector and -param.direction == self:getDirectionHelper(npcDirector.key) then
            npcDirector:fallBack(2)
            self:subHp(npcDirector.key, param.subHpVal)
        end
    end
end

function DirectorAgent:fireCMD(CMD)
    ACTOR_AGENT:actCMD(CMD)
end

function DirectorAgent:getWinner()
    return self.winner
end

return DirectorAgent

