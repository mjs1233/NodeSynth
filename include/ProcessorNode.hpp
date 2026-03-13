#pragma once
#include <cinttypes>
#include <variant>
#include <vector>
#include <concepts>
#include <array>


class ProcessNodeBase {

public:
	uint32_t ref_count = 0;

	virtual void process(const std::vector<float>& samples,size_t length) = 0;
};



template<typename T>
concept ProcessNodeTrait =
std::is_base_of<ProcessNodeBase, T>::value
&& std::is_constructible<T>::value
&& std::is_destructible<T>::value;


struct realtime_sample_output {

	std::vector<float> samples;
};
struct const_float_output {
	float data;

};
struct trigger_output {

	uint32_t trigger_offset;
};

class ProcessNode : public ProcessNodeBase {

public:

	using output_containter = realtime_sample_output;
	virtual void process(const std::vector<float>& samples, size_t length) {

		//do processing stuff
		for (size_t i = 0; i < length; i++) {
			//.....
		}
	}

	output_containter get_result() {
		return output_containter{};
	}
};

class Manager {


public:
	std::shared_ptr<ProcessNodeBase> callee_real_time;
	std::shared_ptr<ProcessNodeBase> callee_trigger;
public:

	template<ProcessNodeTrait T,typename...Args>
	requires std::is_same<typename T::output_tag,realtime_sample_output>::value
	void add(Args&& ...args) {



		callee_real_time = std::make_shared<T>(std::forward<Args>(args)...);
		
	}

};
