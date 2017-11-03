//
//  Room.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Room.hpp"
#include "Actor.hpp"
#include "Display.hpp"

Room::Room(int width, int height) : width(width), height(height){
    for(int x = 0; x < width; x++){
        tiles.push_back(vector<TileData>());
        for(int y = 0; y < height; y++){
            tiles[x].push_back(TileData());
        }
    }
}

void Room::generate(){
    srand((int)time(0));
    
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
            if(rand()%10 == 0){
                tiles[x][y].tile = tileWall;
            }else if(rand()%30 == 0){
                tiles[x][y].tile = tileDoor;
            }else if(rand()%30 == 0){
                tiles[x][y].tile = tileDoorWay;
            }
            
        }
    }
}

void Room::newActor(Actor* actor){
    actors.emplace(actor->getId(), actor);
}

Actor* Room::getActor(int actorId){
    auto it = actors.find(actorId);
    if(it == actors.end()){
        return nullptr;
    }
    return it->second;
}

void Room::replaceActor(Actor* actor){
    if(actors.find(actor->getId()) != actors.end()){
        delete actors[actor->getId()];
    }
    actors[actor->getId()] = actor;
}

void Room::update(Display* display, double delta, int xOff, int yOff){
    
    if(display){
    
        int wW, wH;
        SDL_GetWindowSize(display->getWindow(), &wW, &wH);
        
        int minX = max(0, xOff/FONT_W);
        int minY = max(0, yOff/FONT_H);
        
        int maxX = min(width, (xOff+wW)/FONT_W+1);
        int maxY = min(height, (yOff+wH)/FONT_H+1);
        
        for(int y = minY; y < maxY; y++){
            for(int x = minX; x < maxX; x++){
                TileData tileData = tileAt(x, y);
                if(tileData.tile != tileEmpty){
                    display->drawChar(getTileChar(tileData), x*FONT_W-xOff-FONT_W/2, y*FONT_H-yOff-FONT_H/2, isTileInv(tileData));
                }
            }
        }
    }
    
    for(auto actorPair : actors){
        actorPair.second->update(this, display, delta, xOff, yOff);
    }
}

TileData Room::tileAt(int x, int y){
    if(x >= 0 && y >= 0 && x < width && y < height){
        return tiles[x][y];
    }else{
        return TileData();
    }
}

void Room::setTile(int x, int y, TileData tile){
    if(x >= 0 && y >= 0 && x < width && y < height){
        tiles[x][y] = tile;
    }
}

Actor* Room::findNearestActor(double cx, double cy, function<bool(Actor*, double)> isValid){
    Actor* close = nullptr;
    double closeDistSq = -1;
    for(auto actorPair : actors){
        Actor* a = actorPair.second;
        double distSq = (a->px-cx)*(a->px-cx) + (a->py-cy)*(a->py-cy);
        if((distSq < closeDistSq || closeDistSq == -1) && isValid(a, distSq)){
            closeDistSq = distSq;
            close = a;
        }
    }
    return close;
}

