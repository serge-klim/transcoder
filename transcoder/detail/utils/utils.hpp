#pragma once
#include "traits.hpp"
#include <boost/mp11/utility.hpp>
#include <type_traits>

namespace tc { inline namespace v1 { namespace utils {

namespace detail {

template <typename E, typename T>
using can_encode = typename std::is_same<byte_t*, decltype(E{}(std::declval<T const&>(), std::declval<byte_t*>(), std::declval<byte_t*>()))>::type;

template <typename D, typename T>
using can_decode = typename std::is_same<T, decltype(D{}(std::declval<byte_t const*&>(), std::declval<byte_t const*>()))>::type;

} // namespace detail

template <typename E, typename T>
using can_encode = boost::mp11::mp_valid_and_true<detail::can_encode, E, T>;

template <typename D, typename T>
using can_decode = boost::mp11::mp_valid_and_true<detail::can_decode, D, T>;

template <typename T, typename Options>
using has_encoder = can_encode<tc::encoder<T, Options>, T>;

template <typename T, typename Options>
using has_decoder = can_decode<tc::decoder<T, Options>, T>;

}}} // namespace tc::v1::utils
