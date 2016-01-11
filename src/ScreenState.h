#pragma once
#include <map>
#include <string>
#include "oxygine-framework.h"
#include <vector>
#include "InputTextHandler.h"

using namespace oxygine;
using namespace std;
class ScreenActor;
class Command;
class MainMenuState;
class GameState;

DECLARE_SMART(ScreenState, spDefaultScreenState)
class ScreenState : public Actor{
public:
	// TODO: List all posible screens
	static ScreenState* mainMenu;
	static ScreenState* gameState;

	//Methods
	ScreenState();
	virtual ~ScreenState();
	virtual ScreenState* update(ScreenActor& screen);
	virtual void enter(ScreenActor& screen);
	
	void switchStates(ScreenState* newState);
	Command* parseVerb(const string& command);
	void handleInput(const string& command, vector<string> options);
	ScreenActor& getScreen();

	// Holds all commands that the current state can use
	// Also holds all possible ways to type commands
	map<string, Command*> knowCommands;
protected:
	ScreenState* stateToSwitchTo = NULL;
	ScreenActor* _screen;
};