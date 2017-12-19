//
//  ActorProjectile.cpp
//  Pestle
//
//  Created by Braeden Atlee on 12/19/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "ActorProjectile.hpp"

void ActorProjectile::update(Room* room, Display* display, double delta, double xOff, double yOff){
    ActorMoving::update(room, display, delta, xOff, yOff);
    
    if(hitSolid){
        markedForRemoval = true;
    }
    
}
