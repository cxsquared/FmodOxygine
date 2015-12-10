//
//  Game.hpp
//  FmodTest_macosx
//
//  Created by Cody Claborn on 12/9/15.
//  Copyright © 2015 oxygine. All rights reserved.
//

#pragma once
#include "oxygine-framework.h"
#include "InputTextHandler.hpp"
using namespace oxygine;

DECLARE_SMART(Game, spGame);
class Game: public Actor {
public:
    Game();
    
    void init();
private:
    void doUpdate(const UpdateState& us);
    
    spInputTextHandler _inputText;
};


