--
--[[FSM State Defination]]--
--

----------------------------[[GlobalState Defination]]----------------------------
local GlobalState = class("GlobalState")
function GlobalState:create()
    local state = GlobalState.new()
    return state
end
function GlobalState:enter(director)
end
function GlobalState:execute(director)
    director:updateTimerList()
end
function GlobalState:exit(director)
end

----------------------------[[StandState Defination]]----------------------------
local StandState = class("StandState")
function StandState:create()
    local state = StandState.new()
    --state.normalAttackTimer = math.random(150,250)
    --state.smallSkillTimer = 0
    --state.bigSkillTimer = 0
    return state
end
function StandState:enter(director)
    --print("Enter StandState:"..director.npcBook.debug_name)
    actorAgent:actCMD({t="stand", key=director.key})
end
function StandState:execute(director)
    --print("StandState:"..director.key)
    director.normalAttackTimer = director.normalAttackTimer + 1
    director.smallSkillTimer = director.smallSkillTimer + 1
    director.bigSkillTimer = director.bigSkillTimer + 1
    
    --check for PauseState
    if director:checkPause() == true then
        director:changeState(director.pauseState)
        return
    end
    
    --check for WalkState
    if director:checkAttackRange() == false then
        director:changeState(director.walkState)
        return
    end
    
    --check for BigSkillState
    if director:checkBigSkill() == true then
        director.bigSkillTimer = 0
        director:changeState(director.bigSkillState)
        return
    end
    
    --check for SmallSkillState
    if director:checkSmallSkill() == true then
        director.smallSkillTimer = 0
        director:changeState(director.smallSkillState)
        return
    end
    
    --check for NormalAttackState
    if director:checkNormalAttack() == true then
        director.normalAttackTimer = 0
        director:changeState(director.normalAttackState)
        return
    end
end
function StandState:exit(director)
end

----------------------------[[WalkState Defination]]---------------------------------
local WalkState = class("WalkState")
function WalkState:create()
    local state = WalkState.new()
    return state
end
function WalkState:enter(director)
    --print("Enter WalkState:"..director.npcBook.debug_name)
    director:startWalk()
end
function WalkState:execute(director)
    director.normalAttackTimer=director.normalAttackTimer+1
    director:walk()
    
    --check for PauseState
    if director:checkPause() == true then
        director:changeState(director.pauseState)
        return
    end
    
    --check for StandState
    if director:checkAttackRange() == true then
        director:changeState(director.standState)
        return
    end
end
function WalkState:exit(director)
    actorAgent:startJX()
end

----------------------------[[NormalAttackState Defination]]--------------------------
local NormalAttackState = class("NormalAttackState")
function NormalAttackState:create()
    local state = NormalAttackState.new()
    state.timer = 0
    return state
end
function NormalAttackState:enter(director)
    --print("Enter NormalAttackState:"..director.npcBook.debug_name)
    --1. attack ready
    director:attackReady()
    director:addAttackSq()
    self.timer = 0
end
function NormalAttackState:execute(director)
    self.timer = self.timer + 1
    
    --check for PauseState
    if director:checkPause() == true then
        director:changeState(director.pauseState)
        return
    end
    
    --2. attack start
    if self.timer == director:getAttackStartTime() then
        director:attackStart()
        return
    end
    
    --3. attack over
    if self.timer >= director:getAttackOverTime() then
        director:changeState(director.standState)
        return
    end
end
function NormalAttackState:exit(director)
end

----------------------------[[SmallSkillState Defination]]----------------------------
local SmallSkillState = class("SmallSkillState")
function SmallSkillState:create()
    local state = SmallSkillState.new()
    state.timer = 0
    return state
end
function SmallSkillState:enter(director)
    --print("Enter SmallSkillState:"..director.npcBook.debug_name)
    director:smallSkillReady()
    director:addSmallSkillSq()
    self.persistentSmallSkillSq = 1
    self.timer = 0
