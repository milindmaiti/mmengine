#include "../utility/macros.h"
#include <gtest/gtest.h>

TEST(MoveEncodingTest, srcTest) {
  int moveEncoding = encode_move(a2, a4, P, 15, 0, 0, 1, 0);
  EXPECT_EQ(decode_src(moveEncoding), a2);
  moveEncoding = encode_move(b3, g8, b, 15, 0, 0, 0, 0);
  EXPECT_EQ(decode_src(moveEncoding), b3);
}

TEST(MoveEncodingTest, dstTest) {
  int moveEncoding = encode_move(a2, a4, P, 15, 0, 0, 1, 0);
  EXPECT_EQ(decode_dst(moveEncoding), a4);
  moveEncoding = encode_move(b3, g8, b, 15, 0, 0, 0, 0);
  EXPECT_EQ(decode_dst(moveEncoding), g8);
}
