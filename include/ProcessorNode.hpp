#pragma once
#include <cinttypes>
#include <variant>
#include <vector>
#include <concepts>
#include <memory>
#include <ranges>

#include <imgui.h>

#include "InputRouter.hpp"
#include "OutputRouter.hpp"
#include "Outputs.hpp"
#include "NodeUIUpdateResult.hpp"
#include "PlayContext.h"

template <typename ...output_containers>
class ProcessNodeBase_t;

using ProcessNodeBase = ProcessNodeBase_t<RealtimeSample, FloatParam, Trigger, VoidOutput>;

template <typename ...output_containers>
class ProcessNodeBase_t {


protected:
	InputRouter input;
	OutputRouter output;


	uint32_t reference_count;
	uint32_t id_value;
	ImVec2 window_position;


public:
	explicit ProcessNodeBase_t(const InputRouter& input, const OutputRouter& output) : 
		id_value(0), 
		reference_count(0), 
		window_position(0, 0), 
		input(input),
		output(output) {

	}

	void set_id(uint32_t id) {

		id_value = id;
	}

	uint32_t id() const {

		return id_value;
	}

	InputRouter& input_router() {

		return input;
	}

	OutputRouter& output_router() {

		return output;
	}


	virtual void process(PlayContext& context) = 0;
	virtual NodeUIUpdateResult update_ui() = 0;
};

template<typename T>
concept ProcessNodeTrait =
std::is_base_of<ProcessNodeBase, T>::value
&& std::is_constructible<T>::value
&& std::is_destructible<T>::value;

template<OutputDataType output_type>
inline void OutputRouter::send(std::unique_ptr<output_type> data) {

	for (auto& connection : output_connection) {

		connection.next_ptr->input_router().recv<output_type>(std::make_unique<output_type>(data), connection.id);
	}
}

template<OutputDataType output_type>
inline bool OutputRouter::check_send(std::unique_ptr<output_type> data) {

	for (auto& connection : output_connection) {
		std::optional<InputPort> port = connection.next_ptr->input_router().get_port(connection.id);

		if (!port.has_value())
			continue;

		if (port.value().type_id != output_type_id)
			continue;

		connection.next_ptr->input_router().recv<output_type>(std::move(data),connection.id);
	}
	return true; 
}
