//
//  Room.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Room_hpp
#define Room_hpp

#include "Global.hpp"
#include "TileData.hpp"

class Actor;
class Display;


class Room {
    int width, height;
    
    vector<vector<TileData>> tiles;
    
public:
    
    map<int, Actor*> actors;
    
    
    Room(int width, int height);
    
    TileData tileAt(int x, int y);
    
    int getWidth(){return width;}
    int getHeight(){return height;}
    
    void generate();
    
    void update(Display* display, double delta, int xOff, int yOff);
    
    void setTile(int x, int y, TileData tile);
    
    void newActor(Actor* actor);
    
    void replaceActor(Actor* actor);
    
    Actor* getActor(int actorId);
    
};

#endif /* Room_hpp */
