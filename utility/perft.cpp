#include "../bitboard/bitboard.h"
#include "macros.h"

ull perftDriver(Game &game, ull &captures, ull &eps, ull &castles,
                ull &promotions, int depth) {
  // add 1 to node count
  if (depth == 0) {
    return 1;
  }
  ull totalMoves = 0;
  std::vector<int> moves = game.generate_moves();
  // Save the current board state to restore for each child move
  BoardState state = game.saveState();
  for (int move : moves) {
    if (!game.makeMove(move, false)) {
      // No need to restore state since makeMove does it for illegal moves
      continue;
    }
    if (depth == 1) {
      // count captures, enPassant moves, castling moves, and promotions on the
      // last edges of subtree
      captures += BitUtil::decode_capture(move);
      eps += BitUtil::decode_en_passant(move);
      castles += BitUtil::decode_castle(move);
      promotions += (BitUtil::decode_promotion(move) > 0);
    }
    // add to total move count
    totalMoves +=
        perftDriver(game, captures, eps, castles, promotions, depth - 1);
    game.restoreState(state);
  }
  return totalMoves;
}
