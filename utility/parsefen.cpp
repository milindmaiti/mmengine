#include "../bitboard/bitboard.h"
#include <_ctype.h>
#include <cassert>
#include <iostream>
#include <string>
void parse_fen(Game &game, std::string &fen) {
  // zero out attributes of the game such as piece palcements, side to move,
  // etc.
  for (ull &bitboard : game.pieceBitboards)
    bitboard = 0;
  for (ull &bitboard : game.occupancyBitboards)
    bitboard = 0;
  int fenPtr = 0;
  game.side = white;
  game.enPassant = NO_SQ;
  game.castle = 0;
  game.halfMoves = 0;
  game.fullMoves = 0;

  for (int rank = 0; rank < BD; rank++) {
    for (int col = 0; col < BD; col++) {
      int square = rank * BD + col;
      if (fen[fenPtr] >= '0' && fen[fenPtr] <= '9') {
        col += (fen[fenPtr] - '0' - 1);
      } else if ((fen[fenPtr] >= 'a' && fen[fenPtr] <= 'z') ||
                 (fen[fenPtr] >= 'A' && fen[fenPtr] <= 'Z')) {
        int curPiece = charPieces[fen[fenPtr]];
        set_bit(game.pieceBitboards[curPiece], square);
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

  // if neither side can castle move by 2
  if (fen[fenPtr] == '-') {
    fenPtr += 2;
  } else {
    while (fen[fenPtr] != ' ') {
      int curPiece = charPieces[fen[fenPtr]];
      if (curPiece == K)
        game.castle |= wk;
      else if (curPiece == Q)
        game.castle |= wq;
      else if (curPiece == k)
        game.castle |= bk;
      else if (curPiece == q)
        game.castle |= bq;
    }
    // skip the space
    fenPtr++;
  }

  // if there is no enPassant square
  if (fen[fenPtr] == '-') {
    fenPtr += 2;
  } else {
    int rank = fen[fenPtr] - 'a';
    fenPtr++;
    int file = fen[fenPtr] - '1';
    fenPtr++;
    int square = rank * 8 + file;
    game.enPassant = square;
    fenPtr++;
  }

  // extract digits of halfMove clock
  while (isdigit(fen[fenPtr])) {
    game.halfMoves = game.halfMoves * 10 + fen[fenPtr] - '0';
  }

  fenPtr++;

  // extract fullMoves number (need to check for end of string as well)
  while (fenPtr < (int)fen.size() && isdigit(fen[fenPtr])) {
    game.fullMoves = game.fullMoves * 10 + fen[fenPtr] - '0';
  }

  // copy all white pieces to the full white bitboard and same for black
  for (int i = p; i <= k; i++)
    game.occupancyBitboards[white] |= game.pieceBitboards[i];
  for (int i = P; i <= K; i++)
    game.occupancyBitboards[black] |= game.pieceBitboards[i];

  // copy all white + black pieces to full bitboard
  game.occupancyBitboards[both] |=
      (game.occupancyBitboards[white] | game.occupancyBitboards[black]);
}
