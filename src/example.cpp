#include "oxygine-framework.h"
#include <functional>
#include "SoundManager.h"
#include "Res.h"
#include "Game.h"

void example_preinit(){
}

void example_init() {
    Res::load();
    SoundManager::init();
    
#ifdef __APPLE__
    SoundManager::loadPlugin("libSoundtrackPlugin.dylib", "sounds/", nullptr);
#endif
    
    SoundManager::loadBank("sounds/Master Bank.bank");
    SoundManager::loadBank("sounds/Master Bank.strings.bank");
    
    SoundManager::loadEvent("event:/Music/MusicTrack");
    SoundManager::loadEvent("event:/Sfx/Attack");
    SoundManager::loadEvent("event:/Sfx/Footsteps");
    SoundManager::loadEvent("event:/Sfx/Buzz");
    SoundManager::loadEvent("event:/Sfx/Typing");
    SoundManager::loadEvent("event:/Ambience/Outside");
    
    SoundDefinition rain;
    rain.mSoundName = "sounds/rain.wav";
    rain.fDefaultVolumedB = 0.0f;
    rain.fMinDistance = 0.0f;
    rain.fMaxDistance = 100.0f;
    rain.bIs3d = false;
    rain.bIsLooping = true;
    rain.bIsStreaming = false;
    
    SoundManager::registerSound(rain, 0);
    
    spGame game = new Game;
    game->init();
    game->attachTo(getStage());
}

void example_update() {
    //SoundManager::update();
}

void example_destroy(){
    SoundManager::destroy();
    Res::free();
}