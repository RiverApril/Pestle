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

struct ExceptionClient{
    string reason;
};

class MenuGame;

class Client{
    
    mutex writeMutex;
    
    int clientId = -1;
    
    MenuGame* game;
    
    string ip;
    int port;
    
    int socketId;
    
    sockaddr_in serverAddress;
    
    thread listenToServerThread;
    
    sharedQueue<PacketInfo> packetQueue;
    
    bool connectedToServer = false;
    
    void listenToServer();
    
    void handlePacket(pid packetId, void* packetPointer);
    
    bool tryToWrite(int socket, void* data, size_t size);
    bool tryToRead(int socket, void* data, size_t size);

    
public:
    Client(MenuGame* game, string ip, int port);
    ~Client();
    
    void sendToServer(Packet* packet, unsigned char size);
    
    void update();
    
    bool isConnectedToServer(){ return connectedToServer; }
    
    int getClientId(){ return clientId; }
    
};


#endif /* Client_hpp */
