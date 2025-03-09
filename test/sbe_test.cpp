#include "cme_eg.hpp"
#include "sbe.hpp"
#include "transcoder/transcoder.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/describe.hpp>
#include <numeric>
#include <type_traits>

namespace cme_test {
struct packet_header {
   // https://cmegroupclientsite.atlassian.net/wiki/spaces/EPICSANDBOX/pages/457638617/MDP+3.0+-+SBE+Technical+Headers
   std::uint32_t msg_seq_num;
   std::uint64_t sending_time;
};

BOOST_DESCRIBE_STRUCT(packet_header, (),
                      (
                          msg_seq_num,
                          sending_time))

} // namespace cme_test

namespace {
struct cme_test_fixture {
   cme_test_fixture()
       // clang-format off
    :data{
        //https://cmegroupclientsite.atlassian.net/wiki/spaces/EPICSANDBOX/pages/457638617/MDP+3.0+-+SBE+Technical+Headers
        0xe3,0x0c,0x00,0x00, // MsgSeqNum (uInt32)
                            0x3d,0x28,0x4a,0x45,0x0b,0x78,0x72,0x17, //SendingTime (uInt64)
        //https://cmegroupclientsite.atlassian.net/wiki/spaces/EPICSANDBOX/pages/457325932/MDP+3.0+-+SBE+Message+Header
                                                                    0x28,0x00, //MsgSize
                                                                              0x0b,0x00, //BlockLength (11)
        //https://www.cmegroup.com/ftp/SBEFix/Production/Templates/templates_FixBinary.xml
        // <ns2:message name="MDIncrementalRefreshVolume37" id="37" description="MDIncrementalRefreshVolume" blockLength="11" semanticType="X" sinceVersion="2">
        0x25,0x00, // -> MDIncrementalRefreshVolume37 (id=37, version = 9)
                  0x01,0x00, // Schema Id
                            0x09,0x00, // Version 
        // <field name="TransactTime" id="60" type="uInt64" description="Start of event processing time in number of nanoseconds since Unix epoch" offset="0" semanticType="UTCTimestamp"/>
                                      0x00,0xc0,0x1b,0x44,0x0b,0x78,0x72,0x17,
        //<field name="MatchEventIndicator" id="5799" type="MatchEventIndicator" description="Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event" offset="8" semanticType="MultipleCharValue"/>
                                                                              0x02,
        //padding!!!
                                                                                   0x00,
        0x00,
        //<group name="NoMDEntries" id="268" description="Number of entries in Market Data message" blockLength="16" dimensionType="groupSize">
             0x10,0x00, // blockLength="16" 
                       0x01, // NumInGroup
                            0x02,0x00,0x00,0x00, //<field name="MDEntrySize" id="271" type="Int32" description="Cumulative traded volume" offset="0" semanticType="Qty"/>
                                                 0x99,0x16,0x00,0x00, // <field name="SecurityID" id="48" type="Int32" description="Security ID" offset="4" semanticType="int"/>
                                                                     0x2e,0x00,0x00,0x00, //<field name="RptSeq" id="83" type="uInt32" description="Market Data entry sequence number per instrument update" offset="8" semanticType="int"/>
        0x00, //<field name="MDUpdateAction" id="279" type="MDUpdateAction" description="Market Data update action" offset="12" semanticType="int"/>
             0x00,0x00,0x00, // padding - 3
                            0x90,0x00, //MsgSize
                                      0x0b,0x00, //BlockLength (11)
        //https://www.cmegroup.com/ftp/SBEFix/Production/Templates/templates_FixBinary.xml
        //<ns2:message name="MDIncrementalRefreshOrderBook47" id="47" description="MDIncrementalRefreshOrderBook" blockLength="11" semanticType="X" sinceVersion="9">
                                                0x2f,0x00,
                                                          0x01,0x00,// Schema Id
                                                                    0x09,0x00, // Version 
        // <field name="TransactTime" id="60" type="uInt64" description="Start of event processing time in number of nanoseconds since Unix epoch" offset="0" semanticType="UTCTimestamp"/>
                                                                              0x00,0xc0,
        0x1b,0x44,0x0b,0x78,0x72,0x17,
                                      0x04,//<field name="MatchEventIndicator" id="5799" type="MatchEventIndicator" description="Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event" offset="8" semanticType="MultipleCharValue"/>
                                           0x00,0x00, //padding!!!
        //<group name="NoMDEntries" id="268" description="Number of entries in Market Data message" blockLength="32" dimensionType="groupSize">
                                                     0x28,0x00,0x03,0xd4,0x78,0x71,0x34,
        0xad,0x00,0x00,0x00,0xe0,0xb7,0x84,0x64,0x77,0x01,0x00,0x00,0x00,0x68,0xaa,0x55,
        0xbd,0x1b,0x00,0x00,0x62,0x00,0x00,0x00,0x99,0x16,0x00,0x00,0x01,0x30,0x00,0x00,
        0x00,0x00,0x00,0x00,0x56,0x78,0x71,0x34,0xad,0x00,0x00,0x00,0x5d,0xb7,0x84,0x64,
        0x77,0x01,0x00,0x00,0x00,0x80,0x33,0x0d,0xa6,0x1b,0x00,0x00,0x01,0x00,0x00,0x00,
        0x99,0x16,0x00,0x00,0x02,0x31,0x00,0x00,0x00,0x00,0x00,0x00,0x57,0x78,0x71,0x34,
        0xad,0x00,0x00,0x00,0x5e,0xb7,0x84,0x64,0x77,0x01,0x00,0x00,0x00,0x68,0xaa,0x55,
        0xbd,0x1b,0x00,0x00,0x01,0x00,0x00,0x00,0x99,0x16,0x00,0x00,0x02,0x31,0x00,0x00,
        0x00,0x00,0x00,0x00,
                            0x60,0x00,//MsgSize
                                      0x0b,0x00, //BlockLength (11)
        //<ns2:message name="MDIncrementalRefreshBook46" id="46" description="MDIncrementalRefreshBook" blockLength="11" semanticType="X" sinceVersion="9">
                                                0x2e,0x00,
                                                          0x01,0x00,// Schema Id
                                                                    0x09,0x00,// Version
                                                                              0x00,0xc0,
        0x1b,0x44,0x0b,0x78,0x72,0x17,
                                      0x04, //<field name="MatchEventIndicator" id="5799" type="MatchEventIndicator" description="Bitmap field of eight Boolean type indicators reflecting the end of updates for a given Globex event" offset="8" semanticType="MultipleCharValue"/>
                                           0x00,0x00, // padding
        //<group name="NoMDEntries" id="268" description="Number of entries in Market Data message" blockLength="32" dimensionType="groupSize">
                                                     0x20,0x00, //blockLength="32"
                                                               0x02, // NumInGroup
                                                                    0x00,0x68,0xaa,0x55,
        0xbd,0x1b,0x00,0x00,0xa9,0x00,0x00,0x00,0x99,0x16,0x00,0x00,0x2f,0x00,0x00,0x00,
        0x02,0x00,0x00,0x00,0x01,0x01,0x30,0x00,0x00,0x00,0x00,0x00,
                                                                    0x00,0x68,0xaa,0x55,
        0xbd,0x1b,0x00,0x00,0x02,0x00,0x00,0x00,0x99,0x16,0x00,0x00,0x30,0x00,0x00,0x00,
        0x02,0x00,0x00,0x00,0x01,0x02,0x31,0x00,0x00,0x00,0x00,0x00,
        //<group name="NoOrderIDEntries" id="37705" description="Number of OrderID entries" blockLength="24" dimensionType="groupSize8Byte">
                                                                    0x18,0x00, //blockLength="24"
                                                                              0x00,
                                                                                   0x00,
        0x00,0x00,//!! dimensionType="groupSize8Byte" !!!
                  0x00,0x00,
                            0x60,0x00,//MsgSize
                                      0x0b,0x00,//BlockLength (11)
        //<ns2:message name="MDIncrementalRefreshSessionStatistics51" id="51" description="MDIncrementalRefreshSessionStatistics" blockLength="11" semanticType="X" sinceVersion="9">
                                                0x33,0x00,
                                                          0x01,0x00,// Schema Id
                                                                    0x09,0x00, // Version
                                                                              0x00,0xc0,
        0x1b,0x44,0x0b,0x78,0x72,0x17,
                                     0x08,        
                                          0x00,0x00, // padding
//<group name="NoMDEntries" id="268" description="Number of entries in Market Data message" blockLength="24" dimensionType="groupSize">
                                                    0x18,0x00, //blockLength="24"
                                                              0x03,0x00, // groupSize
                                                                         0x68,0xaa,0x55,
        0xbd,0x1b,0x00,0x00,0x99,0x16,0x00,0x00,0x31,0x00,0x00,0x00,0xff,0x00,0x37,0xff,
        0xff,0xff,0x7f,0x00,0x00,
                                 0x68,0xaa,0x55,0xbd,0x1b,0x00,0x00,0x99,0x16,0x00,0x00,
        0x32,0x00,0x00,0x00,0xff,0x00,0x38,0xff,0xff,0xff,0x7f,0x00,0x00,
                                                                         0x68,0xaa,0x55,
        0xbd,0x1b,0x00,0x00,0x99,0x16,0x00,0x00,0x33,0x00,0x00,0x00,0x00,0x00,0x34,0xff,
        0xff,0xff,0x7f,0x00,0x28,
                                 0x00,0x1e,0x00,0x1e,0x00,0x01,0x00,0x09,0x00,0x00,0xc0,
        0x1b,0x44,0x0b,0x78,0x72,0x17,0x42,0x49,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0xff,0xff,0xff,0x7f,0x63,0x4c,0x00,0x0f,0x00,0x00,0x28,0x00,0x1e,0x00,
        0x1e,0x00,0x01,0x00,0x09,0x00,0x00,0xc0,0x1b,0x44,0x0b,0x78,0x72,0x17,0x42,0x58,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x7f,0x63,0x4c,
        0x00,0x0f,0x00,0x00,0x28,0x00,0x1e,0x00,0x1e,0x00,0x01,0x00,0x09,0x00,0x00,0xc0,
        0x1b,0x44,0x0b,0x78,0x72,0x17,0x43,0x49,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0xff,0xff,0xff,0x7f,0x63,0x4c,0x00,0x0f,0x00,0x00,0x28,0x00,0x1e,0x00,
        0x1e,0x00,0x01,0x00,0x09,0x00,0x00,0xc0,0x1b,0x44,0x0b,0x78,0x72,0x17,0x45,0x5a,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x7f,0x63,0x4c,
        0x00,0x0f,0x00,0x00,0x28,0x00,0x1e,0x00,0x1e,0x00,0x01,0x00,0x09,0x00,0x00,0xc0,
        0x1b,0x44,0x0b,0x78,0x72,0x17,0x47,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0xff,0xff,0xff,0x7f,0x63,0x4c,0x00,0x0f,0x00,0x00,0x28,0x00,0x1e,0x00,
        0x1e,0x00,0x01,0x00,0x09,0x00,0x00,0xc0,0x1b,0x44,0x0b,0x78,0x72,0x17,0x4f,0x45,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x7f,0x63,0x4c,
        0x00,0x0f,0x00,0x00,0x28,0x00,0x1e,0x00,0x1e,0x00,0x01,0x00,0x09,0x00,0x00,0xc0,
        0x1b,0x44,0x0b,0x78,0x72,0x17,0x52,0x4a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0xff,0xff,0xff,0x7f,0x63,0x4c,0x00,0x0f,0x00,0x00,0x28,0x00,0x1e,0x00,
        0x1e,0x00,0x01,0x00,0x09,0x00,0x00,0xc0,0x1b,0x44,0x0b,0x78,0x72,0x17,0x52,0x4e,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0x7f,0x63,0x4c,
        0x00,0x0f,0x00,0x00   
    } // clang-format on
   {}
   std::array<unsigned char, 708> data;
};
} // namespace

