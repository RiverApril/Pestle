//
//  Actor.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Actor.hpp"
#include "Display.hpp"
#include "Room.hpp"
#include "Geometry.hpp"

Actor::Actor(int id, double px, double py, double rx, double ry) : id(id), px(px), py(py), rx(rx), ry(ry){
    vx = 0;
    vy = 0;
}

void Actor::update(Room* room, Display* display, double delta, int xOff, int yOff){
    
    double dx = vx * delta;
    double dy = vy * delta;
    
    // Radius of the max number of tiles actor takes up
    int xtr = ceil(double(rx)/FONT_W);
    int ytr = ceil(double(ry)/FONT_H);
    
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
        display->drawChar('@', px-FONT_W/2-xOff, py-FONT_H/2-yOff);
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
}

