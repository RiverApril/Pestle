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


bool ClientConnection::tryToWrite(int socket, void* data, size_t size){
    auto writeSize = write(socket, data, size);
    if(writeSize == size){
        return true;
    }else{
        if(writeSize != -1){
            cout << "Only wrote " << writeSize << " bytes out of " << size << endl;
        }
        throw ExceptionServer{"Failed to write to socket"};
    }
}

bool ClientConnection::tryToRead(int socket, void* data, size_t size){
    auto readSize = read(socket, data, size);
    if(readSize == size){
        return true;
    }else{
        if(readSize != -1){
            cout << "Only read " << readSize << " / " << size << " bytes" << endl;
        }else{
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                return false;
            }
        }
    }
    throw ExceptionServer{"Failed to read from socket"};
}


void ClientConnection::sendToClient(Packet* packet, unsigned char size){
    
    //cout << "Sending packet(" << packet->getID() << ") to Client(" << clientId << ")\n";
    
    unique_lock<mutex> lock(writeMutex);
    
    pid packetId = packet->getID();
    
    // send packet id
    if(!tryToWrite(socketId, &packetId, sizeof(packetId))){
        throw ExceptionServer{"Failed to write to socket"};
    }
    
    // send packet size
    if(!tryToWrite(socketId, &size, sizeof(size))){
        throw ExceptionServer{"Failed to write to socket"};
    }
    
    // send packet itself
    if(!tryToWrite(socketId, packet, size)){
        throw ExceptionServer{"Failed to write to socket"};
    }
    
}

void ClientConnection::listenToClient(){
    try{
        while(clientIsConnected){
            pid packetId = PID_ERROR;
            
            if(tryToRead(socketId, &packetId, sizeof(packetId))){
                
                //cout << "(" << clientId << ") Recieved Packet with id: " << packetId << endl;
                
                unsigned char packetSize = 0;
                
                if(tryToRead(socketId, &packetSize, sizeof(packetSize))){
                    
                    //cout << "(" << clientId << ") Packet size is: " << (int)packetSize << endl;
                    
                    void* packetPointer = malloc(packetSize);
                    
                    if(tryToRead(socketId, packetPointer, packetSize)){
                        packetQueue.push(PacketInfo(packetId, packetPointer));
                    }else{
                        free(packetPointer);
                    }
                    
                }
                
            }
        }
    }catch(ExceptionServer e){
        cout << "Client Connection Error: " << e.reason << "\n";
        clientIsConnected = false;
        close(socketId);
    }
}

void ClientConnection::update(){
    while(!packetQueue.empty()){
        PacketInfo packetInfo = packetQueue.front();
        packetQueue.pop();
        
        handlePacket(packetInfo.packetId, packetInfo.packetPointer);
        free(packetInfo.packetPointer);
    }
}



void ClientConnection::handlePacket(pid packetId, void* packetPointer){
    switch (packetId) {
        
        case PID_BI_ActorMove: {
            auto* packet = (Packet_BI_ActorMove*)packetPointer;
            
            Actor* actor = server->room->getActor(packet->actorId);
            if(actor){
                actor->px = packet->px;
                actor->py = packet->py;
                actor->vx = packet->vx;
                actor->vy = packet->vy;
                //cout << "actor(" << actor->getId() << ") moved\n";
            }else{
                cout << "Failed to find actor with id: " << packet->actorId;
            }
            break;
        }
            
        default: {
            throw ExceptionServer{string("Unknown packet with id: ") + to_string(packetId)};
        }
    }
}
