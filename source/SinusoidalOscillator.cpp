#include "SinusoidalOscillator.hpp"
#include <string>

namespace AudioProcessor {
	SinusoidalOscillator::SinusoidalOscillator(): 
		total_sample_count(0),
		ProcessNodeBase(InputRouter(0),OutputRouter()) {

		output_router().set_type_id<RealtimeSample>();

	}

	void SinusoidalOscillator::process() {

		std::shared_ptr<RealtimeSample> output = std::make_shared<RealtimeSample>();
		output->samples.resize(sample_buffer.size());

		for (auto& out : output->samples) {

			out = std::sinf(2.f * 3.141592f * 440.f * (total_sample_count%48000) / 48000.f);

			total_sample_count++;
			
		}
		output_router().check_send(output);
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