#pragma once

#include <array>
#include <tuple>
#include <print>
#include <variant>
#include <functional>

#include <SDL3/SDL.h>

template <typename ...targets>
class MutiStateRenderTarget
{
public:
	std::array<int, sizeof ...(targets)> state_map;
	std::tuple<targets...> target;
	std::array<std::function<bool(SDL_Renderer* renderer, SDL_Rect rect, int state)>, sizeof ...(targets)> render_calls;
	
	MutiStateRenderTarget() {}

	template<typename ...T>
	requires std::is_same_v<std::tuple<targets...>, std::tuple<T...>>
	MutiStateRenderTarget(T&& ...t)
	{
		target = std::make_tuple(std::forward<T>(t)...);
		state_map = std::array<int, sizeof ...(targets)>();
		fill_render_calls(std::index_sequence_for<targets...>{});

	}

	template<std::size_t... Is>
	void fill_render_calls(std::index_sequence<Is...>)
	{
		((render_calls[Is] = [&](SDL_Renderer* renderer, SDL_Rect rect, int state) { return std::get<Is>(target).render(renderer,rect,state); }), ...);
	}



	bool render(SDL_Renderer* renderer, SDL_Rect rect, int state)
	{
		return render_calls[state](renderer, rect, state);
	}

};