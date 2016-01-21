#include "East.h"
#include "GameState.h"

string East::parseNoun(vector<string> options, ScreenState * state)
{
	GameState* game = dynamic_cast<GameState*>(state);

	if (game->level->player->move("e")) {
		// Player did move
		return game->level->player->currentRoom->enteringRoom();
	}

	return "You can't *sgo*e that way.";
}
