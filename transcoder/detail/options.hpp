#pragma once
#include "../options.hpp"
#include "../traits.hpp"
#include <boost/mp11/utility.hpp>

namespace tc { inline namespace v1 { namespace detail {

template<typename T, typename ...O>
struct effective_options {
	using type = options<O...>;
};

template<typename T>
struct effective_options<T> {
	using type = boost::mp11::mp_eval_or<options<>, proto_options, T>;
};

} // namespace detail

template<typename T, typename ...O>
using effective_options = typename detail::effective_options<T, O...>::type;

}}


