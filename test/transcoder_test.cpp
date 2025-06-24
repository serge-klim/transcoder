#define BOOST_TEST_MODULE transcoder_tests
#include "byte_order.hpp"
#include "dummy.hpp"
#include "transcoder/flyweight.hpp"
#include "transcoder/options.hpp"
#include "transcoder/transcoder.hpp"
#include <bit>
#include <boost/endian.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdint>
#include <tuple>
#include <type_traits>

template <>
struct tc::type_id<dummy::type1> {
   constexpr char operator()() const noexcept { return dummy::type1{}.type; }
};

template <>
struct tc::type_id<dummy::arrays_type> {
   constexpr char operator()() const noexcept { return dummy::arrays_type{}.type; }
};

template <>
struct tc::type_id<dummy::nested_type> {
   constexpr char operator()() const noexcept { return dummy::nested_type{}.type; }
};

// template <typename Options>
// struct tc::encoded_sizeof<dummy::var_size, Options, std::true_type> : encoded_sizeof<char, Options> {};

template <typename Options>
struct tc::encoder<dummy::var_size, Options, std::true_type> {
   byte_t* operator()(dummy::var_size const&, byte_t* begin, byte_t* end) noexcept {
      ++begin;
      return begin;
   }
};

BOOST_AUTO_TEST_SUITE(transcoder_test_sute)

BOOST_AUTO_TEST_CASE(static_test) {
   static_assert(!tc::flag::detail::is_byte_order<int>::value, "wrong endianness deduced!");
   static_assert(!tc::flag::detail::is_byte_order<std::integral_constant<int, 0>>::value, "wrong endianness deduced!");
   static_assert(tc::flag::detail::is_byte_order<tc::flag::byte_order<boost::endian::order::big>>::value, "wrong endianness deduced!");

   static_assert(tc::flag::detail::select_byte_order<int, boost::endian::order::native>::value == boost::endian::order::native, "wrong endianness deduced!");
   static_assert(tc::flag::detail::select_byte_order<boost::mp11::mp_list<tc::flag::byte_order<boost::endian::order::native>>, boost::endian::order::native>::value == boost::endian::order::native, "wrong endianness deduced!");
   static_assert(tc::flag::detail::select_byte_order<boost::mp11::mp_list<tc::flag::byte_order<boost::endian::order::big>>, boost::endian::order::native>::value == boost::endian::order::big, "wrong endianness deduced!");

   static_assert(std::is_same_v<tc::options<int, double, float>, tc::options<double, int, float>>, "wrong endianness deduced!");

   static_assert(tc::describe::encoded_member_offset<&dummy::type1::type, tc::options<>>::type::value == 0, "seems tc::detail::encoded_member_offset is broken");
   static_assert(tc::describe::encoded_member_offset<&dummy::type1::i3, tc::options<>>::type::value == sizeof(dummy::type1::type) + sizeof(dummy::type1::s1) + sizeof(dummy::type1::ll2), "seems tc::detail::encoded_member_offset is broken");
}

