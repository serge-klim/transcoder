#pragma once
#include "detail/type_order.hpp"
#include <boost/mp11/list.hpp>
#include <boost/mp11/algorithm.hpp>

namespace tc { inline namespace v1 {

template<typename ...T> using options = boost::mp11::mp_sort<boost::mp11::mp_list<T...>, detail::type_less>;

} }


