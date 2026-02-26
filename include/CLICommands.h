#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include "Appstate.hpp"


struct Command_load
{
	enum LoadTarget
	{
		script
	};
	LoadTarget target;
	std::filesystem::path target_path;

	void fill(const std::vector<std::string>& params)
	{
		for (size_t idx = 0; idx < params.size(); idx++)
		{
			//single argument param 

			if (idx + 1 >= params.size())
				return;

			//multiple argument param 
			if (params[idx] == "-t")
			{
				process_target(params[idx + 1]);
			}
			else if (params[idx] == "-p")
			{
				process_path(params[idx + 1]);
			}
 		}
	}

	void process_target(const std::string& arg)
	{
		if (arg == "src")
		{
			target = LoadTarget::script;
		}
	}
	void process_path(const std::string& arg)
	{
		target_path = arg;
	}

	/*void run(Appstate& state)
	{

	}*/
	
};

struct Command_wave 
{
	void run() {};

};