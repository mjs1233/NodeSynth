#pragma once
#include <concepts>
#include <vector>
#include <array>
#include <string>
#include <print>
#include <deque>
#include <tuple>
#include <optional>
#include <SDL3/SDL.h>
#include "UIComponent.hpp"

template <RenderableUI ...renderable_ui_t>
class UIHandler
{
public:
	std::tuple<std::deque<renderable_ui_t>...> ui_lists;

private:

public:
	UIHandler() {

	}


	template <RenderableUI T>
	requires (std::is_same<renderable_ui_t, T>::value || ...)
	void add(T& ui) {
		std::get<std::deque<T>>(ui_lists).push_back(ui);
	}

	template<RenderableUI T, typename ...Args>
	requires (std::is_same<renderable_ui_t, T>::value || ...) && std::constructible_from<T,Args...>
	void add(Args&& ...args) {
		std::get<std::deque<T>>(ui_lists).emplace_back(std::forward<Args>(args)...);
	}

	void update(Input& input) {
		update_all(std::index_sequence_for<renderable_ui_t...>{},input);
	}

	void render(SDL_Renderer* renderer) {
		render_all(std::index_sequence_for<renderable_ui_t...>{}, renderer);
	}

private:
	template<std::size_t... Is>
	void update_all(std::index_sequence<Is...>,Input& input) {
		(update_deque(std::get<Is>(ui_lists), input), ...);
	}

	template <typename T>
	void update_deque(std::deque<T>& target,Input& input) {
		for (auto& ui : target) {
			ui.update(input);
		}
	}

	template<std::size_t... Is>
	void render_all(std::index_sequence<Is...>, SDL_Renderer* renderer) {
		(render_deque(std::get<Is>(ui_lists), renderer), ...);
	}

	template <typename T>
	void render_deque(std::deque<T>& target, SDL_Renderer* renderer) {
		for (auto& ui : target) {
			ui.render(renderer);
		}
	}


	void clear() {

	}
};