//
//  Timer.cpp
//  FmodTest_macosx
//
//  Created by Cody Claborn on 1/22/16.
//  Copyright © 2016 oxygine. All rights reserved.
//

#include "Timer.h"
#include "core/log.h"

Timer::Timer(){
	//log::messageln("Timer created");
}

Timer::~Timer(){
}

Timer Timer::start(double timerLength, int loops, function<void(Timer&)> callback){
	log::messageln("Timer started time:%d, loops:%d", time, loops);
	this->stop();
    _duration = timerLength;
    _loopsLeft = loops;
    _callback = callback;
    
    _elapsed = 0;
    _running = true;
    _pauseTime = 0;
    
    return *this;
}

void Timer::update(const UpdateState &us) {
	_elapsed += us.dt / 1000.0;
    if (_running && _duration < _elapsed) {
		log::messageln("Timer triggered");
        _loopsLeft--;
        if (_callback != NULL){
            _callback(*this);
			log::messageln("Callback called");
        }
        if (_loopsLeft > 0){
			log::messageln("Timer going another loop");
            _elapsed = 0;
        } else {
			this->stop();
        }
    }
}

void Timer::stop() {
	log::messageln("Timer stoped");
    _running = false;
    _duration = 0;
	_elapsed = 0;
    _loopsLeft = 0;
    _pauseTime = 0;
}

void Timer::pause() {
	// TODO: Make this work
    //_running = false;
    //_pauseTime += (clock() - _start ) / (double) CLOCKS_PER_SEC;
}

void Timer::resume() {
   // _running = true;
}
