//
//  Geometry.cpp
//  Pestle
//
//  Created by Braeden Atlee on 10/10/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#include "Geometry.hpp"


bool AABBOverlap(double ax, double ay, double arx, double ary, double bx, double by, double brx, double bry){
    return fabs(ax-bx) < (arx+brx) && fabs(ay-by) < (ary+bry);
}
