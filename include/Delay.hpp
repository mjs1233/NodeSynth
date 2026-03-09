#pragma once
#include <queue>

#include "UI.hpp"
#include "CircularQueue.hpp"
#include "ProcessorNode.hpp"

namespace AudioProcessor {

	//AudioProcessorNode
	class Delay {

	private:
		std::vector<uint32_t> next_node;
		size_t id;
		size_t ui_id;

		CircularQueue<float, 2000 * 48000> delay_line;

		const uint32_t max_delay_time_ms = 1000;
		size_t delay_sample_count = 500;
		std::queue<float> buffer;

	public:
		Delay(size_t id, UIHandler& ui) : id(id) {

			ui_id = ui.add<NodeUI>(ui_state_t::active, ui_layer_t::layer_1);
			
			
		}

		void process(const std::vector<float>& in, std::vector<float>& out) {

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

		void edit_delay_time(uint32_t time/*ms*/) {
			if (time < max_delay_time_ms) {
				delay_sample_count = static_cast<uint32_t>(static_cast<float>(time) / 1000.f * 48000.f);
			}
		}

		std::vector<uint32_t>& get_next() {
			return next_node;
		}
	};


}