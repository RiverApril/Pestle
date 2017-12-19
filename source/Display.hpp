//
//  Display_h
//  Pestle
//
//  Created by Braeden Atlee on 10/4/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Display_h
#define Display_h

#include "Global.hpp"
#include "Settings.hpp"

struct ExceptionSDLFailed{
    string reason;
};

class Logic;

class Display{
    
    void init();
    
    Settings* settings;
    
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    SDL_Texture* textureFont;
    SDL_Texture* textureFontInv;
    
    SDL_Event* event;
    
    Logic* logic;
    
    const Uint8* keyboardState;
    
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double delta = 0;
    
    
public:
    Display(Logic* logic, Settings* settings);
    ~Display();
    
    void update();
    
    void drawChar(char c, int x, int y, bool inv = false);
    void drawString(string s, int x, int y);
    
    SDL_Window* getWindow(){ return sdlWindow; }
    SDL_Renderer* getRenderer(){ return sdlRenderer; }
    
    bool isKey(int key){ return keyboardState[key]; }
    void getMouse(int& x, int& y){
        SDL_GetMouseState(&x, &y);
    }
};


#endif /* Display_h */
