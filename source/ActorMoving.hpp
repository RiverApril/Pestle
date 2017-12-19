//
//  ActorMoving.hpp
//  Pestle
//
//  Created by Braeden Atlee on 12/19/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef ActorMoving_hpp
#define ActorMoving_hpp

#include "Actor.hpp"

class ActorMoving : public Actor {
protected:
    
    virtual void save(unsigned char*& dataPointer){
        Actor::save(dataPointer);
        SAVE(vx);
        SAVE(vy);
        SAVE(rx);
        SAVE(ry);
    };
    
    virtual void load(unsigned char*& dataPointer){
        Actor::load(dataPointer);
        LOAD(vx);
        LOAD(vy);
        LOAD(rx);
        LOAD(ry);
    };
    
public:
    virtual packet_size_t getSize(){
        return Actor::getSize()
        +sizeof(vx)
        +sizeof(vy)
        +sizeof(rx)
        +sizeof(ry);
    }
    
    double vx, vy; // velocity
    double rx, ry; // radius
    
    // Not saved
    double lastvx, lastvy;
    double lastpx, lastpy;
    bool hasMoved = false;
    bool hasChangedVelocity = false;
    bool hitSolid = false;
    
    ActorMoving(ActorType type): Actor(type){}
    ActorMoving(int id, ActorType type, double rx, double ry): Actor(id, type), rx(rx), ry(ry), vx(0), vy(0){}
    virtual ~ActorMoving(){}
    
    virtual void update(Room* room, Display* display, double delta, double xOff, double yOff);
};

#endif /* ActorMoving_hpp */
