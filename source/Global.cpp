//
//  Global.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/9/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Global.hpp"

void saveToBuffer(unsigned char*& dataPointer, void* obj, packet_size_t size){
    memcpy(dataPointer, obj, size);
    dataPointer += size;
}

void loadFromBuffer(unsigned char*& dataPointer, void* obj, packet_size_t size){
    memcpy(obj, dataPointer, size);
    dataPointer += size;
}

