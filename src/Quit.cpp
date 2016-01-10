#include "Quit.h"
#include "ScreenState.h"
#include "oxygine_include.h"

using namespace oxygine;

string Quit::parseNoun(vector<string> options, ScreenState * state)
{
	if (options.size() > 0 && options[0] == "yes") {
		core::requestQuit();
		return "Quiting game";
	}
	else {
		return "Are you sure? Type quit yes to really quit.";
	}
}
