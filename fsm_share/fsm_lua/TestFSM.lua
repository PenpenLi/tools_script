require ("Util")

-- class: StateMachine
local StateMachine = require ("StateMachine")


-- class: StandState
local StandState = class("StandState")
function StandState:create()
    local state = StandState.new()
    state.timer = 0
    return state
end
function StandState:enter(npc)
	npc:startStand()
	self.timer = 0
end
function StandState:execute(npc)
	self.timer = self.timer + 1
	if self.timer > 10000000 then
		npc:changeState(npc.walkState)
	end
end
function StandState:exit(npc)
end


-- class: WalkState
local WalkState = class("WalkState")
function WalkState:create()
    local state = WalkState.new()
    state.timer = 0
    return state
end
function WalkState:enter(npc)
	npc:startWalk()
	self.timer = 0
end
function WalkState:execute(npc)
	self.timer = self.timer + 1
	if self.timer > 10000000 then
		npc:changeState(npc.standState)
	end
end
function WalkState:exit(npc)
end


-- class: NPC
local NPC = class("NPC", function()
    return {}
end)

function NPC:create(param)
    local npc = NPC.new()
    npc:init(param)
    return npc
end

function NPC:init(param)
	self.standState = StandState:create()
	self.walkState = WalkState:create()

	self.fsm = StateMachine:create({
        owner=self,
        currentState=self.standState
    })
end

function NPC:changeState(state)
    self.fsm:changeState(state)
end

function NPC:startStand()
	print("Now stand...\n")
end

function NPC:startWalk()
	print("Now walk...\n")
end

function NPC:update(dt)
	self.fsm:update()
end

------------------------- testing now!! ------------------------- 
local testNpc = NPC:create({})
while true do
	testNpc:update(1/60)
end


