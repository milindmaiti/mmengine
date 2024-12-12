#include "../bitboard/bitboard.h"
#include "../utility/boardnotation.h"
#include "../utility/macros.h"
#include "../utility/parsefen.h"
#include <gtest/gtest.h>
#include <string>

TEST(generateMoves, whitePawns) {
  Game chess(rookMagics, bishopMagics);
  std::string curPosition =
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq b5 0 1";
  parse_fen(chess, curPosition);

  printBoard(chess);
  chess.generate_moves();
  ASSERT_EQ(1, 1);
}
