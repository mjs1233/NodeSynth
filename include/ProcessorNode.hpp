#pragma once
#include <cinttypes>
#include <variant>
#include <vector>
#include <concepts>
#include <array>

#include <ConfigData.hpp>

template<typename T>
concept AudioProcessNode =
requires (std::vector<float> fv, std::variant<int32_t, bool, float> vari) {
	{ std::declval<T>().process(fv,fv) } -> std::same_as<void>;
	{ std::declval<T>().config_count() } -> std::same_as<size_t>;
	{ std::declval<T>().config_data(0) } -> std::same_as<std::optional<ConfigData>>;
	{ std::declval<T>().update(0,vari) } -> std::same_as<void>;
	{ std::declval<T>().next() } -> std::same_as<std::vector<uint32_t>&>;
};
