#pragma once
#include <cinttypes>
#include <variant>
#include <vector>
#include <concepts>
#include <memory>
#include <ranges>

#include <imgui.h>

#include "ConnectionData.hpp"
#include "ConnectionHandler.hpp"
class ConnectionHandler;
struct connection;
class ConnectionData;

template <typename ...output_containers>
class ProcessNodeBase_t {


protected:
	std::vector<connection> next_nodes;
	ConnectionHandler connection_handler;

	uint32_t reference_count;
	uint32_t id_value;
	ImVec2 window_position;

	uint32_t input_connection_count;

public:
	explicit ProcessNodeBase_t(ConnectionData output_data) : 
		connection_handler(ConnectionHandler(output_data)),
		id_value(0), 
		reference_count(0), 
		window_position(0, 0), 
		input_connection_count(0) {

	}

	ConnectionHandler& connection() {
		return connection_handler;
	}


	void set_id(uint32_t id) {

		id_value = id;
	}

	uint32_t id() const {

		return id_value;
	}

	using data_variant = std::variant<output_containers...>;

	virtual void input(const data_variant& input,uint32_t input_id) = 0;
	virtual void process(data_variant& output) = 0;
	virtual ConnectionData update_ui(bool& connection_start, bool& connection_end) = 0;
};

using type_id_t = uint32_t;

inline uint32_t get_next_type_id() {
	static type_id_t current_id = 0;
	return current_id++;
}

template <typename T>
inline type_id_t get_type_id() {
	static const type_id_t id = get_next_type_id();
	return id;
}

struct OutputHeader {

	type_id_t type_id;
	OutputHeader(type_id_t type_id) : type_id(type_id) {

	}

	virtual ~OutputHeader() = default;
}; 


struct RealtimeSample : public OutputHeader {
	std::vector<float> samples;

	RealtimeSample(type_id_t id) : OutputHeader(id) {}
};

struct FloatParam : public OutputHeader {
	float data;

	FloatParam(type_id_t id) : OutputHeader(id), data(0) {}
};

struct Trigger : public OutputHeader {
	uint32_t trigger_offset;

	Trigger(type_id_t id) : OutputHeader(id),trigger_offset(0) {}
};

using ProcessNodeBase = ProcessNodeBase_t<RealtimeSample, FloatParam, Trigger>;


template<typename T>
concept ProcessNodeTrait =
std::is_base_of<ProcessNodeBase, T>::value
&& std::is_constructible<T>::value
&& std::is_destructible<T>::value;