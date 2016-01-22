#pragma once
#include "oxygine-framework.h"
#include "Res.h"
#include "utils/stringUtils.h"
#include "core/log.h"

using namespace oxygine;
using namespace std;

class ScreenState;

// Class that controls how everything is shown on the screen
// Can make a child class of this if you need custom visuals on the screen
DECLARE_SMART(ScreenActor, spDefaultScreenActor);
class ScreenActor : public Actor
{
public:
	ScreenActor(ScreenState* state);
	virtual ~ScreenActor();

	ScreenState* state;

	void addText(const string& line);
	void clearText();
	virtual void init();
protected:
	void doUpdate(const UpdateState& us);
	void onTextTweenDone(Event *event);
	virtual void createScreen();
	bool isTextTweening = false;

private:
	spTextField _text;
	spSlidingActor _sliding;
	vector<string> textQueue;
	int _numberOfLines;
};

// Class that controls how the text shows up on the screen
class TweenText {
public:
	typedef TextField type;

	TweenText(const string& text) {
		//convert utf8 string to wide string
        colorCommands(text);
		_text = utf8tows(text.c_str());
	}

	void init(TextField& actor) {
		_lastText = actor.getText();
	}

	void update(TextField& actor, float p, const UpdateState& us) {
		// TODO: having multiple commands colored
        // TODO: Make it less glitchy
		int v = lerp<int>(0, (int)_text.size(), p);
        wstring res;
        wstring startCommand = L"<div c = '0x00E5FFFF'>";
        wstring endCommand = L"</div>";
        
        if (v > _startCommandIndex - 1 && v < _startCommandIndex + _commandLength && _commandLength > 0){
            res = utf8tows(_lastText.c_str()) + _text.substr(0, _startCommandIndex) + startCommand + _text.substr(_startCommandIndex+2, v-_startCommandIndex) + endCommand + L"<div c = '0x00000000'>" + _text.substr(v, _text.size()) + L"</div><br/>";
        } else if (v > _startCommandIndex + _commandLength - 1 && _commandLength > 0) {
            res = utf8tows(_lastText.c_str()) + _text.substr(0, _startCommandIndex) + startCommand + _text.substr(_startCommandIndex+2, _commandLength) + endCommand + _text.substr(_startCommandIndex + 2 + _commandLength+2, v) + L"<div c = '0x00000000'>" + _text.substr(v, _text.size()) + L"</div><br/>";
        } else {
            res = utf8tows(_lastText.c_str()) + _text.substr(0, v) + L"<div c = '0x00000000'>" + _text.substr(v, _text.size()) + L"</div><br/>";
        }

		// convert back to utf8 string
		string t = ws2utf8(res.c_str());
		actor.setHtmlText(t);
	}
    
    void colorCommands(string text) {
        _startCommandIndex = text.find("*s");
        _commandLength = text.find("*e") - _startCommandIndex - 2;
        
        if (_commandLength == string::npos || _startCommandIndex == string::npos){
            _startCommandIndex = 0;
            _commandLength = 0;
        }
        
        for (string::size_type i = 0; i < text.size(); ++i) {
            if (string() + text[i] + text[i+1] == "*s"){
                break;
            }
            
            //log::messageln("Looking for spaces index %d: letter %c", i, text[i]);
        }
        
        //log::messageln("Start command %d. Command length %d", _startCommandIndex, _commandLength);
        
    }

private:
	wstring _text;
	string _lastText;
    size_t _startCommandIndex = 0;
    size_t _commandLength = 0;
};

