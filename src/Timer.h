//
//  Timer.hpp
//  FmodTest_macosx
//
//  Created by Cody Claborn on 1/22/16.
//  Copyright © 2016 oxygine. All rights reserved.
//

#pragma once
#include <functional>
#include "oxygine-framework.h"

using namespace oxygine;
using namespace std;

class Timer {
public:
	Timer();
    ~Timer();
	Timer start(double time, int loops = 1, function<void(Timer&)> callback=NULL);
    void update(const UpdateState &us);
    void stop();
    void pause();
    void resume();
private:
    int _loopsLeft;
    double _duration;
    double _elapsed;
    function<void(Timer&)> _callback;
    bool _running;
    double _pauseTime = 0;
};
