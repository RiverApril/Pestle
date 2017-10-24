//
//  MenuMain.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef MenuMain_hpp
#define MenuMain_hpp

#include "Global.hpp"
#include "Menu.hpp"

class Display;

class MenuMain : public Menu {
    
public:
    MenuMain(Logic* logic);
    ~MenuMain(){}
    
    void update(Display* display, double delta);
    void keyDown(SDL_Scancode scancode);
    
};

#endif /* MenuMain_hpp */
