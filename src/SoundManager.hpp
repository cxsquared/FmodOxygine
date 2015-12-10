#pragma once
#include "AudioEngine.h"
#include <string>

namespace SoundManager {
    
    extern CAudioEngine aEngine;
    
    void init();
    void destroy();
    void update();
    void loadSound(const string &fileLocation);
    int playSound(const string &fileLocation);
    void stopChannel(int nChannelId);
    void loadBank(const string &bankFileLocaiton);
    void loadEvent(const string &eventName);
    void playEvent(const string &eventName);
    void stopEvent(const string &eventName);
    void setEventParam(const string &eventName, const string &paramName, float value);
    bool isEventPlaying(const string &eventName);
}
