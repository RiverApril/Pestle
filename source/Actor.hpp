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

enum ActorType{
    ACTOR_TYPE_None,
    ACTOR_TYPE_Player,
    ACTOR_TYPE_Enemy,
    ACTOR_TYPE_Projectile
};

class Actor {
    int id;
    ActorType type;

protected:
    virtual void save(unsigned char*& dataPointer){
        SAVE(type);
        SAVE(id);
        SAVE(px);
        SAVE(py);
    }
    virtual void load(unsigned char*& dataPointer){
        // intentionally not loading type
        LOAD(id);
        LOAD(px);
        LOAD(py);
    }

public:
    virtual packet_size_t getSize(){
        return sizeof(id)
        +sizeof(type)
        +sizeof(px)
        +sizeof(py);
    }
    
    double px, py; // position
    
    // Not saved:
    bool markedForRemoval = false;

    int getId(){ return id; }
    ActorType getType(){ return type; }
    
    Actor(ActorType type): type(type){}
    Actor(int id, ActorType type) : id(id), type(type), px(0), py(0){}
    virtual ~Actor(){}
    
    virtual void update(Room* room, Display* display, double delta, double xOff, double yOff);
    
    virtual char getSymbol(){
        return '?';
    }

    static Actor* loadActor(unsigned char*& dataPointer);
    static void saveActor(Actor* actor, unsigned char*& dataPointer);
    
};


#endif /* Actor_hpp */
