//
//  Global.h
//  Pestle
//
//  Created by Braeden Atlee on 10/4/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Global_h
#define Global_h

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>

#include <thread>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h> 
#include <fcntl.h>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#define VERSION "0.1a"

#define FONT_W 7
#define FONT_H 12

#define DRAW_DEBUG 0

typedef unsigned char byte;

using namespace std;


#define SAVE(a) (saveToBuffer(bufferPointer, &(a), sizeof(a)))
#define LOAD(a) (loadFromBuffer(bufferPointer, &(a), sizeof(a)))

void saveToBuffer(byte*& dataPointer, void* obj, size_t size);
void loadFromBuffer(byte*& dataPointer, void* obj, size_t size);

#endif /* Global_h */
