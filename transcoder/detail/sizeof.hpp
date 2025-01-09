#pragma once
#include "../basic_types.hpp"
#include "../traits.hpp"
#include "utils.hpp"
#include "describe.hpp"
#include "tuple.hpp"
#include <type_traits>

namespace tc {	inline namespace v1 {

template<typename T, typename Options> 
struct encoded_sizeof<T, Options, utils::trivially_encodable<T>> : std::integral_constant<std::size_t, sizeof(T)> {};

template <std::size_t N, typename Options>
struct encoded_sizeof<padding<N>, Options, std::true_type> : std::integral_constant<std::size_t, N> {};

template <typename T, std::size_t N, typename Options>
struct encoded_sizeof<std::array<T, N>, Options, utils::trivially_encodable<T>> : std::integral_constant<std::size_t, sizeof(T) * N> {};

template <typename T, std::size_t N, typename Options>
struct encoded_sizeof<T[N], Options, utils::trivially_encodable<T>> : std::integral_constant<std::size_t, sizeof(T) * N> {};

template<typename ...T, typename Options>
struct encoded_sizeof<std::tuple<T...>, Options, typename std::conjunction<utils::trivially_encodable<T>...>::type>
	: tuple::encoded_tuple_sizeof<std::tuple<T...>, Options> {
};

template<typename T, typename Options>
struct encoded_sizeof<T, Options, typename boost::describe::has_describe_members<T>::type>
	: describe::encoded_structure_sizeof<T, Options> {
};

}} // namespace tc::v1
