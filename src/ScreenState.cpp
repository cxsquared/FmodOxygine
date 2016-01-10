#include "ScreenState.h"
#include "ScreenActor.h"
#include "Command.h"
#include <iostream>
#include "MainMenuState.h"
#include "GameState.h"

ScreenState* ScreenState::mainMenu = new MainMenuState();
ScreenState* ScreenState::gameState = new GameState();

ScreenState::ScreenState()
{
}

ScreenState::~ScreenState() {
}

ScreenState* ScreenState::update(ScreenActor& screen)
{
	return stateToSwitchTo;
}

void ScreenState::enter(ScreenActor& screen)
{
	// Reset variables add this actor to the screen
	_screen = &screen;
	if (this->getParent() != &screen) {
		screen.addChild(this);
	}
	screen.clearText();
	this->stateToSwitchTo = NULL;

	knowCommands["Quit"] = Command::quit;
	knowCommands["quit"] = Command::quit;
}

void ScreenState::switchStates(ScreenState* newState)
{
	stateToSwitchTo = newState;
}

Command* ScreenState::parseVerb(const string& command) {
	auto w = knowCommands.find(command);
	if (w == knowCommands.end()) {
		// Command not found
		return Command::invalid;
	}

	return w->second;
}

void ScreenState::handleInput(const string & command, vector<string> options)
{
	Command* com = parseVerb(command);
	_screen->addText(com->parseNoun(options, this));
}

ScreenActor & ScreenState::getScreen()
{
	return *_screen;
}


