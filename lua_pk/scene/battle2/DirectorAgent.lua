require("Cocos2d")

local NpcDirector = require "scene.battle2.directors.NpcDirector"
local DirectorAgent = class("DirectorAgent", function()
    return {}
end)

local cells_max_row, cells_max_col = 24, 8
cell_width  = (960/(cells_max_row/2))
cell_height = (440/(cells_max_col))
local height_offset = 130
local stand_celly_map = {{3}, {2,4}, {2,3,4}, {1.5,2.5,3.5,4.5}, {1,2,3,4,5}, {0.5,1.5,2.5,3.5,4.5,5.5}, {0.5,1.5,2.5,3.5,4.5,5.5, 6},
    {0.5,1.5,2.5,3.5,4.5,5.5,6,6.5}}

-----------------------[[External API]]-----------------------------
function DirectorAgent:create()
    local directorAgent = DirectorAgent.new()
    directorAgent:initM()
    return directorAgent
end

function DirectorAgent:start()
    actorAgent:actCMD({t="init_actor_agent", data=self.actorAgentData})
    self:setTimeout(self.gameOverTime*30, function() self:gameOver() end)
    self:restart()
end

function DirectorAgent:updateM(dt)
    self:updateTimerList()
    if self.isPause then return end
    self:clearFrameData()
    self:updateJXSkills()
    for i=1, #self.directors do
        if self.directors[i] then
            self.directors[i]:updateM(dt)
        end
    end
end

function DirectorAgent:assignTowerCell()
    local config = {{row=22,col=3},{row=24,col=2},{row=26,col=4},{row=28,col=2},{row=30,col=4}}
    self.towerNum = self.towerNum + 1
    if self.towerNum > #config then
        return {row=20+2*self.towerNum, col=4}
    end
    return config[self.towerNum]
end

function DirectorAgent:assignCol(key, row)
    --assert(row<=cells_max_row,"")
    row = math.min(row, cells_max_row)
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

function DirectorAgent:calEnterOrderNum(key)
end

function DirectorAgent:updateCell(key, from, to)
    --assert(self.cells[from.row][from.col] == key, "cells err")
    --self.cells[from.row][from.col] = 0
    --self.cells[to.row][to.col] = key
end

function DirectorAgent:clearCell(director)
    if self.cells[director.row] and self.cells[director.row][director.col] then
        self.cells[director.row][director.col] = 0
    end
end

function DirectorAgent:calPosition(row, col)
    local x = (row-6-0.5) * cell_width
    x = x + (col-1) * 0.2 * cell_width
    --local y = (col-0.5) * cell_height + height_offset
    local y = col*cell_height + height_offset
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

function DirectorAgent:pause()
    self.isPause = true
end

function DirectorAgent:resume()
    self.isPause = false
end

function DirectorAgent:suspendForBigSkill(fromDirector)
    --pve battle only left hero fire skill lead to black screen
    if self.fightMode ==1 and fromDirector.direction~=1 then return end
    local pauseDt = fromDirector:getBigSkillOverTime()+20
    for i=1, #self.directors do
        if self.directors[i] ~= fromDirector then
            self.directors[i]:setPauseType(7, {pauseDt=pauseDt})
        end
    end
end

function DirectorAgent:disableAutoFireBigSkill()
    self.isEnableAuto = false
end

function DirectorAgent:enableAutoFireBigSkill()
    self.isEnableAuto = true
end

function DirectorAgent:setAutoFight(isAuto)
    self.isAuto = isAuto
end

function DirectorAgent:isAutoFight()
    self.isEnableAuto = self.isEnableAuto or true
    return (self.isAuto or self.fightMode==2) and self.isEnableAuto
end

function DirectorAgent:setFightMode(mode)
    self.fightMode = mode
end

function DirectorAgent:setActorAgentData(agentData)
    self.actorAgentData = agentData
    self.gameOverTime = agentData.roundTime
    --self:addJXSkills(Helper.calJXSkills(agentData.memberList), 1)
