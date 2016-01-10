#include "Look.h"
#include "GameState.h"

string Look::parseNoun(vector<string> options, ScreenState * state)
{
	GameState* game = dynamic_cast<GameState*>(state);

	return game->level->player->currentRoom->description;
}
