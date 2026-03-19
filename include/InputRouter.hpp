#pragma once

#include <print>
#include <vector>
#include <functional>
#include <memory>
#include <tuple>
#include <deque>
#include <utility>
#include <concepts>
#include <optional>
#include <map>
#include <type_traits>

#include "ConnectionData.hpp"
#include "ProcessorNode.hpp"

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

template<typename ...output_types>
class InputRouter {

private:
	std::tuple<OutputRecvFunctions<output_types>...> recv_callback;
	size_t max_target_ports;
public:

	InputRouter(size_t max_target_ports) : max_target_ports(max_target_ports) {


	}

	template<OutputDataType output_type, PortIDEnumU32 port_id_type>
	void add_recv(const std::function<void(std::shared_ptr<OutputHeader>, uint32_t id)>& func, port_id_type port_id) {

		std::get<OutputRecvFunctions<output_type>>(recv_callback)[std::to_underlying(port_id)] = func;
	}

	template<OutputDataType output_type, PortIDEnumU32 port_id_type>
	void recv(std::shared_ptr<OutputHeader> data, port_id_type port_id) {

		if (std::to_underlying(port_id) > max_target_ports) {

			LOG(std::print("port id underlying value out of range"))
				return;
		}

		if (std::get<OutputRecvFunctions<output_type>>(recv_callback)[std::to_underlying(port_id)] == nullptr) {

			LOG(std::print("recv callback type not registered"))
				return;
		}

		std::get<OutputRecvFunctions<output_type>>(recv_callback)[std::to_underlying(port_id)](std::static_pointer_cast<output_type>(data), std::to_underlying(port_id));
	}

};


