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
#include "Network.hpp"

#define OVERLOAD_PACKET_SEND_DATA virtual void sendData(NetworkParticipant* n, socket_t socket){n->tryToWrite(socket, this, sizeof(*this));}
#define OVERLOAD_PACKET_GET_SIZE virtual packet_size_t getSize(){ return sizeof(*this); }

enum pid{
    PID_ERROR = 0,
    PID_S2C_TellClientsID,
    PID_S2C_NewActor,
    PID_BI_ActorMove,
    PID_S2C_NewRoom,
    PID_S2C_SetTile
};

struct PacketInfo{
    PacketInfo(pid packetId, void* packetPointer, size_t packetSize) : packetId(packetId), packetPointer(packetPointer), packetSize(packetSize){}
    pid packetId;
    void* packetPointer;
    size_t packetSize;
};

struct Packet {
    virtual ~Packet(){}
    virtual pid getID(){
        return PID_ERROR;
    }
    virtual void sendData(NetworkParticipant* n, socket_t socket){
        n->tryToWrite(socket, this, sizeof(*this));
    }
    virtual packet_size_t getSize(){ return 0; }
};

struct Packet_S2C_TellClientsID : Packet {
    pid getID(){ return PID_S2C_TellClientsID; }
    OVERLOAD_PACKET_SEND_DATA
    OVERLOAD_PACKET_GET_SIZE
    
    int newClientId;
};

struct Packet_S2C_NewActor : Packet {
    pid getID(){ return PID_S2C_NewActor; }
    
    Packet_S2C_NewActor(Actor* actor) {
        dataSize = actor->getSize();
        data = (unsigned char*)malloc(dataSize);
        unsigned char* place = data;
        Actor::saveActor(actor, place);
    }

    Packet_S2C_NewActor(unsigned char* packetPointer){
        memcpy(&dataSize, packetPointer, sizeof(dataSize));
        packetPointer += sizeof(dataSize);
        data = packetPointer;
    }

    virtual packet_size_t getSize(){
        return sizeof(dataSize) + dataSize;
    }

    virtual void sendData(NetworkParticipant* n, socket_t socket){
        unsigned char* allData = (unsigned char*)malloc(sizeof(dataSize)+dataSize);
        memcpy(allData, &dataSize, sizeof(dataSize));
        memcpy(allData+sizeof(dataSize), data, dataSize);
        
        n->tryToWrite(socket, allData, dataSize+sizeof(dataSize));
    }

    Actor* getActor(){
        unsigned char* dataLoc = data;
        Actor* actor = Actor::loadActor(dataLoc);
        return actor;
    }

    size_t dataSize;
    unsigned char* data;
};

struct Packet_BI_ActorMove : Packet {
    Packet_BI_ActorMove(ActorMoving* actor){
        actorId = actor->getId();
        px = actor->px;
        py = actor->py;
        vx = actor->vx;
        vy = actor->vy;
    }
    pid getID(){ return PID_BI_ActorMove; }
    OVERLOAD_PACKET_SEND_DATA
    OVERLOAD_PACKET_GET_SIZE
    
    int actorId;
    double px, py, vx, vy;
};

struct Packet_S2C_NewRoom : Packet {
    pid getID(){ return PID_S2C_NewRoom; }
    OVERLOAD_PACKET_SEND_DATA
    OVERLOAD_PACKET_GET_SIZE
    
    int width, height;
};

struct Packet_S2C_SetTile : Packet {
    pid getID(){ return PID_S2C_SetTile; }
    OVERLOAD_PACKET_SEND_DATA
    OVERLOAD_PACKET_GET_SIZE
    
    int x, y;
    TileData tile;
};


#endif /* Packet_hpp */
