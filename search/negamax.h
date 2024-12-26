#pragma once
#include "bitboard.h"
using std::vector;
class Engine {
public:
  int mxDepth;
  int killerNum;
  int ply;
  int MXHISTORY;
  vector<vector<int>> killerMoves;
  vector<vector<int>> historyMoves;
  Engine(int mxdepth, int killernum = 2, int mxhist = 100);
  int evaluateMove(int move, Game &game);
  int negamax(Game &game, int depth, int alpha, int beta, int initialDepth,
              int &bstMove, ull &nodes);
  int searchPosition(Game &game, int depth, int &bstMove, ull &nodes);
  int quiesenceSearch(Game &game, int alpha, int beta, ull &nodes);
};
