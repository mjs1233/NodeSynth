#pragma once

#include <print>
#include <vector>
#include <functional>
#include <optional>
#include <memory>
#include <tuple>
#include <utility>
#include <concepts>
#include <type_traits>

#include "Outputs.hpp"
#include "ProcessorNode.hpp"

inline type_id_t get_next_type_id() {
	static type_id_t current_id = 1;
	return current_id++;
}

template <typename T>
inline type_id_t get_type_id() {
	static const type_id_t id = get_next_type_id();
	return id;
}

template<typename T>
concept PortIDEnumU32 = std::is_enum<T>::value && std::is_same<std::underlying_type_t<T>, uint32_t>::value;

template<typename T>
concept OutputDataType = std::is_base_of<OutputHeader, T>::value;

template <typename T>
struct OutputRecvFunctions {

	std::vector<std::function<void(std::shared_ptr<T>, uint32_t)>> functions;

	std::function<void(std::shared_ptr<T>, uint32_t)>& operator[](size_t idx) {

		return functions[idx];
	}
};

struct InputPort {

	type_id_t type_id;
	std::string port_name;
	uint32_t connected_node_id;
	bool allocated;

	InputPort() : 
		type_id(0),
		port_name(""),
		connected_node_id(0),
		allocated(false) {

	}

	InputPort(type_id_t type_id, std::string_view port_name) : 
		type_id(type_id),
		port_name(port_name),
		connected_node_id(0),
		allocated(false) {

	}
};

struct InputPortList {

	std::vector<InputPort> ports;

	InputPort& operator[](size_t idx) {

		return ports[idx];
	}
};

template<typename ...output_types>
class InputRouterBase {

private:
	//약간 희소 행렬 느낌?
	std::tuple<OutputRecvFunctions<output_types>...> recv_callback;
	InputPortList input_ports;
	size_t max_port_count;
	

public:

	InputRouterBase(size_t max_port_count = 0) : 
		max_port_count(max_port_count) {

		set_max_port_count(max_port_count);
	}

	void set_max_port_count(size_t max_port_count) {

		this->max_port_count = max_port_count;
		std::apply([max_port_count](auto&... args) {
			(args.functions.resize(max_port_count), ...);
			}, recv_callback);

		this->input_ports.ports.resize(max_port_count);
	}

	template<OutputDataType output_type, PortIDEnumU32 port_id_type>
	requires (std::same_as<output_types,output_type> || ... )
	void add_recv(const std::function<void(std::shared_ptr<OutputHeader>, uint32_t id)>& func, port_id_type port_id) {

		std::get<OutputRecvFunctions<output_type>>(recv_callback)[std::to_underlying(port_id)] = func;
	}

	template<OutputDataType output_type, PortIDEnumU32 port_id_type>
	requires (std::same_as<output_types, output_type> || ... )
	void recv(std::shared_ptr<OutputHeader> data, port_id_type port_id) {

		recv(data, std::to_underlying(port_id));
	}

	template<OutputDataType output_type>
		requires (std::same_as<output_types, output_type> || ...)
	void recv(std::shared_ptr<OutputHeader> data, uint32_t port_id) {

		if (port_id > max_port_count) {
			return;
		}

		if (!input_ports[port_id].allocated) {
			return;
		}

		if (std::get<OutputRecvFunctions<output_type>>(recv_callback)[port_id] == nullptr) {
			return;
		}

		std::get<OutputRecvFunctions<output_type>>(recv_callback)[port_id](std::static_pointer_cast<output_type>(data), port_id);
	}

	template<OutputDataType output_type>
	requires (std::same_as<output_types, output_type> || ... )
	void add_port(const std::string_view& name,uint32_t port_id) {

		type_id_t type_id = get_type_id<output_type>();

		input_ports[port_id] = InputPort(type_id,name);
	}


	std::optional<InputPort> get_port(uint32_t port_id) const {

		if (port_id >= max_port_count) {

			return std::nullopt;
		}

		if (input_ports.ports[port_id].allocated) {
			
			return std::nullopt;
		}

		return input_ports.ports[port_id];
	}

	bool add_prev(uint32_t node_id, uint32_t port_id) {

		if (!get_port(port_id).has_value())
			return false;

		InputPort& port = get_port_reference(port_id);
		port.allocated = true;
		port.connected_node_id = node_id;

		return true;
	}

	uint32_t get_ref_count() {

		// Consider caching the ref count for efficiency.

		uint32_t count = 0;

		for (uint32_t idx = 0; idx < max_port_count; idx++) {
			count = input_ports[idx].allocated;
		}

		return count;
	}

private:

	// this function does not check whether the port exists
	inline InputPort& get_port_reference(uint32_t port_id) {

		return input_ports.ports[port_id];
	}

};

using InputRouter = InputRouterBase<RealtimeSample, FloatParam, Trigger>;