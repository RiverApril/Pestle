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
    
    Packet_S2C_TellClientsID(int newClientId) : newClientId(newClientId){}
    
    Packet_S2C_TellClientsID(unsigned char* dataPointer){
        LOAD(newClientId);
    }
    
    virtual packet_size_t getSize(){
        return sizeof(newClientId);
    }
    
    virtual void sendData(NetworkParticipant* n, int socket){
        packet_size_t size = getSize();
        unsigned char* dataPointer = (unsigned char*)malloc(size);
        SAVE(newClientId);
        n->tryToWrite(socket, dataPointer-size, size);
    }
    
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
    pid getID(){ return PID_BI_ActorMove; }
    
    Packet_BI_ActorMove(ActorMoving* actor){
        actorId = actor->getId();
        px = actor->px;
        py = actor->py;
        vx = actor->vx;
        vy = actor->vy;
    }
    
    Packet_BI_ActorMove(unsigned char* dataPointer){
        LOAD(actorId);
        LOAD(px);
        LOAD(py);
        LOAD(vx);
        LOAD(vy);
    }
    
    virtual packet_size_t getSize(){
        return sizeof(actorId) + sizeof(px) + sizeof(py) + sizeof(vx) + sizeof(vy);
    }
    
    virtual void sendData(NetworkParticipant* n, int socket){
        packet_size_t size = getSize();
        unsigned char* dataPointer = (unsigned char*)malloc(size);
        SAVE(actorId);
        SAVE(px);
        SAVE(py);
        SAVE(vx);
        SAVE(vy);
        n->tryToWrite(socket, dataPointer-size, size);
    }
    
    int actorId;
    double px, py, vx, vy;
};

struct Packet_S2C_NewRoom : Packet {
    pid getID(){ return PID_S2C_NewRoom; }
    
    Packet_S2C_NewRoom(int width, int height) : width(width), height(height){}
    
    Packet_S2C_NewRoom(unsigned char* dataPointer){
        LOAD(width);
        LOAD(height);
    }
    
    virtual packet_size_t getSize(){
        return sizeof(width) + sizeof(height);
    }
    
    virtual void sendData(NetworkParticipant* n, int socket){
        packet_size_t size = getSize();
        unsigned char* dataPointer = (unsigned char*)malloc(size);
        SAVE(width);
        SAVE(height);
        n->tryToWrite(socket, dataPointer-size, size);
    }
    
    int width, height;
};

struct Packet_S2C_SetTile : Packet {
    pid getID(){ return PID_S2C_SetTile; }
    
    Packet_S2C_SetTile(int x, int y, TileData tile) : x(x), y(y), tile(tile){}
    
    Packet_S2C_SetTile(unsigned char* dataPointer){
        LOAD(x);
        LOAD(y);
        LOAD(tile.tile);
        LOAD(tile.meta);
    }
    
    virtual packet_size_t getSize(){
        return sizeof(x) + sizeof(y) + sizeof(tile.tile) + sizeof(tile.meta);
    }
    
    virtual void sendData(NetworkParticipant* n, int socket){
        packet_size_t size = getSize();
        unsigned char* dataPointer = (unsigned char*)malloc(size);
        SAVE(x);
        SAVE(y);
        SAVE(tile.tile);
        SAVE(tile.meta);
        n->tryToWrite(socket, dataPointer-size, size);
    }
    
    int x, y;
    TileData tile;
};


#endif /* Packet_hpp */