end
function SmallSkillState:execute(director)
    self.timer = self.timer + 1
    
    --check for PauseState
    if director:checkPause() == true then
        director:changeState(director.pauseState)
        return
    end
    
    --2. attack start
    if self.timer == director:getSmallSkillStartTime() then
        director:smallSkillStart()
        return
    end

    --3. attack persistent
    if self.timer == director.npcBook.persistentSmallSkillFramesNum*self.persistentSmallSkillSq and self.persistentSmallSkillSq <= director.npcBook.persistentSmallSkillCount then
        --print("timer:"..self.timer)
        director:smallSkillOnGoing()
        self.persistentSmallSkillSq = self.persistentSmallSkillSq+1
        return
    end
    
    --4. attack over
    if self.timer >= director:getSmallSkillOverTime() then
        director:smallSkillOver()
        director:changeState(director.standState)
        return
    end
end
function SmallSkillState:exit(director)
end

----------------------------[[BigSkillState Defination]]----------------------------
local BigSkillState = class("BigSkillState")
function BigSkillState:create()
    local state = BigSkillState.new()
    state.timer = 0
    return state
end
function BigSkillState:enter(director)
    --print("Enter BigSkillState:"..director.npcBook.debug_name)
    director:bigSkillReady()
    director:addBigSkillSq()
    self.timer = 0
    self.persistentBigSkillSq = 1
end
function BigSkillState:execute(director)
    self.timer = self.timer + 1
    
    --check for PauseState
    if director:checkPause() == true then
        director:changeState(director.pauseState)
        return
    end
    
    --2. attack start
    if self.timer == director:getBigSkillStartTime() then
        director:bigSkillStart()
        return
    end
    
    --3. attack persistent
    if self.timer == director.npcBook.persistentBigSkillFramesNum*self.persistentBigSkillSq and self.persistentBigSkillSq <= director.npcBook.persistentBigSkillCount then
        director:bigSkillOnGoing()
        self.persistentBigSkillSq = self.persistentBigSkillSq+1
        return
    end

    --4. attack over
    if self.timer >= director:getBigSkillOverTime() then
        director:bigSkillOver()
        director:changeState(director.standState)
        return
    end
end
function BigSkillState:exit(director)
end

----------------------------[[PauseState Defination]]----------------------------
local PauseState = class("PauseState")
function PauseState:create()
    local state = PauseState.new()
    state.timer = 0
    return state
end
function PauseState:enter(director)
    --print("Enter PauseState:"..director.npcBook.debug_name)
    director:pauseStart()
    self.timer = 0
end
function PauseState:execute(director)
    self.timer = self.timer + 1
    if self.timer >= director:getPauseOverTime() or director:pauseTypeChange() then
       director:changeState(director.standState)
       return 
    end
end
function PauseState:exit(director)
    director:pauseOver()
end


------------------------------------------------------------------------------------
----------------------------[[NpcDirector defination]]------------------------------
------------------------------------------------------------------------------------
local StateMachine = require "scene.battle2.directors.StateMachine"

local NpcDirector = class("NpcDirector",function()
    return {}
end)

------------[[External API]]----------------
function NpcDirector:create(npcBook)
    local npcDirector = NpcDirector.new()
    npcDirector:initM(npcBook)
    return npcDirector
end

function NpcDirector:updateM(dt)
    self.dt = dt
    self.fsm:update()
end

function NpcDirector:changeState(state)
    self.fsm:changeState(state)
end

------------------stand related---------------------
function NpcDirector:checkAttackRange()
    if self:isTower() then return true end
    self.attackTarget = directorAgent:getTargetDirector(self.key)
    if self.attackTarget == nil then return true end
    local ret = directorAgent:getAttackOrder(self)
    
    local distanceX = math.abs(self.position.x - self.attackTarget.position.x)
    local toY = self.attackTarget.position.y + ret.attackOrder*cell_height/2
    local distanceY = self.position.y - toY
    
    --set speedX
    if distanceX <= (self.npcBook.attackRange*cell_width*1.2) then
        self.speedX = 0
    else 
        self.speedX = 90
    end
    
    --set speedY
    self.speedY = 0
    if self.npcBook.attackRange==1 and self.direction == ret.moveYDirection then
        self.zOrderList = ret.zOrderList
        if distanceY > 5 then
            self.speedY = -50
        elseif distanceY < -5 then
            self.speedY = 50
        else
            self.speedY = 0
        end
    end
    
    if self.npcBook.attackRange==1 then
        if distanceX <= (self.npcBook.attackRange*cell_width*1.2) then
            if math.abs(distanceY)<=5 then
                return true
            end
            if self.direction ~= ret.moveYDirection then
                return true
            end
        end
    else
        if distanceX <= self.npcBook.attackRange*cell_width*1.2 then
            return true
        end
    end
    
    return false
