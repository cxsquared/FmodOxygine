#include "ScreenState.h"
#include "ScreenActor.h"

ScreenState::~ScreenState() {
}

ScreenState* ScreenState::update(ScreenActor& screen)
{
	return NULL;
}

void ScreenState::enter(ScreenActor& screen)
{
	screen.addText("I've just made a really cool screen.");
}


