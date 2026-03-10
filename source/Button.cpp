#include "Button.hpp"

template<>
uint32_t Button<Rect>::render(SDL_Renderer* renderer) {

	SDL_Color rect_color = color_map[render_state];
	SDL_SetRenderDrawColor(renderer, rect_color.r, rect_color.g, rect_color.b, rect_color.a);
	drawable.render(renderer);
	return 0;
}
