local const_speedX_num = 160
local const_speedY_num = 80
--local const_speedY_num = 0
local speedup_factor = 1/1.2
local intervalFactor = 0.6
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
    if director.isStop == true then return end
    director.normalAttackTimer = director.normalAttackTimer + 1
    director.smallSkillTimer = director.smallSkillTimer + 1
    director.bigSkillTimer = director.bigSkillTimer + 1
    
    --check for BigSkillState
    if director:checkBigSkill() == true then
        director.bigSkillTimer = 0
        director:changeState(director.bigSkillState)
        return
    end
    
    --check for PauseState
    if director:checkPause() == true then
        director:changeState(director.pauseState)
        return
    end
    
    --check for FuckedState
    if director:checkFucked() == true then
        director:changeState(director.fuckedState)
        return
    end
    
    --check for BigSkillState
    --[[
    if director:checkBigSkill() == true then
        director.bigSkillTimer = 0
        director:changeState(director.bigSkillState)
        return
    end
    ]]
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
    
    --check for WalkState
    if director:checkWalk() == true then
        director:changeState(director.walkState)
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
    
    if director:checkBigSkill() == true then
        director.bigSkillTimer = 0
        director:changeState(director.bigSkillState)
        return
    end
    
    --check for PauseState
    if director:checkPause() == true then
        director:changeState(director.pauseState)
        return
    end
    
    --check for FuckedState
    if director:checkFucked() == true then
        director:changeState(director.fuckedState)
        return
    end
    
    --check for BigSkillState
    --[[
    if director:checkBigSkill() == true then
        director.bigSkillTimer = 0
        director:changeState(director.bigSkillState)
        return
    end
    ]]
    --check for NormalAttackState
    if director:checkNormalAttack() == true then
        director.normalAttackTimer = 0
        director:changeState(director.normalAttackState)
        return
    end

    --check for StandState
    if director:checkWalk() == false then
        director:changeState(director.standState)
        return
    end
end
function WalkState:exit(director)
    actorAgent:startJX()
    director:stopWalk()
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
    
    if director:checkBigSkill() == true then
        director.bigSkillTimer = 0
        director:changeState(director.bigSkillState)
        return
    end
    
    --check for PauseState
    if director:checkPause() == true then
        director:changeState(director.pauseState)
        return
    end
    
    --check for FuckedState
    if director:checkFucked() == true then
        director:changeState(director.fuckedState)
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
    
    if director:checkBigSkill() == true then
        director.bigSkillTimer = 0
        director:changeState(director.bigSkillState)
        return
    end
    
    --check for PauseState
    if director:checkPause() == true then
        director:changeState(director.pauseState)
        return
    end
    
    --check for FuckedState
    if director:checkFucked() == true then
        director:changeState(director.fuckedState)
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
    
    --check for FuckedState
    if director:checkFucked() == true then
        director:changeState(director.fuckedState)
        return
    end
    
    --2. attack start
    if self.timer == director:getBigSkillStartTime() then
        director:bigSkillStart()
        return
    end
    
    --3. attack persistent
    self.persistentBigSkillSq = self.persistentBigSkillSq or 1
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
    director:clearBigSkillPower()
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
        --[[
       if director.currPauseType == 7 then
           if director.fsm:getPreviousState() == director.smallSkillState or
                director.fsm:getPreviousState() == director.bigSkillState then
                director:changeState(director.restState)
                return
           end
       end
       ]]
       director:changeState(director.standState)
       return 
    end
end
function PauseState:exit(director)
    director:pauseOver()
end

----------------------------[[FuckedState Defination]]----------------------------
local FuckedState = class("FuckedState")
function FuckedState:create()
    local state = FuckedState.new()
    return state
end
function FuckedState:enter(director)
    print("FuckedState enter")
    self.timer = 0
    actorAgent:actCMD({t="fucked_s", key=director.key})
end
function FuckedState:execute(director)
    self.timer = self.timer + 1
    if self.timer >= 45 then
        director:resetFucked()
        actorAgent:actCMD({t="fucked_o", key=director.key})
        director:changeState(director.standState)
        return
    end
end
function FuckedState:exit(director)
end

----------------------------[[RestState Defination]]----------------------------
local RestState = class("RestState")
function RestState:create()
    local state = RestState.new()
    return state
end
function RestState:enter(director)
    --print("RestState enter "..director.npcBook.name)
    actorAgent:actCMD({t="suspend_s", key=director.key, sid=director.npcBook.soulId, seq=director.restSeq})
end
function RestState:execute(director)
    director.restTimer = director.restTimer - 1
    if director.restTimer <= 0 then
        --director:changeState(director.standState)
        director:popState()
    end
    
    --check for BigSkillState
    if director:checkBigSkill() == true then
        director.bigSkillTimer = 0
        director:changeState(director.bigSkillState)
        return
    end
end
function RestState:exit(director)
    --print("RestState exit "..director.npcBook.name)
    actorAgent:actCMD({t="suspend_o", key=director.key, sid=director.npcBook.soulId, seq=director.restSeq})
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
    if self.isStop == true then return end
    self.dt = dt
    self.fsm:update()
end

function NpcDirector:changeState(state)
    self.fsm:changeState(state)
end

function NpcDirector:pushState(state)
    self.fsm:pushState(state)
end

function NpcDirector:popState()
    self.fsm:popState()
end

function NpcDirector:rest(param)
    self.restTimer = param.dt
    self.restSeq = param.seq
    if self.fsm:getCurrentState()==self.restState then return end
    self:pushState(self.restState)
end

