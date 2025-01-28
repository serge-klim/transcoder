#pragma once
#include "traits.hpp"
#include "detail/decoder.hpp"
#include "options.hpp"
#include <boost/mp11/algorithm.hpp>
#include <variant>
#include <iterator>
#include <type_traits>
#include <tuple>
#include <cassert>

namespace tc { inline namespace v1 {

template<typename T, typename Options = proto_options<T>>
class flyweight {
public:
	using type = T;
	constexpr flyweight(byte_t const* ptr = nullptr) noexcept : ptr_{ptr} {}
	constexpr static auto type_id() noexcept { return tc::type_id<T>{}(); }
	constexpr static auto size() noexcept { return encoded_sizeof<T, Options>{}(); }
	constexpr auto data() const noexcept { return ptr_; }
	T value() const {
		auto begin = ptr_;
		return decoder<T, Options>{}(begin, begin + encoded_sizeof<T, Options>{}());
	}
	operator T() const { return value();}
private:
	byte_t const* ptr_;
};


template<auto P, typename T, typename Options>
auto get(flyweight<T, Options> const& fw) {
	if constexpr (std::is_member_object_pointer_v<decltype(P)>) {
		static constexpr auto offset = tc::describe::encoded_member_offset<P, Options>::type::value;
		auto data = fw.data() + offset;
		return tc::member_decoder<P, Options>{}(data, data - offset + encoded_sizeof<T, Options>{}());
	} else {
		static_assert(std::is_same_v<T, T>, "only implemented for pointers now");
	}
}

template <auto... P, typename T, typename Options, typename Enable = std::enable_if_t<(sizeof...(P) > 1), void>>
auto get(flyweight<T, Options> const& fw) {
   return std::make_tuple(get<P,T, Options>(fw)...);
}


template<typename T, typename Options>
struct encoded_sizeof<flyweight<T, Options>, Options, std::true_type> : encoded_sizeof<T, Options> {};

template<typename T, typename Options>
struct type_id<flyweight<T, Options>> : type_id<T> {};

template<typename T, typename Options>
struct decoder<flyweight<T, Options>, Options, /*std::is_same<std::size_t, decltype(std::declval<encoded_sizeof<T>>()) >*/ std::true_type> {
	flyweight<T, Options> operator()(byte_t const*& begin, [[maybe_unused]] byte_t const* end) noexcept {
		assert(static_cast<std::size_t>(std::distance(begin, end)) >= (encoded_sizeof<T, Options>{}()));
		auto fw = begin;
		begin += encoded_sizeof<T, Options>{}();
		return { fw };
	}
};

namespace detail {

template<typename T> struct as_flyweight;
template<typename ...T> struct as_flyweight<std::variant<T...>> {
	using type = std::variant<flyweight<T>...>;
};
}

template<typename T>
using as_flyweight = typename detail::as_flyweight<T>::type;

} }


