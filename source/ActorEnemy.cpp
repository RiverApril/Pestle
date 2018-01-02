//
//  ActorEnemy.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/27/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "ActorEnemy.hpp"
#include "Room.hpp"
#include "ActorPlayer.hpp"
#include "Geometry.hpp"

void ActorEnemy::update(Room* room, Display* display, double delta, double xOff, double yOff){
    ActorLiving::update(room, display, delta, xOff, yOff);

    if(!display){
        if(targetId == -1){
            ActorPlayer* nearPlayer = dynamic_cast<ActorPlayer*>(room->findNearestActor(px, py, [](Actor* actor, double distSq){
                Actor* player = dynamic_cast<ActorPlayer*>(actor);
                return player != nullptr;
            }));
            if(nearPlayer){
                targetId = nearPlayer->getId();
            }
        }
        
        Actor* target = room->getActor(targetId);
        
        if(target && lineOfSight(room, this, target)){
            targetX = target->px;
            targetY = target->py;
        }else{
            targetId = -1;
        }
        
        double dd = (targetX - px)*(targetX - px) + (targetY - py)*(targetY - py);
        if(dd > FONT_W*FONT_W){
            double d = sqrt(dd);
            double dx = (targetX - px)/d;
            double dy = (targetY - py)/d;
            vx = dx * moveSpeed;
            vy = dy * moveSpeed;
        }else{
            vx = 0;
            vy = 0;
        }
    }
    
}
