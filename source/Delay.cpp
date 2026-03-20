#include "Delay.hpp"
#include <utility>
#include <string>

namespace AudioProcessor {


	//exposition only
	Delay::Delay() : delay_sample_count(10), 
		delay_line(CircularQueue<float>(48000)), 
		ProcessNodeBase(InputRouter(3),OutputRouter()) {

		output.set_type_id<output_container>();
	};

	//exposition only
	Delay::Delay(size_t delay_line_sample_count) : 
		delay_sample_count(10), 
		delay_line(CircularQueue<float>(delay_line_sample_count)),
		ProcessNodeBase(InputRouter(3), OutputRouter()) {
	
	}

	//exposition only
	void Delay::process() {

		std::shared_ptr<RealtimeSample> output = std::make_shared<RealtimeSample>();
		output->samples.resize(input_samples.size());

		while (delay_sample_count < delay_line.size()) {
			delay_line.pop();
		}

		for (size_t idx = 0; idx < input_samples.size(); idx++) {

			delay_line.push(input_samples[idx]);

			if (delay_sample_count <= delay_line.size()) {
				output->samples[idx] = delay_line.front();
				delay_line.pop();
			}
		}
	}

	//exposition only
	void Delay::update_ui(bool& connection_start, ProcessNodeBase& start_node) {
		
		ImGui::SetNextWindowSize({ 350,130 });
		std::string str_id = std::string() + "DELAY" + std::to_string(id());
		
		ImGui::Begin(str_id.c_str());
		window_position = ImGui::GetCursorScreenPos();
		ImGui::BeginGroup();


		if (connection_start) {

			if (ImGui::Button("SAMPLE")) {

				start_node.output_router().add_next(
					std::shared_ptr<ProcessNodeBase>((ProcessNodeBase*)(this)),
					std::to_underlying(InputTypes::sample));
			}
			if (ImGui::Button("MIX")) {

				start_node.output_router().
					add_next(std::shared_ptr<ProcessNodeBase>((ProcessNodeBase*)(this)),
					std::to_underlying(InputTypes::mix));
			}
			if (ImGui::Button("DELAY(ms)")) {

				start_node.output_router().add_next(std::shared_ptr<ProcessNodeBase>((ProcessNodeBase*)(this)),
					std::to_underlying(InputTypes::delay_ms));
			}
		}
		else {

			ImGui::Button("SAMPLE");
			ImGui::Button("MIX");
			ImGui::Button("DELAY(ms)");
		}

		

		float delay_ms = static_cast<float>(delay_sample_count) / 48000.f * 1000;
		ImGui::SetNextItemWidth(200);
		ImGui::SliderFloat("DELAY_SLIDER", &delay_ms, 0, 2);
		ImGui::EndGroup();

		ImGui::SameLine();
		const char* right_label = "OUT";
		float right_width = ImGui::CalcTextSize(right_label).x
			+ ImGui::GetStyle().FramePadding.x * 2;

		float space = ImGui::GetContentRegionAvail().x;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + space - right_width);

		connection_start = ImGui::Button("OUT");

		ImGui::End();

		return;
	}

	//exposition only
	void Delay::edit_delay_time(uint32_t time/*ms*/) {

		if (time < max_delay_time_ms) {
			delay_sample_count = static_cast<uint32_t>(static_cast<float>(time) / 1000.f * 48000.f);
		}
	}

	//exposition only
	void Delay::edit_mix_ratio(float ratio) {

		mix = ratio;
	}
}