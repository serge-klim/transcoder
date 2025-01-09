#pragma once
#include "sbe.hpp"
#include "transcoder/basic_types.hpp"
#include "transcoder/byte_order.hpp"
#include "transcoder/options.hpp"
#include <array>
#include <cstdint>

// auto generated from https://www.cmegroup.com/ftp/SBEFix/Production/Templates/templates_FixBinary.xml
// by cme_gen(WIP)

namespace cme_test {

tc::options<tc::flag::little_endian> protocol_options(...);

using Asset = std::array<char, 6>;
using CFICode = std::array<char, 6>;
using CHAR = char;
using Currency = std::array<char, 3>;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int32NULL = sbe::optional<std::int32_t, 2147483647>;
using Int8 = std::int8_t;
using Int8NULL = sbe::optional<std::int8_t, 127>;
using LocalMktDate = sbe::optional<std::uint16_t, 65535>;
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
using uInt32NULL = sbe::optional<std::uint32_t, 4294967295>;
using uInt64 = std::uint64_t;
using uInt64NULL = sbe::optional<std::uint64_t, 18446744073709551615ULL>;
using uInt8 = std::uint8_t;
using uInt8NULL = sbe::optional<std::uint8_t, 255>;
struct Decimal9 {
   std::int64_t mantissa;
   static constexpr std::int8_t exponent = -9;
};
struct Decimal9NULL {
   std::int64_t mantissa;
   static constexpr std::int8_t exponent = -9;
};
struct DecimalQty {
   std::int32_t mantissa;
   static constexpr std::int8_t exponent = -4;
};
struct FLOAT {
   std::int64_t mantissa;
   static constexpr std::int8_t exponent = -7;
};
struct MaturityMonthYear {
   std::uint16_t year;
   std::uint8_t month;
   std::uint8_t day;
   std::uint8_t week;
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
    sbe::optional<std::int64_t, 9223372036854775807LL> mantissa;
    static constexpr std::int8_t exponent = -7;
 };
 // TODO: update generator ^^^
//using PRICENULL = sbe::optional<PRICE, PRICE{9223372036854775807LL}>;

