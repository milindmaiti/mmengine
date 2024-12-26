#include "../bitboard/bitboard.h"
#include "../search/evaluation.h"
#include "../search/negamax.h"
#include "../uci/parsecommands.h"
#include "../utility/evaluationtables.h"
#include "../utility/macros.h"
#include "parsefen.h"
#include <gtest/gtest.h>

TEST(evaluatePieces, startPosition) {
  Game chess(rookMagics, bishopMagics);
  parse_fen(chess, startPosition);
  ull eval = evaluatePosition(chess);
  EXPECT_EQ(eval, 0ULL);

  std::string modifiedStart =
      "rnbqkbnr/ppp2ppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  parse_fen(chess, modifiedStart);
  eval = evaluatePosition(chess);
  EXPECT_EQ(eval, 180ULL);
}

TEST(evaluatePieces, mateInOne) {
  Game chess(rookMagics, bishopMagics);
  parse_fen(chess, startPosition);
  ull eval = evaluatePosition(chess);
  EXPECT_EQ(eval, 0ULL);

  std::string modifiedStart = "4k3/1Q6/3K4/8/8/8/8/8 w - - 0 1";
  parse_fen(chess, modifiedStart);
  int bestMove;
  negamaxDriver(chess, 1, bestMove);
  EXPECT_EQ(moveToUciMove(bestMove), "b7e7");
}
