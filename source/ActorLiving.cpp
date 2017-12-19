//
//  ActorLiving.cpp
//  Pestle
//
//  Created by Braeden Atlee on 12/19/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "ActorLiving.hpp"


void ActorLiving::update(Room* room, Display* display, double delta, double xOff, double yOff){
    ActorMoving::update(room, display, delta, xOff, yOff);
    if(hp <= 0 && alive){
        alive = false;
        die();
    }
}