end

------------------walk related---------------------
function NpcDirector:walk()
    local shiftX = self.speedX*self.dt*self.direction
    local shiftY = self.speedY*self.dt
    
    if self.direction==1 and directorAgent:isRightTeamMove() then
        directorAgent:rightTeamMove(-shiftX, 0)
        if shiftY ~= 0 then
            self:move(0, shiftY)
        end
    else
        self:move(shiftX, shiftY)
    end
end

function NpcDirector:move(shiftX, shiftY)
    self.position.x = self.position.x + shiftX
    self.position.y = self.position.y + shiftY

    local newCell = directorAgent:calCell(self.position.x, self.position.y)
    if newCell.row ~= self.row or newCell.col ~= self.col then
        directorAgent:updateCell(
            self.key, 
            {row=self.row, col=self.col}, 
            {row=newCell.row, col=newCell.col}
        ) 
        self.row = newCell.row
        self.col = newCell.col
    end

    actorAgent:actCMD({t="move", key=self.key, dx=shiftX, dy=shiftY, z=self.zOrderList})
end

function NpcDirector:startWalk()
    --actorAgent:stopJX()
    actorAgent:actCMD({t="walk", key=self.key})
end

------------------normal attack---------------------
function NpcDirector:attackReady()
    self.attackTarget = directorAgent:getTargetDirector(self.key)
    actorAgent:actCMD({t="atk_r", key=self.key})
end

function NpcDirector:attackStart()
    if self.attackTarget == nil then return end
    local dt = self:getAttackDt()
    self:setTimeout(math.floor(dt*30), function()
        if self.attackTarget then
            directorAgent:onAttack(self.key, self.attackTarget.key, self:getAttackHp()) 
        end
    end)
    actorAgent:actCMD({t="atk_s", key=self.key, tKey=self.attackTarget.key, dt=self:getAttackDt()})
end

function NpcDirector:checkNormalAttack()
    if self:canNormalAttack() == false then return false end
    if self.normalAttackTimer > 30*self.npcBook.attackInterval then
        if self:isTower() then
            local targetDirector = directorAgent:getTargetDirector(self.key)
            if targetDirector == nil then return true end
            local distanceX = math.abs(self.position.x - targetDirector.position.x)
            if distanceX > (self.npcBook.attackRange*cell_width*1.2) then
                return false
            end
        end
        return true
    end
    return false
end

function NpcDirector:getAttackStartTime()
    return math.floor(self.npcBook.normalAttackReadyTime*30)
end

function NpcDirector:getAttackOverTime()
    if self.attackTarget == nil then return 0 end
    local distance = math.abs(self.row - self.attackTarget.row)
    if self.npcBook.attackRange > 1 then
        return math.floor(30*(distance/8 + self.npcBook.normalAttackReadyTime))
    else
        return math.floor((self.npcBook.normalAttackReadyTime)*30+1)
    end
end

function NpcDirector:getAttackDt()
    if self.attackTarget == nil then return 0 end
    local distance = math.abs(self.row - self.attackTarget.row)
    if self.npcBook.attackRange > 1 then
        return distance/8
    else
        return 1/30
    end
end

function NpcDirector:getAttackHp()
    return self.npcBook.normalAttackVal
end

function NpcDirector:addAttackSq()
    self.attackSq = self.attackSq + 1
end

------------------small skill---------------------
function NpcDirector:smallSkillReady()
    self.smallSkillTargets = directorAgent:getSmallSkillTargets(self.key)
    actorAgent:actCMD({t="small_r", key=self.key})
