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


    socketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socketId == INVALID_SOCKET){
        throw NetworkException{"Failed to open client socket"};
    }
    
    hostent* server = gethostbyname(ip.c_str());
    
    if(!server){
        throw NetworkException{"Failed to resolve server"};
    }
    
    //serverAddress = sockaddr_in{0};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddress.sin_port = htons(port);
    
    if(connect(socketId, (sockaddr *) &serverAddress, sizeof(serverAddress)) == SOCKET_ERROR){
        throw NetworkException{"Failed to connect to server: " + GET_SOCKET_ERROR};
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

void Client::sendToServer(Packet* packet){
    #if STATS_DEBUG
    packetsSentCount++;
    #endif
    unique_lock<mutex> lock(writeMutex);
    sendToSocket(socketId, packet);
}

void Client::listenToServer(){
    try{
        while(connectedToServer){
            pid packetId = PID_ERROR;
            
            if(tryToRead(socketId, &packetId, sizeof(packetId))){
                
                //cout << "Recieved Packet with id: " << packetId << endl;
                
                packet_size_t packetSize = 0;
                
                if(tryToRead(socketId, &packetSize, sizeof(packetSize))){
                    
                    //cout << "Packet size is: " << (int)packetSize << endl;
                    
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
            handlePacket(packetInfo.packetId, packetInfo.packetPointer, packetInfo.packetSize);
        }catch(NetworkException e){
            cout << "Client Error: " << e.reason << "\n";
        }
        free(packetInfo.packetPointer);
    }
}

void Client::handlePacket(pid packetId, void* packetPointer, packet_size_t packetSize){
    
    #if STATS_DEBUG
    packetsReceivedCount++;
    #endif
    
    switch (packetId) {
        case PID_S2C_TellClientsID: {
            
            auto* packet = (Packet_S2C_TellClientsID*)(new Packet_S2C_TellClientsID((unsigned char*)packetPointer));
            
            if(clientId == -1){
                clientId = packet->newClientId;
                cout << "Client got an id: " << clientId << endl;
            }else{
                throw NetworkException{"Recieved new ID, but already has an ID."};
            }
            
            delete packet;
            break;
        }
        case PID_S2C_NewActor: {

            auto* packet = (Packet_S2C_NewActor*)(new Packet_S2C_NewActor((unsigned char*)packetPointer));
            
            Actor* a = packet->getActor();
            game->room->replaceActor(a);
            cout << "New Actor(" << a->getId() << ")\n";

            delete packet;
            break;
        }
        case PID_BI_ActorMove: {
            
            auto* packet = (Packet_BI_ActorMove*)(new Packet_BI_ActorMove((unsigned char*)packetPointer));
            
            ActorMoving* actorMoving = dynamic_cast<ActorMoving*>(game->room->getActor(packet->actorId));
            if(actorMoving){
                actorMoving->px = packet->px;
                actorMoving->py = packet->py;
                actorMoving->vx = packet->vx;
                actorMoving->vy = packet->vy;
            }else{
                cout << "Failed to find actor with id: " << packet->actorId << "\n";
            }
            
            delete packet;
            break;
        }
        case PID_S2C_NewRoom: {
            
            auto* packet = (Packet_S2C_NewRoom*)(new Packet_S2C_NewRoom((unsigned char*)packetPointer));
            
            game->room = new Room(packet->width, packet->height);
            cout << "New Room with size " << packet->width << ", " << packet->height << "\n";
            
            delete packet;
            break;
        }
        case PID_S2C_SetTile: {
            
            auto* packet = (Packet_S2C_SetTile*)(new Packet_S2C_SetTile((unsigned char*)packetPointer));
            
            game->room->setTile(packet->x, packet->y, packet->tile);
            
            delete packet;
            break;
        }
        default: {
            throw NetworkException{string("Unknown packet with id: ") + to_string(packetId)};
        }
    }
}


