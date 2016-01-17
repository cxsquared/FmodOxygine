#include "Attack.h"
#include "GameState.h"
#include "Enemy.h"

string Attack::parseNoun(vector<string> options, ScreenState * state)
{
	GameState* game = dynamic_cast<GameState*>(state);

	if (options.size() > 0) {
		// C++11 fun way to loop through vectors
		for (auto &enemy : game->level->player->currentRoom->enemies) {
			if (enemy->getName() == options[0]) {
				return game->level->player->attack(*enemy);
			}
		}

		return "The player doesn't see anyone by that name to attack.";
	}

	return "You must specify a target to attack.";
}
