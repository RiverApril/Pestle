//
//  Server.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include "Global.hpp"
#include "Utility.hpp"

class Room;
class ClientConnection;
struct Packet;
class Actor;

struct NewClientInfo {
    NewClientInfo(sockaddr_in address, socket_t socketId) : address(address), socketId(socketId){}
    sockaddr_in address;
    socket_t socketId;
};

class Server{
    
    vector<ClientConnection*> clientConnections;
    
    socket_t socketId;
    sockaddr_in serverAddress;
    
    sharedQueue<NewClientInfo> newClientQueue;
    
    bool serverRunning = false;
    
    int nextActorId = 1;
    
    thread listenThread;
    
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double delta = 0;
    
public:
    
    Room* room;
    
    Server(int port);
    ~Server();
    
    bool isServerRunning(){return serverRunning;}
    
    void listenForClients();
    
    void handleNewClient(NewClientInfo info);
    
    void sendToAll(Packet* packet);
    void sendToAllBut(int clientId, Packet* packet);
    
    void newActor(Actor* actor);
    
    void update();
    
};

#endif /* Server_hpp */