------------------stand related---------------------
function NpcDirector:checkWalk()
    if self:isTower() then return false end
    if self.isSheep then return false end
    local standInfo = directorAgent:getStandInfo(self)
    self.attackTarget = standInfo.target
    if standInfo.target == nil then
        self.speedY = 0
        return true 
    end
    
    --set speedX
    local attackDistanceX = self.npcBook.attackRange*cell_width*1.2
    if self.npcBook.attackRange == 1 then
        attackDistanceX = self.npcBook.ajustAttackX*cell_width
    end
    if standInfo.standCelly % 2 == 0 then
        attackDistanceX = attackDistanceX - 20
    else
        attackDistanceX = attackDistanceX + 20
    end
    
    local distanceX = math.abs(self.position.x - self.attackTarget.position.x)
    if distanceX <= attackDistanceX then
        self.speedX = 0
    else 
        self.speedX = const_speedX_num
    end
    
    if self.npcBook.big_skill_range_x then
        if distanceX > self.npcBook.big_skill_range_x*cell_width or self.attackTarget.npcBook.name=="mutong" then
            if self.isInBigSkillRange == true then
                self.isInBigSkillRange = false
                actorAgent:actCMD({t="fire_no", key=self.key, sid=self.npcBook.soulId})
            end
        else
            if self.isInBigSkillRange == false then
                self.isInBigSkillRange = true
                actorAgent:actCMD({t="fire_yes", key=self.key, sid=self.npcBook.soulId})
            end
        end
    end
    
    --set speedY
    if self.speedX==0 then
        local distanceY = standInfo.standy-self.position.y
        if distanceY > 5 then
            self.speedY = const_speedY_num
        elseif distanceY < -5 then
            self.speedY = -const_speedY_num
        else
            self.speedY = 0
        end
        if self.position.y < 150 or self.position.y > 430 then
            self.speedY = 0
        end
    end
    
    if self.speedX==0 and self.speedY==0 then
        return false
    else
        return true
    end
end

------------------walk related---------------------
function NpcDirector:walk()
    local shiftX = self.speedX*self.dt*self.direction
    if self.position.y < 150 or self.position.y > 430 then
        self.speedY = 0
    end
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
    actorAgent:actCMD({t="walk", key=self.key, sid=self.npcBook.soulId})
end

function NpcDirector:stopWalk()
    actorAgent:actCMD({t="walk_o", key=self.key, sid=self.npcBook.soulId})
end

------------------normal attack---------------------
function NpcDirector:attackReady()
    --self.attackTarget = directorAgent:getTargetDirector(self.key)
    actorAgent:actCMD({t="atk_r", key=self.key})
end

function NpcDirector:attackStart()
    if self.npcBook.name == "ZhaoHuanFaZhen_juexingSkill" then
        directorAgent:onAttack(self.key, self.key, 0) 
        return
    end
    self:addBigSkillPower()
    if self.attackTarget == nil then return end
    local dt = self:getAttackDt()
    self:setTimeout(math.floor(dt*30), function()
        if self.attackTarget then
            local attackHp = self:getAttackHp()
            directorAgent:onAttack(self.key, self.attackTarget.key, attackHp) 
            if self.isKuangBao then
                print("~~~~~~~~KuangBao Add Hp~~~~~~~~~~")
                self:onAddHp(self.key, math.max(attackHp*0.05, 4))
            end
        end
    end)
    if self.npcBook.direction == 1 then
        actorAgent:actCMD({t="atk_s", key=self.key, tKey=self.attackTarget.key, dt=self:getAttackDt(), 
            pr=self:getBigSkillPowerRate(), sid=self.npcBook.soulId})
    else
        actorAgent:actCMD({t="atk_s", key=self.key, tKey=self.attackTarget.key, dt=self:getAttackDt()})
    end
end

function NpcDirector:checkNormalAttack()
    if self:canNormalAttack() == false then return false end
    local interval = 30*self.npcBook.attackInterval*intervalFactor
    if self.isKuangBao then interval = interval*0.5 end
    if self.normalAttackTimer > interval then
        if self:isTower() then
            self.attackTarget = directorAgent:getTarget(self)
            if self.attackTarget == nil then return true end
            local distanceX = math.abs(self.position.x - self.attackTarget.position.x)
            if distanceX > (self.npcBook.attackRange*cell_width*1.2) then
                return false
            end
        end
        return true
    end
    return false
end

function NpcDirector:getAttackStartTime()
    return math.floor(self.npcBook.normalAttackReadyTime*speedup_factor*30)
end

function NpcDirector:getAttackOverTime()
    if self.attackTarget == nil then return 0 end
    local distance = math.abs(self.row - self.attackTarget.row)
    if self.npcBook.attackRange > 1 then
        return math.floor(30*(distance/8 + self.npcBook.normalAttackReadyTime*speedup_factor))
    else
        return math.floor((self.npcBook.normalAttackReadyTime*speedup_factor)*30+1)
    end
end

function NpcDirector:getAttackDt()
    if self.attackTarget == nil then return 0 end
    local distance = math.abs(self.row - self.attackTarget.row)
    if self.npcBook.attackRange > 1 then
        if self.npcBook.name == "MoFaZhaChe" then
            return self.npcBook.normalAttckDuration
        else
            return math.min(distance/12,1.0)
        end
    else
        return 1/30
    end
end

function NpcDirector:getAttackHp()
    if self.isTuTeng then
        return self.add_atk_factor*self.npcBook.normalAttackVal
    end
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
    --self.smallSkillTargets = directorAgent:getSmallSkillTargets(self.key)
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
            local smallSkillParam = self:getSmallSkillParam()
            directorAgent:onSmallSkill(self.key, key, smallSkillParam)
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
    if self.smallSkillTimer > 30*self.npcBook.smallSkillInterval*intervalFactor then
        return true
    end
    return false
end

function NpcDirector:getSmallSkillStartTime()
    return math.floor(self.npcBook.smallSkillReadyTime*speedup_factor*30)
end

function NpcDirector:getSmallSkillOverTime()
    if self.attackTarget == nil then return 0 end
    local distance = math.abs(self.row - self.attackTarget.row)
    if self.npcBook.attackRange > 1 then
        return math.floor(30*(distance/8 + self.npcBook.smallSkillReadyTime*speedup_factor))
    else
        return math.floor((self.npcBook.smallSkillDuration + self.npcBook.smallSkillReadyTime*speedup_factor)*30)
    end
end

