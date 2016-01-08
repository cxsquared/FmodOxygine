//
//  Res.cpp
//  FmodTest_macosx
//
//  Created by Cody Claborn on 12/9/15.
//  Copyright © 2015 oxygine. All rights reserved.
//

#include "Res.h"

namespace Res {
    Resources gameResources;
    
    void load() {
        gameResources.loadXML("xmls/res.xml");
    }
    
    void free() {
        gameResources.free();
    }
}
