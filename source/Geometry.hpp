//
//  Geometry.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/10/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include "Global.hpp"

class Room;
class Actor;

bool AABBOverlap(double ax, double ay, double arx, double ary, double bx, double by, double brx, double bry);
bool lineSegmentCast(Room* room, double x1, double y1, double x2, double y2);
bool lineOfSight(Room* room, Actor* a, Actor* b);

#endif /* Geometry_hpp */
