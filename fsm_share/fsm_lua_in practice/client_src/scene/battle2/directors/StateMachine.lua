--Created by chris.li letchrisgo@gmail.com
--State must has these functions {onEnter(); onExit(); execute();}

local StateMachine = class("StateMachine", function()
    return {}
end)

function StateMachine:ctor() 
end

function StateMachine:create(param)
    local stateMachine = StateMachine.new()
    stateMachine:initM(param)
    return stateMachine
end

function StateMachine:initM(param)
    self.owner = param.owner
    self.currentState = param.currentState
    self.previousState = nil
    self.globalState = param.globalState
end

function StateMachine:getCurrentState()
    return self.currentState
end

function StateMachine:setCurrentState(state)
    self.previousState = self.currentState
    self.currentState = state
end

function StateMachine:getPreviousState()
    return self.previousState
end

function StateMachine:setPreviousState(state)
    self.previousState = state 
end

function StateMachine:getGlobalState()
    return self.globalState
end

function StateMachine:setGlobalState(state)
    self.globalState = state
end

function StateMachine:update()
    if self.globalState then
        self.globalState:execute(self.owner)
    end

    if self.currentState then
        self.currentState:execute(self.owner)
    end
end

function StateMachine:changeState(state)
    if state == nil then return end

    self.previousState = self.currentState
    self.currentState:exit(self.owner)

    self.currentState = state
    self.currentState:enter(self.owner)
end

function StateMachine:pushState(state)
    self.previousState = self.currentState
    self.currentState = state
    self.currentState:enter(self.owner)
end

function StateMachine:popState()
    self.currentState:exit(self.owner)
    self.currentState = self.previousState
    self.previousState = nil
end

function StateMachine:revertPreviousState()
    self:changeState(self.previousState)
end

function StateMachine:isInState(state)
    if self.currentState == state then
        return true
    else
        return false
    end
end

return StateMachine

