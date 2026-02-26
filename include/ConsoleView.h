#pragma once
#include <print>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <ranges>
#include "CLICommands.h"

class ConsoleView
{

public:
private:

public:
	ConsoleView()
	{
		
	}

	void update()
	{
		std::print(">> ");
		std::string input;
		std::getline(std::cin,input);

	}

	void processInput(std::string& input)
	{
		std::vector<std::string> blocks;
		std::string buffer;
		for (char ch : input)
		{
			if (ch == ' ' && buffer.size() != 0)
			{
				blocks.push_back(buffer);
				buffer.clear();
				continue;
			}
			
			buffer += ch;
		}
		if (buffer.size() != 0)
		{
			blocks.push_back(buffer);
		}

		if (blocks.size() == 1)
		{
			//...

			return;
		}

		//block.size() != 1

		if (blocks[0] == "load")
		{
			Command_load load = {};
			load.fill(std::vector<std::string>(blocks.begin() + 1, blocks.end()));

		}

	}

	void processLoad(const std::vector<std::string>& params)
	{
	}

};

