#pragma once
#include "AudioEngine.h"
#include <string>

namespace SoundManager {
    
    extern CAudioEngine aEngine;
    
    void init();
    void destroy();
    void update(float fTimeDeltaSeconds);
    int registerSound(const SoundDefinition &tSoundDefinition, int nSoundId, bool bLoad = true);
    void unRegisterSound(int nSoundId);
    int playSound(int nSoundId);
    void setChannelVolume(int nChannelId,float fVolumedB);
    void stopChannel(int nChannelId, float fFadeTimeSeconds = 0.0f);
    void loadBank(const string &bankFileLocaiton);
    void loadEvent(const string &eventName);
    void playEvent(const string &eventName);
    void stopEvent(const string &eventName);
    void setEventParam(const string &eventName, const string &paramName, float value);
    bool isEventPlaying(const string &eventName);
}
