#pragma once
#include <cinttypes>
#include <concepts>
#include <map>
#include <SDL3/SDL.h>

#include "UIComponent.hpp"
#include <Rect.hpp>

template <Drawable drawable_t>
class Button
{
private:
	enum class render_state_t {
		idle,
		hover,
		click
	};

public:
	enum class event_type_t {
		idle,
		hover,
		click
	};

public:
	SDL_FRect rect;
	std::map<render_state_t, SDL_Color> color_map;
	render_state_t render_state;
	std::function<void(Button*, event_type_t event_type)> callback;
	ui_state_t ui_state;
	ui_layer_t ui_layer;
	drawable_t drawable;
	uint32_t tag_value;

	float offset_x;
	float offset_y;

public:
	Button() :
		rect(SDL_FRect{0,}),
		render_state(render_state_t::idle),
		ui_state(ui_state_t::active),
		ui_layer(ui_layer_t::layer_1),
		tag_value(0),
		offset_x(0), offset_y(0) {
	
	}

	Button(SDL_FRect rect,
		ui_layer_t ui_layer,
		const drawable_t& drawable,
		uint32_t tag,
		SDL_Color idle_color, SDL_Color hover_color, SDL_Color click_color) :
		rect(rect),
		render_state(render_state_t::idle),
		ui_state(ui_state_t::active),
		ui_layer(ui_layer),
		tag_value(tag),
		offset_x(0), offset_y(0) {
		
		this->color_map[render_state_t::idle] = idle_color;
		this->color_map[render_state_t::hover] = hover_color;
		this->color_map[render_state_t::click] = click_color;
	
		this->drawable = drawable;
	}


	uint32_t update(Input& input) {

		if (input.mouse_x > rect.x && input.mouse_x < rect.x + rect.w
			&& input.mouse_y > rect.y && input.mouse_y < rect.y + rect.h) {
			if (input.left_mouse_button) {
				render_state = render_state_t::click;

				if(callback != nullptr)
					callback(this, event_type_t::click);

				return std::to_underlying(event_type_t::click);
			}
			else {
				render_state = render_state_t::hover;
				return std::to_underlying(event_type_t::hover);
			}
		}
		else {
			render_state = render_state_t::idle;
			return std::to_underlying(event_type_t::idle);
		}


	}
	ui_state_t state() const { return ui_state; }
	ui_layer_t layer() { return ui_layer; }

	uint32_t render(SDL_Renderer* renderer) {

		return 0;
	}

	void offset(float x, float y) {
		offset_x = x;
		offset_y = y;

		drawable.offset(rect.x + x, rect.y + y);
	}


	uint32_t tag() const {
		return tag_value;
	}
};

template<>
uint32_t Button<Rect>::render(SDL_Renderer* renderer);


	