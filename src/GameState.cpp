#include "GameState.h"
#include "Command.h"
#include "ScreenActor.h"

void GameState::enter(ScreenActor & screen)
{
	ScreenState::enter(screen);

	knowCommands["look"] = Command::look;
	knowCommands["Look"] = Command::look;
	knowCommands["l"] = Command::look;

	level = new Level();

	screen.addText(level->player->currentRoom.description);
	screen.addText(level->player->currentRoom.getExits());
}
