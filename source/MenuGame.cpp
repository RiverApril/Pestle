//
//  MenuGame.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/9/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "MenuGame.hpp"
#include "MenuMain.hpp"
#include "Logic.hpp"
#include "Room.hpp"
#include "Display.hpp"
#include "ActorPlayer.hpp"
#include "ClientAction.hpp"


MenuGame::MenuGame(Logic* logic) : Menu(logic){
    room = new Room(50, 30);
    //playerActor = new Actor(logic->client->getClientId(), 0, 0, (FONT_W/2.0)-1, (FONT_H/2.0)-2);
    //room->newActor(playerActor);
}

MenuGame::~MenuGame(){
    delete room;
}

void MenuGame::update(Display* display, double delta){
    
    try{
    
        if(logic->client){
            if(logic->client->isConnectedToServer()){
                logic->client->update();
            }else{
                logic->changeMenu(new MenuMain(logic));
            }
            #if STATS_DEBUG
            display->drawString("Packets in: "+to_string(logic->client->packetsReceivedCount), 0, 0);
            display->drawString("Packets out: "+to_string(logic->client->packetsSentCount), 0, FONT_H);
            #endif
        }
        
        if(player){
            
            double vx = 0;
            double vy = 0;

            double speed = 0.05;

            if(display->isKey(logic->settings->keyLeft)){
                vx -= speed;
            }
            if(display->isKey(logic->settings->keyRight)){
                vx += speed;
            }
            if(display->isKey(logic->settings->keyUp)){
                vy -= speed;
            }
            if(display->isKey(logic->settings->keyDown)){
                vy += speed;
            }

            player->vx = vx;
            player->vy = vy;
            if(player->hasChangedVelocity){
                auto* packet = new Packet_BI_ActorMove(player);
                logic->client->sendToServer(packet);
                delete packet;
            }
            
            if(shootTimer > 0){
                shootTimer -= delta;
            }
            if(display->isKey(logic->settings->keyShoot)){
                if(shootTimer <= 0){
                    shootTimer = 500;
                    int mx, my;
                    display->getMouse(mx, my);
                    auto* packet = new Packet_C2S_ClientAction(actionShoot, mx+viewXOff, my+viewYOff);
                    logic->client->sendToServer(packet);
                    delete packet;
                }
            }

            int wW, wH;
            SDL_GetWindowSize(display->getWindow(), &wW, &wH);

            viewXOff += ((player->px - wW/2) - viewXOff) / 4;
            viewYOff += ((player->py - wH/2) - viewYOff) / 4;
            
            if(room){
                room->update(display, delta, viewXOff, viewYOff);
            }
            
        }else{
            player = dynamic_cast<ActorPlayer*>(room->getActor(logic->client->getClientId()));
            if(player){
                cout << "Found Player Actor: " << player->getId() << "\n";
            }
        }
        
    }catch(NetworkException e){
        cout << "Client Error: " << e.reason << "\n";
        logic->changeMenu(new MenuMain(logic));
    }
    
    //cout << 1000/delta << "\n";
    
}

void MenuGame::keyDown(SDL_Scancode scancode){
    
}

void MenuGame::keyUp(SDL_Scancode scancode){
    
}

