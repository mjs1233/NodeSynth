#pragma once
#include <cinttypes>
#include <concepts>
#include <map>
#include <SDL3/SDL.h>

#include "UIComponent.hpp"

class Button
{
private:
	enum class render_state_t
	{
		idle,
		hover,
		click
	};

public:
	enum class event_type_t
	{
		click
	};

public:
	SDL_FRect rect;
	std::map<render_state_t, SDL_Color> color_map;
	render_state_t render_state;
	std::function<void(Button*, event_type_t event_type)> callback;
	ui_state_t ui_state;
	ui_layer_t ui_layer;
public:
	Button() :
		rect(SDL_FRect{0,}),
		render_state(render_state_t::idle),
		ui_state(ui_state_t::active),
		ui_layer(ui_layer_t::layer_1) {
	
		color_map[render_state_t::idle]  = SDL_Color{ 0xff,0x0,0x0,0xff };
		color_map[render_state_t::hover] = SDL_Color{ 0x00,0xff,0x0,0xff };
		color_map[render_state_t::click] = SDL_Color{ 0x00,0x0,0xff,0xff };
	
	}

	Button(SDL_FRect rect, ui_layer_t ui_layer, SDL_Color idle_color, SDL_Color hover_color, SDL_Color click_color) :
		rect(rect),
		render_state(render_state_t::idle),
		ui_state(ui_state_t::active),
		ui_layer(ui_layer) {
	

		color_map[render_state_t::idle]  = idle_color;
		color_map[render_state_t::hover] = hover_color;
		color_map[render_state_t::click] = click_color;
	
	}


	void update(Input& input) {

		if (input.mouse_x > rect.x && input.mouse_x < rect.x + rect.w
			&& input.mouse_y > rect.y && input.mouse_y < rect.y + rect.h) {
			if (input.left_mouse_button) {
				render_state = render_state_t::click;
				callback(this, event_type_t::click);
			}
			else {
				render_state = render_state_t::hover;
			}
		}
		else {
			render_state = render_state_t::idle;
		}

	}
	ui_state_t state() const { return ui_state; }
	ui_layer_t layer() { return ui_layer; }

	uint32_t render(SDL_Renderer* renderer) {

		SDL_Color rect_color = color_map[render_state];
		SDL_SetRenderDrawColor(renderer, rect_color.r, rect_color.g, rect_color.b, rect_color.a);
		SDL_RenderRect(renderer, &rect);
		return 0;
	}
};