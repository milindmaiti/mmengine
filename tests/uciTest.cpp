#include "../bitboard/bitboard.h"
#include "../uci/parsecommands.h"
#include "../utility/parsefen.h"
#include "macros.h"
#include <gtest/gtest.h>

TEST(uciTest, parseMoveTest) {
  Game game(rookMagics, bishopMagics);
  parse_fen(game, startPosition);
  std::string move = "e2e4";
  int gameMove = parse_move(game, move);
  EXPECT_EQ(decode_src(gameMove), e2);
  EXPECT_EQ(decode_dst(gameMove), e4);
  EXPECT_EQ(decode_double_push(gameMove), 1);

  move = "e2e5";
  gameMove = parse_move(game, move);
  EXPECT_EQ(gameMove, 0);
}