BOOST_AUTO_TEST_CASE(simply_encode_decode_test) {
   auto in = 1;
   auto res = tc::encode(in);
   BOOST_CHECK_EQUAL(res.size(), sizeof(in));
   auto tmp = *reinterpret_cast<decltype(in) const*>(res.data());
   tmp = boost::endian::conditional_reverse<tc::default_byte_order(), boost::endian::order::native>(tmp);
   BOOST_CHECK_EQUAL(tmp, in);
   auto begin = res.data();
   auto out = tc::decode<decltype(in)>(begin, begin + res.size());
   BOOST_CHECK_EQUAL(in, out);
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(endianness_encode_decode_test) {
   auto in = 1;
   auto res = tc::encode<tc::flag::native_endian>(in);
   BOOST_CHECK_EQUAL(res.size(), sizeof(in));
   BOOST_CHECK_EQUAL(*reinterpret_cast<decltype(in) const*>(res.data()), in);
   auto begin = res.data();
   auto out = tc::decode<decltype(in)>(begin, begin + res.size());
   BOOST_CHECK_NE(in, out);
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
   auto reversed = boost::endian::conditional_reverse<tc::default_byte_order(), boost::endian::order::native>(out);
   BOOST_CHECK_EQUAL(reversed, in);
   /*auto*/ begin = res.data();
   auto out1 = tc::decode<decltype(in), tc::flag::native_endian>(begin, begin + res.size());
   BOOST_CHECK_EQUAL(out1, in);
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(dummy_type1_encode_decode_test) {
   auto in = dummy::type1{'1', 1, 0xf00002, 3, 4, 5, 6009};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   auto out = tc::decode<decltype(in)>(begin, begin + res.size());
   BOOST_CHECK_EQUAL(in.s1, out.s1);
   BOOST_CHECK_EQUAL(in.ll2, out.ll2);
   BOOST_CHECK_EQUAL(in.i3, out.i3);
   BOOST_CHECK_EQUAL(in.f4, out.f4);
   BOOST_CHECK_EQUAL(in.d5, out.d5);
   BOOST_CHECK_EQUAL(in.d6, out.d6);
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(dummy_chrono_encode_decode_test) {
   auto in = dummy::time{std::chrono::nanoseconds{111}, std::chrono::duration<std::uint64_t, std::nano>{333}};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   auto out = tc::decode<decltype(in)>(begin, begin + res.size());
   BOOST_CHECK_EQUAL(in.ns, out.ns);
   BOOST_CHECK_EQUAL(in.nasdaq_time, out.nasdaq_time);
}

BOOST_AUTO_TEST_CASE(dummy_type1_encode_decode_as_tuple_test) {
   auto in = dummy::type1{'1', 1, 0xf00002, 3, 4, 5, 6009};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   using tuple = std::tuple<
       char,
       short,
       long long,
       int,
       float,
       double,
       /*long*/ double>;
   auto out = tc::decode<tuple>(begin, begin + res.size());
   BOOST_CHECK_EQUAL(in.type, std::get<0>(out));
   BOOST_CHECK_EQUAL(in.s1, std::get<1>(out));
   BOOST_CHECK_EQUAL(in.ll2, std::get<2>(out));
   BOOST_CHECK_EQUAL(in.i3, std::get<3>(out));
   BOOST_CHECK_EQUAL(in.f4, std::get<4>(out));
   BOOST_CHECK_EQUAL(in.d5, std::get<5>(out));
   BOOST_CHECK_EQUAL(in.d6, std::get<6>(out));
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(dummy_type1_encode_as_tuple_decode_as_type_test) {
   using tuple = std::tuple<
       char,
       short,
       long long,
       int,
       float,
       double,
       /*long*/ double>;

   auto in = tuple{'1', 1, 0xf00002ll, 3, 4.f, 5., 6006.};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   auto out = tc::decode<dummy::type1>(begin, begin + res.size());
   BOOST_CHECK_EQUAL(std::get<0>(in), out.type);
   BOOST_CHECK_EQUAL(std::get<1>(in), out.s1);
   BOOST_CHECK_EQUAL(std::get<2>(in), out.ll2);
   BOOST_CHECK_EQUAL(std::get<3>(in), out.i3);
   BOOST_CHECK_EQUAL(std::get<4>(in), out.f4);
   BOOST_CHECK_EQUAL(std::get<5>(in), out.d5);
   BOOST_CHECK_EQUAL(std::get<6>(in), out.d6);
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(dummy_type2_encode_decode_test) {
   auto in = dummy::arrays_type{'2', {11, 12, 13}, {0xf00011, 0xf00012, 0xf00013, 0xf00014, 0xf00015}, {3, 4, 5, 6, 7, 8, 9}, {4.4f, 5.5f, 5.6f}, {5.005, 6.006, 7.007, 8.008, 9.009}, {6001, 6002, 6003}};

   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   auto out = tc::decode<decltype(in)>(begin, begin + res.size());
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.sa1), cend(in.sa1), cbegin(out.sa1), cend(out.sa1));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.lla2), cend(in.lla2), cbegin(out.lla2), cend(out.lla2));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.ia3), cend(in.ia3), cbegin(out.ia3), cend(out.ia3));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.fa4), cend(in.fa4), cbegin(out.fa4), cend(out.fa4));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.da5), cend(in.da5), cbegin(out.da5), cend(out.da5));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.da6), cend(in.da6), cbegin(out.da6), cend(out.da6));
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(dummy_c_arrays_type_encode_decode_test) {
   auto in = dummy::c_arrays_type{'3', {11, 12, 13}, {0xf00011, 0xf00012, 0xf00013, 0xf00014, 0xf00015}, {3, 4, 5, 6, 7, 8, 9}, {4.4f, 5.5f, 5.6f}, {5.005, 6.006, 7.007, 8.008, 9.009}, {6001, 6002, 6003}};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   // auto out = tc::decode<decltype(in)>(begin, begin + res.size());
   auto out = tc::decode<dummy::arrays_type>(begin, begin + res.size());
   BOOST_CHECK_EQUAL_COLLECTIONS(in.sa1, in.sa1 + std::ssize(in.sa1), cbegin(out.sa1), cend(out.sa1));
   BOOST_CHECK_EQUAL_COLLECTIONS(in.lla2, in.lla2 + std::size(in.lla2), cbegin(out.lla2), cend(out.lla2));
   BOOST_CHECK_EQUAL_COLLECTIONS(in.ia3, in.ia3 + std::ssize(in.ia3), cbegin(out.ia3), cend(out.ia3));
   BOOST_CHECK_EQUAL_COLLECTIONS(in.fa4, in.fa4 + std::ssize(in.fa4), cbegin(out.fa4), cend(out.fa4));
   BOOST_CHECK_EQUAL_COLLECTIONS(in.da5, in.da5 + std::ssize(in.da5), cbegin(out.da5), cend(out.da5));
   BOOST_CHECK_EQUAL_COLLECTIONS(in.da6, in.da6 + std::ssize(in.da6), cbegin(out.da6), cend(out.da6));
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(dummy_nested_type_encode_decode_test) {
   auto in = dummy::nested_type{
       '4',
       {'1', 1, 0xf00002, 3, 4, 5, 6006},
       {111111, 2222222, 333333},
       {'2',
        {11, 12, 13},
        {0xf00011, 0xf00012, 0xf00013, 0xf00014, 0xf00015},
        {3, 4, 5, 6, 7, 8, 9},
        {4.4f, 5.5f, 5.6f},
        {5.005, 6.006, 7.007, 8.008, 9.009},
        {6001, 6002, 6003}}};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   auto out = tc::decode<decltype(in)>(begin, begin + res.size());
   BOOST_CHECK_EQUAL(in.simple.s1, out.simple.s1);
   BOOST_CHECK_EQUAL(in.simple.ll2, out.simple.ll2);
   BOOST_CHECK_EQUAL(in.simple.i3, out.simple.i3);
   BOOST_CHECK_EQUAL(in.simple.f4, out.simple.f4);
   BOOST_CHECK_EQUAL(in.simple.d5, out.simple.d5);
   BOOST_CHECK_EQUAL(in.simple.d6, out.simple.d6);

   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.lla), cend(in.lla), cbegin(out.lla), cend(out.lla));

   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.sa1), cend(in.at.sa1), cbegin(out.at.sa1), cend(out.at.sa1));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.lla2), cend(in.at.lla2), cbegin(out.at.lla2), cend(out.at.lla2));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.ia3), cend(in.at.ia3), cbegin(out.at.ia3), cend(out.at.ia3));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.fa4), cend(in.at.fa4), cbegin(out.at.fa4), cend(out.at.fa4));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.da5), cend(in.at.da5), cbegin(out.at.da5), cend(out.at.da5));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.da6), cend(in.at.da6), cbegin(out.at.da6), cend(out.at.da6));
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(dummy_nested_type_decode_from_set_test) {
   auto in = dummy::nested_type{
       '4',
       {'1', 1, 0xf00002, 3, 4, 5, 6006},
       {111111, 2222222, 333333},
       {'2',
        {11, 12, 13},
        {0xf00011, 0xf00012, 0xf00013, 0xf00014, 0xf00015},
        {3, 4, 5, 6, 7, 8, 9},
        {4.4f, 5.5f, 5.6f},
        {5.005, 6.006, 7.007, 8.008, 9.009},
        {6001, 6002, 6003}}};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   tc::decode<dummy::messages>(begin, begin + res.size(), [&](auto const& value) {
      if constexpr (std::is_same_v<decltype(value), dummy::nested_type const&>) {
         BOOST_CHECK_EQUAL(in.simple.s1, value.simple.s1);
         BOOST_CHECK_EQUAL(in.simple.ll2, value.simple.ll2);
         BOOST_CHECK_EQUAL(in.simple.i3, value.simple.i3);
         BOOST_CHECK_EQUAL(in.simple.f4, value.simple.f4);
         BOOST_CHECK_EQUAL(in.simple.d5, value.simple.d5);
         BOOST_CHECK_EQUAL(in.simple.d6, value.simple.d6);

         BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.lla), cend(in.lla), cbegin(value.lla), cend(value.lla));

         BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.sa1), cend(in.at.sa1), cbegin(value.at.sa1), cend(value.at.sa1));
         BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.lla2), cend(in.at.lla2), cbegin(value.at.lla2), cend(value.at.lla2));
         BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.ia3), cend(in.at.ia3), cbegin(value.at.ia3), cend(value.at.ia3));
         BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.fa4), cend(in.at.fa4), cbegin(value.at.fa4), cend(value.at.fa4));
         BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.da5), cend(in.at.da5), cbegin(value.at.da5), cend(value.at.da5));
         BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.da6), cend(in.at.da6), cbegin(value.at.da6), cend(value.at.da6));
      } else {
         BOOST_ERROR("invalid type decoded, dummy::nested_type - expected");
      }
   });
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(decode_set_as_variant_test) {
   auto in = dummy::type1{'1', 1, 0xf00002, 3, 4, 5, 6006};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   auto variant_out = tc::decode<dummy::messages>(begin, begin + res.size());
   std::visit([&in](auto const& out) {
      if constexpr (std::is_same_v<decltype(out), dummy::type1 const&>) {
         BOOST_CHECK_EQUAL(in.s1, out.s1);
         BOOST_CHECK_EQUAL(in.ll2, out.ll2);
         BOOST_CHECK_EQUAL(in.i3, out.i3);
         BOOST_CHECK_EQUAL(in.f4, out.f4);
         BOOST_CHECK_EQUAL(in.d5, out.d5);
         BOOST_CHECK_EQUAL(in.d6, out.d6);
      } else {
         BOOST_CHECK_MESSAGE(false, "wrong type has been decoded");
      }
   },
              variant_out);
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(decode_set_test) {
   auto in = dummy::type1{'1', 1, 0xf00002, 3, 4, 5, 6006};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   tc::decode<dummy::messages>(begin, begin + res.size(), [&in](auto const& out) {
      if constexpr (std::is_same_v<decltype(out), dummy::type1 const&>) {
         BOOST_CHECK_EQUAL(in.s1, out.s1);
         BOOST_CHECK_EQUAL(in.ll2, out.ll2);
         BOOST_CHECK_EQUAL(in.i3, out.i3);
         BOOST_CHECK_EQUAL(in.f4, out.f4);
         BOOST_CHECK_EQUAL(in.d5, out.d5);
         BOOST_CHECK_EQUAL(in.d6, out.d6);
      } else {
         BOOST_CHECK_MESSAGE(false, "wrong type has been decoded");
      }
   });
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(decode_set_invalid_type) {
   auto invalid = std::array<tc::byte_t, 20>{0x11};
   auto wrapper = [](auto const& invalid) {
      auto begin = invalid.data();
      [[maybe_unused]] auto should_never_get_here = tc::decode<dummy::messages>(begin, begin + invalid.size());
   };
   BOOST_CHECK_THROW(wrapper(invalid), tc::v1::unknown_type_error);
}

BOOST_AUTO_TEST_CASE(flyweigt_dummy_nested_type_encode_decode_test) {
   auto in = dummy::nested_type{
       '4',
       {'1', 1, 0xf00002, 3, 4, 5, 6006},
       {111111, 2222222, 333333},
       {'2',
        {11, 12, 13},
        {0xf00011, 0xf00012, 0xf00013, 0xf00014, 0xf00015},
        {3, 4, 5, 6, 7, 8, 9},
        {4.4f, 5.5f, 5.6f},
        {5.005, 6.006, 7.007, 8.008, 9.009},
        {6001, 6002, 6003}}};

   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   auto out = tc::decode<tc::flyweight<decltype(in)>>(begin, begin + res.size());
   BOOST_CHECK_EQUAL(in.type, tc::get<&dummy::nested_type::type>(out));
   auto simple = tc::get<&dummy::nested_type::simple>(out);
   BOOST_CHECK_EQUAL(in.simple.s1, simple.s1);
   BOOST_CHECK_EQUAL(in.simple.ll2, simple.ll2);
   BOOST_CHECK_EQUAL(in.simple.i3, simple.i3);
   BOOST_CHECK_EQUAL(in.simple.f4, simple.f4);
   BOOST_CHECK_EQUAL(in.simple.d5, simple.d5);
   BOOST_CHECK_EQUAL(in.simple.d6, simple.d6);

   auto lla = tc::get<&dummy::nested_type::lla>(out);
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.lla), cend(in.lla), cbegin(lla), cend(lla));
   auto at = tc::get<&dummy::nested_type::at>(out);
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.sa1), cend(in.at.sa1), cbegin(at.sa1), cend(at.sa1));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.lla2), cend(in.at.lla2), cbegin(at.lla2), cend(at.lla2));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.ia3), cend(in.at.ia3), cbegin(at.ia3), cend(at.ia3));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.fa4), cend(in.at.fa4), cbegin(at.fa4), cend(at.fa4));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.da5), cend(in.at.da5), cbegin(at.da5), cend(at.da5));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.da6), cend(in.at.da6), cbegin(at.da6), cend(at.da6));

   auto value = out.value();
   BOOST_CHECK_EQUAL(in.simple.s1, value.simple.s1);
   BOOST_CHECK_EQUAL(in.simple.ll2, value.simple.ll2);
   BOOST_CHECK_EQUAL(in.simple.i3, value.simple.i3);
   BOOST_CHECK_EQUAL(in.simple.f4, value.simple.f4);
   BOOST_CHECK_EQUAL(in.simple.d5, value.simple.d5);
   BOOST_CHECK_EQUAL(in.simple.d6, value.simple.d6);

   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.lla), cend(in.lla), cbegin(value.lla), cend(value.lla));

   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.sa1), cend(in.at.sa1), cbegin(value.at.sa1), cend(value.at.sa1));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.lla2), cend(in.at.lla2), cbegin(value.at.lla2), cend(value.at.lla2));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.ia3), cend(in.at.ia3), cbegin(value.at.ia3), cend(value.at.ia3));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.fa4), cend(in.at.fa4), cbegin(value.at.fa4), cend(value.at.fa4));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.da5), cend(in.at.da5), cbegin(value.at.da5), cend(value.at.da5));
   BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.da6), cend(in.at.da6), cbegin(value.at.da6), cend(value.at.da6));
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());

   {
      auto [simple, lla, at] = tc::get<&dummy::nested_type::simple, &dummy::nested_type::lla, &dummy::nested_type::at>(out);
      BOOST_CHECK_EQUAL(in.simple.s1, simple.s1);
      BOOST_CHECK_EQUAL(in.simple.ll2, simple.ll2);
      BOOST_CHECK_EQUAL(in.simple.i3, simple.i3);
      BOOST_CHECK_EQUAL(in.simple.f4, simple.f4);
      BOOST_CHECK_EQUAL(in.simple.d5, simple.d5);
      BOOST_CHECK_EQUAL(in.simple.d6, simple.d6);

      BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.lla), cend(in.lla), cbegin(lla), cend(lla));

      BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.sa1), cend(in.at.sa1), cbegin(at.sa1), cend(at.sa1));
      BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.lla2), cend(in.at.lla2), cbegin(at.lla2), cend(at.lla2));
      BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.ia3), cend(in.at.ia3), cbegin(at.ia3), cend(at.ia3));
      BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.fa4), cend(in.at.fa4), cbegin(at.fa4), cend(at.fa4));
      BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.da5), cend(in.at.da5), cbegin(at.da5), cend(at.da5));
      BOOST_CHECK_EQUAL_COLLECTIONS(cbegin(in.at.da6), cend(in.at.da6), cbegin(at.da6), cend(at.da6));
   }
}

