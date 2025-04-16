#pragma once
#include "sbe.hpp"
#include "transcoder/basic_types.hpp"
#include <boost/detail/bitmask.hpp>
#include <variant>
#include <array>
#include <cstdint>

// auto generated from https://www.cmegroup.com/ftp/SBEFix/Production/Templates/templates_FixBinary.xml
// by cme_gen(WIP)

namespace cme_test {

using Asset = std::array<char, 6>;
using CFICode = std::array<char, 6>;
using CHAR = char;
using Currency = std::array<char, 3>;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int32NULL = sbe::optional<std::int32_t, 2147483647>;
using Int8 = std::int8_t;
using Int8NULL = sbe::optional<std::int8_t, 127>;
using LocalMktDate = sbe::optional<std::uint16_t, 65535U>;
using MDFeedType = std::array<char, 3>;
using QuoteReqId = std::array<char, 23>;
using SecurityExchange = std::array<char, 4>;
using SecurityGroup = std::array<char, 6>;
using SecuritySubType = std::array<char, 5>;
using SecurityType = std::array<char, 6>;
using Symbol = std::array<char, 20>;
using Text = std::array<char, 180>;
using UnderlyingSymbol = std::array<char, 20>;
using UnitOfMeasure = std::array<char, 30>;
using UserDefinedInstrument = std::array<char, 1>;
using uInt32 = std::uint32_t;
using uInt32NULL = sbe::optional<std::uint32_t, 4294967295U>;
using uInt64 = std::uint64_t;
using uInt64NULL = sbe::optional<std::uint64_t, 18446744073709551615ULL>;
using uInt8 = std::uint8_t;
using uInt8NULL = sbe::optional<std::uint8_t, 255U>;

struct Decimal9 {
   std::int64_t mantissa;
   static constexpr std::int8_t exponent = -9;
};
struct Decimal9NULL {
   sbe::optional<std::int64_t, 9223372036854775807> mantissa;
   static constexpr std::int8_t exponent = -9;
};
struct DecimalQty {
   sbe::optional<std::int32_t, 2147483647> mantissa;
   static constexpr std::int8_t exponent = -4;
};
struct FLOAT {
   std::int64_t mantissa;
   static constexpr std::int8_t exponent = -7;
};
struct MaturityMonthYear {
   sbe::optional<std::uint16_t, 65535> year;
   sbe::optional<std::uint8_t, 255> month;
   sbe::optional<std::uint8_t, 255> day;
   sbe::optional<std::uint8_t, 255> week;
};
struct PRICE {
   std::int64_t mantissa;
   static constexpr std::int8_t exponent = -7;
};
struct PRICE9 {
   std::int64_t mantissa;
   static constexpr std::int8_t exponent = -9;
};
struct PRICENULL {
   sbe::optional<std::int64_t, 9223372036854775807> mantissa;
   static constexpr std::int8_t exponent = -7;
};
struct PRICENULL9 {
   sbe::optional<std::int64_t, 9223372036854775807> mantissa;
   static constexpr std::int8_t exponent = -9;
};
struct groupSize {
   std::uint16_t blockLength;
   std::uint8_t numInGroup;
};
struct groupSize8Byte {
   std::uint16_t blockLength;
   tc::padding<5> padding_1;
   std::uint8_t numInGroup;
};
struct groupSizeEncoding {
   std::uint16_t blockLength;
   std::uint16_t numInGroup;
};
struct messageHeader {
   std::uint16_t blockLength;
   std::uint16_t templateId;
   std::uint16_t schemaId;
   std::uint16_t version;
};
enum class AggressorSide : std::uint8_t {
   NoAggressor = 0,
   Buy = 1,
   Sell = 2,
   NotPresent = 255
};
enum class EventType : uInt8 {
   Activation = 5,
   LastEligibleTradeDate = 7
};
enum class HaltReason : uInt8 {
   GroupSchedule = 0,
   SurveillanceIntervention = 1,
   MarketEvent = 2,
   InstrumentActivation = 3,
   InstrumentExpiration = 4,
   Unknown = 5,
   RecoveryInProcess = 6
};
enum class LegSide : uInt8 {
   BuySide = 1,
   SellSide = 2
};
enum class MDEntryType : CHAR {
   Bid = '0',
   Offer = '1',
   Trade = '2',
   OpenPrice = '4',
   SettlementPrice = '6',
   TradingSessionHighPrice = '7',
   TradingSessionLowPrice = '8',
   ClearedVolume = 'B',
   OpenInterest = 'C',
   ImpliedBid = 'E',
   ImpliedOffer = 'F',
   BookReset = 'J',
   SessionHighBid = 'N',
   SessionLowOffer = 'O',
   FixingPrice = 'W',
   ElectronicVolume = 'e',
   ThresholdLimitsandPriceBandVariation = 'g'
};
enum class MDEntryTypeBook : CHAR {
   Bid = '0',
   Offer = '1',
   ImpliedBid = 'E',
   ImpliedOffer = 'F',
   BookReset = 'J'
};
enum class MDEntryTypeDailyStatistics : CHAR {
   SettlementPrice = '6',
   ClearedVolume = 'B',
   OpenInterest = 'C',
   FixingPrice = 'W'
};
enum class MDEntryTypeStatistics : CHAR {
   OpenPrice = '4',
   HighTrade = '7',
   LowTrade = '8',
   HighestBid = 'N',
   LowestOffer = 'O'
};
enum class MDUpdateAction : uInt8 {
   New = 0,
   Change = 1,
   Delete = 2,
   DeleteThru = 3,
   DeleteFrom = 4,
   Overlay = 5
};
enum class OpenCloseSettlFlag : std::uint8_t {
   DailyOpenPrice = 0,
   IndicativeOpeningPrice = 5,
   NotPresent = 255
};
enum class OrderUpdateAction : uInt8 {
   New = 0,
   Update = 1,
   Delete = 2
};
enum class PutOrCall : uInt8 {
   Put = 0,
   Call = 1
};
enum class SecurityTradingEvent : uInt8 {
   NoEvent = 0,
   NoCancel = 1,
   ResetStatistics = 4,
   ImpliedMatchingON = 5,
   ImpliedMatchingOFF = 6
};
enum class SecurityTradingStatus : std::uint8_t {
   TradingHalt = 2,
   Close = 4,
   NewPriceIndication = 15,
   ReadyToTrade = 17,
   NotAvailableForTrading = 18,
   UnknownorInvalid = 20,
   PreOpen = 21,
   PreCross = 24,
   Cross = 25,
   PostClose = 26,
   NoChange = 103,
   NotPresent = 255
};
enum class SecurityUpdateAction : CHAR {
   Add = 'A',
   Delete = 'D',
   Modify = 'M'
};
enum class InstAttribValue : uInt32 {
   ElectronicMatchEligible = 0,
   OrderCrossEligible = 1,
   BlockTradeEligible = 2,
   EFPEligible = 3,
   EBFEligible = 4,
   EFSEligible = 5,
   EFREligible = 6,
   OTCEligible = 7,
   iLinkIndicativeMassQuotingEligible = 8,
   NegativeStrikeEligible = 9,
   NegativePriceOutrightEligible = 10,
   IsFractional = 11,
   VolatilityQuotedOption = 12,
   RFQCrossEligible = 13,
   ZeroPriceOutrightEligible = 14,
   DecayingProductEligibility = 15,
   VariableProductEligibility = 16,
   DailyProductEligibility = 17,
   GTOrdersEligibility = 18,
   ImpliedMatchingEligibility = 19,
   TriangulationEligible = 20,
   VariableCabEligible = 21
};
BOOST_BITMASK(InstAttribValue)

enum class MatchEventIndicator : uInt8 {
   LastTradeMsg = 0,
   LastVolumeMsg = 1,
   LastQuoteMsg = 2,
   LastStatsMsg = 3,
   LastImpliedMsg = 4,
   RecoveryMsg = 5,
   Reserved = 6,
   EndOfEvent = 7
};
BOOST_BITMASK(MatchEventIndicator)

enum class SettlPriceType : uInt8 {
   FinalDaily = 0,
   Actual = 1,
   Rounded = 2,
   Intraday = 3,
   ReservedBits = 4,
   NullValue = 7
};
BOOST_BITMASK(SettlPriceType)

//////////////////////////////////////

struct SecurityStatus30 {
   std::uint16_t msg_size;
   messageHeader message_header;
   uInt64 transactTime;                                                                           // Start of event processing time in number of nanoseconds since Unix epoch.
   SecurityGroup securityGroup;                                                                   // Security Group
   Asset asset;                                                                                   // Product Code within Security Group specified
   sbe::optional<std::int32_t, 2147483647> securityID;                                            // If this tag is present, 35=f message is sent for the instrument
   sbe::optional<std::uint16_t, 65535> tradeDate;                                                 // Trade Session Date
   MatchEventIndicator matchEventIndicator;                                                       // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   sbe::optional<SecurityTradingStatus, SecurityTradingStatus::NotPresent> securityTradingStatus; // Identifies the trading status applicable to the instrument or Security Group
   HaltReason haltReason;                                                                         // Identifies the reason for the status change
   SecurityTradingEvent securityTradingEvent;                                                     // Identifies an additional event or a rule related to the status
};

struct MDIncrementalRefreshVolume37 {
   // https://cmegroupclientsite.atlassian.net/wiki/spaces/EPICSANDBOX/pages/457325932/MDP+3.0+-+SBE+Message+Header
   std::uint16_t msg_size;
   messageHeader message_header;
   uInt64 transactTime;                     // Start of event processing time in number of nanoseconds since Unix epoch
   MatchEventIndicator matchEventIndicator; // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   tc::padding<2> block_padding;
   // Number of entries in Market Data message
   struct NoMDEntries {
      Int32 mDEntrySize;                       // Cumulative traded volume
      Int32 securityID;                        // Security ID
      uInt32 rptSeq;                           // Market Data entry sequence number per instrument update
      MDUpdateAction mDUpdateAction;           // Market Data update action
      static constexpr char mDEntryType = 'e'; // Electronic Volume entry provides cumulative session trade volume updated with the event
      tc::padding<3> padding;
   };
   sbe::group<NoMDEntries, groupSize> noMDEntries;
};

struct SnapshotFullRefresh38 {
   std::uint16_t msg_size;
   messageHeader message_header;
   uInt32 lastMsgSeqNumProcessed;                                                                   // Sequence number of the last Incremental feed packet processed. This value is used to synchronize the snapshot loop with the real-time feed
   uInt32 totNumReports;                                                                            // Total number of messages replayed in the loop
   Int32 securityID;                                                                                // Security ID
   uInt32 rptSeq;                                                                                   // Sequence number of the last Market Data entry processed for the instrument
   uInt64 transactTime;                                                                             // Timestamp of the last event security participated in, sent as number of nanoseconds since Unix epoch
   uInt64 lastUpdateTime;                                                                           // UTC Date and time of last Security Definition add, update or delete on a given Market Data channel
   sbe::optional<std::uint16_t, 65535> tradeDate;                                                   // Trade session date sent as number of days since Unix epoch
   sbe::optional<SecurityTradingStatus, SecurityTradingStatus::NotPresent> mDSecurityTradingStatus; // Identifies the current trading state of the instrument
   PRICENULL highLimitPrice;                                                                        // Upper price threshold for the instrument
   PRICENULL lowLimitPrice;                                                                         // Lower price threshold for the instrument
   PRICENULL maxPriceVariation;                                                                     // Differential value for price banding
   // Number of entries in Market Data message
   struct NoMDEntries {
      PRICENULL mDEntryPx;                                                                  // Market Data entry price
      sbe::optional<std::int32_t, 2147483647> mDEntrySize;                                  // Market Data entry quantity
      sbe::optional<std::int32_t, 2147483647> numberOfOrders;                               // Aggregate number of orders at the given price level
      sbe::optional<std::int8_t, 127> mDPriceLevel;                                         // Aggregate book position
      sbe::optional<std::uint16_t, 65535> tradingReferenceDate;                             // Indicates the date of trade session corresponding to a statistic entry
      sbe::optional<OpenCloseSettlFlag, OpenCloseSettlFlag::NotPresent> openCloseSettlFlag; // Flag describing  Open Price entry
      SettlPriceType settlPriceType;                                                        // Bitmap field of eight Boolean type indicators representing settlement price type
      MDEntryType mDEntryType;                                                              // Market Data entry type
   };
   sbe::group<NoMDEntries, groupSize> noMDEntries;
};

struct MDIncrementalRefreshBook46 {
   std::uint16_t msg_size;
   messageHeader message_header;
   uInt64 transactTime;                     // Start of event processing time in number of nanoseconds since Unix epoch
   MatchEventIndicator matchEventIndicator; // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   tc::padding<2> block_padding;
   // Number of entries in Market Data message
   struct NoMDEntries {
      PRICENULL9 mDEntryPx;                                   // Market Data entry price
      sbe::optional<std::int32_t, 2147483647> mDEntrySize;    // Market Data entry size
      Int32 securityID;                                       // Security ID
      uInt32 rptSeq;                                          // Market Data entry sequence number per instrument update
      sbe::optional<std::int32_t, 2147483647> numberOfOrders; // In Book entry - aggregate number of orders at given price level
      uInt8 mDPriceLevel;                                     // Aggregate book level
      MDUpdateAction mDUpdateAction;                          //  Market Data update action
      MDEntryTypeBook mDEntryType;                            // Market Data entry type
      tc::padding<5> padding;
   };
   sbe::group<NoMDEntries, groupSize> noMDEntries;
   ////<----------group NoOrderIDEntries
   // Number of OrderID entries
   struct NoOrderIDEntries {
      uInt64 orderID;                                                     // Unique Order ID
      sbe::optional<std::uint64_t, 18446744073709551615ULL> mDOrderPriority; // Order priority for execution on the order book
      sbe::optional<std::int32_t, 2147483647> mDDisplayQty;               // Visible qty of order
      sbe::optional<std::uint8_t, 255> referenceID;                       // Reference to corresponding Price and Security ID, sequence of MD entry in the message
      OrderUpdateAction orderUpdateAction;                                // Order book update action to be applied to the order referenced by OrderID
      tc::padding<2> padding;
   };
   sbe::group<NoOrderIDEntries, groupSize8Byte> noOrderIDEntries;
};
struct MDIncrementalRefreshOrderBook47 {
   std::uint16_t msg_size;
   messageHeader message_header;
   uInt64 transactTime;                     // Start of event processing time in number of nanoseconds since Unix epoch
   MatchEventIndicator matchEventIndicator; // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   tc::padding<2> block_padding;
   // Number of entries in Market Data message
   struct NoMDEntries {
      sbe::optional<std::uint64_t, 18446744073709551615ULL> orderID;         // Order ID
      sbe::optional<std::uint64_t, 18446744073709551615ULL> mDOrderPriority; // Order priority for execution on the order book
      PRICENULL9 mDEntryPx;                                               // Order price
      sbe::optional<std::int32_t, 2147483647> mDDisplayQty;               // Visible order qty
      Int32 securityID;                                                   // Security ID
      MDUpdateAction mDUpdateAction;                                      // Order book update action to be applied to the order referenced by OrderID
      MDEntryTypeBook mDEntryType;                                        // Market Data entry type
      tc::padding<6> padding;
   };
   sbe::group<NoMDEntries, groupSize> noMDEntries;
};

struct MDIncrementalRefreshSessionStatistics51 {
   std::uint16_t msg_size;
   messageHeader message_header;
   uInt64 transactTime;                     // Start of event processing time in number of nanoseconds since Unix epoch
   MatchEventIndicator matchEventIndicator; // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   tc::padding<2> block_padding;
   // Number of entries in Market Data message
   struct NoMDEntries {
      PRICE9 mDEntryPx;                                                                     // Market Data entry price
      Int32 securityID;                                                                     // Security ID
      uInt32 rptSeq;                                                                        // MD Entry sequence number per instrument update
      sbe::optional<OpenCloseSettlFlag, OpenCloseSettlFlag::NotPresent> openCloseSettlFlag; // Flag describing IOP and Open Price entries
      MDUpdateAction mDUpdateAction;                                                        // Market Data update action
      MDEntryTypeStatistics mDEntryType;                                                    // Market Data entry type
      sbe::optional<std::int32_t, 2147483647> mDEntrySize;                                  // Indicative Opening Quantity
      tc::padding<1> padding;
   };
   sbe::group<NoMDEntries, groupSize> noMDEntries;
};
} // namespace cme_test

