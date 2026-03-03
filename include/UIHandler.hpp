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

	void update() {

	}


	void clear() {

	}
};