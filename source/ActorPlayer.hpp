//
//  ActorPlayer.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/25/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef ActorPlayer_hpp
#define ActorPlayer_hpp

#include "ActorLiving.hpp"

class ActorPlayer : public ActorLiving {
protected:
    virtual void save(unsigned char*& dataPointer){
        ActorLiving::save(dataPointer);
    };
    virtual void load(unsigned char*& dataPointer){
        ActorLiving::load(dataPointer);
    };

public:
    
    virtual packet_size_t getSize(){
        return ActorLiving::getSize();
    }
    
    ActorPlayer(): ActorLiving(ACTOR_TYPE_Player){}
    ActorPlayer(int id): ActorLiving(id, ACTOR_TYPE_Player, (FONT_W/2.0)-1, (FONT_H/2.0)-2, 10){}
    virtual ~ActorPlayer(){}

    virtual void update(Room* room, Display* display, double delta, double xOff, double yOff){
        ActorLiving::update(room, display, delta, xOff, yOff);
    }

    virtual void die(){}
    virtual char getSymbol(){ return '@'; }
};


#endif /* ActorPlayer_hpp */
