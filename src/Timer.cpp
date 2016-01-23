//
//  Timer.cpp
//  FmodTest_macosx
//
//  Created by Cody Claborn on 1/22/16.
//  Copyright © 2016 oxygine. All rights reserved.
//

#include "Timer.h"

Timer::Timer(){
}

Timer::~Timer(){
}

Timer Timer::start(double time, int loops, function<void(Timer)> callback){
    _duration = time;
    _loopsLeft = loops;
    _callback = callback;
    
    _start = clock();
    _running = true;
    _pauseTime = 0;
    
    return *this;
}

void Timer::update() {
    if (_running && _duration < ( (clock() - _start ) / (double) CLOCKS_PER_SEC) + _pauseTime) {
        _loopsLeft--;
        if (_callback != NULL){
            _callback(*this);
        }
        if (_loopsLeft > 0){
            _start = clock();
        } else {
            _running = false;
        }
    }
}

void Timer::stop() {
    _running = false;
    _duration = 0;
    _loopsLeft = 0;
    _pauseTime = 0;
}

void Timer::pause() {
    _running = false;
    _pauseTime += (clock() - _start ) / (double) CLOCKS_PER_SEC;
}

void Timer::resume() {
    _running = true;
}
