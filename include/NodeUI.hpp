#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <concepts>
#include <print>
#include <functional>

#include "Widget.hpp"


class NodeUI : public Widget<NodeUI>
{
private:
	SDL_Color top_bar_color{60,60,60 ,0xFF};
	SDL_Color body_color{ 120,120, 120 ,0xFF };
	SDL_Color connect_point_color{ 0xFF, 0xFF, 0xFF ,0xFF };

public:

public:
	NodeUI() :
		Widget<NodeUI>(SDL_FRect{ 0,0,100,80 }) {}

	void renderImpl(SDL_Renderer* renderer);
	void updateImpl(const Input& input);

};