#pragma once
#include <boost/type_index.hpp>
#include <string>
#include <type_traits>
#include <cstddef>

namespace tc { inline namespace v1 {

template<typename T>
struct type_name {
	std::string operator()() {
		return boost::typeindex::type_id<T>().pretty_name();
	}
};

} }


