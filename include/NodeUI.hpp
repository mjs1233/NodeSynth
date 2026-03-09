#pragma once
#include "UIComponent.hpp"
#include "Button.hpp"

class NodeUI {
	SDL_FRect rect;
	ui_state_t ui_state;
	ui_layer_t ui_layer;

	Button<Rect> remove_button;
	Button<Rect> add_output_connection_button;
	std::vector<Button<Rect>> input_connection_button;

public:
	NodeUI() :
		rect(SDL_FRect{0,0,100,100}),
		ui_state(ui_state_t::active),
		ui_layer(ui_layer_t::layer_1) {

	}

	NodeUI(size_t input_connection_count,ui_state_t ui_state, ui_layer_t ui_layer) : 
		rect(SDL_FRect{ 0,0,100,100 }),
		ui_state(ui_state), 
		ui_layer(ui_layer) {

		input_connection_button.resize(input_connection_count);
			
		float offset = 0;
		for (size_t idx = 0; idx < input_connection_count; idx++) {

			input_connection_button[idx] = 
				Button<Rect>(
					SDL_FRect{ 0,10,30,20 }, 
					ui_layer_t::layer_1, 
					Rect(
						ui_layer_t::layer_1, 
						SDL_FRect{ 0,10 + offset,30,20 }, 
						SDL_Color{ 0xff,0x0,0x0,0xff }
					)
				);

			offset += 20.f;
		}
	}

	void update(Input& input) {

		for (auto& ui : input_connection_button) {
			ui.update(input);
		}

	}

	ui_state_t state() { return ui_state; }
	ui_layer_t layer() { return ui_layer; }

	uint32_t render(SDL_Renderer* renderer) {

		SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xff);
		SDL_RenderRect(renderer, &rect);

		for (auto& ui : input_connection_button) {
			ui.render(renderer);
		}
		return 0;
	}

};