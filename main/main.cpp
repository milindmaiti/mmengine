#include "../bitboard/bitboard.h"
#include "../utility/genspecialboards.h"
#include "../utility/macros.h"
int main() {
  Game chess = Game(rookMagics, bishopMagics);
  for (int square = a7; square <= h7; square++)
    set_bit(chess.pieceBitboards[P], square);
  for (int square = a2; square <= h2; square++)
    set_bit(chess.pieceBitboards[p], square);

  set_bit(chess.pieceBitboards[Q], d8);
  set_bit(chess.pieceBitboards[K], e8);
  set_bit(chess.pieceBitboards[B], f8);
  set_bit(chess.pieceBitboards[B], c8);
  set_bit(chess.pieceBitboards[N], b8);
  set_bit(chess.pieceBitboards[N], g8);
  set_bit(chess.pieceBitboards[R], a8);
  set_bit(chess.pieceBitboards[R], h8);

  set_bit(chess.pieceBitboards[q], d1);
  set_bit(chess.pieceBitboards[k], e1);
  set_bit(chess.pieceBitboards[b], f1);
  set_bit(chess.pieceBitboards[b], c1);
  set_bit(chess.pieceBitboards[n], b1);
  set_bit(chess.pieceBitboards[n], g1);
  set_bit(chess.pieceBitboards[r], a1);
  set_bit(chess.pieceBitboards[r], h1);
  print_bitboard(chess.pieceBitboards[P]);
  print_bitboard(chess.pieceBitboards[p]);
  printBoard(chess);
}
