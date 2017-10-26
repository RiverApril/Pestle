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
#include "Network.hpp"

class Server;

class ClientConnection : public NetworkParticipant{
    
    Server* server;
    
    mutex writeMutex;
    
    sockaddr_in address;
    int socketId;
    
    int clientId;
    
    bool clientIsConnected = false;
    
    thread listenToClientThread;
    
    sharedQueue<PacketInfo> packetQueue;
    
    void listenToClient();
    
public:
    ClientConnection(Server* server, sockaddr_in address, int socketId, int clientId);
    ~ClientConnection();
    
    int getClientId(){ return clientId; }
    
    void handlePacket(pid packetId, void* packetPointer, packet_size_t packetSize);
    
    void sendToClient(Packet* packet);
    
    void update();
    
    bool isClientConnected(){ return clientIsConnected; }
    
};

#endif /* ClientConnection_hpp */
