#include "NodeUI.hpp"

void NodeUI::renderImpl(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, bodyColor.r, bodyColor.g, bodyColor.b, bodyColor.a);
	SDL_RenderFillRect(renderer, &bounds);
	SDL_SetRenderDrawColor(renderer, topBarColor.r, topBarColor.g, topBarColor.b, topBarColor.a);
	SDL_FRect topBarRect{ bounds.x,bounds.y,bounds.w,20 };
	SDL_RenderFillRect(renderer, &topBarRect);
}

void NodeUI::updateImpl(const Input& input)
{

}