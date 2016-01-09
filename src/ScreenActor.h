#pragma once
#include "oxygine-framework.h"
#include "Res.h"
#include "utils\stringUtils.h"

using namespace oxygine;
using namespace std;

class ScreenState;

DECLARE_SMART(ScreenActor, spDefaultScreenActor);
class ScreenActor : public Actor
{
public:
	ScreenActor(ScreenState* state);
	virtual ~ScreenActor();

	ScreenState* state;

	void addText(const string& line);
	void clearText();
protected:
	void doUpdate(const UpdateState& us);
	void onTextTweenDone(Event *event);
	virtual void createScreen();

private:
	spTextField _text;
};

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