end

function NpcDirector:smallSkillStart()
    self.smallSkillTargets = directorAgent:getSmallSkillTargets(self.key)
    local tSet = {}
    for i=1, #self.smallSkillTargets do
        local smalledTimeItem = {}
        local tSetItem = {}
        local key = self.smallSkillTargets[i].key
        local dt = self:getSmallSkillDt(key)
        tSetItem.key = key
        tSetItem.dt = dt
        tSet[tostring(i)] = tSetItem
        self:setTimeout(math.floor(dt*30), function () 
            directorAgent:onSmallSkill(self.key, key, self:getSmallSkillHp())
        end)
    end
    if  self.npcBook.name == "KeLuoMaGuSi" then
        actorAgent:actCMD({t="small_s", key=self.key, tSet=tSet,row=self.row, col=self.col})
    else
        actorAgent:actCMD({t="small_s", key=self.key, tSet=tSet})
    end
end

function NpcDirector:smallSkillOver()
end

function NpcDirector:checkSmallSkill()
    if self:canSmallSkill() == false then return false end
    if self.smallSkillTimer > 30*self.npcBook.smallSkillInterval then
        return true
    end
    return false
end

function NpcDirector:getSmallSkillStartTime()
    return math.floor(self.npcBook.smallSkillReadyTime*30)
end

function NpcDirector:getSmallSkillOverTime()
    if self.attackTarget == nil then return 0 end
    local distance = math.abs(self.row - self.attackTarget.row)
    if self.npcBook.name == "LaGeNaLuoSi" then
        return math.floor(self.npcBook.smallSkillDuration*30)
    end
    if self.npcBook.attackRange > 1 then
        return math.floor(30*(distance/8 + self.npcBook.smallSkillReadyTime))
    else
        return math.floor((self.npcBook.smallSkillDuration + self.npcBook.smallSkillReadyTime)*30)
    end
end

function NpcDirector:getSmallSkillDt(targetKey)
    if self.attackTarget == nil then return 0 end
    local distance = math.abs(self.row - self.attackTarget.row)
    if self.npcBook.name == "Jaina" or self.npcBook.name == "LaGeNaLuoSi" or
        self.npcBook.name == "AoNiKeXiYa" or self.npcBook.name == "KeLuoMaGuSi" then
        return self.npcBook.smallSkillDuration
    end
    if self.npcBook.attackRange > 1 then
        return distance/8
    else
        return self.npcBook.smallSkillDuration
    end
end

---------persistentSmallSkill----------------
function NpcDirector:smallSkillOnGoing()
    self.smallSkillTargets = directorAgent:getSmallSkillTargets(self.key)
    if self.smallSkillTargets==nil or #self.smallSkillTargets == 0 then
        return
    end
    for i=1, #self.smallSkillTargets do
        local targetKey = self.smallSkillTargets[i].key
        directorAgent:onPersistentSkill(self.key, targetKey, self:getSmallSkillHp())
    end
end

function NpcDirector:getSmallSkillHp()
    return self.npcBook.smallSkillAttackVal
end

function NpcDirector:addSmallSkillSq()
    self.smallSkillSq = self.smallSkillSq + 1
end

------------------big skill---------------------
function NpcDirector:bigSkillReady()
    actorAgent:actCMD({t="big_r", key=self.key})
end

function NpcDirector:bigSkillStart()
    self.bigSkillTargets = directorAgent:getBigSkillTargets(self.key)
    local tSet = {}
    for i=1, #self.bigSkillTargets do
        local bigedTimeItem = {}
        local tSetItem = {}
        local key = self.bigSkillTargets[i].key
        local dt = self:getBigSkillDt(key)
        tSetItem.key = key
        tSetItem.dt = dt
        tSet[tostring(i)] = tSetItem
        self:setTimeout(math.floor(dt*30), function () 
            --persiitent skill no biged cmd
            if self.npcBook.persistentBigSkillFramesNum ~= 0 then return end 
            directorAgent:onBigSkill(self.key, key, self:getBigSkillHp())
        end)
    end
    if  self.npcBook.name == "DiXueLingZhu" or self.npcBook.name == "KeLuoMaGuSi" or
        self.npcBook.name == "MaNuoLuoSi" then
        actorAgent:actCMD({t="big_s", key=self.key, tSet=tSet,row=self.row, col=self.col})
    else
        actorAgent:actCMD({t="big_s", key=self.key, tSet=tSet})
    end
