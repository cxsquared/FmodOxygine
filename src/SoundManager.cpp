#include "SoundManager.h"

namespace SoundManager{
    
    CAudioEngine aEngine;
    
    void init() {
        aEngine.Init();
    }
    
    void update(float fTimeDeltaSeconds) {
        //cout << "Sound Manager updating ..." << endl;
        aEngine.Update(fTimeDeltaSeconds);
    }
    
    void destroy() {
        aEngine.Shutdown();
    }
    
    int playSound(int nSoundId) {
        return aEngine.PlaySound(nSoundId);
    }
    
    void setChannelVolume(int nChannelId,float fDb) {
        aEngine.SetChannelVolume(nChannelId, fDb);
    }
    
    void stopChannel(int nChannelId, float fFadeTimeSeconds)  {
        aEngine.StopChannel(nChannelId, fFadeTimeSeconds);
    }
    
    int registerSound(const SoundDefinition &tSoundDefinition, int nSoundId, bool bLoad){
        return aEngine.RegisterSound(tSoundDefinition, nSoundId, bLoad);
    }
    
    void loadBank(const string &bankFileLocaiton){
        aEngine.LoadBank(bankFileLocaiton,  FMOD_STUDIO_LOAD_BANK_NORMAL);
    }
    
    void loadEvent(const string &eventName){
        aEngine.LoadEvent(eventName);
    }
    
    void playEvent(const string &eventName){
        aEngine.PlayEvent(eventName);
    }
    
    void stopEvent(const string &eventName) {
        aEngine.StopEvent(eventName);
    }
    
    void setEventParam(const string &eventName, const string &paramName, float fValue){
        aEngine.SetEventParameter(eventName, paramName, fValue);
    }
    
    bool isEventPlaying(const string &eventName) {
        bool isPlaying = aEngine.IsEventPlaying(eventName);
        cout << eventName << " is playing= " << isPlaying << endl;
        return isPlaying;
    }
    
    void loadPlugin(const char* file, const char* path, unsigned int *handle) {
        aEngine.loadPlugin(file, path, handle);
    }
}