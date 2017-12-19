//
//  Display.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/4/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Display.hpp"
#include "Logic.hpp"

Display::Display(Logic* logic, Settings* settings) : logic(logic), settings(settings){
    try {
        init();
    }catch(ExceptionSDLFailed e){
        cout << e.reason << endl;
    }
}

Display::~Display(){
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}

void Display::init(){
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        throw ExceptionSDLFailed{string("SDL_INIT Failed: ") + SDL_GetError()};
    }
    
    
    sdlWindow = SDL_CreateWindow("Pestle " VERSION, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, settings->winWidth, settings->winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if(!sdlWindow){
        throw ExceptionSDLFailed{string("SDL_CreateWindow Failed: ") + SDL_GetError()};
    }
    
    auto imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)){
        throw ExceptionSDLFailed{string("IMG_INIT Failed: ") + SDL_GetError()};
    }
    
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!sdlRenderer){
        throw ExceptionSDLFailed{string("SDL_CreateRenderer Failed: ") + SDL_GetError()};
    }
    
    textureFont = IMG_LoadTexture(sdlRenderer, "resources/font.png");
    if(!textureFont){
        throw ExceptionSDLFailed{string("IMG_LoadTexture Failed: ") + SDL_GetError()};
    }
    
    textureFontInv = IMG_LoadTexture(sdlRenderer, "resources/fontInv.png");
    if(!textureFontInv){
        throw ExceptionSDLFailed{string("IMG_LoadTexture Failed: ") + SDL_GetError()};
    }
    
    int w, h;
    SDL_QueryTexture(textureFont, nullptr, nullptr, &w, &h);
    
    event = new SDL_Event();
    
    keyboardState = SDL_GetKeyboardState(NULL);
    
}

void Display::drawChar(char c, int x, int y, bool inv){
    SDL_Rect src, dst;
    src.x = (c-' ')%16 * FONT_W;
    src.y = (c-' ')/16 * FONT_H;
    src.w = FONT_W;
    src.h = FONT_H;
    
    dst.x = x;
    dst.y = y;
    dst.w = FONT_W;
    dst.h = FONT_H;
    SDL_RenderCopy(sdlRenderer, inv?textureFontInv:textureFont, &src, &dst);
}

void Display::drawString(string s, int x, int y){
    for(char c : s){
        drawChar(c, x, y);
        x += FONT_W;
    }
}

void Display::update(){
    
    last = now;
    now = SDL_GetPerformanceCounter();
    delta = ((now - last) * 1000.0) / SDL_GetPerformanceFrequency();
    
    SDL_RenderClear(sdlRenderer);
    
    SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 0xFF);
    SDL_RenderFillRect(sdlRenderer, NULL);
    
    logic->update(this, delta);
    
    SDL_RenderPresent(sdlRenderer);
    
    while(SDL_PollEvent(event)){
        logic->processEvent(event);
    }
    
}


