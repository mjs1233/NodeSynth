#pragma once
#include <optional>

#include "ProcessorNode.hpp"


namespace AudioProcessor {

	//AudioProcessorNode
	//exposition only
	class SinusoidalOscillator : public ProcessNodeBase {

	private:
		std::vector<float> sample_buffer;

		uint64_t total_sample_count;

	public:

		SinusoidalOscillator();

		using output_container = RealtimeSample;


		virtual void process() override;
		virtual NodeUIUpdateResult update_ui() override;
	private:

	};


}