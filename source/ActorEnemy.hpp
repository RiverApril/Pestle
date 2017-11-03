//
//  ActorEnemy.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/27/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef ActorEnemy_hpp
#define ActorEnemy_hpp

#include "Actor.hpp"

class ActorEnemy : public ActorLiving {
protected:
    virtual void save(unsigned char*& dataPointer){
        ActorLiving::save(dataPointer);
        SAVE(targetId);
        SAVE(moveSpeed);
    };
    virtual void load(unsigned char*& dataPointer){
        ActorLiving::load(dataPointer);
        LOAD(targetId);
        LOAD(moveSpeed);
    };

public:
    
    int targetId = -1;
    double moveSpeed = 0.04;

    // Not saved
    Actor* target;

    virtual size_t getSize(){
        return ActorLiving::getSize() + sizeof(targetId) + sizeof(moveSpeed);
    }
    
    ActorEnemy(): ActorLiving(ACTOR_TYPE_Enemy){}
    ActorEnemy(int id): ActorLiving(id, ACTOR_TYPE_Enemy, (FONT_W/2.0)-1, (FONT_H/2.0)-2, 10){}
    virtual ~ActorEnemy(){}

    virtual void update(Room* room, Display* display, double delta, int xOff, int yOff);

    virtual void die(){}
    virtual char getSymbol(){ return 'E'; }
};


#endif /* ActorEnemy_hpp */
