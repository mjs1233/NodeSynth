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

		BufferPool<float>::id_type input_id;



	public:

		Delay();

		Delay(size_t delay_line_sample_count);

		using output_container = RealtimeSample;


		virtual void process(PlayContext& context) override;
		virtual NodeUIUpdateResult update_ui() override;
	private:
		void init_ports();
		void input_sample(std::shared_ptr<OutputHeader> samples);
		void edit_delay_time(std::shared_ptr<OutputHeader> input/*ms*/);
		void edit_mix_ratio(std::shared_ptr<OutputHeader> ratio);

	};               


}