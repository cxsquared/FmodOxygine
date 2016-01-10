#include "Mail.h"
#include "ScreenState.h"

string Mail::parseNoun(vector<string> options, ScreenState* state)
{
	if (options.size() > 0) {
		if (options[0] == "help" || options[0] == "-h") {
			return "The mail command is used to check your messages";
		}
		return "Mail takes no arguments.";
	}

	state->switchStates(ScreenState::gameState);

	return "Switching States";
}
