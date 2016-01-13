#include "South.h"
#include "GameState.h"

string South::parseNoun(vector<string> options, ScreenState * state)
{
	GameState* game = dynamic_cast<GameState*>(state);

	if (game->level->player->move("s")) {
		// Player did move
		return game->level->player->currentRoom->enteringRoom();
	}

	return "You can't go that way.";
}
