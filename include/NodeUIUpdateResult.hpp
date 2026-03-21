#pragma once
#include <deque>
#include <vector>
#include <cinttypes>
#include <optional>

struct NodeUIUpdateResult {

	std::optional<uint32_t> clicked_input_pin;
	bool output_pin_clicked;
};