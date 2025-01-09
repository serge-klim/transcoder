#pragma once
#include "traits.hpp"
#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/bind.hpp>
#include <type_traits>

namespace tc { inline namespace v1 { namespace detail {

template<typename T>
using trivially_encodable = typename std::conjunction<std::is_trivially_copyable<T>, std::negation<std::is_class<T>>, std::negation<std::is_array<T>>>::type;

//template<typename T, typename Options>
//using has_constexpr_encoded_sizeof = std::is_same<typename encoded_sizeof<T, Options>::value_type, std::size_t>;

template<typename Sizeofs>
using encoded_elements_offsets = boost::mp11::mp_partial_sum<Sizeofs, std::integral_constant<std::size_t, 0>, boost::mp11::mp_plus>;

template<typename Offsets>
using elements_offsets = boost::mp11::mp_pop_back<boost::mp11::mp_push_front<Offsets, std::integral_constant<std::size_t, 0>>>;

template<typename L, typename Options>
using type_list_sizeof = boost::mp11::mp_transform_q<boost::mp11::mp_bind_back<encoded_sizeof, Options>, L>;

template<typename Sizeofs>
using encoded_sequence_sizeof = boost::mp11::mp_fold<Sizeofs, std::integral_constant<std::size_t, 0>, boost::mp11::mp_plus>;
            
}}} // namespace tc::v1::detail	



