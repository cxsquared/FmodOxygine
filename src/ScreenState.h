#pragma once
#include <map>
#include <string>
#include <oxygine-framework.h>
#include "InputTextHandler.h"

using namespace oxygine;
using namespace std;
class ScreenActor;
class Command;

DECLARE_SMART(ScreenState, spDefaultScreenState)
class ScreenState : public Actor{
public:
	// TODO: List all posible screens
	
	//Methods
	ScreenState();
	virtual ~ScreenState();
	virtual ScreenState* update(ScreenActor& screen);
	virtual void enter(ScreenActor& screen);
	
	void switchStates(ScreenState* newState);
	Command* parseVerb(const string& command);

	// TODO: Implement Command Array
	map<string, Command*> knowCommands;
protected:
	ScreenState* stateToSwitchTo = NULL;
};