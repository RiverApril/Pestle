//
//  Client.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Client.hpp"
#include "MenuGame.hpp"
#include "Packet.hpp"
#include "Room.hpp"


Client::Client(MenuGame* game, string ip, int port) : game(game){
    
    socketId = socket(AF_INET, SOCK_STREAM, 0);
    if(socketId < 0){
        throw ExceptionClient{"Failed to open client socket"};
    }
    
    hostent* server = gethostbyname(ip.c_str());
    
    if(!server){
        throw ExceptionClient{"Failed to resolve server"};
    }
    
    serverAddress = sockaddr_in{0};
    
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    
    if(connect(socketId, (sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
        throw ExceptionClient{"Failed to connect to server: " + to_string(errno)};
    }
    
    //fcntl(socketId, F_SETFL, O_NONBLOCK); want blocking
    
    connectedToServer = true;
    
    listenToServerThread = thread([](Client* c){
        c->listenToServer();
    }, this);
}

Client::~Client(){
    connectedToServer = false;
    close(socketId);
}

bool Client::tryToWrite(int socket, void* data, size_t size){
    auto writeSize = write(socket, data, size);
    if(writeSize == size){
        return true;
    }else{
        if(writeSize != -1){
            cout << "Only wrote " << writeSize << " bytes out of " << size << endl;
        }
        throw ExceptionClient{"Failed to write to socket"};
    }
}

bool Client::tryToRead(int socket, void* data, size_t size){
    auto readSize = read(socket, data, size);
    if(readSize == size){
        return true;
    }else{
        if(readSize != -1){
            cout << "Only read " << readSize << " bytes out of " << size << endl;
        }else{
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                return false;
            }
        }
    }
    throw ExceptionClient{"Failed to read from socket"};
}

void Client::sendToServer(Packet* packet, unsigned char size){
    unique_lock<mutex> lock(writeMutex);
    
    pid packetId = packet->getID();
    
    // send packet id
    tryToWrite(socketId, &packetId, sizeof(packetId));
    
    // send packet size
    tryToWrite(socketId, &size, sizeof(size));
    
    // send packet itself
    tryToWrite(socketId, packet, size);
    
}

void Client::listenToServer(){
    try{
        while(connectedToServer){
            pid packetId = PID_ERROR;
            
            if(tryToRead(socketId, &packetId, sizeof(packetId))){
                
                //cout << "Recieved Packet with id: " << packetId << endl;
                
                unsigned char packetSize = 0;
                
                if(tryToRead(socketId, &packetSize, sizeof(packetSize))){
                    
                    //cout << "Packet size is: " << (int)packetSize << endl;
                    
                    void* packetPointer = malloc(packetSize);
                    
                    if(tryToRead(socketId, packetPointer, packetSize)){
                        
                        packetQueue.push(PacketInfo(packetId, packetPointer));
                        
                    }else{
                        free(packetPointer);
                    }
                }
                
            }
        }
    }catch(ExceptionClient e){
        cout << "Client Error: " << e.reason << "\n";
        connectedToServer = false;
        close(socketId);
    }
}

void Client::update(){
    while(!packetQueue.empty()){
        PacketInfo packetInfo = packetQueue.front();
        packetQueue.pop();
        try{
            handlePacket(packetInfo.packetId, packetInfo.packetPointer);
        }catch(ExceptionClient e){
            cout << "Client Error: " << e.reason << "\n";
        }
        free(packetInfo.packetPointer);
    }
}

void Client::handlePacket(pid packetId, void* packetPointer){
    
    switch (packetId) {
        case PID_S2C_TellClientsID: {
            
            auto* packet = (Packet_S2C_TellClientsID*)packetPointer;
            
            if(clientId == -1){
                clientId = packet->newClientId;
                cout << "Client got an id: " << clientId << endl;
            }else{
                throw ExceptionClient{"Recieved new ID, but already has an ID."};
            }
            
            break;
        }
        case PID_S2C_NewActor: {
            
            auto* packet = (Packet_S2C_NewActor*)packetPointer;
            
            Actor* a = new Actor(packet->actorCopy);
            game->room->replaceActor(a);
            cout << "New Actor(" << a->getId() << ")\n";
            break;
        }
        case PID_BI_ActorMove: {
            auto* packet = (Packet_BI_ActorMove*)packetPointer;
            
            Actor* actor = game->room->getActor(packet->actorId);
            if(actor){
                actor->px = packet->px;
                actor->py = packet->py;
                actor->vx = packet->vx;
                actor->vy = packet->vy;
            }else{
                cout << "Failed to find actor with id: " << packet->actorId;
            }
            break;
        }
        case PID_S2C_NewRoom: {
            auto* packet = (Packet_S2C_NewRoom*)packetPointer;
            
            game->room = new Room(packet->width, packet->height);
            cout << "New Room with size " << packet->width << ", " << packet->height << "\n";
            break;
        }
        case PID_S2C_SetTile: {
            auto* packet = (Packet_S2C_SetTile*)packetPointer;
            
            game->room->setTile(packet->x, packet->y, packet->tile);
            break;
        }
        default: {
            throw ExceptionClient{string("Unknown packet with id: ") + to_string(packetId)};
        }
    }
}