BOOST_AUTO_TEST_CASE(dummy_type1_encode_decode_as_flyweight_tuple_test) {
   auto in = dummy::type1{'1', 1, 0xf00002, 3, 4, 5, 6006};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   using tuple = std::tuple<
       char,
       short,
       long long,
       int,
       float,
       double,
       /*long*/ double>;
   auto out = tc::decode<tc::flyweight<tuple, tc::options<>>>(begin, begin + res.size());
   auto value = out.value();
   BOOST_CHECK_EQUAL(in.type, std::get<0>(value));
   BOOST_CHECK_EQUAL(in.s1, std::get<1>(value));
   BOOST_CHECK_EQUAL(in.ll2, std::get<2>(value));
   BOOST_CHECK_EQUAL(in.i3, std::get<3>(value));
   BOOST_CHECK_EQUAL(in.f4, std::get<4>(value));
   BOOST_CHECK_EQUAL(in.d5, std::get<5>(value));
   BOOST_CHECK_EQUAL(in.d6, std::get<6>(value));
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

BOOST_AUTO_TEST_CASE(decode_set_as_variant_flyweight_test) {
   auto in = dummy::type1{'1', 1, 0xf00002, 3, 4, 5, 6006};
   auto res = tc::encode(in);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in));
   auto begin = res.data();
   auto variant_out = tc::decode<tc::as_flyweight<dummy::messages>>(begin, begin + res.size());
   std::visit([&in](auto const& out) {
      if constexpr (std::is_same_v<decltype(out.value()), dummy::type1>) {
         auto val = out.value();
         BOOST_CHECK_EQUAL(in.s1, val.s1);
         BOOST_CHECK_EQUAL(in.ll2,val.ll2);
         BOOST_CHECK_EQUAL(in.i3, val.i3);
         BOOST_CHECK_EQUAL(in.f4, val.f4);
         BOOST_CHECK_EQUAL(in.d5, val.d5);
         BOOST_CHECK_EQUAL(in.d6, val.d6);
      } else {
         BOOST_CHECK_MESSAGE(false, "wrong type has been decoded");
      }
   },
              variant_out);
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}


