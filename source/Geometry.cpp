//
//  Geometry.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/10/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Geometry.hpp"
#include "Room.hpp"
#include "ActorMoving.hpp"


bool AABBOverlap(double ax, double ay, double arx, double ary, double bx, double by, double brx, double bry){
    return fabs(ax-bx) < (arx+brx) && fabs(ay-by) < (ary+bry);
}

bool lineSegmentCast(Room* room, double x1, double y1, double x2, double y2){
    double dx = x2-x1;
    double dy = y2-y1;
    double d = sqrt(dx*dx+dy*dy);
    dx /= d;
    dy /= d;
    
    //cout << "d: " << d << "\n";
    
    for(double t = 0; t <= d; t+=0.1){ // this is a terrible and lazy way to do this
        int xx = (int)round((x1+dx*t)/FONT_W);
        int yy = (int)round((y1+dy*t)/FONT_H);
        if(isTileSolid(room->tileAt(xx, yy))){
            return true;
        }
    }
    return false;
    
}

bool lineOfSight(Room* room, Actor* a, Actor* b){
    
    if(!lineSegmentCast(room, a->px, a->py, b->px, b->py)){
        return true;
    }else{
        auto bb = dynamic_cast<ActorMoving*>(b);
        if(bb){
            return !(lineSegmentCast(room, a->px, a->py, bb->px - bb->rx, bb->py - bb->ry)
            && lineSegmentCast(room, a->px, a->py, bb->px - bb->rx, bb->py + bb->ry)
            && lineSegmentCast(room, a->px, a->py, bb->px + bb->rx, bb->py - bb->ry)
            && lineSegmentCast(room, a->px, a->py, bb->px + bb->rx, bb->py + bb->ry));
        }
        return true;
    }
}
