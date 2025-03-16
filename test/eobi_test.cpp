#include "eobi_eg.hpp"
#include "transcoder/transcoder.hpp"
#include "transcoder/type_name.hpp"
#include <boost/mp11/algorithm.hpp>
#include <boost/test/unit_test.hpp>
#include <cstring>
#include <array>
#include <numeric>
#include <type_traits>

BOOST_AUTO_TEST_SUITE(eobi_test_sute)

BOOST_AUTO_TEST_CASE_TEMPLATE(encode_decode_message_test, message, eobi_test::messages) {
    using deduced_options = tc::effective_options<message>;
    //static_assert(boost::mp11::mp_find<deduced_options, nasdaq::itch::v5_0::tag>::value != boost::mp11::mp_size<deduced_options>::value, "oops! tagging seems broken");
    auto data = std::array<tc::byte_t, 1500>{};
    std::iota(begin(data), end(data), 11);
    auto id = tc::type_id<message>{}();
    std::memcpy(&reinterpret_cast<message*>(data.data())->MessageHeader.TemplateID, &id, sizeof(id));

    auto begin = data.data();
    //auto decoded_variant = tc::decode<nasdaq::itch::v5_0::messages>(begin, begin + data.size());
    auto decoded_variant = tc::decode<eobi_test::messages>(begin, begin + data.size());
	std::visit([&data](auto const& msg) {
		if constexpr (!std::is_same_v<decltype(msg), message const&>) {
            BOOST_CHECK_MESSAGE(false, "wrong type has been decoded: " << tc::type_name<decltype(msg)>{}() << " instead of " << tc::type_name<message>{}());
		}
        auto out = tc::encode<message>(msg);
        BOOST_REQUIRE_LE(out.size(), data.size());
        auto begin = data.data();
        BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(out), cend(out), begin, begin + out.size());
        auto begin_out = out.data();
        [[maybe_unused]] auto decoded = tc::decode<message>(begin_out, begin_out + out.size());
        BOOST_CHECK_EQUAL(std::distance(out.data(), begin_out), out.size());
	}, decoded_variant);
}


BOOST_AUTO_TEST_SUITE_END()
