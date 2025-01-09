#pragma once
#include "../basic_types.hpp"
#include "../traits.hpp"
#include "sizeof.hpp"
#include "utils.hpp"
#include "sint.hpp"
#include "byte_order.hpp"

#include <boost/describe.hpp>
#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/tuple.hpp>
#include <boost/endian.hpp>
#include <array>
#include <span>
#include <tuple>
#include <bit>
#include <iterator>
#include <type_traits>
#include <cassert>

namespace tc { inline namespace v1 {

template <typename T, typename Options>
struct encoder<T, Options, utils::trivially_encodable<T>> {
   byte_t* operator()(T const& value, byte_t* begin, [[maybe_unused]] byte_t* end) noexcept {
      constexpr auto encoded_size = encoded_sizeof<T, Options>::value;
      static_assert(encoded_size == sizeof(T), "seems tc::encoded_sizeof specialized incorrectly");
      assert(std::distance(begin, end) >= encoded_size);
      constexpr auto byte_order = flag::byte_order_from_options<Options, default_byte_order()>;
      if constexpr (sizeof(T) == sizeof(char) || byte_order == boost::endian::order::native) {
         std::memcpy(begin, &value, encoded_size);
      } else {
         auto tmp = std::bit_cast<typename detail::simplified_integer_type<T>::type>(value);
         boost::endian::conditional_reverse_inplace<boost::endian::order::native, byte_order>(tmp);
         std::memcpy(begin, &tmp, encoded_size);
      }
      return std::next(begin, encoded_size);
   }
};

template <std::size_t N, typename Options>
struct encoder<padding<N>, Options, std::true_type> {
   byte_t* operator()(padding<N> const&, byte_t* begin, [[maybe_unused]] byte_t* end) noexcept {
      constexpr auto encoded_size = encoded_sizeof<padding<N>, Options>::value;
      static_assert(encoded_size == N, "seems tc::encoded_sizeof specialized incorrectly");
      assert(std::distance(begin, end) >= encoded_size);
      return begin + encoded_size;
   }
};

namespace detail {

template <class T, std::size_t N, typename Options>
byte_t* encode_span(std::span<T, N> value, byte_t* begin, byte_t* end) noexcept {
   if constexpr (sizeof(T) == sizeof(char) || flag::byte_order_from_options<Options, default_byte_order()> == boost::endian::order::native) {
      constexpr auto encoded_size = encoded_sizeof<T, Options>::value * N;
      assert(std::distance(begin, end) >= encoded_size);
      std::memcpy(begin, value.data(), value.size() * sizeof(T));
      return std::next(begin, encoded_size);
   } else {
      //"TODO:encode at once to certain size";
      for (auto const& x : value)
         begin = encoder<T, Options>{}(x, begin, end);
      return begin;
   }
}
} // namespace detail

template <typename T, std::size_t N, typename Options>
struct encoder<std::array<T, N>, Options, utils::trivially_encodable<T>> {
   byte_t* operator()(std::array<T, N> const& value, byte_t* begin, byte_t* end) noexcept {
      return detail::encode_span<T const, N, Options>(value, begin, end);
   }
};

template <typename T, std::size_t N, typename Options>
struct encoder<T[N], Options, utils::trivially_encodable<T>> {
   byte_t* operator()(const T (&value)[N], byte_t* begin, byte_t* end) noexcept {
      return detail::encode_span<T const, N, Options>(value, begin, end);
   }
};

// template <typename T, std::size_t N, typename Options>
// struct encoder<T[N], Options, utils::trivially_encodable<T>> {
//    byte_t* operator()(T const* value, byte_t* begin, byte_t* end) noexcept  {
//       return detail::encode_span<T const, N, Options>(value, begin, end);
//    }
// };

template <typename... T, typename Options>
struct encoder<std::tuple<T...>, Options, typename std::conjunction<utils::trivially_encodable<T>...>::type> {
   byte_t* operator()(std::tuple<T...> const& value, byte_t* begin, byte_t* end) noexcept {
      [[maybe_unused]] static constexpr auto encoded_size = encoded_sizeof<std::tuple<T...>, Options>::value;
      assert(std::distance(begin, end) >= encoded_size);
      boost::mp11::tuple_for_each(value, [&begin, end](auto member) {
         assert(begin < end);
         begin = encoder<decltype(member), Options>{}(member, begin, end);
      });
      return begin;
   }
};

template <typename T, typename Options>
struct encoder<T, Options, typename std::conjunction<boost::describe::has_describe_members<T>, std::negation<std::is_same<typename describe::encoded_structure_sizeof<T, Options>::type, tc::not_encodable>>>::type> {
   byte_t* operator()(T const& value, byte_t* begin, byte_t* end) noexcept {
      // TODO:throw for dynamic_size maybe?
      assert((std::distance(begin, end) >= utils::encoded_sizeof_adapter<T, Options>{}(value)));
      boost::mp11::mp_for_each<boost::describe::describe_members<T, boost::describe::mod_public>>([&](auto member) {
         assert(begin < end);
         begin = member_encoder<decltype(member)::pointer, Options>{}(value.*member.pointer, begin, end);
      });
      return begin;
   }
};

}} // namespace tc::v1
