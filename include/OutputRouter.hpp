#pragma once
#include <concepts>
#include <memory>

#include "InputRouter.hpp"
#include "ProcessorNode.hpp"
#include "Outputs.hpp"

class OutputRouter {
private:
	type_id_t type_id;
	std::shared_ptr<ProcessNodeBase> next_input;
public:

	template<typename T>
	OutputRouter() {

		type_id = get_type_id<T>();
	}

	void set_next(std::shared_ptr<ProcessNodeBase> next_input) {

		this->next_input = next_input;
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