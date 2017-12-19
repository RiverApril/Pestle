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
#include "ActorPlayer.hpp"
#include "ActorEnemy.hpp"

Server::Server(int port){
    
    if(SDL_Init(0) < 0){
        throw NetworkException{string("SDL_INIT Failed: ") + SDL_GetError()};
    }
    
    room = new Room(50, 30);
    room->generate();

    room->newActor(new ActorEnemy(nextActorId++));

    
    // AF_INET means internet domain instead of file domain
    // SOCK_STREAM means to use a stream instead of a block, so it chooses TCP
    socketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(socketId == INVALID_SOCKET){
        throw NetworkException{"Failed to open server socket"};
    }
    
    serverAddress = sockaddr_in{0};
    
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port); // corrects byte order
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Sets the address to be the server's address
    
    if(bind(socketId, (const struct sockaddr*) &serverAddress, (socklen_t)sizeof(serverAddress)) == SOCKET_ERROR){
        throw NetworkException{"Faild to bind socket to port"};
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
            socket_t clientSocketId = accept(socketId, (struct sockaddr*) &clientAddress, (socklen_t*) &clientAddressLength);
            if(clientSocketId == INVALID_SOCKET){
                throw NetworkException{"Failed to accept client connection"};
            }else{
                newClientQueue.push(NewClientInfo(clientAddress, clientSocketId));
            }
        }
    }catch(NetworkException e){
        cout << "Server Error: " << e.reason << "\n";
    }
    serverRunning = false;
    for(ClientConnection* client : clientConnections){
        delete client;
    }
    close(socketId);
}

void Server::sendToAll(Packet* packet){
    for(ClientConnection* client : clientConnections){
        client->sendToClient(packet);
    }
}

void Server::sendToAllBut(int clientId, Packet* packet){
    for(ClientConnection* client : clientConnections){
        if(client->getClientId() != clientId){
            client->sendToClient(packet);
        }
    }
}

void Server::newActor(Actor* actor){
    room->newActor(actor);
    auto* packet = new Packet_S2C_NewActor(actor);
    sendToAll(packet);
    delete packet;
}

void Server::handleNewClient(NewClientInfo info){
    ClientConnection* client = new ClientConnection(this, info.address, info.socketId, nextActorId++);
    clientConnections.push_back(client);
    
    {
        // Send welcome packet to tell the client their ID
        auto* packet = new Packet_S2C_TellClientsID(client->getClientId());
        client->sendToClient(packet);
        delete packet;
    }
    
    cout << "New client connected, assigned ID: " << client->getClientId() << endl;
    
    {
        // Send Room data to client
        auto* packet = new Packet_S2C_NewRoom(room->getWidth(), room->getHeight());
        client->sendToClient(packet);
        delete packet;
    }
    
    {
        // Send Tiles to client
        auto* packet = new Packet_S2C_SetTile(0, 0, TileData());
        for(int i = 0; i < room->getWidth(); i++){
            packet->x = i;
            for(int j = 0; j < room->getHeight(); j++){
                packet->y = j;
                packet->tile = room->tileAt(i, j);
                client->sendToClient(packet);
            }
        }
        delete packet;
    }
    
    {
        
        // Send all the actors to the client
        for(auto actorPair : room->actors){
            auto* packet = new Packet_S2C_NewActor(actorPair.second);
            client->sendToClient(packet);
            delete packet;
        }
    }
    
    // Create a new actor for the player
    ActorPlayer* playerActor = new ActorPlayer(client->getClientId());
    newActor(playerActor);
}

void Server::update() {
    
    last = now;
    now = SDL_GetPerformanceCounter();
    delta = ((now - last) * 1000.0) / SDL_GetPerformanceFrequency();

    if(delta < SERVER_UPDATE_DELAY){
        using namespace chrono_literals;
        this_thread::sleep_for((SERVER_UPDATE_DELAY - delta)*1s);
    }
    
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
        ActorMoving* actorMoving = dynamic_cast<ActorMoving*>(actorPair.second);

        if(actorMoving && actorMoving->hasChangedVelocity){
            auto* packet = new Packet_BI_ActorMove(actorMoving);
            sendToAllBut(packet->actorId, packet);
            delete packet;
        }
    }
}





