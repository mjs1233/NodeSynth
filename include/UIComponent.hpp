#include <SDL3/SDL.h>
#include <vector>
#include <concepts>
#include <variant>
#include <functional>
#include <array>
#include <tuple>

#include "Input.hpp"

enum class ui_state {
	none,
	disabled,
	active
};

template <typename T>
concept RenderableUI = requires () {
	{ std::declval<T>().update(std::declval<Input&>()) } -> std::same_as<void>;
	{ std::declval<T>().state() } -> std::same_as<ui_state>;
	{ std::declval<T>().layer() } -> std::same_as<uint32_t>;
	{ std::declval<T>().render(std::declval<SDL_Renderer*>()) } -> std::same_as<uint32_t>;
}&&
std::is_default_constructible<T>::value;
