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

template <UIComponent ...ui_component_t>
class UIHandler
{
public:
	std::tuple<std::deque<ui_component_t>...> ui_lists;

private:

public:
	UIHandler() {

	}


	template <UIComponent T>
	requires (std::is_same<ui_component_t, T>::value || ...)
	void add(T& ui) {
		std::get<std::deque<T>>(ui_lists).push_back(ui);
	}

	template<UIComponent T, typename ...Args>
	requires (std::is_same<ui_component_t, T>::value || ...) && std::constructible_from<T,Args...>
	size_t add(Args&& ...args) {

		std::print("CALL ADD2\n");
		std::get<std::deque<T>>(ui_lists).emplace_back(std::forward<Args>(args)...);
		return std::get<std::deque<T>>(ui_lists).size() - 1;
	}

	void update(Input& input) {
		update_all(std::index_sequence_for<ui_component_t...>{},input);
	}

	void render(SDL_Renderer* renderer) {
		render_all(std::index_sequence_for<ui_component_t...>{}, renderer);
	}

	template <UIComponent T>
	requires (std::is_same<ui_component_t, T>::value || ...)
	T& get(size_t idx) {
		return std::get<std::deque<T>>(ui_lists)[idx];
	}

	template <UIComponent T>
	requires (std::is_same<ui_component_t, T>::value || ...)
	void clear() {
		std::get<std::deque<T>>(ui_lists).clear();
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

	template <std::size_t... ls>
	void clear_all(std::index_sequence<ls...>) {
		(clear_deque(std::get<ls>(ui_lists)), ...);
	}

	template <typename T>
	void clear_deque(std::deque<T>& target) {
		target.clear();
	}

};