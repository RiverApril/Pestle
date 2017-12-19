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
#include "Display.hpp"
#include "Room.hpp"
#include "Geometry.hpp"

void Actor::update(Room* room, Display* display, double delta, double xOff, double yOff){
    if(display){
        display->drawChar(getSymbol(), (int)(px-FONT_W/2-xOff), (int)(py-FONT_H/2-yOff));
    }
}

void ActorMoving::update(Room* room, Display* display, double delta, double xOff, double yOff){

    double dx = vx * delta;
    double dy = vy * delta;
    
    // Radius of the max number of tiles actor takes up
    int xtr = (int)ceil(double(rx)/FONT_W);
    int ytr = (int)ceil(double(ry)/FONT_H);
    
    int pcx = int(px/FONT_W);
    int pcy = int(py/FONT_H);
    
    for(int x = pcx-xtr; x <= pcx+xtr; x++){
        for(int y = pcy-ytr; y <= pcy+ytr; y++){
            if(isTileSolid(room->tileAt(x, y))){
                if(AABBOverlap(px+dx, py, rx, ry, x*FONT_W, y*FONT_H, FONT_W/2.0, FONT_H/2.0)){
                    dx = 0;
                    vx = 0;
                    if(px < x*FONT_W){
                        px = x*FONT_W-FONT_W/2.0-rx;
                    }else{
                        px = x*FONT_W+FONT_W/2.0+rx;
                    }
                }
                if(AABBOverlap(px, py+dy, rx, ry, x*FONT_W, y*FONT_H, FONT_W/2.0, FONT_H/2.0)){
                    dy = 0;
                    vy = 0;
                    if(py < y*FONT_H){
                        py = y*FONT_H-FONT_H/2.0-ry;
                    }else{
                        py = y*FONT_H+FONT_H/2.0+ry;
                    }
                }
            }
        }
    }
    
    px += dx;
    py += dy;

    if(display){
        if(DRAW_DEBUG){
            SDL_Rect rect;
            rect.x = px-rx-xOff;
            rect.y = py-ry-yOff;
            rect.w = rx*2+1;
            rect.h = ry*2+1;
            SDL_SetRenderDrawColor(display->getRenderer(), 255, 0, 0, 64);
            SDL_RenderDrawRect(display->getRenderer(), &rect);
        }
    }
    
    hasMoved = px != lastpx || py != lastpy || vx != lastvx || vy != lastvy;
    
    lastpx = px;
    lastpy = py;
    lastvx = vx;
    lastvy = vy;

    Actor::update(room, display, delta, xOff, yOff);
}


Actor* Actor::loadActor(unsigned char*& dataPointer){
    ActorType type;
    LOAD(type);

    Actor* actor;

    switch(type){
        case ACTOR_TYPE_Player:{
            actor = new ActorPlayer(type);
            break;
        }
        case ACTOR_TYPE_Enemy:{
            actor = new ActorEnemy(type);
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


