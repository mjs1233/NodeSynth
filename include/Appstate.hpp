#pragma once
#include "InputMode.hpp"
#include "Input.hpp"
class Appstate
{
public:
	Input input;
	InputMode input_mode;
public:
	Appstate()
	{
		input = Input();
		input_mode = InputMode::Console;
	}

};