function NpcDirector:getSmallSkillDt(targetKey)
    if self.attackTarget == nil then return 0 end
    local distance = math.abs(self.row - self.attackTarget.row)
    if self.npcBook.name == "Jaina" or self.npcBook.name == "LuKui" or
        self.npcBook.name == "AoNiKeXiYa" or self.npcBook.name == "KeLuoMaGuSi"
        or self.npcBook.name == "AErSaSi" or self.npcBook.name == "Kelthuzad" then
        return self.npcBook.smallSkillDuration
    end
    if self.npcBook.attackRange > 1 then
        return math.min(distance/12,1.0)
    else
        return self.npcBook.smallSkillDuration
    end
end

---------persistentSmallSkill----------------
function NpcDirector:smallSkillOnGoing()

    --self.smallSkillTargets = directorAgent:getSmallSkillTargets(self.key)
    if self.smallSkillTargets==nil or #self.smallSkillTargets == 0 then
        return
    end
    if self.npcBook.name == "LaZuoKeLao" then
        print("[NpcDirector]~~~~~~~~~~~~~smallSkillOnGoing~~~~~~~~~~~~~~~~~~")
    end
    for i=1, #self.smallSkillTargets do
        local targetKey = self.smallSkillTargets[i].key
        directorAgent:onPersistentSkill(self.key, targetKey, self:getSmallOnGoingHp())
    end
end

function NpcDirector:getSmallSkillParam()
    local param = {}
    param.fromKey = self.key
    param.hp = self:getSmallSkillHp()
    param.xuanyuntime = self.npcBook.small_xuanyuntime
    param.chenmotime = self.npcBook.small_chenmotime
    param.noname_data1 = self.npcBook.small_noname_data1
    param.noname_data2 = self.npcBook.small_noname_data2
    param.noname_data3 = self.npcBook.small_noname_data3
    param.add_def_factor = self.npcBook.small_add_def_factor
    param.add_baoji_factor = self.npcBook.small_add_baoji_factor
    param.add_gedang_factor = self.npcBook.small_add_gedang_factor
    param.add_hit_factor = self.npcBook.small_add_hit_factor
    param.add_miss_factor = self.npcBook.small_add_miss_factor
    return param
end

function NpcDirector:getSmallSkillHp()
    if self.isTuTeng then
        return self.add_atk_factor*self.npcBook.smallSkillAttackVal
    end
    return self.npcBook.smallSkillAttackVal
end

function NpcDirector:getSmallOnGoingHp()
    return self.npcBook.smallOnGoingHp or 0
end

function NpcDirector:addSmallSkillSq()
    self.smallSkillSq = self.smallSkillSq + 1
end

------------------big skill---------------------
function NpcDirector:bigSkillReady()
    if self.npcBook.name == "Jaina" then
        actorAgent:actCMD({t="big_r", key=self.key, row=self.row, col=self.col})
    else
        actorAgent:actCMD({t="big_r", key=self.key})
    end
    
    -- just test fire bigskill at the same time
    directorAgent:suspendForBigSkill(self)
    
    --directorAgent:pause()
    --directorAgent:setTimeout(15, function () directorAgent:resume() end)
end

function NpcDirector:bigSkillStart()
    self.bigSkillTargets = directorAgent:getBigSkillTargets(self.key)
    --if #self.bigSkillTargets==0 then return end
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
            directorAgent:onBigSkill(self.key, key, self:getBigSkillParam())
            if self.npcBook.name == "XianXueNvWangLan" then
                local addHp = math.max(self:getBigSkillHp()*0.05, 4)
                self:onAddHp(self.key, addHp)
                actorAgent:actCMD({t="persistent_in", key=self.key, hp=addHp})
            end
        end)
    end
    --just ensure tSet not empty!
    --[[
    if #tSet==0 then
        print("Fuck!! no target")
        tSet["1"] = {key=self.key, dt=self:getBigSkillDt(self.key)}
    end
    ]]
    if  self.npcBook.name == "DiXueLingZhu" or self.npcBook.name == "KeLuoMaGuSi" or
        self.npcBook.name == "MaNuoLuoSi" or self.npcBook.name == "Jaina" then
        actorAgent:actCMD({t="big_s", key=self.key, tSet=tSet,row=self.row, col=self.col, sid=self.npcBook.soulId, direction=self.direction})
    else
        actorAgent:actCMD({t="big_s", key=self.key, tSet=tSet, sid=self.npcBook.soulId, direction=self.direction})
    end
    directorAgent:disableAutoFireBigSkill()
    --directorAgent:resume()
end

function NpcDirector:bigSkillOver()
    directorAgent:enableAutoFireBigSkill()
    if self.direction == 1 then
        actorAgent:actCMD({t="big_o", sid=self.npcBook.soulId})
    end
end

function NpcDirector:fireBigSkill()
    print("[NpcDirector] fireBigSkill by clicking")
    self.isBigSkillFired = true
end

function NpcDirector:checkBigSkill()
    if self:canBigSkill() == false then return false end
    --if self.bigSkillTimer > 30*self.npcBook.bigSkillInterval*intervalFactor then
    if self:isBigSkillPowerFull() then
        if self:isTower() == true then
            self:clearBigSkillPower()
            return true
        elseif self.npcBook.direction == -1 then 
            self:clearBigSkillPower()
            return true
        elseif directorAgent:isAutoFight() == true then
            self:clearBigSkillPower()
            return true
        elseif self.isBigSkillFired == true then
            self.isBigSkillFired = false
            print("[NpcDirector] can fireBigSkill")
            self:clearBigSkillPower()
            return true 
        end
    end
    return false
end

function NpcDirector:addBigSkillPower()
    self.bigSkillPower = self.bigSkillPower + 1
    self.bigSkillPower = math.min(self.bigSkillPower, self.npcBook.bigSkillInterval/self.npcBook.attackInterval)
end

function NpcDirector:getBigSkillPowerRate()
    return math.ceil(100*self.bigSkillPower/(self.npcBook.bigSkillInterval/self.npcBook.attackInterval))
end

function NpcDirector:isBigSkillPowerFull()
    return self.bigSkillPower >= self.npcBook.bigSkillInterval/self.npcBook.attackInterval
end

function NpcDirector:clearBigSkillPower()
    self.bigSkillPower = 0
end

function NpcDirector:getBigSkillStartTime()
    return math.floor(self.npcBook.bigSkillReadyTime*speedup_factor*30)
