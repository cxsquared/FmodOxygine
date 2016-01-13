#include "Exits.h"
#include "GameState.h"

string Exits::parseNoun(vector<string> options, ScreenState * state)
{
	GameState* game = dynamic_cast<GameState*>(state);

	return game->level->player->currentRoom->getExits();
}
