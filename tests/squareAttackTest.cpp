#include "bitboard/bitboard.h"
#include "utility/boardnotation.h"
#include "utility/parsefen.h"
#include "movegen/bishopmove.h"
#include "utility/macros.h"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(SquareAttackTest, StartingPosition) {
  Game chess(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);
  parse_fen(chess, ArrayUtil::startPosition);

  ASSERT_EQ(chess.is_square_attacked(Notation::h3, Notation::white), true);
  ASSERT_EQ(chess.is_square_attacked(Notation::a3, Notation::white), true);
  ASSERT_EQ(chess.is_square_attacked(Notation::a4, Notation::white), false);
  ASSERT_EQ(chess.is_square_attacked(Notation::a6, Notation::white), false);
  ASSERT_EQ(chess.is_square_attacked(Notation::a6, Notation::black), true);
}

TEST(SquareAttackTest, CMKPosition) {
  Game chess(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);
  parse_fen(chess, ArrayUtil::cmkPosition);

  ASSERT_EQ(chess.is_square_attacked(Notation::f5, Notation::white), true);
  ASSERT_EQ(chess.is_square_attacked(Notation::c3, Notation::white), true);
  ASSERT_EQ(chess.is_square_attacked(Notation::d6, Notation::white), false);

  ASSERT_EQ(chess.is_square_attacked(Notation::h6, Notation::white), true);
  ASSERT_EQ(chess.is_square_attacked(Notation::b1, Notation::white), true);

  ASSERT_EQ(chess.is_square_attacked(Notation::h8, Notation::black), true);
  ASSERT_EQ(chess.is_square_attacked(Notation::a5, Notation::black), true);
}