end

function NpcDirector:getBigSkillOverTime()
    return math.floor((self.npcBook.bigSkillDuration + self.npcBook.bigSkillReadyTime*speedup_factor)*30)
end

function NpcDirector:getBigSkillDt(targetKey)
    local targetDirector = directorAgent:getDirector(targetKey)
    if targetDirector == nil then
        print("[NpcDirector]there is no director for "..targetKey)
        return nil
    end
    local director = self
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
        self.npcBook.name == "MaNuoLuoSi" or self.npcBook.name == "KeLuoMaGuSi" or
        self.npcBook.name == "AKeMengDe" or self.npcBook.name == "WoJin" or
        self.npcBook.name == "XianXueNvWangLan" or self.npcBook.name == "KuangGongXiaoBing" or
        self.npcBook.name == "ZhuRiZhe" or self.npcBook.name == "HuaiTe" then
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
        print("[NpcDirector]there is no target for "..self.npcBook.name)
        return
    end
    --[[
    if self.npcBook.name == "DuanGuZhe" then
        print("~~~~~~~DuanGuZhe bigSkillOnGoing~~~~~~~~~")
    end
    ]]
    for i=1, #self.bigSkillTargets do
        local targetKey = self.bigSkillTargets[i].key
        directorAgent:onPersistentSkill(self.key, targetKey, self:getBigOnGoingHp())
    end
end

function NpcDirector:getBigSkillParam()
    local param = {}
    param.fromKey = self.key
    param.hp = self:getBigSkillHp()
    param.xuanyuntime = self.npcBook.big_xuanyuntime
    param.chenmotime = self.npcBook.big_chenmotime
    param.noname_data1 = self.npcBook.big_noname_data1
    param.noname_data2 = self.npcBook.big_noname_data2
    param.noname_data3 = self.npcBook.big_noname_data3
    param.add_def_factor = self.npcBook.big_add_def_factor
    param.add_baoji_factor = self.npcBook.big_add_baoji_factor
    param.add_gedang_factor = self.npcBook.big_add_gedang_factor
    param.add_hit_factor = self.npcBook.big_add_hit_factor
    param.add_miss_factor = self.npcBook.big_add_miss_factor
    return param
end

function NpcDirector:getBigSkillHp()
    if self.isTuTeng then
        return self.add_atk_factor*self.npcBook.bigSkillAttackVal
    end
    return self.npcBook.bigSkillAttackVal
end

function NpcDirector:getBigOnGoingHp()
    local hp = self.npcBook.bigOnGoingHp or 0
    if self.npcBook.persistentBigSkillCount ~= 0 then
        return hp/self.npcBook.persistentBigSkillCount
    end
    return hp
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
        if self.direction == 1 then
            self.position.x = math.max(self.position.x - cell_width*2, 50)
        else
            self.position.x = math.min(self.position.x + cell_width*2, 910)
        end
        actorAgent:actCMD({t="tui_s", key=self.key, dt=self:getPauseOverTime(), tPos={x=self.position.x, y=self.position.y}})
    elseif self.currPauseType == 3 then
        local dt = self.globalSkillData.dt
        local hp = self.globalSkillData.hp
        local seq = self.globalSkillData.seq
        local skillId = self.globalSkillData.skillId
        actorAgent:actCMD({t="global_s", key=self.key, dt=dt, seq=seq, skillId=skillId})
        actorAgent:stopJX()
        
        if self.direction == 1 then return end
        for i=1, 5 do
            local time = math.floor(dt*30*i/5)
            self:setTimeout(time, function()
                directorAgent:onGlobalSkill(self.key, hp)
            end)
        end
        self:setTimeout((dt+0.5)*30, function()
            local param = {}
            param.fromKey = self.key
            param.hp = 0
            param.xuanyuntime = 3000
            param.chenmotime = 0
            param.noname_data1 = 0
            param.noname_data2 = 0
            param.noname_data3 = 0
            self:setPauseType(1, param)
        end)
        
    elseif self.currPauseType == 4 then
        actorAgent:actCMD({t="ice_s", key=self.key})
    elseif self.currPauseType == 5 then -- normal interrupt(npc run embattled action)
        actorAgent:actCMD({t="interrupt_s", key=self.key})
    elseif self.currPauseType == 6 then
        local pauseTime = self:getPauseOverTime()
        for i=1, 6 do
            local time = math.floor(30*i*pauseTime/6)
            self:setTimeout(time, function()
                self:onPersistentSkill(self.key, self:getBigOnGoingHp())
            end)
        end
        actorAgent:actCMD({t="bound_s", key=self.key})
    elseif self.currPauseType == 7 then
        actorAgent:actCMD({t="suspend_s", key=self.key, sid=self.npcBook.soulId})
    elseif self.currPauseType == 8 then
        actorAgent:actCMD({t="yun_s", key=self.key})
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
        --[[
        local hurtHp = self.pauseParam["hp"]
        if self.pauseParam and self.pauseParam["hp"] then
            hurtHp = self.pauseParam["hp"]
        else
            hurtHp = self.pauseParamSet[4].hp
        end]]
        local hurtHp = self.pauseParamSet[4].hp
        self:subHp(hurtHp)
        actorAgent:actCMD({t="atked",fkey=self.key,key=self.key, hp=hurtHp, eft=0})
        actorAgent:actCMD({t="ice_o", key=self.key})
    elseif self.currPauseType == 5 then
        actorAgent:actCMD({t="interrupt_o", key=self.key})
    elseif self.currPauseType == 6 then
        actorAgent:actCMD({t="bound_o", key=self.key})
    elseif self.currPauseType == 7 then
        actorAgent:actCMD({t="suspend_o", key=self.key, sid=self.npcBook.soulId})
    elseif self.currPauseType == 8 then
        actorAgent:actCMD({t="yun_o", key=self.key})
    end
    self.currPauseType = 0
end