 struct PRICENULL9 {
    sbe::optional<std::int64_t, 9223372036854775807LL> mantissa;
    static constexpr std::int8_t exponent = -9;
 };
//  TODO: update generator ^^^
//using PRICENULL9 = sbe::optional<PRICE9, PRICE9{9223372036854775807LL}>;

struct groupSize {
   std::uint16_t blockLength;
   std::uint8_t numInGroup;
};
struct groupSize8Byte {
   std::uint16_t blockLength;
   tc::padding<5> padding; // TODO: fix based on offset: <type name="numInGroup" description="NumInGroup" offset="7" primitiveType="uint8"/>
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

// enum AggressorSide : std::uint8_t
//{
//    NoAggressor = 0,
//    Buy = 1,
//    Sell = 2
// };
// enum EventType : uInt8
//{
//    Activation = 5,
//    LastEligibleTradeDate = 7
// };
enum HaltReason : uInt8 {
   GroupSchedule = 0,
   SurveillanceIntervention = 1,
   MarketEvent = 2,
   InstrumentActivation = 3,
   InstrumentExpiration = 4,
   Unknown = 5,
   RecoveryInProcess = 6
};
// enum LegSide : uInt8
//{
//    BuySide = 1,
//    SellSide = 2
// };

enum MDEntryType : CHAR {
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

enum MDEntryTypeBook : CHAR {
   MDEntryTypeBook_Bid = '0',
   MDEntryTypeBook_Offer = '1',
   MDEntryTypeBook_ImpliedBid = 'E',
   MDEntryTypeBook_ImpliedOffer = 'F',
   MDEntryTypeBook_BookReset = 'J'
};
// enum MDEntryTypeDailyStatistics : CHAR
//{
//    SettlementPrice = '6',
//    ClearedVolume = 'B',
//    OpenInterest = 'C',
//    FixingPrice = 'W'
// };

enum MDEntryTypeStatistics : CHAR {
   MDEntryTypeStatistics_OpenPrice = '4',
   MDEntryTypeStatistics_HighTrade = '7',
   MDEntryTypeStatistics_LowTrade = '8',
   MDEntryTypeStatistics_HighestBid = 'N',
   MDEntryTypeStatistics_LowestOffer = 'O'
};

enum MDUpdateAction : uInt8 {
   New = 0,
   Change = 1,
   Delete = 2,
   DeleteThru = 3,
   DeleteFrom = 4,
   Overlay = 5
};
enum OpenCloseSettlFlag : std::uint8_t {
   DailyOpenPrice = 0,
   IndicativeOpeningPrice = 5
};
enum OrderUpdateAction : uInt8 {
   OrderUpdateAction_New = 0,
   OrderUpdateAction_Update = 1,
   OrderUpdateAction_Delete = 2
};
// enum PutOrCall : uInt8
//{
//    Put = 0,
//    Call = 1
// };
enum SecurityTradingEvent : uInt8 {
   NoEvent = 0,
   NoCancel = 1,
   ResetStatistics = 4,
   ImpliedMatchingON = 5,
   ImpliedMatchingOFF = 6
};
enum SecurityTradingStatus : std::uint8_t {
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
   NoChange = 103
};
// enum SecurityUpdateAction : CHAR
//{
//    Add = 'A',
//    Delete = 'D',
//    Modify = 'M'
// };
// enum InstAttribValue : uInt32
//{
//    ElectronicMatchEligible = 0,
//    OrderCrossEligible = 1,
//    BlockTradeEligible = 2,
//    EFPEligible = 3,
//    EBFEligible = 4,
//    EFSEligible = 5,
//    EFREligible = 6,
//    OTCEligible = 7,
//    iLinkIndicativeMassQuotingEligible = 8,
//    NegativeStrikeEligible = 9,
//    NegativePriceOutrightEligible = 10,
//    IsFractional = 11,
//    VolatilityQuotedOption = 12,
//    RFQCrossEligible = 13,
//    ZeroPriceOutrightEligible = 14,
//    DecayingProductEligibility = 15,
//    VariableProductEligibility = 16,
//    DailyProductEligibility = 17,
//    GTOrdersEligibility = 18,
//    ImpliedMatchingEligibility = 19,
//    TriangulationEligible = 20,
//    VariableCabEligible = 21
// };
enum MatchEventIndicator : uInt8 {
   LastTradeMsg = 0,
   LastVolumeMsg = 1,
   LastQuoteMsg = 2,
   LastStatsMsg = 3,
   LastImpliedMsg = 4,
   RecoveryMsg = 5,
   Reserved = 6,
   EndOfEvent = 7
};
enum SettlPriceType : uInt8 {
   FinalDaily = 0,
   Actual = 1,
   Rounded = 2,
   Intraday = 3,
   ReservedBits = 4,
   NullValue = 7
};

struct SecurityStatus30 {
   std::uint16_t msg_size;
   std::uint16_t block_length;
   std::uint16_t template_id = 30;
   std::uint16_t schema_id;
   std::uint16_t version;
   uInt64 transactTime;                         // Start of event processing time in number of nanoseconds since Unix epoch.
   SecurityGroup securityGroup;                 // Security Group
   Asset asset;                                 // Product Code within Security Group specified
   Int32NULL securityID;                        // If this tag is present, 35=f message is sent for the instrument
   LocalMktDate tradeDate;                      // Trade Session Date
   MatchEventIndicator matchEventIndicator;     // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   SecurityTradingStatus securityTradingStatus; // Identifies the trading status applicable to the instrument or Security Group
   HaltReason haltReason;                       // Identifies the reason for the status change
   SecurityTradingEvent securityTradingEvent;   // Identifies an additional event or a rule related to the status
};

struct MDIncrementalRefreshVolume37 {
   // https://cmegroupclientsite.atlassian.net/wiki/spaces/EPICSANDBOX/pages/457325932/MDP+3.0+-+SBE+Message+Header
   std::uint16_t msg_size;
   std::uint16_t block_length;
   std::uint16_t template_id = 37;
   std::uint16_t schema_id;
   std::uint16_t version;
   uInt64 transactTime;                     // Start of event processing time in number of nanoseconds since Unix epoch
   MatchEventIndicator matchEventIndicator; // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   tc::padding<2> padding;
   ////<----------group NoMDEntries
   // Number of entries in Market Data message
   struct NoMDEntries {
      Int32 mDEntrySize;             // Cumulative traded volume
      Int32 securityID;              // Security ID
      uInt32 rptSeq;                 // Market Data entry sequence number per instrument update
      MDUpdateAction mDUpdateAction; // Market Data update action
      // MDEntryTypeVol mDEntryType;    // Electronic Volume entry provides cumulative session trade volume updated with the event
      tc::padding<3> padding;
   };
   sbe::group<NoMDEntries, groupSize> noMDEntries;
};

struct SnapshotFullRefresh38 {
   std::uint16_t msg_size;
   std::uint16_t block_length;
   std::uint16_t template_id = 38;
   std::uint16_t schema_id;
   std::uint16_t version;
   uInt32 lastMsgSeqNumProcessed;                 // Sequence number of the last Incremental feed packet processed. This value is used to synchronize the snapshot loop with the real-time feed
   uInt32 totNumReports;                          // Total number of messages replayed in the loop
   Int32 securityID;                              // Security ID
   uInt32 rptSeq;                                 // Sequence number of the last Market Data entry processed for the instrument
   uInt64 transactTime;                           // Timestamp of the last event security participated in, sent as number of nanoseconds since Unix epoch
   uInt64 lastUpdateTime;                         // UTC Date and time of last Security Definition add, update or delete on a given Market Data channel
   LocalMktDate tradeDate;                        // Trade session date sent as number of days since Unix epoch
   SecurityTradingStatus mDSecurityTradingStatus; // Identifies the current trading state of the instrument
   PRICENULL highLimitPrice;                      // Upper price threshold for the instrument
   PRICENULL lowLimitPrice;                       // Lower price threshold for the instrument
   PRICENULL maxPriceVariation;                   // Differential value for price banding
                                                  ////<----------group NoMDEntries
   // Number of entries in Market Data message
   struct NoMDEntries {
      PRICENULL mDEntryPx;                   // Market Data entry price
      Int32NULL mDEntrySize;                 // Market Data entry quantity
      Int32NULL numberOfOrders;              // Aggregate number of orders at the given price level
      Int8NULL mDPriceLevel;                 // Aggregate book position
      LocalMktDate tradingReferenceDate;     // Indicates the date of trade session corresponding to a statistic entry
      OpenCloseSettlFlag openCloseSettlFlag; // Flag describing  Open Price entry
      SettlPriceType settlPriceType;         // Bitmap field of eight Boolean type indicators representing settlement price type
      MDEntryType mDEntryType;               // Market Data entry type
   };
   sbe::group<NoMDEntries, groupSize> noMDEntries;
};
struct QuoteRequest39 {
   std::uint16_t msg_size;
   std::uint16_t block_length;
   std::uint16_t template_id = 39;
   std::uint16_t schema_id;
   std::uint16_t version;
   uInt64 transactTime;                     // Start of event processing time in number of nanoseconds since Unix epoch
   QuoteReqId quoteReqID;                   // Quote Request ID defined by the exchange
   MatchEventIndicator matchEventIndicator; // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   tc::padding<3> padding;
   ////<----------group NoRelatedSym
   // Indicates the number of repeating symbols specified
   struct NoRelatedSym {
      Symbol symbol;      // Instrument Name or Symbol
      Int32 securityID;   // Security ID
      Int32NULL orderQty; // Quantity requested
      Int8 quoteType;     // Type of quote requested
      Int8NULL side;      // Side requested
      tc::padding<2> padding;
   };
   sbe::group<NoRelatedSym, groupSize> noRelatedSym;
};

struct MDIncrementalRefreshBook46 {
   std::uint16_t msg_size;
   std::uint16_t block_length;
   std::uint16_t template_id = 46;
   std::uint16_t schema_id;
   std::uint16_t version;
   uInt64 transactTime;                     // Start of event processing time in number of nanoseconds since Unix epoch
   MatchEventIndicator matchEventIndicator; // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   tc::padding<2> padding;
   ////<----------group NoMDEntries
   // Number of entries in Market Data message
   struct NoMDEntries {
      PRICENULL9 mDEntryPx;          // Market Data entry price
      Int32NULL mDEntrySize;         // Market Data entry size
      Int32 securityID;              // Security ID
      uInt32 rptSeq;                 // Market Data entry sequence number per instrument update
      Int32NULL numberOfOrders;      // In Book entry - aggregate number of orders at given price level
      uInt8 mDPriceLevel;            // Aggregate book level
      MDUpdateAction mDUpdateAction; //  Market Data update action
      MDEntryTypeBook mDEntryType;   // Market Data entry type
      tc::padding<5> padding;
   };
   sbe::group<NoMDEntries, groupSize> noMDEntries;
   ////<----------group NoOrderIDEntries
   // Number of OrderID entries
   struct NoOrderIDEntries {
      uInt64 orderID;                      // Unique Order ID
      uInt64NULL mDOrderPriority;          // Order priority for execution on the order book
      Int32NULL mDDisplayQty;              // Visible qty of order
      uInt8NULL referenceID;               // Reference to corresponding Price and Security ID, sequence of MD entry in the message
      OrderUpdateAction orderUpdateAction; // Order book update action to be applied to the order referenced by OrderID
      tc::padding<2> padding;
   };
   sbe::group<NoOrderIDEntries, groupSize8Byte> noOrderIDEntries; // TODO:parse group size correctly!!!
};
struct MDIncrementalRefreshOrderBook47 {
   std::uint16_t msg_size;
   std::uint16_t block_length;
   std::uint16_t template_id = 47;
   std::uint16_t schema_id;
   std::uint16_t version;
   uInt64 transactTime;                     // Start of event processing time in number of nanoseconds since Unix epoch
   MatchEventIndicator matchEventIndicator; // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   tc::padding<2> padding;
   ////<----------group NoMDEntries
   // Number of entries in Market Data message
   struct NoMDEntries {
      uInt64NULL orderID;            // Order ID
      uInt64NULL mDOrderPriority;    // Order priority for execution on the order book
      PRICENULL9 mDEntryPx;          // Order price
      Int32NULL mDDisplayQty;        // Visible order qty
      Int32 securityID;              // Security ID
      MDUpdateAction mDUpdateAction; // Order book update action to be applied to the order referenced by OrderID
      MDEntryTypeBook mDEntryType;   // Market Data entry type
      tc::padding<6> padding;
   };
   sbe::group<NoMDEntries, groupSize> noMDEntries;
};
struct MDIncrementalRefreshSessionStatistics51 {
   std::uint16_t msg_size;
   std::uint16_t block_length;
   std::uint16_t template_id = 51;
   std::uint16_t schema_id;
   std::uint16_t version;
   uInt64 transactTime;                     // Start of event processing time in number of nanoseconds since Unix epoch
   MatchEventIndicator matchEventIndicator; // Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event
   tc::padding<2> padding;
   ////<----------group NoMDEntries
   // Number of entries in Market Data message
   struct NoMDEntries {
      PRICE9 mDEntryPx;                      // Market Data entry price
      Int32 securityID;                      // Security ID
      uInt32 rptSeq;                         // MD Entry sequence number per instrument update
      OpenCloseSettlFlag openCloseSettlFlag; // Flag describing IOP and Open Price entries
      MDUpdateAction mDUpdateAction;         // Market Data update action
      MDEntryTypeStatistics mDEntryType;     // Market Data entry type
      Int32NULL mDEntrySize;                 // Indicative Opening Quantity
      tc::padding<1> padding;
   };
   sbe::group<NoMDEntries, groupSize> noMDEntries;
};

} // namespace cme_test

#include <boost/describe.hpp>

namespace cme_test {

BOOST_DESCRIBE_STRUCT(groupSize, (), (blockLength, numInGroup))
BOOST_DESCRIBE_STRUCT(groupSize8Byte, (), (blockLength, padding, numInGroup))
BOOST_DESCRIBE_STRUCT(PRICE, (), (mantissa))
BOOST_DESCRIBE_STRUCT(PRICE9, (), (mantissa))
BOOST_DESCRIBE_STRUCT(PRICENULL, (), (mantissa))
BOOST_DESCRIBE_STRUCT(PRICENULL9, (), (mantissa))

BOOST_DESCRIBE_STRUCT(SecurityStatus30, (),
                      (
                          msg_size,
                          block_length,
                          template_id,
                          schema_id,
                          version,
                          transactTime,
                          securityGroup,
                          asset,
                          securityID,
                          tradeDate,
                          matchEventIndicator,
                          securityTradingStatus,
                          haltReason,
                          securityTradingEvent))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshVolume37::NoMDEntries, (),
                      (
                          mDEntrySize,
                          securityID,
                          rptSeq,
                          mDUpdateAction,
                          // MDEntryTypeVol mDEntryType;    // Electronic Volume entry provides cumulative session trade volume updated with the event
                          padding))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshVolume37, (),
                      (
                          msg_size,
                          block_length,
                          template_id,
                          schema_id,
                          version,
                          transactTime,
                          matchEventIndicator,
                          padding,
                          noMDEntries))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshBook46::NoMDEntries, (),
                      (
                          mDEntryPx,      // Market Data entry price
                          mDEntrySize,    // Market Data entry size
                          securityID,     // Security ID
                          rptSeq,         // Market Data entry sequence number per instrument update
                          numberOfOrders, // In Book entry - aggregate number of orders at given price level
                          mDPriceLevel,   // Aggregate book level
                          mDUpdateAction, //  Market Data update action
                          mDEntryType,    // Market Data entry type
                          padding))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshBook46::NoOrderIDEntries, (),
                      (
                          orderID,           // Unique Order ID
                          mDOrderPriority,   // Order priority for execution on the order book
                          mDDisplayQty,      // Visible qty of order
                          referenceID,       // Reference to corresponding Price and Security ID, sequence of MD entry in the message
                          orderUpdateAction, // Order book update action to be applied to the order referenced by OrderID
                          padding))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshBook46, (),
                      (msg_size,
                       block_length,
                       template_id,
                       schema_id,
                       version,
                       transactTime,
                       matchEventIndicator,
                       padding,
                       noMDEntries,
                       noOrderIDEntries))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshOrderBook47::NoMDEntries, (),
                      (
                          orderID,         // Order ID
                          mDOrderPriority, // Order priority for execution on the order book
                          mDEntryPx,       // Order price
                          mDDisplayQty,    // Visible order qty
                          securityID,      // Security ID
                          mDUpdateAction,  // Order book update action to be applied to the order referenced by OrderID
                          mDEntryType,     // Market Data entry type
                          padding))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshOrderBook47, (),
                      (msg_size,
                       block_length,
                       template_id,
                       schema_id,
                       version,
                       transactTime,
                       matchEventIndicator,
                       padding,
                       noMDEntries))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshSessionStatistics51::NoMDEntries, (),
                      (mDEntryPx,          // Market Data entry price
                       securityID,         // Security ID
                       rptSeq,             // MD Entry sequence number per instrument update
                       openCloseSettlFlag, // Flag describing IOP and Open Price entries
                       mDUpdateAction,     // Market Data update action
                       mDEntryType,        // Market Data entry type
                       mDEntrySize,        // Indicative Opening Quantity
                       padding))

BOOST_DESCRIBE_STRUCT(MDIncrementalRefreshSessionStatistics51, (),
                      (msg_size,
                       block_length,
                       template_id,
                       schema_id,
                       version,
                       transactTime,
                       matchEventIndicator,
                       padding,
                       noMDEntries))

} // namespace cme_test
