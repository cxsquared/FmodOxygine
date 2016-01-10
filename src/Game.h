//
//  Game.hpp
//  FmodTest_macosx
//
//  Created by Cody Claborn on 12/9/15.
//  Copyright © 2015 oxygine. All rights reserved.
//

#pragma once
#include "oxygine-framework.h"
#include "InputTextHandler.h"
#include "ScreenActor.h"
using namespace oxygine;

DECLARE_SMART(Game, spGame);
class Game: public Actor {
public:
    Game();
    
    void init();
	void handleInput(Event* ev);
private:
    void doUpdate(const UpdateState& us);
    
    spInputTextHandler _inputText;
	spDefaultScreenActor _screen;
	spSprite _background;
};


