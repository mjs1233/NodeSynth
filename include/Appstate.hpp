#pragma once
#include "InputMode.hpp"
#include "Input.hpp"
#include "UIManager.hpp"
class Appstate
{
public:
	Input input;
	InputMode input_mode;
	UIManager ui;
public:
	Appstate()
	{
		input = Input();
		input_mode = InputMode::Console;
		ui = UIManager();
	}

};