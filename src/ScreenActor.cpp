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
	if (newState != NULL) {
		cout << "Switching to a new state!" << endl;
		// If state returns a new state then delete the old one
		// And then set the current state to new state
		state = newState;

		// Initializing state
		state->enter(*this);
	}
}

void ScreenActor::createScreen()
{
	// Createing a new Text Field and making it non interactable
	_text = new TextField;
	_text->setTouchEnabled(false);

	TextStyle style;
	style.color = Color::Black;
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
	//TODO: Tween text to look really cool
	//TODO: Add text scrolling

	// Add a line then add new text
	_text->setText(_text->getText() + '\n' + line);
}

void ScreenActor::clearText()
{
	_text->setText("");
}
