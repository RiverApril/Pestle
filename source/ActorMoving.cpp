//
//  ActorMoving.cpp
//  Pestle
//
//  Created by Braeden Atlee on 12/19/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "ActorMoving.hpp"
#include "Display.hpp"
#include "Room.hpp"
#include "Geometry.hpp"


void ActorMoving::update(Room* room, Display* display, double delta, double xOff, double yOff){
    
    double dx = vx * delta;
    double dy = vy * delta;
    
    // Radius of the max number of tiles actor takes up
    int xtr = (int)ceil(double(rx)/FONT_W);
    int ytr = (int)ceil(double(ry)/FONT_H);
    
    int pcx = int(px/FONT_W);
    int pcy = int(py/FONT_H);
    
    hitSolid = false;
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
                    hitSolid = true;
                }
                if(AABBOverlap(px, py+dy, rx, ry, x*FONT_W, y*FONT_H, FONT_W/2.0, FONT_H/2.0)){
                    dy = 0;
                    vy = 0;
                    if(py < y*FONT_H){
                        py = y*FONT_H-FONT_H/2.0-ry;
                    }else{
                        py = y*FONT_H+FONT_H/2.0+ry;
                    }
                    hitSolid = true;
                }
            }
        }
    }
    
    px += dx;
    py += dy;
    
    if(display){
        if(DRAW_DEBUG){
            SDL_Rect rect;
            rect.x = (int)(px-rx-xOff);
            rect.y = (int)(py-ry-yOff);
            rect.w = (int)(rx*2+1);
            rect.h = (int)(ry*2+1);
            SDL_SetRenderDrawColor(display->getRenderer(), 255, 0, 0, 64);
            SDL_RenderDrawRect(display->getRenderer(), &rect);
        }
    }
    
    hasChangedVelocity = vx != lastvx || vy != lastvy;
    hasMoved = px != lastpx || py != lastpy;
    
    lastpx = px;
    lastpy = py;
    lastvx = vx;
    lastvy = vy;
    
    Actor::update(room, display, delta, xOff, yOff);
}
