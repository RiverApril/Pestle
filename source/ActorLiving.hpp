//
//  ActorLiving.hpp
//  Pestle
//
//  Created by Braeden Atlee on 12/19/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef ActorLiving_hpp
#define ActorLiving_hpp

#include "ActorMoving.hpp"

class ActorLiving : public ActorMoving {
protected:
    
    virtual void save(unsigned char*& dataPointer){
        ActorMoving::save(dataPointer);
        SAVE(hp);
        SAVE(maxHp);
        SAVE(alive);
    };
    
    virtual void load(unsigned char*& dataPointer){
        ActorMoving::load(dataPointer);
        LOAD(hp);
        LOAD(maxHp);
        LOAD(alive);
    };
    
public:
    virtual packet_size_t getSize(){
        return ActorMoving::getSize()
        +sizeof(hp)
        +sizeof(maxHp)
        +sizeof(alive);
    }
    
    double hp, maxHp;
    bool alive;
    
    ActorLiving(ActorType type): ActorMoving(type){}
    ActorLiving(int id, ActorType type, double rx, double ry, double maxHp): ActorMoving(id, type, rx, ry), maxHp(maxHp), hp(maxHp), alive(true){}
    virtual ~ActorLiving(){}
    
    virtual void update(Room* room, Display* display, double delta, double xOff, double yOff);
    
    virtual void die(){}
};

#endif /* ActorLiving_hpp */
