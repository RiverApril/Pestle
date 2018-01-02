//
//  TileData.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/7/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef TileData_hpp
#define TileData_hpp

enum Tile{
    tileEmpty,
    tileWall,
    tileDoor,
    tileDoorWay
};

struct TileData{
    
    TileData(){}
    TileData(Tile tile) : tile(tile){}
    TileData(Tile tile, int meta) : tile(tile), meta(meta){}
    
    Tile tile = tileEmpty;
    int meta = 0;
};

char getTileChar(TileData tileData);
bool isTileInv(TileData tileData);
bool isTileSolid(TileData tileData);

#endif /* TileData_hpp */
