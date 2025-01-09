#pragma once
#include <type_traits>
#include <cstddef>

namespace tc { inline namespace v1 {


using byte_t = /*unsigned*/ char;

template<typename T, typename Options, typename Enable = std::true_type> struct encoded_sizeof;
template<typename T, typename Options, typename Enable = std::true_type> struct encoder;
template<typename T, typename Options, typename Enable = std::true_type> struct decoder;

template<auto T, typename Options, typename Enable = std::true_type> struct encoded_member_sizeof;
template<auto T, typename Options, typename Enable = std::true_type> struct member_encoder;
template<auto T, typename Options, typename Enable = std::true_type> struct member_decoder;

template<typename T, typename U, U T::* P, typename Options> struct encoded_member_sizeof<P, Options> : encoded_sizeof<U, Options> {};
template<typename T, typename U, U T::* P, typename Options> struct member_encoder<P, Options> : encoder<U, Options> {};
template<typename T, typename U, U T::* P, typename Options> struct member_decoder<P, Options> : decoder<U, Options> {};


template<typename T> struct type_id;

template<typename T>
using proto_options = decltype(protocol_options(std::declval<T>()));  // tc::options<> protocol_options(...) - has to be declared in protocol namespace


} }


