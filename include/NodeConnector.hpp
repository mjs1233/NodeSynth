#pragma once
#include <array>
#include <vector>
#include <concepts>
#include <cinttypes>


class NodeConnector
{

public:
	std::vector<size_t> input_node;
	std::vector<size_t> output_node;

	NodeConnector() {}

	NodeConnector(std::initializer_list<std::initializer_list<size_t>> list)
	{
		auto list_iter = list.begin();

		if (list.size() == 1)
		{
			input_node.assign(list_iter->begin(), list_iter->end());
		}

		input_node.assign(list_iter->begin(), list_iter->end());
		list_iter++;
		output_node.assign(list_iter->begin(), list_iter->end());
	}
};