end

function DirectorAgent:fireBigSkill(param)
    local soulId = param.id
    for i=1, #self.directors do
        if self.directors[i].npcBook.direction == 1 and self.directors[i].npcBook.soulId == soulId then
            self.directors[i]:fireBigSkill()
            return
        end
    end
end

function DirectorAgent:canFireSkill(soulId)
    if self.isPause then return false end
    return true
    --[[
    for i=1, #self.directors do
        if self.directors[i].npcBook.direction == 1 and self.directors[i].npcBook.soulId == soulId then
            if self.directors[i]:isWalking()==false then
                return true
            else
                return false
            end
        end
    end
    return false
    ]]
end

function DirectorAgent:addJXSkills(jxSkills, direction)
    self.jxSkills = self.jxSkills or {}
    for k, v in pairs(jxSkills) do
        v.timer = 0
        v.direction = direction
        self.jxSkills[v.type*direction] = v
    end
end

function DirectorAgent:updateJXSkills()
    for k, v in pairs(self.jxSkills) do
        if v then
            v.timer = v.timer + 1
            if v.timer == 30*v["cd"] and v.direction==1 then
                --this jx skill is ok
                print("JX "..v.type.." is ready")
                actorAgent:actCMD({t="jx", type=v.type, p=100, direction=1})
            elseif v.timer > 30*v["cd"] then
                if self.isAuto == true or 2==self.fightMode then
                    self:setTimeout(3, function() self:fireJXSkill({skillid=v.type, direction=v.direction}) end)
                end
            elseif (v.timer % 30 == 0) and v.direction==1 then
                actorAgent:actCMD({t="jx", type=v.type, direction=1, p=math.ceil(100*v.timer/(30*v["cd"]))})
            end
        end
    end
end

