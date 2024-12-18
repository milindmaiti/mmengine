#include "../bitboard/bitboard.h"
#include "../utility/boardnotation.h"
#include "../utility/macros.h"
#include "../utility/parsefen.h"
#include <gtest/gtest.h>

TEST(makeMoves, trickyPosition) {
  Game chess(rookMagics, bishopMagics);
  std::string curPosition =
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPB1PPP/R2BK2R w KQkq - 0 1";
  parse_fen(chess, curPosition);

  int curMove = encode_move(e1, f1, K, 0, 1, 0, 0, 0);
  int works = chess.makeMove(curMove, false);
  ASSERT_EQ(works, false);

  curMove = encode_move(e1, e2, K, 0, 1, 0, 0, 0);
  ASSERT_EQ(chess.makeMove(curMove, false), false);
  curMove = encode_move(f3, e2, Q, 0, 0, 0, 0, 0);
  ASSERT_EQ(chess.makeMove(curMove, true), false);

  ASSERT_EQ(chess.makeMove(curMove, false), true);
  ASSERT_EQ(chess.makeMove(encode_move(a6, e2, b, 0, 0, 1, 0, 0), true), true);
  /*print_board(chess);*/

  chess = Game(rookMagics, bishopMagics);
  curPosition =
      "r3k2r/p2pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q1p/PPPB1PPP/R2BK2R w KQkq c6 0 1";
  parse_fen(chess, curPosition);
  auto moveList = chess.generate_moves();
  /*print_board(chess);*/
  /*print_moves(moveList);*/

  int enPassantMove = encode_move(d5, c6, P, 0, 0, 0, 0, 1);
  ASSERT_EQ(chess.makeMove(enPassantMove, false), true);
  /*print_board(chess);*/
}
