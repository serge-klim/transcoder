#pragma once
#include "transcoder/traits.hpp"
#include <optional>
#include <variant>
#include <array>
#include <cstdint>

namespace md_prsr { inline namespace v1 {

template <typename T, T Null>
class optional {
 public:
   using value_type = T;
   static constexpr value_type null_value = Null;
   optional() = default;
   optional(T value) : value_{std::move(value)} {}
   optional(std::optional<T> value) : value_{value ? *std::move(value) : null_value} {}
   constexpr T const& raw_value() const noexcept { return value_; }
   constexpr bool has_value() const noexcept { return value_ != null_value; }
   explicit constexpr operator bool() const noexcept { return has_value(); }
   operator std::optional<T>() const { return has_value() ? std::make_optional(value_) : std::nullopt; }

 private:
   T value_ = null_value;
};

}} // namespace md_prsr::v1

template <typename T, T Null, typename Options>
struct tc::encoded_sizeof<md_prsr::v1::optional<T, Null>, Options, std::true_type> : tc::encoded_sizeof<T, Options> {};

template <typename T, T Null, typename Options>
struct tc::decoder<md_prsr::v1::optional<T, Null>, Options, std::true_type> : private tc::decoder<T, Options> {
   md_prsr::v1::optional<T, Null> operator()(byte_t const*& begin, [[maybe_unused]] byte_t const* end) noexcept {
      return {tc::decoder<T, Options>::operator()(begin, end)};
   }
};

template <typename T, T Null, typename Options>
struct tc::encoder<md_prsr::v1::optional<T, Null>, Options, std::true_type> : private tc::encoder<T, Options> {
   byte_t* operator()(md_prsr::v1::optional<T, Null> const& value, byte_t* begin, [[maybe_unused]] byte_t* end) noexcept {
      return {tc::encoder<T, Options>::operator()(value.raw_value(), begin, end)};
   }
};



namespace eobi_test {

using BodyLen=md_prsr::optional<std::uint16_t,0xFFFF>;
using TemplateID = std::uint16_t;
using MsgSeqNum = std::uint32_t;
struct MessageHeaderComp {
   BodyLen bodyLen;
   TemplateID templateID;
   MsgSeqNum msgSeqNum;
};
using SecurityID=std::uint64_t;
using TransactTime = std::uint64_t;
using SecurityDesc = std::array<char, 40>;
using SecuritySubType = std::uint32_t;

struct AddComplexInstrument {
   MessageHeaderComp messageHeader;
   SecurityID securityID;
   TransactTime transactTime;
   SecurityDesc securityDesc;
   SecuritySubType securitySubType;
   //md_prsr::optional<ProductComplex, ProductComplex::NoValue> productComplex;
   //md_prsr::optional<ImpliedMarketIndicator, ImpliedMarketIndicator::NoValue> impliedMarketIndicator;
   //QuantityScalingFactor quantityScalingFactor;
   //LegRatioMultiplier legRatioMultiplier;
   //NoLegs noLegs;
   //Pad2 pad2;
   //md_prsr::optional<LastFragment, LastFragment::NoValue> lastFragment;
   //InstrmtLegGrpComp instrmtLegGrp;
};


using messages = std::variant<AddComplexInstrument
                               //AddFlexibleInstrument,
                               //InstrumentSummary,
    >;

} // namespace eobi_test

#include <boost/describe.hpp>
namespace eobi_test {

BOOST_DESCRIBE_STRUCT(MessageHeaderComp, (), (bodyLen, templateID, msgSeqNum))

//BOOST_DESCRIBE_ENUM(ProductComplex, SimpleInstrument, StandardOptionStrategy, NonStandardOptionStrategy, VolatilityStrategy, FuturesSpread, InterProductSpread, StandardFuturesStrategy, PackAndBundle, Strip, FlexibleInstrument, CommodityStrip, ScaledSimpleInstrument, NonStandardVolatilityStrategy, TotalReturnFutureStrategy, NoValue)
//BOOST_DESCRIBE_ENUM(ImpliedMarketIndicator, NotImplied, ImpliedInOut, NoValue)
//BOOST_DESCRIBE_ENUM(LastFragment, N, Y, NoValue)
//BOOST_DESCRIBE_ENUM(LegSecurityType, LegSecurityMultiLeg, LegSecurityUnderlyingLeg, NoValue)
//BOOST_DESCRIBE_ENUM(LegSide, Buy, Sell, NoValue)
//BOOST_DESCRIBE_ENUM(RelatedPriceType, Basis, NoValue)
//BOOST_DESCRIBE_ENUM(TradingStyle, Regular, TRF_TAC, TRF_TAM, MOC, NoValue)
//BOOST_DESCRIBE_STRUCT(InstrmtLegGrpComp, (), (legSymbol, pad4, legSecurityID, legPrice, legRatioQty, legSecurityType, legSide, relatedPriceType, tradingStyle, relatedPrice, ))
//
BOOST_DESCRIBE_STRUCT(AddComplexInstrument, (), (messageHeader, securityID, transactTime, securityDesc, securitySubType/*, productComplex, impliedMarketIndicator, quantityScalingFactor, legRatioMultiplier, noLegs, pad2, lastFragment, instrmtLegGrp*/ ))


} // namespace eobi_test


template <>
struct tc::type_id<eobi_test::messages> {
   using type = tc::proto_type_id<&eobi_test::MessageHeaderComp::templateID>;
};

template<> struct tc::type_id<eobi_test::AddComplexInstrument> { constexpr std::uint16_t operator()() const noexcept { return 13400;} };
//template<> struct tc::type_id<eobi_test::AddFlexibleInstrument> { constexpr std::uint16_t operator()() const noexcept { return 13401;} };
//template<> struct tc::type_id<eobi_test::InstrumentSummary> { constexpr std::uint16_t operator()() const noexcept { return 13601;} };
