//
//  Server.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Server.hpp"
#include "Packet.hpp"
#include "ClientConnection.hpp"
#include "Room.hpp"

Server::Server(int port){
    
    if(SDL_Init(0) < 0){
        throw ExceptionServer{string("SDL_INIT Failed: ") + SDL_GetError()};
    }
    
    room = new Room(50, 30);
    room->generate();
    
    // AF_INET means internet domain instead of file domain
    // SOCK_STREAM means to use a stream instead of a block, so it chooses TCP
    socketId = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socketId < 0){
        throw ExceptionServer{"Failed to open server socket"};
    }
    
    serverAddress = sockaddr_in{0};
    
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port); // corrects byte order
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Sets the address to be the server's address
    
    if(bind(socketId, (const struct sockaddr*) &serverAddress, (socklen_t)sizeof(serverAddress)) < 0){
        throw ExceptionServer{"Faild to bind socket to port"};
    }
    
    serverRunning = true;
    
    cout << "Server Started\n";
    
    //listenThread = thread(listenFunc, this);
    
    listenThread = thread([](Server* s){
        s->listenForClients();
    }, this);
    
}

Server::~Server(){
    serverRunning = false;
    close(socketId);
    listenThread.join();
    delete room;
}

void Server::listenForClients() {
    
    listen(socketId, 5); // 5 is the size of the backlog: connections that can wait
    
    
    cout << "Server Listening for Clients\n";
    
    try{
        while(serverRunning) {
            sockaddr_in clientAddress{0};
            auto clientAddressLength = sizeof(clientAddress);
            int clientSocketId = accept(socketId, (struct sockaddr*) &clientAddress, (socklen_t*) &clientAddressLength);
            if(clientSocketId < 0){
                throw ExceptionServer{"Failed to accept client connection"};
            }else{
                newClientQueue.push(NewClientInfo(clientAddress, clientSocketId));
            }
        }
    }catch(ExceptionServer e){
        cout << "Server Error: " << e.reason << "\n";
    }
    serverRunning = false;
    for(ClientConnection* client : clientConnections){
        delete client;
    }
    close(socketId);
}

void Server::sendToAll(Packet* packet, unsigned char size){
    for(ClientConnection* client : clientConnections){
        client->sendToClient(packet, size);
    }
}

void Server::sendToAllBut(int clientId, Packet* packet, unsigned char size){
    for(ClientConnection* client : clientConnections){
        if(client->getClientId() != clientId){
            client->sendToClient(packet, size);
        }
    }
}

void Server::newActor(Actor* actor){
    room->newActor(actor);
    auto packet = Packet_S2C_NewActor(actor);
    sendToAll(&packet, (unsigned char)sizeof(packet));
}

void Server::handleNewClient(NewClientInfo info){
    ClientConnection* client = new ClientConnection(this, info.address, info.socketId, nextActorId++);
    clientConnections.push_back(client);
    
    {
        // Send welcome packet to tell the client their ID
        auto packet = Packet_S2C_TellClientsID();
        packet.newClientId = client->getClientId();
        client->sendToClient(&packet, (unsigned char)sizeof(packet));
    }
    
    cout << "New client connected, assigned ID: " << client->getClientId() << endl;
    
    {
        // Send Room data to client
        auto packet = Packet_S2C_NewRoom();
        packet.width = room->getWidth();
        packet.height = room->getHeight();
        client->sendToClient(&packet, (unsigned char)sizeof(packet));
    }
    
    {
        // Send Tiles to client
        auto packet = Packet_S2C_SetTile();
        for(int i = 0; i < room->getWidth(); i++){
            packet.x = i;
            for(int j = 0; j < room->getHeight(); j++){
                packet.y = j;
                packet.tile = room->tileAt(i, j);
                client->sendToClient(&packet, (unsigned char)sizeof(packet));
            }
        }
    }
    
    {
        
        // Send all the actors to the client
        for(auto actorPair : room->actors){
            auto packet = Packet_S2C_NewActor(actorPair.second);
            client->sendToClient(&packet, (unsigned char)sizeof(packet));
        }
    }
    
    // Create a new actor for the player
    Actor* playerActor = new Actor(client->getClientId(), 0, 0, (FONT_W/2.0)-1, (FONT_H/2.0)-2);
    newActor(playerActor);
}

void Server::update() {
    
    last = now;
    now = SDL_GetPerformanceCounter();
    delta = ((now - last) * 1000.0) / SDL_GetPerformanceFrequency();
    
    while(!newClientQueue.empty()){
        NewClientInfo info = newClientQueue.front();
        newClientQueue.pop();
        
        handleNewClient(info);
    }
        
    
    for(int i = 0; i < clientConnections.size(); i++){
        clientConnections[i]->update();
        
        if(!clientConnections[i]->isClientConnected()){
            clientConnections.erase(clientConnections.begin() + i);
            i--;
        }
    }
    room->update(nullptr, delta, 0, 0);
    for(auto actorPair : room->actors){
        Actor* actor = actorPair.second;
        
        auto packet = Packet_BI_ActorMove(actor);
        sendToAllBut(packet.actorId, &packet, sizeof(Packet_BI_ActorMove));
    }
}





