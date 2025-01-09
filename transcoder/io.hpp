#pragma once
#include "flyweight.hpp"
#include "type_name.hpp"
#include "detail/sint.hpp"
#include <boost/mp11/algorithm.hpp>
#include <boost/describe.hpp>
#include <array>
#include <ranges>
#include <iostream>
#include <type_traits>

namespace tc { inline namespace v1 { namespace io {

template<typename T, typename Enable = std::true_type>
struct formatter {
    std::ostream& operator()(std::ostream& out, T const& val) const {
        out << val;
        return out;
    }
};

template<typename T, std::size_t N>
struct formatter<std::array<T, N>, typename std::is_integral<T>::type> {
    std::ostream& operator()(std::ostream& out, std::array<T, N> const& val) const {
        std::ranges::copy(val, std::ostream_iterator<typename tc::detail::simplified_integer_type<T>::type>{ out });
        return out;
    }
};

template<std::size_t N>
struct formatter<std::array<char, N>, std::true_type> {
    std::ostream& operator()(std::ostream& out, std::array<char, N> const& val) const {
        std::ranges::copy(val, std::ostream_iterator<char>{ out });
        return out;
    }
};

template<typename T>
struct formatter<tc::flyweight<T>, std::true_type> {
    std::ostream& operator()(std::ostream& out, tc::flyweight<T> const& val) const {
        return formatter<T>{}(out, static_cast<T>(val));
    }
};

template<auto> struct mem_formatter;
template<typename T, typename U, U T::* P> struct mem_formatter<P> : formatter<U> {};

template<typename T>
struct formatter<T, typename boost::describe::has_describe_members<T>::type> {
    std::ostream& operator()(std::ostream& out, T const& value) const {
        out << type_name<T>{}() << "{\n";
        using M = boost::describe::describe_members<T, boost::describe::mod_public>;
        boost::mp11::mp_for_each<M>([&out, &value](auto member) {
            out << '\t' << member.name << ':';
            mem_formatter<decltype(member)::pointer>{}(out, value.*member.pointer) << '\n';
        });
        out << "}\n";
        return out;
    }
};

template<typename T>
std::ostream& dump(std::ostream& out, T const& value) {
    return io::formatter<T>{}(out, value);
}

} // namespace io



} }


