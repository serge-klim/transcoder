#pragma once
#include "../basic_types.hpp"
#include "../traits.hpp"
#include "utils.hpp"
#include "sint.hpp"
#include "byte_order.hpp"
#include "describe.hpp"
#include "tuple.hpp"

#include <boost/endian.hpp>
#include <boost/describe.hpp>
#include <boost/mp11/list.hpp>
#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/utility.hpp>
#include <array>
#include <bit>
#include <iterator>
#include <chrono>
#include <type_traits>
#include <cassert>

namespace tc { inline namespace v1 {

template <typename T, typename Options>
struct decoder<T, Options, utils::trivially_encodable<T>> {
   T operator()(byte_t const*& begin, [[maybe_unused]] byte_t const* end) noexcept {
      constexpr auto size = encoded_sizeof<T, Options>::value;
      static_assert(size == sizeof(T), "seems tc::encoded_sizeof specialized incorrectly");
      assert(std::distance(begin, end) >= size);
      begin += encoded_sizeof<T, Options>::value;
      constexpr auto byte_order = flag::byte_order_from_options<Options, default_byte_order()>;
      if constexpr (size == sizeof(char) || byte_order == boost::endian::order::native) {
         auto res = T{};
         std::memcpy(&res, begin - size, size);
         return res;
      } else {
         auto res = typename detail::simplified_integer_type<T>::type{};
         std::memcpy(&res, begin - size, size);
         return std::bit_cast<T>(boost::endian::conditional_reverse<byte_order, boost::endian::order::native>(res));
      }
   }
};

template <std::size_t N, typename Options>
struct decoder<padding<N>, Options, std::true_type> {
   padding<N> operator()(byte_t const*& begin, [[maybe_unused]] byte_t const* end) noexcept {
      constexpr auto size = encoded_sizeof<padding<N>, Options>::value;
      static_assert(size == N, "seems tc::encoded_sizeof specialized incorrectly");
      assert(std::distance(begin, end) >= size);
      begin += size;
      return {};
   }
};

template <class Rep, class Period, typename Options>
struct decoder<std::chrono::duration<Rep, Period>, Options, utils::trivially_encodable<Rep>> {
   std::chrono::duration<Rep, Period> operator()(byte_t const*& begin, [[maybe_unused]] byte_t const* end) noexcept {     
      return std::chrono::duration<Rep, Period>{decoder<Rep, Options>{}(begin, end)};
   }
};

template <typename T, std::size_t N, typename Options>
struct decoder<std::array<T, N>, Options, utils::trivially_encodable<T>> {
   std::array<T, N> operator()(byte_t const*& begin, byte_t const* end) noexcept {
      auto res = std::array<T, N>{};
      static_assert(encoded_sizeof<T, Options>::value == sizeof(T), "seems tc::encoded_sizeof specialized incorrectly");
      constexpr auto size = encoded_sizeof<T, Options>::value * res.size();
      assert(std::distance(begin, end) >= size);
      constexpr auto byte_order = flag::byte_order_from_options<Options, default_byte_order()>;
      if constexpr (encoded_sizeof<T, Options>::value == sizeof(char) || byte_order == boost::endian::order::native) {
         std::memcpy(res.data(), begin, size);
         begin += size;
      } else {
         //"TODO:decode at once to certain size";
         for (auto&& v : res)
            v = decoder<T, Options>{}(begin, end);
      }
      return res;
   }
};

template <typename... T, typename Options>
struct decoder<std::tuple<T...>, Options, typename std::conjunction<utils::trivially_encodable<T>...>::type> {
   std::tuple<T...> operator()(byte_t const*& begin, byte_t const* end) noexcept {
      using struct_sizeof = decltype(members_sizeof(0, encoded_sizeof<T, Options>{}...));
      if constexpr (std::is_same_v<std::size_t, struct_sizeof>) {
         // if constexpr (boost::mp11::mp_valid_and_true<detail::has_constexpr_encoded_sizeof, std::tuple<T...>, Options>::value) {
         using sizeofs = tuple::encoded_elements_sizeof<std::tuple<T...>, Options>;
         using offsets = detail::elements_offsets<tuple::encoded_elements_offsets<std::tuple<T...>, Options>>;
         return decode(offsets{}, sizeofs{}, begin, end);
      } else
         return decode(begin, end);
   }

