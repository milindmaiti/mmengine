#include "negamax.h"
#include "../bitboard/bitboard.h"
#include "../utility/boardnotation.h"
#include "../utility/evaluationtables.h"
#include "../utility/macros.h"
#include "evaluation.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdio>
#include <ios>
#include <numeric>

using std::vector;
static int INF = 1e5;
const int startMx = 1e9;

Engine::Engine(int mxdepth, int killernum, int mxhist, int fullmoves,
               int redlimit)
    : mxDepth(mxdepth), killerNum(killernum), ply(0), MXHISTORY(mxhist),
      FullDepthMoves(fullmoves), reductionLimit(redlimit), followPv(false),
      killerMoves(this->killerNum, vector<int>(this->mxDepth)),
      historyMoves(NUM_PIECES, vector<int>(NUM_SQ)),
      pvTable(this->mxDepth, vector<int>(this->mxDepth)) {}
int Engine::evaluateMove(int move, Game &game) {
  // give largest bonus to best variation found in previous search
  if (this->followPv && move == this->pvTable[0][ply]) {
    return 20000;
  }
  if (decode_capture(move)) {

    int lowerPiece, higherPiece;
    int dst = decode_dst(move);

    int cap = -1;
    if (game.side == white) {
      lowerPiece = p, higherPiece = q;
    } else {
      lowerPiece = P, higherPiece = Q;
    }
    for (int capturedPiece = lowerPiece; capturedPiece <= higherPiece;
         capturedPiece++) {
      if (get_bit(game.pieceBitboards[capturedPiece], dst)) {
        cap = capturedPiece;
      }
    }
    // extra 10k to put captures above killer moves
    return mvvtable[decode_piece(move)][cap] + 10000;
  } else {
    if (ply < mxDepth && this->killerMoves[0][ply] == move)
      return 9000;
    else if (ply < mxDepth && this->killerMoves[0][ply] == move)
      return 8000;
    else
      return historyMoves[decode_piece(move)][decode_dst(move)];
    return 0;
  }
}
int getCapturedPiece(int move, Game &game) {
  int lowerPiece, higherPiece;
  int dst = decode_dst(move);

  if (game.side == white) {
    lowerPiece = p, higherPiece = q;
  } else {
    lowerPiece = P, higherPiece = Q;
  }
  for (int capturedPiece = lowerPiece; capturedPiece <= higherPiece;
       capturedPiece++) {
    if (get_bit(game.pieceBitboards[capturedPiece], dst)) {
      return capturedPiece;
    }
  }
  return -1;
};
int Engine::quiesenceSearch(Game &game, int alpha, int beta, ull &nodes) {
  nodes++;
  int eval = evaluatePosition(game);
  alpha = std::max(alpha, eval);
  if (alpha >= beta)
    return alpha;
  // TODO: make separate capture move generator to cut down constant time
  auto moveList = game.generate_moves();

  sort(moveList.begin(), moveList.end(), [&](int move1, int move2) {
    return evaluateMove(move1, game) > evaluateMove(move2, game);
  });

  copy_current_board();
  for (int move : moveList) {
    if (!game.makeMove(move, true))
      continue;

    ply++;
    int evaluation = -quiesenceSearch(game, -beta, -alpha, nodes);
    ply--;
    alpha = std::max(alpha, evaluation);
    if (alpha >= beta)
      return alpha;
    pop_current_copy();
  }
  return alpha;
}

