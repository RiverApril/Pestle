//
//  MenuMain.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "MenuMain.hpp"
#include "MenuGame.hpp"
#include "Client.hpp"
#include "Logic.hpp"
#include "Display.hpp"


MenuMain::MenuMain(Logic* logic) : Menu(logic){
    
}

void MenuMain::update(Display* display, double delta){
    
    display->drawString("Pestle", 0, 0);
    
    // temp:
    string ip;
    int port;
    /*cout << "Enter ip: ";
    cin >> ip;
    cout << "Enter port: ";
    cin >> port;*/
    ip = "127.0.0.1";
    port = 1337;
    
    MenuGame* game = new MenuGame(logic);
    
    try{
        // no local for now, just connect to server:
        logic->client = new Client(game, ip, port);
    }catch(NetworkException e){
        cout << "Network Error: " << e.reason << "\n";
        throw ExceptionGameQuit({"Network Error, quitting."});
    }
    
    logic->changeMenu(game);
}

void MenuMain::keyDown(SDL_Scancode scancode){
    
}


