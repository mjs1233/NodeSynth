#pragma once
#include <memory>
#include <functional>
#include <concepts>
#include <iostream>
#include <print>

#include <SDL3/SDL.h>
#include "Input.hpp"


template <typename render_target_t>
//check render_target_t has "render(SDL_Renderer*, SDL_Rect, int)" member function
requires requires 
{ 
	std::declval<render_target_t>().render(nullptr, SDL_Rect(), 0);
}
class Button
{
public:
	struct state_code
	{
	public:
		int idle;
		int hover;
		int clicked;

		state_code()
		{
			idle = 0;
			hover = 1;
			clicked = 2;
		}
		state_code(int idle_, int hover_, int clicked_)
		{
			idle = idle_;
			hover = hover_;
			clicked = clicked_;
		}
	};


public:
	SDL_Rect rect;
	render_target_t render_target;

private:
	int state;
	state_code state_code_;
	bool mouse_button_down = false;

public:
	template<typename ...Argv>
	requires std::is_constructible<render_target_t,Argv...>::value
	Button(Argv&& ... argv)
	{
		rect = SDL_Rect();
		state = 0;
		this->render_target = render_target_t(std::forward<Argv>(argv)...);
		state_code_ = { 0,1,2 };
	}

	Button(const render_target_t& render_target)
	{
		rect = SDL_Rect();
		state = 0;
		this->render_target = render_target;
		state_code_ = { 0,1,2 };
	}

	void update(const Input& input)
	{
		if (input.mouse_x > rect.x && input.mouse_x < rect.x + rect.w
			&& input.mouse_y > rect.y && input.mouse_y < rect.y + rect.h)
		{
			if (input.left_mouse_button)
			{
				mouse_button_down = input.left_mouse_button_down;
				state = state_code_.clicked;
			}
			else
			{
				state = state_code_.hover;
			}
		}
		else
		{
			state = state_code_.idle;
		}
	}

	bool operator()(SDL_Renderer* renderer)
	{
		render_target.render(renderer, rect, state);
		return mouse_button_down;
	}
	

};
