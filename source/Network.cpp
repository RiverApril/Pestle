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
    ssize_t wrote;
    auto remainingSize = size;
    unsigned char* dataPointer = (unsigned char*)data;

    while(remainingSize > 0 && (wrote = write(socket, dataPointer, remainingSize)) > 0 ){
        dataPointer += wrote;
        remainingSize -= wrote;
    }
    if(remainingSize == 0){
        return true;
    }else{
        throw NetworkException{"Failed to write to socket"};
    }
}

bool NetworkParticipant::tryToRead(socket_t socket, void* data, packet_size_t size){
    ssize_t red; // read was taken
    auto remainingSize = size;
    unsigned char* dataPointer = (unsigned char*)data;

    while(remainingSize > 0 && (red = read(socket, dataPointer, remainingSize)) > 0 ){
        dataPointer += red;
        remainingSize -= red;
    }
    if(remainingSize == 0){
        return true;
    }else{
        throw NetworkException{"Failed to read from socket"};
    }
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

