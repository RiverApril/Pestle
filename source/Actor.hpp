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
    ACTOR_TYPE_Enemy
};

class Actor {
    int id;
    ActorType type;

protected:
    virtual void save(unsigned char*& dataPointer){SAVE(type); SAVE(id); SAVE(px); SAVE(py); }
    virtual void load(unsigned char*& dataPointer){LOAD(id); LOAD(px); LOAD(py); } // intentionally not loading type

public:
    virtual packet_size_t getSize(){ return sizeof(id) + sizeof(type) + sizeof(px) + sizeof(py); }
    
    double px, py; // position

    int getId(){ return id; }
    ActorType getType(){ return type; }
    
    Actor(ActorType type): type(type){}
    Actor(int id, ActorType type) : id(id), type(type), px(0), py(0){}
    virtual ~Actor(){}
    
    virtual void update(Room* room, Display* display, double delta, int xOff, int yOff);
    virtual char getSymbol(){ return '?'; }


    static Actor* loadActor(unsigned char*& dataPointer);
    static void saveActor(Actor* actor, unsigned char*& dataPointer);
    
};

class ActorMoving : public Actor {
protected:
    virtual void save(unsigned char*& dataPointer){ Actor::save(dataPointer); SAVE(vx); SAVE(vy); SAVE(rx); SAVE(ry); };
    virtual void load(unsigned char*& dataPointer){ Actor::load(dataPointer); LOAD(vx); LOAD(vy); LOAD(rx); LOAD(ry); };

public:
    virtual packet_size_t getSize(){ return Actor::getSize()+sizeof(vx)+sizeof(vy)+sizeof(rx)+sizeof(ry); }
    
    double vx, vy; // velocity
    double rx, ry; // radius

    // Not saved
    double lastvx, lastvy;
    double lastpx, lastpy;
    bool hasMoved = false;

    ActorMoving(ActorType type): Actor(type){}
    ActorMoving(int id, ActorType type, double rx, double ry): Actor(id, type), rx(rx), ry(ry), vx(0), vy(0){}
    virtual ~ActorMoving(){}

    virtual void update(Room* room, Display* display, double delta, int xOff, int yOff);
};

class ActorLiving : public ActorMoving {
protected:
    virtual void save(unsigned char*& dataPointer){ ActorMoving::save(dataPointer); SAVE(hp); SAVE(maxHp); SAVE(alive); };
    virtual void load(unsigned char*& dataPointer){ ActorMoving::load(dataPointer); LOAD(hp); LOAD(maxHp); LOAD(alive); };

public:
    virtual packet_size_t getSize(){ return ActorMoving::getSize()+sizeof(hp)+sizeof(maxHp)+sizeof(alive); }
    
    double hp, maxHp;
    bool alive;

    ActorLiving(ActorType type): ActorMoving(type){}
    ActorLiving(int id, ActorType type, double rx, double ry, double maxHp): ActorMoving(id, type, rx, ry), maxHp(maxHp), hp(maxHp), alive(true){}
    virtual ~ActorLiving(){}

    virtual void update(Room* room, Display* display, double delta, int xOff, int yOff){
        ActorMoving::update(room, display, delta, xOff, yOff);
        if(hp <= 0 && alive){
            alive = false;
            die();
        }
    }

    virtual void die(){}
};


#endif /* Actor_hpp */
