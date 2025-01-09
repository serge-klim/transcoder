#pragma once
#include "traits.hpp"
#include "utils.hpp"
#include <boost/describe.hpp>
#include <boost/mp11/algorithm.hpp>
#include <type_traits>

namespace tc { inline namespace v1 { namespace describe {

template<typename D, typename Options>
using encoded_member_sizeof = ::tc::v1::encoded_member_sizeof<D::pointer, Options>;

template<typename T>
using public_members = boost::mp11::mp_rename<boost::describe::describe_members<T, boost::describe::mod_public>, boost::mp11::mp_list>;

template<typename T, typename Options>
using encoded_members_sizeof = boost::mp11::mp_transform_q<boost::mp11::mp_bind_back<encoded_member_sizeof, Options>, public_members<T>>;

template<typename T, typename Options>
using encoded_structure_sizeof = detail::encoded_sequence_sizeof<encoded_members_sizeof<T, Options>>;
            
template<typename T, typename Options>
using encoded_members_offsets = detail::encoded_elements_offsets<encoded_members_sizeof<T, Options>>;

namespace detail {
template <auto P, typename Options>
struct encoded_member_offset_impl;

template <typename T, typename U, U T::* P, typename Options>
struct encoded_member_offset_impl<P, Options>
{
	using position = boost::mp11::mp_find_if_q<public_members<T>, boost::describe::detail::match_by_pointer<P>>; // probably shouldn't be using boost::describe::detail
	using type = ::tc::v1::detail::encoded_sequence_sizeof<boost::mp11::mp_take<encoded_members_sizeof<T, Options>, position>>;
};
} // namespace detail

template <auto P, typename Options>
using encoded_member_offset = typename detail::encoded_member_offset_impl<P, Options>::type;


} } }
