#include "ScreenActor.h"
#include "ScreenState.h"

ScreenActor::ScreenActor(ScreenState* startingState)
{
	createScreen();
	state = startingState;
	state->enter(*this);
}


ScreenActor::~ScreenActor()
{
	state = NULL;
}

void ScreenActor::doUpdate(const UpdateState & us)
{
	ScreenState* newState = state->update(*this);
	if (newState != NULL && !isTextTweening) {
		cout << "Switching to a new state!" << endl;
		// If state returns a new state then delete the old one
		// And then set the current state to new state
		state = newState;

		// Initializing state
		state->enter(*this);
	}

	// See if we need to show anymore text
	if (textQueue.size() > 0 && !isTextTweening) {
		addText(textQueue[0]);
		textQueue.erase(textQueue.begin());
	}
}

void ScreenActor::onTextTweenDone(Event * event)
{
	// TODO: Add skipping tween animation
	isTextTweening = false;
}

void ScreenActor::createScreen()
{
	// Createing a new Text Field and making it non interactable
	_text = new TextField;
	_text->setTouchEnabled(false);

	TextStyle style;
	style.color = Color::White;
	style.hAlign = TextStyle::HALIGN_LEFT;
	style.vAlign = TextStyle::VALIGN_TOP;
	style.multiline = true;
	style.font = Res::gameResources.getResFont("main")->getFont();

	_text->setStyle(style);
	_text->setText("");

	addChild(_text);
}

void ScreenActor::addText(const string& line)
{
	//TODO: Add text scrolling

	// Check if we are adding a line right now
	if (isTextTweening) {
		textQueue.push_back(line);
	}
	else {
		// Add a line then add new text
		_text->addTween(TweenText(line), 1000, 1, false)->setDoneCallback(CLOSURE(this, &ScreenActor::onTextTweenDone));
		isTextTweening = true;
	}
}

void ScreenActor::clearText()
{
	_text->setHtmlText("");
}