BOOST_FIXTURE_TEST_SUITE(cme_test_sute, cme_test_fixture)

BOOST_AUTO_TEST_CASE(basic_decode_test) {
   auto begin = reinterpret_cast<tc::byte_t const*>(data.data());
   auto packet_header = tc::decode<cme_test::packet_header>(begin, begin + data.size());
   BOOST_CHECK_EQUAL(packet_header.msg_seq_num, 0x0ce3);
   auto refresh_volume37 = tc::decode<cme_test::MDIncrementalRefreshVolume37>(begin, begin + data.size());
   BOOST_REQUIRE_EQUAL(refresh_volume37.message_header.templateId, 37);
   BOOST_CHECK_EQUAL(refresh_volume37.msg_size, 0x028);
   BOOST_CHECK_EQUAL(refresh_volume37.message_header.blockLength, 0x0b);
   BOOST_CHECK(refresh_volume37.matchEventIndicator == cme_test::MatchEventIndicator::LastQuoteMsg);
   BOOST_REQUIRE_EQUAL(refresh_volume37.noMDEntries.data.size(), 1);
   BOOST_CHECK_EQUAL(refresh_volume37.noMDEntries.data[0].mDEntrySize, 0x02);
   BOOST_CHECK_EQUAL(refresh_volume37.noMDEntries.data[0].securityID, 0x1699);
   BOOST_CHECK_EQUAL(refresh_volume37.noMDEntries.data[0].rptSeq, 0x02e);
   BOOST_CHECK(refresh_volume37.noMDEntries.data[0].mDUpdateAction == cme_test::MDUpdateAction::New);
   auto refresh_order_book47 = tc::decode<cme_test::MDIncrementalRefreshOrderBook47>(begin, begin + data.size());
   BOOST_REQUIRE_EQUAL(refresh_order_book47.message_header.templateId, 47);
   BOOST_CHECK_EQUAL(refresh_order_book47.msg_size, 0x90);
   BOOST_CHECK_EQUAL(refresh_order_book47.message_header.blockLength, 0x0b);
   BOOST_CHECK(refresh_order_book47.matchEventIndicator == cme_test::MatchEventIndicator::LastImpliedMsg);
   BOOST_REQUIRE_EQUAL(refresh_order_book47.noMDEntries.data.size(), 3);
   auto refresh_order_book46 = tc::decode<cme_test::MDIncrementalRefreshBook46>(begin, begin + data.size());
   BOOST_CHECK(refresh_order_book46.matchEventIndicator == cme_test::MatchEventIndicator::LastImpliedMsg);
   BOOST_REQUIRE_EQUAL(refresh_order_book46.message_header.templateId, 46);
   BOOST_REQUIRE_EQUAL(refresh_order_book46.noMDEntries.data.size(), 2);
   BOOST_REQUIRE_EQUAL(refresh_order_book46.noOrderIDEntries.data.size(), 0);
   auto refresh_session_statistics51 = tc::decode<cme_test::MDIncrementalRefreshSessionStatistics51>(begin, begin + data.size());
   //BOOST_CHECK(refresh_session_statistics51.matchEventIndicator, 8);
   BOOST_REQUIRE_EQUAL(refresh_session_statistics51.message_header.templateId, 51);
   BOOST_REQUIRE_EQUAL(refresh_session_statistics51.noMDEntries.data.size(), 3);
   auto security_status30_1 = tc::decode<cme_test::SecurityStatus30>(begin, begin + data.size());
   BOOST_CHECK_EQUAL(security_status30_1.msg_size, 0x028);
   BOOST_CHECK_EQUAL(security_status30_1.message_header.blockLength, 30);
   auto security_status30_2 = tc::decode<cme_test::SecurityStatus30>(begin, begin + data.size());
   BOOST_CHECK_EQUAL(security_status30_2.msg_size, 0x028);
   BOOST_CHECK_EQUAL(security_status30_2.message_header.blockLength, 30);
   auto security_status30_3 = tc::decode<cme_test::SecurityStatus30>(begin, begin + data.size());
   BOOST_CHECK_EQUAL(security_status30_3.msg_size, 0x028);
   BOOST_CHECK_EQUAL(security_status30_3.message_header.blockLength, 30);
}

