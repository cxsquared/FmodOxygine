#include "SoundManager.hpp"

namespace SoundManager{
    
    CAudioEngine aEngine;
    
    void init() {
        aEngine.Init();
    }
    
    void update() {
        aEngine.Update();
    }
    
    void destroy() {
        aEngine.Shutdown();
    }
    
    void playSound(const string &fileName) {
        aEngine.PlaySounds(fileName);
    }
    
    void loadSound(const string &fileName) {
        aEngine.LoadSound(fileName);
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
    
    void setEventParam(const string &eventName, const string &paramName, float fValue){
        aEngine.SetEventParameter(eventName, paramName, fValue);
    }
    
    bool isEventPlaying(const string &eventName) {
        bool isPlaying = aEngine.IsEventPlaying(eventName);
        cout << eventName << " is playing= " << isPlaying << endl;
        return isPlaying;
    }
}