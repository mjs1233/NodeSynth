#pragma once
#include <concepts>
#include <array>
#include <map>
#include <SDL3/SDL.h>

template <typename T,size_t value_count>
requires std::is_enum<T>::value
class ColorMap {
public:
	std::map<T, SDL_Color> color_map;

	ColorMap() {};
};

