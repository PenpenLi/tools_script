----------------------------[[FSM State Defination]]--------------------------------
--[[Stand State Defination]]--
local StandState = class("StandState")
function StandState:create()
    local state = StandState.new()
    state.normalAttackTimer = math.random(200)
    state.smallSkillTimer = 0
    state.bigSkillTimer = 0
    return state
end
function StandState:enter(npcDirector)
    --cclog(npcDirector.key.." enter StandState")
    DIRECTOR_AGENT:fireCMD({cmdId="stand", npcKey=npcDirector.key})
end
function StandState:execute(npcDirector)
    self.normalAttackTimer = self.normalAttackTimer + 1
    self.smallSkillTimer = self.smallSkillTimer + 1
    self.bigSkillTimer = self.bigSkillTimer + 1
    
    --check for global skill state
    if npcDirector:getGlobalSkillId() > 0 then
        npcDirector:changeState(npcDirector.globalSkillState)
        return
    end
    
    --check for DizzyState
    if npcDirector:getDizzyTimerAndClear() > 0 then
        npcDirector:changeState(npcDirector.dizzyState)
        return
    end
    
    --check for FallbackState
    if npcDirector:getFallbackTimerAndClear() > 0 then
        npcDirector:changeState(npcDirector.fallbackState)
        return
    end
    
    --check for WalkState
    if npcDirector:isInAttackRange() == false then
        npcDirector:changeState(npcDirector.walkState)
        return
    end
    
    --check for BigSkillState
    if self.bigSkillTimer >= 30*npcDirector.npcBook.bigSkillInterval then
        self.bigSkillTimer = 0
        npcDirector:changeState(npcDirector.bigSkillAttackState)
        return
    end
    
    --check for SmallSkillState
    if self.smallSkillTimer >= 30*npcDirector.npcBook.smallSkillInterval then
        self.smallSkillTimer = 0
        npcDirector:changeState(npcDirector.smallSkillAttackState)
        return
    end
    
    --check for AttackState
    if self.normalAttackTimer >= 30*npcDirector.npcBook.attackInterval then
        self.normalAttackTimer = 0
        npcDirector:changeState(npcDirector.normalAttackState)
        return
    end
end
function StandState:exit(npcDirector)
end

--[[Walk State Defination]]--
local WalkState = class("WalkState")
function WalkState:create()
    local state = WalkState.new()
    state.timer = 0
    return state
end
function WalkState:enter(npcDirector)
    --cclog(npcDirector.key.." enter WalkState")
    self.timer = npcDirector:calWalkTimer()
    --[[
    printJSON({
        cmdId="walk", 
        npcKey=npcDirector.key, 
        duration=self.timer/30, 
        targetPosition=npcDirector.targetPosition
    })
    ]]--
    DIRECTOR_AGENT:fireCMD({
        cmdId="walk", 
        npcKey=npcDirector.key, 
        duration=self.timer/30, 
        targetPosition=npcDirector.targetPosition
    })
end
function WalkState:execute(npcDirector)
    self.timer = self.timer - 1
    
    --check for global skill state
    --[[
    if npcDirector:getGlobalSkillId() > 0 then
        npcDirector:changeState(npcDirector.globalSkillState)
        return
    end
    ]]--
    
    --check for DizzyState
    if npcDirector:getDizzyTimerAndClear() > 0 then
        npcDirector:changeState(npcDirector.dizzyState)
        return
    end
    
    --check for FallbackState
    if npcDirector:getFallbackTimerAndClear() > 0 then
        npcDirector:changeState(npcDirector.fallbackState)
        return
    end
    --check for StandState
    if self.timer <= 0 then
        npcDirector:changeState(npcDirector.standState)
    end
end
function WalkState:exit(npcDirector)
    npcDirector:updateWalkedPosition()
end

--[[NormalAttack State Defination]]--
local NormalAttackState = class("NormalAttackState")
function NormalAttackState:create()
    local state = NormalAttackState.new()
    state.timer = 0
    state.tempSubHpVal = 0
    return state
