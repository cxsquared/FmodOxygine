#include "Look.h"
#include "GameState.h"
#include "Item.h"

string Look::parseNoun(vector<string> options, ScreenState * state)
{
	GameState* game = dynamic_cast<GameState*>(state);
	
	if (options.size() > 0) {
		if (options[0] == "at" && options.size() > 1) {
			for (auto item : game->level->player->currentRoom->items) {
				if (item->getName() == options[1]) {
					return item->Interact(*game->level);
				}
			}
		}
		else {
			for (auto item : game->level->player->currentRoom->items) {
				if (item->getName() == options[0]) {
					return item->Interact(*game->level);
				}
			}
		}

		return "There isn't anything like that in the room.";
	}

	return game->level->player->currentRoom->getDescritption();
}
