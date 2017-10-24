//
//  Packet.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/9/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Packet_hpp
#define Packet_hpp

#include "Actor.hpp"
#include "TileData.hpp"

enum pid{
    PID_ERROR,
    PID_S2C_TellClientsID,
    PID_S2C_NewActor,
    PID_BI_ActorMove,
    PID_S2C_NewRoom,
    PID_S2C_SetTile
};


struct PacketInfo{
    PacketInfo(pid packetId, void* packetPointer) : packetId(packetId), packetPointer(packetPointer){}
    pid packetId;
    void* packetPointer;
};

struct Packet {
    virtual pid getID(){ return PID_ERROR; }
};

struct Packet_S2C_TellClientsID : public Packet {
    pid getID(){ return PID_S2C_TellClientsID; }
    
    int newClientId;
};

struct Packet_S2C_NewActor : public Packet {
    Packet_S2C_NewActor(Actor* actor) : actorCopy(*actor){}
    pid getID(){ return PID_S2C_NewActor; }
    
    Actor actorCopy;
};

struct Packet_BI_ActorMove : public Packet {
    Packet_BI_ActorMove(Actor* actor){
        actorId = actor->getId();
        px = actor->px;
        py = actor->py;
        vx = actor->vx;
        vy = actor->vy;
    }
    pid getID(){ return PID_BI_ActorMove; }
    
    int actorId;
    double px, py, vx, vy;
};

struct Packet_S2C_NewRoom : public Packet {
    pid getID(){ return PID_S2C_NewRoom; }
    
    int width, height;
};

struct Packet_S2C_SetTile : public Packet {
    pid getID(){ return PID_S2C_SetTile; }
    
    int x, y;
    TileData tile;
};


#endif /* Packet_hpp */
