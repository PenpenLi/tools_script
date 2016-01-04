//
//  StateMachine.h
//  AI-1
//
//  Created by chris li on 14-3-11.
//
//

#ifndef __AI_1__StateMachine__
#define __AI_1__StateMachine__

#include <cassert>
#include <string>

#include "State.h"

template <class T>
class StateMachine {
private:
    //point to the owner of this state machine
    T*            m_pOwner;
    
    //current state
    State<T>*     m_pCurrentState;
    
    //previous state
    State<T>*     m_pPreviousState;
    
    //global state is refreshed each the state machine is updated
    State<T>*     m_pGlobalState;
    
public:
    StateMachine(T* pOwner): m_pOwner(pOwner),
                             m_pCurrentState(NULL),
                             m_pPreviousState(NULL),
                             m_pGlobalState(NULL)
    {
    }
    
    virtual ~StateMachine() {}
    
    //current state getter
    State<T>* getCurrentState() const
    {
        return m_pCurrentState;
    }
    
    //current state setter
    void setCurrentState(State<T>* pState)
    {
        m_pCurrentState = pState;
    }
    
    //previous state getter
    State<T>* getPreviousState() const
    {
        return m_pPreviousState;
    }
    
    //previous state setter
    void setPreviousState(State<T>* pState)
    {
        m_pPreviousState = pState;
    }
    
    //global state getter
    State<T>* getGlobalState() const
    {
        return m_pGlobalState;
    }
    
    //global state setter
    void setGlobalState(State<T>* pState)
    {
        m_pGlobalState = pState;
    }
    
    //update the FSM, the key driver of state machine
    void update() const
    {
        if (m_pGlobalState)
        {
            m_pGlobalState->execute(m_pOwner);
        }
        
        if (m_pCurrentState)
        {
            m_pCurrentState->execute(m_pOwner);
        }
    }
    
    //change to new state
    void changeState(State<T>* pNewState)
    {
        assert(NULL != pNewState);
        
        m_pPreviousState = m_pCurrentState;
        
        m_pCurrentState->exit(m_pOwner);
        
        m_pCurrentState = pNewState;

        m_pCurrentState->enter(m_pOwner);
    }
    
    //revert to previous state
    void revertPreviousState()
    {
        changeState(m_pPreviousState);
    }
    
    //check if the current state is equal to the given state
    bool isInState(const State<T>& st) const
    {
        return typeid(*m_pCurrentState) == typeid(st);
    }
};

#endif /* defined(__AI_1__StateMachine__) */
