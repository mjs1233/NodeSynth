#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <concepts>
#include <variant>
#include <functional>
#include <array>
#include <tuple>

#include "Input.hpp"
#include "UIConfig.hpp"

template <typename T>
concept Drawable =
std::default_initializable<T> &&
	requires {
		{ std::declval<T>().layer() } -> std::same_as<ui_layer_t>;
		{ std::declval<T>().render(std::declval<SDL_Renderer*>()) } -> std::same_as<uint32_t>;
};

template <typename T>
concept UIComponent =
Drawable<T>
&& requires () {
		{ std::declval<T>().update(std::declval<Input&>()) } -> std::same_as<void>;
		{ std::declval<T>().state() } -> std::same_as<ui_state_t>;
};
