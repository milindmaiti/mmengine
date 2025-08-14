#include "utility/macros.h"
#include <gtest/gtest.h>

TEST(MoveEncodingTest, srcTest) {
  int moveEncoding = BitUtil::encode_move(Notation::a2, Notation::a4,
                                          Notation::P, 15, 0, 0, 1, 0);
  EXPECT_EQ(BitUtil::decode_src(moveEncoding), Notation::a2);
  moveEncoding = BitUtil::encode_move(Notation::b3, Notation::g8, Notation::b,
                                      15, 0, 0, 0, 0);
  EXPECT_EQ(BitUtil::decode_src(moveEncoding), Notation::b3);
}

TEST(MoveEncodingTest, dstTest) {
  int moveEncoding = BitUtil::encode_move(Notation::a2, Notation::a4,
                                          Notation::P, 15, 0, 0, 1, 0);
  EXPECT_EQ(BitUtil::decode_dst(moveEncoding), Notation::a4);
  moveEncoding = BitUtil::encode_move(Notation::b3, Notation::g8, Notation::b,
                                      15, 0, 0, 0, 0);
  EXPECT_EQ(BitUtil::decode_dst(moveEncoding), Notation::g8);
}
