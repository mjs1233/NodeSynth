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

#include "ConnectionData.hpp"
#include "ProcessorNode.hpp"

using type_id_t = uint32_t;

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

	Trigger(type_id_t id) : OutputHeader(id), trigger_offset(0) {}
};


inline static type_id_t get_next_type_id() {
	static type_id_t current_id = 1;
	return current_id++;
}

template <typename T>
inline static type_id_t get_type_id() {
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

template <OutputDataType T>
struct InputPort {

	type_id_t type_id;
	std::string port_name;
	bool allocated;

	InputPort() : 
		type_id(0),
		port_name(""),
		allocated(false) {

	}

	InputPort(type_id_t type_id, std::string port_name) : 
		type_id(type_id),
		port_name(port_name),
		allocated(true) {

	}
};

template <OutputDataType T>
struct InputPortList {

	std::vector<InputPort<T>> ports;

	InputPort<T>& operator[](size_t idx) {

		return ports[idx];
	}
};

template<typename ...output_types>
class InputRouterBase {

private:
	std::tuple<OutputRecvFunctions<output_types>...> recv_callback;//약간 희소 행렬 느낌?
	std::tuple<InputPortList<output_types>...> input_ports;
	size_t max_port_count;
	uint32_t next_port_id;
	

public:

	InputRouterBase(size_t max_port_count = 0) : 
		max_port_count(max_port_count),
		next_port_id(0) {

		
		std::apply([](auto recv_funcs){
			recv_funcs.functions.resize(max_port_count);
			}, recv_callback);
		

		std::apply([](auto port_list) {
			port_list.ports.resize(max_port_count);
			}, input_ports);
	}

	void set_max_port_count(size_t max_port_count) {

		this->max_port_count = max_port_count;
	}

	template<OutputDataType output_type, PortIDEnumU32 port_id_type>
	requires (std::same_as<output_types,output_type> || ... )
	void add_recv(const std::function<void(std::shared_ptr<OutputHeader>, uint32_t id)>& func, port_id_type port_id) {

		std::get<OutputRecvFunctions<output_type>>(recv_callback)[std::to_underlying(port_id)] = func;
	}

	template<OutputDataType output_type, PortIDEnumU32 port_id_type>
	requires (std::same_as<output_types, output_type> || ...)
	void recv(std::shared_ptr<OutputHeader> data, port_id_type port_id) {

		if (std::to_underlying(port_id) > max_port_count) {

			LOG(std::print("port id underlying value out of range"))
				return;
		}

		if (std::get<OutputRecvFunctions<output_type>>(recv_callback)[std::to_underlying(port_id)] == nullptr) {

			LOG(std::print("recv callback type not registered"))
				return;
		}

		std::get<OutputRecvFunctions<output_type>>(recv_callback)[std::to_underlying(port_id)](std::static_pointer_cast<output_type>(data), std::to_underlying(port_id));
	}

	template<OutputDataType output_type>
	requires (std::same_as<output_types, output_type> || ... )
	uint32_t add_port(const std::string_view& name) {

		uint32_t new_id = next_port_id;
		next_port_id++;

		type_id_t type_id = get_type_id<output_type>();

		std::get<InputPort<output_type>>(input_ports).ports[new_id] = InputPort<output_type>(type_id,name);

		return new_id;
	}

	template<OutputDataType output_type>
	std::optional<InputPort<output_type>> get_port(uint32_t id) {

		if (id >= max_port_count) {

			return std::nullopt;
		}

		if (std::get<InputPort<output_type>>(input_ports).port[id].allocated) {
			
			return std::nullopt;
		}

		return std::get<InputPort<output_type>>(input_ports).ports[id];
	}

};
using InputRouter = InputRouterBase<RealtimeSample, FloatParam, Trigger>;

class OutputRouter {
private:
	type_id_t type_id;
	std::shared_ptr<InputRouter> next_input;
public:

	template<typename T>
	OutputRouter() {

		type_id = get_type_id<T>();
	}

	void set_next(std::shared_ptr<InputRouter> next_input) {

		this->next_input = next_input;
	}

	template<OutputDataType output_type>
	void send(std::shared_ptr<output_type> data,uint32_t id) {
		
		next_input->recv(std::static_pointer_cast<OutputHeader>(data), id);
	}

	template<OutputDataType output_type>
	bool check_send(std::shared_ptr<output_type> data, uint32_t id) {

		std::optional<InputPort<output_type>> port =
			next_input->get_port<output_type>(id);

		//check port 

		if (!port.has_value()) {

			return false;
		}

		if (port.value().type_id != type_id) {

			return false;
		}

		//send
		next_input->recv(std::static_pointer_cast<OutputHeader>(data), id);
		
	}

	
};