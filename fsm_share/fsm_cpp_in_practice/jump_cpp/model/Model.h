//
//  Model.h
//  HiFly
//
//  Created by chris li on 14-4-21.
//
//

#ifndef HiFly_Model_h
#define HiFly_Model_h

class Model
{
    // calle this function when data is updated
    virtual void dataUpdated() = 0;
};

//////////////////////////////////////Model Define//////////////////////////////////////
#define EVENT_MODEL_PLAYER      "EVENT_MODEL_PLAYER"

#endif
