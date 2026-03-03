#pragma once
#include <cinttypes>
#include <concepts>
#include <SDL3/SDL.h>

#include "UIComponent.hpp"

class Button
{
public:
	Button() {

	}

	Button(uint32_t x) {}


	void update(Input& input) {}
	ui_state state() { return ui_state::active; }
	uint32_t layer() { return 0; }
	uint32_t render(SDL_Renderer* renderer) {}
};