end
function NormalAttackState:enter(npcDirector)
    --cclog(npcDirector.key.." enter NormalAttackState")
    self.timer = npcDirector:calNormalAttackTimer()
    self.tempSubHpVal = npcDirector:calNormalAttackHurtVal()

    DIRECTOR_AGENT:fireCMD({
        cmdId = "normalAttack", 
        npcKey = npcDirector.key, 
        targetNpcKey = npcDirector.nearestAttackNpc.key,
        subHpVal = self.tempSubHpVal,
        duration = self.timer/30
    })
    
    if npcDirector.nearestAttackNpc then
        DIRECTOR_AGENT:onNormalAttackSuccess({
            npcKey=npcDirector.key, 
            nearestAttackNpc=npcDirector.nearestAttackNpc.key, 
            subHpVal=self.tempSubHpVal
        })
    end
end
function NormalAttackState:execute(npcDirector)
    self.timer = self.timer - 1
    
    --check for global skill state
    if npcDirector:getGlobalSkillId() > 0 then
        npcDirector:changeState(npcDirector.globalSkillState)
        return
    end

    --check for DizzyState
    if npcDirector:getDizzyTimerAndClear() > 0 then
        npcDirector:changeState(npcDirector.dizzyState)
        return
    end
    
    --check for FallbackState
    if npcDirector:getFallbackTimerAndClear() > 0 then
        npcDirector:changeState(npcDirector.fallbackState)
        return
    end
    
    --check for StandState
    if self.timer <= 0 then
        --[[
        if npcDirector.nearestAttackNpc then
            DIRECTOR_AGENT:onNormalAttackSuccess({
                npcKey=npcDirector.key, 
                nearestAttackNpc=npcDirector.nearestAttackNpc.key, 
                subHpVal=self.tempSubHpVal
            })
        end
        ]]--
        npcDirector:changeState(npcDirector.standState)
    end
end
function NormalAttackState:exit(npcDirector)
    self.tempSubHpVal = 0
end

--[[SmallSkillAttack State Defination]]--
local SmallSkillAttackState = class("SmallSkillAttackState")
function SmallSkillAttackState:create()
    local state = SmallSkillAttackState.new()
    state.timer = 0
    state.tempSubHpVal = 0
    return state
end
function SmallSkillAttackState:enter(npcDirector)
    --cclog(npcDirector.key.." enter SmallSkillAttackState")
    self.timer = npcDirector:calSmallSkillAttackTimer()
    self.tempSubHpVal = npcDirector:calSmallSkillHurtVal()
    DIRECTOR_AGENT:fireCMD({
        cmdId = "smallSkillAttack", 
        npcKey = npcDirector.key, 
        targetNpcKey = npcDirector.nearestAttackNpc.key,
        subHpVal = self.tempSubHpVal,
        duration = self.timer/30.0
    })
end
function SmallSkillAttackState:execute(npcDirector)
    self.timer = self.timer - 1
    
    --check for global skill state
    if npcDirector:getGlobalSkillId() > 0 then
        npcDirector:changeState(npcDirector.globalSkillState)
        return
    end
    
    --check for DizzyState
    if npcDirector:getDizzyTimerAndClear() > 0 then
        npcDirector:changeState(npcDirector.dizzyState)
        return
    end
    
    --check for FallbackState
    if npcDirector:getFallbackTimerAndClear() > 0 then
        npcDirector:changeState(npcDirector.fallbackState)
        return
    end

    --check for StandState
    if self.timer <= 0 then
        self.timer = nil
        npcDirector:changeState(npcDirector.standState)
    end
end
function SmallSkillAttackState:exit(npcDirector)
    DIRECTOR_AGENT:onSmallSkillAttackSuccess({npcKey=npcDirector.key, nearestAttackNpc=npcDirector.nearestAttackNpc.key, subHpVal=self.tempSubHpVal})
    self.tempSubHpVal = 0
end

--[[BigSkillAttack State Defination]]--
local BigSkillAttackState = class("BigSkillAttackState")
function BigSkillAttackState:create()
    local state = BigSkillAttackState.new()
    state.timer = 0
    state.tempSubHpVal = 0
    return state
end
function BigSkillAttackState:enter(npcDirector)
    --cclog(npcDirector.key.." enter BigSkillAttackState")
    self.timer = npcDirector:calBigSkillAttackTimer()
    self.tempSubHpVal = npcDirector:calBigSkillHurtVal()
    DIRECTOR_AGENT:fireCMD({
        cmdId = "bigSkillAttack", 
        npcKey = npcDirector.key, 
        targetNpcKey = npcDirector.nearestAttackNpc.key,
        subHpVal = self.tempSubHpVal,
        duration = self.timer/30
    })
