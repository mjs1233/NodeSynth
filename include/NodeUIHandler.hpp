#pragma once
#include <vector>
#include <cinttypes>
#include "NodeUI.hpp"

//concept UIComponent
class NodeUIHandler {

	
	std::vector<NodeUI> nodeui;

public:
	NodeUIHandler() {

	}

	ui_layer_t layer() {
		return ui_layer_t::layer_1;
	}

	uint32_t render(SDL_Renderer* renderer,float x,float y) {
		for (auto& ui : nodeui) {

			if (ui.state() == ui_state_t::active)
				ui.render(renderer,0,0);
		}
	}

	uint32_t update(Input& input) {

		for (auto& ui : nodeui) {

			if(ui.state() == ui_state_t::active)
				ui.update(input);
		}
	}

	ui_state_t state() {
		return ui_state_t::active;
	}

	uint32_t tag() {
		return 0;
	}

};