function DirectorAgent:fireJXSkill(data)
    self.jxSeq = self.jxSeq or 0
    self.jxSeq = self.jxSeq + 1
    local skillid = data.skillid
    local jx = self.jxSkills[skillid*data.direction]
    --print("[DirectorAgent]fire JX skill:")
    if not (jx and jx.timer >= 30*jx["cd"]) then return end
    jx.timer = 0 
    actorAgent:actCMD({t="jx", type=skillid, p=0, direction=data.direction})
    
    --Dian Ci Zhen Dang, Huo Pao Hong Zha
    if skillid == 1 or skillid == 2 then
        for i=1, #self.directors do
            if self.directors[i].direction == -1*data.direction then
                data.hp = jx["zhili"]
                data.seq = self.jxSeq
                data.dt = jx["dt"]/1000
                if skillid == 1 then
                    self.directors[i]:onCiBao(data)
                elseif skillid == 2 then
                    self.directors[i]:onHongZha(data)
                end
            end
        end
    --cibao_s  hongzha_s
    --Zhao Huan Jian Ta Che
    elseif skillid == 3 then
        --zhaohuanjianta_s
        print("~~~~~~~~~~~~~addDirector~~~~~~~~~~~~~~~")
        local level = 5
        local towerNpcBook = bookAgent:copyNpcBook(20005)
        towerNpcBook.npcLevel = level
        towerNpcBook.normalAttackVal = 20+level*21
        towerNpcBook.currHp = 200+level*74
        towerNpcBook.maxHp = towerNpcBook.currHp
        towerNpcBook.armorVal = 5+level*5
        towerNpcBook.attackInterval = 4
        towerNpcBook.dex = 10000
        towerNpcBook.block = 0
        towerNpcBook.direction = data.direction
        if towerNpcBook.direction==1 then
            towerNpcBook.row = 6+4
        else
            towerNpcBook.row = 18-4
        end
        
        towerNpcBook.col = 2
        actorAgent:actCMD({t="zhaohuanjianta_s", pos=self:calPosition(towerNpcBook.row, towerNpcBook.col)}) --send a wrong pos here
        self:addDirector(towerNpcBook)
        local director = self:getDirector(towerNpcBook.key)
        director:stop()
        self:setTimeout(60, function()
            director:restart()
        end)
        
        
    --Zhao Huan Mo Fa Ta
    elseif skillid == 4 then
        --zhaohuanmofata_s
        print("~~~~~~~~~~~~~addDirector~~~~~~~~~~~~~~~")
        local level = 5
        local towerNpcBook = bookAgent:copyNpcBook(20004)
        towerNpcBook.npcLevel = level
        towerNpcBook.normalAttackVal = 20+level*21
        towerNpcBook.currHp = 200+level*74
        towerNpcBook.maxHp = towerNpcBook.currHp
        towerNpcBook.armorVal = 5+level*5
        towerNpcBook.attackInterval = 4
        towerNpcBook.dex = 10000
        towerNpcBook.block = 0
        towerNpcBook.direction = data.direction
        if towerNpcBook.direction == 1 then
            towerNpcBook.row = 6+4
        else
            towerNpcBook.row = 18-4
        end
        towerNpcBook.col = 1
        actorAgent:actCMD({t="zhaohuanmofata_s", pos=self:calPosition(towerNpcBook.row, towerNpcBook.col)}) --send a wrong pos here
        self:addDirector(towerNpcBook)
        local director = self:getDirector(towerNpcBook.key)
        director:stop()
        self:setTimeout(60, function()
            director:restart()
        end)
        
    --Zhao Huan Yan Wu
    elseif skillid == 5 then
        for i=1, #self.directors do
            if self.directors[i].direction == -1*data.direction then
                self.directors[i]:onYanWu()
            end
        end
    --Zhao Huan Fei Ting
    elseif skillid == 6 then
        local enemys = self:getDirectors(-1*data.direction)
        local j = nil
        local hp = math.pow(2,32)-1
        for i=1,#enemys do
            if  enemys[i].npcBook.currHp < hp then
                hp = enemys[i].npcBook.currHp
                j = i
            end
        end
        enemys[j]:onZhaoHuanFeiTing()

    --Hui Zhi Zhao Huan Zhen
    elseif skillid == 7 then
        --zhaohuanfazhen_s
        print("~~~~~~~~~~~~~addDirector~~~~~~~~~~~~~~~")
        local level = 5
        local towerNpcBook = bookAgent:copyNpcBook(20017)
        towerNpcBook.npcLevel = level
        towerNpcBook.normalAttackVal = 20+level*21
        towerNpcBook.currHp = 200+level*74
        towerNpcBook.maxHp = towerNpcBook.currHp
        towerNpcBook.armorVal = 5+level*5
        --towerNpcBook.attackInterval = 2
        towerNpcBook.dex = 10000
        towerNpcBook.block = 0
        towerNpcBook.direction = data.direction
        if towerNpcBook.direction == 1 then
            towerNpcBook.row = 6+4
        else
            towerNpcBook.row = 18-4
        end
        towerNpcBook.col = self:assignCol(towerNpcBook.key, towerNpcBook.row)
        towerNpcBook.isCellAssign = true
        actorAgent:actCMD({t="zhaohuanfazhen_s", pos=self:calPosition(towerNpcBook.row, towerNpcBook.col)})
        self:addDirector(towerNpcBook)
        local director = self:getDirector(towerNpcBook.key)
        director:stop()
        self:setTimeout(60, function()
            director:restart()
        end)
    --GPS
    elseif skillid == 8 then
        for i=1, #self.directors do
            if self.directors[i].direction == 1*data.direction then
                self.directors[i]:onGPS()
            end
        end
    --Zhao Huan Tu Teng Zhu
    elseif skillid == 9 then
        for i=1, #self.directors do
            if self.directors[i].direction == 1 then
                self.directors[i]:onTuTeng()
                --self.directors[i]:onAddHp2(self.directors[i].key, 100)
            end
        end

    --Add HP,  
    elseif skillid==10 then
        local hp = jx["zhili"]
        for i=1, #self.directors do
            if self.directors[i].direction == 1*data.direction then
                self.directors[i]:onAddHp2(self.directors[i].key, hp)
            end
        end

    --Jie Du,    
    elseif skillid==11 then
        for i=1, #self.directors do
            if self.directors[i].direction == 1*data.direction then
                self.directors[i]:onJieDu()
            end
        end

    --Sheng Guang
    elseif skillid==13 then
        for i=1, #self.directors do
            if self.directors[i].direction == 1*data.direction then
                self.directors[i]:onShengGuang(data.seq)
            end
        end

    --Xu Ruo 
    elseif skillid==12 then
        for i=1, #self.directors do
            if self.directors[i].direction == -1*data.direction then
                self.directors[i]:onXuRuo()
                --self.directors[i]:onAddHp2(self.directors[i].key, 100)
            end
        end

    elseif skillid==14 or skillid==15 or skillid==16 then
        data.dt = 3
        data.hp = jx["zhili"]
        data.seq = self.jxSeq
        data.skillId = skillid
        for i=1, #self.directors do
            if self.directors[i].direction == -1*data.direction then 
                self.directors[i]:handleGlobalSkill(data)
            end
        end
    end
