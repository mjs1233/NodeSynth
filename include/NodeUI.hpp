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
	SDL_Color topBarColor{60,60,60 ,0xFF};
	SDL_Color bodyColor{ 120,120, 120 ,0xFF };
	SDL_Color connectPointColor{ 0xFF, 0xFF, 0xFF ,0xFF };

public:

public:
	NodeUI() :
		Widget<NodeUI>(SDL_FRect{ 0,0,100,80 }) {}

	void renderImpl(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, bodyColor.r, bodyColor.g, bodyColor.b, bodyColor.a);
		SDL_RenderFillRect(renderer, &bounds);
		SDL_SetRenderDrawColor(renderer, topBarColor.r, topBarColor.g, topBarColor.b, topBarColor.a);
		SDL_FRect topBarRect{ bounds.x,bounds.y,bounds.w,20 };
		SDL_RenderFillRect(renderer, &topBarRect);
	}

	void updateImpl(const Input& input)
	{
		
	}
};