#pragma once
#include "ScreenState.h"
#include "Level.h"

DECLARE_SMART(GameState, spGameScreenState)
class GameState : public ScreenState {
public:
	void enter(ScreenActor& screen);
	Level* level;
    
protected:
    void doUpdate(const UpdateState& us);
};
