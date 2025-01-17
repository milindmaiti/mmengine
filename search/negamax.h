#pragma once
#include "bitboard.h"
#include <memory>
using std::vector;

struct iterativeReturn {
  vector<int> evals;
  vector<int> nodeCounts;
  vector<vector<int>> pvs;
};

class Engine {
public:
  // max depth of any search
  int mxDepth;
  // number of killer moves to store (killer moves cause beta cutoffs in sibling
  // nodes)
  int killerNum;
  // current depth in game tree (inverse of depth variable in searchPosition)
  int ply;
  // number of
  int MXHISTORY;
  int FullDepthMoves;
  int reductionLimit;
  int windowSize;
  bool followPv;
  bool nullVar;
  std::shared_ptr<std::atomic<bool>> stopFlag;
  vector<vector<int>> killerMoves;
  vector<vector<int>> historyMoves;
  vector<vector<int>> pvTable;
  Engine(int mxdepth = 128, int killernum = 2, int mxhist = 100,
         int fullmoves = 4, int redlimit = 3, int windowsz = 1000);
  int evaluateMove(int move, Game &game);
  int negamax(Game &game, int depth, int alpha, int beta, int initialDepth,
              ull &nodes);
  void searchPosition(Game &game, int depth, iterativeReturn &ret);
  int quiesenceSearch(Game &game, int alpha, int beta, ull &nodes);
};
