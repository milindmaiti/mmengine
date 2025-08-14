#include "../utility/perft.h"
#include "../bitboard/bitboard.h"
#include "../utility/macros.h"
#include "parsefen.h"
#include <gtest/gtest.h>
#include <vector>

struct positionStat {
  ull nodes, captures, enPassants, castles, promotions;
};
TEST(perftTest, startingPosition) {
  Game game(rookMagics, bishopMagics);
  parse_fen(game, startPosition);
  std::vector<positionStat> startPosition = {{20, 0, 0, 0, 0},
                                             {400, 0, 0, 0, 0},
                                             {8902, 34, 0, 0, 0},
                                             {197281, 1576, 0, 0, 0},
                                             {4865609, 82719, 258, 0, 0},
                                             {119060324, 2812008, 5248, 0, 0}};
  ull nodes = 0, captures = 0, eps = 0, castles = 0, promotions = 0;
  for (ull i = 0; i < startPosition.size(); i++) {
    nodes = 0, captures = 0, eps = 0, castles = 0, promotions = 0;
    nodes = perftDriver(game, captures, eps, castles, promotions, i + 1);
    EXPECT_EQ(nodes, startPosition[i].nodes);
    EXPECT_EQ(captures + eps, startPosition[i].captures);
    EXPECT_EQ(eps, startPosition[i].enPassants);
    EXPECT_EQ(castles, startPosition[i].castles);
    EXPECT_EQ(promotions, startPosition[i].promotions);
  }
}

TEST(perftTest, trickyPosition) {
  Game game(rookMagics, bishopMagics);
  parse_fen(game, trickyPosition);
  std::vector<positionStat> trickyPosition = {
      {48, 8, 0, 2, 0},
      {2039, 351, 1, 91, 0},
      {97862, 17102, 45, 3162, 0},
      {4085603, 757163, 1929, 128013, 15172},
      {193690690, 35043416, 73365, 4993637, 8392},
  };

  ull nodes = 0, captures = 0, eps = 0, castles = 0, promotions = 0;
  for (ull i = 0; i < trickyPosition.size(); i++) {
    nodes = 0, captures = 0, eps = 0, castles = 0, promotions = 0;
    nodes = perftDriver(game, captures, eps, castles, promotions, i + 1);
    EXPECT_EQ(nodes, trickyPosition[i].nodes);
    EXPECT_EQ(captures + eps, trickyPosition[i].captures);
    EXPECT_EQ(eps, trickyPosition[i].enPassants);
    EXPECT_EQ(castles, trickyPosition[i].castles);
    EXPECT_EQ(promotions, trickyPosition[i].promotions);
  }
}

TEST(perftTest, endgamePosition) {
  Game game(rookMagics, bishopMagics);
  std::string endPosition = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
  parse_fen(game, endPosition);
  std::vector<positionStat> endgamePosition = {
      {14, 1, 0, 0, 0},
      {191, 14, 0, 0, 0},
      {2812, 209, 2, 0, 0},
      {43238, 3348, 123, 0, 0},
      {674624, 52051, 1165, 0, 0},
      {11030083, 940350, 33325, 0, 7552}};

  ull nodes = 0, captures = 0, eps = 0, castles = 0, promotions = 0;
  for (ull i = 0; i < endgamePosition.size(); i++) {
    nodes = 0, captures = 0, eps = 0, castles = 0, promotions = 0;
    nodes = perftDriver(game, captures, eps, castles, promotions, i + 1);
    EXPECT_EQ(nodes, endgamePosition[i].nodes);
    EXPECT_EQ(captures + eps, endgamePosition[i].captures);
    EXPECT_EQ(eps, endgamePosition[i].enPassants);
    EXPECT_EQ(castles, endgamePosition[i].castles);
    EXPECT_EQ(promotions, endgamePosition[i].promotions);
  }
}

TEST(perftTest, TalkChessPosition) {
  Game game(rookMagics, bishopMagics);
  parse_fen(game, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  std::vector<ull> talkChessNodes = {44, 1486, 62379, 2103487, 89941194};
  ull captures = 0, eps = 0, castles = 0, promotions = 0;
  for (ull i = 0; i < talkChessNodes.size(); i++) {
    captures = 0, eps = 0, castles = 0, promotions = 0;
    ASSERT_EQ(perftDriver(game, captures, eps, castles, promotions, i + 1),
              talkChessNodes[i]);
  }
}

TEST(perftTest, edwardsPosition) {
  Game game(rookMagics, bishopMagics);
  std::string edwardsPosition = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/"
                                "P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
  parse_fen(game, edwardsPosition);
  std::vector<ull> edwardsNodes = {46, 2079, 89890, 3894594, 164075551};
  ull captures = 0, eps = 0, castles = 0, promotions = 0;
  for (ull i = 0; i < edwardsNodes.size(); i++) {
    captures = 0, eps = 0, castles = 0, promotions = 0;
    ASSERT_EQ(perftDriver(game, captures, eps, castles, promotions, i + 1),
              edwardsNodes[i]);
  }
}

TEST(perftTest, checkPosition) {

  Game game(rookMagics, bishopMagics);

  std::string checkPosition =
      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
  parse_fen(game, checkPosition);
  std::vector<ull> checkNodes = {6, 264, 9467, 422333, 15833292};
  ull captures = 0, eps = 0, castles = 0, promotions = 0;
  for (ull i = 0; i < checkNodes.size(); i++) {
    captures = 0, eps = 0, castles = 0, promotions = 0;
    ASSERT_EQ(perftDriver(game, captures, eps, castles, promotions, i + 1),
              checkNodes[i]);
  }
}
