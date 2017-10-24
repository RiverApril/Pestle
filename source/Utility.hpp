//
//  Utility.hpp
//  Pestle
//
//  Created by Braeden Atlee on 10/10/17.
//  Copyright © 2017 Braeden Atlee. All rights reserved.
//

#ifndef Utility_hpp
#define Utility_hpp

#include "Global.hpp"

template <typename T>
class sharedQueue{
    
    queue<T> queue;
    mutex lockMutex;
    
public:
    
    T& front(){
        unique_lock<mutex> lock(lockMutex);
        return queue.front();
    }
    void pop(){
        unique_lock<mutex> lock(lockMutex);
        queue.pop();
    }
    
    void push(const T& item){
        unique_lock<mutex> lock(lockMutex);
        queue.push(item);
    }
    
    size_t size(){
        unique_lock<mutex> lock(lockMutex);
        return queue.size();
    }
    
    bool empty(){
        unique_lock<mutex> lock(lockMutex);
        return queue.empty();
    }
    
};

#endif /* Utility_hpp */