end

--------------------walk related----------------------
function DirectorAgent:getTarget(director)
    local target = nil
    local minDistance = 9999999
    
    local function calDistance(director, targetDirector)
        local deltaX = director.position.x - targetDirector.position.x
        local deltaY = director.position.y - targetDirector.position.y
        return deltaX*deltaX + deltaY*deltaY
    end
    
    for i=1, #self.directors do
        local d = self.directors[i]
        if d.npcBook.direction ~= director.direction then
            local distance = calDistance(director, d)
            if distance < minDistance and director~=d then
                target = d
                minDistance = distance
            end
        end
    end
    
    return target
end

function DirectorAgent:getStandInfo(director)
    local rangeDirectors = {}
    local minDistance = 9999999
    local targetDirector = nil
    
    local function calDistance(director, targetDirector)
        local deltaX = director.position.x - targetDirector.position.x
        local deltaY = director.position.y - targetDirector.position.y
        return deltaX*deltaX + deltaY*deltaY
    end
    
    local minAttackRange = 100
    for i=1, #self.directors do
        local d = self.directors[i]
        if d.npcBook.direction == director.direction then
            if d.npcBook.attackRange < minAttackRange then
                minAttackRange = d.npcBook.attackRange
            end
            if d.npcBook.attackRange == director.npcBook.attackRange then
                table.insert(rangeDirectors, d)
            end
        else
            local distance = calDistance(director, d)
            if  distance < minDistance then
                targetDirector = d
                minDistance = distance
            end
        end
    end
    
    table.sort(rangeDirectors, function(a, b) return a.position.y < b.position.y end)
    local idx = 0
    local tmpY = -1000
    local isOverlap = false
    for i=1, #rangeDirectors do
        if math.abs(rangeDirectors[i].position.y-tmpY) < cell_height then
            isOverlap = true
        end
        tmpY = rangeDirectors[i].position.y
        
        if rangeDirectors[i] == director then
            idx = i
        end
    end
    local standCelly = 1
    if #rangeDirectors > #stand_celly_map then
        standCelly = 3
    elseif #rangeDirectors > 0 then
        local tmp = stand_celly_map[#rangeDirectors]
        standCelly =  tmp[idx]
    end
    
    local standy = director.position.y
    if isOverlap or (targetDirector and math.abs(targetDirector.position.y-director.position.y) > 3*cell_height) then
        standy = (standCelly*cell_height + height_offset)
    end
    --remove flowing code, it's means that all NPC will adjust their position, not only the first rank
    --[[
    if director.npcBook.attackRange > minAttackRange then
        standy = director.position.y
    end
    ]]
    return {standy=standy, target=targetDirector, standCelly=standCelly}
end

function DirectorAgent:isRightTeamMove()
    local maxX = 0
    local directors = self:getDirectors(1)
    for i=1, #directors do
        if directors[i].position.x > maxX then
            maxX = directors[i].position.x
        end
    end
    if maxX > WIN_SIZE.width*0.44 then
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
function DirectorAgent:onAttack(fromKey, targetKey, hp)
    local target = self:getDirector(targetKey)
    if target then
        if fromKey == targetKey and target.npcBook.name == "ZhaoHuanFaZhen_juexingSkill" then
            print("~~~~~~~~~~~~~ZhaoHuanFaZhen_juexingSkill addDirector~~~~~~~~~~~~~~~")
            local level = target.npcBook.npcLevel
            local randomId = math.random(1,3)
            local towerNpcBook = bookAgent:copyNpcBook(20013+randomId)
            print("ZhaoHuanFaZhen_juexingSkill:"..20013+randomId)
            towerNpcBook.npcLevel = level
            towerNpcBook.normalAttackVal = target.npcBook.normalAttackVal/3
            towerNpcBook.currHp = target.npcBook.currHp/3
            towerNpcBook.maxHp = towerNpcBook.currHp
            towerNpcBook.armorVal = target.npcBook.armorVal/3
            --towerNpcBook.attackInterval = 700
            towerNpcBook.dex = 10000
            towerNpcBook.block = 0
            towerNpcBook.direction = target.direction
            if towerNpcBook.direction==1 then
                towerNpcBook.row = 6
            else
                towerNpcBook.row = 18
            end
            towerNpcBook.col = 2
            
            local cell = self:calCell(target.position.x, target.position.y)
            towerNpcBook.row = cell.row
            towerNpcBook.col = cell.col
            self:addDirector(towerNpcBook)
        else
            target:onAttack(fromKey,hp)
        end
    end
end

function DirectorAgent:onAttack_LaGeNaLuoSi(fromKey, excludeKey, hp)
    local from = self:getDirector(fromKey)
    for i=1, #self.directors do
        local tmpDirector = self.directors[i]
        if tmpDirector.direction ~= from.direction and tmpDirector.key ~= excludeKey then
            tmpDirector:onAttack(fromKey,hp)
        end
    end
end

--------------------small skill------------------------
function DirectorAgent:getSmallSkillTargets(key)
    local smallSkillTargets = {}
    local director = self:getDirector(key)
    local target = director.attackTarget
    if director.npcBook.name == "Tyrande" or director.npcBook.name == "Jaina" or
        director.npcBook.name == "ShahelasiZhuMu" or director.npcBook.name == "KeLuoMaGuSi" or
        director.npcBook.name == "WoJin" then
        if director.npcBook.name == "Tyrande" then
            local teammates = self:getDirectors(director.direction)
            local j = nil
            local hp = math.pow(2,32)-1
            for i=1,#teammates do
                if  teammates[i].hp < hp then
                    hp = teammates[i].hp
                    j = i
                end
            end
            table.insert(smallSkillTargets, teammates[j])
        else
            smallSkillTargets = self:getDirectors(director.direction*-1)
        end
    elseif director.npcBook.name == "YiLiDan" then
        smallSkillTargets = self:getAttackRangeTarget(key,3,5)
    elseif director.npcBook.name == "LaSuoLiAn" then
        table.insert(smallSkillTargets, director)
    elseif director.npcBook.name == "AErSaSi" then
        local minHpRate = 1
        local minHpRateDirector = director
        for i=1, #self.directors do
            if self.directors[i].npcBook.direction == director.direction then
                local rate = self.directors[i].hp/self.directors[i].npcBook.currHp
                if rate < minHpRate then
                    minHpRate = rate
                    minHpRateDirector = self.directors[i]
                end
            end
        end
        table.insert(smallSkillTargets, minHpRateDirector)
    elseif director.npcBook.name == "AKeMengDe" then
        smallSkillTargets = self:getDirectors(director.direction*-1)
    else
       table.insert(smallSkillTargets, target)
    end
    return smallSkillTargets
end

function DirectorAgent:onSmallSkill(fromKey, targetKey, smallSkillParam)
    local hp = smallSkillParam.hp
    local target = self:getDirector(targetKey)
    if target == nil then return end
    target:onSmallSkill(fromKey, smallSkillParam)
end

--------------------big skill------------------------
function DirectorAgent:getBigSkillTargets(key)
    local bigSkillTargets = {}
    local director = self:getDirector(key)
    if director.npcBook.name == "FengXing" or director.npcBook.name == "Captain" or
        director.npcBook.name == "XiaoHei" or director.npcBook.name == "Tyrande" or
        director.npcBook.name == "DiXueLingZhu" or director.npcBook.name == "ShahelasiZhuMu" or
        director.npcBook.name == "KeLuoMaGuSi" or director.npcBook.name == "AErSaSi" or
        director.npcBook.name == "LuKui" or director.npcBook.name == "WaSiQi" or director.npcBook.name == "Lina" then
        if director.npcBook.name == "Tyrande" or director.npcBook.name == "AErSaSi" then
            bigSkillTargets = self:getDirectors(director.direction)
        else
            bigSkillTargets = self:getDirectors(director.direction*-1)
        end
    elseif director.npcBook.name == "YiLiDan" then
        bigSkillTargets = self:getAttackRangeTarget(key,5,4)
    elseif director.npcBook.name == "MaNuoLuoSi" then
        bigSkillTargets = self:getAttackRangeTarget(key,6,6)
    elseif director.npcBook.name == "ZhuRiZhe" then
        table.insert(bigSkillTargets, director)
    else
        --[[
        if director.attackTarget and director.attackTarget.isLive then
            table.insert(bigSkillTargets, director.attackTarget)
        end
        ]]
        local function calDistance(director, targetDirector)
            local deltaX = director.position.x - targetDirector.position.x
            local deltaY = director.position.y - targetDirector.position.y
            return deltaX*deltaX + deltaY*deltaY
        end

        local minDistance = 999999
        local targetDirector = nil
        for i=1, #self.directors do
            local d = self.directors[i]
            if d.npcBook.direction ~= director.direction then
                local distance = calDistance(director, d)
                if distance < minDistance then
                    targetDirector = d
                    minDistance = distance
                end
            end
        end
        if targetDirector then
            table.insert(bigSkillTargets, targetDirector)
        end
    end
    --[[
    for i=#bigSkillTargets,1,-1 do
        local distance = math.abs(bigSkillTargets[i].position.x - director.position.x)
        if distance > 960 then
            table.remove(bigSkillTargets, i)
        end
    end
    ]]
    return bigSkillTargets
end

function DirectorAgent:onBigSkill(fromKey, targetKey, bigSkillParam)
    local hp = bigSkillParam.hp
    local target = self:getDirector(targetKey)
    if target == nil then return end
    target:onBigSkill(fromKey, bigSkillParam)
    
    local from = self:getDirector(fromKey)
    if from == nil then return end
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
        brotherNpcBook1.row = from.row + from.direction
        brotherNpcBook1.col = from.col
        self:addDirector(brotherNpcBook1)
        --[[
        local brotherNpcBook2 = bookAgent:copyNpcBook(20011)
        brotherNpcBook2.direction = from.direction
        brotherNpcBook2.row = from.row
        brotherNpcBook2.col = from.col - 1
        self:addDirector(brotherNpcBook2)
        ]]
        
    elseif from.npcBook.name =="tonglingta" then
        print("~~~~~~~~~~~~~addDirector~~~~~~~~~~~~~~~")
        local level = from.npcBook.npcLevel
        local towerNpcBook = bookAgent:copyNpcBook(20010)
        towerNpcBook.npcLevel = level
        towerNpcBook.normalAttackVal = 20+level*21
        towerNpcBook.currHp = 200+level*74
        towerNpcBook.maxHp = towerNpcBook.currHp
        towerNpcBook.armorVal = 5+level*5
        towerNpcBook.attackInterval = 700
        towerNpcBook.dex = 10000
        towerNpcBook.block = 0
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


function DirectorAgent:canHandleGlobalSkill(skillid)
    return true
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
    local director = self:getDirector(key)
    actorAgent:actCMD({t="die", key=key, sid=director.npcBook.soulId, direction=director.npcBook.direction})
    director:stop()
    if director.npcBook.name == "mutong" then
        for i=1, #self.directors do
            if self.directors[i].direction == 1 then
                self.directors[i]:onAddHp(key, 5000)
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
        print("[DirectorAgent]die "..self.directors[idx].npcBook.debug_name)
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
    self.frameNum = 0
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
    self.towerNum = 0
    self.dieKeys = self.dieKeys or {}
    self:resetDirectors()
    self.isPause = false
    for i=1, #self.directors do
        self.directors[i]:restart()
    end
end

function DirectorAgent:resetDirectors()
    self.directors = self.directors or {}
    for i=1, #self.directors do
        self.directors[i]:stop()
    end
    
    local function addDirectors(teamBook)
        for i=1, #teamBook do
            local npcBook = teamBook[i]
            npcBook.enterOrder = i
            local npcDirector = NpcDirector:create(npcBook)
            table.insert(self.directors, npcDirector)
        end
    end
    
    local function configJXSkill(direction)
        local memberList = {}
        for i=1, #self.directors do
            if self.directors[i].npcBook.direction == direction then
                table.insert(memberList, {soulId=self.directors[i].npcBook.soulId, zhili=self.directors[i].npcBook.zhili})
            end
        end
        -- reset all right jxSkill
        self.jxSkills = self.jxSkills or {}
        for k, v in pairs(self.jxSkills) do
            if self.jxSkills[k].direction == -1 then
                self.jxSkills[k] = nil
            end
        end
        if direction == 1 and self.actorAgentData.isEmulate then
            self:addJXSkills(Helper.createJXSkill(2, 2000), direction)
        else
            self:addJXSkills(Helper.calJXSkills(memberList), direction)
        end      
    end
    
    if #self:getDirectors(1) == 0 then
        local leftTeamBook = bookAgent:getLeftTeamBook()
        addDirectors(leftTeamBook)
        configJXSkill(1)
    end
    
    if #self:getDirectors(-1) == 0 then
        local rightTeamBook = bookAgent:getRightTeamBook()
        if rightTeamBook==nil then
            self:gameOver()
            return
        end
        addDirectors(rightTeamBook)
        configJXSkill(-1)
        --[[
        --jx config
        local rightMemberList = {}
        for i=1, #self.directors do
            if self.directors[i].npcBook.direction == -1 then
                table.insert(rightMemberList, {soulId=self.directors[i].npcBook.soulId, zhili=self.directors[i].npcBook.zhili})
            end
        end
        for k, v in pairs(self.jxSkills) do
            if self.jxSkills[k].direction == -1 then
                self.jxSkills[k] = nil
            end
        end
        self:addJXSkills(Helper.calJXSkills(rightMemberList), -1)
        ]]
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
    local function getDirectorCount(direction)
        local count = 0
        for i=1, #self.directors do
            if self.directors[i].npcBook.direction == direction then
                count = count + 1
            end
        end
        return count
    end
    if getDirectorCount(-1) == 0 then
        self.isPause = true
        --self.restartTimer = 60
        self:setTimeout(15, function() self:restart() end)
        for i=1, #self.directors do
            self.directors[i]:stop()
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
    param.leftTime = self.gameOverTime - math.ceil(self.frameNum/30)
    actorAgent:onBattleOver(param)
end

function DirectorAgent:getDirectors(direction)
    local directors = {}
    for i=1, #self.directors do
        if direction == self.directors[i].npcBook.direction then
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
        if self.directors[i].npcBook.direction ~= direction then
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
    self.frameNum = self.frameNum + 1
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


