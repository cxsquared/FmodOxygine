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
}