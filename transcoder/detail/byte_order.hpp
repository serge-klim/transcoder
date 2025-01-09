#pragma once
#include "../byte_order.hpp"
#include <type_traits>
#include <boost/endian.hpp>
#include <boost/mp11/algorithm.hpp>


namespace tc { inline namespace v1 { namespace flag { namespace detail {

template <typename T>
struct is_byte_order : std::false_type{};

template <boost::endian::order Order>
struct is_byte_order<tc::v1::flag::byte_order<Order>> : std::true_type{};


template <typename T, boost::endian::order DefaultOrder>
struct select_byte_order {
   static constexpr boost::endian::order value = DefaultOrder;
};

template <template <class...> class L, typename boost::endian::order DefaultOrder>
struct select_byte_order<L<>, DefaultOrder> {
   static constexpr boost::endian::order value = DefaultOrder;
};

template <template <class...> class L, typename boost::endian::order Order, typename boost::endian::order DefaultOrder>
struct select_byte_order<L<byte_order<Order>>, DefaultOrder>
{
   static constexpr boost::endian::order value = Order;
};

template <template <class...> class L, class... T, typename boost::endian::order Order>
struct select_byte_order<L<T...>, Order>
{
   template<typename ...Invalid>
   struct more_then_one_byte_order_flag_has_been_found;
   static constexpr boost::endian::order value = more_then_one_byte_order_flag_has_been_found<T...>::value;
};

} // namespace detail

 template <typename Options, boost::endian::order DefaultByteOrder = boost::endian::order::native>
 static constexpr boost::endian::order byte_order_from_options =
     detail::select_byte_order<boost::mp11::mp_copy_if<Options, detail::is_byte_order>, DefaultByteOrder>::value;

}}}


