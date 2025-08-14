#include "bitboard/bitboard.h"
#include "utility/parsefen.h"
#include "utility/macros.h"
#include <gtest/gtest.h>

TEST(EnPassant, CaptureFlagSet) {
  Game game(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);
  std::string fen = "8/8/8/3pP3/8/8/8/8 w - d6 0 1";
  parse_fen(game, fen);
  auto moves = game.generate_moves();
  int expected = BitUtil::encode_move(Notation::e5, Notation::d6,
                                      Notation::P, 0, 0, 1, 0, 1);
  bool found = false;
  for (int move : moves) {
    if (move == expected) {
      found = true;
      break;
    }
  }
  ASSERT_TRUE(found);
}
