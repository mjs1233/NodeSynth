#pragma once
#include <concepts>
#include <vector>
#include <variant>
#include <cinttypes>
#include <string>


class ConnectionData {

public:
	enum class data_type {
		_default,
		_trigger,
		_float,
		_float01,
		_real_time_sample
	};

	ConnectionData() :

		id(0),
		name(""),
		type(data_type::_default),
		offset_x(0),
		offset_y(0) {

	}

	ConnectionData(uint32_t id, const std::string& name, data_type type,float offset_x,float offset_y) :
		id(id),
		name(name),
		type(type),
		offset_x(offset_x),
		offset_y(offset_y) {
	
	}
	
	uint32_t id;
	std::string name;
	data_type type;
	float offset_x;
	float offset_y;
};