BOOST_AUTO_TEST_CASE(basic_encode_test) {
   auto encode = [](tc::byte_t const* begin, tc::byte_t const* end) {
      auto packet_header = tc::decode<cme_test::packet_header>(begin, end);
      auto refresh_volume37 = tc::decode<cme_test::MDIncrementalRefreshVolume37>(begin, end);
      auto refresh_order_book47 = tc::decode<cme_test::MDIncrementalRefreshOrderBook47>(begin, end);
      auto refresh_order_book46 = tc::decode<cme_test::MDIncrementalRefreshBook46>(begin, end);
      auto refresh_session_statistics51 = tc::decode<cme_test::MDIncrementalRefreshSessionStatistics51>(begin, end);
      auto security_status30_1 = tc::decode<cme_test::SecurityStatus30>(begin, end);
      auto security_status30_2 = tc::decode<cme_test::SecurityStatus30>(begin, end);
      auto security_status30_3 = tc::decode<cme_test::SecurityStatus30>(begin, end);
      BOOST_CHECK_EQUAL(security_status30_3.msg_size, 0x028);
      BOOST_CHECK_EQUAL(security_status30_3.message_header.blockLength, 30);
      return tc::encode(packet_header, security_status30_1,
                        refresh_volume37 ,
                        security_status30_2,
                        refresh_order_book47,
                        security_status30_3,
                        refresh_order_book46,
                        security_status30_1,
                        refresh_session_statistics51
      );
   };
   auto buffer = encode(reinterpret_cast<tc::byte_t const*>(data.data()), reinterpret_cast<tc::byte_t const*>(data.data()) + data.size());
   BOOST_REQUIRE(!buffer.empty());
   auto begin = buffer.data();
   auto packet_header = tc::decode<cme_test::packet_header>(begin, begin + data.size());
   BOOST_CHECK_EQUAL(packet_header.msg_seq_num, 0x0ce3);
   auto security_status30_1 = tc::decode<cme_test::SecurityStatus30>(begin, begin + data.size());
   BOOST_CHECK_EQUAL(security_status30_1.msg_size, 0x028);
   BOOST_CHECK_EQUAL(security_status30_1.message_header.blockLength, 30);
   auto refresh_volume37 = tc::decode<cme_test::MDIncrementalRefreshVolume37>(begin, begin + data.size());
   BOOST_REQUIRE_EQUAL(refresh_volume37.message_header.templateId, 37);
   BOOST_CHECK_EQUAL(refresh_volume37.msg_size, 0x028);
   BOOST_CHECK_EQUAL(refresh_volume37.message_header.blockLength, 0x0b);
   BOOST_CHECK(refresh_volume37.matchEventIndicator == cme_test::MatchEventIndicator::LastQuoteMsg);
   BOOST_REQUIRE_EQUAL(refresh_volume37.noMDEntries.data.size(), 1);
   BOOST_CHECK_EQUAL(refresh_volume37.noMDEntries.data[0].mDEntrySize, 0x02);
   BOOST_CHECK_EQUAL(refresh_volume37.noMDEntries.data[0].securityID, 0x1699);
   BOOST_CHECK_EQUAL(refresh_volume37.noMDEntries.data[0].rptSeq, 0x02e);
   BOOST_CHECK(refresh_volume37.noMDEntries.data[0].mDUpdateAction == cme_test::MDUpdateAction::New);
   auto security_status30_2 = tc::decode<cme_test::SecurityStatus30>(begin, begin + data.size());
   BOOST_CHECK_EQUAL(security_status30_2.msg_size, 0x028);
   BOOST_CHECK_EQUAL(security_status30_2.message_header.blockLength, 30);
   auto refresh_order_book47 = tc::decode<cme_test::MDIncrementalRefreshOrderBook47>(begin, begin + data.size());
   BOOST_REQUIRE_EQUAL(refresh_order_book47.message_header.templateId, 47);
   BOOST_CHECK_EQUAL(refresh_order_book47.msg_size, 0x90);
   BOOST_CHECK_EQUAL(refresh_order_book47.message_header.blockLength, 0x0b);
   BOOST_CHECK(refresh_order_book47.matchEventIndicator == cme_test::MatchEventIndicator::LastImpliedMsg);
   BOOST_REQUIRE_EQUAL(refresh_order_book47.noMDEntries.data.size(), 3);
   auto security_status30_3 = tc::decode<cme_test::SecurityStatus30>(begin, begin + data.size());
   BOOST_CHECK_EQUAL(security_status30_3.msg_size, 0x028);
   BOOST_CHECK_EQUAL(security_status30_3.message_header.blockLength, 30);
   auto refresh_order_book46 = tc::decode<cme_test::MDIncrementalRefreshBook46>(begin, begin + data.size());
   BOOST_CHECK(refresh_order_book46.matchEventIndicator == cme_test::MatchEventIndicator::LastImpliedMsg);
   BOOST_REQUIRE_EQUAL(refresh_order_book46.message_header.templateId, 46);
   BOOST_REQUIRE_EQUAL(refresh_order_book46.noMDEntries.data.size(), 2);
   BOOST_REQUIRE_EQUAL(refresh_order_book46.noOrderIDEntries.data.size(), 0);
   auto security_status30_4 = tc::decode<cme_test::SecurityStatus30>(begin, begin + data.size());
   BOOST_CHECK_EQUAL(security_status30_4.msg_size, 0x028);
   BOOST_CHECK_EQUAL(security_status30_4.message_header.blockLength, 30);
   auto refresh_session_statistics51 = tc::decode<cme_test::MDIncrementalRefreshSessionStatistics51>(begin, begin + data.size());
   //BOOST_CHECK_EQUAL(refresh_session_statistics51.matchEventIndicator, 8);
   BOOST_REQUIRE_EQUAL(refresh_session_statistics51.message_header.templateId, 51);
   BOOST_REQUIRE_EQUAL(refresh_session_statistics51.noMDEntries.data.size(), 3);
}

