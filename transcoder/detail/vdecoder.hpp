#pragma once
#include "../traits.hpp"
#include "../error.hpp"
#include "utils.hpp"
#include "sint.hpp"
#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/list.hpp>
#include <variant>
#include <span>
#include <algorithm>
#include <type_traits>
#include <cstring>
#include <stdexcept>

namespace tc { inline namespace v1 { 

template<typename ...T, typename Options>
struct decoder<std::variant<T...>, Options, std::true_type> {
	using type_list = boost::mp11::mp_list<T...>;
	template<typename H>
	void operator()(byte_t const*& begin, byte_t const* end, H&& handler);
	std::variant<T...> operator()(byte_t const*& begin, byte_t const* end) /*noexcept
		(std::conjunction_v<boost::mp11::mp_contains<type_list, unexpected>, boost::mp11::mp_contains<type_list, more_wanted>>)*/;
};

namespace detail {
std::size_t find_type_ix(std::span<std::uint8_t const> ixs, std::uint8_t id);

//template<typename T, std::size_t N>
//std::size_t find_type_ix(std::span<T const, N> ixs, std::uint8_t id) noexcept {
//
//}

template<typename T, typename Options, typename H>
void dispatch_type(tc::byte_t const*& begin, tc::byte_t const* end, H&& handler) {
	std::forward<H>(handler)(tc::decoder<T, Options>{}(begin, end));
}

template<typename T>
struct dispatcher : std::integral_constant<decltype(type_id<T>{}()), type_id<T>{}() > {
	using type = T;
};

}
}}

template<typename ...T, typename Options> template<typename H>
void tc::v1::decoder<std::variant<T...>, Options, std::true_type>::operator()(byte_t const*& begin, byte_t const* end, H&& handler) {
	using dispatcher = void(*)(byte_t const*&, byte_t const*, H&&);
	static constexpr dispatcher dispatchers[] = { &detail::dispatch_type<T,Options,H>..., };

	using id_type = typename detail::simplified_integer_type<decltype(type_id<boost::mp11::mp_front<type_list>>{}()) > ::type;
	auto size = std::distance(begin, end);
	if (static_cast<std::size_t>(size) < sizeof(id_type))
		std::forward<H>(handler)(more_wanted{ sizeof(id_type) });
	static constexpr id_type ids[] = { type_id<T>{}()..., };
	static constexpr auto ids_end = ids + std::ssize(ids);
	id_type type;
	std::memcpy(&type, begin, sizeof(type));
	if(auto i = std::find(ids, ids_end, type); i!=ids_end)
		(*dispatchers[std::distance(ids,i)])(begin, end, std::forward<H>(handler));
	else
		std::forward<H>(handler)(unknown_type{});
	//static const auto selector = type_selector<T...>{};
	//typename type_selector<T...>::value_t type;
	//std::memcpy(&type, begin, sizeof(type));
	//if (auto ix = selector.find(type); ix < selector.size())
	//	(*dispatchers[ix])(begin, end, std::forward<H>(handler));
	//else
	//	std::forward<H>(handler)(unknown_type{});
}

//template<typename ...T, typename Options> template<typename H>
//void tc::v1::decoder<std::variant<T...>, Options, std::true_type>::operator()(byte_t const*& begin, byte_t const* end, H&& handler) {
//	using diptachers = boost::mp11::mp_sort < boost::mp11::mp_list<detail::dispatcher<T>... >, boost::mp11::mp_less>;
//	decode(diptachers{}, begin, end, std::forward<H>(handler));
//}
//
//template<typename ...T, typename Options>
//template<typename H, typename ...D>
//void tc::v1::decoder<std::variant<T...>, Options, std::true_type>::decode(boost::mp11::mp_list<D...>, byte_t const*& begin, byte_t const* end, H&& handler) {
//	static_assert(sizeof...(T) == sizeof...(D));
//	using id_type = typename detail::simplified_integer_type<decltype(type_id<boost::mp11::mp_front<type_list>>{}()) > ::type;
//	auto size = std::distance(begin, end);
//	if (size < sizeof(id_type))
//		std::forward<H>(handler)(more_wanted{ sizeof(id_type) });
//
//	using dispatcher = void(*)(byte_t const*&, byte_t const*, H&&);
//	static constexpr id_type ids[] = { type_id<typename D::type>{}()..., };
//	static constexpr dispatcher dispatchers[] = { &detail::dispatch_type<typename D::type,Options,H>..., };
//	static constexpr auto ids_end = ids + std::ssize(ids);
//	id_type type;
//	std::memcpy(&type, begin, sizeof(type));
//	if(auto i = std::lower_bound(ids, ids_end, type); i!=ids_end && *i == type)
//		(*dispatchers[std::distance(ids,i)])(begin, end, std::forward<H>(handler));
//	else
//		std::forward<H>(handler)(unknown_type{});
//}

template<typename ...T, typename Options>
std::variant<T...>  tc::v1::decoder<std::variant<T...>, Options, std::true_type>::operator()(byte_t const*& begin, byte_t const* end) /*noexcept
		(std::conjunction_v<boost::mp11::mp_contains<type_list, unexpected>, boost::mp11::mp_contains<type_list, more_wanted>>)*/ {
	auto res = std::variant<T...>{};
	(*this)(begin, end, [&](auto&& type) {
		if constexpr (std::is_assignable_v<decltype(res), decltype(type)>)
			res = std::move(type);
		else
			handle_error(std::forward<std::decay_t<decltype(type)>>(type));
	});
	return res;
}