end
function BigSkillAttackState:execute(npcDirector)
    self.timer = self.timer - 1
    
    --check for global skill state
    if npcDirector:getGlobalSkillId() > 0 then
        npcDirector:changeState(npcDirector.globalSkillState)
        return
    end
    
    --check for DizzyState
    if npcDirector:getDizzyTimerAndClear() > 0 then
        npcDirector:changeState(npcDirector.dizzyState)
        return
    end
    
    --check for FallbackState
    if npcDirector:getFallbackTimerAndClear() > 0 then
        npcDirector:changeState(npcDirector.fallbackState)
        return
    end

    --check for StandState
    --if self.timer <= 0 or npcDirector:getHurtTagAndClear()==1 then
    if self.timer <= 0 then
        --big skill sucess
        if self.timer <= 0 then
            DIRECTOR_AGENT:onBigSkillAttackSuccess({npcKey=npcDirector.key, nearestAttackNpc=npcDirector.nearestAttackNpc.key, subHpVal=self.tempSubHpVal})
            self.tempSubHpVal = 0
        end
        npcDirector:changeState(npcDirector.standState)
        return
    end
end
function BigSkillAttackState:exit(npcDirector)
end

--[[Dizzy State Defination]]--
local DizzyState = class("DizzyState")
function DizzyState:create()
    local state = DizzyState.new()
    state.timer = 0
    return state
end
function DizzyState:enter(npcDirector)
 cclog(npcDirector.key.." enter DizzyState")
    self.timer = npcDirector:calDizzyTimer()
    self.tempSubHpVal = npcDirector:calDizzyHurtVal()
    DIRECTOR_AGENT:fireCMD({
        cmdId = "dizzy", 
        npcKey = npcDirector.key,
        targetNpcKey = npcDirector.key,
        subHpVal = self.tempSubHpVal,
        duration = self.timer/30
    })
end
function DizzyState:execute(npcDirector)
    self.timer = self.timer - 1
    if self.timer <= 0 then
        npcDirector:changeState(npcDirector.standState)
    end
end
function DizzyState:exit(npcDirector)
    cclog(npcDirector.key.." exit DizzyState")
end

--[[Fallback State Defination]]--
local FallbackState = class("FallbackState")
function FallbackState:create()
    local state = FallbackState.new()
    state.timer = 0
    return state
end
function FallbackState:enter(npcDirector)
    cclog(npcDirector.key.." enter FallbackState")
    self.timer = npcDirector:calFallbackTimer()
    self.tempSubHpVal = npcDirector:calFallbackHurtVal()
    DIRECTOR_AGENT:fireCMD({
        cmdId="fallback", 
        npcKey=npcDirector.key, 
        duration=self.timer/30, 
        subHpVal=self.tempSubHpVal,
        targetPosition=npcDirector.targetPosition
    })
end
function FallbackState:execute(npcDirector)
    self.timer = self.timer - 1
    npcDirector:getFallbackTimerAndClear()
    
    if self.timer <= 0 then
        npcDirector:changeState(npcDirector.standState)
    end
end
function FallbackState:exit(npcDirector)
    cclog(npcDirector.key.." exit FallbackState")
end

--[[Global Skill State Defination]]--
local GlobalSkillState = class("GlobalSkillState")
function GlobalSkillState:create()
    local state = GlobalSkillState.new()
    state.timer = 0
    return state
end
function GlobalSkillState:enter(npcDirector)
    cclog(npcDirector.key.." enter GlobalSkillState")
    self.timer = npcDirector:calGlobalSkillTimer()
    self.tempSubHpVal = npcDirector:calGlobalSkillHurtVal()
    --[[
    DIRECTOR_AGENT:fireCMD({
        cmdId="globalSkill", 
        npcKey=npcDirector.key, 
        duration=self.timer/30, 
        subHpVal=self.tempSubHpVal,
        targetPosition=npcDirector.targetPosition
    })
    ]]--
