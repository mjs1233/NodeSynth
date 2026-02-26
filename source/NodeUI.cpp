#include "NodeUI.hpp"

void NodeUI::renderImpl(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, body_color.r, body_color.g, body_color.b, body_color.a);
	SDL_RenderFillRect(renderer, &bounds);
	SDL_SetRenderDrawColor(renderer, top_bar_color.r, top_bar_color.g, top_bar_color.b, top_bar_color.a);
	SDL_FRect topBarRect{ bounds.x,bounds.y,bounds.w,20 };
	SDL_RenderFillRect(renderer, &topBarRect);
}

void NodeUI::updateImpl(const Input& input)
{

}