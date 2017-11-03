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

struct Packet;

struct NetworkException {
    string reason;
};

struct NetworkParticipant{

    bool tryToWrite(socket_t socket, void* data, packet_size_t size);
    bool tryToRead(socket_t socket, void* data, packet_size_t size);

    void sendToSocket(socket_t socket, Packet* packet);

};



#endif /* MenuMain_hpp */
