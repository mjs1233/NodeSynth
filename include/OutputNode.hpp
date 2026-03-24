#pragma once
#include <optional>

#include "ProcessorNode.hpp"
#include "BufferPool.hpp"


namespace AudioProcessor {

	//AudioProcessorNode
	//exposition only
	class OutputNode : public ProcessNodeBase {

	private:

		bool input_buffer_filled;
		BufferPool<float>::id_type input_buffer_id;
		uint64_t total_sample_count;

	public:
		OutputNode();

		using output_container = VoidOutput;


		virtual void process(PlayContext& context) override;
		virtual NodeUIUpdateResult update_ui() override;

	private:
		void input_sample(std::shared_ptr<RealtimeSample> samples);
	};


}