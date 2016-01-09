#include "ScreenState.h"
#include "ScreenActor.h"
#include "Command.h"
#include <iostream>

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
	screen.addChild(this);

	knowCommands["mail"] = Command::mail;
	knowCommands["Mail"] = Command::mail;
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


