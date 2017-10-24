//
//  Settings.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/4/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Settings_hpp
#define Settings_hpp

#include "Global.hpp"

class Settings{
public:
    int winWidth = 640;
    int winHeight = 480;
    
    SDL_Scancode keyLeft  = SDL_SCANCODE_A;
    SDL_Scancode keyRight = SDL_SCANCODE_D;
    SDL_Scancode keyUp    = SDL_SCANCODE_W;
    SDL_Scancode keyDown  = SDL_SCANCODE_S;
};

#endif /* Settings_hpp */
