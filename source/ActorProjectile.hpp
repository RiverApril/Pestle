//
//  ActorProjectile.hpp
//  Pestle
//
//  Created by Braeden Atlee on 12/19/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef ActorProjectile_hpp
#define ActorProjectile_hpp


#include "ActorMoving.hpp"

class ActorProjectile : public ActorMoving {
protected:
    virtual void save(unsigned char*& dataPointer){
        ActorMoving::save(dataPointer);
        SAVE(ownerId);
    };
    virtual void load(unsigned char*& dataPointer){
        ActorMoving::load(dataPointer);
        LOAD(ownerId);
    };
    
public:
    
    int ownerId = -1;
    
    virtual packet_size_t getSize(){
        return ActorMoving::getSize() + sizeof(ownerId);
    }
    
    ActorProjectile(): ActorMoving(ACTOR_TYPE_Projectile){}
    ActorProjectile(int id, int ownerId, double px, double py, double vx, double vy): ActorMoving(id, ACTOR_TYPE_Projectile, 2, 2){
        this->ownerId = ownerId;
        this->px = px;
        this->py = py;
        this->vx = vx;
        this->vy = vy;
    }
    virtual ~ActorProjectile(){}
    
    virtual void update(Room* room, Display* display, double delta, double xOff, double yOff);
    
    virtual char getSymbol(){ return '.'; }
};


#endif /* ActorProjectile_hpp */
