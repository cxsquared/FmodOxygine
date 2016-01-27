#include "Sign.h"

Sign::Sign(string text)
{
	_signText = text;
	_name = "sign";
	_description = "readable sign";
}

string Sign::Interact(Level & level)
{
	return _signText;
}
