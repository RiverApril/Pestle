//
//  Network.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/25/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Network_hpp
#define Network_hpp

#include "Global.hpp"

typedef unsigned short packet_size_t;

struct Packet;

struct NetworkException {
    string reason;
};

struct NetworkParticipant{

    bool tryToWrite(int socket, void* data, packet_size_t size);
    bool tryToRead(int socket, void* data, packet_size_t size);

    void sendToSocket(int socket, Packet* packet);

};



#endif /* MenuMain_hpp */