#include <boost/describe.hpp>

namespace cme_test {

BOOST_DESCRIBE_STRUCT(Decimal9, (), (mantissa, ))

BOOST_DESCRIBE_STRUCT(Decimal9NULL, (), (mantissa, ))

BOOST_DESCRIBE_STRUCT(DecimalQty, (), (mantissa, ))

BOOST_DESCRIBE_STRUCT(FLOAT, (), (mantissa, ))

BOOST_DESCRIBE_STRUCT(MaturityMonthYear, (), (year, month, day, week, ))

BOOST_DESCRIBE_STRUCT(PRICE, (), (mantissa, ))

BOOST_DESCRIBE_STRUCT(PRICE9, (), (mantissa, ))

BOOST_DESCRIBE_STRUCT(PRICENULL, (), (mantissa, ))

BOOST_DESCRIBE_STRUCT(PRICENULL9, (), (mantissa, ))

BOOST_DESCRIBE_STRUCT(groupSize, (), (blockLength, numInGroup, ))

BOOST_DESCRIBE_STRUCT(groupSize8Byte, (), (blockLength, padding_1, numInGroup, ))

BOOST_DESCRIBE_STRUCT(groupSizeEncoding, (), (blockLength, numInGroup, ))

BOOST_DESCRIBE_STRUCT(messageHeader, (), (blockLength, templateId, schemaId, version, ))

BOOST_DESCRIBE_ENUM(AggressorSide, NoAggressor, Buy, Sell, )
BOOST_DESCRIBE_ENUM(EventType, Activation, LastEligibleTradeDate, )
BOOST_DESCRIBE_ENUM(HaltReason, GroupSchedule, SurveillanceIntervention, MarketEvent, InstrumentActivation, InstrumentExpiration, Unknown, RecoveryInProcess, )
BOOST_DESCRIBE_ENUM(LegSide, BuySide, SellSide, )
BOOST_DESCRIBE_ENUM(MDEntryType, Bid, Offer, Trade, OpenPrice, SettlementPrice, TradingSessionHighPrice, TradingSessionLowPrice, ClearedVolume, OpenInterest, ImpliedBid, ImpliedOffer, BookReset, SessionHighBid, SessionLowOffer, FixingPrice, ElectronicVolume, ThresholdLimitsandPriceBandVariation, )
BOOST_DESCRIBE_ENUM(MDEntryTypeBook, Bid, Offer, ImpliedBid, ImpliedOffer, BookReset, )
BOOST_DESCRIBE_ENUM(MDEntryTypeDailyStatistics, SettlementPrice, ClearedVolume, OpenInterest, FixingPrice, )
BOOST_DESCRIBE_ENUM(MDEntryTypeStatistics, OpenPrice, HighTrade, LowTrade, HighestBid, LowestOffer, )
BOOST_DESCRIBE_ENUM(MDUpdateAction, New, Change, Delete, DeleteThru, DeleteFrom, Overlay, )
BOOST_DESCRIBE_ENUM(OpenCloseSettlFlag, DailyOpenPrice, IndicativeOpeningPrice, )
BOOST_DESCRIBE_ENUM(OrderUpdateAction, New, Update, Delete, )
BOOST_DESCRIBE_ENUM(PutOrCall, Put, Call, )
BOOST_DESCRIBE_ENUM(SecurityTradingEvent, NoEvent, NoCancel, ResetStatistics, ImpliedMatchingON, ImpliedMatchingOFF, )
BOOST_DESCRIBE_ENUM(SecurityTradingStatus, TradingHalt, Close, NewPriceIndication, ReadyToTrade, NotAvailableForTrading, UnknownorInvalid, PreOpen, PreCross, Cross, PostClose, NoChange, )
BOOST_DESCRIBE_ENUM(SecurityUpdateAction, Add, Delete, Modify, )
BOOST_DESCRIBE_ENUM(InstAttribValue, ElectronicMatchEligible, OrderCrossEligible, BlockTradeEligible, EFPEligible, EBFEligible, EFSEligible, EFREligible, OTCEligible, iLinkIndicativeMassQuotingEligible, NegativeStrikeEligible, NegativePriceOutrightEligible, IsFractional, VolatilityQuotedOption, RFQCrossEligible, ZeroPriceOutrightEligible, DecayingProductEligibility, VariableProductEligibility, DailyProductEligibility, GTOrdersEligibility, ImpliedMatchingEligibility, TriangulationEligible, VariableCabEligible, )
BOOST_DESCRIBE_ENUM(MatchEventIndicator, LastTradeMsg, LastVolumeMsg, LastQuoteMsg, LastStatsMsg, LastImpliedMsg, RecoveryMsg, Reserved, EndOfEvent, )
BOOST_DESCRIBE_ENUM(SettlPriceType, FinalDaily, Actual, Rounded, Intraday, ReservedBits, NullValue, )

BOOST_DESCRIBE_STRUCT(SecurityStatus30, (), (
	msg_size,
	message_header,
	transactTime,
	securityGroup,
	asset,
	securityID,
	tradeDate,
	matchEventIndicator,
	securityTradingStatus,
	haltReason,
	securityTradingEvent,
))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshVolume37, (), (
	msg_size,
	message_header,
	transactTime,
	matchEventIndicator,
	block_padding,
	noMDEntries,
 ))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshVolume37::NoMDEntries, (), (
	mDEntrySize,
	securityID,
	rptSeq,
	mDUpdateAction,
	padding,
))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshBook46, (), (
	msg_size,
	message_header,
	transactTime,
	matchEventIndicator,
	block_padding,
	noMDEntries,
 	noOrderIDEntries,
 ))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshBook46::NoMDEntries, (), (
	mDEntryPx,
	mDEntrySize,
	securityID,
	rptSeq,
	numberOfOrders,
	mDPriceLevel,
	mDUpdateAction,
	mDEntryType,
	padding,
))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshBook46::NoOrderIDEntries, (), (
	orderID,
	mDOrderPriority,
	mDDisplayQty,
	referenceID,
	orderUpdateAction,
	padding,
))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshOrderBook47, (), (
	msg_size,
	message_header,
	transactTime,
	matchEventIndicator,
	block_padding,
	noMDEntries,
 ))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshOrderBook47::NoMDEntries, (), (
	orderID,
	mDOrderPriority,
	mDEntryPx,
	mDDisplayQty,
	securityID,
	mDUpdateAction,
	mDEntryType,
	padding,
))


BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshSessionStatistics51, (), (
	msg_size,
	message_header,
	transactTime,
	matchEventIndicator,
	block_padding,
	noMDEntries,
 ))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshSessionStatistics51::NoMDEntries, (), (
	mDEntryPx,
	securityID,
	rptSeq,
	openCloseSettlFlag,
	mDUpdateAction,
	mDEntryType,
	mDEntrySize,
	padding,
))

using messages = std::variant<SecurityStatus30,MDIncrementalRefreshVolume37,MDIncrementalRefreshBook46,MDIncrementalRefreshOrderBook47,MDIncrementalRefreshSessionStatistics51>;

struct extended_message_header {
   std::uint16_t msg_size;
   std::uint16_t blockLength;
   std::uint16_t templateId;
   std::uint16_t schemaId;
   std::uint16_t version;
};
BOOST_DESCRIBE_STRUCT(extended_message_header, (),
                      (   msg_size,
                          blockLength,
						  templateId))

} // namespace cme_test

#include "transcoder/traits.hpp"
#include "transcoder/byte_order.hpp"
#include "transcoder/options.hpp"

namespace cme_test {
tc::options<tc::flag::little_endian> protocol_options(...);
} // namespace cme_test

template <>
struct tc::type_id<cme_test::messages> {
   using type = tc::proto_type_id<&cme_test::extended_message_header::templateId>;
};

template<> struct tc::type_id<cme_test::SecurityStatus30> { constexpr std::uint16_t operator()() const noexcept { return 30;} };
template<> struct tc::type_id<cme_test::MDIncrementalRefreshVolume37> { constexpr std::uint16_t operator()() const noexcept { return 37;} };
template<> struct tc::type_id<cme_test::MDIncrementalRefreshBook46> { constexpr std::uint16_t operator()() const noexcept { return 46;} };
template<> struct tc::type_id<cme_test::MDIncrementalRefreshOrderBook47> { constexpr std::uint16_t operator()() const noexcept { return 47;} };
template<> struct tc::type_id<cme_test::MDIncrementalRefreshSessionStatistics51> { constexpr std::uint16_t operator()() const noexcept { return 51;} };
