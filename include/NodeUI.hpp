#pragma once
#include "UIComponent.hpp"
#include "UI.hpp"
#include "Button.hpp"

class NodeUI {
	SDL_FRect rect;
	ui_state_t ui_state;
	ui_layer_t ui_layer;

	Button<Rect> remove_button;
	Button<Rect> add_output_connection_button;
	std::vector<Button<Rect>> input_connection_button;

	std::vector<bool> input_connection_click;

	uint32_t tag_value;

	float offset_x;
	float offset_y;

public:
	NodeUI() :
		rect(SDL_FRect{0,0,100,100}),
		ui_state(ui_state_t::active),
		ui_layer(ui_layer_t::layer_1),
		tag_value(0){

	}

	NodeUI(size_t input_connection_count,uint32_t tag,ui_state_t ui_state, ui_layer_t ui_layer) : 
		rect(SDL_FRect{ 0,0,100,100 }),
		ui_state(ui_state), 
		ui_layer(ui_layer),
		tag_value(tag){

		input_connection_button.resize(input_connection_count);
		input_connection_click.resize(input_connection_count);

		float offset = 0;
		for (size_t idx = 0; idx < input_connection_count; idx++) {

			input_connection_button[idx] =
				Button<Rect>(
					SDL_FRect{ 0,10 + offset,30,20 },
					ui_layer_t::layer_1,
					Rect(
						ui_layer_t::layer_1,
						SDL_FRect{ 0,10 + offset,20,30 },
						SDL_Color{ 0xff,0x0,0x0,0xff }
					),
					idx,
					SDL_Color{ 0xff,0x0,0x0,0xff },
					SDL_Color{ 0x0,0xff,0x0,0xff },
					SDL_Color{ 0x0,0x0,0xff,0xff }
				);


			offset += 20.f;
		}
	}

	uint32_t update(Input& input) {

		for (auto& ui : input_connection_button) {
			Button<Rect>::event_type_t button_result = 
				static_cast<Button<Rect>::event_type_t>(ui.update(input));
			
			input_connection_click[ui.tag()] = false;
			if (button_result == Button<Rect>::event_type_t::click) {
				input_connection_click[ui.tag()] = true;
			}
		}
		return 0;

	}

	ui_state_t state() { 
		return ui_state; 
	}

	ui_layer_t layer() { 
		return ui_layer; 
	}


	void offset(float x, float y) {
		offset_x = x;
		offset_y = y;

		remove_button.offset(rect.x + x, rect.y + y);
		add_output_connection_button.offset(rect.x + x, rect.y + y);

		for (auto& ui : input_connection_button) {
			ui.offset(rect.x + x, rect.y + y);
		}
	}


	uint32_t render(SDL_Renderer* renderer) {

		SDL_FRect offset_rect = rect;
		offset_rect.x += offset_x;
		offset_rect.y += offset_y;

		SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xff);
		SDL_RenderFillRect(renderer, &offset_rect);


		for (auto& ui : input_connection_button) {
			ui.render(renderer);
		}
		return 0;
	}

	uint32_t tag() const {

		return tag_value;
	}

};