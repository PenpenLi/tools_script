//
//  State.h
//  AI-1
//
//  Created by chris li on 14-3-5.
//
//

#ifndef __AI_1__State__
#define __AI_1__State__
//------------------------------------------------------------------------
//
//  Name:   State.h
//
//  Desc:   abstract base class to define an interface for a state
//
//  Author: chris.li (letchrisgo@gmail.com)
//
//------------------------------------------------------------------------


template <class T>
class State {
public:
    
    virtual ~State(){}
    
    //execute when the state is entered
    virtual void enter(T* pEntity) = 0;
    
    //execute when each update occurs
    virtual void execute(T* pEntity) = 0;
    
    //execute when the state is exited
    virtual void exit(T* pEntity) = 0;
};

#endif /* defined(__AI_1__State__) */
