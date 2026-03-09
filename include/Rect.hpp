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

public:
	Rect() : ui_layer(ui_layer_t::layer_1), rect(SDL_FRect{ 0, }), color(SDL_Color{0,}){}
	Rect(ui_layer_t layer, SDL_FRect rect,SDL_Color color) : ui_layer(layer), rect(rect), color(color) {}

	ui_layer_t layer() { return ui_layer; }

	uint32_t render(SDL_Renderer* renderer) {

		return 0;
	}
};