end

function NpcDirector:bigSkillOver()
end

function NpcDirector:checkBigSkill()
    if self:canBigSkill() == false then return false end
    if self.bigSkillTimer > 30*self.npcBook.bigSkillInterval then
        return true
    end
    return false
end

function NpcDirector:getBigSkillStartTime()
    return math.floor(self.npcBook.bigSkillReadyTime*30)
end

function NpcDirector:getBigSkillOverTime()
    return math.floor((self.npcBook.bigSkillDuration + self.npcBook.bigSkillReadyTime)*30)
end

function NpcDirector:getBigSkillDt(targetKey)
    local targetDirector = directorAgent:getDirector(targetKey)
    local director = directorAgent:getDirector(self.key)
    local tx = targetDirector.position.x
    local x =  director.position.x
    local distance = math.abs(tx-x)
    local speed = director.npcBook.missileSpeed
    local dt = distance/speed
    if  dt < 0.3 then
        dt =0.3
    end --if the distance is very close
    if self.npcBook.name == "Captain" then
        dt =  WIN_SIZE.width*0.8/speed
    elseif self.npcBook.name == "FanKeLiFu" or self.npcBook.name == "KuLouWang" or
           self.npcBook.name == "Tyrande" or self.npcBook.name == "DiXueLingZhu" or
        self.npcBook.name == "LaGeNaLuoSi" or self.npcBook.name == "ShahelasiZhuMu" or
        self.npcBook.name == "AoNiKeXiYa"  or self.npcBook.name == "KeLuoMaGuSi"  or
        self.npcBook.name == "MaNuoLuoSi" or self.npcBook.name == "KeLuoMaGuSi" then
        dt =  self.npcBook.bigSkillDuration
    elseif self.npcBook.name == "XiaoHei" then
        local rand =  math.random()
        local flag = -1
        if  math.floor(math.random()*10)%2 == 0 then flag = flag*-1 end
        dt = self.npcBook.bigSkillDuration + rand*flag/5
    end
    return dt
end

--------------------persistentBigSkill-----------------
function NpcDirector:bigSkillOnGoing()
    self.bigSkillTargets = directorAgent:getBigSkillTargets(self.key)
    if self.bigSkillTargets==nil or #self.bigSkillTargets == 0 then
        return
    end
    for i=1, #self.bigSkillTargets do
        local targetKey = self.bigSkillTargets[i].key
        directorAgent:onPersistentSkill(self.key, targetKey, self:getBigSkillHp())
    end
end

function NpcDirector:getBigSkillHp()
    return self.npcBook.bigSkillAttackVal
end

function NpcDirector:addBigSkillSq()
    self.bigSkillSq = self.bigSkillSq + 1
end

------------------pause state---------------------
function NpcDirector:pauseStart()
    self.currPauseType = self.pauseType
    self.pauseType = 0
    
    if self.currPauseType == 1 then
        actorAgent:actCMD({t="yun_s", key=self.key})
    elseif self.currPauseType == 2 then
        local tmpRow = self.row
        if self.direction == 1 then
            self.row = self.row - 2
        else
            self.row = self.row + 2
        end
        self.position = directorAgent:calPosition(self.row, self.col)
        directorAgent:updateCell(self.key, {row=tmpRow, col=self.col}, {row=self.row, col=self.col})
        actorAgent:actCMD({t="tui_s", key=self.key, dt=self:getPauseOverTime(), tPos={x=self.position.x, y=self.position.y}})
    elseif self.currPauseType == 3 then
        local dt = self.globalSkillData.dt
        local hp = self.globalSkillData.hp
        local seq = self.globalSkillData.seq
        local skillId = self.globalSkillData.skillId
        actorAgent:actCMD({t="global_s", key=self.key, dt=dt, seq=seq, skillId=skillId})
        actorAgent:stopJX()
        for i=1, 5 do
            local time = math.floor(dt*30*i/5)
            self:setTimeout(time, function()
                directorAgent:onGlobalSkill(self.key, hp)
            end)
        end
    elseif self.currPauseType == 4 then
        actorAgent:actCMD({t="ice_s", key=self.key})
    end
