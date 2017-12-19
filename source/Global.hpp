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
#include <fcntl.h>
#include <chrono>
#include <mutex>

#include <cstdint>

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")

#define close(a) closesocket(a)
typedef SOCKET socket_t;
#define write(a, b, c) send(a, (const char*)b, c, 0)
#define read(a, b, c) recv(a, (char*)b, c, 0)
#define GET_SOCKET_ERROR (WSAGetLastError())

#else

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <unistd.h> 

typedef int socket_t;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define GET_SOCKET_ERROR (to_string(errno))

#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define VERSION "0.1a"

#define FONT_W 7
#define FONT_H 12

#define DRAW_DEBUG 1
#define STATS_DEBUG 1

#define SERVER_UPDATE_DELAY (1.0/60.0)

using namespace std;


typedef unsigned short packet_size_t;


#define SAVE(a) (saveToBuffer(dataPointer, &(a), sizeof(a)))
#define LOAD(a) (loadFromBuffer(dataPointer, &(a), sizeof(a)))

void saveToBuffer(unsigned char*& dataPointer, void* obj, packet_size_t size);
void loadFromBuffer(unsigned char*& dataPointer, void* obj, packet_size_t size);

#endif /* Global_h */
