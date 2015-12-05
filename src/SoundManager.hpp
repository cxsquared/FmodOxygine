#pragma once
#include "AudioEngine.h"
#include <string>

namespace SoundManager {
    
    extern CAudioEngine aEngine;
    
    void init();
    void destroy();
    void update();
    void loadSound(const string &fileName);
    void playSound(const string &fileName);
}
