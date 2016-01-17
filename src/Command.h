#pragma once
#include <vector>
#include <string>
#include <sstream>

using namespace std;\

// Listing all classes so we can reference them
class ScreenState;
class InvalidCommand;
class Mail;
class Music;
class Look;
class Go;
class Clear;
class Quit;
class MainMenu;
class Reset; 
class South;
class North;
class West;
class East;
class Up;
class Down;
class Exits;
class Attack;

class Command {
public:
	virtual ~Command() {};
	virtual string parseNoun(vector<string> options, ScreenState* state) = 0;

	// List all commands as static variables
	static Command* invalid;
	static Command* mail;
	static Command* music;
	static Command* look;
	static Command* go;
	static Command* clear;
	static Command* quit;
	static Command* mainMenu;
	static Command* reset;
	static Command* south;
	static Command* north;
	static Command* east;
	static Command* west;
	static Command* up;
	static Command* down;
	static Command* exits;
	static Command* attack;

	//TODO: Allow for questions and responses to commands (mini states?)
};