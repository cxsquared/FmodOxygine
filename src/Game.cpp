//
//  Game.cpp
//  FmodTest_macosx
//
//  Created by Cody Claborn on 12/9/15.
//  Copyright © 2015 oxygine. All rights reserved.
//

#include "Game.hpp"
#include "SoundManager.hpp"

Game::Game(){
}

void Game::init() {
    SoundManager::playEvent("event:/Music/MusicTrack");
    
    setSize(getStage()->getSize());
    
    _inputText = new InputTextHandler;
    _inputText->attachTo(this);
    _inputText->setPosition(getStage()->getSize()/2);
}

void Game::doUpdate(const UpdateState& us) {
}
