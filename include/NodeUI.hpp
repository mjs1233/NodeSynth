#pragma once
#include "UIComponent.hpp"
#include "Button.hpp"

class NodeUI {
	SDL_FRect rect;
	ui_state_t ui_state;
	ui_layer_t ui_layer;

	Button<Rect> remove_button;
	Button<Rect> add_node_button;

public:
	NodeUI() :
		rect(SDL_FRect{0,0,100,100}),
		ui_state(ui_state_t::active),
		ui_layer(ui_layer_t::layer_1) {

	}

	NodeUI(ui_state_t ui_state, ui_layer_t ui_layer) : 
		rect(SDL_FRect{ 0,0,100,100 }),
		ui_state(ui_state), 
		ui_layer(ui_layer) {
	}

	void update(Input& input) {


	}

	ui_state_t state() { return ui_state; }
	ui_layer_t layer() { return ui_layer; }

	uint32_t render(SDL_Renderer* renderer) {

		return 0;
	}

};