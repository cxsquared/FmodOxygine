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
#include "Res.hpp"

using namespace oxygine;
using namespace std;

DECLARE_SMART(TextWithBackground, spTextWithBackground);
class TextWithBackground: public ColorRectSprite {
public:
    
    TextWithBackground(const string& defText) {
        
        text = new TextField;
        text->setInputEnabled(false);
        
        TextStyle style;
        style.color = Color::Black;
        style.hAlign = TextStyle::HALIGN_CENTER;
        style.vAlign = TextStyle::VALIGN_MIDDLE;
        style.multiline = true;
        style.font = Res::gameResources.getResFont("main")->getFont();
        
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
    
    InputTextHandler() {
        _input = new InputText;
        
        _input->addEventListener(Event::COMPLETE, CLOSURE(this, &InputTextHandler::onComplete));
        
        spTextWithBackground t = new TextWithBackground("...");
        t->setSize(200, 6);
        t->setPosition(getWidth() / 2 - t->getWidth() /2, 0);
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
