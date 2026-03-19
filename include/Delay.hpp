#pragma once
#include <queue>
#include <optional>

#include "CircularQueue.hpp"
#include "ProcessorNode.hpp"

namespace AudioProcessor {

	//AudioProcessorNode
	//exposition only
	class Delay : public ProcessNodeBase {

	private:
		enum class InputTypes : uint32_t {
			sample = 0,
			mix = 1,
			delay_ms = 2
		};

		CircularQueue<float> delay_line;

		const uint32_t max_delay_time_ms = 1000;
		size_t delay_sample_count;

		float mix = 0;
		std::vector<float> input_samples;



	public:
		Delay();

		Delay(size_t delay_line_sample_count);

		using output_container = RealtimeSample;


		virtual void input(const data_variant& input, uint32_t input_id_num) override;
		virtual void process(data_variant& output) override;
		virtual ConnectionData update_ui(bool& connection_start, bool& connection_end) override;
	private:
		void init_connection_handler();
		void edit_delay_time(uint32_t time/*ms*/);
		void edit_mix_ratio(float ratio);

	};               


}