function NpcDirector:getPauseOverTime()
    if self.currPauseType == 1 then
        return math.max(self.pauseParamSet[1]["xuanyuntime"]*30/1000, 30)
    elseif self.currPauseType == 2 then
        return 10
    elseif self.currPauseType == 3 then
        return 30*self.globalSkillData.dt
    -- ice
    elseif self.currPauseType == 4 then
        return math.max(self.pauseParamSet[4]["xuanyuntime"]*30/1000, 30)
    elseif self.currPauseType == 5 then
        return 10
    -- bound
    elseif self.currPauseType == 6 then
        return math.max(self.pauseParamSet[6]["xuanyuntime"]*30/1000, 30)
    -- suspend when someone fire big skill
    elseif self.currPauseType == 7 then
        return math.min(self.pauseParamSet[7]["pauseDt"], 45)
    elseif self.currPauseType == 8 then
        return math.max(self.pauseParamSet[8]["xuanyuntime"]*30/1000, 30*20)
    end
    return 30*1
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
function NpcDirector:setPauseType(type, pauseParam)
    if type~=7 and self.isCiBao == true then return end
    if self.currPauseType == 3 then return end
    self.pauseType = type
    self.pauseParam = pauseParam
    self.pauseParamSet[type] = pauseParam
end

function NpcDirector:pauseTypeChange()
    return (self.currPauseType ~= self.pauseType) and (self.pauseType ~= 0)
end

function NpcDirector:getArmorVal()
    if self.isTuTeng then
        return (self.npcBook.armorVal-self.jianFangVal)*self.add_def_factor
    end
    return (self.npcBook.armorVal-self.jianFangVal)
end

------------------fucked state---------------------
function NpcDirector:checkFucked()
    return self.isFucked
end

function NpcDirector:setFucked()
    self.isFucked = true
end

function NpcDirector:resetFucked()
    self.isFucked = false
end

------------------API for directorAgent------------
function NpcDirector:onAttack(fromKey,hp)
    local effect = self:rollEffectType(fromKey, hp)
    local hurt = 0
    if effect.hurt ~= 0 then
        hurt = math.max(effect.hurt - self:getArmorVal(),4+math.floor(self.npcBook.npcLevel/100))
        hurt = math.floor(hurt)
        self:subHp(hurt)
    end
    
    actorAgent:actCMD({t="atked",fkey=fromKey,key=self.key, hp=hurt, eft=effect.eft})
    
    if self:checkDie() == true and self.isLive then
        self.isLive = false
        --actorAgent:actCMD({t="die", key=self.key})
        directorAgent:onDie(self.key)
    end
end

function NpcDirector:onSmallSkill(fromKey, smallSkillParam)
    local hp = smallSkillParam.hp
    local fromDirector = directorAgent:getDirector(fromKey)
    if  fromDirector.npcBook.name == "Tyrande" then
        if self.hp-hp >= self.npcBook.maxHp then
            --hp =  self.hp - self.npcBook.maxHp
            self.hp = self.npcBook.maxHp
        else
            self.hp = self.hp-hp
        end
        actorAgent:actCMD({t="smalled", key=self.key, hp=hp, fKey=fromKey})
    elseif fromDirector.npcBook.name == "FanKeLiFu" then
        local effect = self:rollEffectType(fromKey, hp)
        effect.eft = 4
        effect.hurt = hp
        local hurt = math.max(effect.hurt - self:getArmorVal(),1)
        hurt = math.floor(hurt)
        --self.hp = self.hp - hurt
        self:subHp(hurt)
        actorAgent:actCMD({t="atked",fkey=fromKey,key=self.key, hp=hurt, eft=effect.eft})
    elseif fromDirector.npcBook.name == "YiLiDan" then
        --self:startJianFang(100)
        --self:setTimeout(5*30, function() self:stopJianFang() end)
        self:startJianFang(smallSkillParam["add_def_factor"]*self.npcBook.armorVal)
        self:setTimeout(smallSkillParam["noname_data1"]*30/1000, function() self:stopJianFang() end)
    elseif fromDirector.npcBook.name == "ShahelasiZhuMu" then
        --silence skill, make others can't fire skill
        --self:setSilence(true)
        self:startSilence()
        local silenceTime = math.max(smallSkillParam["chenmotime"]*30/1000, 30)
        self:setTimeout(silenceTime,function() self:stopSilence() end)
    elseif fromDirector.npcBook.name == "AErSaSi" then
        self:startAErSaSiShield(hp)
        local time = math.max(smallSkillParam["noname_data1"]*30/1000,30)
        self:setTimeout(time,function()
            self:stopAErSaSiShield()
        end)
    elseif fromDirector.npcBook.name == "LuKui" then
        self:startFan()
        local time = math.max(smallSkillParam["noname_data1"]*30/1000, 30)
        self:setTimeout(time,function()
            self:stopFan()
        end)
    elseif fromDirector.npcBook.name == "LaSuoLiAn" then
        self:startKuangBao()
        local time = math.max(smallSkillParam["noname_data1"]*30/1000, 30)
        self:setTimeout(time, function()
            self:stopKuangBao()
        end)
    elseif fromDirector.npcBook.name == "AKeMengDe" then
        local effect = self:rollEffectType(fromKey, hp)
        local hurt = math.max(effect.hurt - self:getArmorVal(),1)
        hurt = math.floor(hurt)
        --self.hp = self.hp - hurt
        self:subHp(hurt)
        actorAgent:actCMD({t="smalled", key=self.key, hp=hurt, fKey=fromKey, eft=effect.eft})
        self:startSilence()
        local silenceTime = math.max(smallSkillParam["chenmotime"]*30/1000, 30)
        self:setTimeout(silenceTime,function() self:stopSilence() end)
    else
        local effect = self:rollEffectType(fromKey, hp)
        local hurt = math.max(effect.hurt - self:getArmorVal(),1)
        hurt = math.floor(hurt)
        --self.hp = self.hp - hurt
        self:subHp(hurt)
        actorAgent:actCMD({t="smalled", key=self.key, hp=hurt, fKey=fromKey, eft=effect.eft})
    end
    
    if self:checkDie() == true and self.isLive then
        self.isLive = false
        --actorAgent:actCMD({t="die", key=self.key})
        directorAgent:onDie(self.key)
    end
    
    if fromDirector.npcBook.name == "DiXueLingZhu" then
        self:setPauseType(5, smallSkillParam)
    elseif fromDirector.npcBook.name == "WaSiQi" then
        self:setPauseType(1, smallSkillParam)
    end
