//
//  main.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/4/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "main.hpp"
#include "Logic.hpp"
#include "Display.hpp"
#include "Server.hpp"

int main(int argc, char *argv[]){
    
    bool startClient = true;
    
    if(argc == 2){
        if(string(argv[1]).compare("server") == 0){
            startClient = false;
        }else{
            startClient = true;
        }
    }
    
    cout << "Size of int: " << sizeof(int) << "\n";
    cout << "Size of double: " << sizeof(double) << "\n";
    cout << "Size of size_t: " << sizeof(size_t) << "\n";
    cout << "Size of packet_size_t: " << sizeof(packet_size_t) << "\n";


#ifdef _WIN32
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
#endif
    
    if(startClient){
        
        Settings* settings = new Settings();
        
        Logic* logic = new Logic(settings);
        Display* display = new Display(logic, settings);
        
        try{
            while(true){
                display->update();
            }
        }catch(ExceptionGameQuit e){
            cout << "Game Quit: " << e.reason << "\n";
        }
        
    } else {
        
        try{
            Server* server = new Server(1337);
            while(server->isServerRunning()){
                server->update();
            }
        }catch(NetworkException e){
            cout << "Server Error: " << e.reason << "\n";
        }
        
    }
    
    return 0;
}

