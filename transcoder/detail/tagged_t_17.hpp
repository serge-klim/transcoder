#pragma once
#include <boost/operators.hpp>
#include <type_traits>
#include <cstdint>

namespace tc { inline namespace v1 {

template <typename T, typename Tag>
struct tagged_t
    : boost::totally_ordered1<tagged_t<T, Tag>, boost::totally_ordered2<tagged_t<T, Tag>, T>> {
   T value;
   static_assert(std::is_trivial<T>::value, "T expected to be trivial");
   tagged_t() = default;
   explicit constexpr tagged_t(T v) noexcept : value(v) {}
   constexpr operator const T&() const noexcept { return value; }
   constexpr operator T&() { return value; }
   bool operator==(tagged_t const& other) const noexcept { return value == other.value; }
   bool operator<(tagged_t const& other) const noexcept { return value < other.value; }
};

}} // namespace tc::v1
