#include "bitboard/bitboard.h"
#include "movegen/bishopmove.h"
#include "movegen/rookmove.h"
#include "utility/boardnotation.h"
#include "utility/macros.h"
#include <gtest/gtest.h>
#include <vector>

TEST(PieceTest, BishopTest) {
  Game chess(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);
  ull occupancyMask = 0ULL;
  BitUtil::set_bit(occupancyMask, Notation::f4);
  ull pred = get_bishop_attack(Notation::h6, occupancyMask, chess.bishopMasks,
                               chess.bishopAttacks, chess.bishopMagics);
  ull ans = 0ULL;
  BitUtil::set_bit(ans, Notation::f4);
  BitUtil::set_bit(ans, Notation::g5);
  BitUtil::set_bit(ans, Notation::g7);
  BitUtil::set_bit(ans, Notation::f8);
  ASSERT_EQ(ans, pred);

  occupancyMask = 0ULL;
  ans = 0ULL;
  pred = get_bishop_attack(Notation::a1, occupancyMask, chess.bishopMasks,
                           chess.bishopAttacks, chess.bishopMagics);
  std::vector<int> moves{Notation::b2, Notation::c3, Notation::d4, Notation::e5,
                         Notation::f6, Notation::g7, Notation::h8};
  for (int move : moves)
    BitUtil::set_bit(ans, move);
  ASSERT_EQ(ans, pred);

  occupancyMask = 0ULL;
  BitUtil::set_bit(occupancyMask, Notation::h7);
  BitUtil::set_bit(occupancyMask, Notation::b1);
  BitUtil::set_bit(occupancyMask, Notation::a6);
  BitUtil::set_bit(occupancyMask, Notation::f1);
  ans = 0ULL;
  pred = get_bishop_attack(Notation::d3, occupancyMask, chess.bishopMasks,
                           chess.bishopAttacks, chess.bishopMagics);
  moves = {Notation::a6, Notation::b5, Notation::c4, Notation::c2, Notation::b1,
           Notation::e2, Notation::f1, Notation::e4, Notation::f5,
           Notation::g6, Notation::h7};
  for (int move : moves)
    BitUtil::set_bit(ans, move);
  ASSERT_EQ(ans, pred);

  occupancyMask = 0ULL;
  BitUtil::set_bit(occupancyMask, Notation::g6);
  BitUtil::set_bit(occupancyMask, Notation::b1);
  BitUtil::set_bit(occupancyMask, Notation::a6);
  BitUtil::set_bit(occupancyMask, Notation::f1);
  ans = 0ULL;
  pred = get_bishop_attack(Notation::d3, occupancyMask, chess.bishopMasks,
                           chess.bishopAttacks, chess.bishopMagics);
  moves = {Notation::a6, Notation::b5, Notation::c4, Notation::c2, Notation::b1,
           Notation::e2, Notation::f1, Notation::e4, Notation::f5,
           Notation::g6};
  for (int move : moves)
    BitUtil::set_bit(ans, move);
  ASSERT_EQ(ans, pred);
}

TEST(PieceTest, RookTest) {
  Game chess(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);
  ull occupancyMask = 0ULL;
  BitUtil::set_bit(occupancyMask, Notation::a5);
  BitUtil::set_bit(occupancyMask, Notation::g7);
  BitUtil::set_bit(occupancyMask, Notation::g3);
  ull pred = get_rook_attack(Notation::g5, occupancyMask, chess.rookMasks,
                             chess.rookAttacks, chess.rookMagics);
  ull ans = 0ULL;
  std::vector<int> moves{Notation::a5, Notation::b5, Notation::c5, Notation::d5,
                         Notation::e5, Notation::f5, Notation::h5,
                         Notation::g6, Notation::g7, Notation::g4,
                         Notation::g3};
  for (int move : moves)
    BitUtil::set_bit(ans, move);
  ASSERT_EQ(ans, pred);

  occupancyMask = 0ULL;
  ans = 0ULL;
  pred = get_rook_attack(Notation::a1, occupancyMask, chess.rookMasks,
                         chess.rookAttacks, chess.rookMagics);
  moves = {Notation::a2, Notation::a3, Notation::a4, Notation::a5, Notation::a6,
           Notation::a7, Notation::a8, Notation::b1, Notation::c1, Notation::d1,
           Notation::e1, Notation::f1, Notation::g1, Notation::h1};
  for (int move : moves)
    BitUtil::set_bit(ans, move);
  ASSERT_EQ(ans, pred);

  occupancyMask = 0ULL;
  ans = 0ULL;
  BitUtil::set_bit(occupancyMask, Notation::d8);
  BitUtil::set_bit(occupancyMask, Notation::b4);
  BitUtil::set_bit(occupancyMask, Notation::f4);
  BitUtil::set_bit(occupancyMask, Notation::d2);
  pred = get_rook_attack(Notation::d4, occupancyMask, chess.rookMasks,
                         chess.rookAttacks, chess.rookMagics);
  moves = {Notation::b4, Notation::c4, Notation::d5, Notation::d6, Notation::d7,
           Notation::d8, Notation::e4, Notation::f4, Notation::d3,
           Notation::d2};
  for (int move : moves)
    BitUtil::set_bit(ans, move);
  ASSERT_EQ(ans, pred);
}
