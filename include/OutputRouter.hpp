#pragma once
#include <concepts>
#include <memory>
#include "ProcessorNode.hpp"

#include "Outputs.hpp"


class OutputRouter {
private:
	struct Connection {
		std::shared_ptr<ProcessNodeBase> next_ptr;
		uint32_t id;

		Connection(std::shared_ptr<ProcessNodeBase> next_ptr, uint32_t port_id) :
			next_ptr(next_ptr),
			id(port_id) {
		}
	};


	type_id_t type_id;
	std::vector<Connection> output_connection;
public:

	OutputRouter() {

	}

	template<typename T>
	void set_type_id() {

		type_id = get_type_id<T>();
	}

	void add_next(std::shared_ptr<ProcessNodeBase> next_input,uint32_t id) {

		output_connection.push_back(Connection(next_input, id));
	}

	template<OutputDataType output_type>
	void send(std::shared_ptr<output_type> data, uint32_t id) {

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