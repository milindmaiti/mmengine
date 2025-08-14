#include "bitboard/bitboard.h"
#include "uci/parsecommands.h"
#include "utility/parsefen.h"
#include "utility/macros.h"
#include <gtest/gtest.h>

TEST(uciTest, parseMoveTest) {
  Game game(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);
  parse_fen(game, ArrayUtil::startPosition);
  std::string move = "e2e4";
  int gameMove = parse_move(game, move);
  EXPECT_EQ(BitUtil::decode_src(gameMove), Notation::e2);
  EXPECT_EQ(BitUtil::decode_dst(gameMove), Notation::e4);
  EXPECT_EQ(BitUtil::decode_double_push(gameMove), 1);

  move = "e2e5";
  gameMove = parse_move(game, move);
  EXPECT_EQ(gameMove, 0);
}
