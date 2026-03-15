#pragma once
#include <tuple>

namespace TMP {

	template <size_t L, typename T, typename ...>
	struct tuple_gen {
		using type = tuple_gen<L, T, std::tuple<>>::type;
	};

	template <size_t L, typename T, typename ...Ls>
	struct tuple_gen <L, T, std::tuple <Ls...>> {
		using type = tuple_gen<L - 1, T, std::tuple<T, Ls...>>::type;
	};

	template<typename T, typename ...Ls>
	struct tuple_gen <0, T, std::tuple<Ls...>> {
		using type = std::tuple<Ls...>;
	};
}

//TMP::tuple_gen<3, xX>::type
