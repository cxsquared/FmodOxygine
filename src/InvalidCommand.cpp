#include "InvalidCommand.h"
#include "ScreenState.h"

string InvalidCommand::parseNoun(vector<string> options, ScreenState* state)
{
	return "That command doesn't work.";
}
