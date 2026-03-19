#include "Delay.hpp"
#include <utility>


namespace AudioProcessor {


	//exposition only
	Delay::Delay() : delay_sample_count(10), 
		delay_line(CircularQueue<float>(48000)), 
		ProcessNodeBase(ConnectionData(0, "output", ConnectionData::data_type::_real_time_sample, 0, 0)) {

		input_connection_count = 3;
		init_connection_handler();
	};


	//exposition only
	Delay::Delay(size_t delay_line_sample_count) : 
		delay_sample_count(10), 
		delay_line(CircularQueue<float>(delay_line_sample_count)),
		ProcessNodeBase(ConnectionData(0,"output",ConnectionData::data_type::_real_time_sample,0,0)) {
	

		input_connection_count = 3;
		init_connection_handler();
	}


	void Delay::init_connection_handler() {

		connection_handler.add_input(
			std::to_underlying(InputID_t::sample),
			ConnectionData{
				std::to_underlying(InputID_t::sample),
				"SAMPLE",
				ConnectionData::data_type::_real_time_sample,
				0,30
			});

		connection_handler.add_input(
			std::to_underlying(InputID_t::mix),
			ConnectionData{
				std::to_underlying(InputID_t::mix),
				"MIX",
				ConnectionData::data_type::_float01,
				0,60
			});

		connection_handler.add_input(
			std::to_underlying(InputID_t::delay_ms),
			ConnectionData{
				std::to_underlying(InputID_t::delay_ms),
				"DELAY",
				ConnectionData::data_type::_float,
				0,90
			});
	}


	//exposition only
	void Delay::input(const data_variant& input, uint32_t input_id_num) {

		InputID_t input_id = static_cast<InputID_t>(input_id_num);

		if (input_id == InputID_t::sample) {

			input_samples = std::get<realtime_sample_output>(input).samples;
		}
		else if (input_id == InputID_t::mix) {

			edit_mix_ratio(std::get<const_float_output>(input).data);
		}
		else if (input_id == InputID_t::delay_ms) {

			edit_delay_time(std::get<const_float_output>(input).data);
		}
	}

	//exposition only
	void Delay::process(data_variant& output) {

		while (delay_sample_count < delay_line.size()) {
			delay_line.pop();
		}

		for (size_t idx = 0; idx < input_samples.size(); idx++) {

			delay_line.push(input_samples[idx]);

			if (delay_sample_count <= delay_line.size()) {
				std::get<output_container>(output).samples[idx] = delay_line.front();
				delay_line.pop();
			}
		}
	}

	//exposition only
	ConnectionData Delay::update_ui(bool& connection_start,bool& connection_end) {
		
		ImGui::SetNextWindowSize({ 350,130 });
		std::string str_id = std::string() + "DELAY" + std::to_string(id());
		
		ImGui::Begin(str_id.c_str());
		window_position = ImGui::GetCursorScreenPos();
		ImGui::BeginGroup();

		ConnectionData conn;
		bool input_press_flag = false;

		if (ImGui::Button("SAMPLE")) {

			conn = connection_handler.get_input(std::to_underlying(InputID_t::sample)).value();
			conn.offset_x = ImGui::GetItemRectMin().x;
			conn.offset_y = ImGui::GetItemRectMin().y;

			input_press_flag = true;
		}
		if (ImGui::Button("MIX")) {

			conn = connection_handler.get_input(std::to_underlying(InputID_t::mix)).value();
			conn.offset_x = ImGui::GetItemRectMin().x;
			conn.offset_y = ImGui::GetItemRectMin().y;
			input_press_flag = true;
		}

		if (ImGui::Button("DELAY(ms)")) {

			conn = connection_handler.get_input(std::to_underlying(InputID_t::delay_ms)).value();
			conn.offset_x = ImGui::GetItemRectMin().x;
			conn.offset_y = ImGui::GetItemRectMin().y;
			input_press_flag = true;
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
		connection_end   = input_press_flag;

		ImGui::End();

		return conn;
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