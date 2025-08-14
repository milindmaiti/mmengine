#include "bitboard/bitboard.h"
#include "utility/boardnotation.h"
#include "utility/macros.h"
#include "utility/parsefen.h"
#include <gtest/gtest.h>
#include <string>
#include <algorithm>

TEST(generateMoves, trickyPositionWhite) {
  Game chess(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);
  std::string curPosition =
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq b5 0 1";
  parse_fen(chess, curPosition);

  /*print_board(chess);*/
  auto moveList = chess.generate_moves();

  std::vector<int> masterMovesList;
  masterMovesList.push_back(BitUtil::encode_move(Notation::a2, Notation::a3,
                                                 Notation::P, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a2, Notation::a4,
                                                 Notation::P, 0, 0, 0, 1, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::b2, Notation::b3,
                                                 Notation::P, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::d5, Notation::d6,
                                                 Notation::P, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::d5, Notation::e6,
                                                 Notation::P, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::g2, Notation::g3,
                                                 Notation::P, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::g2, Notation::g4,
                                                 Notation::P, 0, 0, 0, 1, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::g2, Notation::h3,
                                                 Notation::P, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::c3, Notation::a4,
                                                 Notation::N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::c3, Notation::b5,
                                                 Notation::N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::c3, Notation::b1,
                                                 Notation::N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::c3, Notation::d1,
                                                 Notation::N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e5, Notation::c6,
                                                 Notation::N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e5, Notation::c4,
                                                 Notation::N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e5, Notation::d3,
                                                 Notation::N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e5, Notation::g4,
                                                 Notation::N, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e5, Notation::g6,
                                                 Notation::N, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e5, Notation::f7,
                                                 Notation::N, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e5, Notation::d7,
                                                 Notation::N, 0, 0, 1, 0, 0));

  masterMovesList.push_back(BitUtil::encode_move(Notation::d2, Notation::c1,
                                                 Notation::B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::d2, Notation::e3,
                                                 Notation::B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::d2, Notation::f4,
                                                 Notation::B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::d2, Notation::g5,
                                                 Notation::B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::d2, Notation::h6,
                                                 Notation::B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e2, Notation::f1,
                                                 Notation::B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e2, Notation::d1,
                                                 Notation::B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e2, Notation::d3,
                                                 Notation::B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e2, Notation::c4,
                                                 Notation::B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e2, Notation::b5,
                                                 Notation::B, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e2, Notation::a6,
                                                 Notation::B, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a1, Notation::b1,
                                                 Notation::R, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a1, Notation::c1,
                                                 Notation::R, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a1, Notation::d1,
                                                 Notation::R, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::h1, Notation::g1,
                                                 Notation::R, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::h1, Notation::f1,
                                                 Notation::R, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f3, Notation::d3,
                                                 Notation::Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f3, Notation::e3,
                                                 Notation::Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f3, Notation::g3,
                                                 Notation::Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f3, Notation::g4,
                                                 Notation::Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f3, Notation::f4,
                                                 Notation::Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f3, Notation::f5,
                                                 Notation::Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f3, Notation::h5,
                                                 Notation::Q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f3, Notation::h3,
                                                 Notation::Q, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f3, Notation::f6,
                                                 Notation::Q, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e1, Notation::f1,
                                                 Notation::K, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e1, Notation::d1,
                                                 Notation::K, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e1, Notation::g1,
                                                 Notation::K, 0, 1, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e1, Notation::c1,
                                                 Notation::K, 0, 1, 0, 0, 0));

  std::sort(moveList.begin(), moveList.end());
  std::sort(masterMovesList.begin(), masterMovesList.end());

  ASSERT_EQ(moveList.size(), masterMovesList.size());
  for (ull i = 0; i < moveList.size(); i++) {
    EXPECT_EQ(moveList[i], masterMovesList[i]);
  }
}

TEST(generateMoves, trickyPositionBlack) {
  Game chess(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);
  std::string curPosition =
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq b5 0 1";
  parse_fen(chess, curPosition);

  /*print_board(chess);*/
  auto moveList = chess.generate_moves();

  std::vector<int> masterMovesList;
  masterMovesList.push_back(BitUtil::encode_move(Notation::b4, Notation::b3,
                                                 Notation::p, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::b4, Notation::c3,
                                                 Notation::p, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::c7, Notation::c6,
                                                 Notation::p, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::c7, Notation::c5,
                                                 Notation::p, 0, 0, 0, 1, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::d7, Notation::d6,
                                                 Notation::p, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e6, Notation::d5,
                                                 Notation::p, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::g6, Notation::g5,
                                                 Notation::p, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::h3, Notation::g2,
                                                 Notation::p, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::b6, Notation::a4,
                                                 Notation::n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::b6, Notation::c8,
                                                 Notation::n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::b6, Notation::c4,
                                                 Notation::n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f6, Notation::g8,
                                                 Notation::n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f6, Notation::h7,
                                                 Notation::n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f6, Notation::h5,
                                                 Notation::n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f6, Notation::g4,
                                                 Notation::n, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f6, Notation::e4,
                                                 Notation::n, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::f6, Notation::d5,
                                                 Notation::n, 0, 0, 1, 0, 0));

  masterMovesList.push_back(BitUtil::encode_move(Notation::b6, Notation::d5,
                                                 Notation::n, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::g7, Notation::h6,
                                                 Notation::b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::g7, Notation::f8,
                                                 Notation::b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a6, Notation::b7,
                                                 Notation::b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a6, Notation::c8,
                                                 Notation::b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a6, Notation::b5,
                                                 Notation::b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a6, Notation::c4,
                                                 Notation::b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a6, Notation::d3,
                                                 Notation::b, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a6, Notation::e2,
                                                 Notation::b, 0, 0, 1, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a8, Notation::b8,
                                                 Notation::r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a8, Notation::c8,
                                                 Notation::r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::a8, Notation::d8,
                                                 Notation::r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::h8, Notation::f8,
                                                 Notation::r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::h8, Notation::g8,
                                                 Notation::r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::h8, Notation::h7,
                                                 Notation::r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::h8, Notation::h6,
                                                 Notation::r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::h8, Notation::h5,
                                                 Notation::r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::h8, Notation::h4,
                                                 Notation::r, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e7, Notation::f8,
                                                 Notation::q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e7, Notation::d8,
                                                 Notation::q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e7, Notation::d6,
                                                 Notation::q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e7, Notation::c5,
                                                 Notation::q, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e8, Notation::d8,
                                                 Notation::k, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e8, Notation::f8,
                                                 Notation::k, 0, 0, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e8, Notation::g8,
                                                 Notation::k, 0, 1, 0, 0, 0));
  masterMovesList.push_back(BitUtil::encode_move(Notation::e8, Notation::c8,
                                                 Notation::k, 0, 1, 0, 0, 0));

  std::sort(moveList.begin(), moveList.end());
  std::sort(masterMovesList.begin(), masterMovesList.end());
  /*print_moves(moveList);*/
  /*print_moves(masterMovesList);*/
  ASSERT_EQ(moveList.size(), masterMovesList.size());
  for (ull i = 0; i < moveList.size(); i++) {
    EXPECT_EQ(moveList[i], masterMovesList[i]);
  }
}

TEST(generateMoves, promotionTest) {
  std::string promoPosition = "r3k3/1P6/8/1b3pP1/8/8/8/R3K2R w KQq f6 0 1";
  Game game(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);
  parse_fen(game, promoPosition);
  auto moveList = game.generate_moves();
  print_moves(moveList);
}
