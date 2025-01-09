#pragma once
#include "traits.hpp"
#include "utils.hpp"
#include "utils/sizeof.hpp"
#include "utils/utils.hpp"
#include <boost/describe.hpp>
#include <boost/mp11/algorithm.hpp>
#include <type_traits>

namespace tc { inline namespace v1 {

namespace detail {

template <auto M>
struct member_type;
template <typename T, typename U, U T::*P>
struct member_type<P> {
   using type = U;
};

} // namespace detail

namespace detail {

template <typename Options>
struct encoded_member_sizeof_adapter {

   template <auto M>
   struct conexpr_encoded_member_sizeof {
      constexpr std::size_t operator()(typename member_type<M>::type const&) const noexcept {
         return tc::encoded_member_sizeof<M, Options>::value;
      }
   };

   template <auto M, typename S = decltype(tc::encoded_member_sizeof<M, Options>::value)>
   static auto check(int) -> conexpr_encoded_member_sizeof<M>;

   template <auto M, typename S = decltype(tc::encoded_member_sizeof<M, Options>{}(std::declval<typename member_type<M>::type const&>()))>
   static auto check(double) -> tc::encoded_member_sizeof<M, Options>;

   template <auto M>
   static auto check(...) -> tc::utils::encoded_sizeof_adapter<typename member_type<M>::type, Options>;
};

} // namespace detail


namespace describe {

template <auto M, typename Options>
using encoded_member_sizeof_adapter = decltype(detail::encoded_member_sizeof_adapter<Options>::template check<M>(0));

//template <typename D, typename Options>
//using encoded_member_sizeof = std::conditional_t<utils::has_member_sizeof<D, Options>::value, ::tc::v1::encoded_member_sizeof<D::pointer, Options>, encoded_sizeof<typename detail::member_type<D::pointer>::type, Options>>;

template <typename T>
using public_members = boost::mp11::mp_rename<boost::describe::describe_members<T, boost::describe::mod_public>, boost::mp11::mp_list>;

template <typename Options>
struct eval_member_sizeof {
   template<typename D>
   static auto select_sizeof(int) -> // encoded_member_sizeof<D::pointer, Options>::type
       std::integral_constant<std::size_t, encoded_member_sizeof<D::pointer, Options>::value>;

   template <typename D>
   static auto select_sizeof(float)-> //encoded_sizeof<typename detail::member_type<D::pointer>::type, Options>::type
       std::integral_constant<std::size_t, encoded_sizeof<typename detail::member_type<D::pointer>::type, Options>::value>;


   template <typename D>
   static auto select_sizeof(...) -> std::conditional_t<
										std::disjunction_v<
                                          utils::can_encode<member_encoder<D::pointer, Options>, typename detail::member_type<D::pointer>::type>,
                                          utils::can_encode<encoder<typename detail::member_type<D::pointer>::type, Options>, typename detail::member_type<D::pointer>::type>
										>, tc::dynamic_size, tc::not_encodable> ;
   template <typename D>
   // using fn = typename encoded_member_sizeof<T, Options>::type;
   using fn = decltype(select_sizeof<D>(0));
};



template <typename T, typename Options>
//using encoded_members_sizeof = boost::mp11::mp_transform_q<boost::mp11::mp_bind_back<encoded_member_sizeof, Options>, public_members<T>>;
using encoded_members_sizeof = boost::mp11::mp_transform_q<eval_member_sizeof<Options>, public_members<T>>;


//template <typename T, typename Options>
////using encoded_structure_sizeof = detail::encoded_sequence_sizeof<encoded_members_sizeof<T, Options>>;
//using encoded_structure_sizeof = utils::reduce_sizeof_sequence<encoded_members_sizeof<T, Options>>;

template <typename T, typename Options, typename S = utils::reduce_sizeof_sequence<encoded_members_sizeof<T, Options>>>
struct encoded_structure_sizeof {
   using type = S;
};

template <typename T, typename Options, std::size_t N>
struct encoded_structure_sizeof<T, Options, std::integral_constant<size_t, N>> : std::integral_constant<size_t, N> {};

template <typename T, typename Options>
struct encoded_structure_sizeof<T, Options, dynamic_size> {
   using type = dynamic_size;
   std::size_t operator()(T const& value) const {
      auto res = std::size_t{0};
      boost::mp11::mp_for_each<public_members<T>>([&res, &value](auto descriptor) {
         res += encoded_member_sizeof_adapter<decltype(descriptor)::pointer, Options>{}(value.*descriptor.pointer);
      });
      return res;
   }
};

template <typename T, typename Options>
using encoded_members_offsets = detail::encoded_elements_offsets<encoded_members_sizeof<T, Options>>;

namespace detail {
template <auto P, typename Options>
struct encoded_member_offset_impl;

template <typename T, typename U, U T::*P, typename Options>
struct encoded_member_offset_impl<P, Options> {
   using position = boost::mp11::mp_find_if_q<public_members<T>, boost::describe::detail::match_by_pointer<P>>; // probably shouldn't be using boost::describe::detail
   using type = ::tc::v1::detail::encoded_sequence_sizeof<boost::mp11::mp_take<encoded_members_sizeof<T, Options>, position>>;
};
} // namespace detail

template <auto P, typename Options>
using encoded_member_offset = typename detail::encoded_member_offset_impl<P, Options>::type;

} // namespace describe
}} // namespace tc::v1
