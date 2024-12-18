#include "../bitboard/bitboard.h"
#include "../movegen/bishopmove.h"
#include "../movegen/rookmove.h"
#include "../utility/boardnotation.h"
#include "macros.h"
#include <gtest/gtest.h>
#include <vector>

TEST(PieceTest, BishopTest) {
  Game chess(rookMagics, bishopMagics);
  ull occupancyMask = 0ULL;
  set_bit(occupancyMask, f4);
  ull pred = get_bishop_attack(h6, occupancyMask, chess.bishopMasks,
                               chess.bishopAttacks, chess.bishopMagics);
  ull ans = 0ULL;
  set_bit(ans, f4);
  set_bit(ans, g5);
  set_bit(ans, g7);
  set_bit(ans, f8);
  ASSERT_EQ(ans, pred);

  occupancyMask = 0ULL;
  ans = 0ULL;
  pred = get_bishop_attack(a1, occupancyMask, chess.bishopMasks,
                           chess.bishopAttacks, chess.bishopMagics);
  std::vector<int> moves{b2, c3, d4, e5, f6, g7, h8};
  for (int move : moves)
    set_bit(ans, move);
  ASSERT_EQ(ans, pred);

  occupancyMask = 0ULL;
  set_bit(occupancyMask, h7);
  set_bit(occupancyMask, b1);
  set_bit(occupancyMask, a6);
  set_bit(occupancyMask, f1);
  ans = 0ULL;
  pred = get_bishop_attack(d3, occupancyMask, chess.bishopMasks,
                           chess.bishopAttacks, chess.bishopMagics);
  moves = {a6, b5, c4, c2, b1, e2, f1, e4, f5, g6, h7};
  for (int move : moves)
    set_bit(ans, move);
  ASSERT_EQ(ans, pred);

  occupancyMask = 0ULL;
  set_bit(occupancyMask, g6);
  set_bit(occupancyMask, b1);
  set_bit(occupancyMask, a6);
  set_bit(occupancyMask, f1);
  ans = 0ULL;
  pred = get_bishop_attack(d3, occupancyMask, chess.bishopMasks,
                           chess.bishopAttacks, chess.bishopMagics);
  moves = {a6, b5, c4, c2, b1, e2, f1, e4, f5, g6};
  for (int move : moves)
    set_bit(ans, move);
  ASSERT_EQ(ans, pred);
}

TEST(PieceTest, RookTest) {
  Game chess(rookMagics, bishopMagics);
  ull occupancyMask = 0ULL;
  set_bit(occupancyMask, a5);
  set_bit(occupancyMask, g7);
  set_bit(occupancyMask, g3);
  ull pred = get_rook_attack(g5, occupancyMask, chess.rookMasks,
                             chess.rookAttacks, chess.rookMagics);
  ull ans = 0ULL;
  std::vector<int> moves{a5, b5, c5, d5, e5, f5, h5, g6, g7, g4, g3};
  for (int move : moves)
    set_bit(ans, move);
  ASSERT_EQ(ans, pred);

  occupancyMask = 0ULL;
  ans = 0ULL;
  pred = get_rook_attack(a1, occupancyMask, chess.rookMasks, chess.rookAttacks,
                         chess.rookMagics);
  moves = {a2, a3, a4, a5, a6, a7, a8, b1, c1, d1, e1, f1, g1, h1};
  for (int move : moves)
    set_bit(ans, move);
  ASSERT_EQ(ans, pred);

  occupancyMask = 0ULL;
  ans = 0ULL;
  set_bit(occupancyMask, d8);
  set_bit(occupancyMask, b4);
  set_bit(occupancyMask, f4);
  set_bit(occupancyMask, d2);
  pred = get_rook_attack(d4, occupancyMask, chess.rookMasks, chess.rookAttacks,
                         chess.rookMagics);
  moves = {b4, c4, d5, d6, d7, d8, e4, f4, d3, d2};
  for (int move : moves)
    set_bit(ans, move);
  ASSERT_EQ(ans, pred);
}
