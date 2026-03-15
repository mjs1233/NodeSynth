#pragma once
#include <concepts>
#include <vector>
#include <variant>
#include <cinttypes>
#include <string>


class ConfigData {

public:
	enum class data_type {
		_trigger,
		_float,
		_float01,
		_real_time_sample
	};

	ConfigData(uint32_t id, const std::string& name, data_type type) :
		id(id),
		name(name),
		type(type) {}
	
	uint32_t id;
	std::string name;
	data_type type;
};