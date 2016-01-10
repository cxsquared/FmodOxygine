#include "Clear.h"
#include "ScreenState.h"
#include "ScreenActor.h"

string Clear::parseNoun(vector<string> options, ScreenState * state)
{
	state->getScreen().clearText();

	return "Screen Cleared";
}
