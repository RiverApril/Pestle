//
//  TileData.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "TileData.hpp"


char getTileChar(TileData tileData){
    switch (tileData.tile) {
        case tileEmpty: return ' ';
        case tileWall: return ' ';
        case tileDoor: return '%';
        case tileDoorWay: return '/';
        default: return '?';
    }
}

bool isTileInv(TileData tileData){
    switch (tileData.tile) {
        case tileWall:
        case tileDoor:
            return true;
        default: return false;
    }
}

bool isTileSolid(TileData tileData){
    switch (tileData.tile) {
        case tileWall:
        case tileDoor:
            return true;
        default: return false;
    }
}
