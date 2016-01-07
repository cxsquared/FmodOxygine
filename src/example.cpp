#include "oxygine-framework.h"
#include <functional>
#include "SoundManager.hpp"
#include "Res.hpp"
#include "Game.hpp"

void example_preinit(){
}

void example_init() {
    Res::load();
    SoundManager::init();
    SoundManager::loadBank("sounds/Master Bank.bank");
    SoundManager::loadBank("sounds/Master Bank.strings.bank");
    
    SoundManager::loadEvent("event:/Music/MusicTrack");
    SoundManager::loadEvent("event:/Sfx/Buzz");
    
    SoundManager::loadSound("sounds/rain.wav");
    
    spGame game = new Game;
    game->init();
    game->attachTo(getStage());
}

void example_update() {
    SoundManager::update();
}

void example_destroy(){
    SoundManager::destroy();
    Res::free();
}