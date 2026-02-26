#pragma once

#include <SDL3/SDL.h>
#include <print>
/*
class RenderEndpoint_Rect
{
public:
	SDL_Color color;
	int p = 0;
public:

	RenderEndpoint_Rect()
	{
		this->color = SDL_Color{ 0xFF,0xFF,0xFF,0xFF };
	}

	RenderEndpoint_Rect(SDL_Color color) : color(color)
	{}

	bool render(SDL_Renderer* renderer, SDL_Rect rect, int state)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

		std::print("{} {} {}\n", color.r, color.g, color.b);
		SDL_FRect frect = { 0, };
		SDL_RectToFRect(&rect, &frect);
		SDL_RenderFillRect(renderer, &frect);
		return true;
	}



};*/