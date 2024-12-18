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
  // save the current board state to restore after exploring each subtree
  copy_current_board();
  for (int move : moves) {

    if (!game.makeMove(move, false)) {
      // no need to restore board state since makeMove does that for illegal
      // moves
      continue;
    }
    if (depth == 1) {
      // count captures, enPassant moves, castling moves, and promotions on the
      // last edges of subtree
      captures += decode_capture(move);
      eps += decode_en_passant(move);
      castles += decode_castle(move);
      promotions += (decode_promotion(move) > 0);
    }
    // add to total move count
    totalMoves +=
        perftDriver(game, captures, eps, castles, promotions, depth - 1);
    // restore original board for the next move
    pop_current_copy();
  }
  return totalMoves;
}