int Engine::negamax(Game &game, int depth, int alpha, int beta,
                    int initialDepth, ull &nodes) {

  // reached the leaf nodes so activate quiesenceSearch
  // evaluatePosition returns evaluation relative to color
  if (depth == 0) {
    return quiesenceSearch(game, alpha, beta, nodes);
  }
  nodes++;
  int inCheck = game.is_square_attacked(
      LSOneIndex(game.pieceBitboards[(game.side == white) ? K : k]),
      !game.side);
  auto moveList = game.generate_moves();

  // Pre-calculating scores for every move seems to be a lot slower
  /*vector<int> scores(moveList.size());*/
  /*for (int i = 0; i < (int)scores.size(); i++)*/
  /*  scores[i] = evaluateMove(moveList[i], game);*/
  /*vector<int> indices(moveList.size());*/
  /*std::iota(indices.begin(), indices.end(), 0);*/
  /*sort(indices.begin(), indices.end(),*/
  /*     [&](int i1, int i2) { return scores[i1] > scores[i2]; });*/
  /**/
  /*for (int i = 0; i < (int)indices.size(); i++) {*/
  /*  // While current element hasn't been moved to its final position*/
  /*  while (indices[i] != i) {*/
  /*    // Index where current element should go*/
  /*    size_t target_pos = indices[i];*/
  /**/
  /*    // Swap elements*/
  /*    std::swap(moveList[i], moveList[target_pos]);*/
  /**/
  /*    // Update indices to reflect the swap*/
  /*    std::swap(indices[i], indices[target_pos]);*/
  /*  }*/
  /*}*/

  sort(moveList.begin(), moveList.end(), [&](int move1, int move2) {
    return evaluateMove(move1, game) > evaluateMove(move2, game);
  });

  // if king is in check increase depth since it is very possible to get mated
  // if depth isn't increased
  if (inCheck && ply < mxDepth - depth)
    depth++;
  copy_current_board();
  int numMoves = 0;
  // copy the follow pv flag because we need to reset it after each move is
  // tried
  bool copyFollow = this->followPv;
  bool foundPv = false;
  for (int move : moveList) {
    if (move != this->pvTable[0][ply]) {
      this->followPv = false;
    }
    if (!game.makeMove(move, false)) {
      continue;
    }
    ply++;
    int evaluation;
    // if we have found our principal variation move, do a null window search on
    // the other moves
    if (foundPv) {
      // do a cheap search to see if there is possibly a better node (relies on
      // good move ordering)

      // if the move is not a check, capture, promotion, or castle, and we still
      // have a lot more depth to recurse and we've already considered the top 4
      // moves, then do a smaller search with one less depth to test if a move
      // could be good or not
      if (inCheck == false && decode_capture(move) == 0 &&
          decode_promotion(move) == 0 && decode_castle(move) == 0 &&
          depth >= this->reductionLimit && numMoves >= this->FullDepthMoves) {

        evaluation =
            -negamax(game, depth - 2, -alpha - 1, -alpha, initialDepth, nodes);
      } else
        // a little hack to make sure the next if is executed
        evaluation = alpha + 1;
      if (evaluation > alpha) {
        // do a full depth search still with a zero window to test if a move is
        // better than alpha
        evaluation =
            -negamax(game, depth - 1, -alpha - 1, -alpha, initialDepth, nodes);
        // if the move could be better than pv move then do a full search
        if (evaluation > alpha && evaluation < beta)
          evaluation =
              -negamax(game, depth - 1, -beta, -alpha, initialDepth, nodes);
      }
    } else
      evaluation =
          -negamax(game, depth - 1, -beta, -alpha, initialDepth, nodes);
    ply--;
    // fail-soft alpha-beta (returned value can be outside alpha beta cutoffs)
    if (evaluation > alpha) {
      alpha = evaluation;
      // update best move
      foundPv = true;
      if (ply < (int)this->pvTable.size()) {
        this->pvTable[ply][ply] = move;
        for (int nextDepth = ply + 1;
             nextDepth <= std::min(initialDepth, (int)this->pvTable.size());
             nextDepth++) {

          this->pvTable[ply][nextDepth] = this->pvTable[ply + 1][nextDepth];
        }
      }

      // update history values by adding a bonus proportional to depth (want to
      // reward moves higher up in tree since there are less of them)
      if (decode_capture(move) == 0) {

        int clampedBonus = std::clamp(depth * depth, -MXHISTORY, MXHISTORY);
        this->historyMoves[decode_piece(move)][decode_dst(move)] +=
            clampedBonus -
            this->historyMoves[decode_piece(move)][decode_dst(move)] *
                abs(clampedBonus) / MXHISTORY;
      }
    }
    numMoves++;
    if (alpha >= beta) {
      // update killer moves so that these moves will be checked first in
      // sibling nodes

      if (decode_capture(move) == 0) {
        if (ply < mxDepth) {
          this->killerMoves[1][ply] = this->killerMoves[0][ply];
          this->killerMoves[1][ply] = move;
        }
      }
      // huge bug fix, make sure to pop in all control paths
      pop_current_copy();
      return alpha;
    }
    this->followPv = copyFollow;
    pop_current_copy();
  }
  if (numMoves == 0) {
    // current side is checkmated (provide bonus for shorter checkmates)
    // depth variable decreases with each recursive call
    if (inCheck) {
      return -INF + ply;
    }
    // stalemate
    else
      return 0;
  }
  return alpha;
}

iterativeReturn Engine::searchPosition(Game &game, int depth) {
  for (int i = 0; i < (int)this->pvTable.size(); i++) {
    std::fill(this->pvTable[i].begin(), this->pvTable[i].end(), 0);
  }
  for (int i = 0; i < (int)this->historyMoves.size(); i++) {
    std::fill(this->historyMoves[i].begin(), this->historyMoves[i].end(), 0);
  }

  iterativeReturn ret;
  for (int curDepth = 1; curDepth <= depth; curDepth++) {
    followPv = true;
    ull nodes = 0;
    int eval = negamax(game, curDepth, -startMx, startMx, curDepth, nodes);
    ret.nodeCounts.push_back(nodes);
    ret.evals.push_back(eval);
    ret.pvs.push_back(this->pvTable[0]);
  }
  return ret;
}
