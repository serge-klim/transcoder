#include "dummy.hpp"
#include "transcoder/detail/utils/sizeof.hpp"
#include "transcoder/detail/describe.hpp"
#include "transcoder/transcoder.hpp"
#include <boost/describe.hpp>
#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/utility.hpp>
#include <boost/test/unit_test.hpp>
#include <tuple>
#include <array>
#include <type_traits>
#include <cstdint>

struct dummy_options;
struct not_encodable_type {};
struct dynamic_size_encodable {};
struct explicit_dynamic_size_encodable {};
struct explicit_member_dynamic_size_encodable {};

struct dummy_compound_not_encodable {
   dynamic_size_encodable dynamic_size_encodable0;
   not_encodable_type not_encodable_type1;
   explicit_member_dynamic_size_encodable explicit_member_dynamic_size_encodable2;
   char c3;
   int i4;
   std::uint64_t ui5;
   std::uint64_t ui_explicit_sizeof6;
};

BOOST_DESCRIBE_STRUCT(dummy_compound_not_encodable, (),
                      (
                          dynamic_size_encodable0,
                          not_encodable_type1,
                          explicit_member_dynamic_size_encodable2,
                          c3,
                          i4,
                          ui5,
                          ui_explicit_sizeof6))

struct dummy_compound_dynamic_size_encodable {
   dynamic_size_encodable dynamic_size_encodable0;
   // explicit_member_dynamic_size_encodable explicit_member_dynamic_size_encodable1;
   char c2;
};

BOOST_DESCRIBE_STRUCT(dummy_compound_dynamic_size_encodable, (),
                      (
                          dynamic_size_encodable0,
                          //                          explicit_member_dynamic_size_encodable1,
                          c2))

template <typename Options>
struct tc::encoder<dynamic_size_encodable, Options, std::true_type> {
   byte_t* operator()(dynamic_size_encodable const& value, byte_t* begin, byte_t* end) noexcept;
};

template <typename Options>
struct tc::member_encoder<&dummy_compound_not_encodable::explicit_member_dynamic_size_encodable2, Options> {
   byte_t* operator()(explicit_member_dynamic_size_encodable const& value, byte_t* begin, byte_t* end) noexcept;
};

template <typename Options>
struct tc::encoded_sizeof<struct explicit_dynamic_size_encodable, Options> {
   //TODO:profile what impact does this have on compilation time!
   //using type = tc::dynamic_size;
};

template <typename Options>
struct tc::encoder<explicit_dynamic_size_encodable, Options, std::true_type> {
   byte_t* operator()(explicit_dynamic_size_encodable const& value, byte_t* begin, byte_t* end) noexcept;
};

template <typename Options>
struct tc::decoder<explicit_dynamic_size_encodable, Options, std::true_type> {
   byte_t* operator()(explicit_dynamic_size_encodable const& value, byte_t* begin, byte_t* end) noexcept;
};

template <typename Options>
struct tc::encoded_member_sizeof<&dummy_compound_not_encodable::ui_explicit_sizeof6, Options, std::true_type> : encoded_sizeof<std::uint64_t, Options> {};

