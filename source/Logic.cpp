//
//  GameLogic.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/4/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Logic.hpp"
#include "MenuMain.hpp"

Logic::Logic(Settings* settings) : settings(settings){
    activeMenu = new MenuMain(this);
}

void Logic::changeMenu(Menu* menu){
    if(activeMenu){
        delete activeMenu;
    }
    activeMenu = menu;
}

void Logic::update(Display* display, double delta){
    activeMenu->update(display, delta);
}

void Logic::processEvent(SDL_Event* event){
    switch(event->type){
        case SDL_QUIT:{
            throw ExceptionGameQuit{"SDL_QUIT"};
            break;
        }
        case SDL_WINDOWEVENT:{
            switch (event->window.event) {
                case SDL_WINDOWEVENT_SIZE_CHANGED:{
                    settings->winWidth = event->window.data1;
                    settings->winHeight = event->window.data1;
                    break;
                }
            }
            break;
        }
        case SDL_KEYDOWN:{
            keyDown(event->key.keysym.scancode);
            break;
        }
        case SDL_KEYUP:{
            keyUp(event->key.keysym.scancode);
            break;
        }
    }
}

void Logic::keyDown(SDL_Scancode scancode){
    activeMenu->keyDown(scancode);
}

void Logic::keyUp(SDL_Scancode scancode){
    activeMenu->keyUp(scancode);
}



