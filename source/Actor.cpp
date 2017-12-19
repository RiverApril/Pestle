//
//  Actor.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Actor.hpp"
#include "ActorPlayer.hpp"
#include "ActorEnemy.hpp"
#include "ActorProjectile.hpp"
#include "Display.hpp"

void Actor::update(Room* room, Display* display, double delta, double xOff, double yOff){
    if(display){
        display->drawChar(getSymbol(), (int)(px-FONT_W/2-xOff), (int)(py-FONT_H/2-yOff));
    }
}

Actor* Actor::loadActor(unsigned char*& dataPointer){
    ActorType type;
    LOAD(type);

    Actor* actor;

    switch(type){
        case ACTOR_TYPE_Player:{
            actor = new ActorPlayer();
            break;
        }
        case ACTOR_TYPE_Enemy:{
            actor = new ActorEnemy();
            break;
        }
        case ACTOR_TYPE_Projectile:{
            actor = new ActorProjectile();
            break;
        }
        case ACTOR_TYPE_None:{
            cout << "Error trying to make an actor, the type is none.\n";
            break;
        }
    }

    actor->load(dataPointer);
    return actor;
}

void Actor::saveActor(Actor* actor, unsigned char*& dataPointer){
    actor->save(dataPointer);
}