BOOST_AUTO_TEST_SUITE(transcoder_traits_test_sute)

  BOOST_AUTO_TEST_CASE(eval_member_sizeof_test) {

    using public_members = boost::describe::describe_members<dummy_compound_not_encodable, boost::describe::mod_public>;

    static_assert(std::is_same_v<tc::describe::eval_member_sizeof<dummy_options>::template fn<boost::mp11::mp_at_c<public_members, 0>>, tc::dynamic_size>, "eval_member_sizeof::fn seems broken");
    static_assert(std::is_same_v<tc::describe::eval_member_sizeof<dummy_options>::template fn<boost::mp11::mp_at_c<public_members, 1>>, tc::not_encodable>, "eval_member_sizeof::fn seems broken");
    static_assert(std::is_same_v<tc::describe::eval_member_sizeof<dummy_options>::template fn<boost::mp11::mp_at_c<public_members, 2>>, tc::dynamic_size>, "eval_member_sizeof::fn seems broken");
    static_assert(!std::is_same_v<tc::describe::eval_member_sizeof<dummy_options>::template fn<boost::mp11::mp_at_c<public_members, 3>>, tc::dynamic_size>, "eval_member_sizeof::fn seems broken");
    static_assert(!std::is_same_v<tc::describe::eval_member_sizeof<dummy_options>::template fn<boost::mp11::mp_at_c<public_members, 3>>, tc::dynamic_size>, "eval_member_sizeof::fn seems broken");
    static_assert(std::is_same_v<typename tc::describe::eval_member_sizeof<dummy_options>::template fn<boost::mp11::mp_at_c<public_members, 3>>::value_type, std::size_t>, "eval_member_sizeof::fn seems broken");
    BOOST_CHECK(true);
 }

 BOOST_AUTO_TEST_CASE(can_encode_static_test) {
   static_assert(!tc::utils::can_encode<tc::encoder<not_encodable_type, dummy_options>, not_encodable_type>::value, "tc::utils::is_encoder seems broken");
   static_assert(!tc::utils::can_encode<tc::encoder<not_encodable_type, dummy_options>, int>::value, "tc::utils::is_encoder seems broken");
   static_assert(tc::utils::can_encode<tc::encoder<dynamic_size_encodable, dummy_options>, dynamic_size_encodable>::value, "tc::utils::is_encoder seems broken");
   static_assert(!tc::utils::can_encode<tc::encoder<dynamic_size_encodable, dummy_options>, int>::value, "tc::utils::is_encoder seems broken");

   static_assert(tc::utils::can_encode<tc::member_encoder<&dummy_compound_not_encodable::i4, dummy_options>, int>::value, "tc::utils::is_encoder seems broken");
   static_assert(!tc::utils::can_encode<tc::member_encoder<&dummy_compound_not_encodable::dynamic_size_encodable0, dummy_options>, not_encodable_type>::value, "tc::utils::is_encoder seems broken");
   static_assert(!tc::utils::can_encode<tc::member_encoder<&dummy_compound_not_encodable::not_encodable_type1, dummy_options>, int>::value, "tc::utils::is_encoder seems broken");
   static_assert(tc::utils::can_encode<tc::member_encoder<&dummy_compound_not_encodable::explicit_member_dynamic_size_encodable2, dummy_options>, explicit_member_dynamic_size_encodable>::value, "tc::utils::is_encoder seems broken");
   BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(can_decode_static_test) {
   static_assert(tc::utils::can_decode<tc::decoder<int, dummy_options>, int>::value, "tc::utils::can_decode seems broken");
   static_assert(!tc::utils::can_decode<tc::decoder<not_encodable_type, dummy_options>, not_encodable_type>::value, "tc::utils::can_decode seems broken");
   static_assert(!tc::utils::can_decode<tc::decoder<not_encodable_type, dummy_options>, int>::value, "tc::utils::can_decode seems broken");
   static_assert(!tc::utils::can_decode<tc::decoder<dynamic_size_encodable, dummy_options>, dynamic_size_encodable>::value, "tc::utils::can_decode seems broken");
   static_assert(!tc::utils::can_decode<tc::decoder<dynamic_size_encodable, dummy_options>, int>::value, "tc::utils::can_decode seems broken");
   static_assert(!tc::utils::has_decoder<dummy::var_size, dummy_options>::value, "tc::utils::has_decoder seems broken");
   
   BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(has_encoder_static_test) {

   static_assert(!tc::utils::has_encoder<not_encodable_type, dummy_options>::value, "tc::utils::has_encoder seems broken");
   static_assert(tc::utils::has_encoder<dynamic_size_encodable, dummy_options>::value, "tc::utils::has_encoder seems broken");
   static_assert(tc::utils::has_encoder<char, dummy_options>::value, "char encoder seems broken");
   static_assert(tc::utils::has_encoder<int, dummy_options>::value, "int encoder seems broken");
   static_assert(tc::utils::has_encoder<std::uint64_t, dummy_options>::value, "std::uint64_t encoder seems broken");
   static_assert(tc::utils::has_encoder<std::array<std::uint64_t, 5>, dummy_options>::value, "std::array<std::uint64_t, 5> encoder seems broken");
   static_assert(!tc::utils::has_encoder<dummy::var_size, dummy_options>::value, "tc::utils::encoder seems broken");
   BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(has_decoder_static_test) {

   static_assert(!tc::utils::has_decoder<not_encodable_type, dummy_options>::value, "tc::utils::has_encoder seems broken");
   static_assert(!tc::utils::has_decoder<dynamic_size_encodable, dummy_options>::value, "tc::utils::has_encoder seems broken");
   static_assert(tc::utils::has_decoder<char, dummy_options>::value, "char encoder seems broken");
   static_assert(tc::utils::has_decoder<int, dummy_options>::value, "int encoder seems broken");
   static_assert(tc::utils::has_decoder<std::uint64_t, dummy_options>::value, "std::uint64_t encoder seems broken");
   static_assert(tc::utils::has_decoder<std::array<std::uint64_t, 5>, dummy_options>::value, "std::array<std::uint64_t, 5> encoder seems broken");

   BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(encoded_sizeof_static_test) {
   static_assert(std::is_same_v<typename tc::encoded_sizeof<int, dummy_options>::type, std::integral_constant<std::size_t, sizeof(int)>>, "tc::encoded_sizeof seems broken");
   static_assert(std::is_same_v<typename tc::encoded_sizeof<short , dummy_options>::type, std::integral_constant<std::size_t, sizeof(short)>>, "tc::encoded_sizeof seems broken");
   static_assert(std::is_same_v<typename tc::encoded_sizeof<unsigned char, dummy_options>::type, std::integral_constant<std::size_t, sizeof(unsigned char)>>, "tc::encoded_sizeof seems broken");
   static_assert(std::is_same_v<typename tc::encoded_sizeof<dummy_compound_dynamic_size_encodable, dummy_options>::type, tc::dynamic_size>, "tc::encoded_sizeof seems broken");

   // TODO: temporary(platform specifics) remove after testing
   static_assert(std::is_same_v<typename tc::encoded_sizeof<dummy::type1, dummy_options>::type, std::integral_constant<std::size_t, 35>>, "tc::encoded_sizeof seems broken");
   // BOOST_CHECK_EQUAL((tc::encoded_sizeof<dummy::type1, dummy_options>::value), 0);
   BOOST_CHECK_LE((tc::encoded_sizeof<dummy::type1, dummy_options>::value), sizeof(dummy::type1));
   BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(encoded_size_static_test) {
   static_assert(tc::encoded_sizeof<int, dummy_options>::value == tc::encoded_size<int>(), "tc::encoded_sizeof seems broken");
   static_assert(tc::encoded_sizeof<short, dummy_options>::value == tc::encoded_size<short>(), "tc::encoded_sizeof seems broken");
   static_assert(tc::encoded_sizeof<unsigned char, dummy_options>::value == tc::encoded_size<unsigned char>(), "tc::encoded_sizeof seems broken");
   static_assert(tc::encoded_sizeof<dummy::type1, dummy_options>::value == tc::encoded_size<dummy::type1>());
   BOOST_CHECK(true);
}


BOOST_AUTO_TEST_SUITE_END()
