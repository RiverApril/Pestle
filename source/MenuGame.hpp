//
//  MenuGame.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/9/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef MenuGame_hpp
#define MenuGame_hpp

#include "Global.hpp"
#include "Menu.hpp"

class Display;
class Room;
class ActorPlayer;

class MenuGame : public Menu {
    
    int viewXOff = 0;
    int viewYOff = 0;
    
public:
    
    Room* room = nullptr;
    ActorPlayer* player = nullptr;
    
    MenuGame(Logic* logic);
    ~MenuGame();
    
    void update(Display* display, double delta);
    void keyDown(SDL_Scancode scancode);
    void keyUp(SDL_Scancode scancode);
    
};

#endif /* MenuGame_hpp */
