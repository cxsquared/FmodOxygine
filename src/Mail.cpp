#include "Mail.h"
#include "ScreenState.h"

string Mail::parseNoun(string nouns, ScreenState state)
{
	if (nouns != "") {
		return "Mail takes no arguments.";
	}

	state.switchStates(new ScreenState());

	return "Switching States";
}
