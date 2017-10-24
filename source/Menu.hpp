//
//  Menu.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Menu_hpp
#define Menu_hpp

#include "Global.hpp"

class Logic;
class Display;

class Menu {
    
protected:
    
    Logic* logic;
    
public:
    Menu(Logic* logic);
    virtual ~Menu(){}
    
    virtual void update(Display* display, double delta){}
    virtual void keyDown(SDL_Scancode scancode){}
    virtual void keyUp(SDL_Scancode scancode){}
    
};

#endif /* Menu_hpp */
