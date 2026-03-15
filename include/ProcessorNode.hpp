#pragma once
#include <cinttypes>
#include <variant>
#include <vector>
#include <concepts>
#include <memory>
#include <ranges>
#include "ConfigData.hpp"


template <typename ...output_containers>
class ProcessNodeBase_t {

protected:
	std::vector<uint32_t> next_nodes;
	uint32_t reference_count;
	uint32_t id_value;

public:
	ProcessNodeBase_t() : id_value(0), reference_count(0) {

	}

	const std::vector<uint32_t>& next() {

		return next_nodes;
	}

	void add_next(uint32_t id) {

		next_nodes.push_back(id);
	}

	void remove_next(uint32_t id) {

		next_nodes = next_nodes
			| std::views::filter([&](uint32_t v) { return v != id; })
			| std::ranges::to<std::vector<uint32_t>>();

	}

	uint32_t ref_count() const {

		return reference_count;
	}

	void add_ref() {

		reference_count++;
	}

	bool dec_ref() {

		reference_count--;
		return reference_count;
	}


	void set_id(uint32_t id) {

		id_value = id;
	}

	uint32_t id() {

		return id_value;
	}

	using data_variant = std::variant<output_containers...>;

	virtual void input_connection_data(ConfigData& config_data) = 0;
	virtual void input(const data_variant& input,uint32_t input_id) = 0;
	virtual void process(data_variant& output) = 0;
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