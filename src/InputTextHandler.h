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
#include "Res.h"
#include "SoundManager.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace oxygine;
using namespace std;

DECLARE_SMART(TextWithBackground, spTextWithBackground);
class TextWithBackground: public ColorRectSprite {
public:
    
    TextWithBackground(const string& defText) {
        
        text = new TextField;
        text->setTouchEnabled(false);
        
        TextStyle style;
        style.color = Color::White;
        style.hAlign = TextStyle::HALIGN_LEFT;
        style.vAlign = TextStyle::VALIGN_MIDDLE;
        style.multiline = true;
        style.font = Res::gameResources.getResFont("main")->getFont();
        
        text->setStyle(style);
        text->setText(defText);
        
        addChild(text);
        
        cout << "Text with background added" << endl;
    }
    
    spTextField text;
    
    void sizeChanged(const Vector2& size) {
        text->setSize(size);
    }
};

// Custom event class
class HandleInputEvent : public Event {
public:
	enum {EVENT = makefourcc('H', 'a', 'I', 'n') };

	string command;
	vector<string> options;

	HandleInputEvent(string command, vector<string> options) :Event(EVENT), command(command), options(options) {}
};

DECLARE_SMART(InputTextHandler, spInputTextHandler);
class InputTextHandler : public Actor {
public:
    spInputText _input;
    spTextWithBackground _current;
    
    int rainChannel = -1;
    
    InputTextHandler() {
        _input = new InputText;
        
        _input->addEventListener(Event::COMPLETE, CLOSURE(this, &InputTextHandler::onComplete));
        
        spTextWithBackground t = new TextWithBackground("Sound Command");
        t->setSize(getStage()->getSize().x, 60);
        //t->setPosition(getWidth() / 2 - t->getWidth() /2, 0);
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
        //cout << "Event complete called." << endl;
        if(_current) {
            _current->setColor(Color::White);
        }
        
        string curText = _current->text->getText();
        
        //cout << "Input text = " << curText << endl;
		
		// Start splitting string into an vector
        string buf; // Have a buffer string
        stringstream ss(curText); // Insert the string into a stream
        
        vector<string> tokens; // Create vector to hold our words
        
        while (ss >> buf) // TODO: Learn what's actually happening here
            tokens.push_back(buf);

		// Send event
		if (tokens.size() > 0) {
			string command = tokens[0];
			tokens.erase(tokens.begin());
			HandleInputEvent event(command, tokens);
			getParent()->dispatchEvent(&event);

			cout << "Event sent" << endl;
		}
        
		/*
        if (tokens.size() > 1) {
			// Old music commands
            if (tokens[0] == "set" && tokens.size() > 2) {
                if (tokens[1] == "music"){
                    SoundManager::setEventParam("event:/Music/MusicTrack", "Intensity", stof(tokens[2]));
                } else if (tokens[1] == "rain" && rainChannel >= 0) {
                    SoundManager::setChannelVolume(rainChannel, stof(tokens[2]));
                }
            } else if (tokens[0] == "play") {
                if (tokens[1] == "music" && SoundManager::isEventPlaying("event:/Music/MusicTrack") == false) {
                    SoundManager::playEvent("event:/Music/MusicTrack");
                } else if (tokens[1] == "rain") {
                    rainChannel = SoundManager::playSound("sounds/rain.wav");
                    cout << "Rain channel " << rainChannel << endl;
                }
            } else if (tokens[0] == "stop") {
                if (tokens[1] == "music") {
                    SoundManager::stopEvent("event:/Music/MusicTrack");
                } else if (tokens[1] == "rain" && rainChannel >= 0){
                    SoundManager::stopChannel(rainChannel);
                    rainChannel = -1;
                }
            }
			// end music commands
        }
        // spliting command into different strings
		*/

        _current = 0;
        InputText::stopAnyInput();
    }
                       
    ~InputTextHandler() {
        InputText::stopAnyInput();
    }
                       
    void clicked(string id) {
    }
};
