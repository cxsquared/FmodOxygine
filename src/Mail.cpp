#include "Mail.h"
#include "ScreenState.h"

string Mail::parseNoun(vector<string> options, ScreenState* state)
{
	if (options.size() > 0) {
		return "Mail takes no arguments.";
	}

	state->switchStates(new ScreenState());

	return "Switching States";
}
