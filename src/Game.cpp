//
//  Game.cpp
//  FmodTest_macosx
//
//  Created by Cody Claborn on 12/9/15.
//  Copyright © 2015 oxygine. All rights reserved.
//

#include "Game.h"
#include "SoundManager.h"
#include "ScreenState.h"
#include "ScreenActor.h"

Game::Game(){
}

void Game::init() {
    SoundManager::playEvent("event:/Music/MusicTrack");
    
    setSize(getStage()->getSize());
    
	// Create text input
    _inputText = new InputTextHandler;
    _inputText->attachTo(this);
	_inputText->setPosition(0, getStage()->getSize().y - 60);

	// Create screen
	_screen = new ScreenActor(new ScreenState());
	_screen->attachTo(this);
	_screen->setSize(getStage()->getSize().x, getStage()->getSize().y - 60);
	_screen->setPosition(0, 0);

	this->addEventListener(HandleInputEvent::EVENT, CLOSURE(this, &Game::handleInput));
}

void Game::handleInput(Event * ev)
{
	HandleInputEvent* event = static_cast<HandleInputEvent*>(ev);

	cout << "Parsing verb " << event->command << endl;

	_screen->state->parseVerb(event->command);
}

void Game::doUpdate(const UpdateState& us) {
}
