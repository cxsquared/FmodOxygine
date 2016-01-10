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

	level = new Level();

	screen.addText(level->player->currentRoom->description);
	screen.addText(level->player->currentRoom->getExits());
}
