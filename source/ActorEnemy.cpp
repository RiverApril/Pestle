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

void ActorEnemy::update(Room* room, Display* display, double delta, double xOff, double yOff){
    ActorLiving::update(room, display, delta, xOff, yOff);

    if(!display){
        if(targetId == -1){
            ActorPlayer* nearPlayer = dynamic_cast<ActorPlayer*>(room->findNearestActor(px, py, [](Actor* actor, double distSq){
                Actor* player = dynamic_cast<ActorPlayer*>(actor);
                return player;
            }));
            if(nearPlayer){
                targetId = nearPlayer->getId();
                target = nearPlayer;
            }
        }
        if(!target || target->getId() != targetId){
            target = room->getActor(targetId);
        }
        
        if(target){
            double d = sqrt((target->px - px)*(target->px - px) + (target->py - py)*(target->py - py));
            if(d > FONT_W){
                double dx = (target->px - px)/d;
                double dy = (target->py - py)/d;
                vx = dx * moveSpeed;
                vy = dy * moveSpeed;
            }else{
                vx = 0;
                vy = 0;
            }
        }
    }
    
}
