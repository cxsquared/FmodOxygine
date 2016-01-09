#pragma once
#include "oxygine-framework.h"
#include "Res.h"
#include <string>

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
	virtual void createScreen();

private:
	spTextField _text;
};

