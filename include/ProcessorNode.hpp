#pragma once
#include <cinttypes>
#include <vector>
#include <concepts>
#include <array>

template<typename T>
concept AudioProcessNode =
requires (std::vector<float> fv) {
	{ std::declval<T>().process(fv,fv) } -> std::same_as<void>;
	{ std::declval<T>().get_next() } -> std::same_as<std::vector<uint32_t>&>;
};
