#include "../bitboard/bitboard.h"
#include "../utility/boardnotation.h"
#include "../utility/macros.h"
#include "../utility/parsefen.h"
#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(StartPositionTest, PiecePlacement) {
  ull bitboard = 0ULL;
  Game game;
  // set starting position piece occupancies
  for (int i = a8; i <= h7; i++)
    set_bit(bitboard, i);
  for (int i = a2; i <= h1; i++)
    set_bit(bitboard, i);

  std::string startingPos =
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  parse_fen(game, startingPos);

  print_bitboard(game.occupancyBitboards[both]);
  print_bitboard(bitboard);
  EXPECT_EQ(game.occupancyBitboards[both], bitboard);
}
TEST(HelloTest, BasicAssertions) {

  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
