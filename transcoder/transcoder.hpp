#pragma once
#include "detail/options.hpp"
#include "detail/encoder.hpp"
#include "detail/decoder.hpp"
#include "detail/vdecoder.hpp"
#include <boost/mp11/tuple.hpp>
#include <boost/mp11/utility.hpp>
#include <vector>
#include <tuple>
#include <cassert>

namespace tc { inline namespace v1 {

namespace wrapped {

template<typename Options, typename T>
byte_t* encode(T const& value, byte_t* begin, byte_t* end) { return encoder<T, Options>{}(value, begin, end); }

template <typename Options, typename T, typename Sizeof = encoded_sizeof<T, Options>>
[[nodiscard]] std::vector<byte_t> encode(T const& value) {
    auto buffer = std::vector<byte_t>(Sizeof::value);
	auto begin = buffer.data();
	encode<Options,T>(value, begin, begin + buffer.size());
	return buffer;
}

template<typename T, typename Options>
[[nodiscard]] T decode(byte_t*& begin, byte_t const* end) {
	byte_t const* b = begin;
	auto res = decoder<T, Options>{}(b, end);
	begin += b - begin;
	return res;
}

template<typename T, typename Options>
[[nodiscard]] T decode(byte_t const*& begin, byte_t const* end) { return decoder<T, Options>{}(begin, end); }

template <typename T, typename Options, typename Handler>
void decode(byte_t *&begin, byte_t const *end, Handler &&handler) {
	byte_t const* b = begin;
	decoder<T, Options>{}(b, end, std::forward<Handler>(handler));
	begin += b - begin;
}

template <typename T, typename Options, typename Handler>
void decode(byte_t const *&begin, byte_t const *end, Handler&& handler) { 
	decoder<T, Options>{}(begin, end, std::forward<Handler>(handler));
}

} // namespace wrapped

template<typename ...Options, typename ...T>
byte_t* encode(byte_t* begin, byte_t* end, T const& ...value) {
	boost::mp11::tuple_for_each(std::forward_as_tuple(value...), [&begin, end](auto const& val) {
		using type = std::decay_t<decltype(val)>;
		begin = wrapped::encode<effective_options<type, Options...>>(val, begin, end);
	});
	return begin;
}

template<typename ...Options, typename ...T>
[[nodiscard]] std::vector<byte_t> encode(T const& ...value) { 
	using type_list = boost::mp11::mp_list<T...>;
	using option_list = boost::mp11::mp_list<effective_options<T, Options...>... >;
    using sizeofs = utils::encoded_sequence_sizeof<type_list, option_list>;
    using encoded_size = utils::reduce_sizeof_sequence<sizeofs>;
    static_assert(!std::is_same_v<encoded_size, not_encodable>, "can't encode sequence, consider implementing tc::encoder/tc::encoded_sz for all specified types");
    auto size = std::size_t{0};
    if constexpr (std::is_same_v<encoded_size, dynamic_size>) {
	   //encoded_sizeof_implementation_is_missing_for<...>... but seems has encoder, therefore probably can be encoded to preallocated buffer 
       size = (... + utils::encoded_sizeof_adapter<T, effective_options<T, Options...>>{}(value));
	} else {
       size = encoded_size::value;
	}
    auto res = std::vector<byte_t>(size);
    auto begin = res.data();
    begin = encode<Options...>(begin, begin + res.size(), value...);
    assert(begin == res.data() + res.size());
    return res;
}

template<typename T, typename ...Options> 
[[nodiscard]] T decode(byte_t*& begin, byte_t const* end) { return wrapped::decode<T, effective_options<T, Options...>>(begin, end);}

template<typename T, typename ...Options> 
[[nodiscard]] T decode(byte_t const*& begin, byte_t const* end) { return wrapped::decode<T, effective_options<T, Options...>>(begin, end); }

template <typename T, typename Iterator, typename Handler, typename... Options>
void decode(Iterator& begin, Iterator end, Handler &&handler) { wrapped::decode<T, effective_options<T, Options...>>(begin, end, std::forward<Handler>(handler)); }

template <typename T, typename Handler, typename... Options>
void decode(byte_t const *&begin, byte_t const *end, Handler &&handler) { wrapped::decode<T, effective_options<T, Options...>>(begin, end, std::forward<Handler>(handler)); }

}} // namespace tc::v1
