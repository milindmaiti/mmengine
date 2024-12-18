#include "../bitboard/bitboard.h"
#include "../utility/boardnotation.h"
#include "../utility/macros.h"
#include "../utility/parsefen.h"
#include <gtest/gtest.h>
#include <string>

TEST(generateMoves, trickyPositionWhite) {
  Game chess(rookMagics, bishopMagics);
  std::string curPosition =
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq b5 0 1";
  parse_fen(chess, curPosition);

  /*printBoard(chess);*/
  auto moveList = chess.generate_moves();

  std::vector<int> masterMovesList;
  masterMovesList.push_back(encode_move(a2, a3, P, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a2, a4, P, 0, 0, 0, 1, 0));
  masterMovesList.push_back(encode_move(b2, b3, P, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(d5, d6, P, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(d5, e6, P, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(g2, g3, P, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(g2, g4, P, 0, 0, 0, 1, 0));
  masterMovesList.push_back(encode_move(g2, h3, P, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(c3, a4, N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(c3, b5, N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(c3, b1, N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(c3, d1, N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e5, c6, N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e5, c4, N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e5, d3, N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e5, g4, N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e5, g6, N, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(e5, f7, N, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(e5, d7, N, 0, 0, 1, 0, 0));

  masterMovesList.push_back(encode_move(d2, c1, B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(d2, e3, B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(d2, f4, B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(d2, g5, B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(d2, h6, B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e2, f1, B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e2, d1, B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e2, d3, B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e2, c4, B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e2, b5, B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e2, a6, B, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(a1, b1, R, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a1, c1, R, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a1, d1, R, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(h1, g1, R, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(h1, f1, R, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f3, d3, Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f3, e3, Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f3, g3, Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f3, g4, Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f3, f4, Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f3, f5, Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f3, h5, Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f3, h3, Q, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(f3, f6, Q, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(e1, f1, K, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e1, d1, K, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e1, g1, K, 0, 1, 0, 0, 0));
  masterMovesList.push_back(encode_move(e1, c1, K, 0, 1, 0, 0, 0));

  sort(moveList.begin(), moveList.end());
  sort(masterMovesList.begin(), masterMovesList.end());

  ASSERT_EQ(moveList.size(), masterMovesList.size());
  for (int i = 0; i < (int)moveList.size(); i++) {
    EXPECT_EQ(moveList[i], masterMovesList[i]);
  }
}

TEST(generateMoves, trickyPositionBlack) {
  Game chess(rookMagics, bishopMagics);
  std::string curPosition =
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq b5 0 1";
  parse_fen(chess, curPosition);

  /*printBoard(chess);*/
  auto moveList = chess.generate_moves();

  std::vector<int> masterMovesList;
  masterMovesList.push_back(encode_move(b4, b3, p, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(b4, c3, p, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(c7, c6, p, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(c7, c5, p, 0, 0, 0, 1, 0));
  masterMovesList.push_back(encode_move(d7, d6, p, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e6, d5, p, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(g6, g5, p, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(h3, g2, p, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(b6, a4, n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(b6, c8, n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(b6, c4, n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f6, g8, n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f6, h7, n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f6, h5, n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f6, g4, n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(f6, e4, n, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(f6, d5, n, 0, 0, 1, 0, 0));

  masterMovesList.push_back(encode_move(b6, d5, n, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(g7, h6, b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(g7, f8, b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a6, b7, b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a6, c8, b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a6, b5, b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a6, c4, b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a6, d3, b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a6, e2, b, 0, 0, 1, 0, 0));
  masterMovesList.push_back(encode_move(a8, b8, r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a8, c8, r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(a8, d8, r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(h8, f8, r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(h8, g8, r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(h8, h7, r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(h8, h6, r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(h8, h5, r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(h8, h4, r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e7, f8, q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e7, d8, q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e7, d6, q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e7, c5, q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e8, d8, k, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e8, f8, k, 0, 0, 0, 0, 0));
  masterMovesList.push_back(encode_move(e8, g8, k, 0, 1, 0, 0, 0));
  masterMovesList.push_back(encode_move(e8, c8, k, 0, 1, 0, 0, 0));

  sort(moveList.begin(), moveList.end());
  sort(masterMovesList.begin(), masterMovesList.end());
  /*printMoves(moveList);*/
  /*printMoves(masterMovesList);*/
  ASSERT_EQ(moveList.size(), masterMovesList.size());
  for (int i = 0; i < (int)moveList.size(); i++) {
    EXPECT_EQ(moveList[i], masterMovesList[i]);
  }
}

TEST(generateMoves, promotionTest) {
  std::string promoPosition = "r3k3/1P6/8/1b3pP1/8/8/8/R3K2R w KQq f6 0 1";
  Game game(rookMagics, bishopMagics);
  parse_fen(game, promoPosition);
  auto moveList = game.generate_moves();
  printMoves(moveList);
}