end

function NpcDirector:onBigSkill(fromKey, bigSkillParam)
    local hp = bigSkillParam.hp
    local fromDirector = directorAgent:getDirector(fromKey)
    if fromDirector.npcBook.name == "WoJin" then
        self:startSheep()
        local sheepTime = math.max(bigSkillParam["xuanyuntime"]*30/1000, 60)
        self:setTimeout(sheepTime,function() self:stopSheep() end)
    elseif fromDirector.npcBook.name == "WaSiQi" then
        for i=1, 5 do
            local time = math.floor(30*i*0.5)
            self:setTimeout(time, function()
                print("~~~~~~~~~~~~~~~~WaSiQi Big SKill ~~~~~~~~~~~~~")
                self:onPersistentSkill(fromKey, hp/5)
            end)
        end
    elseif fromDirector.npcBook.name == "AErSaSi" or fromDirector.npcBook.name == "Tyrande" then
        local hurt = 1*hp
        --self.hp = self.hp - hurt
        self:subHp(hurt)
        actorAgent:actCMD({t="biged", key=self.key, hp=hurt, fKey=fromKey, eft=0})

    elseif fromDirector.npcBook.name == "KeLuoMaGuSi" then
        -- do nothing
    elseif fromDirector.npcBook.name == "ZhuRiZhe" then
        self:startJianFang(bigSkillParam["add_def_factor"]*self.npcBook.armorVal)
        self:setTimeout(bigSkillParam["noname_data1"]*30/1000, function() self:stopJianFang() end)
    else
        local effect = self:rollEffectType(fromKey, hp)
        local hurt = math.max(effect.hurt - self:getArmorVal(),1)
        hurt = math.floor(hurt)
        --self.hp = self.hp - hurt
        self:subHp(hurt)
        actorAgent:actCMD({t="biged", key=self.key, hp=hurt, fKey=fromKey, eft=effect.eft})
    end

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
        bigedHandler(bigSkillParam)
    end
end

function NpcDirector:onGlobalSkill(hp)
    local hurt = math.max(hp,1)
    --self.hp = self.hp - hurt
    self:subHp(hurt)
    actorAgent:actCMD({t="global_in", key=self.key, hp=hurt})
    
    if self:checkDie() == true and self.isLive then
        self.isLive = false
        --actorAgent:actCMD({t="die", key=self.key})
        directorAgent:onDie(self.key)
    end
end

function NpcDirector:onPersistentSkill(fromKey, hp)
    local fromDirector = directorAgent:getDirector(fromKey)
    if fromDirector and fromDirector.npcBook.name == "Tyrande" then
        self:onAddHp(self.key, -hp)
        return
    end
    
    if self.hp-hp >= self.npcBook.maxHp then
        hp =  self.hp - self.npcBook.maxHp
        self.hp = self.npcBook.maxHp
    else
        --self.hp = self.hp-hp
        self:subHp(hp)
    end
    actorAgent:actCMD({t="persistent_in", key=self.key, fKey=fromKey,hp=hp, direction=self.direction})

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
    --local fromDirector = directorAgent:getDirector(fromKey)
    --local dt = math.abs(fromDirector.row - self.row)/10
    local dt = 0.5
    self:setTimeout(math.floor(dt*30), function() self:subHp(-hp) end)
    actorAgent:actCMD({t="addHp", key=fromKey, tkey=self.key, hp= -hp, dt=dt})
end

function NpcDirector:onAddHp2(fromKey, hp)
    --local fromDirector = directorAgent:getDirector(fromKey)
    --local dt = math.abs(fromDirector.row - self.row)/10
    local dt = 1.5
    self:setTimeout(math.floor(dt*30), function() self:subHp(-hp) end)
    actorAgent:actCMD({t="addHp2",key=fromKey, tkey=self.key, hp= -hp, dt=dt})
end

function NpcDirector:onJieDu()
    self.isZhongDu = false
    actorAgent:actCMD({t="jiedu_s", key=self.key})
    self:setTimeout(20, function() 
        actorAgent:actCMD({t="jiedu_o", key=self.key})
    end)
end

function NpcDirector:onShengGuang(data)
    self.isXuRuo = false
    self.isZhongDu = false
    actorAgent:actCMD({t="shengguang_s", key=self.key, seq=data.seq})
    self:setTimeout(30*data.dt, function() 
        actorAgent:actCMD({t="shengguang_o", key=self.key})
    end)
end

function NpcDirector:onTuTeng(data)
    self.isTuTeng = true
    self.add_atk_factor = data.add_atk_factor
    self.add_def_factor = data.add_def_factor
    actorAgent:actCMD({t="tuteng_s", key=self.key})
    self:setTimeout(data.dt*30, function() 
        self.isTuTeng = false
        actorAgent:actCMD({t="tuteng_o", key=self.key})
    end)
end

function NpcDirector:onXuRuo(data)
    self.isXuRuo = true
    actorAgent:actCMD({t="xuruo_s", key=self.key})
    self:setTimeout(data.dt*30, function() 
        self.isXuRuo = false
        actorAgent:actCMD({t="xuruo_o", key=self.key})
    end)
end

function NpcDirector:onZhaoHuanFeiTing(data)
    self.isFeiTing = true
    actorAgent:actCMD({t="zhaohuanfeiting_s", key=self.key})
    self:setTimeout(data.dt*30, function() 
        self.isFeiTing = false
        actorAgent:actCMD({t="zhaohuanfeiting_o"})
    end)
end

function NpcDirector:onLeiDian(data)
    self.isLeiDian = true
    actorAgent:actCMD({t="leidian_s", key=self.key})
    local dt = math.max(data.dt*30, 45)
    self:setTimeout(45, function() 
        self.isLeiDian = false
        actorAgent:actCMD({t="leidian_o", key=self.key})
        local param = {}
        param.fromKey = self.key
        param.hp = 0
        param.xuanyuntime = data.dt
        param.chenmotime = 0
        param.noname_data1 = 0
        param.noname_data2 = 0
        param.noname_data3 = 0
        self:setPauseType(8, param)
    end)
