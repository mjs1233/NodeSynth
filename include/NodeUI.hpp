#pragma once
#include "UIComponent.hpp"
#include "Button.hpp"

class NodeUI {
	SDL_FRect rect;
	ui_state_t ui_state;
	ui_layer_t ui_layer;

	Button<Rect> remove_button;
	Button<Rect> add_node_button;


	NodeUI() {


	}

	void update(Input& input) {


	}

	ui_state_t state() const { return ui_state; }
	ui_layer_t layer() { return ui_layer; }

	uint32_t render(SDL_Renderer* renderer) {

		return 0;
	}

};