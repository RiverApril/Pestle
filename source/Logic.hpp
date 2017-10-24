//
//  Logic.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/4/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Logic_hpp
#define Logic_hpp

#include "Global.hpp"
#include "Settings.hpp"
#include "Client.hpp"

struct ExceptionGameQuit{
    string reason;
};


class Display;
class Menu;
class Client;

class Logic{
    
public:
    Logic(Settings* settings);
    
    void update(Display* display, double delta);
    void processEvent(SDL_Event* event);
    
    void changeMenu(Menu* menu);
    
    Client* client = nullptr;
    
    Settings* settings = nullptr;
    
private:
    
    void keyDown(SDL_Scancode scancode);
    void keyUp(SDL_Scancode scancode);
    
    Menu* activeMenu = nullptr;
    
};


#endif /* Logic_hpp */
