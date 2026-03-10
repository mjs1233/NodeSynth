#pragma once
#include <functional>

#include "UIComponent.hpp"
#include "Text.hpp"
#include "Button.hpp"

class DropDownUI {
	

	float offset_x;
	float offset_y;

	uint32_t tag;

	std::vector<Button<Text>> itemui;

	std::function<void(DropDownUI* self, size_t clicked_idx)> onclick_callback;

public:
	DropDownUI() : tag(0), offset_x(0), offset_y(0) {

	}

	DropDownUI(uint32_t tag, size_t item_count,
		const std::function<void(DropDownUI* self, size_t clicked_idx)>& callback) :
		tag(tag),
		itemui(std::vector<Button<Text>>(item_count)),
		onclick_callback(callback),
		offset_x(0), offset_y(0) {

	}


	ui_layer_t layer() {
		return ui_layer_t::layer_1;
	}

	uint32_t render(SDL_Renderer* renderer,float x,float y) {
		for (auto& ui : itemui) {

			if (ui.state() == ui_state_t::active)
				ui.render(renderer);
		}
	}

	uint32_t update(Input& input) {

		uint32_t idx = 0;
		for (auto& ui : itemui) {

			if (ui.state() == ui_state_t::active) {
				Button<Text>::event_type_t result = static_cast<Button<Text>::event_type_t>(ui.update(input));
				if (result == Button<Text>::event_type_t::click) {
					onclick_callback(this,idx);
					return idx;
				}
			}
			idx++;
		}
	}

	void offset(float x, float y) {
		offset_x = x;
		offset_y = y;
	}


	ui_state_t state() {
		return ui_state_t::active;
	}

	uint32_t tag() {
		return 0;
	}

};