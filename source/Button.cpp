#include "Button.hpp"

void Button::renderImpl(SDL_Renderer* renderer)
{
    // std::print("button render\n");
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &bounds);
}

void Button::updateImpl(const Input& input)
{
	state_type prev_state = state;

	if (input.mouse_x > bounds.x && input.mouse_x < bounds.x + bounds.w
		&& input.mouse_y > bounds.y && input.mouse_y < bounds.y + bounds.h)
	{
		if (input.left_mouse_button)
		{
			state = state_type::clicked;

		}
		else
		{
			state = state_type::hover;
		}
	}
	else
	{
		state = state_type::idle;
	}

	if (prev_state != state)
		callback(this, callback_type::state_changed);
}