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
    Button(const SDL_FRect& bounds,const std::function<void(Button*, callback_type)>& callback, SDL_Color color) : 
		Widget<Button>(bounds), 
		callback(callback),
		color(color),
		text(""),
		state(state_type::idle) {}

	void renderImpl(SDL_Renderer* renderer);
	void updateImpl(const Input& input);

};
