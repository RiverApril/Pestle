//
//  Network.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Network.hpp"
#include "Packet.hpp"

bool NetworkParticipant::tryToWrite(socket_t socket, void* data, packet_size_t size){
    auto writeSize = write(socket, data, size);
    if(writeSize == size){
        return true;
    }else{
        if(writeSize != -1){
            cout << "Only wrote " << writeSize << " bytes out of " << size << endl;
        }
        throw NetworkException{"Failed to write to socket"};
    }
}

bool NetworkParticipant::tryToRead(socket_t socket, void* data, packet_size_t size){
    auto readSize = read(socket, data, size);
    if(readSize == size){
        return true;
    }else{
        if(readSize != -1){
            cout << "Only read " << readSize << " / " << size << " bytes" << endl;
        }
    }
    throw NetworkException{"Failed to read from socket"};
}

void NetworkParticipant::sendToSocket(socket_t socket, Packet* packet){
    packet_size_t size = packet->getSize();
    
    pid packetId = packet->getID();
    
    // send packet id
    tryToWrite(socket, &packetId, sizeof(packetId));
    
    // send packet size
    tryToWrite(socket, &size, sizeof(size));
    
    // send packet itself
    packet->sendData(this, socket);
}

