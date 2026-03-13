#pragma once
#include <queue>
#include <optional>

#include "CircularQueue.hpp"
#include "ProcessorNode.hpp"

namespace AudioProcessor {

	//AudioProcessorNode
	//exposition only
	class Delay {

	private:
		std::vector<uint32_t> next_node;
		size_t id;
		size_t ui_id;

		CircularQueue<float, 2000 * 48000> delay_line;

		const uint32_t max_delay_time_ms = 1000;
		size_t delay_sample_count;

		float mix = 0;

	public:
		Delay();

		void process(const std::vector<float>& in, std::vector<float>& out);

		void update(const uint32_t config_id, const std::variant<int32_t, bool, float>& value);

		std::vector<uint32_t>& next();

	private:
		void edit_delay_time(uint32_t time/*ms*/);
		void edit_mix_ratio(float ratio);

	};


}