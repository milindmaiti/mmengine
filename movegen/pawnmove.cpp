#include "../utility/macros.h"
// Creates masks for the squares that the pawns attack
ull mask_pawn_attacks(int square, int side) {
  ull bitboard = 0ULL;
  ull attacks = bitboard;
  set_bit(bitboard, square);

  if (side == white) {
    ull right_attacks = (bitboard >> 7) & NO_A_FILE;
    ull left_attacks = (bitboard >> 9) & NO_H_FILE;
    attacks |= right_attacks;
    attacks |= left_attacks;
  } else {
    ull right_attacks = (bitboard << 7) & NO_H_FILE;
    ull left_attacks = (bitboard << 9) & NO_A_FILE;
    attacks |= right_attacks;
    attacks |= left_attacks;
  }
  return attacks;
}
