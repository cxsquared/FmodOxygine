#pragma once
#include <map>
#include <string>

class ScreenActor;

class ScreenState {
public:
	// TODO: List all posible screens
	
	//Methods
	virtual ~ScreenState();
	virtual ScreenState* update(ScreenActor& screen);
	virtual void enter(ScreenActor& screen);

	// TODO: Implement Command Array
};