#include "SinusoidalOscillator.hpp"
#include "Debug.hpp"
#include <string>

namespace AudioProcessor {
	SinusoidalOscillator::SinusoidalOscillator(): 
		total_sample_count(0),
		ProcessNodeBase(InputRouter(0),OutputRouter()) {

		output_router().set_type_id<RealtimeSample>();

	}

	void SinusoidalOscillator::process(PlayContext& context) {

		LOG(std::println("sin osc {}", id()))
		BufferPool<float>& buffer_pool = context.sample_pool;

		std::unique_ptr<RealtimeSample> p_output_container = std::make_unique<RealtimeSample>();
		BufferPool<float>::alloc_result_t exp_id = buffer_pool.alloc_block();

		//check alloc state
		if (!exp_id.has_value())
			return;

		BufferPool<float>::id_type output_id = exp_id.value();
		float* output = buffer_pool.ptr(output_id);

		for (size_t idx = 0; idx < context.buffer_frames; idx++) {

			output[idx] = std::sinf(2.f * 3.141592f * 440.f * (total_sample_count%48000) / 48000.f);

			total_sample_count++;
			
		}
		output_router().check_send(std::move(p_output_container));
	}

	NodeUIUpdateResult SinusoidalOscillator::update_ui() {

		ImGui::SetNextWindowSize({ 350,130 });
		std::string str_id = std::string() + "OSC {SIN} 440HZ" + std::to_string(id());

		ImGui::Begin(str_id.c_str());
		window_position = ImGui::GetCursorScreenPos();

		NodeUIUpdateResult result{};

		result.output_pin_clicked = ImGui::Button("OUT");

		ImGui::End();

		return result;
	}
}