#include "bitboard/bitboard.h"
#include "utility/boardnotation.h"
#include "utility/macros.h"
#include "utility/parsefen.h"
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

TEST(FenTest, StartPosition) {
  ull fullOccupancy = 0ULL;
  ull curWhite = 0ULL, curBlack = 0ULL;
  Game game(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);
  // set starting position piece occupancies
  for (int i = Notation::a8; i <= Notation::h7; i++) {
    BitUtil::set_bit(fullOccupancy, i);
    BitUtil::set_bit(curBlack, i);
  }
  for (int i = Notation::a2; i <= Notation::h1; i++) {
    BitUtil::set_bit(fullOccupancy, i);
    BitUtil::set_bit(curWhite, i);
  }

  parse_fen(game, ArrayUtil::startPosition);

  EXPECT_EQ(game.occupancyBitboards[Notation::both], fullOccupancy);
  EXPECT_EQ(game.occupancyBitboards[Notation::white], curWhite);
  EXPECT_EQ(game.occupancyBitboards[Notation::black], curBlack);
  EXPECT_EQ(game.side, Notation::white);
  EXPECT_EQ(game.enPassant, Notation::NO_SQ);
  EXPECT_EQ(game.castle, 15);
  EXPECT_EQ(game.halfMoves, 0);
  EXPECT_EQ(game.fullMoves, 1);
}

TEST(FenTest, EmptyPosition) {
  ull fullOccupancy = 0ULL;
  ull curWhite = 0ULL, curBlack = 0ULL;
  int castle = 0;
  Game game(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);

  parse_fen(game, ArrayUtil::emptyPosition);

  EXPECT_EQ(game.occupancyBitboards[Notation::both], fullOccupancy);
  EXPECT_EQ(game.occupancyBitboards[Notation::white], curWhite);
  EXPECT_EQ(game.occupancyBitboards[Notation::black], curBlack);
  EXPECT_EQ(game.side, Notation::white);
  EXPECT_EQ(game.enPassant, Notation::NO_SQ);
  EXPECT_EQ(game.castle, castle);
  EXPECT_EQ(game.halfMoves, 25);
  EXPECT_EQ(game.fullMoves, 60);
}

TEST(FenTest, TrickyPosition) {
  ull fullOccupancy = 0ULL;
  ull curWhite = 0ULL, curBlack = 0ULL;
  ull blackK = 0, blackQ = 0, blackR = 0, blackB = 0, blackN = 0, blackP = 0;
  ull whiteK = 0, whiteQ = 0, whiteR = 0, whiteB = 0, whiteN = 0, whiteP = 0;

  BitUtil::set_bit(blackK, Notation::e8);
  BitUtil::set_bit(blackQ, Notation::e7);
  BitUtil::set_bit(blackR, Notation::a8);
  BitUtil::set_bit(blackR, Notation::h8);
  BitUtil::set_bit(blackB, Notation::g7);
  BitUtil::set_bit(blackB, Notation::a6);
  BitUtil::set_bit(blackN, Notation::b6);
  BitUtil::set_bit(blackN, Notation::f6);
  std::vector<int> bpawns{Notation::a7, Notation::b4, Notation::c7, Notation::d7,
                          Notation::e6, Notation::f7, Notation::g6,
                          Notation::h3};
  for (int pawn : bpawns)
    BitUtil::set_bit(blackP, pawn);

  BitUtil::set_bit(whiteK, Notation::e1);
  BitUtil::set_bit(whiteQ, Notation::f3);
  BitUtil::set_bit(whiteR, Notation::a1);
  BitUtil::set_bit(whiteR, Notation::h1);
  BitUtil::set_bit(whiteB, Notation::e2);
  BitUtil::set_bit(whiteB, Notation::d2);
  BitUtil::set_bit(whiteN, Notation::e5);
  BitUtil::set_bit(whiteN, Notation::c3);
  std::vector<int> wpawns{Notation::a2, Notation::b2, Notation::c2, Notation::d5,
                          Notation::e4, Notation::f2, Notation::g2,
                          Notation::h2};
  for (int pawn : wpawns)
    BitUtil::set_bit(whiteP, pawn);
  curWhite = (whiteK | whiteQ | whiteB | whiteN | whiteR | whiteP);
  curBlack = (blackK | blackQ | blackB | blackN | blackR | blackP);
  fullOccupancy = (curWhite | curBlack);
  int curCastle = 15;
  Game game(ArrayUtil::rookMagics, ArrayUtil::bishopMagics);

  parse_fen(game, ArrayUtil::trickyPosition);
  EXPECT_EQ(game.occupancyBitboards[Notation::both], fullOccupancy);
  EXPECT_EQ(game.occupancyBitboards[Notation::white], curWhite);
  EXPECT_EQ(game.occupancyBitboards[Notation::black], curBlack);
  EXPECT_EQ(game.side, Notation::white);
  EXPECT_EQ(game.enPassant, Notation::b5);
  EXPECT_EQ(game.castle, curCastle);
  EXPECT_EQ(game.halfMoves, 0);
  EXPECT_EQ(game.fullMoves, 1);
}
