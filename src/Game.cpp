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

	// Create background
	_background = new Sprite();
	_background->setResAnim(Res::gameResources.getResAnim("computerOutline"));
	_background->attachTo(this);
    
	// Create text input
    _inputText = new InputTextHandler;
    _inputText->attachTo(this);
	_inputText->setSize(getStage()->getSize().x - (40 * 2), 60);
	_inputText->setPosition(42, getStage()->getSize().y - 100);

	// Create screen
	_screen = new ScreenActor(ScreenState::mainMenu);
	_screen->attachTo(this);
	_screen->setSize(getStage()->getSize().x - (42 * 2), getStage()->getSize().y - 12 - _inputText->getSize().y - (getStage()->getSize().y - _inputText->getY()));
	_screen->setPosition(42, 40);
	_screen->init();

	this->addEventListener(HandleInputEvent::EVENT, CLOSURE(this, &Game::handleInput));
}

void Game::handleInput(Event * ev)
{
	HandleInputEvent* event = static_cast<HandleInputEvent*>(ev);

	cout << "Parsing verb " << event->command << endl;

	_screen->state->handleInput(event->command, event->options);
}

void Game::doUpdate(const UpdateState& us) {
}
