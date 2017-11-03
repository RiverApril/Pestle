//
//  Client.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp

#include "Global.hpp"
#include "Packet.hpp"
#include "Utility.hpp"
#include "Network.hpp"

class MenuGame;

class Client : public NetworkParticipant{
    
    mutex writeMutex;

    int clientId = -1;
    
    MenuGame* game;
    
    string ip;
    int port;
    
    socket_t socketId;
    
    sockaddr_in serverAddress;
    
    thread listenToServerThread;
    
    sharedQueue<PacketInfo> packetQueue;
    
    bool connectedToServer = false;
    
    void listenToServer();
    
    void handlePacket(pid packetId, void* packetPointer, packet_size_t packetSize);

    
public:
    Client(MenuGame* game, string ip, int port);
    ~Client();
    
    void sendToServer(Packet* packet);
    
    void update();
    
    bool isConnectedToServer(){ return connectedToServer; }
    
    int getClientId(){ return clientId; }
    
    #if STATS_DEBUG
    int packetsReceivedCount = 0;
    int packetsSentCount = 0;
    #endif
    
};


#endif /* Client_hpp */
