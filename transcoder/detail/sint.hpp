#pragma once
#include <type_traits>
#include <cstdint>
//#include <boost/integer.hpp>

namespace tc { inline namespace v1 { namespace detail {

//template <typename T>
//using simplified_integer_type = typename boost::int_t<sizeof(T)/sizeof(std::uint8_t)*8>::exact; 

template<typename T,typename Enable = std::true_type>
struct simplified_integer_type;

template <typename T>
struct simplified_integer_type<T, typename std::conjunction<std::is_trivially_copyable<T>, std::bool_constant<sizeof(T) == sizeof(std::uint8_t)>>::type> {
	using type = std::conditional_t<std::is_signed_v<T>,std::int8_t,std::uint8_t>;
};

template <typename T>
struct simplified_integer_type<T, typename std::conjunction<std::is_trivially_copyable<T>, std::bool_constant<sizeof(T) == sizeof(std::uint16_t)>>::type> {
	using type = std::conditional_t<std::is_signed_v<T>, std::int16_t, std::uint16_t>;
};

template <typename T>
struct simplified_integer_type<T, typename std::conjunction<std::is_trivially_copyable<T>, std::bool_constant<sizeof(T) == sizeof(std::uint32_t)>>::type>{
	using type = std::conditional_t<std::is_signed_v<T>, std::int32_t, std::uint32_t>;
};

template <typename T>
struct simplified_integer_type<T, typename std::conjunction<std::is_trivially_copyable<T>, std::bool_constant<sizeof(T) == sizeof(std::uint64_t)>>::type> {
	using type = std::conditional_t<std::is_signed_v<T>, std::int64_t, std::uint64_t>;
};

} } }
