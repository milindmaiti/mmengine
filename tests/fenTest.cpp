#include "../bitboard/bitboard.h"
#include "../utility/boardnotation.h"
#include "../utility/macros.h"
#include "../utility/parsefen.h"
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

TEST(FenTest, StartPosition) {
  ull fullOccupancy = 0ULL;
  ull curWhite = 0ULL, curBlack = 0ULL;

  Game game;
  // set starting position piece occupancies
  for (int i = a8; i <= h7; i++) {
    set_bit(fullOccupancy, i);
    set_bit(curBlack, i);
  }
  for (int i = a2; i <= h1; i++) {
    set_bit(fullOccupancy, i);
    set_bit(curWhite, i);
  }

  parse_fen(game, startPosition);

  EXPECT_EQ(game.occupancyBitboards[both], fullOccupancy);
  EXPECT_EQ(game.occupancyBitboards[white], curWhite);
  EXPECT_EQ(game.occupancyBitboards[black], curBlack);
  EXPECT_EQ(game.side, white);
  EXPECT_EQ(game.enPassant, NO_SQ);
  EXPECT_EQ(game.castle, 15);
  EXPECT_EQ(game.halfMoves, 0);
  EXPECT_EQ(game.fullMoves, 1);
}

TEST(FenTest, EmptyPosition) {
  ull fullOccupancy = 0ULL;
  ull curWhite = 0ULL, curBlack = 0ULL;
  int castle = 0;
  Game game;

  parse_fen(game, emptyPosition);

  EXPECT_EQ(game.occupancyBitboards[both], fullOccupancy);
  EXPECT_EQ(game.occupancyBitboards[white], curWhite);
  EXPECT_EQ(game.occupancyBitboards[black], curBlack);
  EXPECT_EQ(game.side, white);
  EXPECT_EQ(game.enPassant, NO_SQ);
  EXPECT_EQ(game.castle, castle);
  EXPECT_EQ(game.halfMoves, 25);
  EXPECT_EQ(game.fullMoves, 60);
}

TEST(FenTest, TrickyPosition) {
  ull fullOccupancy = 0ULL;
  ull curWhite = 0ULL, curBlack = 0ULL;
  ull blackK = 0, blackQ = 0, blackR = 0, blackB = 0, blackN = 0, blackP = 0;
  ull whiteK = 0, whiteQ = 0, whiteR = 0, whiteB = 0, whiteN = 0, whiteP = 0;

  set_bit(blackK, e8);
  set_bit(blackQ, e7);
  set_bit(blackR, a8);
  set_bit(blackR, h8);
  set_bit(blackB, g7);
  set_bit(blackB, a6);
  set_bit(blackN, b6);
  set_bit(blackN, f6);
  std::vector<int> bpawns{a7, b4, c7, d7, e6, f7, g6, h3};
  for (int pawn : bpawns)
    set_bit(blackP, pawn);

  set_bit(whiteK, e1);
  set_bit(whiteQ, f3);
  set_bit(whiteR, a1);
  set_bit(whiteR, h1);
  set_bit(whiteB, e2);
  set_bit(whiteB, d2);
  set_bit(whiteN, e5);
  set_bit(whiteN, c3);
  std::vector<int> wpawns{a2, b2, c2, d5, e4, f2, g2, h2};
  for (int pawn : wpawns)
    set_bit(whiteP, pawn);
  curWhite = (whiteK | whiteQ | whiteB | whiteN | whiteR | whiteP);
  curBlack = (blackK | blackQ | blackB | blackN | blackR | blackP);
  fullOccupancy = (curWhite | curBlack);
  int curCastle = 15;
  Game game;

  parse_fen(game, trickyPosition);
  EXPECT_EQ(game.occupancyBitboards[both], fullOccupancy);
  EXPECT_EQ(game.occupancyBitboards[white], curWhite);
  EXPECT_EQ(game.occupancyBitboards[black], curBlack);
  EXPECT_EQ(game.side, white);
  EXPECT_EQ(game.enPassant, b5);
  EXPECT_EQ(game.castle, curCastle);
  EXPECT_EQ(game.halfMoves, 0);
  EXPECT_EQ(game.fullMoves, 1);
}
