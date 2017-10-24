//
//  Actor.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Actor_hpp
#define Actor_hpp

#include "Global.hpp"

class Display;
class Room;

class Actor{
    int id;
    
protected:
    double rx, ry; // radius
    
public:
    
    double px, py; // position
    double vx, vy; // velocity
    
    Actor(int id, double px, double py, double rx, double ry);
    virtual ~Actor(){}
    
    void update(Room* room, Display* display, double delta, int xOff, int yOff);
    
    int getId(){ return id; }
    
};

#endif /* Actor_hpp */
