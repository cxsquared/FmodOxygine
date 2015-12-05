#pragma once
#include "AudioEngine.h"
#include <string>

namespace SoundManager {
    
    extern CAudioEngine aEngine;
    
    void init();
    void destroy();
    void update();
    void loadSound(const string &fileLocation);
    void playSound(const string &fileLocation);
    void loadBank(const string &bankFileLocaiton);
    void loadEvent(const string &eventName);
    void playEvent(const string &eventName);
    void setEventParam(const string &eventName, const string &paramName, float value);
    bool isEventPlaying(const string &eventName);
}
