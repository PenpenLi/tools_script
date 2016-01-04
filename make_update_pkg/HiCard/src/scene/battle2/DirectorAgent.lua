require("Cocos2d")

local NpcDirector = require "scene.battle2.directors.NpcDirector"
local DirectorAgent = class("DirectorAgent", function()
    return {}
end)

local cells_max_row, cells_max_col = 24, 10
--cell_width  = (960/(cells_max_row/2)) * (WIN_SIZE.width/960)
--cell_height = (340/(cells_max_col)) * (WIN_SIZE.height/640)
cell_width  = (960/(cells_max_row/2))
cell_height = (440/(cells_max_col))
local height_offset = 180
-----------------------[[External API]]-----------------------------
function DirectorAgent:create()
    local directorAgent = DirectorAgent.new()
    directorAgent:initM()
    return directorAgent
end

function DirectorAgent:start()
    self:restart()
end

function DirectorAgent:updateM(dt)
    if self.restartTimer and self.restartTimer > 0 then
        self.restartTimer = self.restartTimer - 1
        if self.restartTimer <= 0 then
            self.restartTimer = nil
            self:restart()
        end
    end
    self:updateTimerList()
    
    if self.isPause then return end
    self:clearFrameData()
    for i=1, #self.directors do
        if self.directors[i] then
            self.directors[i]:updateM(dt)
        end
    end
end

function DirectorAgent:assignCol(key, row)
    assert(row<=cells_max_row,"")
    local assign_col_order1 = {2, 4, 3, 1, 5, 2, 4, 3, 1, 5, 2}
    local assign_col_order2 = {4, 2, 3, 1, 5, 4, 2, 3, 1, 5, 4}
    local function calColNum(row)
        local num = 0
        for i=1, #self.cells[row] do
            if self.cells[row][i] ~= 0 then
                num = num + 1
            end
        end
        return num
    end
    local num = calColNum(row)
    
    local col = 0
    if ((row-1)/2)%2 == 0 then
        col = assign_col_order1[num+1]
    else
        col = assign_col_order2[num+1]
    end
    self.cells[row][col] = key
    
    return col
end

function DirectorAgent:updateCell(key, from, to)
    --assert(self.cells[from.row][from.col] == key, "cells err")
    self.cells[from.row][from.col] = 0
    self.cells[to.row][to.col] = key
end

function DirectorAgent:clearCell(director)
    self.cells[director.row][director.col] = 0
end

function DirectorAgent:calPosition(row, col)
    local x = (row-6-0.5) * cell_width
    x = x + (col-1) * 0.2 * cell_width
    local y = (col-0.5) * cell_height + height_offset
    assert((self:calCell(x,y)).row==row,"calPosition err")
    assert((self:calCell(x,y)).col==col,"calPosition err")
    return {x=x, y=y}
end

function DirectorAgent:calCell(x, y)
    local col = math.ceil((y-height_offset)/cell_height)
    x = x - (col-1) * 0.2 * cell_width
    local row = math.ceil(x*1.0/cell_width)+6
    return {row=row, col=col}
end