 private:
   template <typename... U>
   auto members_sizeof(int, encoded_sizeof<U, Options>... args) -> decltype((args() + ...));
   template <typename... U>
   void members_sizeof(double, encoded_sizeof<U, Options>...);

   template <typename U, typename Sizeof>
   auto decode_element(byte_t const* begin) const {
      return decoder<U, Options>{}(begin, begin + Sizeof::value);
   }

   template <typename... Offset, typename... Sizeof>
   std::tuple<T...> decode(boost::mp11::mp_list<Offset...>, boost::mp11::mp_list<Sizeof...>, byte_t const*& begin, [[maybe_unused]] byte_t const* end) {
      constexpr auto size = boost::mp11::mp_back<boost::mp11::mp_list<Offset...>>::value + boost::mp11::mp_back<boost::mp11::mp_list<Sizeof...>>::value;
      assert(size <= std::distance(begin, end));
      auto tmp = begin;
      begin += size;
      return {
          decode_element<T, Sizeof>(tmp + Offset::value)...,
      };
   }

   template <typename... P>
   std::tuple<T...> decode(byte_t const*& begin, byte_t const* end, P&&... parts) {
      auto constexpr n = sizeof...(P);
      if constexpr (n == sizeof...(T))
         return {std::forward<P>(parts)...};
      else {
         using E = boost::mp11::mp_at_c<boost::mp11::mp_list<T...>, n>;
         return decode(begin, end, std::forward<P>(parts)..., decoder<E, Options>{}(begin, end));
      }
   }
};

template <typename T, typename Options>
struct decoder<T, Options, typename std::conjunction<boost::describe::has_describe_members<T>, std::negation<std::is_same<typename describe::encoded_structure_sizeof<T, Options>::type, tc::not_encodable>>>::type> {
   using encoded_size = typename encoded_sizeof<T, Options>::type;
   T operator()(byte_t const*& begin, byte_t const* end) noexcept {
      if constexpr (!std::is_same_v<encoded_size, dynamic_size> && !std::is_same_v<encoded_size, not_encodable>) {
         using sizeofs = describe::encoded_members_sizeof<T, Options>;
         using offsets = detail::elements_offsets<describe::encoded_members_offsets<T, Options>>;
         return decode(boost::describe::describe_members<T, boost::describe::mod_public>{}, offsets{}, sizeofs{}, begin, end);
      } else
         return decode(boost::describe::describe_members<T, boost::describe::mod_public>{}, begin, end);
   }

 private:
   template <typename Descriptor, typename Sizeof>
   auto decode_member(byte_t const* begin) const {
      return member_decoder<Descriptor::pointer, Options>{}(begin, begin + Sizeof::value);
   }

   template <template <class...> class L, typename... Descriptor, typename... Offset, typename... Sizeof>
   T decode(L<Descriptor...>, boost::mp11::mp_list<Offset...>, boost::mp11::mp_list<Sizeof...>, byte_t const*& begin, [[maybe_unused]] byte_t const* end) {
      constexpr auto size = boost::mp11::mp_back<boost::mp11::mp_list<Offset...>>::value + boost::mp11::mp_back<boost::mp11::mp_list<Sizeof...>>::value;
      assert(size <= std::distance(begin, end));
      auto tmp = begin;
      begin += size;
      return {
          decode_member<Descriptor, Sizeof>(tmp + Offset::value)...,
      };
   }

   template <template <class...> class L, typename... Descriptor, typename... P>
   T decode(L<Descriptor...>, byte_t const*& begin, byte_t const* end, P&&... parts) {
      auto constexpr n = sizeof...(P);
      if constexpr (n == sizeof...(Descriptor))
         return {std::forward<P>(parts)...};
      else {
         using D = boost::mp11::mp_at_c<L<Descriptor...>, n>;
         return decode(L<Descriptor...>{}, begin, end, std::forward<P>(parts)..., member_decoder<D::pointer, Options>{}(begin, end));
      }
   }
};

}} // namespace tc::v1