end

function NpcDirector:pauseOver()
    if self.currPauseType == 1 then
        actorAgent:actCMD({t="yun_o", key=self.key})
    elseif self.currPauseType == 2 then
        actorAgent:actCMD({t="tui_o", key=self.key})
    elseif self.currPauseType == 3 then
        actorAgent:actCMD({t="globaled", key=self.key,seq = self.globalSkillData.seq})
        actorAgent:startJX()
    elseif self.currPauseType == 4 then
        actorAgent:actCMD({t="ice_o", key=self.key})
    end
    self.currPauseType = 0
end

function NpcDirector:getPauseOverTime()
    if self.currPauseType == 1 then
        return 30*1
    elseif self.currPauseType == 2 then
        return 10
    elseif self.currPauseType == 3 then
        return 30*self.globalSkillData.dt
    -- ice
    elseif self.currPauseType == 4 then
        return 45
    end
    return 30*3
end

function NpcDirector:checkPause()
    self.pauseType = self.pauseType or 0
    if self.pauseType == 0 then
        return false
    else
        return true
    end
end

function NpcDirector:getCurrPauseType()
    self.currPauseType = self.currPauseType or 0
    return self.currPauseType
end

--(1,Captain), (2, FengXing)
function NpcDirector:setPauseType(type)
    if self.currPauseType == 3 then return end
    self.pauseType = type
end

function NpcDirector:pauseTypeChange()
    return (self.currPauseType ~= self.pauseType) and (self.pauseType ~= 0)
end

------------------API for directorAgent------------
function NpcDirector:onAttack(hp)
    local effect = self:rollEffectType(hp)
    local hurt = math.max(effect.hurt - self.npcBook.armorVal,1)
    hurt = math.floor(hurt)
    self.hp = self.hp - hurt
    actorAgent:actCMD({t="atked", key=self.key, hp=hurt, eft=effect.eft})
    
    if self:checkDie() == true and self.isLive then
        self.isLive = false
        --actorAgent:actCMD({t="die", key=self.key})
        directorAgent:onDie(self.key)
    end
end

function NpcDirector:onSmallSkill(fromKey, hp)
    local fromDirector = directorAgent:getDirector(fromKey)
    if  fromDirector.npcBook.name == "Tyrande" then
        if self.hp-hp >= self.npcBook.maxHp then
            hp =  self.hp - self.npcBook.maxHp
            self.hp = self.npcBook.maxHp
        else
            self.hp = self.hp-hp
        end
        actorAgent:actCMD({t="smalled", key=self.key, hp=hp, fKey=fromKey})
    elseif fromDirector.npcBook.name == "ShahelasiZhuMu" then
        --silence skill, make others can't fire skill
        --self:setSilence(true)
        self:startSilence()
        self:setTimeout(6*30,function() self:stopSilence() end)
    else
        local effect = self:rollEffectType(hp)
        local hurt = math.max(effect.hurt - self.npcBook.armorVal,1)
        hurt = math.floor(hurt)
        self.hp = self.hp - hurt
        actorAgent:actCMD({t="smalled", key=self.key, hp=hurt, fKey=fromKey, eft=effect.eft})
    end
    
    if self:checkDie() == true and self.isLive then
        self.isLive = false
        --actorAgent:actCMD({t="die", key=self.key})
        directorAgent:onDie(self.key)
    end
end

