#pragma once
#include "transcoder/traits.hpp"
#include <variant>
#include <array>
#include <cstdint>

// DataType Pad1
#define LEN_PAD1 1
// DataType Pad2
#define LEN_PAD2 2
// DataType Pad3
#define LEN_PAD3 3
// DataType Pad4
#define LEN_PAD4 4
// DataType Pad5
#define LEN_PAD5 5
// DataType Pad6
#define LEN_PAD6 6
// DataType Pad7
#define LEN_PAD7 7

namespace eobi_test {

struct MessageHeaderComp {
   std::uint16_t BodyLen;
   std::uint16_t TemplateID;
   std::uint32_t MsgSeqNum;
};

struct OrderDetailsComp {
   std::uint64_t TrdRegTSTimePriority;
   std::int64_t DisplayQty;
   std::uint8_t Side;
   std::uint8_t OrdType;
   std::int8_t HHIIndicator;
   std::array<char, LEN_PAD5> Pad5;
   std::int64_t Price;
};

struct Heartbeat {
   MessageHeaderComp MessageHeader;
   std::uint32_t LastMsgSeqNumProcessed;
   std::array<char, LEN_PAD4> Pad4;
};

struct OrderAdd {
   MessageHeaderComp MessageHeader;
   std::uint64_t RequestTime;
   std::int64_t SecurityID;
   OrderDetailsComp OrderDetails;
};

// Message:	    OrderDelete
// TemplateID:  13102
// Alias:       Order Delete
// FIX MsgType: MarketDataOrder = "U21"
struct OrderDelete {
   MessageHeaderComp MessageHeader;
   std::uint64_t RequestTime;
   std::uint64_t TransactTime;
   std::int64_t SecurityID;
   OrderDetailsComp OrderDetails;
};

// Message:	    OrderMassDelete
// TemplateID:  13103
// Alias:       Order Mass Delete
// FIX MsgType: MarketDataOrder = "U21"
struct OrderMassDelete {
   MessageHeaderComp MessageHeader;
   std::int64_t SecurityID;
   std::uint64_t TransactTime;
};

using messages = std::variant<Heartbeat,OrderAdd,OrderDelete,OrderMassDelete>;

} // namespace eobi_test

#include <boost/describe.hpp>
namespace eobi_test {

BOOST_DESCRIBE_STRUCT(MessageHeaderComp, (), (
   BodyLen,
   TemplateID,
   MsgSeqNum
))


BOOST_DESCRIBE_STRUCT(OrderDetailsComp, (), (
   TrdRegTSTimePriority,
   DisplayQty,
   Side,
   OrdType,
   HHIIndicator,
   Pad5,
   Price,
))

BOOST_DESCRIBE_STRUCT(Heartbeat, (), (
   MessageHeader,
   LastMsgSeqNumProcessed,
   Pad4,
))

BOOST_DESCRIBE_STRUCT(OrderAdd, (), (
   MessageHeader,
   RequestTime,
   SecurityID,
   OrderDetails,
))

// Message:	    OrderDelete
// TemplateID:  13102
// Alias:       Order Delete
// FIX MsgType: MarketDataOrder = "U21"
BOOST_DESCRIBE_STRUCT(OrderDelete, (), (
   MessageHeader,
   RequestTime,
   TransactTime,
   SecurityID,
   OrderDetails,
))

// Message:	    OrderMassDelete
// TemplateID:  13103
// Alias:       Order Mass Delete
// FIX MsgType: MarketDataOrder = "U21"
BOOST_DESCRIBE_STRUCT(OrderMassDelete, (), (
   MessageHeader,
   SecurityID,
   TransactTime,
))

} // namespace eobi_test

template <>
struct tc::type_id<eobi_test::messages> {
   using type = tc::proto_type_id<&eobi_test::MessageHeaderComp::TemplateID>;
};

template<> struct tc::type_id<eobi_test::Heartbeat> { constexpr std::uint16_t operator()() const noexcept { return 13001;} };
template<> struct tc::type_id<eobi_test::OrderAdd> { constexpr std::uint16_t operator()() const noexcept { return 13100;} };
template<> struct tc::type_id<eobi_test::OrderDelete> { constexpr std::uint16_t operator()() const noexcept { return 13102;} };
template<> struct tc::type_id<eobi_test::OrderMassDelete> { constexpr std::uint16_t operator()() const noexcept { return 13103;} };
