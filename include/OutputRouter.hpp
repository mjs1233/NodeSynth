#pragma once
#include <concepts>
#include <memory>
#include <deque>

#include "Outputs.hpp"


struct RealtimeSample;
struct FloatParam;
struct Trigger;
struct VoidOutput;
template <typename ...output_containers> class ProcessNodeBase_t;
using ProcessNodeBase = ProcessNodeBase_t<RealtimeSample, FloatParam, Trigger,VoidOutput>;
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
	void send(std::unique_ptr<output_type> data);

	template<OutputDataType output_type>
	bool check_send(std::unique_ptr<output_type> data);


};