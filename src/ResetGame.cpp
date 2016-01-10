#include "ResetGame.h"
#include "ScreenState.h"

string ResetGame::parseNoun(vector<string> options, ScreenState * state)
{
	state->switchStates(ScreenState::gameState);

	return "Reseting Game...";
}
