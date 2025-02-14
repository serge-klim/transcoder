#pragma once
#include <string>
#include <stdexcept>
#include <cstddef>
#include <type_traits>

namespace tc { inline namespace v1 {

struct unknown_type {};
struct more_wanted { std::size_t bytes; };

struct more_wanted_error : std::length_error {
	more_wanted_error(std::size_t bytes) 
		: std::length_error{"not enough data provided to deduce type," + std::to_string(bytes) + " more bytes needed"}, bytes_{bytes} {}
	constexpr std::size_t bytes() const noexcept { return bytes_; }
private:
	std::size_t bytes_;
};

struct unknown_type_error : std::runtime_error {
	unknown_type_error() : std::runtime_error("unknown type"){}
};
inline constexpr std::false_type handle_error(auto const& v) noexcept { return {}; }
[[noreturn]] inline std::true_type handle_error(more_wanted const& v) { throw more_wanted_error{v.bytes}; }
[[noreturn]] inline std::true_type handle_error(unknown_type const&) { throw unknown_type_error{}; }

} }


