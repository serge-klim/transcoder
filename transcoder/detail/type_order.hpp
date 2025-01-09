#pragma once
#include <boost/mp11/integral.hpp>
#include <string_view>


namespace tc { inline namespace v1 { namespace detail {

template <typename T>
constexpr std::string_view type_tag() noexcept {
#ifdef _MSC_VER
	return  __FUNCSIG__;
#else
	return  __PRETTY_FUNCTION__;
#endif
}

template <typename L, typename R>
using type_less = boost::mp11::mp_bool < type_tag<L>() < type_tag<R>() > ;

} } }
