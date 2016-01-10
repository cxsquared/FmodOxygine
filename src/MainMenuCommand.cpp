#include "MainMenuCommand.h"
#include "ScreenState.h"

string MainMenuCommand::parseNoun(vector<string> options, ScreenState * state)
{
	state->switchStates(ScreenState::mainMenu);

	return "Switching to main menu.";
}
