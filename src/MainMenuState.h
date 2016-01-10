#pragma once
#include "ScreenState.h"

DECLARE_SMART(MainMenuState, spMenuScreenState)
class MainMenuState : public ScreenState {
public:
	void enter(ScreenActor& screen) override;
};
