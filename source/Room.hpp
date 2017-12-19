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
    
    void update(Display* display, double delta, double xOff, double yOff);
    
    void setTile(int x, int y, TileData tile);
    
    void newActor(Actor* actor);
    
    void replaceActor(Actor* actor);
    
    Actor* getActor(int actorId);

    // bool isValid(Actor* check, double distSq)
    Actor* findNearestActor(double cx, double cy, function<bool(Actor*, double)> isValid);
    
};

#endif /* Room_hpp */
