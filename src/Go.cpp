#include "Go.h"
#include "GameState.h"

string Go::parseNoun(vector<string> options, ScreenState * state)
{
	GameState* game = dynamic_cast<GameState*>(state);
	
	if (options.size() > 0) {
		if (game->level->player->move(options[0])) {
			// Player did move
			return game->level->player->currentRoom->enteringRoom();
		}
		else {
			return "You can't *sgo*e that way...";
		}
	}
	else {
		return "You need to give a direction to *sgo*e. " + game->level->player->currentRoom->getExits();
	}
}
