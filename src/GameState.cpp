#include "GameState.h"
#include "Command.h"
#include "ScreenActor.h"

void GameState::enter(ScreenActor & screen)
{
	ScreenState::enter(screen);

	knowCommands["look"] = Command::look;
	knowCommands["Look"] = Command::look;
	knowCommands["l"] = Command::look;
	knowCommands["g"] = Command::go;
	knowCommands["go"] = Command::go;
	knowCommands["Go"] = Command::go;
	knowCommands["clear"] = Command::clear;
	knowCommands["Clear"] = Command::clear;
	knowCommands["Main"] = Command::mainMenu;
	knowCommands["main"] = Command::mainMenu;
	knowCommands["reset"] = Command::reset;
	knowCommands["Reset"] = Command::reset;
	knowCommands["South"] = Command::south;
	knowCommands["south"] = Command::south;
	knowCommands["s"] = Command::south;
	knowCommands["S"] = Command::south;
	knowCommands["North"] = Command::north;
	knowCommands["north"] = Command::north;
	knowCommands["n"] = Command::north;
	knowCommands["N"] = Command::north;
	knowCommands["West"] = Command::west;
	knowCommands["west"] = Command::west;
	knowCommands["W"] = Command::west;
	knowCommands["w"] = Command::west;
	knowCommands["East"] = Command::east;
	knowCommands["east"] = Command::east;
	knowCommands["E"] = Command::east;
	knowCommands["e"] = Command::east;
	knowCommands["Up"] = Command::up;
	knowCommands["up"] = Command::up;
	knowCommands["u"] = Command::up;
	knowCommands["U"] = Command::up;
	knowCommands["Down"] = Command::down;
	knowCommands["down"] = Command::down;
	knowCommands["d"] = Command::down;
	knowCommands["D"] = Command::down;
	knowCommands["Exits"] = Command::exits;
	knowCommands["exits"] = Command::exits;

	level = new Level();

	screen.addText(level->player->currentRoom->getDescritption());
	screen.addText(level->player->currentRoom->getExits());
}
