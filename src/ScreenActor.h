#pragma once
#include "oxygine-framework.h"
#include "Res.h"
#include "utils\stringUtils.h"

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
};

// Class that controls how the text shows up on the screen
class TweenText {
public:
	typedef TextField type;

	TweenText(const string& text) {
		//convert utf8 string to wide string
		_text = utf8tows(text.c_str());
	}

	void init(TextField& actor) {
		_lastText = actor.getText();
	}

	void update(TextField& actor, float p, const UpdateState& us) {
		// TODO: Figuer out how to color commands automatically
		int v = lerp<int>(0, (int)_text.size(), p);
		wstring res = utf8tows(_lastText.c_str()) + _text.substr(0, v) + L"<div c = '0x00000000'>" + _text.substr(v, _text.size()) + L"</div><br/>";

		// convert back to utf8 string
		string t = ws2utf8(res.c_str());
		actor.setHtmlText(t);
	}

private:
	wstring _text;
	string _lastText;
};

