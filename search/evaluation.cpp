#include "bitboard/bitboard.h"
#include "utility/evaluationtables.h"
#include "utility/macros.h"

// should probably inline this later
ll evaluatePosition(Game &game) {
  ll evaluation = 0;
  for (int piece = Notation::P; piece <= Notation::k; piece++) {
    ull currentBitboard = game.pieceBitboards[piece];
    while (currentBitboard) {
      int curSquare = BitUtil::LSOneIndex(currentBitboard);
      evaluation += ArrayUtil::materialScores[piece];

      switch (piece) {
      case Notation::P:
        evaluation += pawnScores[curSquare];
        break;
      case Notation::N:
        evaluation += knightScores[curSquare];
        break;
      case Notation::B:
        evaluation += bishopScores[curSquare];
        break;
      case Notation::R:
        evaluation += rookScores[curSquare];
        break;
      case Notation::K:
        evaluation += kingScores[curSquare];
        break;
      case Notation::p:
        evaluation -= pawnScores[ArrayUtil::squareMirror[curSquare]];
        break;
      case Notation::n:
        evaluation -= knightScores[ArrayUtil::squareMirror[curSquare]];
        break;
      case Notation::b:
        evaluation -= bishopScores[ArrayUtil::squareMirror[curSquare]];
        break;
      case Notation::r:
        evaluation -= rookScores[ArrayUtil::squareMirror[curSquare]];
        break;
      case Notation::k:
        evaluation -= kingScores[ArrayUtil::squareMirror[curSquare]];
        break;
      }
      BitUtil::pop_bit(currentBitboard, curSquare);
    }
  }
  return (game.side == Notation::white ? 1 : -1) * evaluation;
}