BOOST_AUTO_TEST_CASE(few_types_test) {
   auto in1 = simple::type1{0xabcd};
   auto in2 = dummy::type1{'1', 1, 0xf00002, 3, 4, 5, 6006};
   auto res = tc::encode(in1, in2);
   BOOST_CHECK(!res.empty());
   BOOST_CHECK_LE(res.size(), sizeof(in1) + sizeof(in2));
   auto begin = res.data();
   BOOST_CHECK_EQUAL(reinterpret_cast<decltype(in1) const*>(begin)->i1, in1.i1);
   auto out1 = tc::decode<decltype(in1)>(begin, begin + res.size());
   BOOST_CHECK_EQUAL(in1.i1, out1.i1);
   auto out2 = tc::decode<decltype(in2)>(begin, begin + res.size());
   BOOST_CHECK_EQUAL(in2.s1, out2.s1);
   BOOST_CHECK_EQUAL(in2.ll2, out2.ll2);
   BOOST_CHECK_EQUAL(in2.i3, out2.i3);
   BOOST_CHECK_EQUAL(in2.f4, out2.f4);
   BOOST_CHECK_EQUAL(in2.d5, out2.d5);
   BOOST_CHECK_EQUAL(in2.d6, out2.d6);
   BOOST_CHECK_EQUAL(begin, res.data() + res.size());
}

// BOOST_AUTO_TEST_CASE(var_size_test) {
//    auto in = dummy::var_size{"name", {{"1", std::chrono::system_clock::now()}, {"2", std::chrono::system_clock::now()}}};
//    auto res = tc::encode(in);
//    auto begin = res.data();
//    auto out = tc::decode<dummy::var_size>(begin, begin+res.size());
// }

BOOST_AUTO_TEST_SUITE_END()