function NpcDirector:onBigSkill(fromKey, hp)
    local effect = self:rollEffectType(hp)
    local hurt = math.max(effect.hurt - self.npcBook.armorVal,1)
    hurt = math.floor(hurt)
    self.hp = self.hp - hurt
    actorAgent:actCMD({t="biged", key=self.key, hp=hurt, fKey=fromKey, eft=effect.eft})
    --printJSON({t="biged", key=self.key, hp=hp, fKey=fromKey})
    if self:checkDie() == true and self.isLive then
        self.isLive = false
        --actorAgent:actCMD({t="die", key=self.key})
        directorAgent:onDie(self.key)
    end
    local fromDirector = directorAgent:getDirector(fromKey)
    if fromDirector == nil then return end
    if self:isTower() then return end
    local bigedHandler = self.bigedHandlers[fromDirector.npcBook.name]
    if bigedHandler then
        bigedHandler({fromKey=fromKey, hp=hp})
    end
end

function NpcDirector:onGlobalSkill(hp)
    local hurt = math.max(hp,1)
    self.hp = self.hp - hurt
    actorAgent:actCMD({t="global_in", key=self.key, hp=hurt})
    
    if self:checkDie() == true and self.isLive then
        self.isLive = false
        --actorAgent:actCMD({t="die", key=self.key})
        directorAgent:onDie(self.key)
    end
end

function NpcDirector:onPersistentSkill(fromKey, hp)
    if self.hp-hp >= self.npcBook.maxHp then
        hp =  self.hp - self.npcBook.maxHp
        self.hp = self.npcBook.maxHp
    else
        self.hp = self.hp-hp
    end
    actorAgent:actCMD({t="persistent_in", key=self.key, hp=hp})

    if self:checkDie() == true and self.isLive then
        self.isLive = false
        --actorAgent:actCMD({t="die", key=self.key})
        directorAgent:onDie(self.key)
    end
    --[[
    local fromDirector = directorAgent:getDirector(fromKey)
    if fromDirector.npcBook.name == "KeLuoMaGuSi" then
        self:setPauseType(4)
    end
    ]]
end

function NpcDirector:onAddHp(fromKey, hp)
    local fromDirector = directorAgent:getDirector(fromKey)
    local dt = math.abs(fromDirector.row - self.row)/10
    self:setTimeout(math.floor(dt*30), function() self.hp = self.hp + 100 end)
    actorAgent:actCMD({t="addHp", key=fromKey, tkey=self.key, hp= -100, dt=dt})
end

function NpcDirector:handleGlobalSkill(skillid,hp,dt,seq)
    self:setPauseType(3)
    local globalSkillData = {}
    globalSkillData.hp = hp
    globalSkillData.dt = dt
    globalSkillData.seq = seq
    globalSkillData.skillId = skillid
    self.globalSkillData = globalSkillData
end

function NpcDirector:checkDie()
    if self.hp <= 0 then
        return true
    end
    return false
end

function NpcDirector:isWalking()
    return (self.fsm:getCurrentState()==self.walkState)
end

function NpcDirector:isPause()
    return (self.fsm:getCurrentState() == self.pauseState and self:getCurrPauseType()==2)
end

function NpcDirector:reset()
    print("[NpcDirector] reset"..self.npcBook.debug_name)
    self:setPauseType(0)
    self:changeState(self.standState)
end

