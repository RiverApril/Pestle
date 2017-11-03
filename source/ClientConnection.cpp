//
//  ClientConnection.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "ClientConnection.hpp"
#include "Server.hpp"
#include "Room.hpp"


ClientConnection::ClientConnection(Server* server, sockaddr_in address, int socketId, int clientId) : server(server), address(address), socketId(socketId), clientId(clientId) {
    
    clientIsConnected = true;
    
    listenToClientThread = thread([](ClientConnection* c){
        c->listenToClient();
    }, this);
    
}

ClientConnection::~ClientConnection(){
    close(socketId);
}


void ClientConnection::sendToClient(Packet* packet){
    unique_lock<mutex> lock(writeMutex);
    sendToSocket(socketId, packet);
}

void ClientConnection::listenToClient(){
    try{
        while(clientIsConnected){
            pid packetId = PID_ERROR;
            
            if(tryToRead(socketId, &packetId, sizeof(packetId))){
                
                //cout << "(" << clientId << ") Recieved Packet with id: " << packetId << endl;
                
                packet_size_t packetSize = 0;
                
                if(tryToRead(socketId, &packetSize, sizeof(packetSize))){
                    
                    //cout << "(" << clientId << ") Packet size is: " << (int)packetSize << endl;
                    
                    void* packetPointer = malloc(packetSize);
                    
                    if(tryToRead(socketId, packetPointer, packetSize)){
                        packetQueue.push(PacketInfo(packetId, packetPointer, packetSize));
                    }else{
                        free(packetPointer);
                    }
                    
                }
                
            }
        }
    }catch(NetworkException e){
        cout << "Client Connection Error: " << e.reason << "\n";
        clientIsConnected = false;
        close(socketId);
    }
}

void ClientConnection::update(){
    while(!packetQueue.empty()){
        PacketInfo packetInfo = packetQueue.front();
        packetQueue.pop();
        
        handlePacket(packetInfo.packetId, packetInfo.packetPointer, packetInfo.packetSize);
        free(packetInfo.packetPointer);
    }
}



void ClientConnection::handlePacket(pid packetId, void* packetPointer, packet_size_t packetSize){
    switch (packetId) {
        
        case PID_BI_ActorMove: {
            
            auto* packet = (Packet_BI_ActorMove*)(new Packet_BI_ActorMove((unsigned char*)packetPointer));
            
            ActorMoving* actorMoving = dynamic_cast<ActorMoving*>(server->room->getActor(packet->actorId));
            if(actorMoving){
                actorMoving->px = packet->px;
                actorMoving->py = packet->py;
                actorMoving->vx = packet->vx;
                actorMoving->vy = packet->vy;
                //cout << "actor(" << actor->getId() << ") moved\n";
            }else{
                cout << "Failed to find actor with id: " << packet->actorId << "\n";
            }
            
            delete packet;
            break;
        }
            
        default: {
            throw NetworkException{string("Unknown packet with id: ") + to_string(packetId)};
        }
    }
}