end

function NpcDirector:onYanWu(data)
    self.isYanWu = true
    self.add_miss_factor = data.add_miss_factor
    actorAgent:actCMD({t="yanwu_s", key=self.key})
    self:setTimeout(data.dt*30, function() 
        self.isYanWu = false
        actorAgent:actCMD({t="yanwu_o", key=self.key})
    end)
end

function NpcDirector:onGPS(data)
    self.isGPS = true
    self.add_miss_factor = data.add_miss_factor
    actorAgent:actCMD({t="weixingdingwei_s", key=self.key})
    self:setTimeout(5*30, function() 
        self.isGPS = false
        actorAgent:actCMD({t="weixingdingwei_o", key=self.key})
    end)
end

function NpcDirector:handleGlobalSkill(data)
    self:setPauseType(3, data)
    self.globalSkillData = {}
    for k, v in pairs(data) do
        self.globalSkillData[k]=v
    end
end

function NpcDirector:onCiBao(data)
    self.isCiBao = true
    actorAgent:actCMD({t="cibao_s", key=self.key,seq=data.seq})

    self:setTimeout(data.dt*30, function()
        self.isCiBao = false
        actorAgent:actCMD({t="cibao_o", key=self.key})
    end)
    --[[
    local hurtHp = data.hp
    if self:isTower() or self:isRecruitHero() then
        hurtHp = 1
    end
    self:setTimeout(10, function()
        self:onAttack(self.key,hurtHp)
    end)
    ]]
end

function NpcDirector:onHongZha(data)
    actorAgent:actCMD({t="hongzha_s", key=self.key, direction=self.npcBook.direction})
    local hurtHp = data.hp
    if self:isTower() or self:isRecruitHero() then
        hurtHp = 1
    end
    if self:isTower() or self:isRecruitHero() then
        for i=1, data.slice_num do
            self:setTimeout(i*data.slice_dt*30, function()
                self:onAttack(self.key,hurtHp/5)
            end)
        end
    end
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

function NpcDirector:stop()
    --print("[NpcDirector] stop"..self.npcBook.debug_name)
    self:clearTimerList()
    if self:getCurrPauseType() ~= 0 then
        self:pauseOver()
        self:setPauseType(0, nil)
    end
    
    self:changeState(self.standState)
    self.isStop = true
end

function NpcDirector:restart()
    self.isStop = false
end

function NpcDirector:isStop()
    return self.isStop
end

------------[[Internal Function]]-----------
function NpcDirector:initM(npcBook)
    self.npcBook = npcBook
    self.key = npcBook.key
    self.direction = npcBook.direction
    
    if npcBook.row and npcBook.col then
        self.row = npcBook.row
        self.col = npcBook.col
    else
        if self:isTower() then
            if not (npcBook.row and npcBook.col) then
                local cell = directorAgent:assignTowerCell()
                self.row = cell.row + 5
                self.col = cell.col
                if self.npcBook.npcId==20000 or self.npcBook.npcId==20017 then
                    self.row = self.row-5
                end
                if self.npcBook.npcId==20002 then
                    self.row = 29
                end
            end
        else
            --[[
            if self.direction == 1 then
                self.row = 12 - npcBook.attackRange
            else
                self.row = 18 + npcBook.attackRange + 6
            end
            self.col = directorAgent:assignCol(self.key, self.row)
            ]]
            if self.direction == 1 then
                self.row = 12 - npcBook.enterOrder
            else
                self.row = 18 + npcBook.enterOrder + 6
            end
            
            self.col = 4
            if npcBook.enterOrder % 2 == 0 then
                self.col = 2
            end
        end
    end
    
    self.position = directorAgent:calPosition(self.row, self.col)
    if npcBook.position then
        --self.position = npcBook.position
    end
    
    self.globalState = GlobalState:create()
    self.pauseState = PauseState:create()
    self.standState = StandState:create()
    self.walkState = WalkState:create()
    self.normalAttackState = NormalAttackState:create()
    self.smallSkillState = SmallSkillState:create()
    self.bigSkillState = BigSkillState:create()
    self.fuckedState = FuckedState:create()
    self.restState = RestState:create()
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
    self.restTimer = 0
    
    -- npc's hp
    self.hp = self.npcBook.currHp
    self.attackSq = 0
    self.smallSkillSq = 0
    self.bigSkillSq = 0
    --self.speedX = const_speedX_num
    self.speedX = 0
    self.speedY = 0
    self.bigSkillPower = 0
    
    -- biged skill handler
    self.bigedHandlers = {
        ["Captain"] = function(param) self:bigedHandler_Captain(param) end,
        ["FengXing"] = function(param) self:bigedHandler_FengXing(param) end,
        ["DiXueLingZhu"] = function(param) self:bigedHandler_Captain(param) end,
        ["Kelthuzad"] = function(param) self:bigedHandler_Captain(param) end,
        ["LaGeNaLuoSi"] = function(param) self:bigedHandler_LaGeNaLuoSi(param) end,
        ["KeLuoMaGuSi"] = function(param) self:bigedHandler_KeLuoMaGuSi(param) end,
        ["LuKui"] = function(param) self:bigedHandler_LuKui(param) end,
    }
    
    self.isFucked = false
    self.isLive = true
    self.isStop = false
    self.zOrder = (11-self.col)*100
    self.isXuRuo = false
    self.isBigSkillFired = false
    self.isTuTeng = false
    self.isZhongDu = false
    self.isGPS = false
    self.isYanWu = false
    self.isFeiTing = false
    self.isLeiDian = false
    self.isCiBao = false
    self.isInBigSkillRange = true
    self.jianFangVal = 0
    self.pauseParamSet = {}
    self.recruit_num = 8
    
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
    if self.npcBook.npcId == 20005 or self.npcBook.npcId == 20004 then 
        return false
    elseif self.npcBook.npcId == 20000 or self.npcBook.npcId == 20002 or self.npcBook.npcId == 20017 then
        return true
    end
    return false
end