function DirectorAgent:getAttackOrder(director)
    local targetDirector = director.attackTarget
    local tmpDirectors = {}
    local leftRangeOneNum, rightRangeOneNum = 0, 0
    for i=1, #self.directors do
        if self.directors[i].npcBook.attackRange == 1 then
            if self.directors[i].direction == 1 then
                leftRangeOneNum = leftRangeOneNum + 1
            else
                rightRangeOneNum = rightRangeOneNum + 1
            end
        end
        if self.directors[i].attackTarget then
            if self.directors[i].attackTarget == targetDirector then
                table.insert(tmpDirectors, self.directors[i])
            end
        end
    end
    table.sort(tmpDirectors,function(a, b) return a.position.y < b.position.y end)
    
    local zOrderList = {}
    local idx = 0
    for i=1, #tmpDirectors do
        zOrderList[tostring(i)] = {k=tmpDirectors[i].key, z=targetDirector.zOrder-i}
        --table.insert(zOrderList, {k=tmpDirectors[i].key, z=targetDirector.zOrder-i})
        if tmpDirectors[i].key == director.key then
            idx = i
        end
    end

    local moveYDirection = 1
    if leftRangeOneNum < rightRangeOneNum then
        moveYDirection = -1
    end
    return {attackOrder=math.floor(idx-#tmpDirectors/2), moveYDirection = moveYDirection, zOrderList=zOrderList}
end

--------------------walk related----------------------
function DirectorAgent:isRightTeamMove()
    local maxX = 0
    local directors = self:getDirectors(1)
    for i=1, #directors do
        if directors[i].position.x > maxX then
            maxX = directors[i].position.x
        end
    end
    if maxX > WIN_SIZE.width/2 then
        return true
    else
        return false
    end
end

--[[Only can be called one time each frame]]--
function DirectorAgent:rightTeamMove(shiftX, shiftY)
    if self.isRightMove == true then
        return
    end
    self.isRightMove = true
    
    local directors = self:getDirectors(-1)
    for i=1, #directors do
        if directors[i]:isPause() == false then
            directors[i]:move(shiftX, shiftY)
        end
    end
    
    local directors = self:getDirectors(1)
    for i=1, #directors do
        if directors[i]:isWalking() == false then
            directors[i]:move(shiftX, shiftY)
        end
    end
    
    --move background and die hero
    actorAgent:actCMD({t="move", key=99999, dx=shiftX, dy=shiftY})
    for i=1, #self.dieKeys do
        actorAgent:actCMD({t="move", key=self.dieKeys[i], dx=shiftX, dy=shiftY})
    end
end

--------------------noraml attack----------------------
function DirectorAgent:getTargetDirector(key)
    local targetDirector = nil
    local director = self:getDirector(key)
    if director == nil then return nil end
    local targetDirectors = self:getDirectors(director.direction*-1)

    local function calDistance(director, targetDirector)
        local rowDistance = director.row - targetDirector.row
        local colDistance = director.col - targetDirector.col
        return rowDistance*rowDistance + colDistance*colDistance*0.5
    end

    local distance = 9999999
    for i=1, #targetDirectors do
        local tmpDistance = calDistance(director, targetDirectors[i])
        if tmpDistance < distance then
            distance = tmpDistance
            targetDirector = targetDirectors[i]
            assert(targetDirector ~= nil, "")
        end
    end

    return targetDirector
end

function DirectorAgent:onAttack(fromKey, targetKey, hp)
    local target = self:getDirector(targetKey)
    if target then
        target:onAttack(hp)
    end
end

function DirectorAgent:onAttack_LaGeNaLuoSi(fromKey, excludeKey, hp)
    local from = self:getDirector(fromKey)
    for i=1, #self.directors do
        local tmpDirector = self.directors[i]
        if tmpDirector.direction ~= from.direction and tmpDirector.key ~= excludeKey then
            tmpDirector:onAttack(hp)
        end
    end
end

--------------------small skill------------------------
function DirectorAgent:getSmallSkillTargets(key)
    local smallSkillTargets = {}
    local target = self:getTargetDirector(key)
    local director = self:getDirector(key)
    if director.npcBook.name == "Tyrande" or director.npcBook.name == "Jaina" or
        director.npcBook.name == "ShahelasiZhuMu" or director.npcBook.name == "KeLuoMaGuSi" then
        if director.npcBook.name == "Tyrande" then
            local teammates = self:getDirectors(director.direction)
            local j = nil
            local hp = math.pow(2,32)-1
            for i=1,#teammates do
                if  teammates[i].npcBook.currHp < hp then
                    hp = teammates[i].npcBook.currHp
                    j = i
                end
            end
            table.insert(smallSkillTargets, teammates[j])
        else
            smallSkillTargets = self:getDirectors(director.direction*-1)
        end
    elseif director.npcBook.name == "YiLiDan" then
        smallSkillTargets = self:getAttackRangeTarget(key,3,5)
    else
       table.insert(smallSkillTargets, target)
    end
    return smallSkillTargets
end

function DirectorAgent:onSmallSkill(fromKey, targetKey, hp)
    local target = self:getDirector(targetKey)
    if target == nil then return end
    target:onSmallSkill(fromKey, hp)
end

--------------------big skill------------------------
function DirectorAgent:getBigSkillTargets(key)
    local bigSkillTargets = {}
    local director = self:getDirector(key)
    if director.npcBook.name == "FengXing" or director.npcBook.name == "Captain" or
        director.npcBook.name == "XiaoHei" or director.npcBook.name == "Tyrande" or
        director.npcBook.name == "DiXueLingZhu" or director.npcBook.name == "ShahelasiZhuMu" or
        director.npcBook.name == "KeLuoMaGuSi" then
        if  director.npcBook.name == "Tyrande" then
            bigSkillTargets = self:getDirectors(director.direction)
        else
            bigSkillTargets = self:getDirectors(director.direction*-1)
        end
    elseif director.npcBook.name == "YiLiDan" then
        bigSkillTargets = self:getAttackRangeTarget(key,5,4)
    elseif director.npcBook.name == "MaNuoLuoSi" then
        bigSkillTargets = self:getAttackRangeTarget(key,6,6)
    else
        table.insert(bigSkillTargets, self:getTargetDirector(key))
    end
    return bigSkillTargets
end

function DirectorAgent:onBigSkill(fromKey, targetKey, hp)
    local target = self:getDirector(targetKey)
    if target == nil then return end
    target:onBigSkill(fromKey, hp)
    
    local from = self:getDirector(fromKey)
    if from.npcBook.name =="Jaina"  then
        print("~~~~~~~~~~~~~addDirector~~~~~~~~~~~~~~~")
        self:removeDirector(20011)
        self:removeDirector(20011)
        local function calCellsNpcNum(row)
            local n = 0
            for i=1, #self.cells[row] do
                if self.cells[row][i] ~= 0 then
                    n = n + 1
                end
            end
            return n
        end
        local row = from.row
        if 0 == calCellsNpcNum(from.row+from.direction) then
            row = from.row+from.direction
        end
        local brotherNpcBook1 = bookAgent:copyNpcBook(20011)
        brotherNpcBook1.direction = from.direction
        brotherNpcBook1.row = from.row
        brotherNpcBook1.col = from.col + 1
        self:addDirector(brotherNpcBook1)
        local brotherNpcBook2 = bookAgent:copyNpcBook(20011)
        brotherNpcBook2.direction = from.direction
        brotherNpcBook2.row = from.row
        brotherNpcBook2.col = from.col - 1
        self:addDirector(brotherNpcBook2)
        
    elseif from.npcBook.name =="tonglingta" then
        print("~~~~~~~~~~~~~addDirector~~~~~~~~~~~~~~~")
        local towerNpcBook = bookAgent:copyNpcBook(20010)
        towerNpcBook.direction = from.direction
        towerNpcBook.row = from.row
        towerNpcBook.col = from.col - 1
        self:addDirector(towerNpcBook)
    end
end

function DirectorAgent:onPersistentSkill(fromKey,targetKey,hp)
    local target = self:getDirector(targetKey)
    if target == nil then return end
    target:onPersistentSkill(fromKey, hp)
end

--------------------global skill------------------------
function DirectorAgent:handleGlobalSkill(data)
    --[[
    --print("[DirectorAgent]event.data:"..data.skillid)
    local targetDirectors = self:getGlobaSkillTargets()
    for i=1,#targetDirectors do
        targetDirectors[i]:handleGlobalSkill(data.skillid,data.hp,data.dt,data.seq)
    end
    ]]
end

function DirectorAgent:getGlobaSkillTargets()
    return self.directors
end

function DirectorAgent:onGlobalSkill(fromKey,hp)
    local target = self:getDirector(fromKey)
    if target == nil then return end
    if  target.direction == -1 then
        target:onGlobalSkill(hp)
    end
end

--------------------die------------------------
function DirectorAgent:onDie(key)
    actorAgent:actCMD({t="die", key=key})
    local director = self:getDirector(key)
    if director.npcBook.name == "mutong" then
        for i=1, #self.directors do
            if self.directors[i].direction == 1 then
                self.directors[i]:onAddHp(key, 100)
            end
        end
    end
    self:clearCell(director)
    local idx = 0
    for i=1, #self.directors do
        idx = idx + 1
        if self.directors[i].key == key then
            break
        end
    end
    if idx ~= 0 then
        assert(director==self.directors[idx], "delete error")
        print("[DirectorAgent]die "..self.directors[idx].npcBook.name)
        table.insert(self.dieKeys, self.directors[idx].key)
        table.remove(self.directors, idx)
        self:checkOver()
    end
end

------------------------[[Internal function]]------------------------

--[[the whole row number is 18, the total column number is 5
1  - 6   is left invisible area,
7  - 12  is left visible area,
13 - 18  is right visible area,
19 - 24  is right invisible area,
so, 7-18 is screen visible area
-----------
When reset stand row, suppose left attack target row is 13, right attack target row is 12
]]--
function DirectorAgent:initM()
    self.cells = {}
    for i=1, cells_max_row do
        self.cells[i] = {}
        for j=1, cells_max_col do
            self.cells[i][j] = 0
        end
    end
    self.timerList = {}
    --[[
    local listener = cc.EventListenerCustom:create("EVENT_GLOBAL_SKILL",function(event)
        self:handleGlobalSkill(event.data)
    end)
    local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
    eventDispatcher:addEventListenerWithFixedPriority(listener, 1)
    ]]
end

function DirectorAgent:restart()
    math.randomseed(os.time())
    self.round = self.round or 0
    self.round = self.round + 1
    self.dieKeys = {}
    self:resetDirectors()
    self.isPause = false
end

function DirectorAgent:resetDirectors()
    self.directors = self.directors or {}
    for i=1, #self.directors do
        self.directors[i]:reset()
    end
    
    local function addDirectors(teamBook)
        for i=1, #teamBook do
            local npcBook = teamBook[i]
            npcBook.enterOrder = i
            local npcDirector = NpcDirector:create(npcBook)
            table.insert(self.directors, npcDirector)
        end
    end
    
    if #self:getDirectors(1) == 0 then
        local leftTeamBook = bookAgent:getLeftTeamBook()
        addDirectors(leftTeamBook)
    end
    
    if #self:getDirectors(-1) == 0 then
        local rightTeamBook = bookAgent:getRightTeamBook()
        if rightTeamBook==nil then
            self:gameOver()
            return
        end
        addDirectors(rightTeamBook)
    end
end

function DirectorAgent:addDirector(npcBook)
    npcBook.enterOrder = #self.directors + 1
    local npcDirector = NpcDirector:create(npcBook)
    table.insert(self.directors, npcDirector)
end

function DirectorAgent:removeDirector(npcId)
    for i=1, #self.directors do
        if self.directors[i].npcBook.npcId == npcId then
            self:onDie(self.directors[i].key)
            break
        end
    end
end

function DirectorAgent:checkOver()
    if self.round > 3 or #self:getDirectors(1) == 0 then
        self:gameOver()
        return
    end
    
    if #self:getDirectors(-1) == 0 then
        self.isPause = true
        --self.restartTimer = 60
        self:setTimeout(60, function() self:restart() end)
        for i=1, #self.directors do
            self.directors[i]:reset()
        end
        --self:restart()
    end
end

function DirectorAgent:gameOver()
    print("[DirectorAgent]game is over")
    self.isPause = true
    local param = {}
    local aliveKey ={}
    local teamLs = self:getDirectors(1)
    --param.leftAtks = #teamLs
    --param.leftDefs = #self:getDirectors(-1)
    local leftAtks = {}
    local leftDefs = {}
    for i=1, #self.directors do     
        table.insert(aliveKey, self.directors[i].key)
        local director = self.directors[i]
        local battleSoulLeft = {}
        if director.direction == 1 then
            --battleSoulLeft.id = director.npcBook.soulId
            battleSoulLeft.pos = director.npcBook.pos
            battleSoulLeft.curHp = director.hp
            table.insert(leftAtks,battleSoulLeft)
        else
            --battleSoulLeft.id = director.npcBook.soulId
            battleSoulLeft.pos = director.npcBook.pos
            battleSoulLeft.curHp = director.hp
            table.insert(leftDefs,battleSoulLeft)
        end
    end
    param.aliveKey = aliveKey
    param.leftAtks = leftAtks
    param.leftDefs = leftDefs
    actorAgent:onBattleOver(param)
end

function DirectorAgent:getDirectors(direction)
    local directors = {}
    for i=1, #self.directors do
        if direction == self.directors[i].direction then
            table.insert(directors, self.directors[i])
        end
    end
    return directors
end

function DirectorAgent:getDirector(key)
    for i=1, #self.directors do
        if key == self.directors[i].key then
            return self.directors[i]
        end
    end
    return nil
end


function DirectorAgent:getAttackRangeTarget(fromKey,rangeRow,rangeCol)
    local targets = {}
    local director = self:getDirector(fromKey)
    local directorRow = director.row
    local directorCol = director.col
    local direction = director.direction
    for i=1, #self.directors do
        if self.directors[i].direction ~= direction then
            local tmpRow = self.directors[i].row
            local tmpCol = self.directors[i].col
            if math.abs(directorRow-tmpRow)<= rangeRow and math.abs(directorCol-tmpCol)<=rangeCol then
                table.insert(targets,self.directors[i])
            end
        end
    end
    return targets
end

function DirectorAgent:clearFrameData()
    self.isRightMove = false
end

function DirectorAgent:setTimeout(time, callback)
    table.insert(self.timerList, {t=time, cb=callback})
end

function DirectorAgent:updateTimerList()
    local idx = 0
    for i=1, #self.timerList do
        idx = idx + 1
        local timer = self.timerList[i]
        timer.t = timer.t - 1
        if timer.t <= 0 then
            timer.cb()
            table.remove(self.timerList, idx)
            return
        end
    end
end

return DirectorAgent


