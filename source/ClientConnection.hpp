//
//  ClientConnection.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef ClientConnection_hpp
#define ClientConnection_hpp

#include "Global.hpp"
#include "Packet.hpp"
#include "Utility.hpp"

class Server;

class ClientConnection{
    
    Server* server;
    
    mutex writeMutex;
    
    sockaddr_in address;
    int socketId;
    
    int clientId;
    
    bool clientIsConnected = false;
    
    thread listenToClientThread;
    
    sharedQueue<PacketInfo> packetQueue;
    
    void listenToClient();
    
    bool tryToWrite(int socket, void* data, size_t size);
    bool tryToRead(int socket, void* data, size_t size);
    
public:
    ClientConnection(Server* server, sockaddr_in address, int socketId, int clientId);
    ~ClientConnection();
    
    int getClientId(){ return clientId; }
    
    void handlePacket(pid packetId, void* packetPointer);
    
    void sendToClient(Packet* packet, unsigned char size);
    
    void update();
    
    bool isClientConnected(){ return clientIsConnected; }
    
};

#endif /* ClientConnection_hpp */
