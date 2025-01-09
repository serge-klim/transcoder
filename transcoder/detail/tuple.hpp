#pragma once
#include "traits.hpp"
#include "utils.hpp"
#include <boost/mp11/algorithm.hpp>
#include <type_traits>

namespace tc { inline namespace v1 { 

namespace tuple {

template<typename T>
using type_list = boost::mp11::mp_rename<T, boost::mp11::mp_list>;

template<typename T, typename Options>
using encoded_elements_sizeof = detail::type_list_sizeof<type_list<T>, Options>;

template<typename T, typename Options>
using encoded_tuple_sizeof = detail::encoded_sequence_sizeof<encoded_elements_sizeof<T, Options>>;

template<typename T, typename Options>
using encoded_elements_offsets = detail::encoded_elements_offsets<encoded_elements_sizeof<T, Options>>;

} } }
