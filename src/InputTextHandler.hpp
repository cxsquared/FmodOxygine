//
//  InputTextHandler.hpp
//  FmodTest_macosx
//
//  Created by Cody Claborn on 12/8/15.
//  Copyright © 2015 oxygine. All rights reserved.
//

#pragma once

#include "InputText.h"
#include "oxygine-framework.h"

using namespace oxygine;
using namespace std;

DECLARE_SMART(TextWithBackground, spTextWithBackground);
class TextWithBackground: public ColorRectSprite {
public:
    
    Resources *gameResources;
    
    TextWithBackground(const string& defText,Resources &res) {
        gameResources = &res;
        
        text = new TextField;
        text->setInputEnabled(false);
        
        TextStyle style;
        style.color = Color::Black;
        style.hAlign = TextStyle::HALIGN_CENTER;
        style.vAlign = TextStyle::VALIGN_MIDDLE;
        style.multiline = true;
        style.font = gameResources->getResFont("main")->getFont();
        
        text->setStyle(style);
        text->setText(defText);
    }
    
    spTextField text;
    
    void sizeChanged(const Vector2& size) {
        text->setSize(size);
    }
};

DECLARE_SMART(InputTextHandler, spInputTextHandler);
class InputTextHandler : public Actor {
public:
    spInputText _input;
    spTextWithBackground _current;
    
    InputTextHandler(Resources &res) {
        _input = new InputText;
        
        _input->addEventListener(Event::COMPLETE, CLOSURE(this, &InputTextHandler::onComplete));
        
        spTextWithBackground t = new TextWithBackground("...", res);
        t->setSize(200, 6);
        t->setPosition(getStage()->getSize() /2 - _current->getSize() /2);
        t->attachTo(this);
        t->addEventListener(TouchEvent::CLICK, CLOSURE(this, &InputTextHandler::onClick));
    }
                       
    void onClick(Event* ev) {
        if (_current) {
            _current->setColor(Color::White);
        }
                           
        _current = safeSpCast<TextWithBackground>(ev->currentTarget);
        _input->start(_current->text);
        _current->setColor(Color::Red);
    }
                       
    void onComplete(Event* ev) {
        if(_current) {
            _current->setColor(Color::White);
        }
        
        _current = 0;
        InputText::stopAnyInput();
    }
                       
    ~InputTextHandler() {
        InputText::stopAnyInput();
    }
                       
    void clicked(string id) {
    }
};
