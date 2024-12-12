#include "../bitboard/bitboard.h"
#include "../utility/boardnotation.h"
#include "../utility/parsefen.h"
#include "bishopmove.h"
#include "macros.h"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(SquareAttackTest, StartingPosition) {
  Game chess(bishopMagics, rookMagics);
  parse_fen(chess, startPosition);

  ASSERT_EQ(chess.is_square_attacked(h3, white), true);
  ASSERT_EQ(chess.is_square_attacked(a3, white), true);
  ASSERT_EQ(chess.is_square_attacked(a4, white), false);
  ASSERT_EQ(chess.is_square_attacked(a6, white), false);
  ASSERT_EQ(chess.is_square_attacked(a6, black), true);
}

TEST(SquareAttackTest, CMKPosition) {
  Game chess(rookMagics, bishopMagics);
  parse_fen(chess, cmkPosition);

  ASSERT_EQ(chess.is_square_attacked(f5, white), true);
  ASSERT_EQ(chess.is_square_attacked(c3, white), true);
  ASSERT_EQ(chess.is_square_attacked(d6, white), false);

  ASSERT_EQ(chess.is_square_attacked(h6, white), true);
  ASSERT_EQ(chess.is_square_attacked(b1, white), true);

  ASSERT_EQ(chess.is_square_attacked(h8, black), true);
  ASSERT_EQ(chess.is_square_attacked(a5, black), true);
}
