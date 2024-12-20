#include "../bitboard/bitboard.h"
#include "../utility/macros.h"
#include "evaluationtables.h"

// should probably inline this later
ull evaluatePosition(Game &game) {
  ull evaluation = 0ULL;
  for (int piece = P; piece <= k; piece++) {
    ull currentBitboard = game.pieceBitboards[piece];
    while (currentBitboard) {
      int curSquare = LSOneIndex(currentBitboard);
      evaluation += materialScores[piece];

      switch (piece) {
      case P:
        evaluation += pawnScores[curSquare];
        break;
      case N:
        evaluation += knightScores[curSquare];
        break;
      case B:
        evaluation += bishopScores[curSquare];
        break;
      case R:
        evaluation += rookScores[curSquare];
        break;
      case K:
        evaluation += kingScores[curSquare];
        break;
      case p:
        evaluation -= pawnScores[squareMirror[curSquare]];
        break;
      case n:
        evaluation -= knightScores[squareMirror[curSquare]];
        break;
      case b:
        evaluation -= bishopScores[squareMirror[curSquare]];
        break;
      case r:
        evaluation -= rookScores[squareMirror[curSquare]];
        break;
      case k:
        evaluation -= kingScores[squareMirror[curSquare]];
        break;
      }
      pop_bit(currentBitboard, curSquare);
    }
  }
  return (game.side == white ? 1 : -1) * evaluation;
}
