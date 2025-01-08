#pragma once
#include "bitboard.h"
using std::vector;

struct iterativeReturn {
  vector<int> evals;
  vector<int> nodeCounts;
  vector<vector<int>> pvs;
};

class Engine {
public:
  int mxDepth;
  int killerNum;
  int ply;
  int MXHISTORY;
  vector<vector<int>> killerMoves;
  vector<vector<int>> historyMoves;
  vector<vector<int>> pvTable;
  Engine(int mxdepth, int killernum = 2, int mxhist = 100);
  int evaluateMove(int move, Game &game);
  int negamax(Game &game, int depth, int alpha, int beta, int initialDepth,
              ull &nodes);
  iterativeReturn searchPosition(Game &game, int depth);
  int quiesenceSearch(Game &game, int alpha, int beta, ull &nodes);
};