------------[[Internal Function]]-----------
function NpcDirector:initM(npcBook)
    self.npcBook = npcBook
    self.key = npcBook.key
    self.direction = npcBook.direction
    
    if self.direction == 1 then
        self.row = 12 - npcBook.attackRange
    else
        self.row = 18 + npcBook.attackRange
    end
    if npcBook.row and npcBook.col then
        self.row = npcBook.row
        self.col = npcBook.col
    end
    if self:isTower() then self.row = 22 end
    --if self:isRecruitHero() then self.row = 12 - 4*self.direction end
    
    self.col = directorAgent:assignCol(self.key, self.row)
    if self:isTower() then self.col = 3 end
    
    self.position = directorAgent:calPosition(self.row, self.col)
    
    self.globalState = GlobalState:create()
    self.pauseState = PauseState:create()
    self.standState = StandState:create()
    self.walkState = WalkState:create()
    self.normalAttackState = NormalAttackState:create()
    self.smallSkillState = SmallSkillState:create()
    self.bigSkillState = BigSkillState:create()
    self.fsm = StateMachine:create({
        owner=self,
        currentState=self.standState,
        globalState=self.globalState
    })
    
    -- state timer
    self.normalAttackTimer = 0
    self.smallSkillTimer = 0
    self.bigSkillTimer = 0
    self.timerList = {}
    
    -- npc's hp
    self.hp = self.npcBook.currHp
    self.attackSq = 0
    self.smallSkillSq = 0
    self.bigSkillSq = 0
    self.speedX = 90
    self.speedY = 0
    
    -- biged skill handler
    self.bigedHandlers = {
        ["Captain"] = function(param) self:bigedHandler_Captain(param) end,
        ["FengXing"] = function(param) self:bigedHandler_FengXing(param) end,
        ["DiXueLingZhu"] = function(param) self:bigedHandler_Captain(param) end,
        ["Kelthuzad"] = function(param) self:bigedHandler_Captain(param) end,
        ["LaGeNaLuoSi"] = function(param) self:bigedHandler_LaGeNaLuoSi(param) end,
        ["KeLuoMaGuSi"] = function(param) self:bigedHandler_KeLuoMaGuSi(param) end       
    }
    
    self.isLive = true
    self.zOrder = (11-self.col)*100
    actorAgent:actCMD({
        t="createNpc",
        key=self.key,
        pos={x=self.position.x, y=self.position.y},
        hp=self.hp,
        npcBook=self.npcBook,
        zOrder=self.zOrder
    })
end

function NpcDirector:isTower()
    return self.npcBook.npcId >= 20000 and self.npcBook.npcId <= 20009
end

function NpcDirector:isRecruitHero()
    return self.npcBook.npcId >= 20010
end

function NpcDirector:canBigSkill()
    if self.npcBook.name == "mutong" or self:isRecruitHero() or self.npcBook.name == "Tower3" then 
        return false 
    end
    if self:getSilence() then return false end
    return true
end

function NpcDirector:canSmallSkill()
    if self:isTower() or self:isRecruitHero() or self.npcBook.name == "Tower3" then 
        return false 
    end
    if self:getSilence() then return false end
    return true
end

function NpcDirector:canNormalAttack()
    if self.npcBook.name == "mutong" then return false end
    return true
end

function NpcDirector:startSilence()
    self.isSilence = true
    actorAgent:actCMD({t="silence_s", key=self.key})
end

function NpcDirector:stopSilence()
    self.isSilence = false
    actorAgent:actCMD({t="silence_o", key=self.key})
end

function NpcDirector:getSilence()
    self.isSilence = self.isSilence or false
    return self.isSilence
end

function NpcDirector:setTimeout(time, callback)
    table.insert(self.timerList, {t=time, cb=callback})
end

function NpcDirector:updateTimerList()
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

-----------------biged handlers defination------------------
function NpcDirector:bigedHandler_Captain(param)
    --when global skill, other pause type not processed
    --if self.pauseType == 3 then return end
    self:setPauseType(1)
end

function NpcDirector:bigedHandler_FengXing(param)
    --when global skill, other pause type not processed
    --if self.pauseType == 3 then return end
    self:setPauseType(2)
end

function NpcDirector:bigedHandler_LaGeNaLuoSi(param)
    directorAgent:onAttack_LaGeNaLuoSi(param.fromKey, self.key, math.floor(param.hp/3))
end

function NpcDirector:bigedHandler_KeLuoMaGuSi(param)
    self:setPauseType(4)
end

function NpcDirector:rollEffectType(hp)
    local randomNum = math.random(1,10000)
    --baoji
    if randomNum<=self.npcBook.cri then
        return {eft=4, hurt=hp*2}
    --gedang
    elseif randomNum>self.npcBook.cri and randomNum<=self.npcBook.cri+self.npcBook.block then
        return {eft=3, hurt=hp/2}
    --miss
    elseif randomNum>self.npcBook.cri+self.npcBook.block and
       randomNum<=self.npcBook.cri+self.npcBook.block+self.npcBook.dex-self.npcBook.agl then
        return {eft=2, hurt=hp*0}
    else
        return {eft=1, hurt=hp}
    end
end

return NpcDirector
