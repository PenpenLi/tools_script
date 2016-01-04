//
//  Bullet.cpp
//  HiFly
//
//  Created by chris li on 14-5-14.
//
//

#include "Bullet.h"
#include "util/GameSound.h"

bool Bullet::init()
{
    if (Node::init())
    {
        return true;
    }
    
    return false;
}

