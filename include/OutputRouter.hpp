#pragma once
#include <concepts>
#include <memory>

#include "Outputs.hpp"


struct RealtimeSample;
struct FloatParam;
struct Trigger;
template <typename ...output_containers> class ProcessNodeBase_t;
using ProcessNodeBase = ProcessNodeBase_t<RealtimeSample, FloatParam, Trigger>;
class InputPort;

class OutputRouter {
public:
	struct Connection {
		std::shared_ptr<ProcessNodeBase> next_ptr;
		uint32_t id;

		Connection(std::shared_ptr<ProcessNodeBase> next_ptr, uint32_t port_id) :
			next_ptr(next_ptr),
			id(port_id) {
		}
	};

private:
	type_id_t output_type_id;
	std::deque<Connection> output_connection;
public:

	OutputRouter() {

	}

	inline type_id_t get_output_type_id() const {

		return output_type_id;
	}

	template<typename T>
	constexpr void set_type_id() {

		output_type_id = get_type_id<T>();
	}


	void add_next(std::shared_ptr<ProcessNodeBase> next_input,uint32_t id) {

		output_connection.push_back(Connection(next_input, id));
	}

	std::deque<Connection>& next() {

		return output_connection;
	}

	template<OutputDataType output_type>
	void send(std::shared_ptr<output_type> data, uint32_t id);

	template<OutputDataType output_type>
	bool check_send(std::shared_ptr<output_type> data, uint32_t id);

	/*template<OutputDataType output_type>
	void send(std::shared_ptr<output_type> data, uint32_t id) {
		
		for (auto& connection : output_connection) {
			connection.next_ptr->input_router().recv(std::static_pointer_cast<OutputHeader>(data), id)
		}
	}

	template<OutputDataType output_type>
	bool check_send(std::shared_ptr<output_type> data, uint32_t id) {


		//send
		for (auto& connection : output_connection) {

			std::optional<InputPort> port = connection.next_ptr->input_router().get_port();

			if (!port.has_value())
				continue;

			if (port.value().type_id != output_type_id) 
				continue;

			connection.next_ptr->input_router().recv(std::static_pointer_cast<OutputHeader>(data), id)
		}
	}*/


};