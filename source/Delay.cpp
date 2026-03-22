#include "Delay.hpp"
#include <utility>
#include <string>
#include "Debug.hpp"

namespace AudioProcessor {


	//exposition only
	Delay::Delay() : 
		delay_sample_count(10),
		delay_line(CircularQueue<float>(48000)),
		ProcessNodeBase(InputRouter(3), OutputRouter()) {

		output.set_type_id<RealtimeSample>();
		init_ports();
	};

	//exposition only
	Delay::Delay(size_t delay_line_sample_count) :
		delay_sample_count(10),
		delay_line(CircularQueue<float>(delay_line_sample_count)),
		ProcessNodeBase(InputRouter(3), OutputRouter()) {

		output.set_type_id<RealtimeSample>();
		init_ports();
	}

	void Delay::init_ports() {

		input.add_port<RealtimeSample>("sample", std::to_underlying(InputTypes::sample));
		input.add_port<FloatParam>("mix", std::to_underlying(InputTypes::mix));
		input.add_port<FloatParam>("delay", std::to_underlying(InputTypes::delay_ms));
	}

	//exposition only
	void Delay::process() {

		LOG(std::println("delay {}",id()))

		std::shared_ptr<RealtimeSample> output = std::make_shared<RealtimeSample>();
		output->samples.resize(sample_buffer.size());

		while (delay_sample_count < delay_line.size()) {
			delay_line.pop();
		}

		for (size_t idx = 0; idx < sample_buffer.size(); idx++) {

			delay_line.push(sample_buffer[idx]);

			if (delay_sample_count <= delay_line.size()) {
				output->samples[idx] = delay_line.front();
				delay_line.pop();
			}
		}

		output_router().check_send(output);
	}

	//exposition only
	NodeUIUpdateResult Delay::update_ui() {

		ImGui::SetNextWindowSize({ 350,130 });
		std::string str_id = std::string() + "DELAY" + std::to_string(id());

		ImGui::Begin(str_id.c_str());
		window_position = ImGui::GetCursorScreenPos();
		ImGui::BeginGroup();

		NodeUIUpdateResult result{};

		if (ImGui::Button("SAMPLE")) {

			result.clicked_input_pin = std::to_underlying(InputTypes::sample);
		}
		if (ImGui::Button("MIX")) {

			result.clicked_input_pin = std::to_underlying(InputTypes::mix);
		}
		if (ImGui::Button("DELAY(ms)")) {

			result.clicked_input_pin = std::to_underlying(InputTypes::delay_ms);
		}


		float delay_ms = static_cast<float>(delay_sample_count) / 48000.f * 1000;
		ImGui::SetNextItemWidth(200);
		ImGui::SliderFloat("DELAY_SLIDER", &delay_ms, 0, 2);
		delay_sample_count = static_cast<uint32_t>(delay_ms / 1000.f * 48000.f);

		ImGui::EndGroup();

		ImGui::SameLine();
		const char* right_label = "OUT";
		float right_width = ImGui::CalcTextSize(right_label).x
			+ ImGui::GetStyle().FramePadding.x * 2;

		float space = ImGui::GetContentRegionAvail().x;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + space - right_width);

		result.output_pin_clicked = ImGui::Button("OUT");

		ImGui::End();

		return result;
	}

	void Delay::input_sample(std::shared_ptr<RealtimeSample> samples) {

		sample_buffer = samples->samples;
	}

	void Delay::edit_delay_time(std::shared_ptr<FloatParam> time/*ms*/) {

		if (time->data < max_delay_time_ms) {
			delay_sample_count = static_cast<uint32_t>(static_cast<float>(time->data) / 1000.f * 48000.f);
		}
	}

	void Delay::edit_mix_ratio(std::shared_ptr<FloatParam> ratio) {

		mix = ratio->data;
	}

}