BOOST_AUTO_TEST_CASE(basic_decode_messages) {
   auto begin = reinterpret_cast<tc::byte_t const*>(data.data());
   auto packet_header = tc::decode<cme_test::packet_header>(begin, begin + data.size());
   BOOST_CHECK_EQUAL(packet_header.msg_seq_num, 0x0ce3);
   auto variant_out1 = tc::decode<cme_test::messages>(begin, begin + data.size());
   std::visit([&](auto const& refresh_volume37) {
      if constexpr (std::is_same_v<decltype(refresh_volume37), cme_test::MDIncrementalRefreshVolume37 const&>) {
         BOOST_REQUIRE_EQUAL(refresh_volume37.message_header.templateId, 37);
         BOOST_CHECK_EQUAL(refresh_volume37.msg_size, 0x028);
         BOOST_CHECK_EQUAL(refresh_volume37.message_header.blockLength, 0x0b);
         BOOST_CHECK(refresh_volume37.matchEventIndicator == cme_test::MatchEventIndicator::LastQuoteMsg);
         BOOST_REQUIRE_EQUAL(refresh_volume37.noMDEntries.data.size(), 1);
         BOOST_CHECK_EQUAL(refresh_volume37.noMDEntries.data[0].mDEntrySize, 0x02);
         BOOST_CHECK_EQUAL(refresh_volume37.noMDEntries.data[0].securityID, 0x1699);
         BOOST_CHECK_EQUAL(refresh_volume37.noMDEntries.data[0].rptSeq, 0x02e);
         BOOST_CHECK(refresh_volume37.noMDEntries.data[0].mDUpdateAction == cme_test::MDUpdateAction::New);
      } else {
         BOOST_CHECK_MESSAGE(false, "wrong type has been decoded");
      }
   },
              variant_out1);

   auto variant_out2 = tc::decode<cme_test::messages>(begin, begin + data.size());
   std::visit([&](auto const& refresh_order_book47) {
      if constexpr (std::is_same_v<decltype(refresh_order_book47), cme_test::MDIncrementalRefreshOrderBook47 const&>) {
         BOOST_REQUIRE_EQUAL(refresh_order_book47.message_header.templateId, 47);
         BOOST_CHECK_EQUAL(refresh_order_book47.msg_size, 0x90);
         BOOST_CHECK_EQUAL(refresh_order_book47.message_header.blockLength, 0x0b);
         BOOST_CHECK(refresh_order_book47.matchEventIndicator == cme_test::MatchEventIndicator::LastImpliedMsg);
         BOOST_REQUIRE_EQUAL(refresh_order_book47.noMDEntries.data.size(), 3);
      } else {
         BOOST_CHECK_MESSAGE(false, "wrong type has been decoded");
      }
   },
              variant_out2);

   auto variant_out3 = tc::decode<cme_test::messages>(begin, begin + data.size());
   std::visit([&](auto const& refresh_order_book46) {
      if constexpr (std::is_same_v<decltype(refresh_order_book46), cme_test::MDIncrementalRefreshBook46 const&>) {
         BOOST_CHECK(refresh_order_book46.matchEventIndicator == cme_test::MatchEventIndicator::LastImpliedMsg);
         BOOST_REQUIRE_EQUAL(refresh_order_book46.message_header.templateId, 46);
         BOOST_REQUIRE_EQUAL(refresh_order_book46.noMDEntries.data.size(), 2);
         BOOST_REQUIRE_EQUAL(refresh_order_book46.noOrderIDEntries.data.size(), 0);
      } else {
         BOOST_CHECK_MESSAGE(false, "wrong type has been decoded");
      }
   },
              variant_out3);
}

BOOST_AUTO_TEST_SUITE_END()
