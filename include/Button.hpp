#pragma once
#include <SDL3/SDL.h>

#include <string>
#include <concepts>
#include <print>
#include <functional>

#include "Widget.hpp"


class Button : public Widget<Button> 
{
private:


public:
	enum class callback_type
	{
		state_changed
	};

	enum class state_type
	{
		idle, hover, clicked
	};
	state_type state;
	SDL_Color color;
    std::string text;
    std::function<void(Button* self, callback_type event_)> callback;

public:
    Button(const SDL_FRect& bounds,const std::function<void(Button* self, callback_type event_)>& callback, SDL_Color color) : 
		Widget<Button>(bounds), 
		callback(callback),
		color(color),
		text(""),
		state(state_type::idle) {}

    void renderImpl(SDL_Renderer* renderer)
    {
       // std::print("button render\n");
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &bounds);
    }

	void updateImpl(const Input& input)
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
			callback(this,callback_type::state_changed);
	}

};
