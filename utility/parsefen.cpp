#include "../bitboard/bitboard.h"
#include <cassert>
#include <string>
void parse_fen(Game &game, const std::string &fen) {
  // zero out attributes of the game such as piece placements, side to move,
  // etc.
  for (ull &bitboard : game.pieceBitboards)
    bitboard = 0;
  for (ull &bitboard : game.occupancyBitboards)
    bitboard = 0;
  int fenPtr = 0;
  game.side = Notation::white;
  game.enPassant = Notation::NO_SQ;
  game.castle = 0;
  game.halfMoves = 0;
  game.fullMoves = 0;

  for (int rank = 0; rank < (int)Constants::BD; rank++) {
    for (int col = 0; col < (int)Constants::BD; col++) {
      int square = rank * (int)Constants::BD + col;
      if (fen[fenPtr] >= '0' && fen[fenPtr] <= '9') {
        col += (fen[fenPtr] - '0' - 1);
      } else if ((fen[fenPtr] >= 'a' && fen[fenPtr] <= 'z') ||
                 (fen[fenPtr] >= 'A' && fen[fenPtr] <= 'Z')) {
        int curPiece =
            ArrayUtil::charPieces[static_cast<unsigned char>(fen[fenPtr])];
        BitUtil::set_bit(game.pieceBitboards[curPiece], square);
      } else if (fen[fenPtr] == '/') {
        col--;
      }

      fenPtr++;
    }
  }
  assert(fen[fenPtr] == ' ');
  fenPtr++;

  // BLACk = 1, WHITE = 0
  game.side = fen[fenPtr] == 'b';
  // next char is guaranteed to be a space so skip
  fenPtr += 2;

  // if neither side can castle move pointer by 2
  if (fen[fenPtr] == '-') {
    fenPtr += 2;
  } else {
    while (fen[fenPtr] != ' ') {
      int curPiece =
          ArrayUtil::charPieces[static_cast<unsigned char>(fen[fenPtr])];
      if (curPiece == Notation::K)
        game.castle |= Notation::wk;
      else if (curPiece == Notation::Q)
        game.castle |= Notation::wq;
      else if (curPiece == Notation::k)
        game.castle |= Notation::bk;
      else if (curPiece == Notation::q)
        game.castle |= Notation::bq;
      fenPtr++;
    }
    // skip the space
    fenPtr++;
  }

  // if there is no enPassant square
  if (fen[fenPtr] == '-') {
    fenPtr += 2;
  } else {
    int file = fen[fenPtr] - 'a';
    fenPtr++;
    int rank = 7 - (fen[fenPtr] - '1');
    fenPtr++;
    int square = rank * 8 + file;
    game.enPassant = square;
    fenPtr++;
  }

  // extract digits of halfMove clock
  while (isdigit(fen[fenPtr])) {
    game.halfMoves = game.halfMoves * 10 + fen[fenPtr] - '0';
    fenPtr++;
  }

  fenPtr++;

  // extract fullMoves number (need to check for end of string as well)
  while (fenPtr < (int)fen.size() && isdigit(fen[fenPtr])) {
    game.fullMoves = game.fullMoves * 10 + fen[fenPtr] - '0';
    fenPtr++;
  }

  // copy all white pieces to the full white bitboard and same for black
  for (int i = Notation::P; i <= Notation::K; i++)
    game.occupancyBitboards[Notation::white] |= game.pieceBitboards[i];
  for (int i = Notation::p; i <= Notation::k; i++)
    game.occupancyBitboards[Notation::black] |= game.pieceBitboards[i];

  // copy all white + black pieces to full bitboard
  game.occupancyBitboards[Notation::both] |=
      (game.occupancyBitboards[Notation::white] |
       game.occupancyBitboards[Notation::black]);
}