end
function GlobalSkillState:execute(npcDirector)
    self.timer = self.timer - 1
    
    if  self.timer % 30 == 0 and npcDirector.npcBook.direction ==-1 then
        npcDirector.npcBook.currHp = npcDirector.npcBook.currHp-100
    end
    
    if self.timer <= 0 then
        DIRECTOR_AGENT:onGlobalSkill({npcKey=npcDirector.key, subHpVal=self.tempSubHpVal})
        npcDirector:changeState(npcDirector.standState)
    end
end
function GlobalSkillState:exit(npcDirector)
    cclog(npcDirector.key.." exit GlobalSkillState")
    npcDirector:setGlobalSkillId(-1)
end
----------------------------[[NpcDirector defination]]------------------------------
local StateMachine = require "scene.battle.directors.StateMachine"
local NpcDirector = class("NpcDirector",function()
    return {}
end)

function NpcDirector:create(param)
    local npcDirector = NpcDirector.new()
    npcDirector:initM(param)
    return npcDirector
end

function NpcDirector:initM(param)
    self.npcBook = param.npcBook
    self.key = param.npcBook.key
    self.hp = param.npcBook.currHp
    self.standState = StandState:create()
    self.walkState = WalkState:create()
    self.normalAttackState = NormalAttackState:create()
    self.smallSkillAttackState = SmallSkillAttackState:create()
    self.bigSkillAttackState = BigSkillAttackState:create()
    self.dizzyState = DizzyState:create()
    self.fallbackState = FallbackState:create()
    self.globalSkillState = GlobalSkillState:create()
    self.fsm = StateMachine:create({owner=self, currentState=self.standState})
end

function NpcDirector:updateM()
    self.fsm:update()
end

function NpcDirector:changeState(state)
    self.fsm:changeState(state)
end

function NpcDirector:isInAttackRange()
    self.nearestAttackNpc = DIRECTOR_AGENT:getNearestAttackNpc(self)
    if self.nearestAttackNpc then
        local distance = math.abs(self.npcBook.row - self.nearestAttackNpc.npcBook.targetRow)
        if  distance <= self.npcBook.attackRange then
            return true
        else
            return false
        end
    end

    return true
end

function NpcDirector:calNormalAttackHurtVal()
    --self.nearestAttackNpc's armorVal and self's attackVal
    --return self.npcBook.normalAttackVal
    --return 80
    return math.random(50, 80)
end

function NpcDirector:calSmallSkillHurtVal()
    --self.nearestAttackNpc's armorVal and self's attackVal
    --return self.npcBook.smallSkillAttackVal
    --return 150
    return math.random(100, 150)
end

function NpcDirector:calBigSkillHurtVal()
    --self.nearestAttackNpc's armorVal and self's attackVal
    --return self.npcBook.bigSkillAttackVal
    return math.random(200, 280)
    --return 250
end

function NpcDirector:calDizzyHurtVal()
    return 500
end

function NpcDirector:getHurtTagAndClear()
    self.hurtTag = self.hurtTag or 0
    local ret = self.hurtTag
    self.hurtTag = 0
    return self.hurtTag
end

function NpcDirector:setHurtTag(tag)
    self.hurtTag = tag
end

function NpcDirector:setGlobalSkillId(globalSkillId)
    self.globalSkillId = globalSkillId
end

function NpcDirector:getGlobalSkillId()
    self.globalSkillId = self.globalSkillId or 0
    return self.globalSkillId
end

function NpcDirector:calGlobalSkillTimer()
    return 5*30
end
function NpcDirector:calGlobalSkillHurtVal()
    return 500
end

--[[this function can only be called by DirectorAgent]]--
function NpcDirector:subHp(hpVal)
    self.hp = self.hp - hpVal
    self.npcBook.currHp = self.hp
    --hurt by others, It will switch to stand state
    self:setHurtTag(1)
end

function NpcDirector:addHp(hpVal)
    self.hp = self.hp + hpVal
    if self.hp >= self.npcBook.maxHp then
        self.hp = self.npcBook.maxHp
    end
    self.npcBook.currHp = self.hp
end

function NpcDirector:getHp()
    return self.hp
end

