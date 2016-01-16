#include "ScreenActor.h"
#include "ScreenState.h"

ScreenActor::ScreenActor(ScreenState* startingState)
{
	_numberOfLines = 0;
	state = startingState;
}


ScreenActor::~ScreenActor()
{
	state = NULL;
}

void ScreenActor::doUpdate(const UpdateState & us)
{
	ScreenState* newState = state->update(*this);
	if (newState != NULL && !isTextTweening && textQueue.size() <= 0) {
		cout << "Switching to a new state!" << endl;
		// Remove old text from queue
		textQueue.clear();
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
    
    _sliding->snap();
}

void ScreenActor::onTextTweenDone(Event * event)
{
	// TODO: Add skipping tween animation
	isTextTweening = false;
}

void ScreenActor::createScreen()
{
	// Create slidding background
	spSlidingActor slidding = new SlidingActor();
	slidding->setSize(getSize());

	// Createing a new Text Field and making it non interactable
	_text = new TextField;
	_text->setTouchEnabled(true);

	TextStyle style;
	style.color = Color::White;
	style.hAlign = TextStyle::HALIGN_LEFT;
	style.vAlign = TextStyle::VALIGN_TOP;
	style.multiline = true;
	style.font = Res::gameResources.getResFont("main")->getFont();

	_text->setStyle(style);
	_text->setText("");
	_text->setWidth(this->getWidth());
	_text->setHeight(this->getHeight() * 2);

	slidding->setContent(_text);
	slidding->attachTo(this);

	_sliding = slidding;
	//addChild(_text);
}

void ScreenActor::addText(const string& line)
{
	//TODO: Prevent text from scrolling when there isn't enough
    //TODO: Clear the oldest text lines when we get so far

	// Check if we are adding a line right now
	if (isTextTweening) {
		textQueue.push_back(line);
	}
	else {
		// Add a line then add new text
		_numberOfLines++;
		if (_numberOfLines > 28) {
			_text->setPosition(_text->getPosition().x, _text->getPosition().y - 48);
		}
		else if (_numberOfLines > 53) {
			clearText();
		}
		_text->addTween(TweenText(line), 1000, 1, false)->setDoneCallback(CLOSURE(this, &ScreenActor::onTextTweenDone));
		isTextTweening = true;
	}
}

void ScreenActor::clearText()
{
	_text->setHtmlText("");
	_numberOfLines = 0;
	_text->setPosition(this->getPosition());
}

void ScreenActor::init()
{
	createScreen();
	state->enter(*this);
}
