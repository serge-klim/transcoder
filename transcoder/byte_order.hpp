#pragma once
#include <type_traits>
#include <boost/endian.hpp>

namespace tc { inline namespace v1 { namespace flag {

template <boost::endian::order Order>
using byte_order = std::integral_constant<boost::endian::order, Order>;

using big_endian = byte_order<boost::endian::order::big>;
using little_endian = byte_order<boost::endian::order::little>;
using native_endian = byte_order<boost::endian::order::native>;

}} // namespace v1

constexpr boost::endian::order default_byte_order() noexcept { return boost::endian::order::big; }

} // namespace tc


