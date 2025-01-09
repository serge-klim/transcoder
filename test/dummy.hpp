#pragma once
#include "transcoder/byte_order.hpp"
#include "transcoder/options.hpp"
#include <boost/describe.hpp>
#include <chrono>
#include <variant>
#include <array>


namespace simple {

struct type1 {
   unsigned int i1;
};

BOOST_DESCRIBE_STRUCT(type1, (),
                      (
                          i1))

tc::options<tc::flag::native_endian> protocol_options(...);

} // namespace simple

namespace dummy { inline namespace v1 {

struct type1 {
   char type = '1';
   short s1;
   long long ll2;
   int i3;
   float f4;
   double d5;
   /*long*/ double d6;
};

struct arrays_type {
   char type = '2';
   std::array<short, 3> sa1;
   std::array<long long, 5> lla2;
   // long long lla2[5];
   std::array<int, 7> ia3;
   std::array<float, 3> fa4;
   std::array</*long*/ double, 5> da5;
   std::array<double, 3> da6;
};

struct c_arrays_type {
   char type = '3';
   short sa1[3];
   long long lla2[5];
   int ia3[7];
   float fa4[3];
   /*long*/ double da5[5];
   double da6[3];
};

struct nested_type {
   char type = '4';
   type1 simple;
   std::array<long long, 3> lla;
   arrays_type at;
};

struct name_time {
   std::string name;
   std::chrono::system_clock::time_point nasdaq_time;
};

struct var_size {
   std::string text;
   std::vector<name_time> vals;
};

using messages = std::variant<arrays_type, type1, nested_type>;

tc::options<tc::flag::big_endian> protocol_options(...);

BOOST_DESCRIBE_STRUCT(type1, (),
                      (
                          type,
                          s1,
                          ll2,
                          i3,
                          f4,
                          d5,
                          d6))

BOOST_DESCRIBE_STRUCT(arrays_type, (),
                      (
                          type,
                          sa1,
                          lla2,
                          ia3,
                          fa4,
                          da5,
                          da6))

BOOST_DESCRIBE_STRUCT(c_arrays_type, (),
                      (
                          type,
                          sa1,
                          lla2,
                          ia3,
                          fa4,
                          da5,
                          da6))

BOOST_DESCRIBE_STRUCT(nested_type, (),
                      (
                          type,
                          simple,
                          lla,
                          at))

BOOST_DESCRIBE_STRUCT(name_time, (),
                      (
                          name,
                          nasdaq_time))

BOOST_DESCRIBE_STRUCT(var_size, (),
                      (
                          text,
                          vals))

}} // namespace dummy::v1
