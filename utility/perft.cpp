#include "../bitboard/bitboard.h"
#include "macros.h"

#define copy_current_board()                                                   \
  std::array<ull, NUM_BITBOARDS> copyPieceBitboards = game.pieceBitboards;     \
  std::array<ull, NUM_OCCUPANCIES> copyOccupancyBitboards =                    \
      game.occupancyBitboards;                                                 \
  int copySide = game.side;                                                    \
  int copyEnPassant = game.enPassant;                                          \
  int copyCastle = game.castle;                                                \
  int copyHalfMoves = game.halfMoves;                                          \
  int copyFullMoves = game.fullMoves;

#define pop_current_copy()                                                     \
  game.pieceBitboards = copyPieceBitboards;                                    \
  game.occupancyBitboards = copyOccupancyBitboards;                            \
  game.side = copySide;                                                        \
  game.enPassant = copyEnPassant;                                              \
  game.castle = copyCastle;                                                    \
  game.halfMoves = copyHalfMoves;                                              \
  game.fullMoves = copyFullMoves;

ull perftDriver(Game &game, ull &captures, ull &eps, ull &castles,
                ull &promotions, int depth) {
  if (depth == 0) {
    return 1;
  }

  ull totalMoves = 0;
  std::vector<int> moves = game.generate_moves();
  copy_current_board();
  for (int move : moves) {

    if (!game.makeMove(move, false)) {

      pop_current_copy();
      continue;
    }
    if (depth == 1) {
      captures += decode_capture(move);
      eps += decode_en_passant(move);
      castles += decode_castle(move);
      promotions += (decode_promotion(move) > 0);
    }
    totalMoves +=
        perftDriver(game, captures, eps, castles, promotions, depth - 1);
    pop_current_copy();
  }
  return totalMoves;
}
