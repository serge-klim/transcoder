#pragma once
#include "traits.hpp"
#include "type_name.hpp"
#include "utils.hpp"
#include <boost/mp11/utility.hpp>
#include <stdexcept>
#include <type_traits>

namespace tc {
inline namespace v1 {

template <typename T>
struct encoded_sizeof_implementation_is_missing_for;

namespace utils {

template <typename T, typename Options>
struct eval_sizeof {
   template <typename U>
   static auto select_sizeof(int) -> // encoded_sizeof<U, Options>::type
       std::integral_constant<std::size_t, encoded_sizeof<U, Options>::value>;

   template <typename U>
   static auto select_sizeof(...) -> std::conditional_t<
                                      utils::can_encode<encoder<U, Options>, T>::value,
                                      dynamic_size, not_encodable>;

   using type = decltype(select_sizeof<T>(0));
};

template <typename L, typename Options>
using eval_sizeof_t = typename eval_sizeof<L,Options>::type;

template <typename L, typename Options>
using encoded_sequence_sizeof = boost::mp11::mp_transform<eval_sizeof_t, L, Options>;

namespace detail {

template <typename... T>
struct reduce_sizeofs;

template <typename T>
struct reduce_sizeofs<T> {
   using type = T;
};

template <std::size_t Sizeof, typename... T>
struct reduce_sizeofs<dynamic_size, ::std::integral_constant<std::size_t, Sizeof>, T...> {
   using type = typename reduce_sizeofs<dynamic_size, T...>::type;
};

template <typename V, typename... T>
struct reduce_sizeofs<V, dynamic_size, T...> {
   using type = typename reduce_sizeofs<dynamic_size, T...>::type;
};

template <typename V, typename... T>
struct reduce_sizeofs<V, not_encodable, T...> {
   using type = not_encodable;
};

template <std::size_t Sizeof1, std::size_t Sizeof2, typename... T>
struct reduce_sizeofs<std::integral_constant<std::size_t, Sizeof1>, std::integral_constant<std::size_t, Sizeof2>, T...> {
   using type = typename reduce_sizeofs<std::integral_constant<std::size_t, Sizeof1 + Sizeof2>, T...>::type;
};

template <typename T>
struct reduce_sizeof_sequence;

// template <typename Sizeofs>
// using reduce_sizeof_sequence = boost::mp11::mp_fold<Sizeofs, std::integral_constant<std::size_t, 0>, boost::mp11::mp_plus>;

template <template <class...> class L, class... T>
struct reduce_sizeof_sequence<L<T...>> {
   using type = typename reduce_sizeofs<std::integral_constant<std::size_t, 0>, T...>::type;
};

} // namespace detail

template <typename... T>
using reduce_sizeofs = typename detail::reduce_sizeofs<std::integral_constant<std::size_t, 0>, T...>::type;

template <typename T>
using reduce_sizeof_sequence = typename detail::reduce_sizeof_sequence<T>::type;

namespace detail {
template <typename Options>
struct encoded_sizeof_adapter {

   template <typename T>
   struct conexpr_encoded_sizeof {
      constexpr std::size_t operator()(T const&) const noexcept {
         return tc::encoded_sizeof<T, Options>::value;
      }
   };

   template <typename T, typename S = decltype(tc::encoded_sizeof<T, Options>::value)>
   static auto check(int) -> conexpr_encoded_sizeof<T>;

   template <typename T, typename S = decltype(tc::encoded_sizeof<T, Options>{}(std::declval<T const&>()))>
   static auto check(double) -> tc::encoded_sizeof<T, Options>;

   //template <typename T>
   //struct encoded_sizeof_implementation_is_missing_for {
   //   [[noreturn]] std::size_t operator()(T const&) const noexcept {
   //      static_assert(has_encoder<T, Options>::value, "can't encode specified type");
   //      throw std::runtime_error{type_name<T>{}() + " probably can be encoded to preallocated buffer"};
   //   }
   //};
   template <typename T>
   static auto check(...) -> tc::encoded_sizeof_implementation_is_missing_for<T>;
};
} // namespace detail

template <typename T, typename Options>
using encoded_sizeof_adapter = decltype(detail::encoded_sizeof_adapter<Options>::template check<T>(0));

}

}} // namespace tc::v1::utils
