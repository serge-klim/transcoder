#pragma once
#include <type_traits>
#include <concepts>

namespace tc { inline namespace v1 {

template <typename T, typename Tag>
requires std::is_trivial_v<T>
struct tagged_t {
   T value;
   constexpr operator const T&() const noexcept { return value; }
   constexpr operator T&() { return value; }
   auto operator<=>(tagged_t const&) const = default;
};

}} // namespace tc::v1
