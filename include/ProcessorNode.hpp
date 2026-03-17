#pragma once
#include <cinttypes>
#include <variant>
#include <vector>
#include <concepts>
#include <memory>
#include <ranges>

#include <imgui.h>

#include "ConnectionData.hpp"


template <typename ...output_containers>
class ProcessNodeBase_t {

public:
	struct connection {
		uint32_t target_id;
		uint32_t connection_id;
		bool operator==(const connection& V_right) {
			return (V_right.connection_id == target_id && V_right.connection_id == connection_id);
		}

		bool operator!=(const connection& V_right) {
			return !(this == V_right);
		}
	};

protected:
	std::vector<connection> next_nodes;
	uint32_t reference_count;
	uint32_t id_value;
	ImVec2 window_position;

	uint32_t input_connection_count;

public:
	ProcessNodeBase_t() : id_value(0), reference_count(0),window_position(0,0), input_connection_count(0){

	}

	const std::vector<connection>& next() {

		return next_nodes;
	}

	void add_next(uint32_t target_id, uint32_t connection_id) {

		next_nodes.push_back(connection{target_id,connection_id});
	}

	void remove_next(const connection& id) {

		next_nodes = next_nodes
			| std::views::filter([&](connection v) { return v != id; })
			| std::ranges::to<std::vector<connection>>();

	}

	uint32_t ref_count() const {

		return reference_count;
	}

	void inc_ref() {

		reference_count++;
	}

	bool dec_ref() {

		reference_count--;
		return reference_count;
	}


	void set_id(uint32_t id) {

		id_value = id;
	}

	uint32_t id() const {

		return id_value;
	}

	uint32_t connection_count() const {

		return input_connection_count;
	}

	using data_variant = std::variant<output_containers...>;

	virtual void connection_data(ConnectionData& connection_data, uint32_t id) = 0;
	virtual void input(const data_variant& input,uint32_t input_id) = 0;
	virtual void process(data_variant& output) = 0;
	virtual ConnectionData update_ui(bool& connection_start, bool& connection_end) = 0;
	virtual ConnectionData output_data() = 0;
};


struct realtime_sample_output {

	std::vector<float> samples;
};

struct const_float_output {
	
	float data;
};

struct trigger_output {

	uint32_t trigger_offset;
};

using ProcessNodeBase = ProcessNodeBase_t<realtime_sample_output, const_float_output, trigger_output>;


template<typename T>
concept ProcessNodeTrait =
std::is_base_of<ProcessNodeBase, T>::value
&& std::is_constructible<T>::value
&& std::is_destructible<T>::value;