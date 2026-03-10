#pragma once
#include "UIComponent.hpp"
#include <print>
//drawable
class Rect
{
public:
	ui_layer_t ui_layer;
	SDL_FRect rect;
	SDL_Color color;
	float offset_x;
	float offset_y;

public:
	Rect() : ui_layer(ui_layer_t::layer_1), 
		rect(SDL_FRect{ 0, }), 
		color(SDL_Color{0,}),
		offset_x(0), offset_y(0) {

	}

	Rect(ui_layer_t layer, SDL_FRect rect,SDL_Color color) : 
		ui_layer(layer), rect(rect), color(color), 
		offset_x(0), offset_y(0) {

	}

	ui_layer_t layer() { 
		return ui_layer; 
	}

	void offset(float x, float y) {
		offset_x = x;
		offset_y = y;
	}
	

	uint32_t render(SDL_Renderer* renderer) {
		SDL_FRect offset_rect = rect;
		offset_rect.x += offset_x;
		offset_rect.y += offset_y;
		SDL_RenderFillRect(renderer, &offset_rect);
		return 0;
	}


};