function NpcDirector:calWalkTimer()
    self.nearestAttackNpc = DIRECTOR_AGENT:getNearestAttackNpc(self)
    if self.nearestAttackNpc == nil then 
        return 0.0001 
    end
    local distance = math.abs(self.npcBook.row - self.nearestAttackNpc.npcBook.targetRow)
    self.walkDistance = distance - self.npcBook.attackRange
    
    self.npcBook.targetRow = self.npcBook.row + self.npcBook.direction * self.walkDistance
    self.npcBook.targetCol = self.npcBook.col
    --print("[calWalkTimer]"..self.walkDistance..":"..self.npcBook.row..":"..self.npcBook.targetRow)
    --[[
    if self.npcBook.attackRange > 1 then
        self.npcBook.targetCol = self.npcBook.col
    else
        self.npcBook.targetCol = self.nearestAttackNpc.npcBook.col
    end
    ]]--
    self.targetPosition = DIRECTOR_AGENT:cell2Position({row=self.npcBook.targetRow, col= self.npcBook.targetCol})
    return self.walkDistance*25
end

function NpcDirector:calNormalAttackTimer()
    self.nearestAttackNpc = DIRECTOR_AGENT:getNearestAttackNpc(self)
    if self.nearestAttackNpc == nil then return 0 end
    local distance = math.abs(self.npcBook.row - self.nearestAttackNpc.npcBook.targetRow)
    if self.npcBook.attackRange > 1 then
        return 30*(distance/8 + self.npcBook.normalAttackReadyTime)
    else
        return self.npcBook.normalAttckDuration*30
    end
end

function NpcDirector:calSmallSkillAttackTimer()
    self.nearestAttackNpc = DIRECTOR_AGENT:getNearestAttackNpc(self)
    if self.nearestAttackNpc == nil then return 0 end
    local distance = math.abs(self.npcBook.row - self.nearestAttackNpc.npcBook.targetRow)
    if self.npcBook.attackRange > 1 then
        return 30*(distance/8 + self.npcBook.smallSkillReadyTime)
    else
        return self.npcBook.smallSkillDuration*30
    end
end

function NpcDirector:calBigSkillAttackTimer()
    self.nearestAttackNpc = DIRECTOR_AGENT:getNearestAttackNpc(self)
    if self.nearestAttackNpc == nil then return 0 end
    local distance = math.abs(self.npcBook.row - self.nearestAttackNpc.npcBook.targetRow)
    if self.npcBook.attackRange > 1 then
        return 30*(distance/8 + self.npcBook.bigSkillReadyTime)
    else
        return self.npcBook.bigSkillDuration*30
    end
end

function NpcDirector:calDizzyTimer()
    return self.calDizzyTimerVal
end

function NpcDirector:setDizzyTimer(dizzyTimer)
    self.dizzyTimer = dizzyTimer*30
end

function NpcDirector:getDizzyTimerAndClear()
    self.dizzyTimer = self.dizzyTimer or 0
    local ret = self.dizzyTimer
    self.dizzyTimer = 0
    self.calDizzyTimerVal = ret
    return ret
end

function NpcDirector:calFallbackTimer()
    return self.calFallbackTimerVal
end

function NpcDirector:setFallbackTimer(fallbackTimer)
    self.fallbackTimer = fallbackTimer*30
end

function NpcDirector:getFallbackTimerAndClear()
    self.fallbackTimer = self.fallbackTimer or 0
    local ret = self.fallbackTimer
    self.fallbackTimer = 0
    self.calFallbackTimerVal = ret
    return ret
end

function NpcDirector:calFallbackHurtVal()
    return 340
end

function NpcDirector:fallBack(fallBackRows)
    if self.npcBook.direction == 1 then
        self.npcBook.targetRow = self.npcBook.row - 2
        if self.npcBook.targetRow < 7 then
            self.npcBook.targetRow = 7
        end
        self.npcBook.row = self.npcBook.targetRow
    else
        self.npcBook.targetRow = self.npcBook.row + 2
        if self.npcBook.targetRow > 18 then
            self.npcBook.targetRow = 18
        end
        self.npcBook.row = self.npcBook.targetRow
    end
    self.npcBook.position = DIRECTOR_AGENT:cell2Position({row=self.npcBook.row, col= self.npcBook.col})
    self.targetPosition = self.npcBook.position
    self:setFallbackTimer(5/30)
end

function NpcDirector:updateWalkedPosition()
    self.npcBook.row = self.npcBook.targetRow
    self.npcBook.position = self.targetPosition
    self.walkDistance = 0
end

return NpcDirector