function NpcDirector:isRecruitHero()
    if self.npcBook.npcId == 20005 or self.npcBook.npcId == 20004 or self.npcBook.npcId == 20017 then 
        return true
    else
        return self.npcBook.npcId >= 20010
    end
end

function NpcDirector:canBigSkill()
    if self.npcBook.name == "mutong" or self:isRecruitHero() or 
       self.npcBook.name == "Tower3" or self.npcBook.name == "Cannon" then 
        return false 
    end
    if self:getSilence() or self:getSheep() then return false end
    self.bigSkillTargets = directorAgent:getBigSkillTargets(self.key)
    if #self.bigSkillTargets <= 0 then return false end
    return true
end

function NpcDirector:canSmallSkill()
    if self:isTower() or self:isRecruitHero() 
       or self.npcBook.name == "Tower3" or self.npcBook.name == "JuDuZhanChe" then 
        return false 
    end
    if self:getSilence() or self:getSheep() then return false end
    return true
end

function NpcDirector:canNormalAttack()
    if self.npcBook.name == "mutong" then return false end
    if self:getSheep() then return false end
    --if self.isCiBao == true then return false end
    if self.speedX==0 then
        return true
    end
    return false
end

function NpcDirector:startSilence()
    self.isSilence = true
    actorAgent:actCMD({t="silence_s", key=self.key, sid=self.npcBook.soulId})
end

function NpcDirector:stopSilence()
    self.isSilence = false
    actorAgent:actCMD({t="silence_o", key=self.key, sid=self.npcBook.soulId})
end

function NpcDirector:startJianFang(jianFangVal)
    print("[NpcDirector]startJianFang")
    self.jianFangVal = jianFangVal or 0
    actorAgent:actCMD({t="jianfang_s", key=self.key, val=self.jianFangVal})
end

function NpcDirector:stopJianFang()
    self.jianFangVal = 0
end

function NpcDirector:startAErSaSiShield(AErSaSi_hp)
    self.AErSaSi_hp = AErSaSi_hp
    actorAgent:actCMD({t="AErSaSi_shield_s", key=self.key, hp=AErSaSi_hp})
end

function NpcDirector:stopAErSaSiShield()
    if self.AErSaSi_hp then
        self.AErSaSi_hp = nil
        actorAgent:actCMD({t="AErSaSi_shield_o", key=self.key})
    end
end

function NpcDirector:startFan()
    self.isSilence = true
    self.direction = self.direction*-1
    self.isFan = true
    actorAgent:actCMD({t="Fan_s", key=self.key})
end

function NpcDirector:stopFan()
    if self.isFan then
        self.direction = self.direction*-1
        self.isFan = false
        self.isSilence = false
        actorAgent:actCMD({t="Fan_o", key=self.key})
    end
end

function NpcDirector:startKuangBao()
    self.isKuangBao = true
end

function NpcDirector:stopKuangBao()
    self.isKuangBao = false
end

function NpcDirector:startZhongDu()
    self.isZhongDu = true
end

function NpcDirector:stopZhongDu()
    self.isZhongDu = false
end

function NpcDirector:subHp(hurt)
    if hurt > self.npcBook.maxHp*0.4 then
        self:setFucked()
    end
    if self.AErSaSi_hp then
        self.AErSaSi_hp = self.AErSaSi_hp - hurt
        if self.AErSaSi_hp <= 0 then
            self:stopAErSaSiShield()
        end
        return
    end
    self.hp = math.min(self.hp - hurt, self.npcBook.maxHp)
end

function NpcDirector:startSheep()
    if self.isCiBao==true then return end
    self.isSheep = true
    actorAgent:actCMD({t="sheep_s", key=self.key})
end

function NpcDirector:stopSheep()
    self.isSheep = false
    actorAgent:actCMD({t="sheep_o", key=self.key})
end

function NpcDirector:getSilence()
    self.isSilence = self.isSilence or false
    return self.isSilence
end

function NpcDirector:getSheep()
    self.isSheep = self.isSheep or false
    return self.isSheep
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

function NpcDirector:clearTimerList()
    for i=1, #self.timerList do
        local timer = self.timerList[i]
        if timer.t > 0 then
            timer.cb()
        end
    end
    self.timerList = {}
end

-----------------biged handlers defination------------------
function NpcDirector:bigedHandler_Captain(param)
    --when global skill, other pause type not processed
    --if self.pauseType == 3 then return end
    self:setPauseType(1, param)
end

function NpcDirector:bigedHandler_FengXing(param)
    --when global skill, other pause type not processed
    --if self.pauseType == 3 then return end
    self:setPauseType(2, param)
end

function NpcDirector:bigedHandler_LaGeNaLuoSi(param)
    directorAgent:onAttack_LaGeNaLuoSi(param.fromKey, self.key, math.floor(param.hp/3))
end

function NpcDirector:bigedHandler_KeLuoMaGuSi(param)
    self:setPauseType(4, param)
end

function NpcDirector:bigedHandler_LuKui(param)
    self:setPauseType(6, param)
end

function NpcDirector:rollEffectType(fromKey, hp)
    local from = directorAgent:getDirector(fromKey)
    local randomNum = math.random(1,10000)
    
    if from.npcBook.dex-self.npcBook.agl <= 0 then
        return {eft=2, hurt=hp*0}
    end
    
    local baoji_point = self.npcBook.cri
    local gedang_point = baoji_point + self.npcBook.block
    local miss_point = gedang_point + from.npcBook.dex-self.npcBook.agl
    if from.isYanWu then
        miss_point = miss_point*from.add_miss_factor
    end
    if from.isGPS then
        miss_point = miss_point*from.add_miss_factor
    end
    if from.isTuTeng then
        
    end
    
    --baoji
    if randomNum <= baoji_point then
        return {eft=4, hurt=hp*2}
    --gedang
    elseif randomNum>baoji_point and randomNum<=gedang_point then
        return {eft=3, hurt=hp/2}
    --miss
    elseif randomNum>gedang_point and randomNum<=miss_point then
        return {eft=2, hurt=hp*0}
    else
        return {eft=1, hurt=hp}
    end
end

return NpcDirector
