#include "Delay.hpp"


namespace AudioProcessor {

	Delay::Delay(size_t id, UIHandler& ui) : id(id), delay_sample_count(10) {

		ui_id = ui.add<NodeUI>(2,ui_state_t::active, ui_layer_t::layer_1);


	}

	void Delay::process(const std::vector<float>& in, std::vector<float>& out) {

		while (delay_sample_count < delay_line.size()) {
			delay_line.pop();
		}

		for (size_t idx = 0; idx < in.size(); idx++) {

			delay_line.push(in[idx]);

			if (delay_sample_count <= delay_line.size()) {
				out[idx] = delay_line.front();
				delay_line.pop();
			}
		}
	}

	size_t Delay::config_count() {
		return 2;
	}

	std::optional<ConfigData> Delay::config_data(const uint32_t config_id) {

		switch (config_id) {
		case 0:
			return ConfigData{ 0,"delay(ms)",ConfigData::data_type::_int };
		case 1:
			return ConfigData{ 1,"mix",ConfigData::data_type::_float01 };
		}
		return std::nullopt;
	}


	void Delay::update(const uint32_t config_id, const std::variant<int32_t, bool, float>& value) {

		switch (config_id) {
		case 0:
			edit_delay_time(std::get<int32_t>(value));
			break;
		case 1:
			edit_mix_ratio(std::get<float>(value));
		default:
			break;
		}

	}

	void Delay::edit_delay_time(uint32_t time/*ms*/) {

		if (time < max_delay_time_ms) {
			delay_sample_count = static_cast<uint32_t>(static_cast<float>(time) / 1000.f * 48000.f);
		}
	}

	void Delay::edit_mix_ratio(float ratio) {

		mix = ratio;
	}

	std::vector<uint32_t>& Delay::next() {

		return next_node;
	}
}