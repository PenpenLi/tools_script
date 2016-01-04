require("Cocos2d")

local ActorAgent = class("ActorAgent", function()
    return {}
end)

-----------------------[[External API]]-----------------------------
function ActorAgent:create(param)
    local actorAgent = ActorAgent.new()
    actorAgent:initM(param)
    return actorAgent
end

function ActorAgent:updateM()
	self.sq = self.sq + 1
	return self.isOver
end

function ActorAgent:actCMD(cmd)
    --printJSON(cmd)
    cmd.sq = self.sq
    table.insert(self.cmds, cmd)
end

function ActorAgent:onBattleOver(param)
	self.isOver = true
	self.battleResult = {endId=(self.fId-34)/31, leftAtk = param.leftAtks,leftDef = param.leftDefs,leftSeconds = 1}
end

function ActorAgent:getBattleCmds()
	return self.cmds
end

function ActorAgent:getBattleResult()
	return self.battleResult
end

function ActorAgent:isBattleOver()
	return self.isOver
end

function ActorAgent:startJX()
end

function ActorAgent:stopJX()
end

------------------------[[Internal function]]------------------------
function ActorAgent:ctor()
end

function ActorAgent:onEnter()
end

function ActorAgent:onExit()
end

function ActorAgent:initM(param)
	self.isOver = false
	self.sq = 0
	self.cmds = {}
	self.fId = param.fId
end

return ActorAgent