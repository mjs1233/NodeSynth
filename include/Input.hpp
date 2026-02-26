#pragma once
#include <SDL3/SDL.h>
#include <print>
class Input
{
public:
	int mouse_x;
	int mouse_y;
	bool left_mouse_button;
	bool left_mouse_button_down;
	bool right_mouse_button;
	bool right_mouse_button_down;

	bool key_state[SDL_Scancode::SDL_SCANCODE_COUNT] = { 0, };
	bool key_down[SDL_Scancode::SDL_SCANCODE_COUNT] = { 0, };
public:
	Input() : 
		left_mouse_button(false), 
		right_mouse_button(false), 
		left_mouse_button_down(false), 
		right_mouse_button_down(false)
	{

		float x, y;
		SDL_GetMouseState(&x, &y);
		mouse_x = static_cast<int>(x);
		mouse_y = static_cast<int>(y);

	}



	void clear_update()
	{
		left_mouse_button_down = false;
		right_mouse_button_down = false;

		for (int i = 0; i < SDL_Scancode::SDL_SCANCODE_COUNT; i++)
		{
			key_down[i] = false;
		}
	}

	void handle_input_event(SDL_Event& event)
	{
		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			handle_key_down(event);
		}
		else if (event.type == SDL_EVENT_KEY_UP)
		{
			handle_key_up(event);
		}
		else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			handle_mouse_down(event);
		}
		else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			handle_mouse_up(event);
		}
		else if (event.type == SDL_EVENT_MOUSE_MOTION)
		{
			handle_mouse_motion(event);
		}
	}

private:
	void handle_mouse_down(SDL_Event& event)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			left_mouse_button = true;
			left_mouse_button_down = true;
		}
		else if (event.button.button == SDL_BUTTON_RIGHT)
		{
			right_mouse_button = true;
			right_mouse_button_down = true;
		}
	}

	void handle_mouse_up(SDL_Event& event)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			left_mouse_button = false;
		}
		else if (event.button.button == SDL_BUTTON_RIGHT)
		{
			right_mouse_button = false;
		}
	}

	void handle_key_down(SDL_Event& event)
	{
		key_state[event.key.scancode] = true;
		key_down[event.key.scancode] = true;
	}

	void handle_key_up(SDL_Event& event)
	{
		key_state[event.key.scancode] = false;
	}

	void handle_mouse_motion(SDL_Event& event)
	{
		mouse_x = static_cast<int>(event.motion.x);
		mouse_y = static_cast<int>(event.motion.y);
		//std::print("{} {}\n", mouse_x, mouse_y);
	}
};