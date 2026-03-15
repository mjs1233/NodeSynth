#include "Delay.hpp"


namespace AudioProcessor {


	//exposition only
	Delay::Delay() : delay_sample_count(10), delay_line(CircularQueue<float>(48000)) {



	}

	//exposition only
	void Delay::input_connection_data(ConfigData& config_data) {


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